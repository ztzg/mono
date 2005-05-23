using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;
using System.Data.SqlClient;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_Common
{
	[TestFixture] public class DbDataAdapter_Update_D : ADONetTesterClass
	{
		SqlConnection conSql;

		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				if (DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionStringSQLClient) == DataBaseServer.SQLServer)
				{
					conSql = new SqlConnection(GHTUtils.DataProvider.ConnectionStringSQLClient);
					conSql.Open();

					SqlCommand Cmd = new SqlCommand("delete from Categories where CategoryName = '49406'",conSql);
					Cmd.ExecuteNonQuery();
				}
				Compare("Setup", "Setup");
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		public void TearDown()
		{
			if (DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionStringSQLClient) == DataBaseServer.SQLServer)
			{
				conSql.Open();
				SqlCommand Cmd = new SqlCommand("delete from Categories where CategoryName = '49406'",conSql);
				Cmd.ExecuteNonQuery();
				if (conSql != null)
				{
					if (conSql.State == ConnectionState.Open) conSql.Close();
				}
			}

		}
		[Test] public void Main()
		{
			DbDataAdapter_Update_D tc = new DbDataAdapter_Update_D();
			Exception exp = null;
			try
			{
				tc.BeginTest("DbDataAdapter_Update_D");
				tc.SetUp();
				tc.run();
				tc.TearDown();
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				tc.EndTest(exp);
			}
		}

		public void run()
		{
			Exception exp = null;

			#region		---- Bug 2880 - MSSQL - Update using identity column on SqlDataAdapter ---- 
			// testing only SQLServerr
			if (DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionStringSQLClient) == DataBaseServer.SQLServer)
			{
				try
				{
					BeginCase("Bug 2880 - MSSQL - Update using identity column on SqlDataAdapter");

					SqlCommand insertCmd = new SqlCommand();
					insertCmd.Connection = conSql;
					insertCmd.CommandType = CommandType.Text;
					insertCmd.CommandText = "INSERT INTO Categories(CategoryName) VALUES(@CategoryName) SELECT CategoryID, CategoryName FROM Categories WHERE CategoryID = @@IDENTITY";
					insertCmd.Parameters.Add("@CategoryName", SqlDbType.VarChar, 15, "CategoryName");

					SqlDataAdapter adapter = new SqlDataAdapter();
					adapter.InsertCommand = insertCmd;

					DataSet ds = new DataSet();
					ds.Tables.Add(new DataTable("Table"));
					ds.Tables[0].Columns.Add(new DataColumn("CategoryID", typeof(int)));
					ds.Tables[0].Columns.Add(new DataColumn("CategoryName", typeof(string)));

					ds.Tables[0].Rows.Add(new object[] {-1, "49406"});
					ds.Tables[0].Rows.Add(new object[] {-1, "49406"});
					ds.Tables[0].Rows.Add(new object[] {-1, "49406"});

					adapter.Update(ds);

					//
					// Id should get updated with the new IDENTITY value. 
					//
					object id =  ds.Tables[0].Rows[0]["CategoryID"];
					Compare(System.Convert.ToInt32(id) > 0,true);
					id =  ds.Tables[0].Rows[1]["CategoryID"];
					Compare(System.Convert.ToInt32(id) > 0,true);
					id =  ds.Tables[0].Rows[2]["CategoryID"];
					Compare(System.Convert.ToInt32(id) > 0,true);

				} 
				catch(Exception ex)
				{
					exp = ex;
				}
				finally
				{
					//if (dr != null)dr.Close();
					if (conSql != null)
					{if (conSql.State == ConnectionState.Open) conSql.Close();}

					EndCase(exp);
					exp = null;
				}

			}
			#endregion

			#region		---- Bug 3222 - modify a table in a dataset and then call the update method ---- 
			// testing only SQLServerr
			if (DataProvider.GetDbType(GHTUtils.DataProvider.ConnectionStringSQLClient) == DataBaseServer.SQLServer)
			{
				try
				{
					BeginCase("Bug 3222 - modify a table in a dataset and then call the update method");

					// insert a row for the sub-test
					SqlCommand insertCmd = new SqlCommand();
					insertCmd.Connection = conSql;
					insertCmd.CommandType = CommandType.Text;
					insertCmd.CommandText = "INSERT INTO Categories(CategoryName) VALUES(@CategoryName) SELECT CategoryID, CategoryName FROM Categories WHERE CategoryID = @@IDENTITY";
					insertCmd.Parameters.Add("@CategoryName", SqlDbType.VarChar, 15, "CategoryName");

					SqlDataAdapter adapter = new SqlDataAdapter();
					adapter.InsertCommand = insertCmd;

					DataSet ds = new DataSet();
					ds.Tables.Add(new DataTable("Table"));
					ds.Tables[0].Columns.Add(new DataColumn("CategoryID", typeof(int)));
					ds.Tables[0].Columns.Add(new DataColumn("CategoryName", typeof(string)));
					ds.Tables[0].Rows.Add(new object[] {-1, "49406"});

					adapter.Update(ds);
					int new_id = System.Convert.ToInt32(ds.Tables[0].Rows[0]["CategoryID"]);

					// create the UpdateCommand
					SqlParameter parm;
					SqlCommand sqlUpdateCommand = new SqlCommand("UPDATE Categories SET CategoryName = @CategoryName " + "WHERE CategoryName = @oldCategoryName", conSql);
					sqlUpdateCommand.Parameters.Add("@CategoryName", SqlDbType.NVarChar, 40, "CategoryName");
					parm = sqlUpdateCommand.Parameters.Add("@oldCategoryName", SqlDbType.NVarChar, 40, "CategoryName");
					parm.SourceVersion = DataRowVersion.Original;
					adapter.UpdateCommand = sqlUpdateCommand;

					ds = new DataSet();
					SqlCommand sqlSelectCommand = new SqlCommand("select * from Categories where CategoryID=" + new_id);
					sqlSelectCommand.Connection = conSql;
					adapter.SelectCommand = sqlSelectCommand;
					adapter.Fill(ds);
					int numRows = adapter.Fill(ds);

					ds.Tables[0].Rows[1]["Description"] = "49406";
					numRows = adapter.Update(ds);

					Compare("no exception","no exception");

				} 
				catch(Exception ex)
				{
					exp = ex;
				}
				finally
				{
					//if (dr != null)dr.Close();
					if (conSql != null)
					{if (conSql.State == ConnectionState.Open) conSql.Close();}

					EndCase(exp);
					exp = null;
				}

			}
			#endregion
		
		}
	}
}
