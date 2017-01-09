//
// Mono.Data.Tds.Protocol.Tds70.cs
//
// Author:
//   Tim Coleman (tim@timcoleman.com)
//   Diego Caravana (diego@toth.it)
//   Sebastien Pouliot (sebastien@ximian.com)
//   Daniel Morgan (danielmorgan@verizon.net)
//
// Copyright (C) 2002 Tim Coleman
// Portions (C) 2003 Motus Technologies Inc. (http://www.motus.com)
// Portions (C) 2003 Daniel Morgan
//

//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

using System;
using System.Globalization;
using System.Text;

using Mono.Security.Protocol.Ntlm;

namespace Mono.Data.Tds.Protocol
{
	public sealed class Tds70 : Tds
	{
		#region Fields

		public readonly static TdsVersion Version = TdsVersion.tds70;
		static readonly decimal SMALLMONEY_MIN = -214748.3648m;
		static readonly decimal SMALLMONEY_MAX = 214748.3647m;

		#endregion // Fields

		#region Constructors

		public Tds70 (string server, int port)
			: this (server, port, 512, 15)
		{
		}

		public Tds70 (string server, int port, int packetSize, int timeout)
			: base (server, port, packetSize, timeout, Version)
		{
		}

		#endregion // Constructors

		#region Methods

		private string BuildExec (string sql)
		{
			string esql = sql.Replace ("'", "''"); // escape single quote
			if (Parameters != null && Parameters.Count > 0)
				return BuildProcedureCall (String.Format ("sp_executesql N'{0}', N'{1}', ", esql, BuildPreparedParameters ()));
			else
				return BuildProcedureCall (String.Format ("sp_executesql N'{0}'", esql));
		}

		private string BuildParameters ()
		{
			if (Parameters == null || Parameters.Count == 0)
				return String.Empty;

			StringBuilder result = new StringBuilder ();
			foreach (TdsMetaParameter p in Parameters) {
				string parameterName = p.ParameterName;
				if (parameterName [0] == '@') {
					parameterName = parameterName.Substring (1);
				}
				if (p.Direction != TdsParameterDirection.ReturnValue) {
					if (result.Length > 0)
						result.Append (", ");
					if (p.Direction == TdsParameterDirection.InputOutput)
						result.AppendFormat ("@{0}={0} output", parameterName);
					else
						result.Append (FormatParameter (p));
				}
			}
			return result.ToString ();
		}

		private string BuildPreparedParameters ()
		{
			StringBuilder parms = new StringBuilder ();
			foreach (TdsMetaParameter p in Parameters) {
				if (parms.Length > 0)
					parms.Append (", ");
				parms.Append (p.Prepare ());
				if (p.Direction == TdsParameterDirection.Output)
					parms.Append (" output");
			}
			return parms.ToString ();
		}

		private string BuildPreparedQuery (string id)
		{
			return BuildProcedureCall (String.Format ("sp_execute {0},", id));
		}

		private string BuildProcedureCall (string procedure)
		{
			string exec = String.Empty;

			StringBuilder declare = new StringBuilder ();
			StringBuilder select = new StringBuilder ();
			StringBuilder set = new StringBuilder ();
			
			int count = 0;
			if (Parameters != null) {
				foreach (TdsMetaParameter p in Parameters) {
					if (p.Direction != TdsParameterDirection.Input) {
						if (count == 0)
							select.Append ("select ");
						else
							select.Append (", ");
						select.Append (p.ParameterName);
							
						declare.Append (String.Format ("declare {0}\n", p.Prepare ()));

						if (p.Direction != TdsParameterDirection.ReturnValue) {
							if (p.Direction == TdsParameterDirection.InputOutput)
								set.Append (String.Format ("set {0}\n", FormatParameter(p)));
							else
								set.Append (String.Format ("set {0}=NULL\n", p.ParameterName));
						}
					
						count += 1;
					}
					
					if (p.Direction == TdsParameterDirection.ReturnValue)
						exec = p.ParameterName + "=";
				}
			}
			exec = "exec " + exec;

			return String.Format ("{0}{1}{2}{3} {4}\n{5}",
				declare.ToString (), set.ToString (), exec,
				procedure, BuildParameters (), select.ToString ());
		}

