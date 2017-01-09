//
// SerializationMap.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//	Ankit Jain <JAnkit@novell.com>
//	Duncan Mak (duncan@ximian.com)
//	Eyal Alaluf (eyala@mainsoft.com)
//
// Copyright (C) 2005 Novell, Inc.  http://www.novell.com
// Copyright (C) 2006 Novell, Inc.  http://www.novell.com
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
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reflection;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

using QName = System.Xml.XmlQualifiedName;

namespace System.Runtime.Serialization
{
/*
	XmlFormatter implementation design inference:

	type definitions:
	- No XML Schema types are directly used. There are some maps from
	  xs:blahType to ms:blahType where the namespaceURI for prefix "ms" is
	  "http://schemas.microsoft.com/2003/10/Serialization/" .

	serializable types:
	- An object being serialized 1) must be of type System.Object, or
	  2) must be null, or 3) must have either a [DataContract] attribute
	  or a [Serializable] attribute to be serializable.
	- When the object is either of type System.Object or null, then the
	  XML type is "anyType".
	- When the object is [Serializable], then the runtime-serialization
	  compatible object graph is written.
	- Otherwise the serialization is based on contract attributes.
	  ([Serializable] takes precedence).

	type derivation:
	- For type A to be serializable, the base type B of A must be
	  serializable.
	- If a type which is [Serializable] and whose base type has a
	  [DataContract], then for base type members [DataContract] is taken.
	- It is vice versa i.e. if the base type is [Serializable] and the
	  derived type has a [DataContract], then [Serializable] takes place
	  for base members.

	known type collection:
	- It internally manages mapping store keyed by contract QNames.
	  KnownTypeCollection.Add() checks if the same QName contract already
	  exists (and raises InvalidOperationException if required).

*/
	internal abstract class SerializationMap
	{
		public const BindingFlags AllInstanceFlags =
			BindingFlags.Public | BindingFlags.NonPublic |
			BindingFlags.Instance;

		public readonly KnownTypeCollection KnownTypes;
		public readonly Type RuntimeType;
		public readonly QName XmlName;
		public List<DataMemberInfo> Members;
#if !NET_2_1
		XmlSchemaSet schema_set;
#endif
 
		//FIXME FIXME
		Dictionary<Type, QName> qname_table = new Dictionary<Type, QName> ();

		protected SerializationMap (
			Type type, QName qname, KnownTypeCollection knownTypes)
		{
			KnownTypes = knownTypes;
			RuntimeType = type;
			if (qname.Namespace == String.Empty)
				qname = new QName (qname.Name,
					"http://schemas.datacontract.org/2004/07/" + type.Namespace);

			XmlName = qname;
			Members = new List<DataMemberInfo> ();
		}

		public DataMemberAttribute GetDataMemberAttribute (
			MemberInfo mi)
		{
			object [] atts = mi.GetCustomAttributes (
				typeof (DataMemberAttribute), false);
			if (atts.Length == 0)
				return null;
			return (DataMemberAttribute) atts [0];
		}

		bool IsPrimitive (Type type)
		{
			return (Type.GetTypeCode (type) != TypeCode.Object || type == typeof (object));
		}

#if !NET_2_1
		/* Returns the XmlSchemaType AND adds it to @schemas */
		public virtual XmlSchemaType GetSchemaType (XmlSchemaSet schemas, Dictionary<QName, XmlSchemaType> generated_schema_types)
		{
			if (IsPrimitive (RuntimeType))
				return null;

			if (generated_schema_types.ContainsKey (XmlName)) // Caching  
				return generated_schema_types [XmlName] as XmlSchemaType;

			XmlSchemaComplexType complex_type = null;

			complex_type = new XmlSchemaComplexType ();
			complex_type.Name = XmlName.Name;
			generated_schema_types [XmlName] = complex_type;

			if (RuntimeType.BaseType == typeof (object)) {
				complex_type.Particle = GetSequence (schemas, generated_schema_types);
			} else {
				//Has a non-System.Object base class
				XmlSchemaComplexContentExtension extension = new XmlSchemaComplexContentExtension ();
				XmlSchemaComplexContent content = new XmlSchemaComplexContent ();

				complex_type.ContentModel = content;
				content.Content = extension;

				KnownTypes.Add (RuntimeType.BaseType);
				SerializationMap map = KnownTypes.FindUserMap (RuntimeType.BaseType);
				//FIXME: map == null ?
				map.GetSchemaType (schemas, generated_schema_types);

				extension.Particle = GetSequence (schemas, generated_schema_types);
				extension.BaseTypeName = GetQualifiedName (RuntimeType.BaseType);
			}
			
			XmlSchemaElement schemaElement = GetSchemaElement (XmlName, complex_type);
			XmlSchema schema = GetSchema (schemas, XmlName.Namespace);
			schema.Items.Add (complex_type);
			schema.Items.Add (schemaElement);
			schemas.Reprocess (schema);

			return complex_type;
		}

