//
// System.ServiceModel.MessageHeader.cs
//
// Author: Duncan Mak (duncan@novell.com)
//
// Copyright (C) 2005 Novell, Inc (http://www.novell.com)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Xml;

namespace System.ServiceModel.Channels
{
	public sealed class MessageHeaders : IEnumerable<MessageHeaderInfo>, IEnumerable
	{
		static string [] empty_strings = new string [0];

		static readonly XmlReaderSettings reader_settings;

		static MessageHeaders ()
		{
			reader_settings = new XmlReaderSettings ();
			reader_settings.ConformanceLevel = ConformanceLevel.Fragment;
		}

		List<MessageHeaderInfo> l;
		Dictionary<Type, XmlObjectSerializer> serializers =
			new Dictionary<Type, XmlObjectSerializer> ();
		MessageVersion version;

		public MessageHeaders (MessageHeaders headers)
			: this (headers.MessageVersion)
		{
			CopyHeadersFrom (headers);
		}

		public MessageHeaders (MessageVersion version)
			: this (version, 10) // let's say 10 is the initial size
		{
		}

		public MessageHeaders (MessageVersion version, int capacity)
		{
			this.version = version;
			l = new List<MessageHeaderInfo> (capacity);
		}
		
		public void Add (MessageHeader header)
		{
			l.Add (header);
		}

		public void CopyHeaderFrom (Message m, int index)
		{
			CopyHeaderFrom (m.Headers, index);
		}

		public void Clear ()
		{
			l.Clear ();
		}

		public void CopyHeaderFrom (MessageHeaders headers, int index)
		{
			l.Add (headers [index]);
		}

		public void CopyHeadersFrom (Message m)
		{
			CopyHeadersFrom (m.Headers);
		}

		public void CopyHeadersFrom (MessageHeaders headers)
		{
			foreach (MessageHeaderInfo h in headers)
				l.Add (h);
		}

		public void CopyTo (MessageHeaderInfo [] dst, int index)
		{
			l.CopyTo (dst, index);
		}

		public int FindHeader (string name, string ns)
		{
			return FindHeader (name, ns, null);
		}

		bool HasActor (string actor, string [] candidates)
		{
			foreach (string c in candidates)
				if (c == actor)
					return true;
			return false;
		}

		public int FindHeader (string name, string ns, params string [] actors)
		{
			int found = 0;
			int retval = -1;
			
			for (int i = 0; i < l.Count; i++) {
				MessageHeaderInfo info = l [i];

				if (info.Name == name && info.Namespace == ns) {
					if (found > 1)
						throw new ArgumentException ("Found multiple matching headers.");
					// When no actors are passed, it never
					// matches such header that has an
					// Actor.
					if (actors == null && info.Actor == String.Empty ||
					    actors != null && HasActor (info.Actor, actors)) {
						retval = i;
						found++;
					}
				}
			}

			return retval;
		}

		public IEnumerator<MessageHeaderInfo> GetEnumerator ()
		{
			return l.GetEnumerator ();
		}

		XmlObjectSerializer GetSerializer<T> ()
		{
			if (!serializers.ContainsKey (typeof (T)))
				serializers [typeof (T)] = new DataContractSerializer (typeof (T));
			return serializers [typeof (T)];
		}

		public T GetHeader<T> (int index)
		{
			if (typeof (T) == typeof (EndpointAddress)) {
				XmlDictionaryReader r = GetReaderAtHeader (index);
				return (T) (object) new EndpointAddress (r.ReadElementContentAsString ());
			}
			else
				return GetHeader<T> (index, GetSerializer<T> ());
		}

		public T GetHeader<T> (int index, XmlObjectSerializer serializer)
		{
			if (serializer == null)
				throw new ArgumentNullException ("serializer");
			XmlDictionaryReader r = GetReaderAtHeader (index);
			return (T) serializer.ReadObject (r, false);
		}

		public T GetHeader<T> (string name, string ns)
		{
			return GetHeader<T> (name, ns, empty_strings);
		}

