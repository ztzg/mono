using System;
using System.Data;
using System.Data.OleDb ;
using System.IO;
using GHTUtils.Data;
using System.Collections;

// Provide All Data required by the diffderent tests e.g.DataTable, DataRow ...
namespace GHTUtils
{
	/// <summary>
	/// Types of Database Servers that tests can be run on.
	/// </summary>
	public enum DataBaseServer
	{
		SQLServer,
		Oracle,
		DB2,
		Sybase,
		PostgreSQL,
		Unknown
	}

	public class DataProvider
	{
		#region Constatntas
		
		#region Private
		//A string containing all printable charachters.
		private const string SAMPLE_STRING = "abcdefghijklmnopqrstuvwxyz1234567890~!@#$%^&*()_+-=[]\\|;:,./<>? ";
		#endregion
		
		#region Public
		/// <summary>
		/// Name of the table in the database, that contain columns of simple types.
		/// </summary>
		public const string SIMPLE_TYPES_TABLE_NAME = "TYPES_SIMPLE";
		/// <summary>
		/// Name of the table in the database, that contain columns of extended types.
		/// </summary>
		public const string EXTENDED_TYPES_TABLE_NAME = "TYPES_EXTENDED";
		/// <summary>
		/// Name of the table in the database, that contain columns of DB specific types.
		/// </summary>
		public const string SPECIFIC_TYPES_TABLE_NAME = "TYPES_SPECIFIC";
		#endregion
		
		#endregion
		public static System.Data.DataTable CreateChildDataTable()
		{
			System.Data.DataTable dtChild = new System.Data.DataTable("Child");
			dtChild.Columns.Add("ParentId",typeof(int));
			dtChild.Columns.Add("ChildId",typeof(int));
			dtChild.Columns.Add("String1",typeof(string));
			dtChild.Columns.Add("String2",typeof(string));
			dtChild.Columns.Add("ChildDateTime",typeof(DateTime));
			dtChild.Columns.Add("ChildDouble",typeof(double));

			dtChild.Rows.Add(new object[] {1,1,"1-String1","1-String2",new DateTime(2000,1,1,0,0,0,0),1.534});
			dtChild.Rows.Add(new object[] {1,2,"2-String1","2-String2",DateTime.MaxValue ,-1.534});
			dtChild.Rows.Add(new object[] {1,3,"3-String1","3-String2",DateTime.MinValue,double.MaxValue/10000});
			dtChild.Rows.Add(new object[] {2,1,"1-String1","1-String2",new DateTime(1973,6,20,0,0,0,0),double.MinValue*10000});
			dtChild.Rows.Add(new object[] {2,2,"2-String1","2-String2",new DateTime(2008,12,1,13,59,59,59),0.45});
			dtChild.Rows.Add(new object[] {2,3,"3-String1","3-String2",new DateTime(2003,1,1,1,1,1,1),0.55});
			dtChild.Rows.Add(new object[] {5,1,"1-String1","1-String2",new DateTime(2002,1,1,1,1,1,1),0});
			dtChild.Rows.Add(new object[] {5,2,"2-String1","2-String2",new DateTime(2001,1,1,1,1,1,1),10});
			dtChild.Rows.Add(new object[] {5,3,"3-String1","3-String2",new DateTime(2000,1,1,1,1,1,1),20});
			dtChild.Rows.Add(new object[] {6,1,"1-String1","1-String2",new DateTime(2000,1,1,1,1,1,0),25});
			dtChild.Rows.Add(new object[] {6,2,"2-String1","2-String2",new DateTime(2000,1,1,1,1,0,0),30});
			dtChild.Rows.Add(new object[] {6,3,"3-String1","3-String2",new DateTime(2000,1,1,0,0,0,0),35});
			dtChild.AcceptChanges();
			return dtChild;

		}

		public static System.Data.DataTable CreateParentDataTable()
		{     

			System.Data.DataTable dtParent = new System.Data.DataTable("Parent");

			dtParent.Columns.Add("ParentId",typeof(int));
			dtParent.Columns.Add("String1",typeof(string));
			dtParent.Columns.Add("String2",typeof(string));

			dtParent.Columns.Add("ParentDateTime",typeof(DateTime));
			dtParent.Columns.Add("ParentDouble",typeof(double));
			dtParent.Columns.Add("ParentBool",typeof(bool));

			dtParent.Rows.Add(new object[] {1,"1-String1","1-String2",new DateTime(2005,1,1,0,0,0,0),1.534,true});
			dtParent.Rows.Add(new object[] {2,"2-String1","2-String2",new DateTime(2004,1,1,0,0,0,1),-1.534,true});
			dtParent.Rows.Add(new object[] {3,"3-String1","3-String2",new DateTime(2003,1,1,0,0,1,0),double.MinValue*10000,false});
			dtParent.Rows.Add(new object[] {4,"4-String1","4-String2",new DateTime(2002,1,1,0,1,0,0),double.MaxValue/10000,true});
			dtParent.Rows.Add(new object[] {5,"5-String1","5-String2",new DateTime(2001,1,1,1,0,0,0),0.755,true});
			dtParent.Rows.Add(new object[] {6,"6-String1","6-String2",new DateTime(2000,1,1,0,0,0,0),0.001,false});
			dtParent.AcceptChanges();
			return dtParent;

		}

		//This method replace the DataSet GetXmlSchema method
		//used to compare DataSets
		//Created by Ofer (13-Nov-03) becuase DataSet GetXmlSchema method is not yet implemented in java 
		public static string GetDSSchema(DataSet ds)
		{

			string strSchema = "DataSet Name=" + ds.DataSetName + "\n"; 
			//Get relations
			foreach (DataRelation dl in ds.Relations)
			{
				strSchema += "\t" + "DataRelation Name=" + dl.RelationName ;
				foreach (DataColumn dc in dl.ParentColumns)
					strSchema += "\t" + "ParentColummn=" +  dc.ColumnName ;
				foreach (DataColumn dc in dl.ChildColumns )
					strSchema += "\t" + "ChildColumn=" +  dc.ColumnName ;
				strSchema += "\n";
			}
			//Get teables
			foreach (DataTable dt in ds.Tables)
			{
				strSchema += "Table=" + dt.TableName + "\t";
				//Get Constraints  
				strSchema += "Constraints =";
				foreach (Constraint cs in dt.Constraints )
					strSchema += cs.GetType().Name + ", ";
				strSchema += "\n";
				//Get PrimaryKey Columns
				strSchema += "PrimaryKey Columns index:=";
				foreach (DataColumn dc in dt.PrimaryKey)
					strSchema += dc.Ordinal + ", ";
				strSchema += "\n";
				//Get Columns
				foreach (DataColumn dc in dt.Columns)
				{
					strSchema += "ColumnName=" + dc.ColumnName + "\t" +
						"ColumnType=" + dc.DataType.Name + "\t" +
						"AllowDBNull=" + dc.AllowDBNull.ToString() + "\t" +
						"DefaultValue=" + dc.DefaultValue.ToString() + "\t" +
						"Unique=" + dc.Unique.ToString() + "\t" +
						"ReadOnly=" + dc.ReadOnly.ToString() + "\n" ;
				}
				strSchema += "\n";
			}
			return strSchema;
		}


		public static string ConnectionString
		{
			get
			{
				System.IO.FileStream file = null;
				StreamReader sr = null;
				string ConString;
				try 
				{
					file = new System.IO.FileStream("ADOConString.txt",FileMode.Open,FileAccess.Read);
					sr = new StreamReader(file);
					ConString = sr.ReadLine();
					return ConString;
				}
				catch (System.Exception ex)
				{
					throw new Exception("Could not open ADOConString.txt. " + ex.Message.ToString());
				}
				finally
				{
					if (file != null) file.Close();
					if (sr != null) sr.Close();
				}
			}
		}

		//	SQLClient does not allow to use the Provider token
		//	since Provider is always the first parameter(in GHT framework),
		//	we trim it.
		public static string ConnectionStringSQLClient
		{
			get
			{
				return ConnectionString.Substring(ConnectionString.IndexOf(";"));
			}
		}
	

		/// <summary>
		/// Resolves the type of DB server specified by the "ADOConString.txt" file.
		/// </summary>
		/// <returns>The type of DB server specified by the "ADOConString.txt" file.</returns>
		public static GHTUtils.DataBaseServer GetDbType()
		{
			return DataProvider.GetDbType(DataProvider.ConnectionString);
		}
		
		/// <summary>
		/// Resolves the type of DB server that the specified connection refers.
		/// </summary>
		/// <param name="OleCon">A valid connection object to a DataBase.</param>
		/// <returns>The type of DB server that the specified connection refers to.</returns>
		public static GHTUtils.DataBaseServer GetDbType(System.Data.OleDb.OleDbConnection OleCon)
		{
			return DataProvider.GetDbType(OleCon.ConnectionString);
		}

		/// <summary>
		/// Resolves the type of DB server that the specified connection string refers.
		/// </summary>
		/// <param name="ConnectionString">A valid connection string to a DataBase server.</param>
		/// <returns>The type of DB server that the specified connection string refers to.</returns>
		public static GHTUtils.DataBaseServer GetDbType(string ConnectionString)
		{
			if (ConnectionString.ToUpper().IndexOf("PROVIDER=SQLOLEDB") >= 0) return DataBaseServer.SQLServer ;
			if (ConnectionString.ToUpper().IndexOf("PROVIDER=SYBASE") >= 0) return DataBaseServer.Sybase ;
			if (ConnectionString.ToUpper().IndexOf("PROVIDER=MSDAORA") >= 0) return DataBaseServer.Oracle;
			if (ConnectionString.ToUpper().IndexOf("PROVIDER=IBMDADB2") >= 0) return DataBaseServer.DB2;
			if ((ConnectionString.ToUpper().IndexOf("PROVIDER=MAINSOFT.JDBC.OLEDB.1") >= 0) || 
				(ConnectionString.ToUpper().IndexOf("PROVIDER=POSTGRES") >= 0) ) return DataBaseServer.PostgreSQL;
			return DataBaseServer.Unknown ;
		}