		public override bool Connect (TdsConnectionParameters connectionParameters)
		{
			if (IsConnected)
				throw new InvalidOperationException ("The connection is already open.");
	
			connectionParms = connectionParameters;

			SetLanguage (connectionParameters.Language);
			SetCharset ("utf-8");
		
			byte[] empty = new byte[0];
			short authLen = 0;
			byte pad = (byte) 0;
			
			byte[] domainMagic = { 6, 0x7d, 0x0f, 0xfd, 0xff, 0x0, 0x0, 0x0,
									0x0, 0xe0, 0x83, 0x0, 0x0,
									0x68, 0x01, 0x00, 0x00, 0x09, 0x04, 0x00, 0x00 };
			byte[] sqlserverMagic = { 6, 0x0, 0x0, 0x0,
										0x0, 0x0, 0x0, 0x0,
										0x0, 0xe0, 0x03, 0x0,
										0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
										0x0, 0x0, 0x0 };
			byte[] magic = null;
			
			if (connectionParameters.DomainLogin)
				magic = domainMagic;
			else
				magic = sqlserverMagic;
			
			string username = connectionParameters.User;
			string domain = null;

			int idx = username.IndexOf ("\\");
			if (idx != -1) {
				domain = username.Substring (0, idx);
				username = username.Substring (idx + 1);

				connectionParameters.DefaultDomain = domain;
				connectionParameters.User = username;
			} else {
				domain = Environment.UserDomainName;
				connectionParameters.DefaultDomain = domain;
			}

			short partialPacketSize = (short) (86 + (
				connectionParameters.Hostname.Length +
				connectionParameters.ApplicationName.Length +
				DataSource.Length +
				connectionParameters.LibraryName.Length +
				Language.Length +
				connectionParameters.Database.Length +
				connectionParameters.AttachDBFileName.Length) * 2);

			if (connectionParameters.DomainLogin) {
				authLen = ((short) (32 + (connectionParameters.Hostname.Length +
					domain.Length)));
				partialPacketSize += authLen;
			} else
				partialPacketSize += ((short) ((username.Length + connectionParameters.Password.Length) * 2));
			
			int totalPacketSize = partialPacketSize;
			
			Comm.StartPacket (TdsPacketType.Logon70);
			
			Comm.Append (totalPacketSize);

			//Comm.Append (empty, 3, pad);
			byte[] version = {0x00, 0x0, 0x0, 0x70};
			Comm.Append (version); // TDS Version 7
			Comm.Append ((int)this.PacketSize); // Set the Block Size
			Comm.Append (empty, 3, pad);
			Comm.Append (magic);

			short curPos = 86;

			// Hostname
			Comm.Append (curPos);
			Comm.Append ((short) connectionParameters.Hostname.Length);
			curPos += (short) (connectionParameters.Hostname.Length * 2);

			if (connectionParameters.DomainLogin) {
				Comm.Append((short)0);
				Comm.Append((short)0);
				Comm.Append((short)0);
				Comm.Append((short)0);
			} else {
				// Username
				Comm.Append (curPos);
				Comm.Append ((short) username.Length);
				curPos += ((short) (username.Length * 2));

				// Password
				Comm.Append (curPos);
				Comm.Append ((short) connectionParameters.Password.Length);
				curPos += (short) (connectionParameters.Password.Length * 2);
			}

			// AppName
			Comm.Append (curPos);
			Comm.Append ((short) connectionParameters.ApplicationName.Length);
			curPos += (short) (connectionParameters.ApplicationName.Length * 2);

			// Server Name
			Comm.Append (curPos);
			Comm.Append ((short) DataSource.Length);
			curPos += (short) (DataSource.Length * 2);

			// Unknown
			Comm.Append ((short) curPos);
			Comm.Append ((short) 0);

			// Library Name
			Comm.Append (curPos);
			Comm.Append ((short) connectionParameters.LibraryName.Length);
			curPos += (short) (connectionParameters.LibraryName.Length * 2);

			// Language
			Comm.Append (curPos);
			Comm.Append ((short) Language.Length);
			curPos += (short) (Language.Length * 2);

			// Database
			Comm.Append (curPos);
			Comm.Append ((short) connectionParameters.Database.Length);
			curPos += (short) (connectionParameters.Database.Length * 2);

			// MAC Address
			Comm.Append((byte) 0);
			Comm.Append((byte) 0);
			Comm.Append((byte) 0);
			Comm.Append((byte) 0);
			Comm.Append((byte) 0);
			Comm.Append((byte) 0);

			// Authentication Stuff
			Comm.Append ((short) curPos);
			if (connectionParameters.DomainLogin) {
				Comm.Append ((short) authLen);
				curPos += (short) authLen;
			} else
				Comm.Append ((short) 0);
			
			// Unknown
			Comm.Append (curPos);
			Comm.Append ((short)( connectionParameters.AttachDBFileName.Length));
			curPos += (short)(connectionParameters.AttachDBFileName.Length*2);
			
			// Connection Parameters
			Comm.Append (connectionParameters.Hostname);
			if (!connectionParameters.DomainLogin) {
				// SQL Server Authentication
				Comm.Append (connectionParameters.User);
				string scrambledPwd = EncryptPassword (connectionParameters.Password);
				Comm.Append (scrambledPwd);
			}
			Comm.Append (connectionParameters.ApplicationName);
			Comm.Append (DataSource);
			Comm.Append (connectionParameters.LibraryName);
			Comm.Append (Language);
			Comm.Append (connectionParameters.Database);

			if (connectionParameters.DomainLogin) {
				// the rest of the packet is NTLMSSP authentication
				Type1Message msg = new Type1Message ();
				msg.Domain = domain;
				msg.Host = connectionParameters.Hostname;
				msg.Flags = NtlmFlags.NegotiateUnicode |
					NtlmFlags.NegotiateNtlm |
					NtlmFlags.NegotiateDomainSupplied |
					NtlmFlags.NegotiateWorkstationSupplied |
					NtlmFlags.NegotiateAlwaysSign; // 0xb201
				Comm.Append (msg.GetBytes ());
			}

			Comm.Append (connectionParameters.AttachDBFileName);
			Comm.SendPacket ();
			MoreResults = true;
			SkipToEnd ();
			
			return IsConnected;
		}

