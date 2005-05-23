using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbConnection_BeginTransaction : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbConnection_BeginTransaction tc = new OleDbConnection_BeginTransaction();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbConnection_BeginTransaction");
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
			OleDbTransaction tran = null;

			try
			{
				BeginCase("BeginTransaction");
				tran = con.BeginTransaction();
				Compare(tran == null, false);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}
			con.Close();
			con.Open();
			try
			{
				BeginCase("BeginTransaction - IsolationLevel ReadCommitted");
				tran = con.BeginTransaction(IsolationLevel.ReadCommitted);
				Compare(tran == null, false);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

		
			DataBaseServer dbType = DataProvider.GetDbType(con);

			//Not supported by JDBC driver for oracle
			if (dbType != DataBaseServer.Oracle) 
			{
				con.Close();
				con.Open();
				try
				{
					BeginCase("BeginTransaction - IsolationLevel ReadUncommitted");
					tran = con.BeginTransaction(IsolationLevel.ReadUncommitted );
					Compare(tran == null, false);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}

				con.Close();
				con.Open();
				try
				{
					BeginCase("BeginTransaction - IsolationLevel RepeatableRead");
					tran = con.BeginTransaction(IsolationLevel.RepeatableRead);
					Compare(tran == null, false);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}
				con.Close();
				con.Open();
				try
				{
					BeginCase("BeginTransaction - IsolationLevel Serializable");
					tran = con.BeginTransaction(IsolationLevel.Serializable );
					Compare(tran == null, false);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}
			}

			// not supported on DB2 and Oracle and Sybase
			if (dbType != DataBaseServer.Oracle && dbType != DataBaseServer.DB2 && dbType != DataBaseServer.Sybase)
			{
				con.Close();
				con.Open();
				try
				{
					BeginCase("BeginTransaction - IsolationLevel Chaos");
					tran = con.BeginTransaction(IsolationLevel.Chaos);
					Compare(tran == null, false);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}
			}
			/*	not supported by MSSQL,DB2,Oracle
				con.Close();
				con.Open();
				try
				{
					BeginCase("BeginTransaction - IsolationLevel Unspecified");
					tran = con.BeginTransaction(IsolationLevel.Unspecified );
					Compare(tran == null, false);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}
			*/
			if (con.State == ConnectionState.Open) con.Close();

		}
	}
}
