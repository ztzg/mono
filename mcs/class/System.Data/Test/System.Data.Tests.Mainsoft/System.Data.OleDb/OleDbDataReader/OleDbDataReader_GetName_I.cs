using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbDataReader_GetName_I : GHTBase
{
	[Test] public void Main()
	{
		OleDbDataReader_GetName_I tc = new OleDbDataReader_GetName_I();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbDataReader_NextResult");
				tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{
		Exception exp = null;
		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
		OleDbCommand cmd = new OleDbCommand("Customers",  con);
		cmd.CommandType = System.Data.CommandType.TableDirect;

		con.Open();

		OleDbDataReader rdr = cmd.ExecuteReader();
		rdr.Read();


		try
		{
			BeginCase("GetName field 0");
			string str = rdr.GetName(0);
			Compare(str.ToUpper(),"CUSTOMERID" );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("GetName last field ");
			string str = rdr.GetName(rdr.FieldCount -1);
			Compare(str.ToUpper(),"FAX" );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		if (con.State == ConnectionState.Open) con.Close();
	}
}











}