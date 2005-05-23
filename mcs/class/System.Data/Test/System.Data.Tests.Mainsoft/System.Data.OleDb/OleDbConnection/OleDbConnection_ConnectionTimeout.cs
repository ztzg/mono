using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;
using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbConnection_ConnectionTimeout : GHTBase
{
	[Test] public void Main()
	{
		OleDbConnection_ConnectionTimeout tc = new OleDbConnection_ConnectionTimeout();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbConnection_ConnectionTimeout");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{
		Exception exp = null;

		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString + ";Connect timeout=44");

		try
		{
			BeginCase("ConnectionTimeout from Connection string");
			Compare(con.ConnectionTimeout , 44);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);

		try
		{
			BeginCase("ConnectionTimeout default");
			Compare(con.ConnectionTimeout , 15);
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