using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class TestId13266 : GHTBase
{
	[Test] public void Main()
	{
		TestId13266 tc = new TestId13266();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbCommand_Transaction");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{

		Exception exp = null;
		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
		con.Open();
		OleDbTransaction txn = con.BeginTransaction();
		OleDbCommand cmd = new OleDbCommand("Select * From Employees", con);

		try
		{
			BeginCase("check Transaction property - default");
			Compare(cmd.Transaction==null , true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		cmd.Transaction = txn;

		try
		{
			BeginCase("check Transaction property");
			Compare(cmd.Transaction , txn);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}
		
		if (con.State == ConnectionState.Open) con.Close();

	}
}
}