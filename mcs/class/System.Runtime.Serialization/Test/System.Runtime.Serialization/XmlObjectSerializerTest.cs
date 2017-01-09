//
// XmlObjectSerializerTest.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//	Ankit Jain <JAnkit@novell.com>
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

//
// This test code contains tests for both DataContractSerializer and
// NetDataContractSerializer. The code could be mostly common.
//

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Net;
using System.Runtime.Serialization;
using System.Text;
using System.Xml;
using NUnit.Framework;
using System.Xml.Serialization;
using System.Xml.Schema;

namespace MonoTests.System.Runtime.Serialization
{
	[TestFixture]
	public class DataContractSerializerTest
	{
		static readonly XmlWriterSettings settings;

		static DataContractSerializerTest ()
		{
			settings = new XmlWriterSettings ();
			settings.OmitXmlDeclaration = true;
		}

		[DataContract]
		class Sample1
		{
			[DataMember]
			public string Member1;
		}

		[Test]
		[ExpectedException (typeof (ArgumentNullException))]
		public void ConstructorTypeNull ()
		{
			new DataContractSerializer (null);
		}

		[Test]
		public void ConstructorKnownTypesNull ()
		{
			// null knownTypes is allowed.
			new DataContractSerializer (typeof (Sample1), null);
			new DataContractSerializer (typeof (Sample1), "Foo", String.Empty, null);
			new DataContractSerializer (typeof (Sample1), new XmlDictionary ().Add ("Foo"), XmlDictionaryString.Empty, null);
		}

		[Test]
		[ExpectedException (typeof (ArgumentNullException))]
		public void ConstructorNameNull ()
		{
			new DataContractSerializer (typeof (Sample1), null, String.Empty);
		}

		[Test]
		[ExpectedException (typeof (ArgumentNullException))]
		public void ConstructorNamespaceNull ()
		{
			new DataContractSerializer (typeof (Sample1), "foo", null);
		}

		[Test]
		[ExpectedException (typeof (ArgumentOutOfRangeException))]
		public void ConstructorNegativeMaxObjects ()
		{
			new DataContractSerializer (typeof (Sample1), null,
				-1, false, false, null);
		}

		[Test]
		public void ConstructorMisc ()
		{
			new DataContractSerializer (typeof (GlobalSample1));
		}

		[Test]
		public void WriteObjectContent ()
		{
			StringWriter sw = new StringWriter ();
			using (XmlWriter xw = XmlWriter.Create (sw, settings)) {
				DataContractSerializer ser =
					new DataContractSerializer (typeof (string));
				xw.WriteStartElement ("my-element");
				ser.WriteObjectContent (xw, "TEST STRING");
				xw.WriteEndElement ();
			}
			Assert.AreEqual ("<my-element>TEST STRING</my-element>",
				sw.ToString ());
		}

