using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class TestId13296 : ADONetTesterClass 
	{
		[Test] public void Main()
		{
			TestId13296 tc = new TestId13296();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_IsDBNull");
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
			OleDbCommand cmd = new OleDbCommand("Select EmployeeID,City From Employees where EmployeeID = 100  ", con);
			OleDbDataReader rdr = cmd.ExecuteReader();
			rdr.Read();

			try
			{
				BeginCase("IsDBNull value");
				Compare(rdr.IsDBNull(1),true );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();
		}
	}
}