		/// <summary>
		/// Creates a DbTypeParametersCollection with default types and data for the TYPES_SIMPLE table.
		/// </summary>
		/// <returns>The initialized DbTypeParametersCollection</returns>
		public static DbTypeParametersCollection GetSimpleDbTypesParameters()
		{
			DbTypeParametersCollection row = new DbTypeParametersCollection(SIMPLE_TYPES_TABLE_NAME);
			switch (DataProvider.GetDbType(DataProvider.ConnectionString))
			{
					#region SQLServer
				case GHTUtils.DataBaseServer.SQLServer:
					row.Add("bit", true, 1);
					row.Add("tinyint", (byte)25, 1);
					row.Add("smallint", (Int16)77, 2);
					row.Add("int", (Int32)2525, 4);
					row.Add("bigint", (Int64)25251414, 8);
					row.Add("decimal", 10M, 9);	//(Decimal)10
					row.Add("numeric", 123123M, 9); //(Decimal)123123
					row.Add("float", 17.1414257, 8);
					row.Add("real", (float)0.71425, 4);
					row.Add("char", "abcdefghij", 10);
					row.Add("nchar", "klmnopqrst", 10);
					row.Add("varchar", "qwertasdfg", 50);
					row.Add("nvarchar", "qwertasdfg", 50);
					break;
					#endregion

					#region Sybase
				case GHTUtils.DataBaseServer.Sybase:
					//row.Add("BIT", true, 1);
					row.Add("TINYINT", (byte)25, 1);
					row.Add("SMALLINT", (Int16)77, 2);
					row.Add("INT", (Int32)2525, 4);
					//row.Add("BIGINT", (Int64)25251414, 8);
					row.Add("DECIMAL", 10M, 9);	//(Decimal)10
					row.Add("NUMERIC", 123123M, 9); //(Decimal)123123
					row.Add("FLOAT", 17.1414257, 8);
					row.Add("REAL", (float)0.71425, 4);
					row.Add("CHAR", "abcdefghij", 10);
					row.Add("NCHAR", "klmnopqrst", 10);
					row.Add("VARCHAR", "qwertasdfg", 50);
					row.Add("NVARCHAR", "qwertasdfg", 50);
					break;
					#endregion

					#region ORACLE
				case GHTUtils.DataBaseServer.Oracle:
					row.Add("NUMBER", 21M, 22);	//(Decimal)21
					row.Add("LONG", SAMPLE_STRING, 2147483647);	//Default data type in .NET is system.String.
					row.Add("FLOAT", 1.234, 22);
					row.Add("VARCHAR", "qwertasdfg", 10);
					row.Add("NVARCHAR", "qwertasdfg", 20);
					row.Add("CHAR", "abcdefghij", 10);
					row.Add("NCHAR", "abcdefghij", 10);
					break;
					#endregion

					#region DB2
				case GHTUtils.DataBaseServer.DB2:
					row.Add("SMALLINT", (Int16)2, 2);
					row.Add("INTEGER", 7777, 4);
					row.Add("BIGINT", (Int64)21767267, 8);
					row.Add("DECIMAL", 123M, 9); //(decimal)123
					row.Add("REAL", (float)0.7, 4);
					row.Add("DOUBLE", 1.7, 8);
					row.Add("CHARACTER", "abcdefghij", 10);
					row.Add("VARCHAR", "qwertasdfg", 10);
					row.Add("LONGVARCHAR", SAMPLE_STRING, 32000);
					break;
					#endregion

					#region PostgreSQL
				case GHTUtils.DataBaseServer.PostgreSQL:
					
					// PostgreSQL ODBC Type BOOL returns String with value "1" 
					// so we don't run it on .NET
//					if (!GHTEnvironment.IsJavaRunTime())
//					{
//						row.Add("BOOL", "1", 1);
//					}
//					else
//					{
						row.Add("BOOL", true, 1);
//					}

					row.Add("INT2", (Int16)21, 2);
					row.Add("INT4", (Int32)30000, 4);
					row.Add("INT8", (Int64)30001, 8);
					row.Add("NUMERIC", 100000M, 10); //(decimal)100000
					row.Add("FLOAT4", (Single)7.23157, 4);
					row.Add("FLOAT8", (Double)7.123456, 8);
					row.Add("VARCHAR", "qwertasdfg", 10);
					row.Add("CHAR", "abcdefghij", 10);
					row.Add("NCHAR", "klmnopqrst", 10);
					break;
					#endregion
			}
			return row;
		}
		/// <summary>
		/// Creates a DbTypeParametersCollection with default types and data for the TYPES_EXTENDED table.
		/// </summary>
		/// <returns>The initialized DbTypeParametersCollection</returns>
		public static DbTypeParametersCollection GetExtendedDbTypesParameters()
		{
			DbTypeParametersCollection row = new DbTypeParametersCollection(EXTENDED_TYPES_TABLE_NAME);
			switch (DataProvider.GetDbType(DataProvider.ConnectionString))
			{
					#region SQLServer
				case GHTUtils.DataBaseServer.SQLServer:
					row.Add("text", SAMPLE_STRING, 16);
					row.Add("ntext", SAMPLE_STRING, 16);
					row.Add("binary", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0}, 50);
					row.Add("varbinary", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0}, 50);
					row.Add("datetime", new DateTime(2004, 8, 9, 20, 30, 15, 500), 8);
					row.Add("smalldatetime", new DateTime(2004, 8, 9, 20, 30, 00), 4);
					break;
					#endregion

					#region Sybase
				case GHTUtils.DataBaseServer.Sybase:
					row.Add("TEXT", SAMPLE_STRING, 16);
					//There is probably a bug in the jdbc driver , we've tried to insert this string using
					//sybase command tool and it gave the same result (3850)
					row.Add("NTEXT", SAMPLE_STRING.Trim() , 16);
					row.Add("BINARY", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0}, 50);
					row.Add("VARBINARY", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
														0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0}, 50);
					row.Add("DATETIME", new DateTime(2004, 8, 9, 20, 30, 15, 500), 8);
					row.Add("SMALLDATETIME", new DateTime(2004, 8, 9, 20, 30, 00), 4);
					break;
					#endregion

					#region ORACLE
				case GHTUtils.DataBaseServer.Oracle:
					row.Add("RAW", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0}, 10);
					row.Add("LONGRAW", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													}, 100);
					row.Add("DATE", new DateTime(2004, 8, 9, 20, 30, 15), 7);
					
					// The .NET Framework provides support for Oracle LOBs in the OracleClient namespace, but not in the OleDb namespace.
					// Since Visual MainWin does not support the OracleClient namespace, a partial support for this important feature is provided in the OleDb namespace.
					// See ms-help://MS.VSCC.2003/VMW.GH.1033/ghdoc/vmwdoc_ADONET_data_access_limitations_51.htm
					if (GHTEnvironment.IsJavaRunTime())
					{
						row.Add("BLOB", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
														,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													}, 4000);
						row.Add("CLOB",	SAMPLE_STRING
							+ SAMPLE_STRING
							+ SAMPLE_STRING
							+ SAMPLE_STRING
							+ SAMPLE_STRING
							+ SAMPLE_STRING
							, 4000);
						row.Add("NCLOB", SAMPLE_STRING
							+ SAMPLE_STRING
							+ SAMPLE_STRING
							+ SAMPLE_STRING
							+ SAMPLE_STRING
							+ SAMPLE_STRING
							, 4000);
					}
					break;
					#endregion

					#region DB2
				case GHTUtils.DataBaseServer.DB2:
					row.Add("DATE", new DateTime(2004, 8, 9, 20, 30, 15, 500).Date);
					row.Add("TIME", new TimeSpan(20, 30, 15));
					row.Add("TIMESTAMP", new DateTime(2004, 8, 9, 20, 30, 15, 500));
					row.Add("BLOB", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
													,0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0
												});
					row.Add("CLOB", SAMPLE_STRING
						+ SAMPLE_STRING
						+ SAMPLE_STRING
						+ SAMPLE_STRING
						+ SAMPLE_STRING
						+ SAMPLE_STRING
						);
					row.Add("DBCLOB", SAMPLE_STRING
						+ SAMPLE_STRING
						+ SAMPLE_STRING
						+ SAMPLE_STRING
						+ SAMPLE_STRING
						+ SAMPLE_STRING
						);
					break;
					#endregion

					#region PostgreSQL
				case GHTUtils.DataBaseServer.PostgreSQL:
					row.Add("BYTEA", new byte[]	{0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
													0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
													0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
													0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0,
													0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF, 0xF0}, 50);
					row.Add("DATE", new DateTime(2004, 8, 9));
					row.Add("INTERVAL", "01:22:00", 8);
					row.Add("TEXT", "abcdefg", 16); 
					row.Add("TIME", new System.TimeSpan(02,02,02));
					row.Add("TIMESTAMP", new DateTime(2004, 8, 9, 20, 30, 15, 500), 8);
					break;
					#endregion

			}
			return row;
		}
		public static DataTable CreateUniqueConstraint()
		{
			DataTable dt = GHTUtils.DataProvider.CreateParentDataTable();
			return CreateUniqueConstraint(dt);
		}
		public static DataTable CreateUniqueConstraint(DataTable dt)
		{
			Constraint con = new UniqueConstraint(dt.Columns["ParentId"]);
			dt.Constraints.Add(con);
			return dt;

		}
		public static void TryToBreakUniqueConstraint()
		{
			//Create the constraint
			DataTable dt =  CreateUniqueConstraint();
			//Try to violate the constraint
	
			DataRow dr1 = dt.NewRow();
			dr1[0] = 1;
			dt.Rows.Add(dr1);

		}
		public static DataSet CreateForigenConstraint()
		{
			DataTable parent = GHTUtils.DataProvider.CreateParentDataTable();
			DataTable child = GHTUtils.DataProvider.CreateChildDataTable(); 
			DataSet ds = new DataSet();
			ds.Tables.Add(parent); 
			ds.Tables.Add(child);

			Constraint con1 = new ForeignKeyConstraint(parent.Columns[0],child.Columns[0]);
			child.Constraints.Add(con1);

			return ds;
		}
		public static void TryToBreakForigenConstraint()
		{
			DataSet ds = CreateForigenConstraint();
			//Code to break:

			DataRow dr =  ds.Tables[1].NewRow();
			dr[0]=7;
			ds.Tables[1].Rows.Add(dr);

			ds.AcceptChanges();
			ds.EnforceConstraints=true;

		}

	} 
}

namespace GHTUtils.Base
{
	public class ADONetTesterClass : GHTUtils.Base.GHTBase 
	{
		#region " Base Constructors "
		protected ADONetTesterClass(System.IO.TextWriter Logger, bool LogOnSuccess) : base(Logger,LogOnSuccess){}
		protected ADONetTesterClass(bool LogOnSuccess):base(Console.Out, LogOnSuccess){}
		protected ADONetTesterClass():base(Console.Out, false){}

		#endregion

		private GHTUtils.DataBaseServer DBType ;


		#region "-----------  Build Update Commands --------------"
		protected void OleDbDataAdapter_BuildUpdateCommands(ref System.Data.OleDb.OleDbDataAdapter oleDBda)
		{
			System.Data.OleDb.OleDbConnection Conn = oleDBda.SelectCommand.Connection;

			oleDBda.DeleteCommand = new System.Data.OleDb.OleDbCommand();
			oleDBda.InsertCommand = new System.Data.OleDb.OleDbCommand();
			oleDBda.UpdateCommand = new System.Data.OleDb.OleDbCommand();

			oleDBda.DeleteCommand.Connection = Conn;
			oleDBda.InsertCommand.Connection = Conn;
			oleDBda.UpdateCommand.Connection = Conn;

			oleDBda.DeleteCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("EmployeeID",System.Data.OleDb.OleDbType.Integer)) ;
			oleDBda.DeleteCommand.Parameters["EmployeeID"].SourceVersion = DataRowVersion.Original;
			oleDBda.DeleteCommand.Parameters["EmployeeID"].SourceColumn = "EmployeeID";