		public T GetHeader<T> (string name, string ns, params string [] actors)
		{
			int idx = FindHeader (name, ns, actors);

			if (idx == -1)
				throw new MessageHeaderException (String.Format ("Header '{0}:{1}' was not found for the argument actors: {2}", ns, name, String.Join (",", actors)));

			return GetHeader<T> (idx);
		}

		public T GetHeader<T> (string name, string ns, XmlObjectSerializer serializer)
		{
			if (serializer == null)
				throw new ArgumentNullException ("serializer");
			int idx = FindHeader (name, ns);

			if (idx < 0)
				throw new MessageHeaderException (String.Format ("Header '{0}:{1}' was not found", ns, name));

			return GetHeader<T> (idx, serializer);
		}

		public XmlDictionaryReader GetReaderAtHeader (int index)
		{
			if (index >= l.Count)
				throw new ArgumentOutOfRangeException (String.Format ("Index is out of range. Current header count is {0}", index));
			MessageHeader item = (MessageHeader) l [index];

			XmlReader reader =
#if !NET_2_1 // FIXME: implement RawMessageHeader
				item is MessageHeader.RawMessageHeader ?
				((MessageHeader.RawMessageHeader) item).CreateReader () :
#endif
				XmlReader.Create (
					new StringReader (item.ToString ()),
					reader_settings);
			reader.MoveToContent ();
			XmlDictionaryReader dr = XmlDictionaryReader.CreateDictionaryReader (reader);
			dr.MoveToContent ();
			return dr;
		}

		public bool HaveMandatoryHeadersBeenUnderstood ()
		{
			throw new NotImplementedException ();
		}

		public bool HaveMandatoryHeadersBeenUnderstood (params string [] actors)
		{
			throw new NotImplementedException ();
		}

		public void Insert (int index, MessageHeader header)
		{
			l.Insert (index, header);
		}

		public void RemoveAll (string name, string ns)
		{
			l.RemoveAll (delegate (MessageHeaderInfo info) {
						return info.Name == name && info.Namespace == ns;
					});
		}

		public void RemoveAt (int index)
		{
			l.RemoveAt (index);
		}

		IEnumerator IEnumerable.GetEnumerator ()
		{
			return ((IEnumerable) l).GetEnumerator ();
		}

		public void WriteHeader (int index, XmlDictionaryWriter writer)
		{
			if (version.Envelope == EnvelopeVersion.None)
				return;

			// For AddressingVersion.None, don't output the item.
			//
			// FIXME: It should even ignore Action, but for now
			// service dispatcher won't work without it.
			if (version.Addressing == AddressingVersion.None &&
			    l [index].Name != "Action")
				return;

			WriteStartHeader (index, writer);
			WriteHeaderContents (index, writer);
			writer.WriteEndElement ();
		}

		public void WriteHeader (int index, XmlWriter writer)
		{
			WriteHeader (index, XmlDictionaryWriter.CreateDictionaryWriter (writer));
		}

		public void WriteHeaderContents (int index, XmlDictionaryWriter writer)
		{
			if (index > l.Count)
				throw new ArgumentOutOfRangeException ("There is no header at position " + index + ".");
			
			MessageHeader h = l [index] as MessageHeader;

			h.WriteHeaderContents (writer, version);
		}

		public void WriteHeaderContents (int index, XmlWriter writer)
		{
			WriteHeaderContents (index, XmlDictionaryWriter.CreateDictionaryWriter (writer));
		}

		public void WriteStartHeader (int index, XmlDictionaryWriter writer)
		{
			if (index > l.Count)
				throw new ArgumentOutOfRangeException ("There is no header at position " + index + ".");

			MessageHeader h = l [index] as MessageHeader;
			
			h.WriteStartHeader (writer, version);
		}

		public void WriteStartHeader (int index, XmlWriter writer)
		{
			WriteStartHeader (index, XmlDictionaryWriter.CreateDictionaryWriter (writer));
		}

