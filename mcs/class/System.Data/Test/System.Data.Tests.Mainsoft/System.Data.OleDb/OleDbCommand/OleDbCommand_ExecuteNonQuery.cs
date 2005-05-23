using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbCommand_ExecuteNonQuery : GHTBase
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
				//prepare Data
				OleDbCommand cmdPrepare = new OleDbCommand("", new OleDbConnection(GHTUtils.DataProvider.ConnectionString));
				cmdPrepare.Connection.Open();
				cmdPrepare.CommandText = "DELETE FROM Employees WHERE EmployeeID = 99999";
				cmdPrepare.ExecuteScalar();
				cmdPrepare.Connection.Close();
				cmdPrepare.Dispose();

				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
				cmd = new OleDbCommand("", con);
				con.Open();
			}
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}
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
			OleDbCommand_ExecuteNonQuery tc = new OleDbCommand_ExecuteNonQuery();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbCommand_ExecuteNonQuery");
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
	
			int intRecordsAffected = 0;

			try
			{
				BeginCase("Execute Insert");
				cmd.CommandText = "INSERT INTO Employees (EmployeeID,FirstName, LastName) VALUES (99999,'OferXYZ', 'Testing')";
				intRecordsAffected = cmd.ExecuteNonQuery();
				Compare(intRecordsAffected, 1);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check insert operation");
				cmd.CommandText = "SELECT FirstName FROM Employees WHERE (EmployeeID = 99999)";
				string  strFirstName = cmd.ExecuteScalar().ToString();
				Compare(strFirstName, "OferXYZ");
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Execute Select");
				cmd.CommandText = "SELECT EmployeeID FROM Employees WHERE (EmployeeID = 99999)";
				intRecordsAffected = cmd.ExecuteNonQuery();
				
				switch (DataProvider.GetDbType())
				{
					case DataBaseServer.PostgreSQL:
						// postgres odbc returns 1
						if (!GHTEnvironment.IsJavaRunTime())
						{
							Compare(intRecordsAffected, 1);
						}
						else
						{
							Compare(intRecordsAffected, -1);
						}
						break;
					default:
						Compare(intRecordsAffected, -1);
						break;
				}
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Execute UPDATE");
				cmd.CommandText = "UPDATE Employees SET FirstName = 'OferABC', LastName = 'TestXYZ' WHERE (EmployeeID = 99999)";
				intRecordsAffected = cmd.ExecuteNonQuery();
				Compare(intRecordsAffected, 1);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}


			try
			{
				BeginCase("Check Update operation");
				cmd.CommandText = "SELECT FirstName FROM Employees WHERE (EmployeeID = 99999)";
				string  strFirstName = cmd.ExecuteScalar().ToString();
				Compare(strFirstName, "OferABC");
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Execute UPDATE");
				cmd.CommandText = "DELETE FROM Employees WHERE (EmployeeID = 99999)";
				intRecordsAffected = cmd.ExecuteNonQuery();
				Compare(intRecordsAffected, 1);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check Delete operation");
				cmd.CommandText = "SELECT FirstName FROM Employees WHERE (EmployeeID = 99999)";
				object obj = cmd.ExecuteScalar();
				Compare(obj==null, true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check OleDBException - update with bad value");
				cmd.CommandText = "UPDATE Employees SET BirthDate = 'bad value' WHERE (EmployeeID = 1)";
				try
				{
					cmd.ExecuteNonQuery(); 
				}
				catch (OleDbException ex)
				{
					exp = ex;
				}
				Compare(exp.GetType().FullName, typeof(OleDbException).FullName);
				exp=null;
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check OleDBException - missing EmployeeID");
				cmd.CommandText = "INSERT INTO Employees (FirstName, BirthDate) VALUES ('Dado', 'Ben David')";
				try
				{
					cmd.ExecuteNonQuery(); 
				}
				catch (OleDbException ex)
				{
					exp = ex;
				}
				Compare(exp.GetType().FullName, typeof(OleDbException).FullName);
				exp=null;
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

		}
	}
}
