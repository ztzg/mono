//
// System.ServiceModel.MessageHeader.cs
//
// Author: Duncan Mak (duncan@novell.com)
//	   Atsushi Enomoto (atsushi@ximian.com)
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
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Text;
using System.Xml;

namespace System.ServiceModel.Channels
{
	public abstract class MessageHeader : MessageHeaderInfo
	{
		static readonly XmlWriterSettings writer_settings;

		static MessageHeader ()
		{
			writer_settings = new XmlWriterSettings ();
			writer_settings.OmitXmlDeclaration = true;
			writer_settings.Indent = true;
		}

		protected MessageHeader () {}

		static string default_actor = String.Empty;
		static bool default_is_ref = false;
		static bool default_must_understand = false;
		static bool default_relay = false;
		static Type [] knownTypes = new Type [1] {typeof (EndpointAddress10)};

#if !NET_2_1 // FIXME: eliminate XmlElement
		internal static MessageHeader CreateInternalHeader (XmlElement el, string soap_ns)
		{
			return new RawMessageHeader (el, soap_ns);
		}
#endif

		public static MessageHeader CreateHeader (string name, string ns, object value)
		{
			return CreateHeader (name, ns, value, default_must_understand);
		}

		public static MessageHeader CreateHeader (string name, string ns, object value, bool must_understand)
		{
			return CreateHeader (name, ns, value, must_understand, default_actor);
		}

		public static MessageHeader CreateHeader (string name, string ns, object value, XmlObjectSerializer formatter)
		{
			return CreateHeader (name, ns, value, formatter, default_must_understand, 
					     default_actor, default_relay);
		}

		public static MessageHeader CreateHeader (string name, string ns, object value, 
						   bool must_understand, string actor)
		{
			return CreateHeader (name, ns, value, must_understand, actor, default_relay);
		}

		public static MessageHeader CreateHeader (string name, string ns, object value, XmlObjectSerializer formatter, 
						   bool must_understand)
		{
			return CreateHeader (name, ns, value, formatter, must_understand, default_actor, default_relay);
		}
		
		public static MessageHeader CreateHeader (string name, string ns, object value, 
						   bool must_understand, string actor, bool relay)
		{
			return CreateHeader (name, ns, value, new DataContractSerializer (value.GetType (), knownTypes),
					must_understand, actor, relay);
		}

		public static MessageHeader CreateHeader (string name, string ns, object value, XmlObjectSerializer formatter,
						   bool must_understand, string actor)
		{
			return CreateHeader (name, ns, value, formatter, must_understand, actor, default_relay);
		}
		
		public static MessageHeader CreateHeader (string name, string ns, object value, XmlObjectSerializer formatter,
						   bool must_understand, string actor, bool relay)
		{
			// FIXME: how to get IsReferenceParameter ?
			return new DefaultMessageHeader (name, ns, value, formatter, default_is_ref, must_understand, actor, relay);
		}

		public virtual bool IsMessageVersionSupported (MessageVersion version)
		{
			if (version.Envelope == EnvelopeVersion.Soap12)
				if (Actor == EnvelopeVersion.Soap11.NextDestinationActorValue)
					return false;

			if (version.Envelope == EnvelopeVersion.Soap11)
				if (Actor == EnvelopeVersion.Soap12.NextDestinationActorValue ||
				    Actor == EnvelopeVersion.Soap12UltimateReceiver)
					return false;

			// by default, it's always supported
			return true;
		}

		protected abstract void OnWriteHeaderContents (XmlDictionaryWriter writer, MessageVersion version);

		protected virtual void OnWriteStartHeader (XmlDictionaryWriter writer, MessageVersion version)
		{
			writer.WriteStartElement (this.Name, this.Namespace);
			WriteHeaderAttributes (writer, version);
		}

		public override string ToString ()
		{
			StringBuilder sb = new StringBuilder ();

			XmlWriter w = XmlWriter.Create (sb, writer_settings);

			WriteHeader (w, MessageVersion.Default);
			w.Close ();

			return sb.ToString ();
		}

		public void WriteHeader (XmlDictionaryWriter writer, MessageVersion version)
		{
			if (writer == null)
				throw new ArgumentNullException ("writer is null.");

			if (version == null)
				throw new ArgumentNullException ("version is null.");

			if (version.Envelope == EnvelopeVersion.None)
				return;

			WriteStartHeader (writer, version);
			WriteHeaderContents (writer, version);

			writer.WriteEndElement ();
		}

