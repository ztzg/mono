//
// SqlGuidTest.cs - NUnit Test Cases for System.Data.SqlTypes.SqlGuid
//
// Authors:
//   Ville Palo (vi64pa@koti.soon.fi)
//   Martin Willemoes Hansen (mwh@sysrq.dk)
//
// (C) 2002 Ville Palo
// (C) 2003 Martin Willemoes Hansen
// 

//
// Copyright (C) 2004 Novell, Inc (http://www.novell.com)
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

using NUnit.Framework;
using System;
using System.Xml;
using System.Data.SqlTypes;

namespace MonoTests.System.Data.SqlTypes
{
	[TestFixture]
        public class SqlGuidTest : Assertion {

		// 00000a01-0000-0000-0000-000000000000
		private SqlGuid Test1;

        	// 00000f64-0000-0000-0000-000000000000
        	private SqlGuid Test2;         	
        	private SqlGuid Test3;

		// 0000fafa-0000-0000-0000-000000000000
		private SqlGuid Test4;
        	
		[SetUp]
                public void GetReady() 
                {
                	byte [] b1 = new byte [16];
                	byte [] b2 = new byte [16];
                	byte [] b3 = new byte [16];
                	byte [] b4 = new byte [16];

                	b1 [0] = 1;
                	b1 [1] = 10;
                	b2 [0] = 100;
                	b2 [1] = 15;
                	b3 [0] = 100;
                	b3 [1] = 15;
                	b4 [0] = 250;
                	b4 [1] = 250;

                   	Test1 = new SqlGuid (b1);
			Test2 = new SqlGuid (b2);
                	Test3 = new SqlGuid (b3);
                	Test4 = new SqlGuid (b4);
                }

                // Test constructor
		[Test]
                public void Create()
                {
			// SqlGuid (Byte[])
			byte [] b = new byte [16];
                	b [0] = 100;
                	b [1] = 200;

                	try {
                		SqlGuid Test = new SqlGuid (b);

				// SqlGuid (Guid)
				Guid TestGuid = new Guid (b);
                		Test = new SqlGuid (TestGuid);

				// SqlGuid (string)
				Test = new SqlGuid ("12345678-1234-1234-1234-123456789012");

				// SqlGuid (int, short, short, byte, byte, byte, byte, byte, byte, byte, byte)
                		Test = new SqlGuid (10, 1, 2, 13, 14, 15, 16, 17, 19, 20 ,21);

                	} catch (Exception e) {
                		Fail ("#A01 " + e);
                	}
                }

                // Test public fields
		[Test]
                public void PublicFields()
                {
                        Assert ("#B01", SqlGuid.Null.IsNull);
                }

                // Test properties
		[Test]
                public void Properties()
                {
                	Guid ResultGuid = new Guid ("00000f64-0000-0000-0000-000000000000");		       
			Assert ("#C01", !Test1.IsNull);
                	Assert ("#C02", SqlGuid.Null.IsNull);
                	AssertEquals ("#C03", ResultGuid, Test2.Value);
                }

                // PUBLIC METHODS
		[Test]
                public void CompareTo()
                {
			String TestString = "This is a test string";
			SqlGuid test1 = new SqlGuid("1AAAAAAA-BBBB-CCCC-DDDD-3EEEEEEEEEEE");
			SqlGuid test2 = new SqlGuid("1AAAAAAA-BBBB-CCCC-DDDD-2EEEEEEEEEEE");
			SqlGuid test3 = new SqlGuid("1AAAAAAA-BBBB-CCCC-DDDD-1EEEEEEEEEEE");
                        Assert ("#D01", Test1.CompareTo (Test3) <  0);
                        Assert ("#D02", Test4.CompareTo (Test1) > 0);
                        Assert ("#D03", Test3.CompareTo (Test2) == 0);
                        Assert ("#D04", Test4.CompareTo (SqlGuid.Null) > 0);
			Assert ("#D05", test1.CompareTo (test2) >  0);
			Assert ("#D06", test3.CompareTo (test2) <  0);
			
                        try {
                                Test1.CompareTo (TestString);
                                Fail("#D05");
                        } catch(Exception e) {
                                AssertEquals ("#D06", typeof (ArgumentException), e.GetType ());
                        }
                }

