using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbConnection_DataSource : ADONetTesterClass 
{
	[Test] public void Main()
	{
		OleDbConnection_DataSource tc = new OleDbConnection_DataSource();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbConnection_DataSource");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{
		Exception exp = null;
		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);

		//test does not apply to ORACLE,DB2,Postgres
		if (DataProvider.GetDbType(con) == DataBaseServer.Oracle) return;
		if (DataProvider.GetDbType(con) ==  DataBaseServer.DB2) return;
		if (DataProvider.GetDbType(con) == DataBaseServer.PostgreSQL) return;

		//get the expected result from the connection string
		string[] arrCon = con.ConnectionString.Split(';');
		string result = null;
		for (int i=0; i < arrCon.Length; i++)
			if (arrCon[i].IndexOf("Data Source") >= 0) 
			{
				result = arrCon[i];
				break;
			}
		result = result.Substring(result.IndexOf('=')+1).Trim();
		try
		{
			BeginCase("check DataSource");
			Compare(con.DataSource  , result);
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
