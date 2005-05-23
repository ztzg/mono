using NUnit.Framework;
using System;
using System.Data;
using System.Data.OleDb;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_OleDb
{
[TestFixture] public class OleDbParameter_ctor_SOtype : GHTBase
{
	private Exception exp;
	[Test] public void Main()
	{
		OleDbParameter_ctor_SOtype tc = new OleDbParameter_ctor_SOtype();
		tc.exp = null;
		try
		{
			tc.BeginTest("OleDbParameter_ctor_SOtype on " + DataProvider.GetDbType().ToString());
			tc.run();
		}
		catch(Exception ex){tc.exp = ex;}
		finally	{tc.EndTest(tc.exp);}
	}

	public void run()
	{
		// Log(string.Format("DB Server={0}.", DataProvider.GetDbType()));
		AllTypes();
		SimpleTypesWithDBNull();
	}

	public void AllTypes()
	{
		exp = null;
		OleDbParameter param = null;

		foreach (OleDbType dbtype in Enum.GetValues(typeof(OleDbType)))
		{
			// not supporting OleDbType.IDispatch and OleDbType.IUnknown
			if (( dbtype == OleDbType.IDispatch) || (dbtype == OleDbType.IUnknown) )
			{
				continue;
			}

			param = new OleDbParameter("myParam",dbtype);

			try
			{
				BeginCase("ctor " + dbtype.ToString());
				Compare(param != null, true);
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

			try
			{
				BeginCase("name " + dbtype.ToString());
				Compare(param.ParameterName ,"myParam");
			} 
			catch(Exception ex){exp = ex;}
			finally{EndCase(exp); exp = null;}

		}
	}

	public void SimpleTypesWithDBNull()
	{
		OleDbConnection con=null;
		OleDbCommand cmd=null;
		OleDbDataReader rdr=null;
		try
		{
			exp = null;
			BeginCase("Test simple types with DBNull");

			string connectionString = DataProvider.ConnectionString;
			con = new	OleDbConnection(connectionString);
			cmd = new OleDbCommand();
			con.Open();
			cmd.Connection = con;

			cmd.CommandText = "GHSP_TYPES_SIMPLE_1";
			cmd.CommandType = CommandType.StoredProcedure;

			AddSimpleTypesNullParams(cmd);

			rdr = cmd.ExecuteReader();
			rdr.Read();
			for (int i=0; i<rdr.FieldCount; i++)
			{
				switch (DataProvider.GetDbType())
				{
					//PostgreSQL returns DBNull as empty String
					case DataBaseServer.PostgreSQL:
						Compare("<unnamed portal " + (i+1).ToString() + ">", rdr.GetValue(i));
						break;
					default:
						Compare(DBNull.Value, rdr.GetValue(i));
						break;
				}
				
			}
			rdr.Close();
		}
		catch (Exception ex)
		{
			exp = ex;
		}
		finally
		{
			EndCase(exp);
			if(rdr != null && !rdr.IsClosed)
			{
				rdr.Close();
			}
			if (con != null && con.State == ConnectionState.Open)
			{
				con.Close();
			}
			exp=null;
		}
	}

	private void AddSimpleTypesNullParams(OleDbCommand cmd)
	{
		OleDbParameter tmpParam;

		switch (DataProvider.GetDbType())
		{
			case DataBaseServer.SQLServer:
				tmpParam = new OleDbParameter("bit", OleDbType.Boolean);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("tinyint", OleDbType.TinyInt);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("smallint", OleDbType.SmallInt);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("integer", OleDbType.Integer);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("bigint", OleDbType.BigInt);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("decimal", OleDbType.Numeric);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("numeric", OleDbType.Numeric);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("float", OleDbType.Double);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("real", OleDbType.Single);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("char", OleDbType.Char);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("nchar", OleDbType.WChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("varchar", OleDbType.VarChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("nvarchar", OleDbType.VarWChar);
				cmd.Parameters.Add(tmpParam);
				break;
			case DataBaseServer.Sybase:
				tmpParam = new OleDbParameter("tinyint", OleDbType.TinyInt);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("smallint", OleDbType.SmallInt);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("integer", OleDbType.Integer);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("decimal", OleDbType.Numeric);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("numeric", OleDbType.Numeric);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("float", OleDbType.Double);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("real", OleDbType.Single);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("char", OleDbType.Char);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("nchar", OleDbType.WChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("varchar", OleDbType.VarChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("nvarchar", OleDbType.VarWChar);
				cmd.Parameters.Add(tmpParam);
				break;
			case DataBaseServer.Oracle:
				tmpParam = new OleDbParameter("NUMBER", OleDbType.Numeric);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("LONG", OleDbType.LongVarChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("FLOAT", OleDbType.Single);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("VARCHAR", OleDbType.VarChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("NVARCHAR", OleDbType.VarWChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("CHAR", OleDbType.Char);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("NCHAR", OleDbType.WChar);
				cmd.Parameters.Add(tmpParam);
				break;
			case DataBaseServer.PostgreSQL:
				tmpParam = new OleDbParameter("NUMERIC", OleDbType.Numeric);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("int2", OleDbType.Numeric);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("int4", OleDbType.LongVarChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("FLOAT", OleDbType.Single);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("VARCHAR", OleDbType.VarWChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("CHAR", OleDbType.Char);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("NCHAR", OleDbType.WChar);
				cmd.Parameters.Add(tmpParam);
				break;
			case DataBaseServer.DB2:
				tmpParam = new OleDbParameter("SMALLINT", OleDbType.SmallInt);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("INTEGER", OleDbType.Integer);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("BIGINT", OleDbType.BigInt);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("DECIMAL", OleDbType.Decimal);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("REAL", OleDbType.Double);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("DOUBLE", OleDbType.Double);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("CHARACTER", OleDbType.Char);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("VARCHAR", OleDbType.VarChar);
				cmd.Parameters.Add(tmpParam);
				tmpParam = new OleDbParameter("LONGVARCHAR", OleDbType.LongVarChar);
				cmd.Parameters.Add(tmpParam);
				break;
			default:
				this.Fail("Unknown DataBaseServer type.");
				break;
		}

		foreach (OleDbParameter current in cmd.Parameters)
		{
			current.Value = DBNull.Value;
		}
	}
}
}
