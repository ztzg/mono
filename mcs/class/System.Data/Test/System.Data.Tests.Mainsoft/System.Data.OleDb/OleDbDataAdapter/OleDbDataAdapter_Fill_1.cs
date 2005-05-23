using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbDataAdapter_Fill_1: ADONetTesterClass 
	{

		OleDbConnection con;
		OleDbCommand cmd;

		[SetUp]
		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
				cmd = new OleDbCommand("", con);
				con.Open();
				// prepare data
				base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);
				Compare("Setup" ,"Setup");
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		[TearDown]
		public void TearDown()
		{
			if (con != null)
			{
				if (con.State == ConnectionState.Open) con.Close();
			}
		}

		public static void Main()
		{
			OleDbDataAdapter_Fill_1 tc = new OleDbDataAdapter_Fill_1();
			Exception exp = null;
			try
			{
				// Every Test must begin with BeginTest
				tc.BeginTest("OleDbDataAdapter_Fill_1");
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
				// Every Test must End with EndTest
				tc.EndTest(exp);
			}
		}

		[Test]
		public void run()
		{
			Exception exp = null;
			//bool NextResultExists = false;

			if (DataProvider.GetDbType(con) == DataBaseServer.Oracle && !GHTEnvironment.IsJavaRunTime())
			{
				System.Text.StringBuilder messageBuilder = new System.Text.StringBuilder();
				messageBuilder.Append("Test \"OleDbDataAdapter_Fill_1\" Skipped when running in .NET against Oracle database:\n");
				messageBuilder.Append("In .NET there is a bug when calling a SP with multiple REFCURSORS from oracle server, the workaround is to use OracleClient and not OleDb.\n");
				messageBuilder.Append("In GH we are not bug complient in this issue, because there is no workaround - We do not support the OracleClient namespace.");
				messageBuilder.Append(" (The java run is not skipped).");
				Log(messageBuilder.ToString());
				return;
			}

			cmd.CommandText = "GH_MULTIRECORDSETS";
			cmd.CommandType = CommandType.StoredProcedure;
			OleDbDataAdapter da = new OleDbDataAdapter(cmd);
			DataSet ds = new DataSet();

			//execute the fill command
			da.Fill(ds);

			try
			{
				BeginCase("Check table count");
				Compare(ds.Tables.Count ,3);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check table 0 rows count");
				Compare(ds.Tables[0].Rows.Count ,2);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check table 0 Columns count");
				Compare(ds.Tables[0].Columns.Count ,2);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check table 1 rows count");
				Compare(ds.Tables[1].Rows.Count ,2);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check table 1 Columns count");
				Compare(ds.Tables[1].Columns.Count ,3);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check table 2 rows count");
				Compare(ds.Tables[2].Rows.Count ,0);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check table 2 Columns count");
				Compare(ds.Tables[2].Columns.Count ,4);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

		}
	}
}