		/* Returns the <xs:sequence> for the data members */
		XmlSchemaSequence GetSequence (XmlSchemaSet schemas,
				Dictionary<QName, XmlSchemaType> generated_schema_types)
		{
			List<DataMemberInfo> members = GetMembers ();

			XmlSchema schema = GetSchema (schemas, XmlName.Namespace);
			XmlSchemaSequence sequence = new XmlSchemaSequence ();
			foreach (DataMemberInfo dmi in members) {
				// delegates are not supported.
				if (!dmi.MemberType.IsAbstract && typeof (System.Delegate).IsAssignableFrom (dmi.MemberType))
					continue;

				XmlSchemaElement element = new XmlSchemaElement ();
				element.Name = dmi.XmlName;

				KnownTypes.Add (dmi.MemberType);
				SerializationMap map = KnownTypes.FindUserMap (dmi.MemberType);
				if (map != null) {
					XmlSchemaType schema_type = map.GetSchemaType (schemas, generated_schema_types);
					if (schema_type is XmlSchemaComplexType)
						element.IsNillable = true;
				} else {
					//Primitive type
					if (dmi.MemberType == typeof (string))
						element.IsNillable = true;
				}

				element.MinOccurs = 0;

				element.SchemaTypeName = GetQualifiedName (dmi.MemberType);
				AddImport (schema, element.SchemaTypeName.Namespace);

				sequence.Items.Add (element);
			}

			schemas.Reprocess (schema);
			return sequence;
		}

		//FIXME: Replace with a dictionary ?
		void AddImport (XmlSchema schema, string ns)
		{
			if (ns == XmlSchema.Namespace || schema.TargetNamespace == ns)
				return;

			foreach (XmlSchemaObject o in schema.Includes) {
				XmlSchemaImport import = o as XmlSchemaImport;
				if (import == null)
					continue;
				if (import.Namespace == ns)
					return;
			}

			XmlSchemaImport imp = new XmlSchemaImport ();
			imp.Namespace = ns;
			schema.Includes.Add (imp);
		}
#endif

		//Returns list of data members for this type ONLY
		public virtual List<DataMemberInfo> GetMembers ()
		{
			throw new NotImplementedException (String.Format ("Implement me for {0}", this));
		}

#if !NET_2_1
		protected XmlSchemaElement GetSchemaElement (QName qname, XmlSchemaType schemaType)
		{
			XmlSchemaElement schemaElement = new XmlSchemaElement ();
			schemaElement.Name = qname.Name;
			schemaElement.SchemaTypeName = qname;

			if (schemaType is XmlSchemaComplexType)
				schemaElement.IsNillable = true;

			return schemaElement;
		}

		protected XmlSchema GetSchema (XmlSchemaSet schemas, string ns)
		{
			ICollection colln = schemas.Schemas (ns);
			if (colln.Count > 0) {
				if (colln.Count > 1)
					throw new Exception (String.Format (
						"More than 1 schema for namespace '{0}' found.", ns));
				foreach (object o in colln)
					//return colln [0]
					return (o as XmlSchema);
			}

			XmlSchema schema = new XmlSchema ();
			schema.TargetNamespace = ns;
			schema.ElementFormDefault = XmlSchemaForm.Qualified;
			schemas.Add (schema);

			return schema;
		}
#endif