		private static string EncryptPassword (string pass)
		{
			int xormask = 0x5a5a;
			int len = pass.Length;
			char[] chars = new char[len];

			for (int i = 0; i < len; ++i) {
				int c = ((int) (pass[i])) ^ xormask;
				int m1 = (c >> 4) & 0x0f0f;
				int m2 = (c << 4) & 0xf0f0;
				chars[i] = (char) (m1 | m2);
			}

			return new String (chars);
		}

		public override bool Reset ()
		{
			// Check validity of the connection - a false removes
			// the connection from the pool
			// NOTE: MS implementation will throw a connection-reset error as it will
			// try to use the same connection
			if (!Comm.IsConnected ())
				return false;

			// Set "reset-connection" bit for the next message packet
			Comm.ResetConnection = true;

			return true;
		}

		public override void ExecPrepared (string commandText, TdsMetaParameterCollection parameters, int timeout, bool wantResults)
		{
			Parameters = parameters;
			ExecuteQuery (BuildPreparedQuery (commandText), timeout, wantResults);
		}
			
		public override void ExecProc (string commandText, TdsMetaParameterCollection parameters, int timeout, bool wantResults)
		{
			Parameters = parameters;
			ExecRPC (commandText, parameters, timeout, wantResults);
		}

		protected override void ExecRPC (string rpcName, TdsMetaParameterCollection parameters, 
						 int timeout, bool wantResults)
		{
			// clean up
			InitExec ();
			Comm.StartPacket (TdsPacketType.RPC);

			Comm.Append ( (short) rpcName.Length);
			Comm.Append (rpcName);
			Comm.Append ( (short) 0); //no meta data
			if (parameters != null) {
				foreach (TdsMetaParameter param in parameters) {
					if (param.Direction == TdsParameterDirection.ReturnValue) 
						continue;
					string pname = param.ParameterName;
					if (pname != null && pname.Length > 0 && pname [0] == '@') {
						Comm.Append ( (byte) pname.Length);
						Comm.Append (pname);
					} else {
						Comm.Append ( (byte) (pname.Length + 1));
						Comm.Append ("@" + pname);
					}
					short status = 0; // unused
					if (param.Direction != TdsParameterDirection.Input)
						status |= 0x01; // output
					Comm.Append ( (byte) status);
					WriteParameterInfo (param);
				}
			}
			Comm.SendPacket ();
			CheckForData (timeout);
			if (!wantResults)
				SkipToEnd ();
		}

