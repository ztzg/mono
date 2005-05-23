using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbDataReader_GetChars : ADONetTesterClass 
	{
		OleDbConnection con;
		char [] Result;

		[SetUp]
		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				//prepare data
				base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);

				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
				Result = new char[100];
				con.Open();
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		[TearDown]
		public void TearDown()
		{
			if (con.State == ConnectionState.Open) con.Close();
		}

		public static void Main()
		{
			OleDbDataReader_GetChars tc = new OleDbDataReader_GetChars();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetChars");
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
			long rdrResults = 0;

			OleDbCommand cmd = new OleDbCommand("Select LastName From Employees Where EmployeeID = 100", con);
			OleDbDataReader rdr = cmd.ExecuteReader();
			rdr.Read();

			//LastName should be "Last100"
	
			try
			{
				BeginCase("check result length");
				rdrResults = rdr.GetChars(0, 0, Result, 0, Result.Length);
				Compare(rdrResults,(long)"Last100".Length  );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("check result - char[0]");
				Compare(Result[0] ,'L');
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("check result - char[last char index]");
				Compare(Result["Last100".Length-1] ,'0');
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

		}
	}
}
