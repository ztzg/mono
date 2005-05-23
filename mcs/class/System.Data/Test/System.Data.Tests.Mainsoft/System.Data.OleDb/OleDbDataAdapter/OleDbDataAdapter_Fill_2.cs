using System;
using System.Data;
using System.Data.OleDb;
using NUnit.Framework;
using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
	[TestFixture]
	public class OleDbDataAdapter_Fill_2 : ADONetTesterClass 
	{
		private string nonUniqueId;
		[Test] public void Main()
		{
			OleDbDataAdapter_Fill_2 tc = new OleDbDataAdapter_Fill_2();
			Exception exp = null;
			try
			{
				tc.BeginTest("OleDbDataAdapter_Fill_2");
				tc.run();
			}
			catch(Exception ex){exp = ex;}
			finally	{tc.EndTest(exp);}
		}

		public void run()
		{
			
			
			OleDbConnection con = new OleDbConnection(GHTUtils.DataProvider.ConnectionString);
			con.Open();

			//doTestThis(con);
			doTestTypes1(con);
			//Don't know how to access diffrent database
			if ((DataProvider.GetDbType(con) != DataBaseServer.DB2) && (DataProvider.GetDbType(con) != DataBaseServer.PostgreSQL))
			{ 
				doTestTypes2(con);
				doTestTypes3(con);
			}
			
			doTestTypes4(con);
		//	doTestTypes5(con);   //Table direct --> multipe tables
			doTestTypes6(con);
			
			if ((DataProvider.GetDbType(con) != DataBaseServer.Oracle) && 
				(DataProvider.GetDbType(con) != DataBaseServer.PostgreSQL))
			{
				doTestTypes7(con); //Diffrent owner
				doTestTypes8(con); //Diffrent owner
				doTestTypes9(con);
			}
			
			if (DataProvider.GetDbType(con) != DataBaseServer.PostgreSQL)
			{
				doTestTypes10(con);
			}

			CallStoredProcedureInPackage(con);
			StoredProcedurePackageambiguity_InsidePackage(con);
			StoredProcedurePackageambiguity_OutsidePackage(con);

			if (con.State == ConnectionState.Open) con.Close();
		}

		public void doTestThis(OleDbConnection con)
		{
			Exception exp = null;
			OleDbCommand cmd = new OleDbCommand("GH_CREATETABLE", con);
				cmd.CommandType = CommandType.StoredProcedure;
				OleDbDataAdapter da = new OleDbDataAdapter(cmd);
				DataSet ds = new DataSet();

				try
				{
					BeginCase("Check effected rows after create table ddl in stored procedure.");
					int RowsAffected;
					RowsAffected = cmd.ExecuteNonQuery();
					int ExpectedRowsAffected;
					switch (DataProvider.GetDbType(con))
					{
						case DataBaseServer.SQLServer:
						case DataBaseServer.Sybase:
							ExpectedRowsAffected = 3;
							break;
						case DataBaseServer.Oracle:
							//In .NET the ExpectedRowsAffected is '1', where as in Java it is '-1', this gap is because of jdbc driver for oracle.
							ExpectedRowsAffected = (GHTUtils.GHTEnvironment.IsJavaRunTime()) ? -1 : 1;
							break;
						case DataBaseServer.DB2:
							ExpectedRowsAffected = -1;
							break;
						default:
							string errMsg = string.Format("GHT: Test not implemented for DB type: {0}", DataProvider.GetDbType(con));
							throw new NotImplementedException(errMsg);
					}
					Compare(RowsAffected ,ExpectedRowsAffected);
				}
				catch(Exception ex)	{exp = ex;}
				finally	{EndCase(exp); exp = null;}
		}


		#region Select by full table name in the same catalog
		public void doTestTypes1(OleDbConnection conn)
		{
			DataSet ds = new DataSet();
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;

			string tableName = getDbObjectName("Employees",conn);
			int expectedRowsCount = 8;

			#region Select by full table name in the same catalog

			string[] arr = new string[2];

			arr[0] = "LastName";
			arr[1] = "FirstName";

			prepareTableForTest(conn,expectedRowsCount,"Employees","EmployeeID",arr);
			comm.CommandText="select max(EmployeeID) from " + tableName;
			// on some databases the max is on a field which is decimal
			decimal maxEmployee = decimal.Parse(comm.ExecuteScalar().ToString()) - expectedRowsCount;

			comm.CommandText = "SELECT EmployeeID FROM " + tableName + " where EmployeeID > " +  maxEmployee.ToString() ;
			da.Fill(ds);

			Exception exp = null;
			try
			{
				BeginCase("Select by full table name in the same catalog");
				Compare(ds.Tables[0].Rows.Count ,expectedRowsCount );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{cleanTableAfterTest(conn,"Employees","EmployeeID",System.Convert.ToInt32(maxEmployee));
				EndCase(exp); exp = null;}

			#endregion //Select by full table name in the same catalog

		}

		#endregion

		#region Select by full table name in the different catalog
		public void doTestTypes2(OleDbConnection conn)
		{
			BeginCase("Select by full table name in the different catalog");
			nonUniqueId = "48951_" +  TestCaseNumber.ToString(); 
			Exception exp=null;
			string tableName = getDbObjectName("Customers",conn,"GHTDB_EX");
			int expectedRowsCount = 5;
			DataSet ds = new DataSet();
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;

			insertIntoStandatTable(conn,tableName,expectedRowsCount,"CustomerID");
		
			comm.CommandText = "SELECT * FROM " + tableName + " where CustomerID='" + nonUniqueId + "'" ;
			ds.Tables.Clear();
			da.Fill(ds);

			try
			{
				
				Compare(ds.Tables[0].Rows.Count ,expectedRowsCount );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;
			cleanStandatTable(conn,tableName,"CustomerID");
			}
		}
		#endregion

		#region Call stored procedure in the different catalog
		public void doTestTypes3(OleDbConnection conn)
		{
			BeginCase("Call stored procedure in the different catalog");
			nonUniqueId = "48951_" +  TestCaseNumber.ToString(); 
			Exception exp =null;
			DataSet ds = new DataSet();
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;

			string tableName = getDbObjectName("Customers",conn,"GHTDB_EX");
			int expectedRowsCount = 5;

			insertIntoStandatTable(conn,tableName,expectedRowsCount,"CustomerID");

			comm.CommandType = CommandType.StoredProcedure;
			comm.CommandText = getDbObjectName("GH_DUMMY",conn,"GHTDB_EX");
			switch (DataProvider.GetDbType(conn))
			{
				case DataBaseServer.SQLServer:
				case DataBaseServer.Sybase:
					comm.Parameters.Add("@CustomerID",nonUniqueId);
					break;
				default:
					comm.Parameters.Add(new OleDbParameter("CustomerIDPrm",OleDbType.Char));
					break;
			}
			
			comm.Parameters[0].Value = nonUniqueId;
			ds.Tables.Clear();
			

			try
			{
				da.Fill(ds);
				Compare(ds.Tables[0].Rows.Count ,expectedRowsCount );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;
			cleanStandatTable(conn,tableName,"CustomerID"); }
		}

#endregion // Call stored procedure in the different catalog

		#region Select using Table direct - single table
		public void doTestTypes4(OleDbConnection conn)
		{
			
			Exception exp =null;
			DataSet ds = new DataSet();
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;
			string tableName = getDbObjectName("Customers",conn);
			//int expectedRowsCount = 5;

			comm.CommandText = tableName;

			comm.CommandType = CommandType.TableDirect;

			ds.Tables.Clear();
			da.Fill(ds);

			try
			{
				BeginCase("Select using Table direct - single table");
				Compare(ds.Tables[0].Rows.Count > 0 ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

#endregion // Select using Table direct - single table

		#region Select using Table direct - multiple tables

		public void doTestTypes5(OleDbConnection conn)
		{

			Exception exp =null;
			DataSet ds = new DataSet();
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;
			//string tableName = getDbObjectName("Customers",conn);
			comm.CommandType = CommandType.TableDirect;
			comm.CommandText = "Categories;Employees";

			ds.Tables.Clear();
			da.Fill(ds);

			try
			{
				BeginCase("Select using Table direct - multiple tables");
				int result =  + ds.Tables[1].Rows.Count + ds.Tables[2].Rows.Count;
				Compare(ds.Tables[0].Rows.Count > 0  ,true );
				Compare(ds.Tables[1].Rows.Count > 0  ,true );
				Compare(ds.Tables[0].Rows.Count == ds.Tables[1].Rows.Count ,true );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}

#endregion // Select using Table direct - multiple tables


		#region Test view

		public void doTestTypes6(OleDbConnection conn)
		{
			Exception exp =null;
			DataSet ds = new DataSet();
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;
			//string tableName = getDbObjectName("Customers",conn);
			comm.CommandType = CommandType.Text;

			switch (DataProvider.GetDbType(conn))
			{
				case DataBaseServer.SQLServer:
				case DataBaseServer.Sybase:
					comm.CommandText = "select * from [Current Product List]";
					break;
				case DataBaseServer.Oracle:
				case DataBaseServer.PostgreSQL:
					comm.CommandText = "select * from Current_Product_List";
					break;
				default:
					comm.CommandText = "select * from DB2ADMIN.Current_Product_List";
					break;
			}


			ds.Tables.Clear();
			da.Fill(ds);

			try
			{
				BeginCase("Testing view");
				Compare(ds.Tables[0].Rows.Count >0,true);
				Compare(ds.Tables[0].Columns.Count,2);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}


		}
		#endregion

		#region select table with diffrent owner - diffrent name

		public void doTestTypes7(OleDbConnection conn)
		{
			Exception exp =null;
			DataSet ds = new DataSet();
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;
			//string tableName = getDbObjectName("Customers",conn);
			comm.CommandType = CommandType.Text;

			//First change ownerShip

			//chageOwnerShip(conn,"Categories","othertestuser");
			comm.CommandText = "SELECT * FROM othertestuser.CategoriesNew";
			da.Fill(ds);

			try
			{
				BeginCase("select table with diffrent owner - diffrent name");
				Compare(ds.Tables[0].Rows.Count ,2);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("select table with diffrent owner - diffrent name --> negetive");
				ds.Tables.Clear();
				comm.CommandText = "select * from " + getDbObjectName("CategoriesNew",conn);
				da.Fill(ds);
			}
			catch (OleDbException ex)
			{
				ExpectedExceptionCaught(ex);
			}
			catch 	{ExpectedExceptionNotCaught("OleDbException"); }
			finally	{EndCase(exp); exp = null;}

			//Change back

			//chageOwnerShip(conn,"othertestuser.Categories","dbo");
		}

		#endregion

		#region select table with diffrent owner - same name

		public void doTestTypes8(OleDbConnection conn)
		{
			Exception exp =null;
			DataSet ds = new DataSet();
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;
			//string tableName = getDbObjectName("Customers",conn);
			comm.CommandType = CommandType.Text;

			//First change ownerShip

			//chageOwnerShip(conn,"Categories","othertestuser");
			comm.CommandText = "SELECT * FROM othertestuser.Categories";
			da.Fill(ds);

			try
			{
				BeginCase("Select table with diffrent owner same name");
				Compare(ds.Tables[0].Rows.Count ,2);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

		}

		#endregion

		#region select table with diffrent owner - SP

		public void doTestTypes9(OleDbConnection conn)
		{
			Exception exp =null;
			DataSet ds = new DataSet();
			BeginCase("Select table with diffrent owner SP");
			nonUniqueId = "48951" ;
			int expectedRowsCount = 5;
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;
			string tableName = getDbObjectName("Employees",conn);
			comm.CommandType = CommandType.StoredProcedure;

			//insertIntoStandatTable(conn,tableName,5,"EmployeeID");
			string[] arr = new string[2];
			arr[0] = "LastName";
			arr[1] = "FirstName";

			int maxValue = prepareTableForTest(conn,expectedRowsCount,"Employees","EmployeeID",arr);

			switch (DataProvider.GetDbType(conn))
			{
				case DataBaseServer.SQLServer:
					comm.Parameters.Add("@EmployeeIdPrm",maxValue.ToString()); 
					break;
				case DataBaseServer.Sybase:
					comm.Parameters.Add("@EmployeeIdPrm",maxValue.ToString()); 
					break;
				default:
					comm.Parameters.Add("EmployeeIdPrm",maxValue.ToString()); 
					break;
			}

			
			try
			{
				comm.CommandText = "othertestuser.GH_DUMMY";
				da.Fill(ds);
				Compare(ds.Tables[0].Rows.Count ,expectedRowsCount);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;
			cleanTableAfterTest (conn,"Employees","EmployeeID",maxValue); }

		}

		#endregion

		#region select table with diffrent owner - and diffrent structure
		public void doTestTypes10(OleDbConnection conn)
		{
			Exception exp =null;
			DataSet ds = new DataSet();
			BeginCase("Select table with diffrent owner and diffrent structure");
			nonUniqueId = "48951" ;
			OleDbCommand comm = new OleDbCommand("",conn);
			OleDbDataAdapter da = new OleDbDataAdapter();
			da.SelectCommand = comm;
			string tableName = getDbObjectName("Categories",conn);
			comm.CommandType = CommandType.Text;
			OleDbDataReader reader = null;

			try
			{
				comm.CommandText = "select CategoryID,CategoryName from " + tableName;
				reader = comm.ExecuteReader();
				//da.Fill(ds);
			}
			catch (Exception  ex) 
			{
				EndCase(ex);
			}
			finally
			{
				reader.Close();
			}

			if (DataProvider.GetDbType(conn) == DataBaseServer.Oracle) 
			{
				comm.CommandText="select CategoryID,CategoryName  from  GHTDB_EX.Categories  where CategoryID = ?";
			}
			else
			{
				comm.CommandText="select * from othertestuser.Categories where CategoryID = ?";
			}
			comm.Parameters.Add("a",10);
			da.Fill(ds);

			try
			{
				Compare(ds.Tables[0].Rows.Count ,1);
			}
			catch(Exception ex)	{exp = ex;}
			finally	
			{
				EndCase(exp); exp = null;}

		}

		#endregion

		#region Oracle - use stored procedure inside package
		// [Test(Description="Call a stored procedure which is defined within a package.")]
		public void CallStoredProcedureInPackage(OleDbConnection con)
		{
			if (DataProvider.GetDbType(con) != DataBaseServer.Oracle)
			{
				//Packages exist only in oracle.
				return;
			}

			Exception exp = null;
			OleDbDataReader rdr = null;
			try
			{
				BeginCase("Call a stored procedure which is defined within a package.");
				exp = null;
				DataSet ds = new DataSet();
				OleDbDataAdapter da = new OleDbDataAdapter();
				OleDbCommand cmd = new OleDbCommand();
				cmd.Connection = con;
				cmd.CommandType = CommandType.StoredProcedure;
				cmd.CommandText = "ghtpkg.ghsp_inPkg";
				cmd.Parameters.Add("CustomerIdPrm", "ALFKI");
				da.SelectCommand = cmd;

				da.Fill(ds);
				Compare(ds.Tables[0].Rows.Count, 1);
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
			}
		}
		// [Test(Description="Call a stored procedure ghsp_pkgAmbig from a package, where ghsp_pkgAmbig is defined both inside and outside of a package.")]
		public void StoredProcedurePackageambiguity_InsidePackage(OleDbConnection con)
		{
			if (DataProvider.GetDbType(con) != DataBaseServer.Oracle)
			{
				//Packages exist only in oracle.
				return;
			}

			Exception exp = null;
			OleDbDataReader rdr = null;
			try
			{
				BeginCase("Call a stored procedure ghsp_pkgAmbig from a package, where ghsp_pkgAmbig is defined both inside and outside of a package.");
				exp = null;
				DataSet ds = new DataSet();
				OleDbDataAdapter da = new OleDbDataAdapter();
				OleDbCommand cmd = new OleDbCommand();
				cmd.Connection = con;
				cmd.CommandType = CommandType.StoredProcedure;
				cmd.CommandText = "ghtpkg.ghsp_pkgAmbig";
				da.SelectCommand = cmd;

				da.Fill(ds);
				Compare(ds.Tables[0].Rows[0]["IN_PKG"], "TRUE");
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
			}
		}
		// [Test(Description="Call a stored procedure ghsp_pkgAmbig not from a package, where ghsp_pkgAmbig is defined both inside and outside of a package.")]
		public void StoredProcedurePackageambiguity_OutsidePackage(OleDbConnection con)
		{
			if (DataProvider.GetDbType(con) != DataBaseServer.Oracle)
			{
				//Packages exist only in oracle.
				return;
			}

			Exception exp = null;
			OleDbDataReader rdr = null;
			try
			{
				BeginCase("Call a stored procedure ghsp_pkgAmbig not from a package, where ghsp_pkgAmbig is defined both inside and outside of a package.");
				exp = null;
				DataSet ds = new DataSet();
				OleDbDataAdapter da = new OleDbDataAdapter();
				OleDbCommand cmd = new OleDbCommand();
				cmd.Connection = con;
				cmd.CommandType = CommandType.StoredProcedure;
				cmd.CommandText = "ghsp_pkgAmbig";
				da.SelectCommand = cmd;

				da.Fill(ds);
				Compare(ds.Tables[0].Rows[0]["IN_PKG"], "FALSE");
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
			}
		}
		#endregion

		private string getDbObjectName(string objectName,OleDbConnection con)
		{
			return getDbObjectName(objectName,con,string.Empty);
			
		}
		private string getDbObjectName(string objectName,OleDbConnection con,string databaseName)
		{
			switch (DataProvider.GetDbType(con))
			{
				case DataBaseServer.SQLServer:
				case DataBaseServer.Sybase:
				{
					if (databaseName == string.Empty)
					{
						return "GHTDB.dbo." + objectName;
					}
					else
					{
						return  databaseName +  ".dbo." + objectName;
					}
				}
				case DataBaseServer.PostgreSQL:
				{
					return "public." + objectName.ToUpper();
				}
				case DataBaseServer.Oracle:
				{
					if (databaseName == string.Empty)
					{
						return "GHTDB." + objectName.ToUpper();
					}
					else
					{
						return  databaseName.ToUpper() + "." + objectName.ToUpper();
					}
					
				}
				case DataBaseServer.DB2: 
				{
					if (databaseName == string.Empty)
					{
						return   "DB2ADMIN." + objectName;
					}
					else
					{
						return  databaseName +  ".DB2ADMIN." + objectName;
					}

					

				}
				default:
				{
					throw new NotImplementedException();
				}


			}

		}



		/// <summary>
		/// This method will prepare table for test
		/// </summary>
		private int prepareTableForTest(OleDbConnection con,int recordsNumber,string baseTableName,string keyField
			,params string[] otherNonNullableFieldsName)
		{
			string tableName = getDbObjectName(baseTableName,con);
			OleDbCommand cmd = new OleDbCommand("select max(" + keyField + ") from " + tableName,con);
			string str_ret = cmd.ExecuteScalar().ToString();
			System.Console.WriteLine("ExecuteScalar:" + str_ret);
			// on some databases the max is on a field which is decimal
			decimal maxRecord = decimal.Parse(str_ret);
			int resultCount = System.Convert.ToInt32(maxRecord)+recordsNumber;
			string sqlStmt = string.Empty;
			string valueStmt = string.Empty;

			//Constrcut the statemnet once : --> TODO://Move this logic to seperate method
			for(int i=0;i<otherNonNullableFieldsName.Length;i++)
			{
				sqlStmt+= otherNonNullableFieldsName[i] + ",";
				valueStmt+="'a',";
			}

			//Trim the last ","
			if (otherNonNullableFieldsName.Length > 0)
			{
				sqlStmt =  sqlStmt.Remove(sqlStmt.Length-1,1);
				sqlStmt = "," + sqlStmt;

				valueStmt =  valueStmt.Remove(valueStmt.Length-1,1);
				valueStmt = "," + valueStmt;
			}
				

			for (int index=System.Convert.ToInt32(maxRecord)+1;index<=resultCount;index++)
			{
				cmd.CommandText="Insert into " + tableName + " (" + keyField + sqlStmt + ") values ("
					+ index + valueStmt + ")";
				cmd.ExecuteNonQuery();
			}
			return System.Convert.ToInt32(maxRecord);

		}
		private void cleanTableAfterTest(OleDbConnection con,string baseTableName, string keyField, int recordNumber)
		{
			string tableName = getDbObjectName(baseTableName, con);
			OleDbCommand cmd = new OleDbCommand("delete from " + tableName + " where " + keyField + " > " + recordNumber  ,con);
			cmd.ExecuteNonQuery();

		}

		private void insertIntoStandatTable(OleDbConnection con,string tableName,int recordsNumber,string keyField)
		{
			OleDbCommand cmd = new OleDbCommand("delete from " + tableName + " where  "  + keyField + "= '" + nonUniqueId + "'",con);
			cmd.ExecuteNonQuery();

			for (int index=0;index<recordsNumber;index++)
			{
				cmd.CommandText = "Insert into " + tableName + "(" + keyField + ") values ('" + nonUniqueId  + "')";
				cmd.ExecuteNonQuery();
			}
		}
		private void cleanStandatTable(OleDbConnection con,string tableName,string keyField)
		{
			OleDbCommand cmd = new OleDbCommand("delete from " + tableName + " where " + keyField + " = '" + nonUniqueId + "'",con);
			cmd.ExecuteNonQuery();

		}

		private void chageOwnerShip(OleDbConnection con,string objectName,string newOwner)
		{
			OleDbCommand cmd = new OleDbCommand();
			cmd.Connection = con;

			switch (DataProvider.GetDbType(con))
			{
				case DataBaseServer.SQLServer:
				case DataBaseServer.Sybase:
				{
					cmd.CommandType = CommandType.StoredProcedure;
					cmd.CommandText = "[dbo].[sp_changeobjectowner]";
					cmd.Parameters.Add("@objname",objectName);
					cmd.Parameters.Add("@newowner",newOwner);
					cmd.ExecuteNonQuery();
					return;

				}
				default:
				{
					throw new NotImplementedException();
				}
			}

		}
	}
}
