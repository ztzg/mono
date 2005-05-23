using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbTransaction_Commit : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbTransaction_Commit tc = new OleDbTransaction_Commit();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbTransaction_Commit");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;

			// ORACLE does not support transactions with savepoints 
			// http://support.microsoft.com/kb/187289/EN-US/
			if (DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionString) == DataBaseServer.Oracle) return;
			if (DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionString) == DataBaseServer.PostgreSQL) return;

			//prepare data
			base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);

			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			con.Open();

			//prepare command for checking database
			OleDbConnection conSelect = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			conSelect.Open();
			OleDbCommand cmdSelect = new OleDbCommand("Select Title From Employees where EmployeeID in (100,200)", conSelect);

			OleDbTransaction txn = con.BeginTransaction();

			//prepare first transaction
			OleDbCommand cmd1 = new OleDbCommand("Update Employees Set Title = 'New Value1' Where EmployeeID = 100",con);
			cmd1.Transaction = txn;
		
			//prepare a second transaction
			OleDbCommand cmd2 = new OleDbCommand("Update Employees Set Title = 'New Value2' Where EmployeeID = 200",con);
			cmd2.Transaction = txn;

			try
			{
				BeginCase("one transaction - After commiting transaction");
				cmd1.ExecuteNonQuery();
				txn.Commit();
				string Result = cmdSelect.ExecuteScalar().ToString();
				Compare(Result,"New Value1" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}


			//prepare data
			base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);

			txn = con.BeginTransaction();
			cmd1.Transaction = txn;
			cmd2.Transaction = txn;
			cmd1.ExecuteNonQuery();
			cmd2.ExecuteNonQuery();
			txn.Commit();
			OleDbDataReader rdr = cmdSelect.ExecuteReader();

			try
			{
				BeginCase("two transaction - check first transaction");
				rdr.Read();
				Compare(rdr.GetString(0),"New Value1" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("two transaction - check second transaction");
				rdr.Read();
				Compare(rdr.GetString(0),"New Value2" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();
			if (conSelect.State == ConnectionState.Open) conSelect.Close();


		}
	}
}
