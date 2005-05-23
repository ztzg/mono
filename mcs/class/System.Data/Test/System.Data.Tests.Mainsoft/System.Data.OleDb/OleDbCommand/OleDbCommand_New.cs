using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbCommand_New : GHTBase
{
	[Test] public void Main()
	{
		OleDbCommand_New tc = new OleDbCommand_New();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDBCommand_New");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{
		Exception exp = null;

		OleDbCommand cmd = null;
		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString );
		OleDbTransaction tran;


		try
		{
			BeginCase("OleDbCommand New");
			cmd = new OleDbCommand();
			Compare(cmd==null, false);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("OleDbCommand - new CommandText");
			cmd = new OleDbCommand("Select * from Table");
			Compare(cmd==null, false);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("OleDbCommand CommandText");
			Compare(cmd.CommandText , "Select * from Table");
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("OleDbCommand - Connection");
			cmd = new OleDbCommand("Select * from Table",con);
			Compare(cmd.Connection ,con);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		con.Open();
		tran = con.BeginTransaction();

		try
		{
			BeginCase("OleDbCommand - Transaction");
			cmd = new OleDbCommand("Select * from Table",con,tran);
			Compare(cmd.Transaction ,tran);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}
		
		if (con.State == ConnectionState.Open) con.Close();

	}
}

}