		private void WriteParameterInfo (TdsMetaParameter param)
		{
			/*
			Ms.net send non-nullable datatypes as nullable and allows setting null values
			to int/float etc.. So, using Nullable form of type for all data
			*/
			param.IsNullable = true;
			TdsColumnType colType = param.GetMetaType ();
			param.IsNullable = false;

			Comm.Append ((byte)colType); // type

			int size = param.Size;
			if (size == 0)
				size = param.GetActualSize ();

			/*
			  If column type is SqlDbType.NVarChar the size of parameter is multiplied by 2
			  FIXME: Need to check for other types
			 */
			if (colType == TdsColumnType.BigNVarChar)
				size <<= 1;
			if (IsLargeType (colType))
				Comm.Append ((short)size); // Parameter size passed in SqlParameter
			else if (IsBlobType (colType))
				Comm.Append (size); // Parameter size passed in SqlParameter
			else
				Comm.Append ((byte)size);

			// Precision and Scale are non-zero for only decimal/numeric
			if ( param.TypeName == "decimal" || param.TypeName == "numeric") {
				Comm.Append ((param.Precision !=0 ) ? param.Precision : (byte) 28);
				Comm.Append (param.Scale);
			}

			size = param.GetActualSize ();
			if (IsLargeType (colType))
				Comm.Append ((short)size);
			else if (IsBlobType (colType))
				Comm.Append (size);
			else
				Comm.Append ((byte)size);

			if (size > 0) {
				switch (param.TypeName) {
				case "money" : {
					Decimal val = (decimal) param.Value;
					int[] arr = Decimal.GetBits (val);

					if (val >= 0) {
						Comm.Append (arr[1]);
						Comm.Append (arr[0]);
					} else {
						Comm.Append (~arr[1]);
						Comm.Append (~arr[0] + 1);
					}
					break;
				}
				case "smallmoney": {
					Decimal val = (decimal) param.Value;
					if (val < SMALLMONEY_MIN || val > SMALLMONEY_MAX)
						throw new OverflowException (string.Format (
							CultureInfo.InvariantCulture,
							"Value '{0}' is not valid for SmallMoney."
							+ "  Must be between {1:N4} and {2:N4}.",
#if NET_2_0
							val,
#else
							val.ToString (CultureInfo.CurrentCulture),
#endif
							SMALLMONEY_MIN, SMALLMONEY_MAX));

					int[] arr = Decimal.GetBits (val);
					int sign = (val>0 ? 1: -1);
					Comm.Append (sign * arr[0]);
					break;
				}
				case "datetime":
					Comm.Append ((DateTime)param.Value, 8);
					break;
				case "smalldatetime":
					Comm.Append ((DateTime)param.Value, 4);
					break;
				case "varchar" :
				case "nvarchar" :
				case "char" :
				case "nchar" :
				case "text" :
				case "ntext" :
					byte [] tmp = param.GetBytes ();
					Comm.Append (tmp);
					break;
				case "uniqueidentifier" :
					Comm.Append (((Guid)param.Value).ToByteArray());
					break;
				default :
					Comm.Append (param.Value);
					break;
				}
			}
			return;
		}

		public override void Execute (string commandText, TdsMetaParameterCollection parameters, int timeout, bool wantResults)
		{
			Parameters = parameters;
			string sql = commandText;
			if (wantResults || (Parameters != null && Parameters.Count > 0))
				sql = BuildExec (commandText);
			ExecuteQuery (sql, timeout, wantResults);
		}

