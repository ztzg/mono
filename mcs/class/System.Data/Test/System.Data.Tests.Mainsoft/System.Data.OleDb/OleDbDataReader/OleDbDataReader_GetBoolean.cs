using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbDataReader_GetBoolean : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbDataReader_GetBoolean tc = new OleDbDataReader_GetBoolean();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetBoolean");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;
		
			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);

			// PostgreSQL ODBC treats Type BOOL as String, so we don't run it on .NET
			if ((DataProvider.GetDbType(con) == DataBaseServer.PostgreSQL) && (!GHTEnvironment.IsJavaRunTime()))
			{
				return; 
			}

			//Do not test with oracle or DB2, because boolean does not exist in their types.
			if ( (DataProvider.GetDbType(con) == DataBaseServer.Oracle) || (DataProvider.GetDbType(con) == DataBaseServer.DB2) )
			{
				return; 
			}

			con.Open();
			OleDbCommand cmd = new OleDbCommand("Select t_bool From TYPES_SIMPLE Where id = '0'",con);
			OleDbDataReader rdr = cmd.ExecuteReader();
			rdr.Read();

			try
			{
				BeginCase("GetBoolean true");
				Boolean blnValue;
				Compare(rdr.IsDBNull(0), false);
				Compare("System.Boolean", rdr.GetValue(0).GetType().FullName);
				blnValue = rdr.GetBoolean(0);
				Compare(blnValue, true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			rdr.Close();
              
			
			if (con.State == ConnectionState.Open) con.Close();

		}
	}
}
