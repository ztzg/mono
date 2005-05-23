using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbTransaction_Begin : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbTransaction_Begin tc = new OleDbTransaction_Begin();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbTransaction_Begin");
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

			/*********************************************************
			 * OLEDB Provider for SQL Server does not allow nested transactions
			 * http://support.microsoft.com/kb/177138/EN-US/
			 * http://support.microsoft.com/default.aspx?scid=KB;EN-US;Q316872&
			*/
			if ((DataProvider.GetDbType(con) == DataBaseServer.SQLServer) ||
				(DataProvider.GetDbType(con) == DataBaseServer.Oracle) ||
				(DataProvider.GetDbType(con) == DataBaseServer.PostgreSQL) ||
				(DataProvider.GetDbType(con) == DataBaseServer.DB2))
			{
				Log(string.Format("Test skipped, nested transactions are not supported in {0}", DataProvider.GetDbType(con)));
				return;
			}

			// How To Implement Nested Transactions with Oracle
			// http://support.microsoft.com/kb/187289/EN-US/

			OleDbTransaction txnOuter = null;
			OleDbTransaction txnInner = null;
		
			try
			{
				BeginCase("Check Outer Transaction Isoloation Level");
			
				txnOuter = con.BeginTransaction();
				txnInner = txnOuter.Begin();

				Compare(txnOuter.IsolationLevel,IsolationLevel.ReadCommitted);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check Inner Transaction Isoloation Level");
				Compare(txnOuter.IsolationLevel,IsolationLevel.RepeatableRead);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();
		}
	}
}
