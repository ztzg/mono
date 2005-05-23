using NUnit.Framework;


using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbDataAdapter_Dispose : GHTBase
{
	[Test] public void Main()
	{
		OleDbDataAdapter_Dispose tc = new OleDbDataAdapter_Dispose();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbDataAdapter_Dispose");
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


	//public TestClass():base(true){}

	//Activate this constructor to log Failures to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, false){}


	//Activate this constructor to log All to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, true){}

	//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES

	public void run()
	{
		Exception exp = null;

		DataSet ds = new DataSet();
		string sqlSelect = "Select * from Customers where CustomerID = 'ABC'";
		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
		
		OleDbCommand cmdSelect = new OleDbCommand(sqlSelect,con);
		OleDbCommand cmdDelete = new OleDbCommand("",con);
		OleDbCommand cmdUpdate = new OleDbCommand("",con);
		OleDbCommand cmdInsert = null;

		con.Open();

		OleDbDataAdapter da = new OleDbDataAdapter(cmdSelect);
		da.DeleteCommand = cmdDelete;
		da.UpdateCommand = cmdUpdate;
		da.InsertCommand = new OleDbCommand("",con);

		cmdInsert =	da.InsertCommand;

		da.Fill(ds);

		try
		{
			BeginCase("Dispose - check DataAdapter select command");
			da.Dispose();
			Compare(da.SelectCommand == null, true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check DataAdapter insert command");
			Compare(da.InsertCommand == null, true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check DataAdapter Delete Command");
			Compare(da.DeleteCommand == null, true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check DataAdapter update command");
			Compare(da.UpdateCommand == null, true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}



		try
		{
			BeginCase("Dispose - check select command object");
			Compare(cmdSelect != null, true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check delete command object");
			Compare(cmdDelete  != null, true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check insert command object");
			Compare(cmdInsert != null, true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check Update command object");
			Compare(cmdUpdate != null, true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check cmdSelect.connection object");
			Compare(cmdSelect.Connection != null ,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check cmdDelete.connection object");
			Compare(cmdDelete.Connection != null ,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check cmdUpdate.connection object");
			Compare(cmdUpdate.Connection != null ,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check cmdInsert.connection object");
			Compare(cmdInsert.Connection != null ,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

		try
		{
			BeginCase("Dispose - check command connection state");
			da.Dispose();
			Compare(cmdSelect.Connection.State ,ConnectionState.Open);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp);exp = null;}

	}
}
}