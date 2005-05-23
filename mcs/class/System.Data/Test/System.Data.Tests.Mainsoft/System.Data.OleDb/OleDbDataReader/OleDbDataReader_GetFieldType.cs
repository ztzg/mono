using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbDataReader_GetFieldType : ADONetTesterClass
	{
		OleDbConnection con;
		OleDbCommand cmd;
		OleDbDataReader rdr;

		[SetUp]
		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
				con.Open();
				cmd = new OleDbCommand("Select OrderID, CustomerID, OrderDate From Orders", con);
				rdr = cmd.ExecuteReader();
				rdr.Read();

			}
			catch(Exception ex){exp = ex;}
			finally	{EndCase(exp);}
		}

		[TearDown]
		public void TearDown()
		{
			if (con.State == ConnectionState.Open) con.Close();
		}

		public static void Main()
		{
			OleDbDataReader_GetFieldType tc = new OleDbDataReader_GetFieldType();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetFieldType");
				tc.SetUp();
				tc.run();
				tc.TearDown();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		[Test]
		public void run()
		{
			Exception exp = null;

			try
			{
				BeginCase("check type string");
				Compare(rdr.GetFieldType(1).FullName,typeof(string).FullName );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("check type date");
				Compare(rdr.GetFieldType(2).FullName,typeof(DateTime).FullName );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}
		}
	}
}
