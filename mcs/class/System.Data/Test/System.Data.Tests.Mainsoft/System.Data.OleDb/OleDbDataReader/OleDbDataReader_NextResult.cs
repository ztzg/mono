using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbDataReader_NextResult : ADONetTesterClass 
	{
		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
		Exception exp = null;

		[Test] public void Main()
		{
			OleDbDataReader_NextResult tc = new OleDbDataReader_NextResult();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_NextResult");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{


			base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);
			con.Open();

			DoTestMultipleResultSetsWithSP();
			DoTestMultipleResultSetsWithSQLText();

			if (con.State == ConnectionState.Open) con.Close();

		}

		public void DoTestMultipleResultSetsWithSQLText()
		{

			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				this.Log("Multiple result sets by sql text is not tested in oracle.");
				return;
			}

			bool NextResultExists = false;
			OleDbDataReader rdr = null;
			OleDbCommand cmd;
			int TblResult0=-1;
			int TblResult1=-1;
			int TblResult2=-1;
			try
			{
				BeginCase("Setup: Get expected results.");

				//get excpected results
				GetExcpectedResults(ref TblResult0, ref TblResult1, ref TblResult2);
				this.Pass("Setup: Get expected results ended.");
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
			}


			string cmdTxt = BuildCommandText();
			cmd = new OleDbCommand(cmdTxt, con);
			cmd.CommandType = CommandType.Text;
			rdr = cmd.ExecuteReader();
			// -------------- ResultSet  1 ------------
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check if ResultSet 1 exists");
				Compare(rdr != null, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check if ResultSet 1 contains data");
				NextResultExists = rdr.Read();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			int i = 1;
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check ResultSet 1 Data");
				while (rdr.Read())
				{
					i++;
				}
				Compare(i, TblResult0);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check ResultSet 1 Schema");
				Compare(rdr.GetSchemaTable().Rows[0].ItemArray.GetValue(0).ToString().ToUpper(), "CUSTOMERID");
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			// -------------- ResultSet  2 ------------
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check if ResultSet 2 exists");
				NextResultExists = rdr.NextResult();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check if ResultSet 2 contains data");
				NextResultExists = rdr.Read();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check ResultSet 2 Data");
				i = 1;
				while (rdr.Read())
				{
					i++;
				}
				Compare(i, TblResult1);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check ResultSet 2 Schema");
				Compare(rdr.GetSchemaTable().Rows[0].ItemArray.GetValue(0).ToString().ToUpper(), "CATEGORYID");
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			// -------------- ResultSet  3 ------------
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check if ResultSet 3 exists");
				NextResultExists = rdr.NextResult();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check if ResultSet 3 contains data");
				NextResultExists = rdr.Read();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check ResultSet 3 Data");
				i = 1;
				while (rdr.Read())
				{
					i++;
				}
				Compare(i, TblResult2);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check ResultSet 3 Schema");
				Compare(rdr.GetSchemaTable().Rows[0].ItemArray.GetValue(0).ToString().ToUpper(), "REGIONID");
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check that resultset 4 does not exist.");
				NextResultExists = rdr.NextResult();
				Compare(NextResultExists, false);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets sql text) - Check that resultset 4 does not contain data.");
				NextResultExists = rdr.Read();
				Compare(NextResultExists, false);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			if (!rdr.IsClosed)
				rdr.Close();
		}

		public void DoTestMultipleResultSetsWithSP()
		{
			if (DataProvider.GetDbType() == DataBaseServer.Oracle && !GHTEnvironment.IsJavaRunTime())
			{
				this.Log("Stored procedures with multiple ref-cursors is not tested on Oracle with .NET due to bug in .NET (only the first ref-cursor is retrived).");
				return;
			}
			
			bool NextResultExists = false;
			OleDbCommand cmd = new OleDbCommand("GH_MULTIRECORDSETS", con);
			cmd.CommandType = CommandType.StoredProcedure;
			OleDbDataReader rdr = cmd.ExecuteReader();

			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check if ResultSet 1 exists");
				Compare(rdr != null, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check if ResultSet 1 contains data");
				NextResultExists = rdr.Read();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check ResultSet 1 Data");
				Compare(rdr.GetValue(1).ToString(), "Yavine");
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check ResultSet 1 Schema");
				Compare(rdr.GetSchemaTable().Rows[0].ItemArray.GetValue(0).ToString().ToUpper(), "EMPLOYEEID");
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}

			
			// -------------- ResultSet  2 ------------
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check if ResultSet 2 exists");
				NextResultExists = rdr.NextResult();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check if ResultSet 2 contains data");
				NextResultExists = rdr.Read();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check ResultSet 2 Data");
				Compare(rdr.GetValue(1).ToString(), "Morgenstern Gesundkost");
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check ResultSet 2 Schema");
				Compare(rdr.GetSchemaTable().Rows[0].ItemArray.GetValue(0).ToString().ToUpper(), "CUSTOMERID");
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}

			// -------------- ResultSet  3 ------------
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check if ResultSet 3 exists");
				NextResultExists = rdr.NextResult();
				Compare(NextResultExists, true);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check if ResultSet 3 contains data");
				NextResultExists = rdr.Read();
				Compare(NextResultExists, false);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check ResultSet 3 Schema");
				Compare(rdr.GetSchemaTable().Rows[0].ItemArray.GetValue(0).ToString().ToUpper(), "ORDERID");
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check that resultset 4 does not exist.");
				NextResultExists = rdr.NextResult();
				Compare(NextResultExists, false);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}
			try
			{
				exp = null;
				BeginCase("(Multiple Resultsets stored proc.) - Check that resultset 4 does not contain data.");
				NextResultExists = rdr.Read();
				Compare(NextResultExists, false);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally	
			{
				EndCase(exp);
			}

			//Cleanup:
			if (!rdr.IsClosed)
			{
				rdr.Close();
			}
		}


		#region "Private Utilities"
		private string BuildCommandText()
		{
			string beginStatement;
			string endStatement;
			string commandDelimiter;
			string[] commands = new string[] {"select * from Customers", "select * from Categories", "select * from Region"};

			GetDBSpecificSyntax(DataProvider.GetDbType(), out beginStatement, out endStatement, out commandDelimiter);

			System.Text.StringBuilder cmdBuilder = new System.Text.StringBuilder();
			cmdBuilder.Append(beginStatement);
			cmdBuilder.Append(" ");
			foreach (string statement in commands)
			{
				cmdBuilder.Append(statement);
				cmdBuilder.Append(commandDelimiter);
				cmdBuilder.Append(" ");
			}
			cmdBuilder.Append(endStatement);

			return cmdBuilder.ToString();
		}
		private void GetDBSpecificSyntax(DataBaseServer dbServer, out string beginStatement, out string endStatement, out string commandDelimiter)
		{
			switch (dbServer)
			{
				case DataBaseServer.SQLServer:
					beginStatement = "BEGIN";
					endStatement = "END";
					commandDelimiter = ";";
					break;
				case DataBaseServer.Sybase:
					beginStatement = "BEGIN";
					endStatement = "END";
					commandDelimiter = "\r\n";
					break;
				case DataBaseServer.Oracle:
					beginStatement = "BEGIN";
					endStatement = "END;";
					commandDelimiter = ";";
					break;

				case DataBaseServer.DB2:
					beginStatement = "BEGIN ATOMIC";
					endStatement = "END";
					commandDelimiter = ";";
					break;

				default:
					this.Fail("Unknown DataBaseServer type");
					throw new ApplicationException("Unknown DataBaseServer type");
			}
		}
		private void GetExcpectedResults(ref int TblResult0, ref int TblResult1, ref int TblResult2)
		{
			//get excpected results
			OleDbCommand cmd = new OleDbCommand("", con);
			cmd.CommandText = "Select count(*) from Customers";
			TblResult0 = (int)cmd.ExecuteScalar();
			cmd.CommandText = "Select count(*) from Categories";
			TblResult1 = (int)cmd.ExecuteScalar();
			cmd.CommandText = "Select count(*) from Region";
			TblResult2 = (int)cmd.ExecuteScalar();
		}

		#endregion
	}
}
