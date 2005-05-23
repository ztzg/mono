using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbConnection_New : GHTBase
{
	[Test] public void Main()
	{
		OleDbConnection_New tc = new OleDbConnection_New();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbConnection_New");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{
		Exception exp = null;
		OleDbConnection con = null;
		try
		{
			BeginCase("OleDbConnection ctor");
			con = new OleDbConnection();
			Compare(con != null ,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("OleDbConnection ctor connection string");
			con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			Compare(con != null ,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("OleDbConnection check connection string");
			Compare(con.ConnectionString  ,GHTUtils.DataProvider.ConnectionString  );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

	}


	//public TestClass():base(true){}

	//Activate this constructor to log Failures to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, false){}

	//Activate this constructor to log All to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, true){}

	//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES

}
}