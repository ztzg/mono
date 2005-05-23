using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbConnection_ConnectionString : GHTBase
	{
		private Exception exp = null;
		private OleDbConnection con = new OleDbConnection();

		[Test] public void Main()
		{
			OleDbConnection_ConnectionString tc = new OleDbConnection_ConnectionString();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbConnection_ConnectionString");

				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			SetConnectionString();
			ConstractorWithConnectionString();
			DB2MissingProperties();
			DoTestCaseForBug3925();
		}

		#region Tests
		private void DB2MissingProperties()
		{
			if (DataProvider.GetDbType() != DataBaseServer.DB2)
			{
				Log("The 'DB2MissingProperties' subtest did not ran because it is relevant only for runs on DB2." );
				return;
			}
			string[] propsToUse;
			string description;

			//The complete list of properties is:
			//Provider, Password, User ID, Data Source, HostName, Port, Location, retrieveMessagesFromServerOnGetMessage
			
			description = "Test connection string without 'retrieveMessagesFromServerOnGetMessage'";
			propsToUse = new string[] {"Provider", "Password", "User ID", "Data Source", "HostName", "Port", "Location"};
			DoTestWithSpecificProperties(propsToUse, description);

			description = "Test connection string without 'HostName'";
			propsToUse = new string[] {"Provider", "Password", "User ID", "Data Source", "Port", "Location", "retrieveMessagesFromServerOnGetMessage"};
			DoTestWithSpecificProperties(propsToUse, description);
			
			description = "Test connection string without 'Port'";
			propsToUse = new string[] {"Provider", "Password", "User ID", "Data Source", "HostName", "Location", "retrieveMessagesFromServerOnGetMessage"};
			DoTestWithSpecificProperties(propsToUse, description);
			
			description = "Test connection string without 'Location'";
			propsToUse = new string[] {"Provider", "Password", "User ID", "Data Source", "HostName", "Port", "retrieveMessagesFromServerOnGetMessage"};
			DoTestWithSpecificProperties(propsToUse, description);
			
			description = "Test connection string without 'HostName' & 'Port'";
			propsToUse = new string[] {"Provider", "Password", "User ID", "Data Source", "Location", "retrieveMessagesFromServerOnGetMessage"};
			DoTestWithSpecificProperties(propsToUse, description);
		}

		void DoTestWithSpecificProperties(string[] propsToUse, string description)
		{
			try
			{
				BeginCase(description);
				exp = null;
				System.Collections.Hashtable connectionProps = GetConnectionStringProps(DataProvider.ConnectionString);
				string actualConString = CreateConStringWithProps(propsToUse, connectionProps);
				con = new OleDbConnection(actualConString);
				con.Open();
				Compare(con.State, ConnectionState.Open);
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				exp=null;
				if (con != null && con.State == ConnectionState.Open)
				{
					con.Close();
				}
			}
		}

		private void ConstractorWithConnectionString()
		{
			con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			try
			{
				BeginCase("Constructor with ConnectionString");
				Compare(con.ConnectionString, GHTUtils.DataProvider.ConnectionString);
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

		private void SetConnectionString()
		{
			con.ConnectionString = GHTUtils.DataProvider.ConnectionString;
			try
			{
				BeginCase("Set ConnectionString");
				Compare(con.ConnectionString, GHTUtils.DataProvider.ConnectionString);
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
		public void DoTestCaseForBug3925()
		{
			exp=null;
			try
			{
				BeginCase("Test Case for bug #3925");
			if (DataProvider.GetDbType() != DataBaseServer.SQLServer || !GHTEnvironment.IsJavaRunTime())
			{
				Skip("This test currently runs only on SQLServer in java.");
				return;
			}
				System.Collections.Hashtable conProps = GetConnectionStringProps(DataProvider.ConnectionString);
				string server = (string)conProps["Data Source"];
				string user = (string)conProps["User Id"];
				string password = (string)conProps["Password"];
				string database = (string)conProps["Initial Catalog"];
				string jdbcUrlTemplate = "JdbcDriverClassName=com.microsoft.jdbc.sqlserver.SQLServerDriver;JdbcURL=\"jdbc:microsoft:sqlserver://{0};User={1};Password={2};DatabaseName={3}\"";
				string conStr = string.Format(jdbcUrlTemplate, server, user, password, database);
				con = new OleDbConnection(conStr);
				con.Open();
				Compare(ConnectionState.Open, con.State);
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				exp = null;
				if (con != null && con.State != ConnectionState.Closed)
				{
					con.Close();
				}
			}
		}
		#endregion
		#region Utilities
		private string CreateConStringWithProps(string[] propsToUse, System.Collections.Hashtable connectionProps)
		{
			System.Text.StringBuilder actualConStringBuilder = new System.Text.StringBuilder();
			foreach(string prop in propsToUse)
			{
				string propString = string.Format("{0}={1}", prop, connectionProps[prop]);
				actualConStringBuilder.Append(propString);
				actualConStringBuilder.Append(";");
			}
			return actualConStringBuilder.ToString();
		}

		private System.Collections.Hashtable GetConnectionStringProps(string connectionString)
		{
			string[] connectionStringProps = connectionString.Split(';');
			System.Collections.Hashtable connectionProps = new System.Collections.Hashtable();
			foreach(string prop in connectionStringProps)
			{
				string[] keyValue = prop.Split('=');
				if (keyValue.Length == 2)
				{
					connectionProps.Add(keyValue[0], keyValue[1]);
				}
				else
				{
					connectionProps.Add(prop, prop);
				}
			}
			return connectionProps;
		}

		#endregion
	}
}
