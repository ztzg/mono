using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbConnection_StateChange : GHTBase
{
	[Test] public void Main()
	{
		OleDbConnection_StateChange tc = new OleDbConnection_StateChange();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbConnection_StateChange");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}


	bool blnEventRaised = false;
	ConnectionState OriginalState,CurrentState;
	public void run()
	{
		
		Exception exp = null;

		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
        
		// ----------- reserved for future versions of the product ---------------
		//Broken	The connection to the data source is broken. This can occur only after the connection has been opened. A connection in this state may be closed and then re-opened. (This value is reserved for future versions of the product).
		//Connecting  The connection object is connecting to the data source. (This value is reserved for future versions of the product.) 2 
		//Executing The connection object is executing a command. (This value is reserved for future versions of the product.) 4 
		//Fetching  The connection object is retrieving data. (This value is reserved for future versions of the product.) 8 

		//-------------- checking only the following: ----------------
		//Closed  The connection is closed.  
		//Open  The connection is open. 


		//add event handler
		con.StateChange +=new StateChangeEventHandler(con_StateChange);

		con.Open();
		try
		{
			BeginCase("ConnectionState Closed");
			Compare(blnEventRaised,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("OriginalState Closed");
			Compare(OriginalState,ConnectionState.Closed );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("CurrentState Open");
			Compare(CurrentState,ConnectionState.Open );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		blnEventRaised = false;
		con.Close();
		try
		{
			BeginCase("ConnectionState Open");
			Compare(blnEventRaised,true);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("OriginalState Open");
			Compare(OriginalState,ConnectionState.Open );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("CurrentState Close");
			Compare(CurrentState,ConnectionState.Closed  );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		if (con.State == ConnectionState.Open) con.Close();
	}

	void con_StateChange(Object sender, System.Data.StateChangeEventArgs e)
	{
		CurrentState = e.CurrentState ;
		OriginalState = e.OriginalState ;
		blnEventRaised = true;


	}

	//public TestClass():base(true){}

	//Activate this constructor to log Failures to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, false){}

	//Activate this constructor to log All to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, true){}

	//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES

}
}