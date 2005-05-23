using System;
using System.Data;
using System.Data.SqlClient;
using NUnit.Framework;
using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_SqlClient
{
	[TestFixture]
	public class SqlConnection_InfoMessage : GHTBase
	{
		private int errorCounter=0;

		public static void Main()
		{
			SqlConnection_InfoMessage tc = new SqlConnection_InfoMessage();
			Exception exp = null;
			try
			{
				// Every Test must begin with BeginTest
				tc.BeginTest("NoName");
				tc.run();
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
			// After test is ready, remove this line
		
		}

		public void run()
		{
			if (DataProvider.GetDbType() != DataBaseServer.SQLServer)
			{
				Log(string.Format("Test Skipped - This test is relevant only for 'SQLServer'"));
				return;
			}

			Exception exp = null;

			// Start Sub Test
			try
			{
				test();
			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				// Every Sub Test must end with EndCase
				EndCase(exp);
				exp = null;
			}
			// End Sub Test
		}

		[Test]
		public void test()
		{
			BeginCase("InfoMessage testing");
			SqlConnection con = new SqlConnection(GHTUtils.DataProvider.ConnectionStringSQLClient);
			con.Open();
			con.InfoMessage+=new SqlInfoMessageEventHandler(con_InfoMessage);
			generateError(con);
			con.Close();
			
		}
		private void generateError(SqlConnection con)
		{
			string errorString = string.Empty;
			SqlCommand cmd = new SqlCommand (string.Empty,con); 
			cmd.CommandText  = "Raiserror ('A sample SQL informational message',10,1)";

			
			cmd.ExecuteNonQuery();
		
		
		
			//				cmd.CommandText = "TestInfoMessage";
			//				cmd.CommandType = CommandType.StoredProcedure;

			
			Compare(errorCounter,1);
		}



		//Activate This Construntor to log All To Standard output
		//public TestClass():base(true){}

		//Activate this constructor to log Failures to a log file
		//public TestClass(System.IO.TextWriter tw):base(tw, false){}

		//Activate this constructor to log All to a log file
		//public TestClass(System.IO.TextWriter tw):base(tw, true){}

		//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES

		private void con_InfoMessage(object sender, SqlInfoMessageEventArgs e)
		{
			errorCounter++;
		}
	}
}