		//FIXME: redundant?
		protected XmlQualifiedName GetQualifiedName (Type type)
		{
			if (qname_table.ContainsKey (type))
				return qname_table [type];

			QName qname = KnownTypes.GetQName (type);
			if (qname.Namespace == KnownTypeCollection.MSSimpleNamespace)
				qname = new QName (qname.Name, XmlSchema.Namespace);

			qname_table [type] = qname;
			return qname;
		}

		public virtual void Serialize (object graph,
			XmlFormatterSerializer serializer)
		{
			foreach (DataMemberInfo dmi in Members) {
				FieldInfo fi = dmi.Member as FieldInfo;
				PropertyInfo pi = fi == null ?
					(PropertyInfo) dmi.Member : null;
				Type type = fi != null ?
					fi.FieldType : pi.PropertyType;
				object value = fi != null ?
					fi.GetValue (graph) :
					pi.GetValue (graph, null);

				serializer.WriteStartElement (dmi.XmlName, dmi.XmlRootNamespace, dmi.XmlNamespace);
				serializer.Serialize (type, value);
				serializer.WriteEndElement ();
			}
		}
		
		/* Deserialize non-primitive types */
		public virtual object DeserializeContent (XmlReader reader,
			XmlFormatterDeserializer deserializer)
		{
			object instance = FormatterServices.GetUninitializedObject (RuntimeType);
			int depth = reader.NodeType == XmlNodeType.None ? reader.Depth : reader.Depth - 1;
			bool [] filled = new bool [Members.Count];
			int memberInd = -1;
			while (reader.NodeType == XmlNodeType.Element && reader.Depth > depth) {
				DataMemberInfo dmi = null;
				for (int i = memberInd + 1; i < Members.Count; i++) {
					if (reader.LocalName == Members [i].XmlName &&
						reader.NamespaceURI == Members [i].XmlRootNamespace) {
						memberInd = i;
						dmi = Members [i];
						break;
					}
				}

				if (dmi == null) {
					reader.Skip ();
					continue;
				}
				SetValue (dmi, instance, deserializer.Deserialize (dmi.MemberType, reader));
				filled [memberInd] = true;
			}
			for (int i = 0; i < Members.Count; i++)
				if (!filled [i] && Members [i].IsRequired)
					throw MissingRequiredMember (Members [i], reader);

			return instance;
		}

		// For now it could be private.
		protected Exception MissingRequiredMember (DataMemberInfo dmi, XmlReader reader)
		{
			return new ArgumentException (String.Format ("Data contract member {0} is required, but missing in the input XML.", new QName (dmi.XmlName, dmi.XmlNamespace)));
		}

		// For now it could be private.
		protected void SetValue (DataMemberInfo dmi, object obj, object value)
		{
			if (dmi.Member is PropertyInfo)
				((PropertyInfo) dmi.Member).SetValue (obj, value, null);
			else
				((FieldInfo) dmi.Member).SetValue (obj, value);
		}

		protected DataMemberInfo CreateDataMemberInfo (DataMemberAttribute dma, MemberInfo mi, Type type)
		{
			KnownTypes.Add (type);
			QName qname = KnownTypes.GetQName (type);
			string rootNamespace = KnownTypes.GetQName (mi.DeclaringType).Namespace;
			if (KnownTypeCollection.IsPrimitiveType (qname))
				return new DataMemberInfo (mi, dma, rootNamespace, null);
			else
				return new DataMemberInfo (mi, dma, rootNamespace, qname.Namespace);
		}
	}

	internal class XmlSerializableMap : SerializationMap
	{
		public XmlSerializableMap (Type type, QName qname, KnownTypeCollection knownTypes)
			: base (type, qname, knownTypes)
		{
		}

		public override void Serialize (object graph, XmlFormatterSerializer serializer)
		{
			IXmlSerializable ixs = graph as IXmlSerializable;
			if (ixs == null)
				//FIXME: Throw what exception here?
				throw new SerializationException ();

			ixs.WriteXml (serializer.Writer);
		}
	}

