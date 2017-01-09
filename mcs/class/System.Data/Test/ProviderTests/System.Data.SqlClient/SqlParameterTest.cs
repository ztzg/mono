//
// SqlParameterTest.cs - NUnit Test Cases for testing the
//                          SqlParameter class
// Author:
//      Senganal T (tsenganal@novell.com)
//      Amit Biswas (amit@amitbiswas.com)
//
// Copyright (c) 2004 Novell Inc., and the individuals listed
// on the ChangeLog entries.
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
using System.Data;
using System.Data.SqlClient;

using NUnit.Framework;

namespace MonoTests.System.Data.SqlClient
{
	[TestFixture]
	[Category ("sqlserver")]
	public class SqlParameterTest
	{
		SqlConnection conn;
		SqlCommand cmd;
		SqlDataReader rdr;

		[SetUp]
		public void SetUp ()
		{
			conn = (SqlConnection) ConnectionManager.Singleton.Connection;
			ConnectionManager.Singleton.OpenConnection ();
		}

		[TearDown]
		public void TearDown ()
		{
			if (cmd != null)
				cmd.Dispose ();
			if (rdr != null)
				rdr.Close ();
			ConnectionManager.Singleton.CloseConnection ();
		}

		[Test] // bug #324840
		public void ParameterSizeTest ()
		{
			string longstring = new String('x', 20480);
			SqlParameter prm;
			cmd = new SqlCommand ("create table #text1 (ID int not null, Val1 ntext)", conn);
			cmd.ExecuteNonQuery ();
			cmd.CommandText = "INSERT INTO #text1(ID,Val1) VALUES (@ID,@Val1)";
			prm = new SqlParameter ();
			prm.ParameterName = "@ID";
			prm.Value = 1;
			cmd.Parameters.Add (prm);

			prm = new SqlParameter ();
			prm.ParameterName = "@Val1";
			prm.Value = longstring;
			prm.SqlDbType = SqlDbType.NText; // Comment and enjoy the truncation
			cmd.Parameters.Add (prm);
			cmd.ExecuteNonQuery ();
			cmd = new SqlCommand ("select datalength(Val1) from #text1", conn);
			Assert.AreEqual (20480 * 2, cmd.ExecuteScalar (), "#1");

			cmd.CommandText = "INSERT INTO #text1(ID,Val1) VALUES (@ID,@Val1)";
			prm = new SqlParameter ();
			prm.ParameterName = "@ID";
			prm.Value = 1;
			cmd.Parameters.Add (prm);

			prm = new SqlParameter ();
			prm.ParameterName = "@Val1";
			prm.Value = longstring;
			//prm.SqlDbType = SqlDbType.NText;
			cmd.Parameters.Add (prm);
			cmd.ExecuteNonQuery ();
			cmd = new SqlCommand ("select datalength(Val1) from #text1", conn);
			Assert.AreEqual (20480 * 2, cmd.ExecuteScalar (), "#2");

			cmd.CommandText = "INSERT INTO #text1(ID,Val1) VALUES (@ID,@Val1)";
			prm = new SqlParameter ();
			prm.ParameterName = "@ID";
			prm.Value = 1;
			cmd.Parameters.Add (prm);

			prm = new SqlParameter ();
			prm.ParameterName = "@Val1";
			prm.Value = longstring;
			prm.SqlDbType = SqlDbType.VarChar;
			cmd.Parameters.Add (prm);
			cmd.ExecuteNonQuery ();
			cmd = new SqlCommand ("select datalength(Val1) from #text1", conn);
			Assert.AreEqual (20480 * 2, cmd.ExecuteScalar (), "#3");
			cmd = new SqlCommand ("drop table #text1", conn);
			cmd.ExecuteNonQuery ();
			conn.Close ();
		}

		[Test] // bug #382635
		public void ParameterSize_compatibility_Test ()
		{
			string longstring = "abcdefghijklmnopqrstuvwxyz";

			cmd = new SqlCommand ("create table #bug382635 (description varchar(20))", conn);
			cmd.ExecuteNonQuery ();

			cmd.CommandText = 
					"CREATE PROCEDURE #sp_bug382635 (@Desc varchar(20)) "
					+ "AS " + Environment.NewLine 
					+ "BEGIN" + Environment.NewLine 
					+ "UPDATE #bug382635 SET description = @Desc" + Environment.NewLine
					+ "END";
			cmd.CommandType = CommandType.Text;
			cmd.ExecuteNonQuery ();

			cmd.CommandText = "INSERT INTO #bug382635 " +
					  "(description) VALUES ('Verifies bug #382635')";
			cmd.ExecuteNonQuery ();

			cmd.CommandText = "#sp_bug382635";
			cmd.CommandType = CommandType.StoredProcedure;

			SqlParameter p1 = new SqlParameter ("@Desc", SqlDbType.NVarChar, 15);
			p1.Value = longstring;
			Assert.AreEqual (longstring, p1.Value);
			cmd.Parameters.Add (p1);
			cmd.ExecuteNonQuery ();

			// Test for truncation
			SqlCommand selectCmd = new SqlCommand ("SELECT DATALENGTH(description), description from #bug382635", conn);

			rdr = selectCmd.ExecuteReader ();
			Assert.IsTrue (rdr.Read (), "#A1");
			Assert.AreEqual (15, rdr.GetValue (0), "#A2");
			Assert.AreEqual (longstring.Substring (0, 15), rdr.GetValue (1), "#A3");
			Assert.AreEqual (longstring, p1.Value, "#A4");
			rdr.Close ();

			// Test to ensure truncation is not done in the Value getter/setter
			p1.Size = 12;
			p1.Value = longstring.Substring (0, 22);
			p1.Size = 14;
			cmd.ExecuteNonQuery ();

			rdr = selectCmd.ExecuteReader ();
			Assert.IsTrue (rdr.Read (), "#B1");
			Assert.AreEqual (14, rdr.GetValue (0), "#B2");
			Assert.AreEqual (longstring.Substring (0, 14), rdr.GetValue (1), "#B3");
			Assert.AreEqual (longstring.Substring (0, 22), p1.Value, "#B4");
			rdr.Close ();

			// Size exceeds size of value
			p1.Size = 40;
			cmd.ExecuteNonQuery ();

			rdr = selectCmd.ExecuteReader ();
			Assert.IsTrue (rdr.Read (), "#C1");
			Assert.AreEqual (20, rdr.GetValue (0), "#C2");
			Assert.AreEqual (longstring.Substring (0, 20), rdr.GetValue (1), "#C3");
			rdr.Close ();
		}

		[Test]
		public void ConversionToSqlTypeInvalid ()
		{
			string insert_data = "insert into datetime_family (id, type_datetime) values (6000, @type_datetime)";
			string delete_data = "delete from datetime_family where id = 6000";

			object [] values = new object [] {
				5,
				true,
				40L,
				"invalid date",
				};

			try {
				for (int i = 0; i < values.Length; i++) {
					object value = values [i];

					cmd = conn.CreateCommand ();
					cmd.CommandText = insert_data;
					SqlParameter param = cmd.Parameters.Add ("@type_datetime", SqlDbType.DateTime);
					param.Value = value;
					cmd.Prepare ();

					try {
						cmd.ExecuteNonQuery ();
						Assert.Fail ("#1:" + i);
					} catch (InvalidCastException) {
						if (value is string)
							Assert.Fail ("#2");
					} catch (FormatException) {
						if (!(value is string))
							Assert.Fail ("#3");
					}
				}
			} finally {
				DBHelper.ExecuteNonQuery (conn, delete_data);
			}
		}
	}
}
