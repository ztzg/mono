using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbDataReader_Read : ADONetTesterClass 
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
				//prepare data
				base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		[TearDown]
		public void TearDown()
		{
		}

		public static void Main()
		{
			OleDbDataReader_Read tc = new OleDbDataReader_Read();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_Read");
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

			cmd.CommandText = "Select EmployeeID, LastName, FirstName, Title, BirthDate From Employees where EmployeeID in (100,200) order by EmployeeID asc";
			OleDbDataReader rdr = cmd.ExecuteReader();

			try
			{
				BeginCase("first row");
				bool read = rdr.Read();
				Compare(read, true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}


			try
			{
				BeginCase("first row - value");
				object obj = rdr.GetValue(0);
				Compare(obj.ToString(), "100");
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}


			try
			{
				BeginCase("Second row");
				bool read = rdr.Read();
				Compare(read, true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Second row - value");
				object obj = rdr.GetValue(0);
				Compare(obj.ToString(), "200");
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("End of data");
				bool read = rdr.Read();
				Compare(read, false);
				rdr.Close();
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Read return false");
				cmd.CommandText= "select * from Orders where OrderID=-909";
				rdr = cmd.ExecuteReader();
				Compare(rdr.Read(),false);
				rdr.Close();
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

		}
	}
}
