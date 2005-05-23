using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbDataReader_GetDataTypeName : ADONetTesterClass
	{
		OleDbConnection con;
		OleDbCommand cmd;
		OleDbDataReader rdr;
		string typeName;

		[SetUp]
		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
				cmd = new OleDbCommand("Select EmployeeID From Employees Where FirstName = 'Oved'",  con);
				cmd.CommandType = System.Data.CommandType.Text;
				con.Open();

				rdr = cmd.ExecuteReader();
				rdr.Read();

				typeName = rdr.GetDataTypeName(0);
				con.Close();
			}
			catch(Exception ex){exp = ex;}
			finally	{EndCase(exp);}
		}

		[TearDown]
		public void TearDown()
		{
		}

		public static void Main()
		{
			OleDbDataReader_GetDataTypeName tc = new OleDbDataReader_GetDataTypeName();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetDataTypeName");
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

			//make database specific test
			switch (DataProvider.GetDbType(con))
			{
				case DataBaseServer.SQLServer:
					try
					{
						BeginCase("check type name");
						Compare(typeName == "DBTYPE_I4" ,true);
					} 
					catch(Exception ex){exp = ex;}
					finally{EndCase(exp); exp = null;}
					break;
				case DataBaseServer.Oracle:
					try
					{
						BeginCase("check type name");
						Compare(typeName == "DBTYPE_NUMERIC" || typeName == "NUMBER" ,true);
					} 
					catch(Exception ex){exp = ex;}
					finally{EndCase(exp); exp = null;}
					break;
				case DataBaseServer.DB2:
					try
					{
						BeginCase("check type name");
						Compare(typeName == "DBTYPE_I4" || typeName == "INTEGER" ,true);
					} 
					catch(Exception ex){exp = ex;}
					finally{EndCase(exp); exp = null;}
					break;
				case DataBaseServer.Unknown:
					break;
			}

		}
	}

}
