using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbTransaction_Connection : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbTransaction_Connection tc = new OleDbTransaction_Connection();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbTransaction_Connection");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;

			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			con.Open();
			OleDbTransaction txn = con.BeginTransaction();

			try
			{
				BeginCase("check connection");
				Compare(txn.Connection,con);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			//check exception - using a command with the same connection as the transaction
			OleDbCommand cmd = new OleDbCommand("Select * from Employees",con);

			//Execute requires the command to have a transaction object when the connection assigned to the command is in a pending local transaction.  
			//The Transaction property of the command has not been initialized.
			try
			{
				BeginCase("Command that uses the transaction connection - exception");
				try
				{
					cmd.ExecuteNonQuery();
				}
				catch (Exception ex){exp=ex;}
				Compare(exp.GetType().FullName ,typeof(InvalidOperationException).FullName);
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();

		}
	}
}