		[Test]
                public void EqualsMethods()
                {
                        Assert ("#E01", !Test1.Equals (Test2));
                        Assert ("#E02", !Test2.Equals (Test4));
                        Assert ("#E03", !Test2.Equals (new SqlString ("TEST")));
                        Assert ("#E04", Test2.Equals (Test3));

                        // Static Equals()-method
                        Assert ("#E05", SqlGuid.Equals (Test2, Test3).Value);
                        Assert ("#E06", !SqlGuid.Equals (Test1, Test2).Value);
                }

		[Test]
                public void GetHashCodeTest()
                {
                        AssertEquals ("#F01", Test1.GetHashCode (), Test1.GetHashCode ());
                	Assert ("#F02", Test1.GetHashCode () != Test2.GetHashCode ());
                        AssertEquals ("#F02", Test3.GetHashCode (), Test2.GetHashCode ());
                }

		[Test]
                public void GetTypeTest()
                {
                        AssertEquals ("#G01", "System.Data.SqlTypes.SqlGuid", Test1.GetType ().ToString ());
                        AssertEquals ("#G02", "System.Guid", Test3.Value.GetType ().ToString ());
                }

		[Test]
                public void Greaters()
                {
                        // GreateThan ()
                        Assert ("#H01", !SqlGuid.GreaterThan (Test1, Test2).Value);
                        Assert ("#H02", SqlGuid.GreaterThan (Test2, Test1).Value);
                        Assert ("#H03", !SqlGuid.GreaterThan (Test2, Test3).Value);
                        // GreaterTharOrEqual ()
                        Assert ("#H04", !SqlGuid.GreaterThanOrEqual (Test1, Test2).Value);
                        Assert ("#H05", SqlGuid.GreaterThanOrEqual (Test2, Test1).Value);
                        Assert ("#H06", SqlGuid.GreaterThanOrEqual (Test2, Test3).Value);
                }

		[Test]
                public void Lessers()
                {
                        // LessThan()
                        Assert ("#I01", !SqlGuid.LessThan (Test2, Test3).Value);
                        Assert ("#I02", !SqlGuid.LessThan (Test2, Test1).Value);
                        Assert ("#I03", SqlGuid.LessThan (Test1, Test2).Value);

                        // LessThanOrEqual ()
                        Assert ("#I04", SqlGuid.LessThanOrEqual (Test1, Test2).Value);
                        Assert ("#I05", !SqlGuid.LessThanOrEqual (Test2, Test1).Value);
                        Assert ("#I06", SqlGuid.LessThanOrEqual (Test2, Test3).Value);
                        Assert ("#I07", SqlGuid.LessThanOrEqual (Test4, SqlGuid.Null).IsNull);
                }

		[Test]
                public void NotEquals()
                {
                        Assert ("#J01", SqlGuid.NotEquals (Test1, Test2).Value);
                        Assert ("#J02", SqlGuid.NotEquals (Test2, Test1).Value);
                        Assert ("#J03", SqlGuid.NotEquals (Test3, Test1).Value);
                        Assert ("#J04", !SqlGuid.NotEquals (Test3, Test2).Value);                      
                        Assert ("#J05", SqlGuid.NotEquals (SqlGuid.Null, Test2).IsNull);
                }

		[Test]
                public void Parse()
                {
                        try {
                                SqlGuid.Parse (null);
                                Fail ("#K01");
                        } catch (Exception e) {
                                AssertEquals ("#K02", typeof (ArgumentNullException), e.GetType ());
                        }

                        try {
                                SqlGuid.Parse ("not-a-number");
                                Fail ("#K03");
                        } catch (Exception e) {
                                AssertEquals ("#K04", typeof (FormatException), e.GetType ());
                        }

                         try {
                                SqlGuid.Parse ("9e400");
                                Fail ("#K05");
                        } catch (Exception e) {
                                AssertEquals ("#K06", typeof (FormatException), e.GetType ());
                        }

                        AssertEquals("#K07", new Guid("87654321-0000-0000-0000-000000000000"), 
                                     SqlGuid.Parse ("87654321-0000-0000-0000-000000000000").Value);
                }

