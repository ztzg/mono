using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbConnection_ChangeDatabase : ADONetTesterClass 
{
	[Test] public void Main()
	{
		OleDbConnection_ChangeDatabase tc = new OleDbConnection_ChangeDatabase();
		Exception exp = null;
		try
		{
			tc.BeginTest("NoName");
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

		//Currently not running on Oracle/DB2: .Net-Failed, GH:Pass
		//not runnig on postgres because "The 'current catalog' property is not supported by the 'Mainsoft.JDBC.OLEDB.1' provider."
		if (DataProvider.GetDbType(con) != DataBaseServer.Oracle  &&
			DataProvider.GetDbType(con) != DataBaseServer.DB2 && 
			DataProvider.GetDbType(con) != DataBaseServer.Sybase &&
			DataProvider.GetDbType(con) != DataBaseServer.PostgreSQL)
		{
			try
			{
				BeginCase("Change DataBase");
				con.ChangeDatabase("GHTDB_EX");
				Compare(con.Database , "GHTDB_EX");
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check DataBase Changed ");
				OleDbCommand cmd = new OleDbCommand("select count(*) from Customers", con);
				object obj = cmd.ExecuteScalar();
				Compare(obj != null, true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

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
}
