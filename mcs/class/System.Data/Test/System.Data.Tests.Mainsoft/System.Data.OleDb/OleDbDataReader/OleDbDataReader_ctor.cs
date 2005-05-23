using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbDataReader_ctor : GHTBase
{
	[Test] public void Main()
	{
		OleDbDataReader_ctor tc = new OleDbDataReader_ctor();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbDataReader_ctor");
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
		OleDbCommand cmd = new OleDbCommand("Select * From Orders", con);
		OleDbDataReader rdr = cmd.ExecuteReader();

		//change a connection's state without closing the datareader (should fail
		try
		{
			BeginCase("InvalidOperationException");
			try
			{
				con.ChangeDatabase("msdb");
				ExpectedExceptionNotCaught(typeof(InvalidOperationException).FullName);
			}
			catch (InvalidOperationException ex) 
			{
				ExpectedExceptionCaught(ex);
			}
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		if (con.State == ConnectionState.Open) con.Close();
	}
}
}