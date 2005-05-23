using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbDataReader_RecordsAffected  : ADONetTesterClass 
	{
		[Test] public void Main()
		{
			OleDbDataReader_RecordsAffected tc = new OleDbDataReader_RecordsAffected();
			Exception exp = null;
			try
			{
				tc.BeginTest("RecordsAffected");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;

			//prepare data
			base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);

			int intRecordsAffected = 0;
			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			con.Open();
			OleDbCommand cmd = new OleDbCommand("Update Employees set Title = 'title' where EmployeeID = 100", con);
			OleDbDataReader rdr = cmd.ExecuteReader();
			rdr.Read();
			intRecordsAffected = rdr.RecordsAffected;

			try
			{
				BeginCase("RecordsAffected");
				Compare(intRecordsAffected,1 );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();
		}
	}
}