		[Test]
		public void WriteObjectToStream ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (int));
			MemoryStream sw = new MemoryStream ();
			ser.WriteObject (sw, 1);
			string expected = "<int xmlns=\"http://schemas.microsoft.com/2003/10/Serialization/\">1</int>";
			byte[] buf = sw.ToArray ();
			Assert.AreEqual (expected, Encoding.UTF8.GetString (buf, 0, buf.Length));
		}

		[Test]
		public void ReadObjectFromStream ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (int));
			string expected = "<int xmlns=\"http://schemas.microsoft.com/2003/10/Serialization/\">1</int>";
			byte[] buf = Encoding.UTF8.GetBytes (expected);
			MemoryStream sw = new MemoryStream (buf);
			object res = ser.ReadObject (sw);
			Assert.AreEqual (1, res);
		}

		// int

		[Test]
		public void SerializeInt ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (int));
			SerializeInt (ser, "<int xmlns=\"http://schemas.microsoft.com/2003/10/Serialization/\">1</int>");
		}


		[Test]
		[Category ("NotWorking")]
		public void NetSerializeInt ()
		{
			NetDataContractSerializer ser =
				new NetDataContractSerializer ();
			// z:Assembly="0" ???
			SerializeInt (ser, String.Format ("<int z:Type=\"System.Int32\" z:Assembly=\"0\" xmlns:z=\"http://schemas.microsoft.com/2003/10/Serialization/\" xmlns=\"http://schemas.microsoft.com/2003/10/Serialization/\">1</int>", typeof (int).Assembly.FullName));
		}

		void SerializeInt (XmlObjectSerializer ser, string expected)
		{
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, 1);
			}
			Assert.AreEqual (expected, sw.ToString ());
		}

		// pass typeof(DCEmpty), serialize int

		[Test]
		public void SerializeIntForDCEmpty ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (DCEmpty));
			// tricky!
			SerializeIntForDCEmpty (ser, "<DCEmpty xmlns:d1p1=\"http://www.w3.org/2001/XMLSchema\" i:type=\"d1p1:int\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\">1</DCEmpty>");
		}

		void SerializeIntForDCEmpty (XmlObjectSerializer ser, string expected)
		{
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, 1);
			}
			XmlComparer.AssertAreEqual (expected, sw.ToString ());
		}

		// DCEmpty

		[Test]
		public void SerializeEmptyClass ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (DCEmpty));
			SerializeEmptyClass (ser, "<DCEmpty xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\" />");
		}

		[Test]
		[Category ("NotWorking")]
		public void NetSerializeEmptyClass ()
		{
			NetDataContractSerializer ser =
				new NetDataContractSerializer ();
			SerializeEmptyClass (ser, String.Format ("<DCEmpty xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" z:Id=\"1\" z:Type=\"MonoTests.System.Runtime.Serialization.DCEmpty\" z:Assembly=\"{0}\" xmlns:z=\"http://schemas.microsoft.com/2003/10/Serialization/\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\" />", this.GetType ().Assembly.FullName));
		}

		void SerializeEmptyClass (XmlObjectSerializer ser, string expected)
		{
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new DCEmpty ());
			}
			Assert.AreEqual (expected, sw.ToString ());
		}

		// string (primitive)

		[Test]
		public void SerializePrimitiveString ()
		{
			XmlObjectSerializer ser =
				new DataContractSerializer (typeof (string));
			SerializePrimitiveString (ser, "<string xmlns=\"http://schemas.microsoft.com/2003/10/Serialization/\">TEST</string>");
		}

		[Test]
		[Category ("NotWorking")]
		public void NetSerializePrimitiveString ()
		{
			XmlObjectSerializer ser = new NetDataContractSerializer ();
			SerializePrimitiveString (ser, "<string z:Type=\"System.String\" z:Assembly=\"0\" xmlns:z=\"http://schemas.microsoft.com/2003/10/Serialization/\" xmlns=\"http://schemas.microsoft.com/2003/10/Serialization/\">TEST</string>");
		}

		void SerializePrimitiveString (XmlObjectSerializer ser, string expected)
		{
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, "TEST");
			}
			Assert.AreEqual (expected, sw.ToString ());
		}

		// QName (primitive but ...)

		[Test]
		[Ignore ("These tests would not make any sense right now since it's populated prefix is not testable.")]
		public void SerializePrimitiveQName ()
		{
			XmlObjectSerializer ser =
				new DataContractSerializer (typeof (XmlQualifiedName));
			SerializePrimitiveQName (ser, "<z:QName xmlns:d7=\"urn:foo\" xmlns:z=\"http://schemas.microsoft.com/2003/10/Serialization/\">d7:foo</z:QName>");
		}

		[Test]
		[Ignore ("These tests would not make any sense right now since it's populated prefix is not testable.")]
		public void NetSerializePrimitiveQName ()
		{
			XmlObjectSerializer ser = new NetDataContractSerializer ();
			SerializePrimitiveQName (ser, "<z:QName z:Type=\"System.Xml.XmlQualifiedName\" z:Assembly=\"System.Xml, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089\" xmlns:d7=\"urn:foo\" xmlns:z=\"http://schemas.microsoft.com/2003/10/Serialization/\">d7:foo</z:QName>");
		}

		void SerializePrimitiveQName (XmlObjectSerializer ser, string expected)
		{
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new XmlQualifiedName ("foo", "urn:foo"));
			}
			Assert.AreEqual (expected, sw.ToString ());
		}

		// DCSimple1

		[Test]
		public void SerializeSimpleClass1 ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (DCSimple1));
			SerializeSimpleClass1 (ser, "<DCSimple1 xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\"><Foo>TEST</Foo></DCSimple1>");
		}

		[Test]
		public void SerializeSimpleXml ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (SimpleXml));
			SerializeSimpleClass1 (ser, @"<simple i:type=""d1p1:DCSimple1"" xmlns:d1p1=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"" xmlns:i=""http://www.w3.org/2001/XMLSchema-instance""><d1p1:Foo>TEST</d1p1:Foo></simple>");
		}

		[Test]
		[Category ("NotWorking")]
		public void NetSerializeSimpleClass1 ()
		{
			NetDataContractSerializer ser =
				new NetDataContractSerializer ();
			SerializeSimpleClass1 (ser, String.Format ("<DCSimple1 xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" z:Id=\"1\" z:Type=\"MonoTests.System.Runtime.Serialization.DCSimple1\" z:Assembly=\"{0}\" xmlns:z=\"http://schemas.microsoft.com/2003/10/Serialization/\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\"><Foo z:Id=\"2\">TEST</Foo></DCSimple1>", this.GetType ().Assembly.FullName));
		}

		void SerializeSimpleClass1 (XmlObjectSerializer ser, string expected)
		{
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new DCSimple1 ());
			}
			Console.WriteLine(sw.ToString());
			Assert.AreEqual (expected, sw.ToString ());
		}

		// NonDC

		[Test]
		// NonDC is not a DataContract type.
		public void SerializeNonDCOnlyCtor ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (NonDC));
		}

		[Test]
		[ExpectedException (typeof (InvalidDataContractException))]
		// NonDC is not a DataContract type.
		public void SerializeNonDC ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (NonDC));
			using (XmlWriter w = XmlWriter.Create (TextWriter.Null, settings)) {
				ser.WriteObject (w, new NonDC ());
			}
		}

		// DCHasNonDC

		[Test]
		[ExpectedException (typeof (InvalidDataContractException))]
		// DCHasNonDC itself is a DataContract type whose field is
		// marked as DataMember but its type is not DataContract.
		public void SerializeDCHasNonDC ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCHasNonDC));
			using (XmlWriter w = XmlWriter.Create (TextWriter.Null, settings)) {
				ser.WriteObject (w, new DCHasNonDC ());
			}
		}

		// DCHasSerializable

		[Test]
		// DCHasSerializable itself is DataContract and has a field
		// whose type is not contract but serializable.
		public void SerializeSimpleSerializable1 ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCHasSerializable));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new DCHasSerializable ());
			}
			Assert.AreEqual ("<DCHasSerializable xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\"><Ser><Doh>doh!</Doh></Ser></DCHasSerializable>", sw.ToString ());
		}

		[Test]
		public void SerializeDCWithName ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCWithName));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new DCWithName ());
			}
			Assert.AreEqual ("<Foo xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\"><FooMember>value</FooMember></Foo>", sw.ToString ());
		}

		[Test]
		public void SerializeDCWithEmptyName1 ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCWithEmptyName));
			StringWriter sw = new StringWriter ();
			DCWithEmptyName dc = new DCWithEmptyName ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				try {
					ser.WriteObject (w, dc);
				} catch (InvalidDataContractException) {
					return;
				}
			}
			Assert.Fail ("Expected InvalidDataContractException");
		}

		[Test]
		[Category ("NotWorking")]
		public void SerializeDCWithEmptyName2 ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCWithName));
			StringWriter sw = new StringWriter ();

			/* DataContractAttribute.Name == "", not valid */
			DCWithEmptyName dc = new DCWithEmptyName ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				try {
					ser.WriteObject (w, dc);
				} catch (InvalidDataContractException) {
					return;
				}
			}
			Assert.Fail ("Expected InvalidDataContractException");
		}

		[Test]
		[Category ("NotWorking")]
		public void SerializeDCWithNullName ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCWithNullName));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				try {
					/* DataContractAttribute.Name == "", not valid */
					ser.WriteObject (w, new DCWithNullName ());
				} catch (InvalidDataContractException) {
					return;
				}
			}
			Assert.Fail ("Expected InvalidDataContractException");
		}

		[Test]
		public void SerializeDCWithEmptyNamespace1 ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCWithEmptyNamespace));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new DCWithEmptyNamespace ());
			}
		}

		// Wrapper.DCWrapped

		[Test]
		public void SerializeWrappedClass ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (Wrapper.DCWrapped));
			SerializeWrappedClass (ser, "<Wrapper.DCWrapped xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\" />");
		}

		[Test]
		[Category ("NotWorking")]
		public void NetSerializeWrappedClass ()
		{
			NetDataContractSerializer ser =
				new NetDataContractSerializer ();
			SerializeWrappedClass (ser, String.Format ("<Wrapper.DCWrapped xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" z:Id=\"1\" z:Type=\"MonoTests.System.Runtime.Serialization.Wrapper+DCWrapped\" z:Assembly=\"{0}\" xmlns:z=\"http://schemas.microsoft.com/2003/10/Serialization/\" xmlns=\"http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization\" />", this.GetType ().Assembly.FullName));
		}

		void SerializeWrappedClass (XmlObjectSerializer ser, string expected)
		{
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new Wrapper.DCWrapped ());
			}
			Assert.AreEqual (expected, sw.ToString ());
		}

		// CollectionContainer : Items must have a setter.
		[Test]
		[ExpectedException (typeof (InvalidDataContractException))]
		[Category ("NotWorking")]
		public void SerializeReadOnlyCollectionMember ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (CollectionContainer));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, null);
			}
		}

		// DataCollectionContainer : Items must have a setter.
		[Test]
		[ExpectedException (typeof (InvalidDataContractException))]
		[Category ("NotWorking")]
		public void SerializeReadOnlyDataCollectionMember ()
		{
			DataContractSerializer ser =
				new DataContractSerializer (typeof (DataCollectionContainer));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, null);
			}
		}

		[Test]
		public void SerializeGuid ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (Guid));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, Guid.Empty);
			}
			Assert.AreEqual (
				"<guid xmlns=\"http://schemas.microsoft.com/2003/10/Serialization/\">00000000-0000-0000-0000-000000000000</guid>",
				sw.ToString ());
		}

		[Test]
		public void SerializeEnum ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (Colors));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new Colors ());
			}

			Assert.AreEqual (
				@"<Colors xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">Red</Colors>",
				sw.ToString ());
		}

		[Test]
		public void SerializeEnum2 ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (Colors));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, 0);
			}

			XmlComparer.AssertAreEqual (
				@"<Colors xmlns:d1p1=""http://www.w3.org/2001/XMLSchema"" i:type=""d1p1:int"" xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">0</Colors>",
				sw.ToString ());
		}

		[Test]
		public void SerializeEnumWithDC ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (ColorsWithDC));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new ColorsWithDC ());
			}

			Assert.AreEqual (
				@"<_ColorsWithDC xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">_Red</_ColorsWithDC>",
				sw.ToString ());
		}

		[Test]
		public void SerializeEnumWithNoDC ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (ColorsEnumMemberNoDC));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new ColorsEnumMemberNoDC ());
			}

			Assert.AreEqual (
				@"<ColorsEnumMemberNoDC xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">Red</ColorsEnumMemberNoDC>",
				sw.ToString ());
		}

		[Test]
		public void SerializeEnumWithDC2 ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (ColorsWithDC));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, 3);
			}

			XmlComparer.AssertAreEqual (
				@"<_ColorsWithDC xmlns:d1p1=""http://www.w3.org/2001/XMLSchema"" i:type=""d1p1:int"" xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">3</_ColorsWithDC>",
				sw.ToString ());
		}

		[Test]
		[ExpectedException (typeof (SerializationException))]
		public void SerializeEnumWithDCInvalid ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (ColorsWithDC));
			StringWriter sw = new StringWriter ();
			ColorsWithDC cdc = ColorsWithDC.Blue;
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, cdc);
			}
		}

		[Test]
		public void SerializeDCWithEnum ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCWithEnum));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, new DCWithEnum ());
			}
 
			Assert.AreEqual (
				@"<DCWithEnum xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><_colors>Red</_colors></DCWithEnum>",
				sw.ToString ());
		}

		[Test]
		public void SerializeDCWithTwoEnums ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCWithTwoEnums));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				DCWithTwoEnums e = new DCWithTwoEnums ();
				e.colors = Colors.Blue;
				e.colors2 = Colors.Green;
				ser.WriteObject (w, e);
			}
 
			Assert.AreEqual (
				@"<DCWithTwoEnums xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><colors>Blue</colors><colors2>Green</colors2></DCWithTwoEnums>",
				sw.ToString ());
		}

		[Test]
		public void SerializeNestingDC2 ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (NestingDC2));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				NestingDC2 e = new NestingDC2 ();
				e.Field = new NestedDC2 ("Something");
				ser.WriteObject (w, e);
			}
 
			Assert.AreEqual (
				@"<NestingDC2 xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""test2""><Field xmlns:d2p1=""test1""><d2p1:Name>Something</d2p1:Name></Field></NestingDC2>",
				sw.ToString ());
		}

		[Test]
		public void SerializeNestingDC ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (NestingDC));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				NestingDC e = new NestingDC ();
				e.Field1 = new NestedDC ("test1");
				e.Field2 = new NestedDC ("test2");
				ser.WriteObject (w, e);
			}
 
			Assert.AreEqual (
				@"<NestingDC xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><Field1><Name>test1</Name></Field1><Field2><Name>test2</Name></Field2></NestingDC>",
				sw.ToString ());
			sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				NestingDC e = new NestingDC ();
				ser.WriteObject (w, e);
			}
 
			Assert.AreEqual (
				@"<NestingDC xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><Field1 i:nil=""true"" /><Field2 i:nil=""true"" /></NestingDC>",
				sw.ToString ());
		}

		[Test]
		public void SerializeDerivedDC ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DerivedDC));
			StringWriter sw = new StringWriter ();
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				DerivedDC e = new DerivedDC ();
				ser.WriteObject (w, e);
			}
 
			Assert.AreEqual (
				@"<DerivedDC xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""Derived""><baseVal xmlns=""Base"">0</baseVal><derivedVal>0</derivedVal></DerivedDC>",
				sw.ToString ());
		}

		[Test]
		public void SerializerDCArray ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (DCWithEnum []));
			StringWriter sw = new StringWriter ();
			DCWithEnum [] arr = new DCWithEnum [2];
			arr [0] = new DCWithEnum (); arr [0].colors = Colors.Red;
			arr [1] = new DCWithEnum (); arr [1].colors = Colors.Green;
			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, arr);
			}

			XmlComparer.AssertAreEqual (
				@"<ArrayOfDCWithEnum xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><DCWithEnum><_colors>Red</_colors></DCWithEnum><DCWithEnum><_colors>Green</_colors></DCWithEnum></ArrayOfDCWithEnum>",
				sw.ToString ());
		}

		[Test]
		public void SerializerDCArray2 ()
		{
			List<Type> known = new List<Type> ();
			known.Add (typeof (DCWithEnum));
			known.Add (typeof (DCSimple1));
			DataContractSerializer ser = new DataContractSerializer (typeof (object []), known);
			StringWriter sw = new StringWriter ();
			object [] arr = new object [2];
			arr [0] = new DCWithEnum (); ((DCWithEnum)arr [0]).colors = Colors.Red;
			arr [1] = new DCSimple1 (); ((DCSimple1) arr [1]).Foo = "hello";

			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, arr);
			}

			XmlComparer.AssertAreEqual (
				@"<ArrayOfanyType xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.microsoft.com/2003/10/Serialization/Arrays""><anyType xmlns:d2p1=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"" i:type=""d2p1:DCWithEnum""><d2p1:_colors>Red</d2p1:_colors></anyType><anyType xmlns:d2p1=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"" i:type=""d2p1:DCSimple1""><d2p1:Foo>hello</d2p1:Foo></anyType></ArrayOfanyType>",
				sw.ToString ());
		}

		[Test]
		public void SerializerDCArray3 ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (int []));
			StringWriter sw = new StringWriter ();
			int [] arr = new int [2];
			arr [0] = 1; arr [1] = 2;

			using (XmlWriter w = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (w, arr);
			}

			XmlComparer.AssertAreEqual (
				@"<ArrayOfint xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.microsoft.com/2003/10/Serialization/Arrays""><int>1</int><int>2</int></ArrayOfint>",
				sw.ToString ());
		}

		[Test]
		public void SerializeNonDCArray ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (SerializeNonDCArrayType));
			StringWriter sw = new StringWriter ();
			using (XmlWriter xw = XmlWriter.Create (sw, settings)) {
				ser.WriteObject (xw, new SerializeNonDCArrayType ());
			}
			Assert.AreEqual (@"<SerializeNonDCArrayType xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><IPAddresses xmlns:d2p1=""http://schemas.datacontract.org/2004/07/System.Net"" /></SerializeNonDCArrayType>",
				sw.ToString ());
		}

		[Test]
		public void SerializeNonDCArrayItems ()
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (SerializeNonDCArrayType));
			StringWriter sw = new StringWriter ();
			using (XmlWriter xw = XmlWriter.Create (sw, settings)) {
				SerializeNonDCArrayType obj = new SerializeNonDCArrayType ();
				obj.IPAddresses = new IPAddress [] {new IPAddress (new byte [] {1, 2, 3, 4})};
				ser.WriteObject (xw, obj);
			}

			XmlDocument doc = new XmlDocument ();
			doc.LoadXml (sw.ToString ());
			XmlNamespaceManager nsmgr = new XmlNamespaceManager (doc.NameTable);
			nsmgr.AddNamespace ("s", "http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization");
			nsmgr.AddNamespace ("n", "http://schemas.datacontract.org/2004/07/System.Net");
			nsmgr.AddNamespace ("a", "http://schemas.microsoft.com/2003/10/Serialization/Arrays");

			Assert.AreEqual (1, doc.SelectNodes ("/s:SerializeNonDCArrayType/s:IPAddresses/n:IPAddress", nsmgr).Count, "#1");
			Assert.AreEqual ("67305985", doc.SelectSingleNode ("/s:SerializeNonDCArrayType/s:IPAddresses/n:IPAddress/n:m_Address", nsmgr).InnerText, "#2");
			XmlElement el = doc.SelectSingleNode ("/s:SerializeNonDCArrayType/s:IPAddresses/n:IPAddress/n:m_Numbers", nsmgr) as XmlElement;
			Assert.IsNotNull (el, "#3");
			Assert.AreEqual (8, el.SelectNodes ("a:unsignedShort", nsmgr).Count, "#4");
		}

		[Test]
		public void DeserializeEnum ()
		{
			Colors c = Deserialize<Colors> (
				@"<Colors xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">Red</Colors>");

			Assert.AreEqual (Colors.Red, c, "#de2");
		}

		[Test]
		public void DeserializeEnum2 ()
		{
			Colors c = Deserialize<Colors> (
				@"<Colors xmlns:d1p1=""http://www.w3.org/2001/XMLSchema"" i:type=""d1p1:int"" xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">1</Colors>",
				typeof (int));

			Assert.AreEqual (Colors.Green, c, "#de4");
		}
		
		[Test]
		[ExpectedException (typeof (SerializationException))]
		public void DeserializeEnumInvalid1 ()
		{
			Deserialize<Colors> (
				@"<Colors xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""></Colors>");
		}

		[Test]
		[ExpectedException (typeof (SerializationException))]
		public void DeserializeEnumInvalid2 ()
		{
			Deserialize<Colors> (
				@"<Colors xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""/>");
		}

		[Test]
		[ExpectedException (typeof (SerializationException))]
		public void DeserializeEnumInvalid3 ()
		{
			//"red" instead of "Red"
			Deserialize<Colors> (
				@"<Colors xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">red</Colors>");
		}

		[Test]
		public void DeserializeEnumWithDC ()
		{
			ColorsWithDC cdc = Deserialize<ColorsWithDC> (
				@"<_ColorsWithDC xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">_Red</_ColorsWithDC>");
			
			Assert.AreEqual (ColorsWithDC.Red, cdc, "#de6");
		}

		[Test]
		[ExpectedException (typeof (SerializationException))]
		public void DeserializeEnumWithDCInvalid ()
		{
			Deserialize<ColorsWithDC> (
				@"<_ColorsWithDC xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">NonExistant</_ColorsWithDC>");
		}

		[Test]
		public void DeserializeDCWithEnum ()
		{
			DCWithEnum dc = Deserialize<DCWithEnum> (
				@"<DCWithEnum xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><_colors>Red</_colors></DCWithEnum>");

			Assert.AreEqual (Colors.Red, dc.colors, "#de8");
		}

		[Test]
		public void DeserializeNestingDC ()
		{
			NestingDC dc = Deserialize<NestingDC> (
				@"<NestingDC xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><Field1><Name>test1</Name></Field1><Field2><Name>test2</Name></Field2></NestingDC>");

			Assert.IsNotNull (dc.Field1, "#N1: Field1 should not be null.");
			Assert.IsNotNull (dc.Field2, "#N2: Field2 should not be null.");
			Assert.AreEqual ("test1", dc.Field1.Name, "#1");
			Assert.AreEqual ("test2", dc.Field2.Name, "#2");
		}

		[Test]
		public void DeserializeNestingDC2 ()
		{
			NestingDC2 dc = Deserialize<NestingDC2> (
				@"<NestingDC2 xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""test2""><Field xmlns:d2p1=""test1""><d2p1:Name>Something</d2p1:Name></Field></NestingDC2>");

			Assert.IsNotNull (dc.Field, "#N1: Field should not be null.");
			Assert.AreEqual ("Something", dc.Field.Name, "#N2");
		}

		[Test]
		public void DeserializeDerivedDC ()
		{
			DerivedDC dc = Deserialize<DerivedDC> (
				@"<DerivedDC xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""Derived""><baseVal xmlns=""Base"">1</baseVal><derivedVal>2</derivedVal></DerivedDC>");

			Assert.AreEqual (1, dc.baseVal, "#N1");
			Assert.AreEqual (2, dc.derivedVal, "#N2");
		}

		[Test]
		public void DeserializeTwice ()
		{
			string xml = 
				@"<any><_ColorsWithDC xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">_Red</_ColorsWithDC> <_ColorsWithDC xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">_Red</_ColorsWithDC></any>";
			DataContractSerializer ser = new DataContractSerializer (typeof (ColorsWithDC));
			XmlReader xr = XmlReader.Create (new StringReader (xml), new XmlReaderSettings ());
			xr.ReadStartElement ();
			object o = ser.ReadObject (xr);
			Assert.AreEqual (typeof (ColorsWithDC), o.GetType (), "#de5");
			ColorsWithDC cdc = (ColorsWithDC) o;
			Assert.AreEqual (ColorsWithDC.Red, o, "#de6");

			o = ser.ReadObject (xr);
			Assert.AreEqual (typeof (ColorsWithDC), o.GetType (), "#de5");
			cdc = (ColorsWithDC) o;
			Assert.AreEqual (ColorsWithDC.Red, o, "#de6");
			Assert.AreEqual (XmlNodeType.EndElement, xr.NodeType, "#de6");
			Assert.AreEqual ("any", xr.LocalName, "#de6");
			xr.ReadEndElement ();
		}


		[Test]
		public void DeserializeEmptyNestingDC ()
		{
			NestingDC dc = Deserialize<NestingDC> (
				@"<NestingDC xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""></NestingDC>");

			Assert.IsNotNull (dc, "#A0: The object should not be null.");
			Assert.IsNull (dc.Field1, "#A1: Field1 should be null.");
			Assert.IsNull (dc.Field2, "#A2: Field2 should be null.");

			dc = Deserialize<NestingDC> (
				@"<NestingDC xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""/>");

			Assert.IsNotNull (dc, "#B0: The object should not be null.");
			Assert.IsNull (dc.Field1, "#B1: Field1 should be null.");
			Assert.IsNull (dc.Field2, "#B2: Field2 should be null.");

			dc = Deserialize<NestingDC> (
				@"<NestingDC xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><Field1 i:nil=""true"" /><Field2 i:nil=""true"" /></NestingDC>");

			Assert.IsNotNull (dc, "#B0: The object should not be null.");
			Assert.IsNull (dc.Field1, "#B1: Field1 should be null.");
			Assert.IsNull (dc.Field2, "#B2: Field2 should be null.");
		}

		[Test]
		[ExpectedException (typeof (SerializationException))]
		public void DeserializeEmptyDCWithTwoEnums ()
		{
			Deserialize<DCWithTwoEnums> (
				@"<DCWithTwoEnums xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><colors i:nil=""true""/><colors2 i:nil=""true""/></DCWithTwoEnums>");
		}

		[Test]
		[Category ("NotWorking")]
		public void DeserializeDCWithNullableEnum ()
		{
			DCWithNullableEnum dc = Deserialize<DCWithNullableEnum> (
				@"<DCWithNullableEnum xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><colors i:nil=""true""/></DCWithNullableEnum>");

			Assert.IsNull (dc.colors, "#B1: Field should be null.");
		}

		[Test]
		public void DeserializeDCWithTwoEnums ()
		{
			DCWithTwoEnums dc = Deserialize<DCWithTwoEnums> (
				@"<DCWithTwoEnums xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><colors>Blue</colors><colors2>Green</colors2></DCWithTwoEnums>");

			Assert.AreEqual (Colors.Blue, dc.colors, "#0");
			Assert.AreEqual (Colors.Green, dc.colors2, "#1");
		}

		[Test]
		public void DeserializerDCArray ()
		{
			DCWithEnum [] dcArray = Deserialize<DCWithEnum []> (
				@"<ArrayOfDCWithEnum xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><DCWithEnum><_colors>Red</_colors></DCWithEnum><DCWithEnum><_colors>Green</_colors></DCWithEnum></ArrayOfDCWithEnum>");

			Assert.AreEqual (2, dcArray.Length, "#N1");
			Assert.AreEqual (Colors.Red, dcArray [0].colors, "#N2");
			Assert.AreEqual (Colors.Green, dcArray [1].colors, "#N3");
		}

		[Test]
		public void DeserializerDCArray2 ()
		{
			string xml = 
				@"<ArrayOfanyType xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.microsoft.com/2003/10/Serialization/Arrays""><anyType xmlns:d2p1=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"" i:type=""d2p1:DCWithEnum""><d2p1:_colors>Red</d2p1:_colors></anyType><anyType xmlns:d2p1=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"" i:type=""d2p1:DCSimple1""><d2p1:Foo>hello</d2p1:Foo></anyType></ArrayOfanyType>";

			List<Type> known = new List<Type> ();
			known.Add (typeof (DCWithEnum));
			known.Add (typeof (DCSimple1));
			DataContractSerializer ser = new DataContractSerializer (typeof (object []), known);
			XmlReader xr = XmlReader.Create (new StringReader (xml));

			object [] dc = (object []) ser.ReadObject (xr);
			Assert.AreEqual (2, dc.Length, "#N1");
			Assert.AreEqual (typeof (DCWithEnum), dc [0].GetType (), "#N2");
			DCWithEnum dc0 = (DCWithEnum) dc [0];
			Assert.AreEqual (Colors.Red, dc0.colors, "#N3");
			Assert.AreEqual (typeof (DCSimple1), dc [1].GetType (), "#N4");
			DCSimple1 dc1 = (DCSimple1) dc [1];
			Assert.AreEqual ("hello", dc1.Foo, "#N4");
		}

		[Test]
		public void DeserializerDCArray3 ()
		{
			int [] intArray = Deserialize<int []> (
				@"<ArrayOfint xmlns:i=""http://www.w3.org/2001/XMLSchema-instance"" xmlns=""http://schemas.microsoft.com/2003/10/Serialization/Arrays""><int>1</int><int>2</int></ArrayOfint>");

			Assert.AreEqual (2, intArray.Length, "#N0");
			Assert.AreEqual (1, intArray [0], "#N1");
			Assert.AreEqual (2, intArray [1], "#N2");
		}

		[Test]
		public void ReadObjectNoVerifyObjectName ()
		{
			string xml = @"<any><Member1 xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization1"">bar1</Member1><Member1 xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization2"">bar2</Member1><Member1 xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization"">bar</Member1></any>";
			VerifyObjectNameTestData res = (VerifyObjectNameTestData)new DataContractSerializer (typeof (VerifyObjectNameTestData))
				.ReadObject (XmlReader.Create (new StringReader (xml)), false);
			Assert.AreEqual ("bar", res.GetMember());
		}

		[Test]
		public void ReadObjectVerifyObjectName ()
		{
			string xml = @"<VerifyObjectNameTestData xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization""><Member1>bar</Member1></VerifyObjectNameTestData>";
			VerifyObjectNameTestData res = (VerifyObjectNameTestData)new DataContractSerializer (typeof (VerifyObjectNameTestData))
				.ReadObject (XmlReader.Create (new StringReader (xml)));
			Assert.AreEqual ("bar", res.GetMember());
		}

		[Test]
		[ExpectedException (typeof (SerializationException))]
		public void ReadObjectWrongNamespace ()
		{
			string xml = @"<VerifyObjectNameTestData xmlns=""http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization2""><Member1>bar</Member1></VerifyObjectNameTestData>";
			new DataContractSerializer (typeof (VerifyObjectNameTestData))
				.ReadObject (XmlReader.Create (new StringReader (xml)));
		}

		[Test]
		public void ReferenceSerialization ()
		{
			var dc = new DataContractSerializer (typeof (ReferenceWrapper));
			var t = new ReferenceType ();
			StringWriter sw = new StringWriter ();
			using (var xw = XmlWriter.Create (sw)) {
				xw.WriteStartElement ("z", "root", "http://schemas.microsoft.com/2003/10/Serialization/");
				dc.WriteObject (xw, new ReferenceWrapper () {T = t, T2 = t});
				xw.WriteEndElement ();
			}
			string xml = @"<?xml version='1.0' encoding='utf-16'?><z:root xmlns:z='http://schemas.microsoft.com/2003/10/Serialization/'><ReferenceWrapper xmlns:i='http://www.w3.org/2001/XMLSchema-instance' xmlns='http://schemas.datacontract.org/2004/07/MonoTests.System.Runtime.Serialization'><T z:Id='i1'><F>x</F></T><T2 z:Ref='i1' /></ReferenceWrapper></z:root>";
			Assert.AreEqual (xml.Replace ('\'', '"'), sw.ToString (), "#1");

			ReferenceWrapper w;
			using (XmlReader r = XmlReader.Create (new StringReader (xml)))
	{
				r.ReadStartElement ();
				w = (ReferenceWrapper) dc.ReadObject (r);
				r.ReadEndElement ();
			}
			Assert.AreEqual (w.T, w.T2, "#2");
		}

		private T Deserialize<T> (string xml)
		{
			return Deserialize<T> (xml, typeof (T));
		}

		private T Deserialize<T> (string xml, Type runtimeType)
		{
			DataContractSerializer ser = new DataContractSerializer (typeof (T));
			XmlReader xr = XmlReader.Create (new StringReader (xml), new XmlReaderSettings ());
			object o = ser.ReadObject (xr);
			Assert.AreEqual (runtimeType, o.GetType (), "#DS0");
			return (T)o;
		}
 	}
 
	public enum Colors {
		Red, Green, Blue
	}

	[DataContract (Name = "_ColorsWithDC")]
	public enum ColorsWithDC {

		[EnumMember (Value = "_Red")]
		Red, 
		[EnumMember]
		Green, 
		Blue
	}


	public enum ColorsEnumMemberNoDC {
		[EnumMember (Value = "_Red")]
		Red, 
		[EnumMember]
		Green, 
		Blue
	}

 	[DataContract]
	public class DCWithEnum {
		[DataMember (Name = "_colors")]
		public Colors colors;
	}

 	[DataContract]
	public class DCWithTwoEnums {
		[DataMember]
		public Colors colors;
		[DataMember]
		public Colors colors2;
	}

 	[DataContract]
	public class DCWithNullableEnum {
		[DataMember]
		public Colors? colors;
	}


	[DataContract (Namespace = "Base")]
	public class BaseDC {
		[DataMember]
		public int baseVal;
	}

	[DataContract (Namespace = "Derived")]
	public class DerivedDC : BaseDC {
		[DataMember]
		public int derivedVal;
	}

 	[DataContract]
	public class NestedDC {
		public NestedDC (string name) { this.Name = name; }

		[DataMember]
		public string Name;
	}

 	[DataContract]
	public class NestingDC {
		[DataMember]
		public NestedDC Field1;
		[DataMember]
		public NestedDC Field2;
	}

 	[DataContract (Namespace = "test1")]
	public class NestedDC2 {
		public NestedDC2 (string name) { this.Name = name; }

		[DataMember]
		public string Name;
	}

 	[DataContract (Namespace = "test2")]
	public class NestingDC2 {
		[DataMember]
		public NestedDC2 Field;
	}

	[DataContract]
	public class DCEmpty
	{
		// serializer doesn't touch it.
		public string Foo = "TEST";
	}

	[DataContract]
	public class DCSimple1
	{
		[DataMember]
		public string Foo = "TEST";
	}

	[DataContract]
	public class DCHasNonDC
	{
		[DataMember]
		public NonDC Hoge= new NonDC ();
	}

	public class NonDC
	{
		public string Whee = "whee!";
	}

	[DataContract]
	public class DCHasSerializable
	{
		[DataMember]
		public SimpleSer1 Ser = new SimpleSer1 ();
	}

	[DataContract (Name = "Foo")]
	public class DCWithName
	{
		[DataMember (Name = "FooMember")]
		public string DMWithName = "value";
	}

	[DataContract (Name = "")]
	public class DCWithEmptyName
	{
	}

	[DataContract (Name = null)]
	public class DCWithNullName
	{
	}

	[DataContract (Namespace = "")]
	public class DCWithEmptyNamespace
	{
	}

	[Serializable]
	public class SimpleSer1
	{
		public string Doh = "doh!";
	}

	public class Wrapper
	{
		[DataContract]
		public class DCWrapped
		{
		}
	}

	[DataContract]
	public class CollectionContainer
	{
		Collection<string> items = new Collection<string> ();

		[DataMember]
		public Collection<string> Items {
			get { return items; }
		}
	}

	[CollectionDataContract]
	public class DataCollection<T> : Collection<T>
	{
	}

	[DataContract]
	public class DataCollectionContainer
	{
		DataCollection<string> items = new DataCollection<string> ();

		[DataMember]
		public DataCollection<string> Items {
			get { return items; }
		}
	}

	[DataContract]
	class SerializeNonDCArrayType
	{
		[DataMember]
		public IPAddress [] IPAddresses = new IPAddress [0];
	}

	[DataContract]
	public class VerifyObjectNameTestData
	{
		[DataMember]
		string Member1 = "foo";

		public string GetMember() { return Member1; }
	}

	[XmlRoot(ElementName = "simple", Namespace = "")]
	public class SimpleXml : IXmlSerializable 
	{
		void IXmlSerializable.ReadXml (XmlReader reader)
		{
		}

		void IXmlSerializable.WriteXml (XmlWriter writer)
		{
		}

		XmlSchema IXmlSerializable.GetSchema ()
		{
			return null;
		}

	}

	[DataContract]
	public class ReferenceWrapper
	{
	        [DataMember (Order = 1)]
	        public ReferenceType T;

	        [DataMember (Order = 2)]
	        public ReferenceType T2;
	}

	[DataContract (IsReference = true)]
	public class ReferenceType
	{
		[DataMember]
		public string F = "x";
	}
}

[DataContract]
class GlobalSample1
{
}
