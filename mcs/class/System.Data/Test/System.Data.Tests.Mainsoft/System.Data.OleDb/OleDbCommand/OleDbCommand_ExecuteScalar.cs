using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbCommand_ExecuteScalar : GHTBase
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
				con.Open();

				//prepare Data
				OleDbCommand cmdPrepare = new OleDbCommand("", con);
				cmdPrepare.CommandText = "DELETE FROM Employees WHERE EmployeeID = 99999";
				cmdPrepare.ExecuteNonQuery();
				cmdPrepare.CommandText = "INSERT INTO Employees (EmployeeID, FirstName, LastName) VALUES (99999,'OferXYZ', 'Testing')";
				cmdPrepare.ExecuteNonQuery();
				cmdPrepare.Dispose();

				cmd = new OleDbCommand("", con);
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
			OleDbCommand_ExecuteScalar tc = new OleDbCommand_ExecuteScalar();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbCommand_ExecuteScalar");
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
			object objResult = null;

			cmd.CommandText="Select FirstName,City From Employees Where EmployeeID=-1";

			try
			{
				BeginCase("Execute Scalar");
				objResult = cmd.ExecuteScalar();
				Compare(objResult==null,true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			cmd.CommandText="Select FirstName,City From Employees Where EmployeeID=99999 ";
		
			try
			{
				BeginCase("Execute Scalar");
				objResult = cmd.ExecuteScalar();
				Compare(objResult.ToString(), "OferXYZ");
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}


			try
			{
				BeginCase("Execute Scalar again");
				objResult = cmd.ExecuteScalar();
				Compare(objResult.ToString(), "OferXYZ");
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

		}
	}
}