		private string FormatParameter (TdsMetaParameter parameter)
		{
			string parameterName = parameter.ParameterName;
			if (parameterName [0] == '@') {
				parameterName = parameterName.Substring (1);
			}
			if (parameter.Direction == TdsParameterDirection.Output)
				return String.Format ("@{0}={0} output", parameterName);
			if (parameter.Value == null || parameter.Value == DBNull.Value)
				return parameter.ParameterName + "=NULL";

			string value = null;
			switch (parameter.TypeName) {
			case "smalldatetime":
			case "datetime":
				DateTime d = Convert.ToDateTime (parameter.Value);
				value = String.Format (base.Locale,
					"'{0:MMM dd yyyy hh:mm:ss.fff tt}'", d);
				break;
			case "bigint":
			case "decimal":
			case "float":
			case "int":
			case "money":
			case "real":
			case "smallint":
			case "smallmoney":
			case "tinyint":
				object paramValue = parameter.Value;
				Type paramType = paramValue.GetType ();
				if (paramType.IsEnum)
					paramValue = Convert.ChangeType (paramValue,
						Type.GetTypeCode (paramType));
				value = paramValue.ToString ();
				break;
			case "nvarchar":
			case "nchar":
				value = String.Format ("N'{0}'", parameter.Value.ToString ().Replace ("'", "''"));
				break;
			case "uniqueidentifier":
				value = String.Format ("'{0}'", ((Guid) parameter.Value).ToString (string.Empty));
				break;
			case "bit":
				if (parameter.Value.GetType () == typeof (bool))
					value = (((bool) parameter.Value) ? "0x1" : "0x0");
				else
					value = parameter.Value.ToString ();
				break;
			case "image":
			case "binary":
			case "varbinary":
				byte[] byteArray = (byte[]) parameter.Value;
				// In 1.0 profile, BitConverter.ToString() throws ArgumentOutOfRangeException when passed a 0-length
				// array, so handle that as a special case.
				if (byteArray.Length == 0)
					value = "0x";
				else
					value = String.Format ("0x{0}", BitConverter.ToString (byteArray).Replace ("-", string.Empty).ToLower ());
				break;
			default:
				value = String.Format ("'{0}'", parameter.Value.ToString ().Replace ("'", "''"));
				break;
			}

			return "@" + parameterName + "=" + value;
		}

		public override string Prepare (string commandText, TdsMetaParameterCollection parameters)
		{
			Parameters = parameters;

			TdsMetaParameterCollection parms = new TdsMetaParameterCollection ();
			TdsMetaParameter parm = new TdsMetaParameter ("@Handle", "int", null);
			parm.Direction = TdsParameterDirection.Output;
			parms.Add (parm);

			parms.Add (new TdsMetaParameter ("@VarDecl", "nvarchar", BuildPreparedParameters ()));
			parms.Add (new TdsMetaParameter ("@Query", "nvarchar", commandText));

			ExecProc ("sp_prepare", parms, 0, true);
			SkipToEnd ();
			return OutputParameters[0].ToString () ;
			//if (ColumnValues == null || ColumnValues [0] == null || ColumnValues [0] == DBNull.Value)
			//	throw new TdsInternalException ();
			//return string.Empty;
			//return ColumnValues [0].ToString ();
		}

		protected override TdsDataColumnCollection ProcessColumnInfo ()
		{
			TdsDataColumnCollection result = new TdsDataColumnCollection ();
			int numColumns = Comm.GetTdsShort ();
			for (int i = 0; i < numColumns; i += 1) {
				byte[] flagData = new byte[4];
				for (int j = 0; j < 4; j += 1) 
					flagData[j] = Comm.GetByte ();

				bool nullable = (flagData[2] & 0x01) > 0;
				//bool caseSensitive = (flagData[2] & 0x02) > 0;
				bool writable = (flagData[2] & 0x0c) > 0;
				bool autoIncrement = (flagData[2] & 0x10) > 0;
				bool isIdentity = (flagData[2] & 0x10) > 0;

				TdsColumnType columnType = (TdsColumnType) (Comm.GetByte () & 0xff);
				if ((byte) columnType == 0xef)
					columnType = TdsColumnType.NChar;

				byte xColumnType = 0;
				if (IsLargeType (columnType)) {
					xColumnType = (byte) columnType;
					if (columnType != TdsColumnType.NChar)
						columnType -= 128;
				}

				int columnSize;
				string tableName = null;

				if (IsBlobType (columnType)) {
					columnSize = Comm.GetTdsInt ();
					tableName = Comm.GetString (Comm.GetTdsShort ());
				} else if (IsFixedSizeColumn (columnType))
					columnSize = LookupBufferSize (columnType);
				else if (IsLargeType ((TdsColumnType) xColumnType))
					columnSize = Comm.GetTdsShort ();
				else
					columnSize = Comm.GetByte () & 0xff;

				byte precision = 0;
				byte scale = 0;

				switch (columnType) {
				case TdsColumnType.NText:
				case TdsColumnType.NChar:
				case TdsColumnType.NVarChar:
					columnSize /= 2;
					break;
				case TdsColumnType.Decimal:
				case TdsColumnType.Numeric:
				  /*
					Comm.Skip (1);
				  */
					precision = Comm.GetByte ();
					scale = Comm.GetByte ();
					break;
				}

				string columnName = Comm.GetString (Comm.GetByte ());

				TdsDataColumn col = new TdsDataColumn ();
				result.Add (col);
#if NET_2_0
				col.ColumnType = columnType;
				col.ColumnName = columnName;
				col.IsAutoIncrement = autoIncrement;
				col.IsIdentity = isIdentity;
				col.ColumnSize = columnSize;
				col.NumericPrecision = precision;
				col.NumericScale = scale;
				col.IsReadOnly = !writable;
				col.AllowDBNull = nullable;
				col.BaseTableName = tableName;
#else
				col ["ColumnType"] = columnType;
				col ["ColumnName"] = columnName;
				col ["IsAutoIncrement"] = autoIncrement;
				col ["IsIdentity"] = isIdentity;
				col ["ColumnSize"] = columnSize;
				col ["NumericPrecision"] = precision;
				col ["NumericScale"] = scale;
				col ["IsReadOnly"] = !writable;
				col ["AllowDBNull"] = nullable;
				col ["BaseTableName"] = tableName;
#endif
			}
			return result;
		}

