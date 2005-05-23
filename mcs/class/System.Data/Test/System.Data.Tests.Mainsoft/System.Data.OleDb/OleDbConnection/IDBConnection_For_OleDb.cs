using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	class IDBConnection_For_OleDb : GHTBase
	{
		string _ConnectionString = "";
		[SetUp]
		public void SetUp()
		{
			try
			{
				_ConnectionString = GHTUtils.DataProvider.ConnectionString;
			}
			catch(Exception ex)
			{
				EndCase(ex);
			}
		}

		[TearDown]
		public void TearDown()
		{
		}

		public static void Main()
		{
			IDBConnection_For_OleDb tc = new IDBConnection_For_OleDb();
			Exception exp = null;
			try
			{
				tc.BeginTest("IDBConnection_For_OleDb");
				tc.SetUp();
				tc.run();
				tc.TearDown();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		[Test]
		public void run()
		{
			Exception exp = null;
			IDbConnection ICon = new OleDbConnection();

			try
			{
				BeginCase("check IDbConnection is null");
				Compare(ICon != null, true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("check IDbConnection type");
				Compare(ICon.GetType().FullName ,typeof(OleDbConnection).FullName);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			ICon = new OleDbConnection(_ConnectionString);

			try
			{
				BeginCase("check IDbConnection connection string");
				Compare(ICon.ConnectionString ,_ConnectionString);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("check IDbConnection ConnectionTimeout");
				Compare(ICon.ConnectionTimeout ,15);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("check IDbConnection state - closed");
				Compare(ICon.State ,ConnectionState.Closed);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			ICon.Open();

			try
			{
				BeginCase("check IDbConnection - open");
				Compare(ICon.State ,ConnectionState.Open );
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("check IDbConnection CreateCommand");
				IDbCommand cmd = ICon.CreateCommand();
				Compare(cmd.GetType().FullName ,typeof(OleDbCommand).FullName);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (ICon.State == ConnectionState.Open) ICon.Close();

		}
	}

}