		public string Action {
			get {
				int idx = FindHeader ("Action", version.Addressing.Namespace);
				return idx < 0 ? null : GetHeader<string> (idx);
			}
			set {
				RemoveAll ("Action", version.Addressing.Namespace);
				Add (MessageHeader.CreateHeader ("Action", version.Addressing.Namespace, value, true));
			}
		}

		public int Count {
			get { return l.Count; }
		}

#if !NET_2_1
		public EndpointAddress FaultTo {
			get {
				int idx = FindHeader ("FaultTo", Constants.WSA1);
				return idx < 0 ? null : GetHeader<EndpointAddress> (idx);
			}
			set {
				if (version.Addressing == AddressingVersion.None)
					throw new InvalidOperationException ("WS-Addressing header is not allowed for AddressingVersion.None");

				RemoveAll ("FaultTo", Constants.WSA1);
				Add (MessageHeader.CreateHeader (
						"FaultTo", 
						Constants.WSA1,
						EndpointAddress10.FromEndpointAddress (value)));
			}
		}

		public EndpointAddress From {
			get {
				int idx = FindHeader ("From", version.Addressing.Namespace);
				return idx < 0 ? null : GetHeader<EndpointAddress> (idx);
			}
			set { 
				if (version.Addressing == AddressingVersion.None)
					throw new InvalidOperationException ("WS-Addressing header is not allowed for AddressingVersion.None");

				RemoveAll ("From", Constants.WSA1);
				Add (MessageHeader.CreateHeader (
						"From", 
						Constants.WSA1,
						EndpointAddress10.FromEndpointAddress (value)));
			}
		}
#endif

		public MessageHeaderInfo this [int index] {
			get { return l [index]; }
		}

		public UniqueId MessageId {
			get { 
				int idx = FindHeader ("MessageID", Constants.WSA1);
				return idx < 0 ? null : new UniqueId (GetHeader<string> (idx));
			}
			set {
				if (version.Addressing == AddressingVersion.None)
					throw new InvalidOperationException ("WS-Addressing header is not allowed for AddressingVersion.None");

				RemoveAll ("MessageID", Constants.WSA1);
				Add (MessageHeader.CreateHeader (
						"MessageID", 
						Constants.WSA1,
						value.ToString ()));
			}
		}

		public MessageVersion MessageVersion { get { return version; } }

		public UniqueId RelatesTo {
			get { 
				int idx = FindHeader ("RelatesTo", Constants.WSA1);
				return idx < 0 ? null : new UniqueId (GetHeader<string> (idx));
			}
			set {
				if (version.Addressing == AddressingVersion.None)
					throw new InvalidOperationException ("WS-Addressing header is not allowed for AddressingVersion.None");

				RemoveAll ("MessageID", Constants.WSA1);
				Add (MessageHeader.CreateHeader (
						"RelatesTo", 
						Constants.WSA1,
						value.ToString ()));
			}

		}

#if !NET_2_1
		public EndpointAddress ReplyTo {
			get {
				int idx = FindHeader ("ReplyTo", Constants.WSA1);
				return idx < 0 ? null : GetHeader<EndpointAddress> (idx);
			}
			set {
				if (version.Addressing == AddressingVersion.None)
					throw new InvalidOperationException ("WS-Addressing header is not allowed for AddressingVersion.None");

				RemoveAll ("ReplyTo", Constants.WSA1);
				Add (MessageHeader.CreateHeader (
						"ReplyTo", 
						Constants.WSA1,
						EndpointAddress10.FromEndpointAddress (value)));
			}
		}
#endif

		public Uri To {
			get {
				int idx = FindHeader ("To", version.Addressing.Namespace);
				//FIXME: return idx < 0 ? null : GetHeader<Uri> (idx);
				return idx < 0 ? null : new Uri (GetHeader<string> (idx));
			}
			set { 
				RemoveAll ("To", version.Addressing.Namespace);
				Add (MessageHeader.CreateHeader (
						"To", 
						version.Addressing.Namespace, 
						value.AbsoluteUri,
						true));
			}
		}

		[MonoTODO]
		public UnderstoodHeaders UnderstoodHeaders {
			get { throw new NotImplementedException (); }
		}
	}
}