	internal class SharedContractMap : SerializationMap
	{
		public SharedContractMap (
			Type type, QName qname, KnownTypeCollection knownTypes)
			: base (type, qname, knownTypes)
		{
			Type baseType = type;
			List <DataMemberInfo> members = new List <DataMemberInfo> ();
			
			while (baseType != null) {
				QName bqname = knownTypes.GetQName (baseType);
					
				members = GetMembers (baseType, bqname, true);
				Members.InsertRange (0, members);
				members.Clear ();

				baseType = baseType.BaseType;
			}

//			Members.Sort (delegate (
//				DataMemberInfo d1, DataMemberInfo d2) {
//					return d1.Order - d2.Order;
//				});
		}

		List<DataMemberInfo> GetMembers (Type type, QName qname, bool declared_only)
		{
			List<DataMemberInfo> data_members = new List<DataMemberInfo> ();
			BindingFlags flags = AllInstanceFlags;
			if (declared_only)
				flags |= BindingFlags.DeclaredOnly;

			foreach (PropertyInfo pi in type.GetProperties (flags)) {
				DataMemberAttribute dma =
					GetDataMemberAttribute (pi);
				if (dma == null)
					continue;
				if (!pi.CanRead || !pi.CanWrite)
					throw new InvalidDataContractException (String.Format (
							"DataMember property {0} must have both getter and setter.", pi));
				data_members.Add (CreateDataMemberInfo (dma, pi, pi.PropertyType));
			}

			foreach (FieldInfo fi in type.GetFields (flags)) {
				DataMemberAttribute dma =
					GetDataMemberAttribute (fi);
				if (dma == null)
					continue;
				if (fi.IsInitOnly)
					throw new InvalidDataContractException (String.Format (
							"DataMember field {0} must not be read-only.", fi));
				data_members.Add (CreateDataMemberInfo (dma, fi, fi.FieldType));
			}

			data_members.Sort (DataMemberInfo.DataMemberInfoComparer.Instance);

			return data_members;
		}

		public override List<DataMemberInfo> GetMembers ()
		{
			return GetMembers (RuntimeType, XmlName, true);
		}
	}

	internal class CollectionTypeMap : SerializationMap
	{
		Type element_type;
		QName element_qname;

		public CollectionTypeMap (
			Type type, Type elementType,
			QName qname, KnownTypeCollection knownTypes)
			: base (type, qname, knownTypes)
		{
			element_type = elementType;
			element_qname = KnownTypes.GetQName (element_type);
		}

		public override void Serialize (object graph,
			XmlFormatterSerializer serializer)
		{
			string ns = element_qname.Namespace;
			if (ns == KnownTypeCollection.MSSimpleNamespace)
				ns = KnownTypeCollection.MSArraysNamespace;

			foreach (object o in (IEnumerable) graph) {
				serializer.WriteStartElement (element_qname.Name, XmlName.Namespace, ns);
				serializer.Serialize (element_type, o);
				serializer.WriteEndElement ();
			}
		}

        public override object DeserializeContent(XmlReader reader, XmlFormatterDeserializer deserializer)
        {
            object instance;
            if (RuntimeType.IsArray)
                instance = new ArrayList ();
            else
                instance = Activator.CreateInstance (RuntimeType, true);
            int depth = reader.NodeType == XmlNodeType.None ? reader.Depth : reader.Depth - 1;
            while (reader.NodeType == XmlNodeType.Element && reader.Depth > depth) {
                object elem = deserializer.Deserialize (element_type, reader);
                if (instance is IList)
                    ((IList)instance).Add (elem);
                else
                    throw new NotImplementedException (String.Format ("Type {0} is not supported", RuntimeType));
            }
            if (RuntimeType.IsArray)
                return ((ArrayList)instance).ToArray (element_type);
            return instance;
        }

