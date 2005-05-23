using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;


namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture] public class OleDbTransaction_ctor : ADONetTesterClass
	{
		[Test] public void Main()
		{
			OleDbTransaction_ctor tc = new OleDbTransaction_ctor();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbTransaction_ctor");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			Exception exp = null;

			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			OleDbTransaction txn = null;

			try
			{
				BeginCase("BeginTransaction - connection close");
				try
				{
					txn = con.BeginTransaction();
				}
				catch (Exception ex) {exp = ex;}
				Compare(exp.GetType().FullName ,typeof(InvalidOperationException).FullName );
				exp=null;
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			con.Open();

			try
			{
				BeginCase("BeginTransaction - connection close");
				txn = con.BeginTransaction();
				Compare(txn == null,false );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();

		}
	}   
}
