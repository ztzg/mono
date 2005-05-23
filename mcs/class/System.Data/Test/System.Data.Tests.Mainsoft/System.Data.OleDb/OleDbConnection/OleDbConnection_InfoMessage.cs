using System;
using System.Data;
using NUnit.Framework;
using GHTUtils;
using GHTUtils.Base;
using System.Data.OleDb;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbConnection_InfoMessage : GHTBase
	{
		private int errorCounter=0;
		public static void Main()
		{
			OleDbConnection_InfoMessage tc = new OleDbConnection_InfoMessage();
			Exception exp = null;
			try
			{
				// Every Test must begin with BeginTest
				tc.BeginTest("OleDbConnection_InfoMessage");
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
		}

		public void run()
		{
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
			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			con.Open();
			con.InfoMessage+=new OleDbInfoMessageEventHandler(con_InfoMessage);
			generateError(con);
			con.Close();
		}
		private void generateError(OleDbConnection con)
		{
			string errorString = string.Empty;
			OleDbCommand cmd = new OleDbCommand(string.Empty,con); 

			switch (DataProvider.GetDbType(con))
			{
				case DataBaseServer.SQLServer:
				case DataBaseServer.Sybase:
				{
					cmd.CommandText  = "Raiserror ('A sample SQL informational message',10,1)";
					break;
				}
				case DataBaseServer.Oracle:
				case DataBaseServer.PostgreSQL:
				{
					cmd.CommandText = "GH_ERROR";
					//cmd.CommandText = "print 'This is a warning.'";
					//cmd.CommandText = "select   count(SUPPLIERID) from GHTDB.PRODUCTS";
					cmd.CommandType = CommandType.StoredProcedure;
					break;
				}

				case DataBaseServer.DB2:
				{
					cmd.CommandText = "SIGNAL SQLSTATE '99999' SET MESSAGE_TEXT ='Blah Blah';";
					break;
				}

				default:
				{
					throw new NotImplementedException(string.Format("GHT: Test is not implemented for {0}", DataProvider.GetDbType(con))); 
				}
			}

			
			//cmd.CommandType = CommandType.StoredProcedure;
			
				cmd.ExecuteNonQuery();
		
		
		
//				cmd.CommandText = "TestInfoMessage";
//				cmd.CommandType = CommandType.StoredProcedure;
			Compare(errorCounter,1);
		}

		private void con_InfoMessage(object sender, OleDbInfoMessageEventArgs e)
		{
			
			foreach(OleDbError err in e.Errors)
			{
				errorCounter++;
				
			}

		}

		//Activate This Construntor to log All To Standard output
		//public TestClass():base(true){}

		//Activate this constructor to log Failures to a log file
		//public TestClass(System.IO.TextWriter tw):base(tw, false){}

		//Activate this constructor to log All to a log file
		//public TestClass(System.IO.TextWriter tw):base(tw, true){}

		//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES

		
	}
}
