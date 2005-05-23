using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;
using GHTUtils.Data;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbDataReader_GetGuiid_I : ADONetTesterClass 
	{

		private const string GUID_COLUMN_NAME = "T_UNIQUEIDENTIFIER";
		private const string GUID_TABLE_NAME = DataProvider.SPECIFIC_TYPES_TABLE_NAME;
		private const string TEST_GUID_STRING = "239A3C5E-8D41-11D1-B675-00C04FA3C554";

		[Test] public void Main()
		{
			OleDbDataReader_GetGuiid_I tc = new OleDbDataReader_GetGuiid_I();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetGuiid_I");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			//Only apply to MSSQL
			if ( (DataProvider.GetDbType() != DataBaseServer.SQLServer))
			{
				return;
			}
		
			DoTestUsingSQLTextOnly();
			DoTestUsingParametersArray();
		}

		public void DoTestUsingSQLTextOnly()
		{
			Exception exp = null;
			OleDbDataReader rdr = null;
			OleDbConnection con = null;
			OleDbCommand cmdDelete = null;
			try
			{
				BeginCase("Test using SQL text only.");
				string rowId = "43973_" + TestCaseNumber.ToString();
				string insertText = string.Format("INSERT INTO {0} (ID, {1}) VALUES ('{2}', '{{{3}}}')", GUID_TABLE_NAME, GUID_COLUMN_NAME, rowId, TEST_GUID_STRING);
				string selectText = string.Format("SELECT {0} FROM {1} WHERE ID='{2}'", GUID_COLUMN_NAME, GUID_TABLE_NAME, rowId);
				string deleteText = string.Format("DELETE FROM {0} WHERE ID='{1}'", GUID_TABLE_NAME, rowId);
				con = new OleDbConnection(DataProvider.ConnectionString);
				OleDbCommand cmdInsert = new OleDbCommand(insertText, con);
				OleDbCommand cmdSelect = new OleDbCommand(selectText, con);
				cmdDelete = new OleDbCommand(deleteText, con);

				con.Open();
				cmdInsert.ExecuteNonQuery();
				rdr = cmdSelect.ExecuteReader();
				rdr.Read();
				Guid  guidValue = rdr.GetGuid (0);
				Guid origGuid = new Guid(TEST_GUID_STRING);
				Compare(guidValue, origGuid);
			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				exp = null;
				if ( (rdr != null) && (!rdr.IsClosed) )
				{
					rdr.Close();
				}
				if (cmdDelete != null)
				{
					cmdDelete.ExecuteNonQuery();
				}
				if ( (con != null) && (con.State != ConnectionState.Closed) )
				{
					con.Close();
				}
			}
		}

		public void DoTestUsingParametersArray()
		{
			Exception exp = null;
			OleDbDataReader rdr = null;
			OleDbConnection con = null;
			DbTypeParametersCollection row = new DbTypeParametersCollection(GUID_TABLE_NAME);
			string rowId = string.Empty;
			try
			{
				BeginCase("Test using parameters array");
				rowId = "43973_" + TestCaseNumber.ToString();
				row.Add("UNIQUEIDENTIFIER", new Guid(TEST_GUID_STRING));
				row.ExecuteInsert(rowId);
				row.ExecuteSelectReader(rowId, out rdr, out con);
				rdr.Read();
				Guid  guidValue = rdr.GetGuid (0);
				Compare(guidValue, row[GUID_COLUMN_NAME].Value);
			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				exp = null;
				if ( (rdr != null) && (!rdr.IsClosed) )
				{
					rdr.Close();
				}
				if (rowId != String.Empty)
				{
					row.ExecuteDelete(rowId);
				}
				if ( (con != null) && (con.State != ConnectionState.Closed) )
				{
					con.Close();
				}
				
			}
		}
	}
	

}
