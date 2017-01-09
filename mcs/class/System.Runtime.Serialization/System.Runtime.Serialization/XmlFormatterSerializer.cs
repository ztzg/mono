//
// XmlFormatterSerializer.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2005 Novell, Inc.  http://www.novell.com
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
#if NET_2_0
using System;
using System.Collections;
using System.IO;
using System.Reflection;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml;
using System.Xml.Schema;

using QName = System.Xml.XmlQualifiedName;

namespace System.Runtime.Serialization
{
	internal class XmlFormatterSerializer
	{
		XmlDictionaryWriter writer;
		object graph;
		KnownTypeCollection types;
		
		bool save_id;
		bool ignore_unknown;
		IDataContractSurrogate surrogate;
		int max_items;

		ArrayList objects = new ArrayList ();

		public static void Serialize (XmlDictionaryWriter writer, object graph,
			KnownTypeCollection types,
			bool ignoreUnknown, int maxItems, string root_ns)
		{
			new XmlFormatterSerializer (writer, types, ignoreUnknown, maxItems, root_ns)
				.Serialize (graph != null ? graph.GetType () : null, graph);
		}

		public XmlFormatterSerializer (XmlDictionaryWriter writer,
			KnownTypeCollection types,
			bool ignoreUnknown, int maxItems, string root_ns)
		{
			this.writer = writer;
			this.types = types;
			ignore_unknown = ignoreUnknown;
			max_items = maxItems;
		}

		public XmlDictionaryWriter Writer {
			get { return writer; }
		}

		public void Serialize (Type type, object graph)
		{
			if (graph == null)
				writer.WriteAttributeString ("nil", XmlSchema.InstanceNamespace, "true");
			else {
				Type actualType = graph.GetType ();
				QName predef = KnownTypeCollection.GetPredefinedTypeName (actualType);
				if (predef != QName.Empty)
					SerializePrimitive (type, graph, predef);
				else
					SerializeNonPrimitive (type, graph);
			}
		}

		public void SerializePrimitive (Type type, object graph, QName qname)
		{
//			writer.WriteStartAttribute ("type", XmlSchema.InstanceNamespace);
//			writer.WriteQualifiedName (qname.Name, qname.Namespace);
//			writer.WriteEndAttribute ();
			writer.WriteString (KnownTypeCollection.PredefinedTypeObjectToString (graph));
		}

		public void WriteStartElement (string rootName, string rootNamespace, string currentNamespace)
		{
			writer.WriteStartElement (rootName, rootNamespace);
			if (!string.IsNullOrEmpty (currentNamespace) && currentNamespace != rootNamespace)
				writer.WriteXmlnsAttribute (null, currentNamespace);
		}

		public void WriteEndElement ()
		{
			writer.WriteEndElement ();
		}

		private void Write_i_type (QName qname)
		{
			writer.WriteStartAttribute ("type", XmlSchema.InstanceNamespace);
			writer.WriteQualifiedName (qname.Name, qname.Namespace);
			writer.WriteEndAttribute ();
		}

		public void SerializeNonPrimitive (Type type, object graph)
		{
			if (objects.Contains (graph))
				throw new SerializationException (String.Format ("Circular reference of an object in the object graph was found: '{0}' of type {1}", graph, graph.GetType ()));
			objects.Add (graph);

			Type actualType = graph.GetType ();

			SerializationMap map = types.FindUserMap (actualType);
			if (map == null) {
				/* Unknown actual type */
				types.Add (actualType);
				map = types.FindUserMap (actualType);
//				throw new InvalidDataContractException (String.Format ("Type {0} has neither Serializable nor DataContract attributes.", type));
			}

			if (type != actualType)
				Write_i_type (map.XmlName);

			map.Serialize (graph, this);
		}
	}
}
#endif
