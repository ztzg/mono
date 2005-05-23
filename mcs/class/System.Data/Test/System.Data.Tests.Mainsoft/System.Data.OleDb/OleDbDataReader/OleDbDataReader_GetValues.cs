using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbDataReader_GetValues : ADONetTesterClass 
	{
		[Test] public void Main()
		{
			OleDbDataReader_GetValues tc = new OleDbDataReader_GetValues();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetValues");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;
			int intValuesCount = 0;

			//prepare data
			base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);

			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			con.Open();
			OleDbCommand cmd = new OleDbCommand("Select CustomerID, ContactName , CompanyName From Customers where CustomerID = 'GH100'", con);
			OleDbDataReader rdr = cmd.ExecuteReader();
			rdr.Read();

			object [] values = null;


			//------ check big array
			try
			{
				BeginCase("GetValues - bigger array - check count");
				values = new object[50];
				intValuesCount = rdr.GetValues(values);
				Compare(intValuesCount ,3 );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("GetValues - bigger array - check CustomerID");
				Compare(values[0].ToString().Trim() ,"GH100" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("GetValues - bigger array - check CompanyName");
				Compare(values[2].ToString() ,"Company100" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("GetValues - bigger array - check DBNull");
				Compare(values[3] == null ,true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			//------ check small array
			try
			{
				BeginCase("GetValues - smaller array - check count");
				values = new object[2];
				intValuesCount = rdr.GetValues(values);
				Compare(intValuesCount ,2 );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("GetValues - smaller array - check CustomerID");
				Compare(values[0].ToString().Trim() ,"GH100" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}


			//------ check exact array
			try
			{
				BeginCase("GetValues - exact array - check count");
				values = new object[3];
				intValuesCount = rdr.GetValues(values);
				Compare(intValuesCount ,3 );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("GetValues - exact array - check CustomerID");
				Compare(values[0].ToString().Trim() ,"GH100" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("GetValues - exact array - check CompanyName");
				Compare(values[2].ToString() ,"Company100" );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();


		}
	}
}
