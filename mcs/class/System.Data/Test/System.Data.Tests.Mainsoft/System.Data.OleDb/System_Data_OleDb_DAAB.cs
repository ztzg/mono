using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;
using GHTUtils.Data;

using NUnit.Framework;

using Microsoft.ApplicationBlocks.Data;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class System_Data_OleDb_DAAB : ADONetTesterClass
	{
		Exception exp = null;
		OleDbDataReader rdr = null;
		string connectionString;
		DataBaseServer dbServerType;

		[TestFixtureSetUp]
		public void TestSetup()
		{
			connectionString = DataProvider.ConnectionString;
			dbServerType = DataProvider.GetDbType(connectionString);
		}
		
		public static void Main()
		{
			System_Data_OleDb_DAAB tc = new System_Data_OleDb_DAAB();
			Exception exp = null;
			try
			{
				tc.BeginTest("System_Data_OleDb_DAAB");
				tc.TestSetup();
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			ExecuteReader_CommandTypeSP_NoParams_RefCursor();
			ExecuteReade_CommandTypeSP_OneParameter();

			ExecuteReader_CommandTypeSP_SIMPLETYPES_MultipleInParams_NoNulls();
			ExecuteReader_CommandTypeSP_SIMPLETYPES_MultipleInParams();
			ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingParametersArray_NoNulls();
			ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingParametersArray();
			ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingTextValues_NoNulls();
			ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingTextValues_AllNulls();

			ExecuteReader_SIMPLETYPES_MultipleParams_NoNulls();
			ExecuteReader_SIMPLETYPES_MultipleInParams();

			ExecuteNonQuery_CommandTypeSP_SIMPLETYPES_MultipleOutParams();
			ExecuteNonQuery_CommandTypeSP_SIMPLETYPES_MultipleOutParams_Nulls() ;

			ExecuteReader_CommandTypeSP_SIMPLETYPES_NoParams();

			ExecuteDataset_CommandTypeSP_SIMPLETYPES_NoParams();
		}

		
		#region public static OleDbDataReader ExecuteReader(string connectionString, CommandType commandType, string commandText, params OleDbParameter[] commandParameters)
		[Test(Description="CommandType.StoredProcedure using DAAB no parameters return RefCursor.")]
		public void ExecuteReader_CommandTypeSP_NoParams_RefCursor()
		{
			try
			{
				BeginCase("CommandType.StoredProcedure using DAAB no parameters return RefCursor.");
				exp = null;
				
				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					rdr = PostgresOleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure,"GH_REFCURSOR1");
				}
				else
				{
					rdr = OleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure,"GH_REFCURSOR1");
				}

				if (rdr.HasRows)
				{
					rdr.Read();
					Compare(rdr.GetString(1),"Yavine");
				}
				else
				{
					this.Fail("rdr.HasRows=0");
				}
			} 
			catch(Exception ex)
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
		
		[Test(Description="CommandType.StoredProcedure using DAAB one parameter")]
		public void ExecuteReade_CommandTypeSP_OneParameter()
		{
			try
			{
				BeginCase("CommandType.StoredProcedure using DAAB one parameter");
				exp = null;
				OleDbParameter p0 = new OleDbParameter("IN_EMPLOYEEID", 1);

				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					rdr = PostgresOleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure,"GH_REFCURSOR2",p0);
				}
				else
				{
					rdr = OleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure,"GH_REFCURSOR2",p0);
				}

				if (rdr.HasRows)
				{
					rdr.Read();
					Compare(rdr.GetString(1),"Yavine");
				}
				else
				{
					this.Fail("rdr.HasRows=0");
				}
			} 
			catch(Exception ex)
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

		[Test(Description="ExecuteReader, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_1\", with one type at a time")]
		public void ExecuteReader_CommandTypeSP_SIMPLETYPES_MultipleInParams()
		{
			DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
			OleDbParameter[] currentParamsSet;
			DataBaseServer testedDbServer = DataProvider.GetDbType();

			if (testedDbServer == DataBaseServer.Oracle)
			{
				//In oracle, datatype FLOAT is by default (in .NET) converted to System.Double when returned from a select statement,
				//but when a FLOAT is returned from a stored-procedure it is converted toSystem.Decimal.
				//Here we test SP then we need to convert the FLOAT value to System.Decimal in order to pass the compare.
				allParams["T_FLOAT"].Value = Convert.ToDecimal(allParams["T_FLOAT"].Value);
			}

			for (int i=0; i<allParams.Count; i++)
			{
				currentParamsSet = CreateParametersSetWithsingleValueInPosition(allParams.Count, i, allParams[i].ParameterName, allParams[i].Value);
				try
				{
					BeginCase(string.Format("ExecuteReader, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_1\", with type {0}", allParams[i].DbTypeName));
					exp = null;

					if (dbServerType == DataBaseServer.PostgreSQL)
					{
						rdr = PostgresOleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_1", currentParamsSet);
					}
					else
					{
						rdr = OleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_1", currentParamsSet);
					}

					rdr.Read();
					object result = rdr.GetValue(i);
					result = WorkaroundOracleCharsPaddingLimitation(testedDbServer, allParams[i], result);
					Compare(currentParamsSet[i].Value, result);
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
		}

		[Test(Description="ExecuteReader, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_1\", all types together.")]
		public void ExecuteReader_CommandTypeSP_SIMPLETYPES_MultipleInParams_NoNulls()
		{
			try
			{
				BeginCase("ExecuteReader, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_1\", all types together");
				exp = null;
				DataBaseServer currentDbServer = DataProvider.GetDbType();

				DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
				if (currentDbServer == DataBaseServer.Oracle)
				{
					//In oracle, datatype FLOAT is by default (in .NET) converted to System.Double when returned from a select statement,
					//but when a FLOAT is returned from a stored-procedure it is converted toSystem.Decimal.
					//Here we test SP then we need to convert the FLOAT value to System.Decimal in order to pass the compare.
					allParams["T_FLOAT"].Value = Convert.ToDecimal(allParams["T_FLOAT"].Value);
				}
				
				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					rdr = PostgresOleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_1", allParams.ToOleDbParameterArray());
				}
				else
				{
					rdr = OleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_1", allParams.ToOleDbParameterArray());
				}

				rdr.Read();
				for (int i=0; i<allParams.Count; i++)
				{
					object val = rdr.GetValue(i);
					val = WorkaroundOracleCharsPaddingLimitation(currentDbServer, allParams[i], val);
					Compare(allParams[i].Value, val);
				}
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

		[Test(Description="ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", with one type at a time using parameters")]
		public void ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingParametersArray()
		{
			DataBaseServer currentDbServer = DataProvider.GetDbType();
			if (currentDbServer == DataBaseServer.Oracle)
			{
				//Cannot call a sored-procedure on oracle that uses out ref-cursor parameter using command type text.
				return;
			}
			OleDbConnection con = new OleDbConnection(connectionString);
			OleDbTransaction transaction=null;
			DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
			OleDbParameter[] currentParamsSet;
			for (int i=0; i<allParams.Count; i++)
			{
				currentParamsSet = CreateParametersSetWithsingleValueInPosition(allParams.Count, i, allParams[i].ParameterName, allParams[i].Value);
				try
				{
					BeginCase(string.Format("ExecuteReader, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_1\", with type {0}, using parameters.", allParams[i].DbTypeName));
					exp = null;

					if (con.State != ConnectionState.Open)
					{
						con.Open();
					}
					transaction = con.BeginTransaction();
					string cmdText = BuildSPTextualCall("GHSP_TYPES_SIMPLE_1", allParams.OleDbParameterPlaceHolderList);
					//this.Log("cmdText=" + cmdText);
					if (dbServerType == DataBaseServer.PostgreSQL)
					{
						rdr = PostgresOleDbHelper.ExecuteReader(transaction, CommandType.Text, cmdText, currentParamsSet);
					}
					else
					{
						rdr = OleDbHelper.ExecuteReader(transaction, CommandType.Text, cmdText, currentParamsSet);
					}
					rdr.Read();
					object result = rdr.GetValue(i);
					result = WorkaroundOracleCharsPaddingLimitation(currentDbServer, allParams[i], result);
					Compare(currentParamsSet[i].Value, result);
				}
				catch (Exception ex)
				{
					exp = ex;
				}
				finally
				{
					if (transaction != null)
					{
						transaction.Commit();
					}

					if (rdr != null && !rdr.IsClosed)
					{
						rdr.Close();
					}
					if (con != null && con.State == ConnectionState.Open)
					{
						con.Close();
					}
					EndCase(exp);
					exp = null;
				}
			}
		}

		[Test(Description="ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using parameters.")]
		public void ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingParametersArray_NoNulls()
		{
			// Cannot call a sored-procedure on oracle that uses out ref-cursor parameter using command type text.
			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				return;
			}
			OleDbConnection con = null;
			OleDbTransaction transaction = null;
			try
			{
				BeginCase("ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using parameters.");
				exp = null;

				DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();

				con = new OleDbConnection(connectionString);
				con.Open();
				transaction = con.BeginTransaction();
				string cmdText = BuildSPTextualCall("GHSP_TYPES_SIMPLE_1", allParams.OleDbParameterPlaceHolderList);

				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					rdr = PostgresOleDbHelper.ExecuteReader(transaction, CommandType.Text, cmdText, allParams.ToOleDbParameterArray());
				}
				else
				{
					rdr = OleDbHelper.ExecuteReader(transaction, CommandType.Text, cmdText, allParams.ToOleDbParameterArray());
				}

				rdr.Read();
				for (int i=0; i<allParams.Count; i++)
				{
					object val = rdr.GetValue(i);
					Compare(allParams[i].Value, val);
				}
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (transaction != null)
				{
					transaction.Commit();
				}
				if (rdr != null && !rdr.IsClosed)
				{
					rdr.Close();
				}
				if (con != null && con.State == ConnectionState.Open)
				{
					con.Close();
				}
				EndCase(exp);
				exp = null;
			}
		}

		[Test(Description="ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", with all values set to null using text values")]
		public void ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingTextValues_AllNulls()
		{
			//Cannot call a sored-procedure on oracle that uses out ref-cursor parameter using command type text.
			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				return;
			}
			OleDbConnection con = null;
			OleDbTransaction transaction = null;
			try
			{
				BeginCase("ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using text values.");
				exp = null;

				DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
				foreach (DbTypeParameter currentParam in allParams)
				{
					currentParam.Value = DBNull.Value;
				}

				con = new OleDbConnection(connectionString);
				con.Open();
				transaction = con.BeginTransaction();
				string cmdText = BuildSPTextualCall("GHSP_TYPES_SIMPLE_1", allParams.ValuesList);
				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					rdr = PostgresOleDbHelper.ExecuteReader(transaction, CommandType.Text, cmdText);
				}
				else
				{
					rdr = OleDbHelper.ExecuteReader(transaction, CommandType.Text, cmdText);
				}

				rdr.Read();
				for (int i=0; i<allParams.Count; i++)
				{
					object val = rdr.GetValue(i);
					Compare(allParams[i].Value, val);
				}
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (transaction != null)
				{
					transaction.Commit();
				}
				if (rdr != null && !rdr.IsClosed)
				{
					rdr.Close();
				}
				if (con != null && con.State == ConnectionState.Open)
				{
					con.Close();
				}
				EndCase(exp);
				exp = null;
			}
		}

		[Test(Description="ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using text values.")]
		public void ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingTextValues_NoNulls()
		{
			//Cannot call a sored-procedure on oracle that uses out ref-cursor parameter using command type text.
			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				return;
			}
			OleDbConnection con = null;
			OleDbTransaction transaction = null;
			try
			{
				BeginCase("ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using text values.");
				exp = null;

				DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();

				con = new OleDbConnection(connectionString);
				con.Open();
				transaction = con.BeginTransaction();
				string cmdText = BuildSPTextualCall("GHSP_TYPES_SIMPLE_1", allParams.ValuesList);
				//this.Log("cmdText=" + cmdText);
				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					// Log("cmdText : " + cmdText); 
					rdr = PostgresOleDbHelper.ExecuteReader(transaction, CommandType.Text, cmdText);
				}
				else
				{
					rdr = OleDbHelper.ExecuteReader(transaction, CommandType.Text, cmdText);
				}

				rdr.Read();
				for (int i=0; i<allParams.Count; i++)
				{
					object val = rdr.GetValue(i);
					Compare(allParams[i].Value, val);
				}
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				if (transaction != null)
				{
					transaction.Commit();
				}
				if (rdr != null && !rdr.IsClosed)
				{
					rdr.Close();
				}
				if (con != null && con.State == ConnectionState.Open)
				{
					con.Close();
				}
				EndCase(exp);
				exp = null;
			}
		}

		#endregion

		#region public static OleDbDataReader ExecuteReader(string connectionString, string spName, bool includeReturnValueParameter, params object[] parameterValues)
		[Test(Description="ExecuteReader, \"GHSP_TYPES_SIMPLE_1\", with one type at a time")]
		public void ExecuteReader_SIMPLETYPES_MultipleInParams()
		{
			//Cannot use this overload of OleDbHelper method when calling a SP on oracle that uses REFCURSOR in roder ot return a result-set.
			//.NET allows you to ignore the REFCURSOR parameter, and the framwork redirect it into the returned DataReader.
			//The problem is that the DAAB verifies the number of parameters, and does not know to ignore the REFCURSOR.
			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				return;
			}
			DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
			object[] currentParamsSet;
			for (int i=0; i<allParams.Count; i++)
			{
				currentParamsSet = CreateValuesSetWithsingleValueInPosition(allParams.Count, i, allParams[i].Value);
				try
				{
					BeginCase(string.Format("ExecuteReader, \"GHSP_TYPES_SIMPLE_1\", with type {0}", allParams[i].DbTypeName));
					exp = null;

					if (dbServerType == DataBaseServer.PostgreSQL)
					{
						rdr = PostgresOleDbHelper.ExecuteReader(connectionString, "GHSP_TYPES_SIMPLE_1", false, currentParamsSet);
					}
					else
					{
						rdr = OleDbHelper.ExecuteReader(connectionString, "GHSP_TYPES_SIMPLE_1", false, currentParamsSet);
					}
					rdr.Read();
					object result = rdr.GetValue(i);
					Compare(currentParamsSet[i], result);
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
		}

		[Test(Description="ExecuteReader, \"GHSP_TYPES_SIMPLE_1\", all types together.")]
		public void ExecuteReader_SIMPLETYPES_MultipleParams_NoNulls()
		{
			//Cannot use this overload of OleDbHelper when calling a SP on oracle that uses REFCURSOR in roder ot return a result-set.
			//.NET allows you to ignore the REFCURSOR parameter, and the framwork redirect it into the returned DataReader.
			//The problem is that the DAAB verifies the number of parameters, and does not know to ignore the REFCURSOR.
			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				return;
			}
			try
			{
				BeginCase("ExecuteReader, \"GHSP_TYPES_SIMPLE_1\", all types together.");
				exp = null;
				object[] allParams;
				allParams = DataProvider.GetSimpleDbTypesParameters().ToValuesArray();

				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					rdr = PostgresOleDbHelper.ExecuteReader(connectionString, "GHSP_TYPES_SIMPLE_1", false, allParams);
				}
				else
				{
					rdr = OleDbHelper.ExecuteReader(connectionString, "GHSP_TYPES_SIMPLE_1", false, allParams);
				}
				rdr.Read();
				for (int i=0; i<allParams.Length; i++)
				{
					object val = rdr.GetValue(i);
					Compare(allParams[i], val);
				}
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
		#endregion

		#region public static OleDbDataReader ExecuteReader(string connectionString, CommandType commandType, string commandText)
		[Test(Description="ExecuteReader, \"GHSP_TYPES_SIMPLE_5\", no parameters.")]
		public void ExecuteReader_CommandTypeSP_SIMPLETYPES_NoParams()
		{
			try
			{
				BeginCase("ExecuteReader, \"GHSP_TYPES_SIMPLE_5\", no parameters.");
				exp = null;
				DbTypeParametersCollection expectedValues = DataProvider.GetSimpleDbTypesParameters();
				DataBaseServer currentDbServer = DataProvider.GetDbType();

				//In oracle, datatype FLOAT is by default (in .NET) converted to System.Double when returned from a select statement,
				//but when a FLOAT is returned from a stored-procedure it is converted toSystem.Decimal.
				//Here we test SP then we need to convert the FLOAT value to System.Decimal in order to pass the compare.
				if (currentDbServer == DataBaseServer.Oracle)
				{
					expectedValues["T_FLOAT"].Value = Convert.ToDecimal(expectedValues["T_FLOAT"].Value);
				}
				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					rdr = PostgresOleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_5");
				}
				else
				{
					rdr = OleDbHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_5");
				}
				rdr.Read();
				for (int i=0; i<expectedValues.Count; i++)
				{
					object val = rdr.GetValue(i);
					val = WorkaroundOracleCharsPaddingLimitation(currentDbServer, expectedValues[i], val);
					Compare(expectedValues[i].Value, val);
				}
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
		#endregion

		#region public static int ExecuteNonQuery(string connectionString, CommandType commandType, string commandText, params OleDbParameter[] commandParameters)
		[Test(Description="ExecuteNonQuery, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_2\" (inout params).")]
		public void ExecuteNonQuery_CommandTypeSP_SIMPLETYPES_MultipleOutParams()
		{
			try
			{
				BeginCase("ExecuteNonQuery, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_2\" (inout params)");
				exp = null;

				DbTypeParametersCollection typesCollection = DataProvider.GetSimpleDbTypesParameters();
				OleDbParameter[] allParams = typesCollection.ToOleDbParameterArray();
				for (int i=0; i<typesCollection.Count; i++)
				{
					//Must set explicitly the direction if it is not In.
					allParams[i].Direction = ParameterDirection.InputOutput;	
					//If a parameter direction is set to Out/InOut, its Size property must be defined.
					allParams[i].Size = typesCollection[i].Size;		
						
					//In Oracle we must explicitly set the DbType of textual out parameters, otherwise we get gibberish back.
					if (DataProvider.GetDbType() == DataBaseServer.Oracle)
					{
						allParams[i].DbType = typesCollection[i].DbType;
					}
				}
				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					PostgresOleDbHelper.ExecuteNonQuery(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_2", allParams);
				}
				else
				{
					OleDbHelper.ExecuteNonQuery(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_2", allParams);
				}
				for (int i=0; i<typesCollection.Count; i++)
				{
					Compare(typesCollection[i].ApplyDefaultDataTransformation(), allParams[i].Value);
				}
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				exp = null;
			}
		}

		[Test(Description="ExecuteNonQuery, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_2\" (inout params), all parameters null.")]
		public void ExecuteNonQuery_CommandTypeSP_SIMPLETYPES_MultipleOutParams_Nulls()
		{
			try
			{
				BeginCase("ExecuteNonQuery, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_2\" (inout params), all parameters null.");
				exp = null;

				DbTypeParametersCollection typesCollection = DataProvider.GetSimpleDbTypesParameters();
				foreach (DbTypeParameter TypeParam in typesCollection)
				{
					TypeParam.Value = DBNull.Value;
				}
				OleDbParameter[] allParams = typesCollection.ToOleDbParameterArray();
				for (int i=0; i<typesCollection.Count; i++)
				{
					//Must set explicitly the direction if it is not In.
					allParams[i].Direction = ParameterDirection.InputOutput;	
					//If a parameter direction is set to Out/InOut, its Size property must be defined.
					allParams[i].Size = typesCollection[i].Size;		
					//In Oracle we must explicitly set the DbType of textual out parameters, otherwise we get gibberish back.
					if (DataProvider.GetDbType() == DataBaseServer.Oracle)
					{
						allParams[i].DbType = typesCollection[i].DbType;
					}
				}
				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					PostgresOleDbHelper.ExecuteNonQuery(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_2", allParams);
				}
				else
				{
					OleDbHelper.ExecuteNonQuery(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_2", allParams);
				}
				for (int i=0; i<typesCollection.Count; i++)
				{
					Compare(typesCollection[i].ApplyDefaultDataTransformation(), allParams[i].Value);
				}
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				exp = null;
			}
		}

		#endregion

		#region public static DataSet ExecuteDataset(OleDbConnection connection, CommandType commandType, string commandText)
		[Test(Description="ExecuteDataset, \"GHSP_TYPES_SIMPLE_5\", no parameters.")]
		public void ExecuteDataset_CommandTypeSP_SIMPLETYPES_NoParams()
		{
			DataSet ds = null;
			try
			{
				BeginCase("ExecuteDataset, \"GHSP_TYPES_SIMPLE_5\", no parameters.");
				exp = null;
				DbTypeParametersCollection expectedValues = DataProvider.GetSimpleDbTypesParameters();
				DataBaseServer testedDbServer = DataProvider.GetDbType();

				//In oracle, datatype FLOAT is by default (in .NET) converted to System.Double when returned from a select statement,
				//but when a FLOAT is returned from a stored-procedure it is converted toSystem.Decimal.
				//Here we test SP then we need to convert the FLOAT value to System.Decimal in order to pass the compare.
				if (testedDbServer == DataBaseServer.Oracle)
				{
					expectedValues["T_FLOAT"].Value = Convert.ToDecimal(expectedValues["T_FLOAT"].Value);
				}
				if (dbServerType == DataBaseServer.PostgreSQL)
				{
					ds = PostgresOleDbHelper.ExecuteDataset(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_5");
				}
				else
				{
					ds = OleDbHelper.ExecuteDataset(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_5");
				}

				for (int i=0; i<expectedValues.Count; i++)
				{
					object val = WorkaroundOracleCharsPaddingLimitation(testedDbServer, expectedValues[i], ds.Tables[0].Rows[0][i]);
					Compare(expectedValues[i].Value, val);
				}
			}
			catch (Exception ex)
			{
				exp = ex;
			}
			finally
			{
				EndCase(exp);
				exp = null;
			}
		}
		#endregion

		#region Private Utilities
		/// <summary>
		/// Builds an OleDbParameter array with specified number of items.
		/// All items are set to null, only the item at the specified position have a value.
		/// </summary>
		/// <param name="iSize">Number of items in the returned array.</param>
		/// <param name="iPositionOfValue">Possition in the array for the non null value.</param>
		/// <param name="sParameterName">Name for the non null parameter.</param>
		/// <param name="oValue">Value for the non null parameter.</param>
		/// <returns>OleDbParameter array with specified number of items.
		/// All items are set to null, only the item at the specified position have a value.</returns>
		private OleDbParameter[] CreateParametersSetWithsingleValueInPosition(int iSize, int iPositionOfValue, string sParameterName, object oValue)
		{
			OleDbParameter[] retVal = new OleDbParameter[iSize];
			for (int i=0; i<iSize; i++)
			{
				retVal[i] = new OleDbParameter();
				if (i==iPositionOfValue)
				{
					retVal[i].ParameterName = sParameterName;
					retVal[i].Value =  oValue;
				}
				else
				{
					retVal[i].ParameterName = "NULL_PARAM";
					retVal[i].Value = DBNull.Value;
				}
			}

			return retVal;
		}
		/// <summary>
		/// Builds an object array with specified number of items.
		/// All items are set to null, only the item at the specified position have a value.
		/// </summary>
		/// <param name="iSize">Number of items in the returned array.</param>
		/// <param name="iPositionOfValue">Possition in the array for the non null value.</param>
		/// <param name="oValue">Value for the non null parameter.</param>
		/// <returns>object array with specified number of items.
		/// All items are set to null, only the item at the specified position have a value.</returns>
		private object[] CreateValuesSetWithsingleValueInPosition(int iSize, int iPositionOfValue, object oValue)
		{
			object[] retVal = new object[iSize];
			for (int i=0; i<iSize; i++)
			{
				retVal[i] = (i==iPositionOfValue) ? oValue : DBNull.Value;
			}

			return retVal;
		}
		private string BuildSPTextualCall(string sSPName, string sParameters)
		{
			string sFormatting = string.Empty;
			switch (DataProvider.GetDbType(connectionString))
			{
				case DataBaseServer.SQLServer:
				case DataBaseServer.Sybase:
					sFormatting = "EXEC {0} {1}";
					break;
				case DataBaseServer.Oracle:
				case DataBaseServer.DB2:
					sFormatting = "CALL {0} ({1})";
					break;
				case DataBaseServer.PostgreSQL:
					sFormatting = "SELECT {0} ({1})";
					break;
				default:
					throw new ApplicationException("Unknown DB server.");
			}

			return string.Format(sFormatting, sSPName, sParameters);
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

		#endregion
	}
}
