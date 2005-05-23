using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbDataReader_GetOrdinal : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbDataReader_GetOrdinal tc = new OleDbDataReader_GetOrdinal();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetOrdinal");
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
				BeginCase("column REGION ordinal");
				Compare(rdr.GetOrdinal("REGION"),6 );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("column not exists");
				try
				{
					int i = rdr.GetOrdinal("blabla");
				}
				catch (Exception ex) {exp=ex;}
				Compare(exp.GetType().FullName,typeof(IndexOutOfRangeException).FullName);
				exp=null;
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();

		}
	}
}