		public override List<DataMemberInfo> GetMembers ()
		{
			//Shouldn't come here at all!
			throw new NotImplementedException ();
		}
		
#if !NET_2_1
		public override XmlSchemaType GetSchemaType (XmlSchemaSet schemas, Dictionary<QName, XmlSchemaType> generated_schema_types)
		{
			if (generated_schema_types.ContainsKey (XmlName))
				return null;

			if (generated_schema_types.ContainsKey (XmlName))
				return generated_schema_types [XmlName];

			QName element_qname = GetQualifiedName (element_type);

			XmlSchemaComplexType complex_type = new XmlSchemaComplexType ();
			complex_type.Name = XmlName.Name;

			XmlSchemaSequence sequence = new XmlSchemaSequence ();
			XmlSchemaElement element = new XmlSchemaElement ();

			element.MinOccurs = 0;
			element.MaxOccursString = "unbounded";
			element.Name = element_qname.Name;

			KnownTypes.Add (element_type);
			SerializationMap map = KnownTypes.FindUserMap (element_type);
			if (map != null) {// non-primitive type
				map.GetSchemaType (schemas, generated_schema_types);
				element.IsNillable = true;
			}

			element.SchemaTypeName = element_qname;

			sequence.Items.Add (element);
			complex_type.Particle = sequence;

			XmlSchema schema = GetSchema (schemas, XmlName.Namespace);
			schema.Items.Add (complex_type);
			schema.Items.Add (GetSchemaElement (XmlName, complex_type));
			schemas.Reprocess (schema);

			generated_schema_types [XmlName] = complex_type;

			return complex_type;
		}
#endif
	}

	internal class SharedTypeMap : SerializationMap
	{
		public SharedTypeMap (
			Type type, QName qname, KnownTypeCollection knownTypes)
			: base (type, qname, knownTypes)
		{
			Members = GetMembers (type, XmlName, false);
		}

		List<DataMemberInfo> GetMembers (Type type, QName qname, bool declared_only)
		{
			List<DataMemberInfo> data_members = new List<DataMemberInfo> ();
			int order = 0;
			BindingFlags flags = AllInstanceFlags;
			if (declared_only)
				flags |= BindingFlags.DeclaredOnly;
			
			foreach (FieldInfo fi in type.GetFields (flags)) {
				if (fi.GetCustomAttributes (
					typeof (NonSerializedAttribute),
					false).Length > 0)
					continue;

				if (fi.IsInitOnly)
					throw new InvalidDataContractException (String.Format ("DataMember field {0} must not be read-only.", fi));
				DataMemberAttribute dma = new DataMemberAttribute ();
				dma.Order = order++;
				data_members.Add (CreateDataMemberInfo (dma, fi, fi.FieldType));
			}

			return data_members;
		}

		public override List<DataMemberInfo> GetMembers ()
		{
			return GetMembers (RuntimeType, XmlName, true);
		}
	}

	internal class EnumMap : SerializationMap
	{
		List<EnumMemberInfo> enum_members;

		public EnumMap (
			Type type, QName qname, KnownTypeCollection knownTypes)
			: base (type, qname, knownTypes)
		{
			bool has_dc = false;
			object [] atts = RuntimeType.GetCustomAttributes (
				typeof (DataContractAttribute), false);
			if (atts.Length != 0)
				has_dc = true;

			enum_members = new List<EnumMemberInfo> ();
			BindingFlags flags = BindingFlags.Public | BindingFlags.DeclaredOnly | BindingFlags.Static;
			
			foreach (FieldInfo fi in RuntimeType.GetFields (flags)) {
				string name = fi.Name;
				if (has_dc) {
					EnumMemberAttribute ema =
						GetEnumMemberAttribute (fi);
					if (ema == null)
						continue;

					if (ema.Value != null)
						name = ema.Value;
				}

				enum_members.Add (new EnumMemberInfo (name, fi.GetValue (null)));
			}
		}

