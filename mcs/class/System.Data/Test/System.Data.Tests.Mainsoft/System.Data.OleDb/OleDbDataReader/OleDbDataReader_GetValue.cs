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
	public class OleDbDataReader_GetValue : ADONetTesterClass
	{
		Exception exp;
		OleDbConnection con;
		char [] Result;

		[SetUp]
		public void SetUp()
		{
			try
			{
				//prepare data
				BeginCase("SetUp");
				base.PrepareDataForTesting(GHTUtils.DataProvider.ConnectionString);

				con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
				Result = new char[100];
				con.Open();
				this.Pass("Setup.");
			}
			catch(Exception ex){exp = ex;}
			finally	{EndCase(exp);}
		}

		[TearDown]
		public void TearDown()
		{
			if (con.State == ConnectionState.Open) con.Close();
		}

		public static void Main()
		{
			OleDbDataReader_GetValue tc = new OleDbDataReader_GetValue();
			tc.exp = null;
			try
			{
				tc.BeginTest("OleDbDataReader_GetValue");
				tc.SetUp();
				tc.run();
			}
			catch (Exception ex)
			{
				tc.exp = ex;
			}
			finally	
			{
				tc.EndTest(tc.exp);
				tc.TearDown();
			}
		}

		[Test]
		public void run()
		{
			SimpleTest();
			TypesTests(DataProvider.GetSimpleDbTypesParameters());
			TypesTests(DataProvider.GetExtendedDbTypesParameters());
		}

		private void SimpleTest()
		{
			OleDbDataReader rdr = null;
			exp = null;
			try
			{
				BeginCase("check value");
				
				OleDbCommand cmd = new OleDbCommand("Select LastName From Employees Where EmployeeID = 100", con);
				rdr = cmd.ExecuteReader();
				rdr.Read();

				Object obj = rdr.GetValue(0); 
				Compare(obj.ToString(), "Last100");
			} 
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (rdr != null)
				{
					rdr.Close();
				}
				EndCase(exp);
				exp = null;
			}
		}
		private void TypesTests(DbTypeParametersCollection typesToTest)
		{
			exp = null;

			DbTypeParametersCollection currentlyTested = new DbTypeParametersCollection(typesToTest.TableName);
			string rowId = "13289";
			object dbValue;
			OleDbDataReader rdr = null;
			OleDbConnection selectCon = null;
			DataBaseServer testedDbServer = DataProvider.GetDbType();

			foreach (DbTypeParameter currentParamType in typesToTest)
			{
				BeginCase("Test value of db type: " + currentParamType.DbTypeName);
				//Prepare data:
				rowId = string.Format("13289_{0}", this.TestCaseNumber);
				currentlyTested.Clear();
				currentlyTested.Add(currentParamType);
				currentlyTested.ExecuteInsert(rowId);

				try
				{
					currentlyTested.ExecuteSelectReader(rowId, out rdr, out selectCon);
					rdr.Read();
					dbValue = WorkaroundOracleCharsPaddingLimitation(testedDbServer, currentParamType, rdr.GetValue(0));
					if (currentParamType.Value.GetType().IsArray)
					{
						Compare(currentParamType.Value as Array, dbValue as Array);
					}
					else
					{
						Compare(currentParamType.Value, dbValue);
					}
				} 
				catch(Exception ex)
				{
					exp = ex;
				}
				finally
				{
					EndCase(exp);
					exp = null;
					if (rdr != null && !rdr.IsClosed)
					{
						rdr.Close();
					}
					if (selectCon != null && selectCon.State != ConnectionState.Closed)
					{
						selectCon.Close();
					}
					currentlyTested.ExecuteDelete(rowId);
				}
			}
		}
		/// <summary>
		/// This is a workaround for the extra white spaces added in oracle to NCHAR & NVARCHAR values.
		/// The problem is a documented GH limitation, see bug #3417.
		/// The workaround is to trim the lemgth of the returned string to the specified length of the parameter/column.
		/// </summary>
		/// <param name="testedServer">The database server we are currently running on.</param>
		/// <param name="val">The value returned from the database.</param>
		/// <returns>The normalized value..</returns>
		private object WorkaroundOracleCharsPaddingLimitation(DataBaseServer testedServer, DbTypeParameter currentParam, object val)
		{
			object origVal = val;
			string dbTypeName = currentParam.DbTypeName.ToUpper();
			if ( (testedServer == DataBaseServer.Oracle) && (dbTypeName == "CHAR" || dbTypeName == "NCHAR") )
			{
				val = ((string)val).Substring(0, currentParam.Size);
				Log(string.Format("Worked around oracle chars padding limitation by triming '{0}' to '{1}'", origVal, val));
			}
			return val;
		}


	}
}