		public void WriteHeader (XmlWriter writer, MessageVersion version)
		{
			WriteHeader (XmlDictionaryWriter.CreateDictionaryWriter (writer), version);
		}

		protected void WriteHeaderAttributes (XmlDictionaryWriter writer, MessageVersion version)
		{
			if (Id != null)
				writer.WriteAttributeString ("u", "Id", Constants.WsuNamespace, Id);
			if (Actor != String.Empty) {
				if (version.Envelope == EnvelopeVersion.Soap11) 
					writer.WriteAttributeString ("s", "actor", version.Envelope.Namespace, Actor);

				if (version.Envelope == EnvelopeVersion.Soap12) 
					writer.WriteAttributeString ("s", "role", version.Envelope.Namespace, Actor);
			}

			// mustUnderstand is the same across SOAP 1.1 and 1.2
			if (MustUnderstand == true)
				writer.WriteAttributeString ("s", "mustUnderstand", version.Envelope.Namespace, "1");

			// relay is only available on SOAP 1.2
			if (Relay == true && version.Envelope == EnvelopeVersion.Soap12)
				writer.WriteAttributeString ("s", "relay", version.Envelope.Namespace, "true");
		}

		public void WriteHeaderContents (XmlDictionaryWriter writer, MessageVersion version)
		{
			this.OnWriteHeaderContents (writer, version);
		}

		public void WriteStartHeader (XmlDictionaryWriter writer, MessageVersion version)
		{
			this.OnWriteStartHeader (writer, version);
		}

		public override string Actor { get { return default_actor; }}

		public override bool IsReferenceParameter { get { return default_is_ref; }}

		public override bool MustUnderstand { get { return default_must_understand; }}

		public override bool Relay { get { return default_relay; }}

#if !NET_2_1 // FIXME: this should be rewritten to eliminate XmlElement
		internal class RawMessageHeader : MessageHeader
		{
			XmlElement source;
			string soap_ns;
			bool is_ref, must_understand, relay;
			string actor;

			public RawMessageHeader (XmlElement source, string soap_ns)
			{
				this.source = source;

				Id = source.HasAttribute ("Id", Constants.WsuNamespace) ?
					source.GetAttribute ("Id", Constants.WsuNamespace) :
					null;

				// FIXME: fill is_ref
				string s = source.GetAttribute ("relay", soap_ns);
				relay = s.Length > 0 ? XmlConvert.ToBoolean (s) : false;
				s = source.GetAttribute ("mustUnderstand", soap_ns);
				must_understand = s.Length > 0 ? XmlConvert.ToBoolean (s) : false;
				actor = source.GetAttribute ("actor", soap_ns);
			}

			public XmlReader CreateReader ()
			{
				return new XmlNodeReader (source);
			}

			protected override void OnWriteHeaderContents (
				XmlDictionaryWriter writer, MessageVersion version)
			{
				source.WriteContentTo (writer);
			}

			public override string Actor { get { return actor; }}

			public override bool IsReferenceParameter { get { return is_ref; }}

			public override bool MustUnderstand { get { return must_understand; }}

			public override string Name { get { return source.LocalName; }}

			public override string Namespace { get { return source.NamespaceURI; }}

			public override bool Relay { get { return relay; }}
		}
#endif

		internal class DefaultMessageHeader : MessageHeader
		{
			string actor, name, ns;
			object value;
			XmlObjectSerializer formatter;
			bool is_ref, must_understand, relay;
			
			internal DefaultMessageHeader (string name, string ns, object value, XmlObjectSerializer formatter, 
						       bool isReferenceParameter,
						       bool mustUnderstand, string actor, bool relay)
			{
				this.name = name;
				this.ns = ns;
				this.value = value;
				this.formatter = formatter;
				this.is_ref = isReferenceParameter;
				this.must_understand = mustUnderstand;
				this.actor = actor;
				this.relay = relay;
			}

			protected override void OnWriteHeaderContents (XmlDictionaryWriter writer,
								       MessageVersion version)
			{
				if (value is EndpointAddress)
					((EndpointAddress) value).WriteTo (version.Addressing, writer, name, ns);
				else
					this.formatter.WriteObjectContent (writer, value);
			}

			public override string Actor { get { return actor; }}

			public override bool IsReferenceParameter { get { return is_ref; }}

			public override bool MustUnderstand { get { return must_understand; }}

			public override string Name { get { return name; }}

			public override string Namespace { get { return ns; }}

			public override bool Relay { get { return relay; }}
		}
	}
}
