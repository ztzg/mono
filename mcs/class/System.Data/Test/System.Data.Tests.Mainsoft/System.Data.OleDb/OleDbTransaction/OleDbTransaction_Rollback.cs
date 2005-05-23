using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbTransaction_Rollback : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbTransaction_Rollback tc = new OleDbTransaction_Rollback();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbTransaction_Rollback");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;

			//prepare data
			base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);


			string Result = "";

			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			OleDbTransaction txn;
			con.Open();
			txn = con.BeginTransaction();
			OleDbCommand cmd = new OleDbCommand("Update Employees Set LastName = 'StamLastName' Where EmployeeID = 100", con, txn);

			cmd.ExecuteNonQuery();
			txn.Rollback();
		
			cmd = new OleDbCommand("Select LastName From Employees Where EmployeeID = 100", con);
			Result = cmd.ExecuteScalar().ToString();

			try
			{
				BeginCase("check value");
				Compare(Result,"Last100" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();
		}
	}
}
