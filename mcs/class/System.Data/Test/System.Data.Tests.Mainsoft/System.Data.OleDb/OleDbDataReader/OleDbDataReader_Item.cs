using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbDataReader_Item : ADONetTesterClass 
{
	[Test] public void Main()
	{
		OleDbDataReader_Item tc = new OleDbDataReader_Item();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbDataReader_Item");
			tc.run();
		}
		catch(Exception ex){exp = ex;}
		finally	{tc.EndTest(exp);}
	}

	public void run()
	{
		Exception exp = null;


		//prepare data



		OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
		OleDbCommand cmd = new OleDbCommand("select ProductID,ProductName,Discontinued from Products where ProductID=1", con);
		con.Open();
		OleDbDataReader rdr =  cmd.ExecuteReader();

		rdr.Read();
		object obj = null;

		switch (DataProvider.GetDbType(con)) 
		{
			case DataBaseServer.DB2:
			case DataBaseServer.SQLServer:
			case DataBaseServer.PostgreSQL:
				try
				{
					BeginCase("Column int - type");
					obj = rdr["ProductID"];
					Compare(obj.GetType().FullName , typeof(int).FullName);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}
				break;
			case DataBaseServer.Oracle:
			case DataBaseServer.Sybase:
				try
				{
					BeginCase("Column Decimal - type");
					obj = rdr["ProductID"];
					Compare(obj.GetType().FullName , typeof(decimal).FullName);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}
				break;
		}

		try
		{
			BeginCase("Column int - value");
			Compare(obj.ToString(),"1");
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("Column string - type");
			obj = rdr["ProductName"];
			Compare(obj.GetType().FullName , typeof(string).FullName);
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("Column string - value");
			Compare(obj.ToString(),"Chai");
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		switch (DataProvider.GetDbType(con)) 
		{
			case DataBaseServer.DB2 :
				try
				{
					BeginCase("Column Int16 - type");
					obj = rdr["Discontinued"];
					Compare(obj.GetType().FullName , typeof(System.Int16).FullName);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}

				try
				{
					BeginCase("Column Int16 - value");
					Compare(obj.ToString(),"0");
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}
				break;
			case DataBaseServer.SQLServer :
				try
				{
					BeginCase("Column bool - type");
					obj = rdr["Discontinued"];
					Compare(obj.GetType().FullName , typeof(bool).FullName);
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}

				try
				{
					BeginCase("Column bool - value");
					Compare(obj.ToString().ToUpper(),"FALSE");
				} 
				catch(Exception ex){exp = ex;}
				finally{EndCase(exp); exp = null;}
				break;
			case DataBaseServer.Oracle :
				//Column type is Decimal - already tested
				break;
		}
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