		private EnumMemberAttribute GetEnumMemberAttribute (
			MemberInfo mi)
		{
			object [] atts = mi.GetCustomAttributes (
				typeof (EnumMemberAttribute), false);
			if (atts.Length == 0)
				return null;
			return (EnumMemberAttribute) atts [0];
		}

#if !NET_2_1
		public override XmlSchemaType GetSchemaType (XmlSchemaSet schemas, Dictionary<QName, XmlSchemaType> generated_schema_types)
		{
			if (generated_schema_types.ContainsKey (XmlName))
				return generated_schema_types [XmlName];

			XmlSchemaSimpleType simpleType = new XmlSchemaSimpleType ();
			simpleType.Name = XmlName.Name;

			XmlSchemaSimpleTypeRestriction simpleRestriction = new XmlSchemaSimpleTypeRestriction ();
			simpleType.Content = simpleRestriction;
			simpleRestriction.BaseTypeName = new XmlQualifiedName ("string", XmlSchema.Namespace);

			foreach (EnumMemberInfo emi in enum_members) {
				XmlSchemaEnumerationFacet e = new XmlSchemaEnumerationFacet ();
				e.Value = emi.XmlName;
				simpleRestriction.Facets.Add (e);
			}

			generated_schema_types [XmlName] = simpleType;
			
			XmlSchema schema = GetSchema (schemas, XmlName.Namespace);
			XmlSchemaElement element = GetSchemaElement (XmlName, simpleType);
			element.IsNillable = true;

			schema.Items.Add (simpleType);
			schema.Items.Add (element);

			return simpleType;
		}
#endif

		public override void Serialize (object graph,
			XmlFormatterSerializer serializer)
		{
			foreach (EnumMemberInfo emi in enum_members) {
				if (Enum.Equals (emi.Value, graph)) {
					serializer.Writer.WriteString (emi.XmlName);
					return;
				}
			}

			throw new SerializationException (String.Format (
				"Enum value '{0}' is invalid for type '{1}' and cannot be serialized.", graph, RuntimeType));
		}

		public override object DeserializeContent (XmlReader reader,
			XmlFormatterDeserializer deserializer)
		{
			string value = reader.NodeType != XmlNodeType.Text ? String.Empty : reader.ReadContentAsString ();

			if (value != String.Empty) {
				foreach (EnumMemberInfo emi in enum_members)
					if (emi.XmlName == value)
						return emi.Value;
			}

			throw new SerializationException (String.Format (
				"Enum value '{0}' is invalid for type '{1}' and cannot be deserialized.", value, RuntimeType));
		}
	}

	internal struct EnumMemberInfo
	{
		public readonly string XmlName;
		public readonly object Value;

		public EnumMemberInfo (string name, object value)
		{
			XmlName = name;
			Value = value;
		}
	}

	internal class DataMemberInfo //: KeyValuePair<int, MemberInfo>
	{
		public readonly int Order;
		public readonly bool IsRequired;
		public readonly string XmlName;
		public readonly MemberInfo Member;
		public readonly string XmlNamespace;
		public readonly string XmlRootNamespace;
		public readonly Type MemberType;

		public DataMemberInfo (MemberInfo member, DataMemberAttribute dma, string rootNamespce, string ns)
		{
			if (dma == null)
				throw new ArgumentNullException ("dma");
			Order = dma.Order;
			Member = member;
			IsRequired = dma.IsRequired;
			XmlName = dma.Name != null ? dma.Name : member.Name;
			XmlNamespace = ns;
			XmlRootNamespace = rootNamespce;
			if (Member is FieldInfo)
				MemberType = ((FieldInfo) Member).FieldType;
			else
				MemberType = ((PropertyInfo) Member).PropertyType;
		}

		public class DataMemberInfoComparer : IComparer<DataMemberInfo>
			, IComparer // see bug #76361
		{
			public static readonly DataMemberInfoComparer Instance
				= new DataMemberInfoComparer ();

			private DataMemberInfoComparer () {}

			public int Compare (object o1, object o2)
			{
				return Compare ((DataMemberInfo) o1,
					(DataMemberInfo) o2);
			}

			public int Compare (DataMemberInfo d1, DataMemberInfo d2)
			{
				if (d1.Order == -1 || d2.Order == -1)
					return String.CompareOrdinal (d1.XmlName, d2.XmlName);

				return d1.Order - d2.Order;
			}
		}
	}
}
#endif
