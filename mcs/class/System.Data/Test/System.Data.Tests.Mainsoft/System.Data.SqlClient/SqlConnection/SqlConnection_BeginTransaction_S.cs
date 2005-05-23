using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;
using System.Data.SqlClient;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_SqlClient
{
	[TestFixture] public class SqlConnection_BeginTransaction_S : ADONetTesterClass
	{
		SqlConnection con;

		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				con = new SqlConnection(GHTUtils.DataProvider.ConnectionStringSQLClient);
				con.Open();
				Compare("Setup", "Setup");
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		public void TearDown()
		{
			if (con != null)
			{
				if (con.State == ConnectionState.Open) con.Close();
			}
		}
		[Test] public void Main()
		{
			SqlConnection_BeginTransaction_S tc = new SqlConnection_BeginTransaction_S();
			Exception exp = null;
			try
			{
				tc.BeginTest("SqlConnection_BeginTransaction_S");

				//testing only on SQLServer
				if (DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionStringSQLClient) != DataBaseServer.SQLServer) return ; 

				tc.SetUp();
				tc.run();
				tc.TearDown();
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				tc.EndTest(exp);
			}
		}

		public void run()
		{
			Exception exp = null;

			#region		---- Bug 2716 - MSSQL - SqlCommand.Transaction ---- 
			// testing only SQLServerr
			if (DataProvider.GetDbType(con.ConnectionString) != DataBaseServer.SQLServer)
			{
				try
				{
					BeginCase("Bug 2716 - MSSQL - SqlCommand.Transaction");
					SqlCommand comm = new SqlCommand("SELECT * FROM Customers",con);

					SqlTransaction trans = con.BeginTransaction("transaction");
					comm.Transaction = trans;

					con.Close();
					Compare(con.State,System.Data.ConnectionState.Closed);
				} 
				catch(Exception ex)
				{
					exp = ex;
				}
				finally
				{
					if (con != null)
					{if (con.State == ConnectionState.Open) con.Close();}

					EndCase(exp);
					exp = null;
				}

			}
			#endregion

		}
	}
}
