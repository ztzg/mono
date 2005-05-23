using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbDataReader_GetString_I : ADONetTesterClass 
	{
		[Test] public void Main()
		{
			OleDbDataReader_GetString_I tc = new OleDbDataReader_GetString_I();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetString_I");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;

		
			base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);

			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			con.Open();
			OleDbCommand cmd = new OleDbCommand("Select FirstName From Employees Where EmployeeID = 100", con);
			OleDbDataReader rdr = cmd.ExecuteReader();
			rdr.Read();

			try
			{
				BeginCase("check value");
				string str = rdr.GetString(0);
				Compare(str,"First100" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();
		}
	}

}