			oleDBda.InsertCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("EmployeeID",System.Data.OleDb.OleDbType.Integer));
			oleDBda.InsertCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("LastName",System.Data.OleDb.OleDbType.VarWChar ,20));
			oleDBda.InsertCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("FirstName",System.Data.OleDb.OleDbType.VarWChar,10));
			oleDBda.InsertCommand.Parameters["EmployeeID"].SourceColumn = "EmployeeID";
			oleDBda.InsertCommand.Parameters["LastName"].SourceColumn = "LastName";
			oleDBda.InsertCommand.Parameters["FirstName"].SourceColumn = "FirstName";

			oleDBda.UpdateCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("Title",System.Data.OleDb.OleDbType.VarWChar,30));
			oleDBda.UpdateCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("EmployeeID",System.Data.OleDb.OleDbType.Integer));
			oleDBda.UpdateCommand.Parameters["EmployeeID"].SourceColumn = "EmployeeID";
			oleDBda.UpdateCommand.Parameters["Title"].SourceColumn = "Title";


			//for OleDB, ODBC
			string deleteSQL = "DELETE FROM Employees WHERE EmployeeID = ?";
			string insertSQL = "INSERT INTO Employees (EmployeeID, LastName, FirstName) VALUES (?, ?, ?)";
			string updateSQL = "UPDATE Employees SET Title = ? WHERE EmployeeID = ?";

			oleDBda.DeleteCommand.CommandText = deleteSQL;
			oleDBda.InsertCommand.CommandText = insertSQL;
			oleDBda.UpdateCommand.CommandText = updateSQL;

		}
		protected void SqlDataAdapter_BuildUpdateCommands(ref System.Data.SqlClient.SqlDataAdapter Sqlda)
		{
			System.Data.SqlClient.SqlConnection Conn = Sqlda.SelectCommand.Connection;

			Sqlda.DeleteCommand = new System.Data.SqlClient.SqlCommand();
			Sqlda.InsertCommand = new System.Data.SqlClient.SqlCommand();
			Sqlda.UpdateCommand = new System.Data.SqlClient.SqlCommand();

			Sqlda.DeleteCommand.Connection = Conn;
			Sqlda.InsertCommand.Connection = Conn;
			Sqlda.UpdateCommand.Connection = Conn;

			Sqlda.DeleteCommand.Parameters.Add(new System.Data.SqlClient.SqlParameter("@EmployeeID",DbType.Int32)) ;
			Sqlda.DeleteCommand.Parameters["@EmployeeID"].SourceVersion = DataRowVersion.Original;
			Sqlda.DeleteCommand.Parameters["@EmployeeID"].SourceColumn = "EmployeeID";

			Sqlda.InsertCommand.Parameters.Add(new System.Data.SqlClient.SqlParameter("@EmployeeID",DbType.Int32));
			Sqlda.InsertCommand.Parameters.Add(new System.Data.SqlClient.SqlParameter("@LastName",System.Data.SqlDbType.VarChar ,20));
			Sqlda.InsertCommand.Parameters.Add(new System.Data.SqlClient.SqlParameter("@FirstName",System.Data.SqlDbType.VarChar ,10));
			Sqlda.InsertCommand.Parameters["@EmployeeID"].SourceColumn = "EmployeeID";
			Sqlda.InsertCommand.Parameters["@LastName"].SourceColumn = "LastName";
			Sqlda.InsertCommand.Parameters["@FirstName"].SourceColumn = "FirstName";

			Sqlda.UpdateCommand.Parameters.Add(new System.Data.SqlClient.SqlParameter("@Title",System.Data.SqlDbType.VarChar,30));
			Sqlda.UpdateCommand.Parameters.Add(new System.Data.SqlClient.SqlParameter("@EmployeeID",DbType.Int32));
			Sqlda.UpdateCommand.Parameters["@EmployeeID"].SourceColumn = "EmployeeID";
			Sqlda.UpdateCommand.Parameters["@Title"].SourceColumn = "Title";

			//for Sql Client
			string deleteSql = "DELETE FROM Employees WHERE EmployeeID = @EmployeeID";
			string insertSql = "INSERT INTO Employees (EmployeeID, LastName, FirstName) VALUES (@EmployeeID, @LastName, @FirstName)";
			string updateSql = "UPDATE Employees SET Title = @Title WHERE EmployeeID = @EmployeeID";

			Sqlda.DeleteCommand.CommandText = deleteSql;
			Sqlda.InsertCommand.CommandText = insertSql;
			Sqlda.UpdateCommand.CommandText = updateSql;

		}
		#endregion

		#region "-----------  System.Data.Common.DBDataAdapter --------------"

		#region " DBDataAdapter - Fill / Fill Schema "

		protected void DbDataAdapter_Fill_Ds(System.Data.Common.DbDataAdapter dbDA)
		{
			DataSet dsResult, dsExpected ;
			int ExpectedRowsCount,ResultRowsCount;
			DataSet ds = new DataSet();

			ExpectedRowsCount = ReadDBData_Fill(dbDA,ref ds,false);

			// create expected dataset to compare result to
			dsExpected = ds.Copy();

			//make some changes, the fill method will overides those changes with data from DB.
			foreach (DataRow dr in ds.Tables[0].Select())
				dr["Country"] = "NeverNeverLand";
			ds.Tables[0].Columns.Remove("HomePhone"); //remove column, this column will be addedd during the fill process
			//ds.Tables.Remove(ds.Tables[1]); //remove the table, this table will be addedd during the fill process
			ds.AcceptChanges();
	
			// create source dataset to be filled
			dsResult = ds.Copy();
		
			//execute fill
			ResultRowsCount = dbDA.Fill(dsResult);
		
			CompareResults_Fill(dsResult,dsExpected);

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}

		protected void DbDataAdapter_Fill_Dt(System.Data.Common.DbDataAdapter dbDA)
		{
			DataSet dsResult, dsExpected ;
			int ExpectedRowsCount,ResultRowsCount;
			DataSet ds = new DataSet();

			ExpectedRowsCount = ReadDBData_Fill(dbDA,ref ds, false);

			//ds.Tables.Remove(ds.Tables[1]); //remove the table, fill only one table

			// create expected dataset to compare result to
			dsExpected = ds.Copy();

			//make some changes, the fill method will overides those changes with data from DB.
			foreach (DataRow dr in ds.Tables[0].Select())
				dr["Country"] = "NeverNeverLand";
			ds.Tables[0].Columns.Remove("HomePhone"); //remove column, this column will be addedd during the fill process
			ds.AcceptChanges();
		
			// create source dataset to be filled
			dsResult = ds.Copy();
		
			//execute fill
			ResultRowsCount = dbDA.Fill(dsResult.Tables["Table"]);

			CompareResults_Fill(dsResult,dsExpected);

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();


		}

		protected void DbDataAdapter_Fill_Ds_Str(System.Data.Common.DbDataAdapter dbDA)
		{
			DataSet dsResult, dsExpected ;
			int ExpectedRowsCount,ResultRowsCount;
			DataSet ds = new DataSet();

			ExpectedRowsCount = ReadDBData_Fill(dbDA,ref ds, false);

			// create expected dataset to compare result to
			dsExpected = ds.Copy();

			//make some changes, the fill method will overides those changes with data from DB.
			foreach (DataRow dr in ds.Tables[0].Select())
				dr["Country"] = "NeverNeverLand";
			ds.Tables[0].Columns.Remove("HomePhone"); //remove column, this column will be addedd during the fill process
			//ds.Tables.Remove(ds.Tables[1]); //remove the table, this table will be addedd during the fill process
			ds.AcceptChanges();
		
			// create source dataset to be filled
			dsResult = ds.Copy();
		
			//execute fill
			ResultRowsCount = dbDA.Fill(dsResult,dsResult.Tables[0].TableName );

			CompareResults_Fill(dsResult,dsExpected);

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}

		protected void DbDataAdapter_Fill_Ds_Int_Int_Str(System.Data.Common.DbDataAdapter dbDA)
		{
			DataSet dsResult, dsExpected ;
			int ExpectedRowsCount,ResultRowsCount;
			DataSet ds = new DataSet();

			ExpectedRowsCount = ReadDBData_Fill(dbDA,ref ds, false);
			// create expected dataset to compare result to
			dsExpected = ds.Copy();

			//make some changes, the fill method will overides those changes with data from DB.
			foreach (DataRow dr in ds.Tables[0].Select())
				dr["Country"] = "NeverNeverLand";	
			ds.Tables[0].Columns.Remove("HomePhone"); //remove column, this column will be addedd during the fill process
			//ds.Tables.Remove(ds.Tables[1]); //remove the table, this table will be addedd during the fill process
			ds.AcceptChanges();
		
			// create source dataset to be filled
			dsResult = ds.Copy();
		
			//execute fill
			ResultRowsCount = dbDA.Fill(dsResult,0,0,dsResult.Tables[0].TableName);
			CompareResults_Fill(dsResult,dsExpected);
		
			dsResult = ds.Copy();
			//modify expected dataset to match the expected result
			for (int i=0; i < dsExpected.Tables[0].Rows.Count ; i++)
			{
				if (i < 5 || i > 14) 
				{
					dsExpected.Tables[0].Rows[i]["Country"] = "NeverNeverLand";
					dsExpected.Tables[0].Rows[i]["HomePhone"] = DBNull.Value; 
				}
			}
			ResultRowsCount = dbDA.Fill(dsResult,5,10,dsResult.Tables[0].TableName);
			CompareResults_Fill(dsResult,dsExpected);

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}


		protected void DbDataAdapter_FillSchema_Ds_SchemaType(System.Data.Common.DbDataAdapter dbDA)
		{
			DataSet dsResult = new DataSet(); 
			DataSet dsExpected = new DataSet();
		
			// create expected dataset to compare result to
			ReadDBData_Fill(dbDA,ref dsExpected ,true);

			//  Note:   When handling batch SQL statements that return multiple results, 
			//	the implementation of FillSchema for the .NET Framework Data Provider for OLEDB 
			//	retrieves schema information for only the first result. 
			//	To retrieve schema information for multiple results, use Fill with the MissingSchemaAction set to AddWithKey
			//			if (dbDA.GetType() == typeof(System.Data.OleDb.OleDbDataAdapter)) 
			//				dsExpected.Tables.Remove(dsExpected.Tables[1]);

			//execute FillSchema

			//dsResult = dsExpected.Copy();
			DataTable[] dtArr = dbDA.FillSchema(dsResult,SchemaType.Mapped );

			//************  Fix .Net bug? (FillSchema method add AutoIncrement=true) *******************
			dsResult.Tables[0].Columns["EmployeeID"].AutoIncrement = false;
		
			CompareResults_Fill(dsResult,dsExpected);

			Exception exp = null;
			try
			{
				BeginCase("Check return value - Table[0]");
				Compare(dtArr[0],dsResult.Tables[0] );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//			if (dbDA.GetType() != typeof(System.Data.OleDb.OleDbDataAdapter)) 
			//				try
			//				{
			//					BeginCase("Check return value - Table[1]");
			//					Compare(dtArr[1],dsResult.Tables[1]);
			//				}
			//				catch(Exception ex)	{exp = ex;}
			//				finally	{EndCase(exp); exp = null;}
	
			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}

		protected void DbDataAdapter_FillSchema_Dt_SchemaType(System.Data.Common.DbDataAdapter dbDA)
		{
			DataSet dsResult = new DataSet(); 
			DataSet dsExpected = new DataSet();
		
			// create expected dataset to compare result to
			ReadDBData_Fill(dbDA,ref dsExpected ,true);

			//dsExpected.Tables.Remove(dsExpected.Tables[1]);

			//execute FillSchema

			dsResult.Tables.Add("Table");
			DataTable dt = dbDA.FillSchema(dsResult.Tables[0],SchemaType.Mapped );

			//************  Fix .Net bug? (FillSchema method add AutoIncrement=true) *******************
			dsResult.Tables[0].Columns["EmployeeID"].AutoIncrement = false;

			CompareResults_Fill(dsResult,dsExpected);
			Exception exp = null;
			try
			{
				BeginCase("Check return value - Table[0]");
				Compare(dt,dsResult.Tables[0] );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}

		protected void DbDataAdapter_FillSchema_Ds_SchemaType_Str(System.Data.Common.DbDataAdapter dbDA)
		{
			DataSet dsResult = new DataSet(); 
			DataSet dsExpected = new DataSet();
		
			// create expected dataset to compare result to
			ReadDBData_Fill(dbDA,ref dsExpected ,true);

			//dsExpected.Tables.Remove(dsExpected.Tables[1]);

			//execute FillSchema

			dsResult.Tables.Add("Table");
			DataTable[] dtArr = dbDA.FillSchema(dsResult,SchemaType.Mapped,dsResult.Tables[0].TableName);

			//************  Fix .Net bug? (FillSchema method add AutoIncrement=true) *******************
			dsResult.Tables[0].Columns["EmployeeID"].AutoIncrement = false;

			CompareResults_Fill(dsResult,dsExpected);
			Exception exp = null;
			try
			{
				BeginCase("Check return value - Table[0]");
				Compare(dtArr[0],dsResult.Tables[0] );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}


		private int ReadDBData_Fill(System.Data.Common.DbDataAdapter dbDA, ref DataSet ds, bool ReadSchemaOnly)
		{
			int ExpectedRowsCount = 0;
			IDbDataAdapter Ida = (IDbDataAdapter)dbDA;
			IDbCommand ICmd = Ida.SelectCommand; 
			IDbConnection IConn = ICmd.Connection; 
			IDataReader Idr;
			IConn.ConnectionString = GHTUtils.DataProvider.ConnectionString;
			ICmd.CommandText = "SELECT EmployeeID, LastName, FirstName, Title, Address, City, Region, Country, Extension, HomePhone  FROM Employees "  ;
			IConn.Open();

			//get db type
			DBType = DataProvider.GetDbType(((IDbDataAdapter)dbDA).SelectCommand.Connection.ConnectionString);

			// Execute data Reader - Get Expected results
			Idr = ICmd.ExecuteReader();

			// create temp dataset to insert results
			ExpectedRowsCount = DataReaderFill_Fill(ref ds,ref Idr,ReadSchemaOnly);
			Idr.Close();
			return ExpectedRowsCount;
		}
		private void CompareResults_Fill(DataSet dsResult,DataSet dsExpected )
		{
			Exception exp = null;

			//			try
			//			{
			//				BeginCase("Compare Rows count");
			//				// ???????   Fill return count for first table only    ??????
			//				Compare(ExpectedRowsCount  ,ResultRowsCount );
			//			}
			//			catch(Exception ex)	{exp = ex;}
			//			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Compare data");
				Compare(dsResult.GetXml() ,dsExpected.GetXml());
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Compare schema");
				Compare(dsResult.GetXmlSchema() ,dsExpected.GetXmlSchema());
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		private int DataReaderFill_Fill(ref DataSet dsExpected, ref IDataReader Idr,bool ReadSchemaOnly)
		{
			bool blnNextResults;
			int RowsAffected = 0;
			object[] objArr = null;
			DataTable SchemaTable = null;
			do
			{
				SchemaTable = Idr.GetSchemaTable();

				//add new table with the right amount of columns, the first table must be named "Table"
				if (dsExpected.Tables.Count == 0)
					dsExpected.Tables.Add(new DataTable("Table"));
				else
					dsExpected.Tables.Add();
				for (int i = 0 ; i < Idr.FieldCount; i++)
				{
					dsExpected.Tables[dsExpected.Tables.Count-1].Columns.Add(new DataColumn(Idr.GetName(i),Idr.GetFieldType(i)));
					if (ReadSchemaOnly)	// add schema info
					{
						dsExpected.Tables[dsExpected.Tables.Count-1].Columns[i].AllowDBNull = (bool)SchemaTable.Rows[i]["AllowDBNull"];
						dsExpected.Tables[dsExpected.Tables.Count-1].Columns[i].AutoIncrement = (bool)SchemaTable.Rows[i]["IsAutoIncrement"];
						dsExpected.Tables[dsExpected.Tables.Count-1].Columns[i].ReadOnly = (bool)SchemaTable.Rows[i]["IsReadOnly"];
						dsExpected.Tables[dsExpected.Tables.Count-1].Columns[i].Unique = (bool)SchemaTable.Rows[i]["IsUnique"];
						if (dsExpected.Tables[dsExpected.Tables.Count-1].Columns[i].DataType == typeof(string))
							dsExpected.Tables[dsExpected.Tables.Count-1].Columns[i].MaxLength = (int)SchemaTable.Rows[i]["ColumnSize"];
					}
				}

				if (!ReadSchemaOnly)
				{
					//array that holds the current rows values
					objArr = new object[Idr.FieldCount];

					//fill the new table
					while (Idr.Read())
					{
						Idr.GetValues(objArr);
						//update existing row, if no row is found - add it as new row
						dsExpected.Tables[dsExpected.Tables.Count-1].LoadDataRow(objArr,false);
						RowsAffected++;
					}
				}

				//get next record set 
				blnNextResults = Idr.NextResult(); 
			} 
			while (blnNextResults);

			// add primary key, fill method will update existing rows instead of insert new ones
			dsExpected.Tables[0].PrimaryKey = new DataColumn[] {dsExpected.Tables[0].Columns["EmployeeID"]};
			//if (ReadSchemaOnly)	dsExpected.Tables[1].PrimaryKey = new DataColumn[] {dsExpected.Tables[1].Columns["CustomerID"]};
			dsExpected.AcceptChanges();

			return RowsAffected;
		
		}

		#endregion

		#region " DBDataAdapter - FillError "
		private bool blnReadDBData_Fill = false;

		protected void DbDataAdapter_FillError(System.Data.Common.DbDataAdapter dbDA)
		{
			Exception exp = null;
			IDbDataAdapter Ida = (IDbDataAdapter)dbDA;
			IDbCommand ICmd = Ida.SelectCommand; 
			IDbConnection IConn = ICmd.Connection; 
			IConn.ConnectionString = GHTUtils.DataProvider.ConnectionString;
			ICmd.CommandText = "SELECT CustomerID, CompanyName, City, Country, Phone FROM Customers ";
			IConn.Open();

			DataSet ds = new DataSet();
			ds.Tables.Add(new DataTable("Customers"));
			ds.Tables[0].Columns.Add("CustomerID",typeof(byte));
        
			//check FillError event
			dbDA.FillError += new FillErrorEventHandler(dbDA_FillError);
			blnReadDBData_Fill = false;
			try
			{
				BeginCase("FillError");
				try 
				{
					dbDA.Fill(ds,"Customers");
				}
				catch (Exception ){};
				Compare(blnReadDBData_Fill ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dbDA.FillError -= new FillErrorEventHandler(dbDA_FillError);

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}
		private void dbDA_FillError(object sender, FillErrorEventArgs args)
		{
			blnReadDBData_Fill = true;
			args.Continue = false;
		}

		#endregion

		#region " DBDataAdapter - Update "
		protected void DbDataAdapter_Update_Ds(System.Data.Common.DbDataAdapter dbDA)
		{
			int NumberOfAffectedRows = 0;
			Exception exp = null;

			// --------- get data from DB -----------------
			DataSet ds = PrepareDBData_Update(dbDA);

			// --------- prepare dataset for update method -----------------
			DataSet dsDB1 = ds.Copy(); 
		
			// --------- prepare dataset for DBConcurrencyException -----------------
			DataSet dsDB2 = ds.Copy(); 
		
			//update expecteed dataset
			dsDB2.Tables[0].Rows.Add(new object[] {9994,"Ofer", "Borshtein", "Delete"});
			dsDB2.Tables[0].Rows.Add(new object[] {9995,"Ofer", "Borshtein", "Update"});
			dsDB2.Tables[0].Rows.Find(9996).Delete();
			dsDB2.AcceptChanges();

			//make changes to the DataBase (through the dataset)
			dsDB1.Tables[0].Rows.Add(new object[] {9991,"Ofer","Borshtein","Insert"});
			dsDB1.Tables[0].Rows.Find(9992).Delete();
			dsDB1.Tables[0].Rows.Find(9993)["Title"] = "Jack the ripper"; 

			//execute update to db
			NumberOfAffectedRows = dbDA.Update(dsDB1);
		
			try
			{
				BeginCase("Number Of Affected Rows");
				Compare(NumberOfAffectedRows ,3 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	

			//get result from db in order to check them
			DataSet dsExpected = new DataSet(); //ds.Reset();
			dbDA.Fill(dsExpected);
			dsExpected.Tables[0].PrimaryKey = new DataColumn[] {dsExpected.Tables[0].Columns["EmployeeID"]};

			CompareResults_Update(dsDB1,dsDB2,ref dbDA);
			CompareResults_Update_Ds_Exception(dsDB2,ref dbDA);

			//Create rows which not exists in the DB but exists in the DS with row state = deleted
			//this will cause the Update to fail.
			dsDB1.Tables[0].Rows.Add(new object[] {9997,"Ofer", "Borshtein", "Delete"});
			dsDB1.Tables[0].Rows.Add(new object[] {9998,"Ofer", "Borshtein", "Delete"});
			dsDB1.AcceptChanges();
			dsDB1.Tables[0].Rows.Find(9997).Delete();
			dsDB1.Tables[0].Rows.Find(9998).Delete();

			//Check System.Data.DBConcurrencyException
			//The exception that is thrown by the DataAdapter during the update operation if the number of rows affected equals zero.
			try
			{
				BeginCase("Check DBConcurrencyException");
				try
				{
					NumberOfAffectedRows = dbDA.Update(dsDB1);
				}
				catch (DBConcurrencyException ex) {exp=ex;}
				Compare(exp.GetType(),typeof(DBConcurrencyException) );
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();


		}

		private void CompareResults_Update_Ds_Exception(DataSet dsResultException,ref System.Data.Common.DbDataAdapter dbDA)
		{
			int NumberOfAffectedRows = 0;
			Exception exp = null;
			Exception e = null;

		
			// --------- check for DBConcurrencyException /UniqueConstraint -----------------
			//	call AcceptChanges after each exception check in order to make sure that we check only the the current row 


			try
			{
				BeginCase("DBConcurrencyException - Delete");
				dsResultException.Tables[0].Rows.Find(9994).Delete();
				//no row with row version delete exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException);}
				catch (DBConcurrencyException dbExp){e = dbExp;}
				Compare(e.GetType(),typeof(DBConcurrencyException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Delete Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();

			try
			{
				BeginCase("DBConcurrencyException - Update");
				dsResultException.Tables[0].Rows.Find(9995)["Title"] = "Jack the ripper"; 
				//no row with row version Update exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException);}
				catch (DBConcurrencyException dbExp){e = dbExp;}
				Compare(e.GetType(),typeof(DBConcurrencyException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Update Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();


			try
			{
				BeginCase("DBConcurrencyException - Insert");
				dsResultException.Tables[0].Rows.Add(new object[] {9996,"Ofer","Borshtein","Insert"});
				//no row with row version Insert exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException);}
				catch (Exception dbExp){e = dbExp;} //throw System.Exception
				Compare(e != null ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Insert Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();


		}
	

		protected void DbDataAdapter_Update_Dt(System.Data.Common.DbDataAdapter dbDA)
		{
			int NumberOfAffectedRows = 0;
			Exception exp = null;

			// --------- get data from DB -----------------
			DataSet ds = PrepareDBData_Update(dbDA);			

			// --------- prepare dataset for update method -----------------
			DataSet dsDB1 = ds.Copy(); 
		
			// --------- prepare dataset for DBConcurrencyException -----------------
			DataSet dsDB2 = ds.Copy(); 

			//update dataset
			dsDB2.Tables[0].Rows.Add(new object[] {9994,"Ofer", "Borshtein", "Delete"});
			dsDB2.Tables[0].Rows.Add(new object[] {9995,"Ofer", "Borshtein", "Update"});
			dsDB2.Tables[0].Rows.Find(9996).Delete();
			dsDB2.AcceptChanges();

		
			dsDB1.Tables[0].Rows.Add(new object[] {9991,"Ofer","Borshtein","Insert"});
			dsDB1.Tables[0].Rows.Find(9992).Delete();
			dsDB1.Tables[0].Rows.Find(9993)["Title"] = "Jack the ripper"; 

			//execute update to db
			NumberOfAffectedRows = dbDA.Update(dsDB1.Tables[0]);
		
			try
			{
				BeginCase("Number Of Affected Rows");
				Compare(NumberOfAffectedRows ,3 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	

			//get result from db in order to check them
			DataSet dsExpected = new DataSet(); //ds.Reset();
			dbDA.Fill(dsExpected);
			dsExpected.Tables[0].PrimaryKey = new DataColumn[] {dsExpected.Tables[0].Columns["EmployeeID"]};

			CompareResults_Update(dsDB1,dsDB2,ref dbDA);
			CompareResults_Update_Dt_Exception(dsDB2,ref dbDA);

			//Create rows which not exists in the DB but exists in the DS with row state = deleted
			//this will cause the Update to fail.
			dsDB1.Tables[0].Rows.Add(new object[] {9997,"Ofer", "Borshtein", "Delete"});
			dsDB1.Tables[0].Rows.Add(new object[] {9998,"Ofer", "Borshtein", "Delete"});
			dsDB1.AcceptChanges();
			dsDB1.Tables[0].Rows.Find(9997).Delete();
			dsDB1.Tables[0].Rows.Find(9998).Delete();

			//Check System.Data.DBConcurrencyException
			//The exception that is thrown by the DataAdapter during the update operation if the number of rows affected equals zero.
			try
			{
				BeginCase("Check DBConcurrencyException");
				try
				{
					NumberOfAffectedRows = dbDA.Update(dsDB1.Tables[0]);
				}
				catch (DBConcurrencyException ex) {exp=ex;}
				Compare(exp.GetType(),typeof(DBConcurrencyException) );
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();


		}

		private void CompareResults_Update_Dt_Exception(DataSet dsResultException,ref System.Data.Common.DbDataAdapter dbDA)
		{
			int NumberOfAffectedRows = 0;
			Exception exp = null;
			Exception e = null;


			// --------- check for DBConcurrencyException /UniqueConstraint -----------------
			//	call AcceptChanges after each exception check in order to make sure that we check only the the current row 


			try
			{
				BeginCase("DBConcurrencyException - Delete");
				dsResultException.Tables[0].Rows.Find(9994).Delete();
				//no row with row version delete exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException.Tables[0]);}
				catch (DBConcurrencyException dbExp){e = dbExp;}
				Compare(e.GetType(),typeof(DBConcurrencyException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Delete Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();

			try
			{
				BeginCase("DBConcurrencyException - Update");
				dsResultException.Tables[0].Rows.Find(9995)["Title"] = "Jack the ripper"; 
				//no row with row version Update exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException.Tables[0]);}
				catch (DBConcurrencyException dbExp){e = dbExp;}
				Compare(e.GetType(),typeof(DBConcurrencyException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Update Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();


			try
			{
				BeginCase("DBConcurrencyException - Insert");
				dsResultException.Tables[0].Rows.Add(new object[] {9996,"Ofer","Borshtein","Insert"});
				//no row with row version Insert exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException.Tables[0]);}
				catch (Exception dbExp){e = dbExp;} //throw System.Exception
				Compare(e != null ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Insert Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();

		}


		protected void DbDataAdapter_Update_Dr(System.Data.Common.DbDataAdapter dbDA)
		{
			int NumberOfAffectedRows = 0;
			Exception exp = null;

			// --------- get data from DB -----------------
			DataSet ds = PrepareDBData_Update(dbDA);

			// --------- prepare dataset for update method -----------------
			DataSet dsDB1 = ds.Copy(); 
		
			// --------- prepare dataset for DBConcurrencyException -----------------
			DataSet dsDB2 = ds.Copy(); 

			//update dataset
			dsDB2.Tables[0].Rows.Add(new object[] {9994,"Ofer", "Borshtein", "Delete"});
			dsDB2.Tables[0].Rows.Add(new object[] {9995,"Ofer", "Borshtein", "Update"});
			dsDB2.Tables[0].Rows.Find(9996).Delete();
			dsDB2.AcceptChanges();

		
			dsDB1.Tables[0].Rows.Add(new object[] {9991,"Ofer","Borshtein","Insert"});
			dsDB1.Tables[0].Rows.Find(9992).Delete();
			dsDB1.Tables[0].Rows.Find(9993)["Title"] = "Jack the ripper"; 

			//execute update to db
		
			DataRow[] drArr = new DataRow[dsDB1.Tables[0].Rows.Count] ;
			dsDB1.Tables[0].Rows.CopyTo(drArr,0);
			NumberOfAffectedRows = dbDA.Update(drArr);
		
			try
			{
				BeginCase("Number Of Affected Rows");
				Compare(NumberOfAffectedRows ,3 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	

			//get result from db in order to check them
			DataSet dsExpected = new DataSet(); //ds.Reset();
			dbDA.Fill(dsExpected);
			dsExpected.Tables[0].PrimaryKey = new DataColumn[] {dsExpected.Tables[0].Columns["EmployeeID"]};

			CompareResults_Update(dsDB1,dsDB2,ref dbDA);
			CompareResults_Update_Dr_Exception(dsDB2,ref dbDA);

			//Create rows which not exists in the DB but exists in the DS with row state = deleted
			//this will cause the Update to fail.
			dsDB1.Tables[0].Rows.Add(new object[] {9997,"Ofer", "Borshtein", "Delete"});
			dsDB1.Tables[0].Rows.Add(new object[] {9998,"Ofer", "Borshtein", "Delete"});
			dsDB1.AcceptChanges();
			dsDB1.Tables[0].Rows.Find(9997).Delete();
			dsDB1.Tables[0].Rows.Find(9998)[1] = "Updated!";
		
			drArr = new DataRow[dsDB1.Tables[0].Rows.Count];
			dsDB1.Tables[0].Rows.CopyTo(drArr,0);

			//Check System.Data.DBConcurrencyException
			//The exception that is thrown by the DataAdapter during the update operation if the number of rows affected equals zero.
			try
			{
				BeginCase("Check DBConcurrencyException");
				try
				{
					NumberOfAffectedRows = dbDA.Update(drArr);
				}
				catch (DBConcurrencyException ex) {exp=ex;}
				Compare(exp.GetType(),typeof(DBConcurrencyException) );
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();


		}

		private void CompareResults_Update_Dr_Exception(DataSet dsResultException,ref System.Data.Common.DbDataAdapter dbDA)
		{
			int NumberOfAffectedRows = 0;
			Exception exp = null;
			Exception e = null;
		
			DataRow[] drArr = new DataRow[dsResultException.Tables[0].Rows.Count];
		
			// --------- check for DBConcurrencyException /UniqueConstraint -----------------
			//	call AcceptChanges after each exception check in order to make sure that we check only the the current row 

			try
			{
				BeginCase("DBConcurrencyException - Delete");
				dsResultException.Tables[0].Rows.Find(9994).Delete();
				dsResultException.Tables[0].Rows.CopyTo(drArr,0);
				//no row with row version delete exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(drArr);}
				catch (DBConcurrencyException dbExp){e = dbExp;}
				Compare(e.GetType(),typeof(DBConcurrencyException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Delete Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();

			try
			{
				BeginCase("DBConcurrencyException - Update");
				dsResultException.Tables[0].Rows.Find(9995)["Title"] = "Jack the ripper"; 
				dsResultException.Tables[0].Rows.CopyTo(drArr,0);
				//no row with row version Update exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(drArr);}
				catch (DBConcurrencyException dbExp){e = dbExp;}
				Compare(e.GetType(),typeof(DBConcurrencyException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Update Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();


			try
			{
				BeginCase("DBConcurrencyException - Insert");
				dsResultException.Tables[0].Rows.Add(new object[] {9996,"Ofer","Borshtein","Insert"});
				dsResultException.Tables[0].Rows.CopyTo(drArr,0);
				//no row with row version Insert exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(drArr);}
				catch (Exception dbExp){e = dbExp;} //throw System.Exception
				Compare(e != null ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Insert Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();

		}


		protected void DbDataAdapter_Update_Ds_Str(System.Data.Common.DbDataAdapter dbDA)
		{
			int NumberOfAffectedRows = 0;
			Exception exp = null;

			// --------- get data from DB -----------------
			DataSet ds = PrepareDBData_Update(dbDA);

			// --------- prepare dataset for update method -----------------
			DataSet dsDB1 = ds.Copy(); 
		
			// --------- prepare dataset for DBConcurrencyException -----------------
			DataSet dsDB2 = ds.Copy(); 
		
			//update dataset
			dsDB2.Tables[0].Rows.Add(new object[] {9994,"Ofer", "Borshtein", "Delete"});
			dsDB2.Tables[0].Rows.Add(new object[] {9995,"Ofer", "Borshtein", "Update"});
			dsDB2.Tables[0].Rows.Find(9996).Delete();
			dsDB2.AcceptChanges();

		
			dsDB1.Tables[0].Rows.Add(new object[] {9991,"Ofer","Borshtein","Insert"});
			dsDB1.Tables[0].Rows.Find(9992).Delete();
			dsDB1.Tables[0].Rows.Find(9993)["Title"] = "Jack the ripper"; 

			//execute update to db
			NumberOfAffectedRows = dbDA.Update(dsDB1,dsDB1.Tables[0].TableName);
		
			try
			{
				BeginCase("Number Of Affected Rows");
				Compare(NumberOfAffectedRows ,3 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	

			//get result from db in order to check them
			DataSet dsExpected = new DataSet(); //ds.Reset();
			dbDA.Fill(dsExpected);
			dsExpected.Tables[0].PrimaryKey = new DataColumn[] {dsExpected.Tables[0].Columns["EmployeeID"]};

			CompareResults_Update(dsDB1,dsDB2,ref dbDA);
			CompareResults_Update_Ds_Str_Exception(dsDB2,ref dbDA);

			//Create rows which not exists in the DB but exists in the DS with row state = deleted
			//this will cause the Update to fail.
			dsDB1.Tables[0].Rows.Add(new object[] {9997,"Ofer", "Borshtein", "Delete"});
			dsDB1.Tables[0].Rows.Add(new object[] {9998,"Ofer", "Borshtein", "Delete"});
			dsDB1.AcceptChanges();
			dsDB1.Tables[0].Rows.Find(9997).Delete();
			dsDB1.Tables[0].Rows.Find(9998).Delete();


			//Check System.Data.DBConcurrencyException
			//The exception that is thrown by the DataAdapter during the update operation if the number of rows affected equals zero.
			try
			{
				BeginCase("Check DBConcurrencyException");
				try
				{
					NumberOfAffectedRows = dbDA.Update(dsDB1,dsDB1.Tables[0].TableName);
				}
				catch (DBConcurrencyException ex) {exp=ex;}
				Compare(exp.GetType(),typeof(DBConcurrencyException) );
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();


		}

		private void CompareResults_Update_Ds_Str_Exception(DataSet dsResultException,ref System.Data.Common.DbDataAdapter dbDA)
		{
			int NumberOfAffectedRows = 0;
			Exception exp = null;
			Exception e = null;

		
			// --------- check for DBConcurrencyException /UniqueConstraint -----------------
			//	call AcceptChanges after each exception check in order to make sure that we check only the the current row 


			try
			{
				BeginCase("DBConcurrencyException - Delete");
				dsResultException.Tables[0].Rows.Find(9994).Delete();
				//no row with row version delete exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException,dsResultException.Tables[0].TableName);}
				catch (DBConcurrencyException dbExp){e = dbExp;}
				Compare(e.GetType(),typeof(DBConcurrencyException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Delete Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();

			try
			{
				BeginCase("DBConcurrencyException - Update");
				dsResultException.Tables[0].Rows.Find(9995)["Title"] = "Jack the ripper"; 
				//no row with row version Update exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException,dsResultException.Tables[0].TableName);}
				catch (DBConcurrencyException dbExp){e = dbExp;}
				Compare(e.GetType(),typeof(DBConcurrencyException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Update Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();


			try
			{
				BeginCase("DBConcurrencyException - Insert");
				dsResultException.Tables[0].Rows.Add(new object[] {9996,"Ofer","Borshtein","Insert"});
				//no row with row version Insert exists - records affected = 0
				NumberOfAffectedRows = -1;
				try {NumberOfAffectedRows = dbDA.Update(dsResultException,dsResultException.Tables[0].TableName);}
				catch (Exception dbExp){e = dbExp;} //throw System.Exception
				Compare(e != null ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null; e = null;}
			try
			{
				BeginCase("Number Of Affected Rows - Insert Exception");
				Compare(NumberOfAffectedRows ,-1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			dsResultException.AcceptChanges();


		}

		/*
			* 
			* insert/update the database with data that will be used in testings
			* 
			*/
		protected void PrepareDataForTesting(string ConnectionString)
		{
			int iExists = 0;
			try
			{

				//create database specific date value
				string strBirthDateValue = "";
				switch (DataProvider.GetDbType(ConnectionString))
				{
					case DataBaseServer.DB2:
						strBirthDateValue = "'1988-05-31-15.33.44'";
						break;
					case DataBaseServer.Oracle:
						strBirthDateValue = "to_date('1988-05-31 15:33:44', 'yyyy-mm-dd HH24:mi:ss')";
						break;
					case DataBaseServer.PostgreSQL:
						strBirthDateValue = "to_timestamp('1988-05-31 15:33:44', 'yyyy-mm-dd HH24:mi:ss')";
						break;
					case DataBaseServer.SQLServer:
					case DataBaseServer.Sybase:
						strBirthDateValue = "'1988-May-31 15:33:44'";
						break;
				}
				OleDbConnection con = new OleDbConnection(ConnectionString);
				try
				{
					con.Open();
				}
				catch (Exception ex)
				{
					throw new Exception("PrepareDataForTesting failed trying to connect to DB using Connection string:" + ConnectionString + "\nException:" +  ex.ToString(),ex);
				}

				OleDbCommand cmd = new OleDbCommand("",con);

				//update / insert to table Employees
				for (int i = 100; i <= 700; i +=100)
				{
					cmd.CommandText = "select 1 from Employees where EmployeeID = " + i.ToString();
					iExists =  System.Convert.ToInt32(cmd.ExecuteScalar());
					if (iExists != 1) 
					{
						cmd.CommandText = "insert into Employees (EmployeeID, LastName, FirstName, Title, BirthDate) " + 
							" Values (" + i.ToString() + 
							",'Last" + i.ToString() 
							+ "','First" + i.ToString() 
							+ "', null, " 
							+ strBirthDateValue + ")";
						cmd.ExecuteNonQuery();
					}
					else
					{
						cmd.CommandText = "update Employees set " 
							+ " LastName = 'Last" + i.ToString() 
							+ "', FirstName = 'First" + i.ToString() 
							+ "', Title = null, BirthDate = " 
							+ strBirthDateValue 
							+ " where EmployeeID = " + i.ToString() ;
						// Log(cmd.CommandText);
						cmd.ExecuteNonQuery();
					}
				}

				//update / insert to table Customers
				for (int i = 100; i <= 700; i +=100)
				{
					cmd.CommandText = "select 1 from Customers where CustomerID = 'GH" + i.ToString() + "'";
					iExists =  System.Convert.ToInt32(cmd.ExecuteScalar());
					if (iExists != 1) 
					{
						cmd.CommandText = "insert into Customers (CustomerID , CompanyName, Phone) Values ('GH" + i.ToString() + "','Company" + i.ToString() + "','00-" + i.ToString() + i.ToString() + "')";
						cmd.ExecuteNonQuery();
					}
					else
					{
						cmd.CommandText = "update Customers set CompanyName = 'Company" + i.ToString() + "', Phone = '00-" + i.ToString() + i.ToString() + "' where CustomerID = 'GH" + i.ToString() + "'";
						cmd.ExecuteNonQuery();
					}
				}

				cmd.CommandText = "select 1 from Customers where CustomerID = 'GH200'";
				iExists =  System.Convert.ToInt32(cmd.ExecuteScalar());
				if (iExists != 1) 
				{
					cmd.CommandText = "insert into Customers (CustomerID , CompanyName) Values ('GH200','Company200')";
					cmd.ExecuteNonQuery();
				}
				else
				{
					cmd.CommandText = "update Customers set CompanyName = 'Company200' where CustomerID = 'GH200'";
					cmd.ExecuteNonQuery();
				}

				con.Close();
			}
			catch (Exception ex)
			{
				throw new Exception("PrepareDataForTesting failed with exception:" + ex.ToString(),ex);
			}
		}

		/*
		*	used to insert data to the database in order to check DataAdapter Update metods 
		*/
		protected DataSet PrepareDBData_Update(System.Data.Common.DbDataAdapter dbDA)
		{
			return PrepareDBData_Update(dbDA,false); 

		}
		protected DataSet PrepareDBData_Update(System.Data.Common.DbDataAdapter dbDA,bool sqlConnectionString)
		{
			IDbDataAdapter Ida = (IDbDataAdapter)dbDA;
			IDbCommand ICmd = Ida.SelectCommand;
			ICmd.CommandText = "SELECT EmployeeID, LastName, FirstName, Title FROM Employees WHERE EmployeeID in (9991,9992,9993,9994,9995,9996)";  
			IDbConnection IConn = ICmd.Connection; 
			if (!sqlConnectionString)
			{
				IConn.ConnectionString = GHTUtils.DataProvider.ConnectionString;
			}
			IConn.Open();
					
			//Insert rows to be updated (insert,delete,update)
			IDbCommand cmd;
			if (sqlConnectionString)
			{
				cmd = new System.Data.SqlClient.SqlCommand();
				cmd.Connection = (System.Data.SqlClient.SqlConnection)IConn; 
			}
			else
			{
				cmd = new System.Data.OleDb.OleDbCommand();
				cmd.Connection = (System.Data.OleDb.OleDbConnection)IConn;
			}


			//run execute after each command because DB2 doesn't support multiple commands
			cmd.CommandText =  "DELETE FROM Employees WHERE EmployeeID in (9991,9992,9993,9994,9995,9996,9997,9998)";
			cmd.ExecuteNonQuery();

			//only for SQL Server
			DataBaseServer DBType =  DataProvider.GetDbType(IConn.ConnectionString);

			cmd.CommandText = "INSERT INTO Employees (EmployeeID, LastName, FirstName, Title) VALUES(9992, 'Ofer', 'Borshtein', 'delete')";
			//if (DBType == DataBaseServer.SQLServer) cmd.CommandText = "SET IDENTITY_INSERT Employees ON;" + cmd.CommandText;
			cmd.ExecuteNonQuery();
			cmd.CommandText = "INSERT INTO Employees (EmployeeID, LastName, FirstName, Title) VALUES(9993, 'Ofer', 'Borshtein', 'Update')";
			//if (DBType == DataBaseServer.SQLServer) cmd.CommandText = "SET IDENTITY_INSERT Employees ON;" + cmd.CommandText;
			cmd.ExecuteNonQuery();
			cmd.CommandText = "INSERT INTO Employees (EmployeeID, LastName, FirstName, Title) VALUES(9996, 'Ofer', 'Borshtein', 'Exp')";
			//if (DBType == DataBaseServer.SQLServer) cmd.CommandText = "SET IDENTITY_INSERT Employees ON;" + cmd.CommandText;
			cmd.ExecuteNonQuery();

			//cmd.CommandText += "INSERT INTO Employees (EmployeeID, LastName, FirstName, Title) VALUES(9991, 'Ofer', 'Borshtein', 'Insert'); ";
			//cmd.CommandText += "INSERT INTO Employees (EmployeeID, LastName, FirstName, Title) VALUES(9994, 'Ofer', 'Borshtein', 'Exp'); ";
			//cmd.CommandText += "INSERT INTO Employees (EmployeeID, LastName, FirstName, Title) VALUES(9995, 'Ofer', 'Borshtein', 'Exp'); ";
			//cmd.CommandText += "INSERT INTO Employees (EmployeeID, LastName, FirstName, Title) VALUES(9997, 'Ofer', 'Borshtein', 'delete'); ";
			//cmd.CommandText += "INSERT INTO Employees (EmployeeID, LastName, FirstName, Title) VALUES(9998, 'Ofer', 'Borshtein', 'delete'); ";

			DataSet ds = new DataSet();
			dbDA.Fill(ds);
			ds.Tables[0].PrimaryKey = new DataColumn[] {ds.Tables[0].Columns["EmployeeID"]};
			return ds;

		}


		private void CompareResults_Update(DataSet dsResult,DataSet dsResultException,ref System.Data.Common.DbDataAdapter dbDA)
		{
			Exception exp = null;
		
			//----------------- Compare dsDB with dsExcepted -----------------
			try
			{
				BeginCase("Insert Row ");
				Compare(dsResult.Tables[0].Rows.Find(9991) == null ,false );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Update Row ");
				Compare(dsResult.Tables[0].Rows.Find(9993)["Title"],"Jack the ripper");
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		
			try
			{
				BeginCase("Delete Row ");
				Compare(dsResult.Tables[0].Rows.Find(9992) ,null);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}			

		}

		#endregion

		protected void DBDataAdapter_DefaultSourceTableName()
		{
			Exception exp = null;
			try
			{
				BeginCase("DefaultSourceTableName");
				Compare(System.Data.Common.DbDataAdapter.DefaultSourceTableName , "Table");
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

		#endregion

		#region "-----------  System.Data.Common.DataAdapter --------------"
		protected void DataAdapter_AcceptChangesDuringFill(System.Data.Common.DbDataAdapter dbDA)
		{
			Exception exp = null;
			IDbDataAdapter Ida = (IDbDataAdapter)dbDA;
			IDbCommand ICmd = Ida.SelectCommand; 
			IDbConnection IConn = ICmd.Connection; 
			IConn.ConnectionString = GHTUtils.DataProvider.ConnectionString;
			IConn.Open();

			PrepareDataForTesting( GHTUtils.DataProvider.ConnectionString);

			//get the total rows count
			ICmd.CommandText = "SELECT Count(*) FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";
			int ExpectedRows = System.Convert.ToInt32(ICmd.ExecuteScalar());
			try
			{
				BeginCase("Check that Expected rows count > 0");
				Compare(ExpectedRows > 0 ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			ICmd.CommandText = "SELECT CustomerID, CompanyName, City, Country, Phone FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";

			DataSet ds = new DataSet();
			dbDA.AcceptChangesDuringFill = false;

        
			try
			{
				BeginCase("Execute Fill - check return rows count");
				int i = dbDA.Fill(ds);
				Compare(i ,ExpectedRows );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
				

			bool blnAcceptChanges = false;

			foreach (DataRow dr in ds.Tables[0].Rows)
			{
				if (dr.RowState != DataRowState.Added ) 
				{
					blnAcceptChanges = true;
					break;
				}
			}
			try
			{
				BeginCase("AcceptChangesDuringFill - false");
				Compare(blnAcceptChanges ,false );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			ds.Reset();
			dbDA.AcceptChangesDuringFill = true;
			dbDA.Fill(ds);
			blnAcceptChanges = false;            			
			foreach (DataRow dr in ds.Tables[0].Rows)
			{
				if (dr.RowState != DataRowState.Unchanged ) 
				{
					blnAcceptChanges = true;
					break;
				}
			}
			try
			{
				BeginCase("AcceptChangesDuringFill - true");
				Compare(blnAcceptChanges ,false );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();

		}

		protected void DataAdapter_ContinueUpdateOnError(System.Data.Common.DbDataAdapter dbDA)
		{
			/*
				!!!!!! Not working (TestName "ContinueUpdateOnError - true, check value 2")!!!!!
				If ContinueUpdateOnError is set to true, no exception is thrown when an error occurs during the update of a row. 
				The update of the row is skipped and the error information is placed in the RowError property of the row in error. 
				The DataAdapter continues to update subsequent rows.
				If ContinueUpdateOnError is set to false, an exception is thrown when an error occurs during the update of a row.
			*/
			Exception exp = null;

	
			IDbDataAdapter Ida = (IDbDataAdapter)dbDA;
			IDbCommand ICmd = Ida.SelectCommand; 
			IDbConnection IConn = ICmd.Connection; 
			IConn.ConnectionString = GHTUtils.DataProvider.ConnectionString;
			IConn.Open();

			PrepareDataForTesting( GHTUtils.DataProvider.ConnectionString);

			//get the total rows count
			ICmd.CommandText = "SELECT Count(*) FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";
			int ExpectedRows = System.Convert.ToInt32(ICmd.ExecuteScalar());
			try
			{
				BeginCase("Check that Expected rows count > 0");
				Compare(ExpectedRows > 0 ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			ICmd.CommandText = "SELECT CustomerID, CompanyName, City, Country, Phone FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";


			DataSet dsMem = new DataSet();	//Disconected dataset
			DataSet dsDB = new DataSet();	//DataBase data
			dbDA.AcceptChangesDuringFill = true;
			//get data from DB
			try
			{
				BeginCase("Execute Fill - check return rows count");
				int i = dbDA.Fill(dsMem);
				Compare(i ,ExpectedRows );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		

			//update data with invalid information (Max. length for Phone is 24)
			//					123456789012345678901234
			string newValue1 = "Very Long String That Will Raise An Error Yep!";
			string oldValue1 = dsMem.Tables[0].Rows[3]["Phone"].ToString();
			string oldValue2 = dsMem.Tables[0].Rows[4]["Phone"].ToString();
			string newValue2 = "03-1234";


			dsMem.Tables[0].Rows[3]["Phone"] = newValue1;
			dsMem.Tables[0].Rows[4]["Phone"] = newValue2;
			dbDA.ContinueUpdateOnError = true;
        
			//will not throw exception
			try
			{
				BeginCase("ContinueUpdateOnError - true, check exception");
				try
				{
					dbDA.Update(dsMem);
				}
				catch(Exception ex){exp = ex;}
				Compare(exp == null,true);
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			dbDA.Fill(dsDB); //get data from DB to check the update operation

			try
			{
				BeginCase("ContinueUpdateOnError - true, check RowError");
				Compare(dsMem.Tables[0].Rows[3].RowError.Length > 0 , true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("ContinueUpdateOnError - true, check value 1");
				Compare(dsDB.Tables[0].Rows[3]["Phone"] , oldValue1);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}


			/*		- Test excluded, it is not working in .NET too!
					//should continue the update
					try
					{
						BeginCase("ContinueUpdateOnError - true, check value 2");
						Compare(dsDB.Tables[0].Rows[4]["Phone"] , newValue2);  //--------- NOT WORKING !!! -----------
					}
					catch(Exception ex)	{exp = ex;}
					finally	{EndCase(exp); exp = null;}
			*/
			dsMem.Reset();
			dsDB.Reset();
			dbDA.Fill(dsMem);
			dsMem.Tables[0].Rows[3]["Phone"] = newValue1 ;
			dsMem.Tables[0].Rows[4]["Phone"] = newValue2;
			dbDA.ContinueUpdateOnError = false;
        
			try
			{
				BeginCase("ContinueUpdateOnError - false, check exception");
				try
				{
					dbDA.Update(dsMem);
				}
				catch(Exception ex){exp = ex;}
				Compare(exp == null,false);
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			dbDA.Fill(dsDB); //get data from DB to check the update operation
			try
			{
				BeginCase("ContinueUpdateOnError - false,check RowError");
				Compare(dsMem.Tables[0].Rows[3].RowError.Length > 0 ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			try
			{
				BeginCase("ContinueUpdateOnError - false,check value 1");
				Compare(dsDB.Tables[0].Rows[3]["Phone"] , oldValue1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}


			try
			{
				BeginCase("ContinueUpdateOnError - false,check value 2");
				Compare(dsDB.Tables[0].Rows[4]["Phone"] , oldValue2 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();

		}
		protected void DataAdapter_MissingMappingAction(System.Data.Common.DbDataAdapter dbDA)
		{
			Exception exp = null;
			IDbDataAdapter Ida = (IDbDataAdapter)dbDA;
			IDbCommand ICmd = Ida.SelectCommand; 
			IDbConnection IConn = ICmd.Connection; 
			IConn.ConnectionString = GHTUtils.DataProvider.ConnectionString;

			IConn.Open();

			//get the total rows count

			PrepareDataForTesting( GHTUtils.DataProvider.ConnectionString);

			ICmd.CommandText = "SELECT Count(*) FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";
			int ExpectedRows = System.Convert.ToInt32(ICmd.ExecuteScalar());
			try
			{
				BeginCase("Check that Expected rows count > 0");
				Compare(ExpectedRows > 0 ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			ICmd.CommandText = "SELECT CustomerID, CompanyName, City, Country, Phone FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";


			//init dataset
			DataSet ds = new DataSet();
			try
			{
				BeginCase("Execute Fill - check return rows count");
				int i = dbDA.Fill(ds);
				Compare(i ,ExpectedRows );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		
			//make dataset schema mismatch with DB
			ds.Tables[0].Columns.Remove("Country");
			ds.Tables[0].Clear();
        
			//--- Default value ---
        
			try
			{
				BeginCase("MissingMappingAction Default value");
				Compare(dbDA.MissingMappingAction , MissingMappingAction.Passthrough);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	
			//--- MissingMappingAction.Error ---
			ds.Tables[0].Clear();
			dbDA.MissingMappingAction  = MissingMappingAction.Error ;
			Exception ExMissingMappingAction = null;
			try
			{
				BeginCase("MissingMappingAction.Error");
				try
				{
					dbDA.Fill(ds);
				}
				catch (InvalidOperationException e)
				{
					ExMissingMappingAction = e;
				}
				Compare(ExMissingMappingAction.GetType() ,typeof(InvalidOperationException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingMappingAction.Error, Row.Count = 0");
				Compare(ds.Tables[0].Rows.Count , 0 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingMappingAction.Error, Column");
				Compare(ds.Tables[0].Columns.IndexOf("Country")  , -1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	

			//--- MissingMappingAction.Ignore ---
			ds.Tables[0].Clear();
			dbDA.MissingMappingAction  = MissingMappingAction.Ignore  ;
			ExMissingMappingAction = null;
			try
			{
				BeginCase("MissingMappingAction.Ignore");
				try
				{
					dbDA.Fill(ds);
				}
				catch (InvalidOperationException e)
				{
					ExMissingMappingAction = e;
				}
				Compare(ExMissingMappingAction ,null);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingMappingAction.Ignore, Row.Count = 0");
				Compare(ds.Tables[0].Rows.Count ,0);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingMappingAction.Ignore, Column");
				Compare(ds.Tables[0].Columns.IndexOf("Country")  , -1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//--- MissingMappingAction.Passthrough ---
			ds.Tables[0].Clear();
			dbDA.MissingMappingAction  = MissingMappingAction.Passthrough   ;
			ExMissingMappingAction = null;
			try
			{
				BeginCase("MissingMappingAction.Passthrough");
				try
				{
					dbDA.Fill(ds);
				}
				catch (InvalidOperationException e)
				{
					ExMissingMappingAction = e;
				}
				Compare(ExMissingMappingAction ,null);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingMappingAction.Passthrough, Row.Count > 0");
				Compare(ds.Tables[0].Rows.Count >= 0 ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingMappingAction.Passthrough, Column");
				Compare(ds.Tables[0].Columns.IndexOf("Country") >= 0  ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}
		protected void DataAdapter_MissingSchemaAction(System.Data.Common.DbDataAdapter dbDA)
		{
			Exception exp = null;
			IDbDataAdapter Ida = (IDbDataAdapter)dbDA;
			IDbCommand ICmd = Ida.SelectCommand; 
			IDbConnection IConn = ICmd.Connection; 
			IConn.ConnectionString = GHTUtils.DataProvider.ConnectionString;
			IConn.Open();

			PrepareDataForTesting( GHTUtils.DataProvider.ConnectionString);

			//get the total rows count
			ICmd.CommandText = "SELECT Count(*) FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";
			int ExpectedRows = System.Convert.ToInt32(ICmd.ExecuteScalar());
			try
			{
				BeginCase("Check that Expected rows count > 0");
				Compare(ExpectedRows > 0 ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			ICmd.CommandText = "SELECT CustomerID, CompanyName, City, Country, Phone FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";

			//get db type
			DBType = DataProvider.GetDbType(((IDbDataAdapter)dbDA).SelectCommand.Connection.ConnectionString);

			//init dataset
			DataSet ds = new DataSet();
			try
			{
				BeginCase("Execute Fill - check return rows count");
				int i = dbDA.Fill(ds);
				Compare(i ,ExpectedRows );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//make dataset schema mismatch with DB
			ds.Tables[0].Columns.Remove("Country");
			ds.Tables[0].Clear();
        

			//--- Default value ---
        
			try
			{
				BeginCase("MissingSchemaAction Default value");
				Compare(dbDA.MissingSchemaAction, MissingSchemaAction.Add);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	
			//--- MissingSchemaAction.Error ---
			ds.Tables[0].Clear();
			dbDA.MissingSchemaAction  = MissingSchemaAction.Error  ;
			Exception ExMissingSchemaAction = null;
			try
			{
				BeginCase("MissingSchemaAction.Error");
				try
				{
					dbDA.Fill(ds);
				}
				catch (InvalidOperationException e)
				{
					ExMissingSchemaAction = e;
				}
				Compare(ExMissingSchemaAction.GetType() ,typeof(InvalidOperationException));
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingSchemaAction.Error, Row.Count = 0");
				Compare(ds.Tables[0].Rows.Count , 0 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingSchemaAction.Error, Column");
				Compare(ds.Tables[0].Columns.IndexOf("Country")  , -1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	

			//--- MissingSchemaAction.Ignore ---
			try
			{
				//catch any exception that might occure 
				BeginCase("MissingSchemaAction.Ignore - invoke");
				ds.Tables[0].Clear();
				dbDA.MissingSchemaAction  = MissingSchemaAction.Ignore  ;
				ExMissingSchemaAction = null;
				dbDA.Fill(ds);
				Compare(true ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingSchemaAction.Ignore, Row.Count = 0");
				Compare(ds.Tables[0].Rows.Count > 0 ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingSchemaAction.Ignore, Column");
				Compare(ds.Tables[0].Columns.IndexOf("Country")  , -1 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

#if !KNOWN_BUG //BUG_NUM:1951
			try
			{
				BeginCase("MissingSchemaAction.Ignore, PrimaryKey");
				Compare(ds.Tables[0].PrimaryKey.Length == 0 ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
#endif

			//--- MissingSchemaAction.Add ---
			try
			{
				//catch any exception that might occure 
				BeginCase("MissingSchemaAction.Add - invoke");
				ds.Tables[0].Clear();
				dbDA.MissingSchemaAction  = MissingSchemaAction.Add   ;
				ExMissingSchemaAction = null;
				dbDA.Fill(ds);
				Compare(true ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingSchemaAction.Add, Row.Count > 0");
				Compare(ds.Tables[0].Rows.Count >= 0 ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingSchemaAction.Add, Column");
				Compare(ds.Tables[0].Columns.IndexOf("Country") >= 0  ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

#if !KNOWN_BUG //BUG_NUM:1952
			//DB2 don't return primary key
			if (DBType != DataBaseServer.DB2) 
			{
				try
				{
					BeginCase("MissingSchemaAction.AddWithKey, PrimaryKey");
					Compare(ds.Tables[0].PrimaryKey.Length  ,0);
				}
				catch(Exception ex)	{exp = ex;}
				finally	{EndCase(exp); exp = null;}
			}

			//--- MissingSchemaAction.AddWithKey ---
			try
			{
				//catch any exception that might occure 
				BeginCase("MissingSchemaAction.AddWithKey - invoke");
				ds.Tables[0].Clear();
				ds.Tables[0].Columns.Remove("Country");
				dbDA.MissingSchemaAction  = MissingSchemaAction.AddWithKey    ;
				ExMissingSchemaAction = null;
				dbDA.Fill(ds);
				Compare(true ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingSchemaAction.AddWithKey, Row.Count > 0");
				Compare(ds.Tables[0].Rows.Count >= 0 ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("MissingSchemaAction.AddWithKey, Column");
				Compare(ds.Tables[0].Columns.IndexOf("Country") >= 0  ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//DB2 don't return primary key
			if (DBType != DataBaseServer.DB2) 
			{
				try
				{
					BeginCase("MissingSchemaAction.AddWithKey, PrimaryKey");
					Compare(ds.Tables[0].PrimaryKey.Length > 0 ,true );
				}
				catch(Exception ex)	{exp = ex;}
				finally	{EndCase(exp); exp = null;}
			}
#endif

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}
		protected void DataAdapter_TableMappings(System.Data.Common.DbDataAdapter dbDA)
		{
			Exception exp = null;
			IDbDataAdapter Ida = (IDbDataAdapter)dbDA;
			IDbCommand ICmd = Ida.SelectCommand; 
			IDbConnection IConn = ICmd.Connection; 
			IConn.ConnectionString = GHTUtils.DataProvider.ConnectionString;
			IConn.Open();

			//--- Default value ---
        
			try
			{
				BeginCase("TableMappings Default value");
				Compare(dbDA.TableMappings.Count ,0);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//init dataset
			DataSet ds = new DataSet();
			ds.Tables.Add("CustTable");
			ds.Tables.Add("EmplTable");

			//load the tables
			dbDA.TableMappings.Add("Table","CustTable");
			ICmd.CommandText = "SELECT CustomerID, CompanyName, City, Country, Phone FROM Customers where CustomerID in ('GH100','GH200','GH300','GH400','GH500','GH600','GH700')";
			dbDA.Fill(ds);

			dbDA.TableMappings.Clear();
			dbDA.TableMappings.Add("Table","EmplTable");
			ICmd.CommandText = " SELECT EmployeeID, LastName, FirstName, Title FROM Employees where EmployeeID in (100,200)";
			dbDA.Fill(ds);

		
		
			try
			{
				BeginCase("TableMappings.Count");
				Compare(ds.Tables.Count ,2);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			try
			{
				BeginCase("Customers rows count");
				Compare(ds.Tables["CustTable"].Rows.Count > 0,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			try
			{
				BeginCase("Employees rows count");
				Compare(ds.Tables["EmplTable"].Rows.Count > 0,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			try
			{
				BeginCase("Employees Columns");
				Compare(ds.Tables["EmplTable"].Columns.IndexOf("EmployeeID") >= 0,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
			try
			{
				BeginCase("Customer Columns");
				Compare(ds.Tables["CustTable"].Columns.IndexOf("CustomerID") >= 0,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//Another checking 

			BeginCase("Check table mappings with stored procedure");
			try
			{
				DataSet ds1 = new DataSet();
				ds1.Tables.Add("EmplTable");
				dbDA.TableMappings.Clear();
				dbDA.TableMappings.Add("Table","EmplTable");
				ICmd.CommandType = CommandType.StoredProcedure;
				ICmd.CommandText = (DataProvider.GetDbType() == DataBaseServer.Oracle) ? "GHTDB.GH_DUMMY" : "othertestuser.GH_DUMMY";
				//TODO:Enbale it to sql server too
				//OleDbCommandBuilder.DeriveParameters((OleDbCommand)ICmd);
				//((OleDbParameter)ICmd.Parameters[0]).Value = "1";
				ICmd.Parameters.Add(new OleDbParameter("EmployeeIDPrm",1));
				dbDA.Fill(ds1);
				Compare(ds1.Tables.Count,1);
				Compare(ds1.Tables[0].Rows.Count >0,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		

			//close connection
			if (  ((IDbDataAdapter)dbDA).SelectCommand.Connection.State != ConnectionState.Closed )
				((IDbDataAdapter)dbDA).SelectCommand.Connection.Close();
		}
		#endregion

		public override void BeginTest(string testName)
		{
			base.BeginTest (testName);
			// Log(String.Format("DataBase Type is {0}", DataProvider.GetDbType()));
		}

		private void Log(DataSet ds,DataSet dsResult,DataSet dsExpected)
		{
			ds.WriteXml("C:\\ds.xml");
			ds.WriteXmlSchema("C:\\ds_S.xml");
		
			dsResult.WriteXml("C:\\dsResult.xml");
			dsExpected.WriteXml("C:\\dsExpected.xml");
		
			dsResult.WriteXmlSchema("C:\\dsResult_S.xml");
			dsExpected.WriteXmlSchema("C:\\dsExpected_S.xml");
		}

	}
}
