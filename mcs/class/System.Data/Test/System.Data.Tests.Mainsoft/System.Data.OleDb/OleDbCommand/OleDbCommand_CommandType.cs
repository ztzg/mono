using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

using Microsoft.ApplicationBlocks.Data;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbCommand_CommandType : GHTBase
	{
		OleDbConnection	con;
		OleDbCommand cmd;
		OleDbDataReader dr = null;

		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
				con.Open();
				cmd = new OleDbCommand("", con);
				Compare("Setup", "Setup");
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		public void TearDown()
		{
			if (con != null)
			{
				if (con.State == ConnectionState.Open) con.Close();
			}
		}

		[Test] public void Main()
		{
			OleDbCommand_CommandType tc = new OleDbCommand_CommandType();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbCommand_CommandType");
				tc.SetUp();
				tc.run();
				tc.TearDown();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;
		
			OleDbCommand cmd = new OleDbCommand();
			try
			{
				BeginCase("CommandType - default");
				Compare(cmd.CommandType , CommandType.Text );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("CommandType - TableDirect");
				cmd.CommandType = CommandType.TableDirect; 
				Compare(cmd.CommandType , CommandType.TableDirect);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("CommandType - Text");
				cmd.CommandType = CommandType.Text  ; 
				Compare(cmd.CommandType , CommandType.Text);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}


			#region		---- CommandType.Text ---- 
			try
			{
				BeginCase("CommandType.Text");

				cmd = new OleDbCommand();
				cmd.Connection = con;
				cmd.CommandType = CommandType.Text;
				
				cmd.CommandText = "{call GH_REFCURSOR1}";
				
				dr = cmd.ExecuteReader(CommandBehavior.CloseConnection);
				if (dr.HasRows)
				{
					Compare(1,dr.FieldCount);
					dr.Read();
					Compare(dr.GetString(1),"Yavine");
				}
				else
					Compare("error","HasRows=0");

			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (dr != null)dr.Close();
				if (con != null)
				{if (con.State == ConnectionState.Open) con.Close();}

				EndCase(exp);
				exp = null;
			}
			#endregion


			#region		---- CommandType.StoredProcedure using DAAB no parameters ---- 
			try
			{
				BeginCase("CommandType.StoredProcedure using DAAB no parameters");
				dr = OleDbHelper.ExecuteReader(GHTUtils.DataProvider.ConnectionString, CommandType.StoredProcedure,"GH_REFCURSOR1");
				if (dr.HasRows)
				{
					dr.Read();
					Compare(dr.GetString(1),"Yavine");
				}
				else
					Compare("error","HasRows=0");
			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (dr != null)dr.Close();
				if (con != null)
				{if (con.State == ConnectionState.Open) con.Close();}

				EndCase(exp);
				exp = null;
			}
			#endregion

			#region		---- CommandType.StoredProcedure using DAAB one parameters ---- 
			try
			{
				BeginCase("CommandType.StoredProcedure using DAAB one parameter");
				OleDbParameter p0 = new OleDbParameter("IN_EmployeeID", 1);
				dr = OleDbHelper.ExecuteReader(GHTUtils.DataProvider.ConnectionString, CommandType.StoredProcedure,"GH_REFCURSOR2",p0);
				if (dr.HasRows)
				{
					dr.Read();
					Compare(dr.GetString(1),"Yavine");
				}
				else
					Compare("error","HasRows=0");


			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (dr != null)dr.Close();
				if (con != null)
				{if (con.State == ConnectionState.Open) con.Close();}

				EndCase(exp);
				exp = null;
			}
			#endregion

			#region		---- CommandType.Text using Parameters.Add ---- 
			try
			{
				BeginCase("CommandType.Text using Parameters.Add");

				con.Open();
				cmd = new OleDbCommand();
				cmd.Connection = con;
				cmd.CommandType = CommandType.Text;
				cmd.CommandText = "{call GH_REFCURSOR3(?)}";

				OleDbParameter param1 = cmd.Parameters.Add("IN_LASTNAME", OleDbType.VarChar,20);
				param1.Direction = ParameterDirection.Input;
				param1.Value = "Yavine"; 

				dr = cmd.ExecuteReader();
				if (dr.HasRows)
				{
					dr.Read();
					Compare(dr.GetValue(0).ToString(),"1");
					Compare(dr.GetString(1),"Yavine");
				}
				else
					Compare("error","HasRows=0");

			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (dr != null)dr.Close();
				if (con != null)
				{if (con.State == ConnectionState.Open) con.Close();}

				EndCase(exp);
				exp = null;
			}
			#endregion
		

			#region		---- CommandType.StoredProcedure manual in out parameters ---- 
			try
			{
				BeginCase("CommandType.StoredProcedure manual in out parameters");

				con.Open();
				cmd = new OleDbCommand();
				cmd.Connection = con;

				cmd.CommandType = CommandType.StoredProcedure;
				cmd.CommandText = "GH_INOUT1";

				//RETURN_VALUE for SQLServer
				if (DataProvider.GetDbType(con) == GHTUtils.DataBaseServer.SQLServer)
				{
					OleDbParameter param0 = cmd.Parameters.Add("@RETURN_VALUE", OleDbType.Integer);
					param0.Direction = ParameterDirection.ReturnValue;
				}

				OleDbParameter param1 = cmd.Parameters.Add("@INPARAM", OleDbType.VarChar,20);
				param1.Direction = ParameterDirection.Input;
				param1.Value = System.Convert.ToString("dummy"); 
	
				OleDbParameter param2 = cmd.Parameters.Add("@OUTPARAM", OleDbType.Integer);//VarNumeric);
				param2.Direction = ParameterDirection.Output;

				int ret = cmd.ExecuteNonQuery();
				int intReturn;
				intReturn = System.Convert.ToInt32(cmd.Parameters["@OUTPARAM"].Value);
				Compare(intReturn,100);

			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (dr != null)dr.Close();
				if (con != null)
				{if (con.State == ConnectionState.Open) con.Close();}

				EndCase(exp);
				exp = null;
			}
			#endregion

			#region		---- ORACLE CommandType.StoredProcedure using DeriveParameters ---- 
			if (DataProvider.GetDbType(con) == GHTUtils.DataBaseServer.Oracle)
			{
				try
				{
					BeginCase("ORACLE CommandType.StoredProcedure using DeriveParameters");

					con.Open();
					cmd = new OleDbCommand();
					cmd.Connection = con;
					cmd.CommandType = CommandType.StoredProcedure;
					cmd.CommandText = "GH_REFCURSOR3";

					OleDbCommandBuilder.DeriveParameters(cmd);
					cmd.Parameters[0].Value = "Yavine"; 
					cmd.Parameters.RemoveAt(1); // the ORACLE DAAB trick is to remove the out parameter

					dr = cmd.ExecuteReader();
					if (dr.HasRows)
					{
						dr.Read();
						Compare(dr.GetValue(0).ToString(),"1");
						Compare(dr.GetString(1),"Yavine");
					}
					else
						Compare("error","HasRows=0");

				} 
				catch(Exception ex)
				{
					exp = ex;
				}
				finally
				{
					if (dr != null)dr.Close();
					if (con != null)
					{if (con.State == ConnectionState.Open) con.Close();}

					EndCase(exp);
					exp = null;
				}
			}		
		#endregion

			#region CommandType.StoredProcedure in order to repreduce bug 4003
			if (DataProvider.GetDbType(con) == GHTUtils.DataBaseServer.SQLServer)
			{
				exp = null;
				try
				{
					if (con.State == ConnectionState.Closed) con.Open();
					BeginCase("Bug 4003");
					OleDbCommand cmd4003 = new OleDbCommand("[othertestuser].[GH_DUMMY]",con);
					//OleDbCommand cmd = new OleDbCommand("[GH_DUMMY]",con);
					cmd4003.CommandType = CommandType.StoredProcedure;
					cmd4003.Parameters.Add("@EmployeeIDPrm","1");
					cmd4003.ExecuteReader();
					
				}
				catch (Exception ex)
				{
					exp=ex;
				}
				finally
				{
					if (con.State == ConnectionState.Open) con.Close();
					EndCase(exp);
				}

			}

			#endregion
		
		}
	}

}
