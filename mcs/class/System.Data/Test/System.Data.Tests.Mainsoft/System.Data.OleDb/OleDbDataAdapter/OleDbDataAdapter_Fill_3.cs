using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbDataAdapter_Fill_3: ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbDataAdapter_Fill_3 tc = new OleDbDataAdapter_Fill_3();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataAdapter_Fill_3");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;

			//in DB2 when trying to fill an empty table - no table is loaded
			System.Data.OleDb.OleDbConnection conn = new System.Data.OleDb.OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			System.Data.OleDb.OleDbDataAdapter oleDBda = new System.Data.OleDb.OleDbDataAdapter();
			oleDBda.SelectCommand = new System.Data.OleDb.OleDbCommand("Select * from GH_EMPTYTABLE",conn);
			
			DataSet ds = new DataSet();
			oleDBda.Fill(ds);
        
			try
			{
				BeginCase("Table count - fill with SP");
				Compare(ds.Tables.Count ,1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//add for bug #2508 - OLEDBDataAdapter.Fill fills only the 1st result set, reported from an evaluation
			if (DataProvider.GetDbType(oleDBda.SelectCommand.Connection) == DataBaseServer.SQLServer)
				//multiple commands can not be done with Oracle or DB2
			{


				//get excpected results
				if (oleDBda.SelectCommand.Connection.State != ConnectionState.Open)
				{
					oleDBda.SelectCommand.Connection.Open();
				}
				OleDbCommand cmd = new OleDbCommand("",oleDBda.SelectCommand.Connection);
				cmd.CommandText = "Select count(*) from Customers";
				int TblResult0 = (int)cmd.ExecuteScalar();
				cmd.CommandText = "Select count(*) from Categories";
				int TblResult1 = (int)cmd.ExecuteScalar();
				cmd.CommandText = "Select count(*) from Region";
				int TblResult2 = (int)cmd.ExecuteScalar();
				if (oleDBda.SelectCommand.Connection.State != ConnectionState.Closed)
				{
					oleDBda.SelectCommand.Connection.Close();
				}


				oleDBda.SelectCommand.CommandText = "Select * from Customers; " +
					"Select * from Categories; " +
					"Select * from Region";
				ds = new DataSet();
				oleDBda.Fill(ds);

				try
				{
					BeginCase("Table count - Fill with query");
					Compare(ds.Tables.Count ,3 );
				}
				catch(Exception ex)	{exp = ex;}
				finally	{EndCase(exp); exp = null;}

				try
				{
					BeginCase("Table 0 rows count");
					Compare(ds.Tables[0].Rows.Count ,TblResult0 );
				}
				catch(Exception ex)	{exp = ex;}
				finally	{EndCase(exp); exp = null;}

				try
				{
					BeginCase("Table 1 rows count");
					Compare(ds.Tables[1].Rows.Count ,TblResult1 );
				}
				catch(Exception ex)	{exp = ex;}
				finally	{EndCase(exp); exp = null;}

				try
				{
					BeginCase("Table 2 rows count");
					Compare(ds.Tables[2].Rows.Count ,TblResult2 );
				}
				catch(Exception ex)	{exp = ex;}
				finally	{EndCase(exp); exp = null;}

			}

 
		}
	}
}
