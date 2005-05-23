using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbCommand_Prepare : GHTBase
	{
		[Test] public void Main()
		{
			OleDbCommand_Prepare tc = new OleDbCommand_Prepare();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbCommand_Prepare");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;
			int intRecordsAffected = 0;

			string sql = "Update Shippers Set CompanyName=? Where ShipperID = 2";
			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			OleDbCommand cmd = new OleDbCommand("", con);
			con.Open();


			//get expected result
			cmd.CommandText = "select count(*) from Shippers where ShipperID = 2";
			int ExpectedRows = int.Parse(cmd.ExecuteScalar().ToString());

			cmd.CommandText = sql;

			//Currently not running on DB2: .Net-Failed, GH:Pass
			//if (con.Provider.IndexOf("IBMDADB2") >= 0) return ;

			cmd.Parameters.Add(new OleDbParameter()); 
			cmd.Parameters[0].ParameterName = "CompName";
			cmd.Parameters[0].OleDbType = OleDbType.VarWChar; //System.InvalidOperationException:
			cmd.Parameters[0].Size = 20; //System.InvalidOperationException
			cmd.Parameters[0].SourceColumn = "CompanyName";
			cmd.Parameters[0].Value = "Comp1";

			try
			{
				BeginCase("Prepare Exception - missing OleDbType");
				try
				{
					cmd.Parameters[0].OleDbType = OleDbType.Empty ;
					cmd.Prepare();
				}
				catch (Exception ex) {exp = ex;}
				Compare(exp.GetType().FullName, typeof(InvalidOperationException).FullName );
				exp=null;
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}
			cmd.Parameters[0].OleDbType = OleDbType.VarWChar; 

			try
			{
				BeginCase("Prepare Exception - missing Size");
				try
				{
					cmd.Parameters[0].Size = 0;
					cmd.Prepare();
				}
				catch (Exception ex) {exp = ex;}
				Compare(exp.GetType().FullName, typeof(InvalidOperationException).FullName );
				exp=null;
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}
			cmd.Parameters[0].Size = 20;

			try
			{
				BeginCase("Prepare Exception - missing Size");
				try
				{
					con.Close();
					cmd.Prepare();
				}
				catch (Exception ex) {exp = ex;}
				Compare(exp.GetType().FullName, typeof(InvalidOperationException).FullName );
				exp=null;
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}
			con.Open();

			try
			{
				BeginCase("ExecuteNonQuery first time");
				intRecordsAffected = cmd.ExecuteNonQuery();
				Compare(intRecordsAffected , ExpectedRows);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}


			try
			{
				BeginCase("ExecuteNonQuery second time, chage value");
				cmd.Parameters[0].Value = "Comp2";
				intRecordsAffected  = cmd.ExecuteNonQuery();
				Compare(intRecordsAffected , ExpectedRows);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}
	
			if (con.State == ConnectionState.Open) con.Close();

		}
	}

}