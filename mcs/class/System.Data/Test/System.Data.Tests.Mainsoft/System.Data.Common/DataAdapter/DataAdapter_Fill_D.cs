using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;
using System.Data.SqlClient;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_Common
{
	[TestFixture] public class DataAdapter_Fill_D : ADONetTesterClass
	{
		SqlConnection con;
		//OleDbCommand cmd;
		//OleDbDataReader dr = null;

		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				Compare("Setup", "Setup");
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		public void TearDown()
		{
		}
		[Test] public void Main()
		{
			DataAdapter_Fill_D tc = new DataAdapter_Fill_D();
			Exception exp = null;
			try
			{
				tc.BeginTest("DataAdapter_Fill_D");
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

			#region		---- Bug 3060 - MSSQL - DataSet Date Null values ---- 

			// testing only SQLServerr
			if (DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionStringSQLClient) == DataBaseServer.SQLServer)
			{
				try
				{
					BeginCase("Bug 3060 - MSSQL - DataSet Date Null values");

					con = new SqlConnection(GHTUtils.DataProvider.ConnectionStringSQLClient);
					con.Open();

					SqlCommand cmd = new SqlCommand("INSERT INTO Employees(EmployeeID,LASTNAME,FIRSTNAME) VALUES(49298,'49298','49298')",con);
					cmd.ExecuteNonQuery();

					cmd = new SqlCommand("select BIRTHDATE from Employees where EmployeeID = 49298",con);
					SqlDataAdapter da = new SqlDataAdapter(cmd);
					DataSet ds = new DataSet();

					da.Fill(ds);
					Compare(ds.Tables[0].Rows[0][0].ToString(),"");

					cmd = new SqlCommand("DELETE FROM Employees WHERE EmployeeID=49298",con);
					cmd.ExecuteNonQuery();
				} 
				catch(Exception ex)
				{
					exp = ex;
				}
				finally
				{
					//if (dr != null)dr.Close();
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
