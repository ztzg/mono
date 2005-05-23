using NUnit.Framework;

using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class TestId13294 : GHTBase
{
	[Test] public void Main()
	{
		TestId13294 tc = new TestId13294();
		Exception exp = null;
		try
		{
			tc.BeginTest("OleDbDataReader_GetSchemaTable");
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
		DataTable tbl = rdr.GetSchemaTable();

		//check that all the columns properties (according to .Net) exists (GH give more properties)

		try
		{
			BeginCase("ColumnName");
				Compare(tbl.Columns.Contains("ColumnName"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("ColumnOrdinal");
			Compare(tbl.Columns.Contains("ColumnOrdinal"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("ColumnSize");
			Compare(tbl.Columns.Contains("ColumnSize"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("NumericPrecision");
			Compare(tbl.Columns.Contains("NumericPrecision"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("NumericScale");
			Compare(tbl.Columns.Contains("NumericScale"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("DataType");
			Compare(tbl.Columns.Contains("DataType"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("ProviderType");
			Compare(tbl.Columns.Contains("ProviderType"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("IsLong");
			Compare(tbl.Columns.Contains("IsLong"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("AllowDBNull");
			Compare(tbl.Columns.Contains("AllowDBNull"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("IsReadOnly");
			Compare(tbl.Columns.Contains("IsReadOnly"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("IsRowVersion");
			Compare(tbl.Columns.Contains("IsRowVersion"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("IsUnique");
			Compare(tbl.Columns.Contains("IsUnique"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("IsKey");
			Compare(tbl.Columns.Contains("IsKey"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("IsAutoIncrement");
			Compare(tbl.Columns.Contains("IsAutoIncrement"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("BaseSchemaName");
			Compare(tbl.Columns.Contains("BaseSchemaName"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("BaseTableName");
			Compare(tbl.Columns.Contains("BaseTableName"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}

		try
		{
			BeginCase("BaseColumnName");
			Compare(tbl.Columns.Contains("BaseColumnName"),true );
		} 
		catch(Exception ex){exp = ex;}
		finally{EndCase(exp); exp = null;}


		if (con.State == ConnectionState.Open) con.Close();
	
		}
	}


}