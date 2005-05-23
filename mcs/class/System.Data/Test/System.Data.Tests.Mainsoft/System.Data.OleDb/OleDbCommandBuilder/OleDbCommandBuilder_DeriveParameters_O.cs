using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbCommandBuilder_DeriveParameters_O : GHTBase
	{
		OleDbConnection	con;
		OleDbCommand cmd;
		Exception exp = null;

		public void SetUp()
		{
			exp=null;
			BeginCase("Setup");
			try
			{
				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
				con.Open();
				cmd = new OleDbCommand("", con);
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
			OleDbCommandBuilder_DeriveParameters_O tc = new OleDbCommandBuilder_DeriveParameters_O();
			try
			{
				tc.BeginTest("OleDbCommandBuilder_DeriveParameters_O");
				tc.SetUp();
				tc.run();
				tc.TearDown();
			}
			catch(Exception ex)
			{
				tc.exp = ex;
			}
			finally
			{
				tc.EndTest(tc.exp);
			}
		}


		public void run()
		{
            RetrieveParameters();
		}

		public void RetrieveParameters()
		{
			exp = null;

			try
			{
				BeginCase("retrieve parameters");
				
				if (DataProvider.GetDbType(con) == DataBaseServer.DB2)
				{
					this.Skip("Not Implemented on DB2.");
					return;
				}

				switch (DataProvider.GetDbType(con))
				{
					case GHTUtils.DataBaseServer.PostgreSQL:
						cmd = new OleDbCommand("GH_MULTIRECORDSETS('a','b','c')", con);
						break;
					default:
						cmd = new OleDbCommand("GH_MultiRecordSets", con);
						break;
				}
				
				cmd.CommandType = CommandType.StoredProcedure;
				OleDbCommandBuilder.DeriveParameters(cmd);

				switch (DataProvider.GetDbType(con))
				{
					case GHTUtils.DataBaseServer.SQLServer:
					case DataBaseServer.Sybase:
						Compare(cmd.Parameters.Count, 1);
						Compare(cmd.Parameters[0].Direction, System.Data.ParameterDirection.ReturnValue);
						Compare(cmd.Parameters[0].ParameterName, "RETURN_VALUE");
						break;
				
					case GHTUtils.DataBaseServer.Oracle:
						Compare(cmd.Parameters.Count, 3);
						Compare(cmd.Parameters[0].Direction, System.Data.ParameterDirection.Output);
						Compare(cmd.Parameters[1].Direction, System.Data.ParameterDirection.Output);
						Compare(cmd.Parameters[2].Direction, System.Data.ParameterDirection.Output);
						Compare(cmd.Parameters[0].ParameterName, "RCT_EMPLOYEES");
						break;
					case DataBaseServer.PostgreSQL:
						Compare(cmd.Parameters.Count, 3);
						Compare(cmd.Parameters[0].Direction, System.Data.ParameterDirection.Output);
						Compare(cmd.Parameters[1].Direction, System.Data.ParameterDirection.Output);
						Compare(cmd.Parameters[2].Direction, System.Data.ParameterDirection.Output);
						break;
					default:
						throw new ApplicationException(string.Format("GHT: Test not implemented for DB type {0}", DataProvider.GetDbType(con)));
				}
			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				exp = null;
			}
		}
	}
}