		public override void Unprepare (string statementId)
		{
			TdsMetaParameterCollection parms = new TdsMetaParameterCollection ();
			parms.Add (new TdsMetaParameter ("@P1", "int", Int32.Parse (statementId)));
			ExecProc ("sp_unprepare", parms, 0, false);
		}
		
		protected override bool IsValidRowCount (byte status, byte op)
		{
			if ((status & (byte)0x10) == 0 || op == (byte)0xc1)
				return false;
			return true; 
		}

		protected override void ProcessReturnStatus ()
		{
			int result = Comm.GetTdsInt ();
			if (Parameters != null) {
				foreach (TdsMetaParameter param in Parameters) {
					if (param.Direction == TdsParameterDirection.ReturnValue) {
						param.Value = result;
						break;
					}
				}
			}
		}

		#endregion // Methods

#if NET_2_0
		#region Asynchronous Methods

		public override IAsyncResult BeginExecuteNonQuery (string cmdText,
                                                          TdsMetaParameterCollection parameters,
                                                          AsyncCallback callback,
                                                          object state)
		{
			Parameters = parameters;
			string sql = cmdText;
			if (Parameters != null && Parameters.Count > 0)
				sql = BuildExec (cmdText);

			IAsyncResult ar = BeginExecuteQueryInternal (sql, false, callback, state);
			return ar;
		}

		public override void EndExecuteNonQuery (IAsyncResult ar)
		{
			EndExecuteQueryInternal (ar);
		}

		public override IAsyncResult BeginExecuteQuery (string cmdText,
                                                                TdsMetaParameterCollection parameters,
                                                                AsyncCallback callback,
                                                                object state)
		{
			Parameters = parameters;
			string sql = cmdText;
			if (Parameters != null && Parameters.Count > 0)
				sql = BuildExec (cmdText);

			IAsyncResult ar = BeginExecuteQueryInternal (sql, true, callback, state);
			return ar;
		}

		public override void EndExecuteQuery (IAsyncResult ar)
		{
			EndExecuteQueryInternal (ar);
		}

		public override IAsyncResult BeginExecuteProcedure (string prolog,
                                                                    string epilog,
                                                                    string cmdText,
                                                                    bool IsNonQuery,
                                                                    TdsMetaParameterCollection parameters,
                                                                    AsyncCallback callback,
                                                                    object state)
		{
			Parameters = parameters;
			string pcall = BuildProcedureCall (cmdText);
			string sql = String.Format ("{0};{1};{2};", prolog, pcall, epilog);

			IAsyncResult ar = BeginExecuteQueryInternal (sql, !IsNonQuery, callback, state);
			return ar;
		}

		public override void EndExecuteProcedure (IAsyncResult ar)
		{
			EndExecuteQueryInternal (ar);
		}

		#endregion // Asynchronous Methods
#endif // NET_2_0
	}
}
