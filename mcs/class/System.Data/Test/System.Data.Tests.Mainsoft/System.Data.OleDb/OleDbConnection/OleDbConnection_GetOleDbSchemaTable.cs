using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbConnection_GetOleDbSchemaTable : ADONetTesterClass
{
	[Test] public void Main()
	{
		OleDbConnection_GetOleDbSchemaTable tc = new OleDbConnection_GetOleDbSchemaTable();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbConnection_GetOleDbSchemaTable");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{
	
		Exception exp = null;
		//bool NextResultExists = false;

		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
		con.Open();

		DataTable dt = null;
		try
		{
			BeginCase("Check table is not null");
			dt = con.GetOleDbSchemaTable(OleDbSchemaGuid.Tables,new object[] {null,null,null,"TABLE"});
			Compare(dt == null,false);
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}



		//check that some of the tables exists
		try
		{
			BeginCase("Table Orders");
			Compare(dt.Select("TABLE_NAME='Orders'").Length ,1 );  
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}

		try
		{
			BeginCase("Table Order Details");
			Compare(dt.Select("TABLE_NAME='Order Details'").Length ,1 );
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}

		try
		{
			BeginCase("Table Customers");
			int expected = (DataProvider.GetDbType(con) != DataBaseServer.Oracle) ? 1 : 2; // in Oracle return a value of 2 instead of 1. ?????
			Compare(dt.Select("TABLE_NAME='Customers'").Length ,expected );
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}

		try
		{
			BeginCase("Table Employees");
			Compare(dt.Select("TABLE_NAME='Employees'").Length ,1 );
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}

		if (con.State == ConnectionState.Open) con.Close();
	}


		
	}


	//public TestClass():base(true){}

	//Activate this constructor to log Failures to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, false){}

	//Activate this constructor to log All to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, true){}

	//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES


}
