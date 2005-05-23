using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbCommand_ExecuteReader : ADONetTesterClass
	{
		OleDbConnection	con;
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
				this.Pass("Setup.");
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
			OleDbCommand_ExecuteReader tc = new OleDbCommand_ExecuteReader();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbCommand_ExecuteReader");
				tc.SetUp();
				tc.run();
				tc.TearDown();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		[Test]
		public void run()
		{
			Exception exp = null;
			bool RecordsExists = false;
			OleDbDataReader rdr =null;

			doTestBug3965();
			DoTestMultipleResultsets();

			cmd.CommandText = "Select FirstName,City From Employees";
			if (con.State != ConnectionState.Open)
			{
				con.Open();
			}


			try
			{
				BeginCase("check reader is null");
				rdr = cmd.ExecuteReader();
				Compare(rdr==null, false);
			} 
			catch(Exception ex){exp = ex;}
			finally
			{
				if (rdr != null) rdr.Close();
				EndCase(exp);
				exp = null;
			}

			try
			{
				BeginCase("check reader.read");
				rdr = cmd.ExecuteReader();
				RecordsExists = rdr.Read();
				Compare(RecordsExists ,true);
			} 
			catch(Exception ex){exp = ex;}
			finally
			{
				rdr.Close();
				EndCase(exp); 
				exp = null;
			}

			try
			{
				BeginCase("execute reader again ");
				rdr = cmd.ExecuteReader();
				Compare(rdr==null, false);
			} 
			catch(Exception ex){exp = ex;}
			finally
			{
				rdr.Close();
				EndCase(exp); 
				exp = null;
			}

			try
			{
				BeginCase("Test compound SQL statement");
				//Build a compund SQL command.
				string[] sqlStatements = new string[] {
														  "INSERT INTO Categories (CategoryName, Description) VALUES('__TEST_RECORD__', 'Inserted')",
														  "UPDATE Categories  SET Description='Updated' WHERE CategoryName='__TEST_RECORD__'",
														  "DELETE FROM Categories WHERE CategoryName='__TEST_RECORD__'" ,
													  };
				cmd.CommandText = CreateCompundSqlStatement(sqlStatements, DataProvider.GetDbType());
				rdr = cmd.ExecuteReader();
				Compare(rdr.Read(), false);
			} 
			catch(Exception ex){exp = ex;}
			finally
			{
				rdr.Close();
				EndCase(exp); 
				exp = null;
			}

			try
			{
				BeginCase("Check sql statement that declares a local variable and uses it.");
				string sqlTxt;
				switch (DataProvider.GetDbType(cmd.Connection))
				{
					case DataBaseServer.SQLServer:
						sqlTxt = "declare @var int; select @var=1;";
						break;
					case DataBaseServer.Sybase:
						sqlTxt = "declare @var int select @var=1";
						break;
					case DataBaseServer.Oracle:
					case DataBaseServer.PostgreSQL:
						sqlTxt = "declare var int;begin var:=1;end;";
						break;
					case DataBaseServer.DB2:
						sqlTxt = "begin atomic declare var integer; set var = 1; end";
						break;
					default:
						throw new ApplicationException(string.Format("GHT: Unknown DataBaseServer '{0}'", DataProvider.GetDbType(cmd.Connection)));
				}
				cmd.CommandText = sqlTxt;
				rdr = cmd.ExecuteReader();
				Compare(rdr.Read(), false);
			} 
			catch(Exception ex){exp = ex;}
			finally
			{
				rdr.Close();
				EndCase(exp); 
				exp = null;
			}

			if (DataProvider.GetDbType() != DataBaseServer.Oracle)
			{
				try
				{
					BeginCase("Check that in a compound SQL statement, resultsets are returned only for SELECT statements. (bug #3358)");
					//prepare db:
					OleDbCommand prepare = new OleDbCommand("DELETE FROM Categories WHERE CategoryName='__TEST_RECORD__'", con);
					prepare.ExecuteNonQuery();


					//Test body
					int resultSetCount ;

					//Build a compund SQL command that contains only one select statement.
					string[] sqlStatements = new string[] {
															  "INSERT INTO Categories (CategoryName, Description) VALUES('__TEST_RECORD__', 'Inserted')",
															  "UPDATE Categories  SET Description='Updated' WHERE CategoryName='__TEST_RECORD__'",
															  "DELETE FROM Categories WHERE CategoryName='__TEST_RECORD__'" ,
															  "SELECT * FROM Categories "
														  };
					string insertCmdTxt = CreateCompundSqlStatement(sqlStatements, DataProvider.GetDbType());
					//this.Log(insertCmdTxt);
					OleDbCommand InsertCmd = new OleDbCommand(insertCmdTxt, con);
					rdr = InsertCmd.ExecuteReader();

					//Count the number of result sets.
					resultSetCount = 0;
					do
					{
						resultSetCount++;
					}while (rdr.NextResult());

					//Test that there is only one result set.
					Compare(resultSetCount, 1);
				} 
				catch(Exception ex){exp = ex;}
				finally
				{
					rdr.Close();
					EndCase(exp); 
					exp = null;
					//cleanup db:
					OleDbCommand cleanup = new OleDbCommand("DELETE FROM Categories WHERE CategoryName='__TEST_RECORD__'", con);
					cleanup.ExecuteNonQuery();
				}
			}

			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				try
				{
					BeginCase("Use out refcursor implicitly to get resultset from stored-procedure in command type text.");
					cmd.CommandText = "{ call GHSP_TYPES_SIMPLE_1 (null, null, 1.234, null, null, null, null)}";
					cmd.CommandType = CommandType.Text;
					rdr = cmd.ExecuteReader();
					Compare(rdr.HasRows, true);
				} 
				catch(Exception ex){exp = ex;}
				finally
				{
					rdr.Close();
					EndCase(exp); 
					exp = null;
				}

				try
				{
					BeginCase("Use out refcursor implicitly to get resultset from stored-procedure in CommandType.StoredProcedure.");
					cmd.CommandText = "GHSP_TYPES_SIMPLE_1";
					cmd.CommandType = CommandType.StoredProcedure;
					cmd.Parameters.Add("", 123456);
					cmd.Parameters.Add("", "asdasd");
					cmd.Parameters.Add("", 1.234);
					cmd.Parameters.Add("", "asdasd");
					cmd.Parameters.Add("", "asdasd");
					cmd.Parameters.Add("", "asdasd");
					cmd.Parameters.Add("", "asdasd");
					rdr = cmd.ExecuteReader();
					Compare(rdr.HasRows, true);
				}
				catch(Exception ex){exp = ex;}
				finally
				{
					rdr.Close();
					EndCase(exp); 
					exp = null;
				}
			}
		}


		//Create the compund sql statement according to the dbserver.
		private string CreateCompundSqlStatement(string[] sqlStatements, DataBaseServer dbServer)
		{
			string beginStatement;
			string endStatement;
			string commandDelimiter;

			GetDBSpecificSyntax(dbServer, out beginStatement, out endStatement, out commandDelimiter);

			System.Text.StringBuilder cmdBuilder = new System.Text.StringBuilder();
			cmdBuilder.Append(beginStatement);
			cmdBuilder.Append(" ");
			foreach (string statement in sqlStatements)
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
					beginStatement = "";
					endStatement = "";
					commandDelimiter = ";";
					break;

				case DataBaseServer.Sybase:
					beginStatement = "BEGIN";
					endStatement = "END";
					commandDelimiter = "";
					break;
				case DataBaseServer.Oracle:
				case DataBaseServer.PostgreSQL:
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

		[Test]
		public void doTestBug3965()
		{
			Exception exp=null;
			BeginCase("Test for bug #3965");
			OleDbConnection con = new OleDbConnection("Provider=SQLOLEDB.1;Data Source=TESTDIR;User ID=ghuser;Password=ghuser;Persist Security Info=True;Initial Catalog=default_grasshopper_db;Packet Size=4096;Connect Timeout=60");
			con.Open();
			string text = "SELECT     td.TESTCYCL.TC_TEST_ID, td.TESTCYCL.TC_STATUS, td.BUG.BG_STATUS, td.BUG.BG_BUG_ID, td.BUG.BG_USER_03, td.BUG.BG_USER_09,";
			text+=  " td.BUG.BG_USER_10, td.BUG.BG_SUMMARY,BG_DETECTION_VERSION";
			text+= " FROM         td.TESTCYCL INNER JOIN";
			text+= " td.TEST ON td.TESTCYCL.TC_TEST_ID = td.TEST.TS_TEST_ID INNER JOIN";
			text+= " td.ALL_LISTS ON td.TEST.TS_SUBJECT = td.ALL_LISTS.AL_ITEM_ID RIGHT OUTER JOIN";
			text+= " td.BUG ON td.TEST.TS_TEST_ID = td.BUG.BG_TEST_REFERENCE";
			OleDbCommand cmd = new OleDbCommand(text,con);
			try 
			{		
				cmd.ExecuteReader();
				Compare(true,true);
				
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
			}
		}


		public void DoTestMultipleResultsets()
		{
			if (DataProvider.GetDbType(con) == DataBaseServer.Oracle && !GHTEnvironment.IsJavaRunTime())
			{
				//In .NET there is a bug when calling a SP with multiple REFCURSORS, the workaround is to use OracleClient and not OleDb.
				//In GH we are not bug complient in this issue, because there is no workaround (We do not support the OracleClient namespace.
				return;
			}
				Exception exp=null;
				BeginCase("Test multi result set from stored procedure");
				OleDbDataReader reader=null;

				try
				{
					//Check SP with the structre : insert Select + update Select + delete Select
					if (con.State != ConnectionState.Open)
					{
						con.Open();
					}
					OleDbCommand cmd1 = new OleDbCommand("GHSP_TYPES_SIMPLE_4",con);
					cmd1.CommandType = System.Data.CommandType.StoredProcedure;
					cmd1.Parameters.Add("ID1",string.Format("13268_{0}", this.TestCaseNumber)); 					

					reader = cmd1.ExecuteReader();
					//Count the number of result sets.
					int resultSetCount = 0;
					//Count the number of the records
					int recordCounter=0;

					do
					{
						//this.Log(string.Format("resultSetCount:{0}",resultSetCount));
						while (reader.Read())
						{
							recordCounter++;
						}
						//this.Log(string.Format("recordCounter:{0}",recordCounter));
						if (resultSetCount != 2)
						{
							Compare(recordCounter,1); //Insert + update 
						}
						else
						{
							Compare(recordCounter,0); //Delete 

						}

						recordCounter=0;
						resultSetCount++;
					}while (reader.NextResult());

					Compare(resultSetCount,3);
				}
				catch (Exception ex)
				{
					exp=ex;

				}
				finally
				{
					EndCase(exp);
					reader.Close();
					con.Close();
				}
		}
	}

}
