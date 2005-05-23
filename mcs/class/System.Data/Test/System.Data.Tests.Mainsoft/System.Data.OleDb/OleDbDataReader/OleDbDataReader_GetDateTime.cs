using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;
using GHTUtils.Data;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbDataReader_GetDateTime : ADONetTesterClass
	{
		private OleDbConnection con;
		private OleDbCommand cmd;
		private OleDbDataReader rdr;
		Exception exp;

		public void TearDown()
		{
			if (con.State == ConnectionState.Open) con.Close();
		}

		[Test] public void Main()
		{
			OleDbDataReader_GetDateTime tc = new OleDbDataReader_GetDateTime();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetDateTime");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			SimpleValue();
			MinDate();
		}

		public void SimpleValue()
		{
			try
			{
				BeginCase("check simple value");
				//prepare data
				base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);

				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);

				con.Open();
				cmd = new OleDbCommand("Select BirthDate From Employees where EmployeeID = 100", con);
				rdr = cmd.ExecuteReader();
				rdr.Read();
				DateTime dt = rdr.GetDateTime(0); //will be 1988-May-31 15:33:44
				Compare(dt,new DateTime(1988,5,31,15,33,44,00));
			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp); 
				if (rdr != null && !rdr.IsClosed)
				{
					rdr.Close();
				}
				if (con != null && con.State != ConnectionState.Closed)
				{
					con.Close();
				}
				exp = null;
			}
		}

		public void MinDate()
		{
			BeginCase("Test Min date.");
			exp = null;
			string[] dateColumns;
			DateTime[] expectedValues;
			
			InitMinDates(out dateColumns, out expectedValues);
			try
			{
				con = new OleDbConnection(DataProvider.ConnectionString);
				cmd = new OleDbCommand();
				cmd.Connection = con;
				cmd.CommandText = BuildMinDateTimeSelectSql(dateColumns);
				con.Open();
				rdr = cmd.ExecuteReader();
				Compare(true, rdr.HasRows);
				bool b = rdr.Read();
				for (int i=0; i<dateColumns.Length && i<expectedValues.Length; i++)
				{
					int j=-1;
					j = rdr.GetOrdinal(dateColumns[i]);
					//DateTime result = rdr.GetDateTime(j);
					object result = rdr.GetValue(j);
					Compare(result, expectedValues[i]);
				}
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				if (rdr != null && !rdr.IsClosed)
				{
					rdr.Close();
				}
				if (con != null && con.State != ConnectionState.Closed)
				{
					con.Close();
				}
			}
		}
		
		private void InitMinDates(out string[] columns, out DateTime[] values)
		{
			switch(DataProvider.GetDbType())
			{
				case DataBaseServer.SQLServer:
				case DataBaseServer.Sybase:
					columns = new string[] {"T_DATETIME", "T_SMALLDATETIME"};
					values = new DateTime[] {new DateTime(1753, 01, 01,00, 00, 00),		//	01/01/1753 00:00:00.000
																  new DateTime(1900, 01, 01,00, 00, 00)};		//	01/01/1900 00:00
					break;
				case DataBaseServer.Oracle:
					columns = new string[] {"T_DATE"};
					values = new DateTime[] {new DateTime(0001, 01, 01,00, 00, 00)}; //01-Jan-0001 12:00:00 AM
					break;
				case DataBaseServer.PostgreSQL:
					columns = new string[] {"T_TIMESTAMP"};
					values = new DateTime[] {new DateTime(1753, 01, 01,00, 00, 00)}; //01-Jan-1753 12:00:00 AM
					break;
				case DataBaseServer.DB2:
					columns = new string[] {"T_DATE", "T_TIMESTAMP"};
					values = new DateTime[] {new DateTime(0001, 01, 01),		//	1/1/0001
																  new DateTime(0001, 01, 01, 00,00,00,0)};	//	1/1/0001 00:00:00.000
					break;
				default:
					throw new ApplicationException(string.Format("GHT ERROR: Unknown DB server [{0}].",DataProvider.GetDbType()));
			}
		}

		private string BuildMinDateTimeSelectSql(string[] dateColumns)
		{
			System.Text.StringBuilder sqlBuilder = new System.Text.StringBuilder();
			sqlBuilder.Append("SELECT ");
			foreach(string col in dateColumns)
			{
				sqlBuilder.Append(col + ", ");
			}
			sqlBuilder.Remove(sqlBuilder.Length - 2, 2);
			sqlBuilder.Append(" FROM TYPES_EXTENDED WHERE ID='MIN'");
			return sqlBuilder.ToString();
		}
	}
}
