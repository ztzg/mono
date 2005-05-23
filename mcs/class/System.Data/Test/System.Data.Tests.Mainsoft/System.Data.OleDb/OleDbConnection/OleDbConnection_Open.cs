using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbConnection_Open : GHTBase
	{
		OleDbConnection con = null;
		string ConString = "";

		[SetUp]
		public void SetUp()
		{
			Exception exp = null;
			BeginCase("Setup");
			try
			{
				ConString = GHTUtils.DataProvider.ConnectionString;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		[TearDown]
		public void TearDown()
		{
			if (con != null)
			{
				con.Close();
			}
		}

		public static void Main()
		{
			OleDbConnection_Open tc = new OleDbConnection_Open();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDBOpenConnection");
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

		[Test]
		public void run()
		{
			Exception exp = null;
      
			BeginCase("Open Connection ");
			try
			{
				con = new OleDbConnection(ConString);
				con.Open();
				Compare(con.State , ConnectionState.Open);
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (con != null) con.Close();
				EndCase(exp);
				exp = null;
			}
		
			ConString = ConString.Replace("User Id", "uid");

			BeginCase("Open Connection - uid");
			try
			{
				con = new OleDbConnection(ConString);
				con.Open();
				Compare(con.State , ConnectionState.Open);
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (con != null) con.Close();
				EndCase(exp);
				exp = null;
			}

			BeginCase("Open Connection - garbage value");
			try
			{
				con = new OleDbConnection("xxx");
				con.Open();
			}
			catch (System.ArgumentException ex)
			{
				ExpectedExceptionCaught(ex); 
			}
			catch 
			{
				ExpectedExceptionNotCaught("System.ArgumentException");
			}
			finally
			{
				if (con != null) con.Close();
				EndCase(exp);
				exp = null;
			}
		}
	}
}
