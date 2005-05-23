using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbTransaction_IsolationLevel : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbTransaction_IsolationLevel tc = new OleDbTransaction_IsolationLevel();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbTransaction_IsolationLevel");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;


			GHTUtils.DataBaseServer dbServer = DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionString);

			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			OleDbTransaction txn = null;
		
			try
			{
				BeginCase("IsolationLevel = ReadCommitted");
				con.Open();
				txn=con.BeginTransaction();
				Compare(txn.IsolationLevel,IsolationLevel.ReadCommitted);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null; if (con.State == ConnectionState.Open) con.Close();}

			//supported only in SQLServer
			if (dbServer == GHTUtils.DataBaseServer.SQLServer) 
			{
				try
				{
					BeginCase("IsolationLevel = Chaos");
					con.Open();
					txn=con.BeginTransaction(IsolationLevel.Chaos);
					Compare(txn.IsolationLevel,IsolationLevel.Chaos);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null; if (con.State == ConnectionState.Open) con.Close();}
			}

			//not supported in Oracle
			if (dbServer != GHTUtils.DataBaseServer.Oracle) 
			{
				try
				{
					BeginCase("IsolationLevel = ReadUncommitted");
					con.Open();
					txn=con.BeginTransaction(IsolationLevel.ReadUncommitted );
					Compare(txn.IsolationLevel,IsolationLevel.ReadUncommitted);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null; if (con.State == ConnectionState.Open) con.Close();}
			}

			//not supported in Oracle
			if (dbServer != GHTUtils.DataBaseServer.Oracle) 
			{
				try
				{
					BeginCase("IsolationLevel = RepeatableRead");
					con.Open();
					txn=con.BeginTransaction(IsolationLevel.RepeatableRead);
					Compare(txn.IsolationLevel,IsolationLevel.RepeatableRead);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null; if (con.State == ConnectionState.Open) con.Close();}
			}

			try
			{
				BeginCase("IsolationLevel = Serializable");
				con.Open();
				txn=con.BeginTransaction(IsolationLevel.Serializable);
				Compare(txn.IsolationLevel,IsolationLevel.Serializable);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null; if (con.State == ConnectionState.Open) con.Close();}

			// not supported in DB2,MSSQL,Oracle,sybase and guess what... Postgres.
			if (dbServer != GHTUtils.DataBaseServer.DB2 
				&& dbServer != GHTUtils.DataBaseServer.SQLServer
				&& dbServer != GHTUtils.DataBaseServer.Oracle 
				&& dbServer != DataBaseServer.PostgreSQL
				&& dbServer != GHTUtils.DataBaseServer.Sybase ) 
			{
				try
				{
					BeginCase("IsolationLevel = Unspecified");
					con.Open();
					txn=con.BeginTransaction(IsolationLevel.Unspecified );
					Compare(txn.IsolationLevel,IsolationLevel.Unspecified );
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null; if (con.State == ConnectionState.Open) con.Close();}
			}



		}
	}
}
