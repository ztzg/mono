using NUnit.Framework;
using System;
using System.Data;
using System.Data.SqlClient;

using System.Xml;
using System.Text;
using System.IO;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data.System_Data_SqlClient
{
	[TestFixture] public class SqlCommand_ExecuteXmlReader_ : ADONetTesterClass
	{
		[Test] public void Main()
		{
			SqlCommand_ExecuteXmlReader_ tc = new SqlCommand_ExecuteXmlReader_();
			Exception exp = null;
			try
			{
				// Every Test must begin with BeginTest
				tc.BeginTest("SqlCommand_ExecuteXmlReader");

				//testing only on SQLServer
				if (DataProvider.GetDbType() != DataBaseServer.SQLServer) return ; 

				tc.run();
			}
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				// Every Test must End with EndTest
				tc.EndTest(exp);
			}
		}

		public void run()
		{
			Exception exp = null;

			// Start Sub Test
			try
			{
				// Every Sub Test must begin with BeginCase
				BeginCase("ExecuteXmlReader 1");

				SqlConnection con = new SqlConnection(GHTUtils.DataProvider.ConnectionStringSQLClient);

				con.Open();
				string selectStr =	"SELECT * FROM Products WHERE PRODUCTID=1 FOR XML AUTO, XMLDATA;" + 
					"SELECT * FROM Orders WHERE ORDERID=1 FOR XML AUTO, XMLDATA;" + 
					"SELECT * FROM Customers WHERE CustomerID like 'A%' FOR XML AUTO, XMLDATA";
			
				SqlCommand comm = new SqlCommand(selectStr,con);
				XmlReader xr = comm.ExecuteXmlReader();

				StringBuilder sb = new StringBuilder();
				while(xr.Read()) 
				{
					sb.Append(xr.ReadOuterXml());
				}
				// Every Sub Test must have a Compare
				string strXml = null;
				Compare(sb.ToString().Length,4391);
			} 
			catch(Exception ex)
			{
				exp = ex;
			}
			finally
			{
				// Every Sub Test must end with EndCase
				EndCase(exp);
				exp = null;
			}
			// End Sub Test
		}


	}
}