		[Test]
                public void Conversions()
                {
			// ToByteArray ()
			AssertEquals ("#L01", (byte)1, Test1.ToByteArray () [0]);
			AssertEquals ("#L02", (byte)15, Test2.ToByteArray () [1]);

			// ToSqlBinary ()
			byte [] b = new byte [2]; 
                	b [0] = 100;
                	b [1] = 15;
		       
                        AssertEquals ("#L03", new SqlBinary (b), Test3.ToSqlBinary ());

                        // ToSqlString ()
                        AssertEquals ("#L04", "00000a01-0000-0000-0000-000000000000",  
				      Test1.ToSqlString ().Value);
                        AssertEquals ("#L05", "0000fafa-0000-0000-0000-000000000000", 
                                      Test4.ToSqlString ().Value);

                        // ToString ()
                        AssertEquals ("#L06", "00000a01-0000-0000-0000-000000000000", 
                                      Test1.ToString ());
                        AssertEquals ("#L07", "0000fafa-0000-0000-0000-000000000000", 
                                      Test4.ToString ());
                }

                // OPERATORS

		[Test]
                public void ThanOrEqualOperators()
                {
                        // == -operator
                        Assert ("#M01", (Test3 == Test2).Value);
                        Assert ("#M02", !(Test1 == Test2).Value);
                        Assert ("#M03", (Test1 == SqlGuid.Null).IsNull);
                        
                        // != -operator
                        Assert ("#M04", !(Test2 != Test3).Value);
                        Assert ("#M05", (Test1 != Test3).Value);
                        Assert ("#M06", (Test1 != SqlGuid.Null).IsNull);

                        // > -operator
                        Assert ("#M07", (Test2 > Test1).Value);
                        Assert ("#M08", !(Test1 > Test3).Value);
                        Assert ("#M09", !(Test3 > Test2).Value);
                        Assert ("#M10", (Test1 > SqlGuid.Null).IsNull);

                        // >=  -operator
                        Assert ("#M12", !(Test1 >= Test3).Value);
                        Assert ("#M13", (Test3 >= Test1).Value);
                        Assert ("#M14", (Test3 >= Test2).Value);
                        Assert ("#M15", (Test1 >= SqlGuid.Null).IsNull);

                        // < -operator
                        Assert ("#M16", !(Test2 < Test1).Value);
                        Assert ("#M17", (Test1 < Test3).Value);
                        Assert ("#M18", !(Test2 < Test3).Value);
                        Assert ("#M19", (Test1 < SqlGuid.Null).IsNull);

                        // <= -operator
                        Assert ("#M20", (Test1 <= Test3).Value);
                        Assert ("#M21", !(Test3 <= Test1).Value);
                        Assert ("#M22", (Test2 <= Test3).Value);
                        Assert ("#M23", (Test1 <= SqlGuid.Null).IsNull);
                }

		[Test]
		public void SqlBinaryToSqlGuid()
		{
			byte [] b = new byte [16];
			b [0] = 100;
			b [1] = 200;
			SqlBinary TestBinary = new SqlBinary (b);
			
			AssertEquals ("#N01", new Guid("0000c864-0000-0000-0000-000000000000"), 
				      ((SqlGuid)TestBinary).Value);
		}

		[Test]
		public void SqlGuidToGuid()
		{
			AssertEquals ("#O01", new Guid("00000a01-0000-0000-0000-000000000000"), 
			              (Guid)Test1);
			AssertEquals ("#O02", new Guid("00000f64-0000-0000-0000-000000000000"), 
			              (Guid)Test2);
		}		

		[Test]
                public void SqlStringToSqlGuid()
                {
                        SqlString TestString = new SqlString ("Test string");
                        SqlString TestString100 = new SqlString ("0000c864-0000-0000-0000-000000000000");

                        AssertEquals ("#P01", new Guid("0000c864-0000-0000-0000-000000000000"), 
                                      ((SqlGuid)TestString100).Value);

                        try {
                                SqlGuid test = (SqlGuid)TestString;
                                Fail ("#P02");
                        } catch(Exception e) {
                                AssertEquals ("#P03", typeof (FormatException), e.GetType ());
                        }
                }
		
		[Test]
		public void GuidToSqlGuid()
		{
			Guid TestGuid = new Guid("0000c864-0000-0000-0000-000007650000");
			AssertEquals ("#Q01", new SqlGuid("0000c864-0000-0000-0000-000007650000"), 
				      (SqlGuid)TestGuid);
		}
#if NET_2_0
		[Test]
		public void GetXsdTypeTest ()
		{
			XmlQualifiedName qualifiedName = SqlGuid.GetXsdType (null);
			NUnit.Framework.Assert.AreEqual ("string", qualifiedName.Name, "#A01");
		}
#endif
        }
}
