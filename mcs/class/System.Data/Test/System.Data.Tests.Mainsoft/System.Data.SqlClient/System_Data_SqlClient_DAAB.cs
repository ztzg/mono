using System;
using System.Data;
using System.Data.SqlClient;

using GHTUtils;
using GHTUtils.Base;
using GHTUtils.Data;

using NUnit.Framework;

using Microsoft.ApplicationBlocks.Data;

namespace tests.system_data_dll.System_Data.System_Data_SqlClient
{
	[TestFixture]
	public class System_Data_SqlClient_DAAB : GHTBase
	{
		Exception exp = null;
		SqlDataReader rdr = null;
		string connectionString;

		[TestFixtureSetUp]
		public void Setup()
		{
			connectionString = DataProvider.ConnectionStringSQLClient;
		}

		public static void Main()
		{
			System_Data_SqlClient_DAAB tc = new System_Data_SqlClient_DAAB();
			Exception exp = null;
			try
			{
				tc.BeginTest("System_Data_SqlClient_DAAB");
				tc.Setup();
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			if (DataProvider.GetDbType() != DataBaseServer.SQLServer)
			{
				//All tests in this class are only for MSSQLServer.
				Log(string.Format("All tests in this class are only for MSSQLServer and cannot be tested on {0}", DataProvider.GetDbType()));
				return;
			}
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
			ExecuteNonQuery_CommandTypeSP_SIMPLETYPES_MultipleOutParams_Nulls();

			ExecuteReader_CommandTypeSP_SIMPLETYPES_NoParams();

			ExecuteDataset_CommandTypeSP_SIMPLETYPES_NoParams();
		}

		#region public static SqlDataReader ExecuteReader(string connectionString, CommandType commandType, string commandText, params SqlParameter[] commandParameters)
		[Test(Description="CommandType.StoredProcedure using DAAB no parameters return RefCursor.")]
		public void ExecuteReader_CommandTypeSP_NoParams_RefCursor()
		{
			try
			{
				BeginCase("CommandType.StoredProcedure using DAAB no parameters return RefCursor.");
				exp = null;
				rdr = SqlHelper.ExecuteReader(connectionString, CommandType.StoredProcedure,"GH_REFCURSOR1");
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
				SqlParameter p0 = new SqlParameter("@IN_EMPLOYEEID", 1);
				rdr = SqlHelper.ExecuteReader(connectionString, CommandType.StoredProcedure,"GH_REFCURSOR2",p0);
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
			SqlParameter[] currentParamsSet;
			for (int i=0; i<allParams.Count; i++)
			{
				currentParamsSet = CreateParametersSetWithsingleValueInPosition(allParams, i);
				try
				{
					BeginCase(string.Format("ExecuteReader, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_1\", with type {0}", allParams[i].DbTypeName));
					exp = null;

					rdr = SqlHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_1", currentParamsSet);
					rdr.Read();
					object result = rdr.GetValue(i);
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

				DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
				rdr = SqlHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_1", allParams.ToSqlParameterArray());
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
			SqlConnection con = new SqlConnection(connectionString);
			DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
			SqlParameter[] currentParamsSet;
			for (int i=0; i<allParams.Count; i++)
			{
				currentParamsSet = CreateParametersSetWithsingleValueInPosition(allParams, i);
				for (int j=0; j<allParams.Count; j++)
				{
					currentParamsSet[j].ParameterName = allParams[j].ParameterName;
				}
				try
				{
					BeginCase(string.Format("ExecuteReader, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_1\", with type {0}, using parameters.", allParams[i].DbTypeName));
					exp = null;

					if (con.State != ConnectionState.Open)
					{
						con.Open();
					}
					string cmdText = "EXEC GHSP_TYPES_SIMPLE_1 " + allParams.SqlParameterPlaceHolderList;
					//this.Log("cmdText=" + cmdText);
					rdr = SqlHelper.ExecuteReader(con, CommandType.Text, cmdText, currentParamsSet);
					rdr.Read();
					object result = rdr.GetValue(i);
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
		[Test(Description="ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using parameters.")]
		public void ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingParametersArray_NoNulls()
		{
			SqlConnection con = null;
			try
			{
				BeginCase("ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using parameters.");
				exp = null;

				DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();

				con = new SqlConnection(connectionString);
				con.Open();
				string cmdText = "EXEC GHSP_TYPES_SIMPLE_1 " + allParams.SqlParameterPlaceHolderList;

				rdr = SqlHelper.ExecuteReader(con, CommandType.Text, cmdText, allParams.ToSqlParameterArray());

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
				if (con != null && con.State == ConnectionState.Open)
				{
					con.Close();
				}
				if (rdr != null && !rdr.IsClosed)
				{
					rdr.Close();
				}
				EndCase(exp);
				exp = null;
			}
		}
		[Test(Description="ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", with all values set to null using text values")]
		public void ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingTextValues_AllNulls()
		{
			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				//Cannot call a sored-procedure on oracle that uses out ref-cursor parameter using command type text.
				return;
			}
			SqlConnection con = null;
			try
			{
				BeginCase("ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using text values.");
				exp = null;

				DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
				foreach (DbTypeParameter currentParam in allParams)
				{
					currentParam.Value = DBNull.Value;
				}

				con = new SqlConnection(connectionString);
				con.Open();
				string cmdText = "EXEC GHSP_TYPES_SIMPLE_1 " + allParams.ValuesList;
				//this.Log("cmdText=" + cmdText);
				rdr = SqlHelper.ExecuteReader(con, CommandType.Text, cmdText);

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
				if (con != null && con.State == ConnectionState.Open)
				{
					con.Close();
				}
				if (rdr != null && !rdr.IsClosed)
				{
					rdr.Close();
				}
				EndCase(exp);
				exp = null;
			}
		}

		[Test(Description="ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using text values.")]
		public void ExecuteReader_CommandTypeTEXT_SIMPLETYPES_MultipleInParamsUsingTextValues_NoNulls()
		{
			if (DataProvider.GetDbType() == DataBaseServer.Oracle)
			{
				//Cannot call a sored-procedure on oracle that uses out ref-cursor parameter using command type text.
				return;
			}
			SqlConnection con = null;
			try
			{
				BeginCase("ExecuteReader, CommandType.Text, \"GHSP_TYPES_SIMPLE_1\", all types together using text values.");
				exp = null;

				DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();

				con = new SqlConnection(connectionString);
				con.Open();
				string cmdText = "EXEC GHSP_TYPES_SIMPLE_1 " +  allParams.ValuesList;
				//this.Log("cmdText=" + cmdText);
				rdr = SqlHelper.ExecuteReader(con, CommandType.Text, cmdText);

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
				if (con != null && con.State == ConnectionState.Open)
				{
					con.Close();
				}
				if (rdr != null && !rdr.IsClosed)
				{
					rdr.Close();
				}
				EndCase(exp);
				exp = null;
			}
		}

		#endregion

		#region public static SqlDataReader ExecuteReader(string connectionString, string spName, bool includeReturnValueParameter, params object[] parameterValues)
		[Test(Description="ExecuteReader, \"GHSP_TYPES_SIMPLE_1\", with one type at a time")]
		public void ExecuteReader_SIMPLETYPES_MultipleInParams()
		{
			DbTypeParametersCollection allParams = DataProvider.GetSimpleDbTypesParameters();
			object[] currentParamsSet;
			for (int i=0; i<allParams.Count; i++)
			{
				currentParamsSet = CreateValuesSetWithsingleValueInPosition(allParams.Count, i, allParams[i].Value);
				try
				{
					BeginCase(string.Format("ExecuteReader, \"GHSP_TYPES_SIMPLE_1\", with type {0}", allParams[i].DbTypeName));
					exp = null;

					rdr = SqlHelper.ExecuteReader(connectionString, "GHSP_TYPES_SIMPLE_1", currentParamsSet);
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
			try
			{
				BeginCase("ExecuteReader, \"GHSP_TYPES_SIMPLE_1\", all types together.");
				exp = null;
				object[] allParams;
				allParams = DataProvider.GetSimpleDbTypesParameters().ToValuesArray();
				rdr = SqlHelper.ExecuteReader(connectionString, "GHSP_TYPES_SIMPLE_1", allParams);
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

		#region public static SqlDataReader ExecuteReader(string connectionString, CommandType commandType, string commandText)
		[Test(Description="ExecuteReader, \"GHSP_TYPES_SIMPLE_5\", no parameters.")]
		public void ExecuteReader_CommandTypeSP_SIMPLETYPES_NoParams()
		{
			try
			{
				BeginCase("ExecuteReader, \"GHSP_TYPES_SIMPLE_5\", no parameters.");
				exp = null;
				DbTypeParametersCollection expectedValues = DataProvider.GetSimpleDbTypesParameters();
				rdr = SqlHelper.ExecuteReader(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_5");
				rdr.Read();
				for (int i=0; i<expectedValues.Count; i++)
				{
					object val = rdr.GetValue(i);
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

		#region public static int ExecuteNonQuery(string connectionString, CommandType commandType, string commandText, params SqlParameter[] commandParameters)
		[Test(Description="ExecuteNonQuery, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_2\" (inout params).")]
		public void ExecuteNonQuery_CommandTypeSP_SIMPLETYPES_MultipleOutParams()
		{
			try
			{
				BeginCase("ExecuteNonQuery, CommandType.StoredProcedure, \"GHSP_TYPES_SIMPLE_2\" (inout params)");
				exp = null;

				DbTypeParametersCollection typesCollection = DataProvider.GetSimpleDbTypesParameters();
				SqlParameter[] allParams = typesCollection.ToSqlParameterArray();
				for (int i=0; i<typesCollection.Count; i++)
				{
					allParams[i].Direction = ParameterDirection.InputOutput;	//Must set explicitly the direction if it is not In.
					allParams[i].Size = typesCollection[i].Size;		//If a parameter direction is set to Out/InOut, its Size property must be defined.
				}
				SqlHelper.ExecuteNonQuery(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_2", allParams);
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
				SqlParameter[] allParams = typesCollection.ToSqlParameterArray();
				for (int i=0; i<typesCollection.Count; i++)
				{
					allParams[i].Direction = ParameterDirection.InputOutput;	//Must set explicitly the direction if it is not In.
					allParams[i].Size = typesCollection[i].Size;		//If a parameter direction is set to Out/InOut, its Size property must be defined.
				}
				SqlHelper.ExecuteNonQuery(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_2", allParams);
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
		
		#region public static DataSet ExecuteDataset(SqlConnection connection, CommandType commandType, string commandText)
		[Test(Description="ExecuteDataset, \"GHSP_TYPES_SIMPLE_5\", no parameters.")]
		public void ExecuteDataset_CommandTypeSP_SIMPLETYPES_NoParams()
		{
			DataSet ds = null;
			try
			{
				BeginCase("ExecuteDataset, \"GHSP_TYPES_SIMPLE_5\", no parameters.");
				exp = null;
				DbTypeParametersCollection expectedValues = DataProvider.GetSimpleDbTypesParameters();
				ds = SqlHelper.ExecuteDataset(connectionString, CommandType.StoredProcedure, "GHSP_TYPES_SIMPLE_5");
				
				foreach (DbTypeParameter result in expectedValues)
				{
					Compare(result.Value, ds.Tables[0].Rows[0][result.DbColumnName]);
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
		/// Builds an SqlParameter array with specified number of items.
		/// All items are set to null, only the item at the specified position have a value.
		/// </summary>
		/// <param name="iSize">Number of items in the returned array.</param>
		/// <param name="iPositionOfValue">Possition in the array for the non null value.</param>
		/// <param name="sParameterName">Name for the non null parameter.</param>
		/// <param name="oValue">Value for the non null parameter.</param>
		/// <returns>SqlParameter array with specified number of items.
		/// All items are set to null, only the item at the specified position have a value.</returns>
		private SqlParameter[] CreateParametersSetWithsingleValueInPosition(DbTypeParametersCollection allTypes, int iPositionOfValue)
		{
			SqlParameter[] retVal = new SqlParameter[allTypes.Count];
			for (int i=0; i<allTypes.Count; i++)
			{
				retVal[i] = new SqlParameter();
				if (i==iPositionOfValue)
				{
					retVal[i].ParameterName = allTypes[iPositionOfValue].ParameterName;
					retVal[i].Value =  allTypes[iPositionOfValue].Value;
				}
				else
				{
					retVal[i].ParameterName = allTypes[i].ParameterName;
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
		#endregion

	}
}
