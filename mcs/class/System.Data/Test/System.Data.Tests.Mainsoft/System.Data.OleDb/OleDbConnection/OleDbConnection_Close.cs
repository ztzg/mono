using System;
using System.Data;
using System.Data.OleDb ;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	class OleDbConnection_Close : GHTBase
	{
		[SetUp]
		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		[TearDown]
		public void TearDown()
		{
		}

		public static void Main()
		{
			OleDbConnection_Close tc = new OleDbConnection_Close();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbConnection_Close");
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

			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);

			try
			{
				BeginCase("Close without open");
				con.Close();
				Compare(con.State , ConnectionState.Closed);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			con.Open();

			try
			{
				BeginCase("Close after open");
				con.Close();
				Compare(con.State , ConnectionState.Closed);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			//An application can call Close more than one time. 
			//No exception is generated.
			try
			{
				BeginCase("Close again");
				con.Close();
				Compare(con.State , ConnectionState.Closed);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			if (con.State == ConnectionState.Open) con.Close();
		}
	}
}