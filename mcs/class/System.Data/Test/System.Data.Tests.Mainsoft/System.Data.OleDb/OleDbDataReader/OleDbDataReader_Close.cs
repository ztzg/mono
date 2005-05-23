using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbDataReader_Close : GHTBase
{
	[Test] public void Main()
	{
		OleDbDataReader_Close tc = new OleDbDataReader_Close();
		Exception exp = null;
		try
		{
			tc.BeginTest("DataReader.IsClosed");
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
		OleDbCommand cmd = new OleDbCommand("Select * From Customers", con);
		OleDbDataReader rdr = cmd.ExecuteReader();

		try
		{
			BeginCase("before closing");
			Compare(rdr.IsClosed ,false);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}


		try
		{
			BeginCase("after closing");
			rdr.Close();
			Compare(rdr.IsClosed ,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		if (con.State == ConnectionState.Open) con.Close();


	}


	//public TestClass():base(true){}

	//Activate this constructor to log Failures to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, false){}

	//Activate this constructor to log All to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, true){}

	//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES

}


}