using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbConnection_Equals : GHTBase
{
	[Test] public void Main()
	{
		OleDbConnection_Equals tc = new OleDbConnection_Equals();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbConnection_Equals");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{
		Exception exp = null;
		OleDbConnection con1 ,con2,con3;
        
		con1 = new OleDbConnection(GHTUtils.DataProvider.ConnectionString );
		con3 = new OleDbConnection(GHTUtils.DataProvider.ConnectionString );

		con2 = con1;
        con1.Open ();

		try
		{
			BeginCase("Equals 1");
			Compare(con1.Equals(con2),true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("Equals 2");
			Compare(Equals(con2,con1),true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("Equals 3");
			Compare(con1.Equals(con3),false);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		con1.Close();

	}


	//public TestClass():base(true){}

	//Activate this constructor to log Failures to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, false){}

	//Activate this constructor to log All to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, true){}

	//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES

}
}