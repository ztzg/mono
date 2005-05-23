using NUnit.Framework;

using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbCommand_Dispose : GHTBase
{
	[Test] public void Main()
	{
		OleDbCommand_Dispose tc = new OleDbCommand_Dispose();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDBCommand_Dispose");
			tc.run();
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

	//problems from Synergy

	//Ofer, 

	//Take a look at #1. Test this to see if we have a problem in dispose. If we do, add a test for this to make sure we catch it in next rounds. 

	//

	//-----Original Message-----
	//From: David Teplitchi [mailto:davidt@mainsoft.com] 
	//Sent: Sunday, March 21, 2004 9:31 AM
	//To: //Oved Yavine//
	//Subject: 2 problems from Synergy


	//Please check those 2 problems reported by Synergy and tell me what do you think.

	//1) The following code works in dotnet but doesn//t work in j2ee.
	//// Oracle Drivers
	//OleDbConnection Connect;
	//OleDbDataReader DbReader;
	//OleDbCommand DbCommand;
	//IDataReader DR;
	//string sSQL;
	//int iField;
	//bool bFound;
	//try
	//{
	//Connect = new OleDbConnection("Provider=\"MSDAORA.1\";User
	//ID=COM;Password=SQL;Data Source=LLO1;HostName=WS10359;Port=1521");
	//Connect.Open();
	//sSQL = "SELECT * FROM PRODUCT WHERE PRO_SKU = //SKU_208//";
	//DbCommand = new OleDbCommand(sSQL, Connect);
	//DbReader = DbCommand.ExecuteReader(CommandBehavior.SingleRow);
	//DR = DbReader;
	//DbCommand.Dispose(); // comment out
	////DbCommand = null;
	//bFound = DbReader.HasRows;
	//DR.Read();
	//// Get Column Ordinal
	//iField = DR.GetOrdinal("PRO_DESCRIPTION");

	//string sDesc = DR.GetString(iField);
	//}
	//catch(Exception e)
	//{
	//this.WriteErrorLog(e.Message);
	//}
	//i have identified the problem as being "DbCommand.Dispose()". If you comment this line out or make it 
	//"DbCommand = null", then its okay. So this instruction doesnt work the same in both in .net and j2ee.


	public void run()
	{
		Exception exp = null;

		//OleDbConnection  con = null;

		//this test was added due to a request from Oved:
		//this bug occur on all databases (SQL,Oracle,DB2)
		OleDbCommand DbCommand = null;
		OleDbConnection Connect = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
		Connect.Open();
		DbCommand = new OleDbCommand("SELECT * FROM Customers", Connect);
		OleDbDataReader  DbReader  = DbCommand.ExecuteReader();

		BeginCase("Check DataReader.IsClosed - before dispose");
		try
		{
			Compare(DbReader.IsClosed,false); //.Net=false, GH=false
		}
		catch (Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}


		BeginCase("Check DataReader.IsClosed - after dispose");
		try
		{
			DbCommand.Dispose();
			Compare(DbReader.IsClosed,false); //.Net=false, GH=true
		}
		catch (Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}


	}
}



}
