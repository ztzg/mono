using System;
using System.Data;

using GHTUtils;
using GHTUtils.Base;

using System.Xml;
using System.Runtime.Serialization;

using NUnit.Framework;

namespace tests.system_data_dll.System_Data
{
	[TestFixture]
	public class DataSet_TypedDataSet : ADONetTesterClass
	{
		private string EventStatus = string.Empty ;

		public static void Main()
		{
			DataSet_TypedDataSet tc = new DataSet_TypedDataSet();
			Exception exp = null;
			try
			{
				tc.BeginTest("DataSet_TypedDataSet");
				tc.run();
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
			int i = 0;
			Exception exp = null;

			//check dataset constructor
			myTypedDataSet ds = null;
			DataSet unTypedDs = new DataSet();
			try
			{
				BeginCase("check dataset constructor");
				ds = new myTypedDataSet();
				Compare(ds == null ,false );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//check dataset type
			try
			{
				BeginCase("check dataset type");
				Compare(ds.GetType() ,typeof(myTypedDataSet) );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
				

			//fill dataset from DB and check row count
			System.Data.OleDb.OleDbDataAdapter da = new System.Data.OleDb.OleDbDataAdapter("", GHTUtils.DataProvider.ConnectionString);
			da.SelectCommand.CommandText = "Select OrderID, CustomerID, EmployeeID, OrderDate, RequiredDate, ShippedDate From Orders where OrderID between 10250 and 10270";

			try
			{
				BeginCase("Fill table [Orders] from DB, check new rows count");
				da.Fill(ds,"Orders");
				da.Fill(unTypedDs, "Orders");
				Compare(ds.Tables["Orders"].Rows.Count, unTypedDs.Tables["Orders"].Rows.Count);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			// remove column [UnitPrice] because java throw java.lang.ClassCastException: [Microsoft][SQLServer 2000 Driver for JDBC]Value can not be converted to requested type.
			da.SelectCommand.CommandText = "Select OrderID, ProductID , UnitPrice , Quantity, '5.0' as Discount from \"Order Details\" where OrderID between 10250 and 10270 ";
			try
			{
				BeginCase("Fill table [Order Details] from DB, check new rows count");
				da.Fill(ds,"Order Details");	
				da.Fill(unTypedDs, "Order Details");
				Compare(ds.Tables["Order Details"].Rows.Count, unTypedDs.Tables["Order Details"].Rows.Count);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			// check DataSet named property "Orders"
			myTypedDataSet.OrdersDataTable tblOrders = null;
			try
			{
				BeginCase("check DataSet named property Orders - by name");
				tblOrders = ds.Orders; 
				Compare(tblOrders ,ds.Tables["Orders"]);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("check DataSet named property Orders - by index");
				tblOrders = ds.Orders; 
				Compare(tblOrders ,ds.Tables[1]);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			
			// add new row AddTableNameRow, check row count
			try
			{
				BeginCase("add new row AddTableNameRow, check row count");
				i = tblOrders.Rows.Count;
				tblOrders.AddOrdersRow("SAVEA",1,new DateTime(1998,05,01,00,00,00,000)
					,new DateTime(1998,05,29,00,00,00,000)
					,new DateTime(1998,05,04,00,00,00,000),1,30.0900m
					,"Save-a-lot Markets","187 Suffolk Ln.","Boise","ID","83720","USA");
				Compare(i+1 ,tblOrders.Rows.Count);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		
			//check the new row AutoIncrement field - AddTableNameRow
			try
			{
				BeginCase("check the new row AutoIncrement field");
				i = (int)tblOrders.Rows[tblOrders.Rows.Count-2][0];
				Compare(i+1 ,(int)tblOrders.Rows[tblOrders.Rows.Count-1][0]);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}


			//Create New Row using NewTableNameRow, check row != null
			myTypedDataSet.OrdersRow drOrders = null;
			try
			{
				BeginCase("Create New Row using NewTableNameRow, check row != null");
				drOrders = tblOrders.NewOrdersRow();
				Compare(drOrders == null ,false);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//Create New Row using NewTableNameRow, check row state
			try
			{
				BeginCase("Create New Row using NewTableNameRow, check row state");
				Compare(drOrders.RowState , DataRowState.Detached );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//add new row NewTableNameRow, check row count
			try
			{
				BeginCase("add new row NewTableNameRow, check row count");
				//drOrders.OrderID = DBNull.Value;
				drOrders.CustomerID = "GREAL";
				drOrders.EmployeeID = 4;
				drOrders.OrderDate = new DateTime(1998,04,30,00,00,00,000);
				drOrders.RequiredDate = new DateTime(1998,06,11,00,00,00,000);
				drOrders["ShippedDate"] = DBNull.Value; 
				drOrders.ShipVia = 3;
				drOrders.Freight = 14.0100m;
				drOrders.ShipName = "Great Lakes";
				drOrders.ShipAddress = "Food Market";
				drOrders.ShipCity = "Baker Blvd.";
				drOrders.ShipRegion = "Eugene";
				drOrders.ShipPostalCode = "OR	97403";
				drOrders.ShipCountry =  "USA";

				i = tblOrders.Rows.Count;
				tblOrders.AddOrdersRow(drOrders);
				Compare(i+1 ,tblOrders.Rows.Count);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//check StrongTypingException
			try
			{
				BeginCase("check StrongTypingException");
				try
				{
					DateTime d = drOrders.ShippedDate; //drOrders.ShippedDate = null, will raise exception
				}
				catch (StrongTypingException  ex) {exp = ex;}
				Compare(exp.GetType(),typeof(StrongTypingException));
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		
			//check the new row AutoIncrement field - NewTableNameRow
			try
			{
				BeginCase("check the new row AutoIncrement field");
				i = (int)tblOrders.Rows[tblOrders.Rows.Count-2][0];
				Compare(i+1 ,(int)tblOrders.Rows[tblOrders.Rows.Count-1][0]);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			// convenience IsNull functions
			// only if it can be null
			try
			{
				BeginCase("Check IsColNameNull");
				Compare(drOrders.IsShipAddressNull()  ,false);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			try
			{
				BeginCase("Check SetColNameNull");
				drOrders.SetShipAddressNull(); 
				Compare(drOrders.IsShipAddressNull()  ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
	
			// Table exposes a public property Count == table.Rows.Count
			try
			{
				BeginCase("public property Count == table.Rows.Count");
				Compare(tblOrders.Count ,tblOrders.Rows.Count );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			// find function
			try
			{
				BeginCase("find function");
				myTypedDataSet.OrdersRow dr = tblOrders[0];
				Compare(tblOrders.FindByOrderID(dr.OrderID),dr );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//Remove row and check row count
			try
			{
				BeginCase("Remove row");
				i = tblOrders.Count; 
				myTypedDataSet.OrdersRow dr = tblOrders[0];
				tblOrders.RemoveOrdersRow(dr);
				Compare(i-1 ,tblOrders.Count );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//first column is readonly
			try
			{
				BeginCase("first column is readonly");
				Compare(tblOrders.OrderIDColumn.ReadOnly  ,true);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			//read only exception
			try
			{
				BeginCase("check ReadOnlyException ");
				try
				{
					tblOrders[0].OrderID = 99;
				}
				catch (ReadOnlyException ex) {exp=ex;}
				Compare(exp.GetType() ,typeof(ReadOnlyException));
				exp = null;
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			tblOrders.AcceptChanges();

			//Check table events
			// add event handlers
			ds.Orders.OrdersRowChanging += new myTypedDataSet.OrdersRowChangeEventHandler(T_Changing);
			ds.Orders.OrdersRowChanged += new myTypedDataSet.OrdersRowChangeEventHandler(T_Changed);
			ds.Orders.OrdersRowDeleting += new myTypedDataSet.OrdersRowChangeEventHandler(T_Deleting);
			ds.Orders.OrdersRowDeleted += new myTypedDataSet.OrdersRowChangeEventHandler(T_Deleted);
        
			//RowChange event order
			try
			{
				BeginCase("RowChange event order");
				tblOrders[0].ShipCity = "Tel Aviv";
				Compare(EventStatus  ,"AB" );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}

			EventStatus = string.Empty; 
			//RowDelet event order
			try
			{
				BeginCase("RowDelet event order");
				tblOrders[0].Delete(); 
				Compare(EventStatus  ,"AB" );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		


			//expose DataColumn as property
			try
			{
				BeginCase("expose DataColumn as property");
				Compare(ds.Orders.OrderIDColumn ,ds.Tables["Orders"].Columns["OrderID"]);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		
			//Accept changes for all deleted and changedd rows.
			ds.AcceptChanges();

			//check relations
			//ChildTableRow has property ParentTableRow
			try
			{
				BeginCase("ChildTableRow has property ParentTableRow");
				myTypedDataSet.OrdersRow dr1 = ds.Order_Details[0].OrdersRow;
				DataRow dr2 = ds.Order_Details[0].GetParentRow(ds.Relations[0]);
				Compare(dr1 ,dr2 );
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}


			//ParentTableRow has property ChildTableRow
			try
			{
				BeginCase("check ParentTableRow property ChildTableRow");
				myTypedDataSet.Order_DetailsRow[] drArr1 = ds.Orders[0].GetOrder_DetailsRows();
				DataRow[] drArr2 = ds.Orders[0].GetChildRows(ds.Relations[0]);
				Compare(drArr1 ,drArr2);
			}
			catch(Exception ex)	{exp = ex;}
			finally	{EndCase(exp); exp = null;}
		}
	
		protected void T_Changing(object sender, myTypedDataSet.OrdersRowChangeEvent e) 
		{ 
			EventStatus += "A";
		
		}

		protected void T_Changed(object sender, myTypedDataSet.OrdersRowChangeEvent e) 
		{ 
			EventStatus += "B";
		}

		protected void T_Deleting(object sender, myTypedDataSet.OrdersRowChangeEvent e) 
		{ 
			EventStatus += "A";
		}

		protected void T_Deleted(object sender, myTypedDataSet.OrdersRowChangeEvent e) 
		{ 
			EventStatus += "B";
		}

   
    
		[Serializable()]
			[System.ComponentModel.DesignerCategoryAttribute("code")]
			[System.Diagnostics.DebuggerStepThrough()]
			[System.ComponentModel.ToolboxItem(true)]
			public class myTypedDataSet : DataSet 
		{
        
			private Order_DetailsDataTable tableOrder_Details;
        
			private OrdersDataTable tableOrders;
        
			private DataRelation relationOrdersOrder_x0020_Details;
        
			public myTypedDataSet() 
			{
				this.InitClass();
				System.ComponentModel.CollectionChangeEventHandler schemaChangedHandler = new System.ComponentModel.CollectionChangeEventHandler(this.SchemaChanged);
				this.Tables.CollectionChanged += schemaChangedHandler;
				this.Relations.CollectionChanged += schemaChangedHandler;
			}
        
			protected myTypedDataSet(SerializationInfo info, StreamingContext context) 
			{
				string strSchema = ((string)(info.GetValue("XmlSchema", typeof(string))));
				if ((strSchema != null)) 
				{
					DataSet ds = new DataSet();
					ds.ReadXmlSchema(new XmlTextReader(new System.IO.StringReader(strSchema)));
					if ((ds.Tables["Order Details"] != null)) 
					{
						this.Tables.Add(new Order_DetailsDataTable(ds.Tables["Order Details"]));
					}
					if ((ds.Tables["Orders"] != null)) 
					{
						this.Tables.Add(new OrdersDataTable(ds.Tables["Orders"]));
					}
					this.DataSetName = ds.DataSetName;
					this.Prefix = ds.Prefix;
					this.Namespace = ds.Namespace;
					this.Locale = ds.Locale;
					this.CaseSensitive = ds.CaseSensitive;
					this.EnforceConstraints = ds.EnforceConstraints;
					this.Merge(ds, false, System.Data.MissingSchemaAction.Add);
					this.InitVars();
				}
				else 
				{
					this.InitClass();
				}
				this.GetSerializationData(info, context);
				System.ComponentModel.CollectionChangeEventHandler schemaChangedHandler = new System.ComponentModel.CollectionChangeEventHandler(this.SchemaChanged);
				this.Tables.CollectionChanged += schemaChangedHandler;
				this.Relations.CollectionChanged += schemaChangedHandler;
			}
        
			[System.ComponentModel.Browsable(false)]
			[System.ComponentModel.DesignerSerializationVisibilityAttribute(System.ComponentModel.DesignerSerializationVisibility.Content)]
			public Order_DetailsDataTable Order_Details 
			{
				get 
				{
					return this.tableOrder_Details;
				}
			}
        
			[System.ComponentModel.Browsable(false)]
			[System.ComponentModel.DesignerSerializationVisibilityAttribute(System.ComponentModel.DesignerSerializationVisibility.Content)]
			public OrdersDataTable Orders 
			{
				get 
				{
					return this.tableOrders;
				}
			}
        
			public override DataSet Clone() 
			{
				myTypedDataSet cln = ((myTypedDataSet)(base.Clone()));
				cln.InitVars();
				return cln;
			}
        
			protected override bool ShouldSerializeTables() 
			{
				return false;
			}
        
			protected override bool ShouldSerializeRelations() 
			{
				return false;
			}
        
			protected override void ReadXmlSerializable(XmlReader reader) 
			{
				this.Reset();
				DataSet ds = new DataSet();
				ds.ReadXml(reader);
				if ((ds.Tables["Order Details"] != null)) 
				{
					this.Tables.Add(new Order_DetailsDataTable(ds.Tables["Order Details"]));
				}
				if ((ds.Tables["Orders"] != null)) 
				{
					this.Tables.Add(new OrdersDataTable(ds.Tables["Orders"]));
				}
				this.DataSetName = ds.DataSetName;
				this.Prefix = ds.Prefix;
				this.Namespace = ds.Namespace;
				this.Locale = ds.Locale;
				this.CaseSensitive = ds.CaseSensitive;
				this.EnforceConstraints = ds.EnforceConstraints;
				this.Merge(ds, false, System.Data.MissingSchemaAction.Add);
				this.InitVars();
			}
        
			protected override System.Xml.Schema.XmlSchema GetSchemaSerializable() 
			{
				System.IO.MemoryStream stream = new System.IO.MemoryStream();
				this.WriteXmlSchema(new XmlTextWriter(stream, null));
				stream.Position = 0;
				return System.Xml.Schema.XmlSchema.Read(new XmlTextReader(stream), null);
			}
        
			internal void InitVars() 
			{
				this.tableOrder_Details = ((Order_DetailsDataTable)(this.Tables["Order Details"]));
				if ((this.tableOrder_Details != null)) 
				{
					this.tableOrder_Details.InitVars();
				}
				this.tableOrders = ((OrdersDataTable)(this.Tables["Orders"]));
				if ((this.tableOrders != null)) 
				{
					this.tableOrders.InitVars();
				}
				this.relationOrdersOrder_x0020_Details = this.Relations["OrdersOrder_x0020_Details"];
			}
        
			private void InitClass() 
			{
				this.DataSetName = "myTypedDataSet";
				this.Prefix = "";
				this.Namespace = "http://www.tempuri.org/myTypedDataSet.xsd";
				this.Locale = new System.Globalization.CultureInfo("en-US");
				this.CaseSensitive = false;
				this.EnforceConstraints = true;
				this.tableOrder_Details = new Order_DetailsDataTable();
				this.Tables.Add(this.tableOrder_Details);
				this.tableOrders = new OrdersDataTable();
				this.Tables.Add(this.tableOrders);
				ForeignKeyConstraint fkc;
				fkc = new ForeignKeyConstraint("OrdersOrder_x0020_Details", new DataColumn[] {
																								 this.tableOrders.OrderIDColumn}, new DataColumn[] {
																																					   this.tableOrder_Details.OrderIDColumn});
				this.tableOrder_Details.Constraints.Add(fkc);
				fkc.AcceptRejectRule = System.Data.AcceptRejectRule.None;
				fkc.DeleteRule = System.Data.Rule.Cascade;
				fkc.UpdateRule = System.Data.Rule.Cascade;
				this.relationOrdersOrder_x0020_Details = new DataRelation("OrdersOrder_x0020_Details", new DataColumn[] {
																															this.tableOrders.OrderIDColumn}, new DataColumn[] {
																																												  this.tableOrder_Details.OrderIDColumn}, false);
				this.Relations.Add(this.relationOrdersOrder_x0020_Details);
			}
        
			private bool ShouldSerializeOrder_Details() 
			{
				return false;
			}
        
			private bool ShouldSerializeOrders() 
			{
				return false;
			}
        
			private void SchemaChanged(object sender, System.ComponentModel.CollectionChangeEventArgs e) 
			{
				if ((e.Action == System.ComponentModel.CollectionChangeAction.Remove)) 
				{
					this.InitVars();
				}
			}
        
			public delegate void Order_DetailsRowChangeEventHandler(object sender, Order_DetailsRowChangeEvent e);
        
			public delegate void OrdersRowChangeEventHandler(object sender, OrdersRowChangeEvent e);
        
			[System.Diagnostics.DebuggerStepThrough()]
				public class Order_DetailsDataTable : DataTable, System.Collections.IEnumerable 
			{
            
				private DataColumn columnOrderID;
            
				private DataColumn columnProductID;
            
				private DataColumn columnUnitPrice;
            
				private DataColumn columnQuantity;
            
				private DataColumn columnDiscount;
            
				internal Order_DetailsDataTable() : 
					base("Order Details") 
				{
					this.InitClass();
				}
            
				internal Order_DetailsDataTable(DataTable table) : 
					base(table.TableName) 
				{
					if ((table.CaseSensitive != table.DataSet.CaseSensitive)) 
					{
						this.CaseSensitive = table.CaseSensitive;
					}
					if ((table.Locale.ToString() != table.DataSet.Locale.ToString())) 
					{
						this.Locale = table.Locale;
					}
					if ((table.Namespace != table.DataSet.Namespace)) 
					{
						this.Namespace = table.Namespace;
					}
					this.Prefix = table.Prefix;
					this.MinimumCapacity = table.MinimumCapacity;
					this.DisplayExpression = table.DisplayExpression;
				}
            
				[System.ComponentModel.Browsable(false)]
				public int Count 
				{
					get 
					{
						return this.Rows.Count;
					}
				}
            
				internal DataColumn OrderIDColumn 
				{
					get 
					{
						return this.columnOrderID;
					}
				}
            
				internal DataColumn ProductIDColumn 
				{
					get 
					{
						return this.columnProductID;
					}
				}
            
				internal DataColumn UnitPriceColumn 
				{
					get 
					{
						return this.columnUnitPrice;
					}
				}
            
				internal DataColumn QuantityColumn 
				{
					get 
					{
						return this.columnQuantity;
					}
				}
            
				internal DataColumn DiscountColumn 
				{
					get 
					{
						return this.columnDiscount;
					}
				}
            
				public Order_DetailsRow this[int index] 
				{
					get 
					{
						return ((Order_DetailsRow)(this.Rows[index]));
					}
				}
            
				public event Order_DetailsRowChangeEventHandler Order_DetailsRowChanged;
            
				public event Order_DetailsRowChangeEventHandler Order_DetailsRowChanging;
            
				public event Order_DetailsRowChangeEventHandler Order_DetailsRowDeleted;
            
				public event Order_DetailsRowChangeEventHandler Order_DetailsRowDeleting;
            
				public void AddOrder_DetailsRow(Order_DetailsRow row) 
				{
					this.Rows.Add(row);
				}
            
				public Order_DetailsRow AddOrder_DetailsRow(OrdersRow parentOrdersRowByOrdersOrder_x0020_Details, int ProductID, System.Decimal UnitPrice, short Quantity, string Discount) 
				{
					Order_DetailsRow rowOrder_DetailsRow = ((Order_DetailsRow)(this.NewRow()));
					rowOrder_DetailsRow.ItemArray = new object[] {
																	 parentOrdersRowByOrdersOrder_x0020_Details[0],
																	 ProductID,
																	 UnitPrice,
																	 Quantity,
																	 Discount};
					this.Rows.Add(rowOrder_DetailsRow);
					return rowOrder_DetailsRow;
				}
            
				public Order_DetailsRow FindByOrderIDProductID(int OrderID, int ProductID) 
				{
					return ((Order_DetailsRow)(this.Rows.Find(new object[] {
																			   OrderID,
																			   ProductID})));
				}
            
				public System.Collections.IEnumerator GetEnumerator() 
				{
					return this.Rows.GetEnumerator();
				}
            
				public override DataTable Clone() 
				{
					Order_DetailsDataTable cln = ((Order_DetailsDataTable)(base.Clone()));
					cln.InitVars();
					return cln;
				}
            
				protected override DataTable CreateInstance() 
				{
					return new Order_DetailsDataTable();
				}
            
				internal void InitVars() 
				{
					this.columnOrderID = this.Columns["OrderID"];
					this.columnProductID = this.Columns["ProductID"];
					this.columnUnitPrice = this.Columns["UnitPrice"];
					this.columnQuantity = this.Columns["Quantity"];
					this.columnDiscount = this.Columns["Discount"];
				}
            
				private void InitClass() 
				{
					this.columnOrderID = new DataColumn("OrderID", typeof(int), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnOrderID);
					this.columnProductID = new DataColumn("ProductID", typeof(int), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnProductID);
					this.columnUnitPrice = new DataColumn("UnitPrice", typeof(System.Decimal), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnUnitPrice);
					this.columnQuantity = new DataColumn("Quantity", typeof(short), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnQuantity);
					this.columnDiscount = new DataColumn("Discount", typeof(string), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnDiscount);
					this.Constraints.Add(new UniqueConstraint("Constraint1", new DataColumn[] {
																								  this.columnOrderID,
																								  this.columnProductID}, true));
					this.columnOrderID.AllowDBNull = false;
					this.columnProductID.AllowDBNull = false;
					this.columnUnitPrice.AllowDBNull = false;
					this.columnQuantity.AllowDBNull = false;
					this.columnDiscount.ReadOnly = true;
				}
            
				public Order_DetailsRow NewOrder_DetailsRow() 
				{
					return ((Order_DetailsRow)(this.NewRow()));
				}
            
				protected override DataRow NewRowFromBuilder(DataRowBuilder builder) 
				{
					return new Order_DetailsRow(builder);
				}
            
				protected override System.Type GetRowType() 
				{
					return typeof(Order_DetailsRow);
				}
            
				protected override void OnRowChanged(DataRowChangeEventArgs e) 
				{
					base.OnRowChanged(e);
					if ((this.Order_DetailsRowChanged != null)) 
					{
						this.Order_DetailsRowChanged(this, new Order_DetailsRowChangeEvent(((Order_DetailsRow)(e.Row)), e.Action));
					}
				}
            
				protected override void OnRowChanging(DataRowChangeEventArgs e) 
				{
					base.OnRowChanging(e);
					if ((this.Order_DetailsRowChanging != null)) 
					{
						this.Order_DetailsRowChanging(this, new Order_DetailsRowChangeEvent(((Order_DetailsRow)(e.Row)), e.Action));
					}
				}
            
				protected override void OnRowDeleted(DataRowChangeEventArgs e) 
				{
					base.OnRowDeleted(e);
					if ((this.Order_DetailsRowDeleted != null)) 
					{
						this.Order_DetailsRowDeleted(this, new Order_DetailsRowChangeEvent(((Order_DetailsRow)(e.Row)), e.Action));
					}
				}
            
				protected override void OnRowDeleting(DataRowChangeEventArgs e) 
				{
					base.OnRowDeleting(e);
					if ((this.Order_DetailsRowDeleting != null)) 
					{
						this.Order_DetailsRowDeleting(this, new Order_DetailsRowChangeEvent(((Order_DetailsRow)(e.Row)), e.Action));
					}
				}
            
				public void RemoveOrder_DetailsRow(Order_DetailsRow row) 
				{
					this.Rows.Remove(row);
				}
			}
        
			[System.Diagnostics.DebuggerStepThrough()]
				public class Order_DetailsRow : DataRow 
			{
            
				private Order_DetailsDataTable tableOrder_Details;
            
				internal Order_DetailsRow(DataRowBuilder rb) : 
					base(rb) 
				{
					this.tableOrder_Details = ((Order_DetailsDataTable)(this.Table));
				}
            
				public int OrderID 
				{
					get 
					{
						return ((int)(this[this.tableOrder_Details.OrderIDColumn]));
					}
					set 
					{
						this[this.tableOrder_Details.OrderIDColumn] = value;
					}
				}
            
				public int ProductID 
				{
					get 
					{
						return ((int)(this[this.tableOrder_Details.ProductIDColumn]));
					}
					set 
					{
						this[this.tableOrder_Details.ProductIDColumn] = value;
					}
				}
            
				public System.Decimal UnitPrice 
				{
					get 
					{
						return ((System.Decimal)(this[this.tableOrder_Details.UnitPriceColumn]));
					}
					set 
					{
						this[this.tableOrder_Details.UnitPriceColumn] = value;
					}
				}
            
				public short Quantity 
				{
					get 
					{
						return ((short)(this[this.tableOrder_Details.QuantityColumn]));
					}
					set 
					{
						this[this.tableOrder_Details.QuantityColumn] = value;
					}
				}
            
				public string Discount 
				{
					get 
					{
						try 
						{
							return ((string)(this[this.tableOrder_Details.DiscountColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrder_Details.DiscountColumn] = value;
					}
				}
            
				public OrdersRow OrdersRow 
				{
					get 
					{
						return ((OrdersRow)(this.GetParentRow(this.Table.ParentRelations["OrdersOrder_x0020_Details"])));
					}
					set 
					{
						this.SetParentRow(value, this.Table.ParentRelations["OrdersOrder_x0020_Details"]);
					}
				}
            
				public bool IsDiscountNull() 
				{
					return this.IsNull(this.tableOrder_Details.DiscountColumn);
				}
            
				public void SetDiscountNull() 
				{
					this[this.tableOrder_Details.DiscountColumn] = System.Convert.DBNull;
				}
			}
        
			[System.Diagnostics.DebuggerStepThrough()]
				public class Order_DetailsRowChangeEvent : EventArgs 
			{
            
				private Order_DetailsRow eventRow;
            
				private DataRowAction eventAction;
            
				public Order_DetailsRowChangeEvent(Order_DetailsRow row, DataRowAction action) 
				{
					this.eventRow = row;
					this.eventAction = action;
				}
            
				public Order_DetailsRow Row 
				{
					get 
					{
						return this.eventRow;
					}
				}
            
				public DataRowAction Action 
				{
					get 
					{
						return this.eventAction;
					}
				}
			}
        
			[System.Diagnostics.DebuggerStepThrough()]
				public class OrdersDataTable : DataTable, System.Collections.IEnumerable 
			{
            
				private DataColumn columnOrderID;
            
				private DataColumn columnCustomerID;
            
				private DataColumn columnEmployeeID;
            
				private DataColumn columnOrderDate;
            
				private DataColumn columnRequiredDate;
            
				private DataColumn columnShippedDate;
            
				private DataColumn columnShipVia;
            
				private DataColumn columnFreight;
            
				private DataColumn columnShipName;
            
				private DataColumn columnShipAddress;
            
				private DataColumn columnShipCity;
            
				private DataColumn columnShipRegion;
            
				private DataColumn columnShipPostalCode;
            
				private DataColumn columnShipCountry;
            
				internal OrdersDataTable() : 
					base("Orders") 
				{
					this.InitClass();
				}
            
				internal OrdersDataTable(DataTable table) : 
					base(table.TableName) 
				{
					if ((table.CaseSensitive != table.DataSet.CaseSensitive)) 
					{
						this.CaseSensitive = table.CaseSensitive;
					}
					if ((table.Locale.ToString() != table.DataSet.Locale.ToString())) 
					{
						this.Locale = table.Locale;
					}
					if ((table.Namespace != table.DataSet.Namespace)) 
					{
						this.Namespace = table.Namespace;
					}
					this.Prefix = table.Prefix;
					this.MinimumCapacity = table.MinimumCapacity;
					this.DisplayExpression = table.DisplayExpression;
				}
            
				[System.ComponentModel.Browsable(false)]
				public int Count 
				{
					get 
					{
						return this.Rows.Count;
					}
				}
            
				internal DataColumn OrderIDColumn 
				{
					get 
					{
						return this.columnOrderID;
					}
				}
            
				internal DataColumn CustomerIDColumn 
				{
					get 
					{
						return this.columnCustomerID;
					}
				}
            
				internal DataColumn EmployeeIDColumn 
				{
					get 
					{
						return this.columnEmployeeID;
					}
				}
            
				internal DataColumn OrderDateColumn 
				{
					get 
					{
						return this.columnOrderDate;
					}
				}
            
				internal DataColumn RequiredDateColumn 
				{
					get 
					{
						return this.columnRequiredDate;
					}
				}
            
				internal DataColumn ShippedDateColumn 
				{
					get 
					{
						return this.columnShippedDate;
					}
				}
            
				internal DataColumn ShipViaColumn 
				{
					get 
					{
						return this.columnShipVia;
					}
				}
            
				internal DataColumn FreightColumn 
				{
					get 
					{
						return this.columnFreight;
					}
				}
            
				internal DataColumn ShipNameColumn 
				{
					get 
					{
						return this.columnShipName;
					}
				}
            
				internal DataColumn ShipAddressColumn 
				{
					get 
					{
						return this.columnShipAddress;
					}
				}
            
				internal DataColumn ShipCityColumn 
				{
					get 
					{
						return this.columnShipCity;
					}
				}
            
				internal DataColumn ShipRegionColumn 
				{
					get 
					{
						return this.columnShipRegion;
					}
				}
            
				internal DataColumn ShipPostalCodeColumn 
				{
					get 
					{
						return this.columnShipPostalCode;
					}
				}
            
				internal DataColumn ShipCountryColumn 
				{
					get 
					{
						return this.columnShipCountry;
					}
				}
            
				public OrdersRow this[int index] 
				{
					get 
					{
						return ((OrdersRow)(this.Rows[index]));
					}
				}
            
				public event OrdersRowChangeEventHandler OrdersRowChanged;
            
				public event OrdersRowChangeEventHandler OrdersRowChanging;
            
				public event OrdersRowChangeEventHandler OrdersRowDeleted;
            
				public event OrdersRowChangeEventHandler OrdersRowDeleting;
            
				public void AddOrdersRow(OrdersRow row) 
				{
					this.Rows.Add(row);
				}
            
				public OrdersRow AddOrdersRow(string CustomerID, int EmployeeID, System.DateTime OrderDate, System.DateTime RequiredDate, System.DateTime ShippedDate, int ShipVia, System.Decimal Freight, string ShipName, string ShipAddress, string ShipCity, string ShipRegion, string ShipPostalCode, string ShipCountry) 
				{
					OrdersRow rowOrdersRow = ((OrdersRow)(this.NewRow()));
					rowOrdersRow.ItemArray = new object[] {
															  null,
															  CustomerID,
															  EmployeeID,
															  OrderDate,
															  RequiredDate,
															  ShippedDate,
															  ShipVia,
															  Freight,
															  ShipName,
															  ShipAddress,
															  ShipCity,
															  ShipRegion,
															  ShipPostalCode,
															  ShipCountry};
					this.Rows.Add(rowOrdersRow);
					return rowOrdersRow;
				}
            
				public OrdersRow FindByOrderID(int OrderID) 
				{
					return ((OrdersRow)(this.Rows.Find(new object[] {
																		OrderID})));
				}
            
				public System.Collections.IEnumerator GetEnumerator() 
				{
					return this.Rows.GetEnumerator();
				}
            
				public override DataTable Clone() 
				{
					OrdersDataTable cln = ((OrdersDataTable)(base.Clone()));
					cln.InitVars();
					return cln;
				}
            
				protected override DataTable CreateInstance() 
				{
					return new OrdersDataTable();
				}
            
				internal void InitVars() 
				{
					this.columnOrderID = this.Columns["OrderID"];
					this.columnCustomerID = this.Columns["CustomerID"];
					this.columnEmployeeID = this.Columns["EmployeeID"];
					this.columnOrderDate = this.Columns["OrderDate"];
					this.columnRequiredDate = this.Columns["RequiredDate"];
					this.columnShippedDate = this.Columns["ShippedDate"];
					this.columnShipVia = this.Columns["ShipVia"];
					this.columnFreight = this.Columns["Freight"];
					this.columnShipName = this.Columns["ShipName"];
					this.columnShipAddress = this.Columns["ShipAddress"];
					this.columnShipCity = this.Columns["ShipCity"];
					this.columnShipRegion = this.Columns["ShipRegion"];
					this.columnShipPostalCode = this.Columns["ShipPostalCode"];
					this.columnShipCountry = this.Columns["ShipCountry"];
				}
            
				private void InitClass() 
				{
					this.columnOrderID = new DataColumn("OrderID", typeof(int), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnOrderID);
					this.columnCustomerID = new DataColumn("CustomerID", typeof(string), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnCustomerID);
					this.columnEmployeeID = new DataColumn("EmployeeID", typeof(int), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnEmployeeID);
					this.columnOrderDate = new DataColumn("OrderDate", typeof(System.DateTime), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnOrderDate);
					this.columnRequiredDate = new DataColumn("RequiredDate", typeof(System.DateTime), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnRequiredDate);
					this.columnShippedDate = new DataColumn("ShippedDate", typeof(System.DateTime), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnShippedDate);
					this.columnShipVia = new DataColumn("ShipVia", typeof(int), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnShipVia);
					this.columnFreight = new DataColumn("Freight", typeof(System.Decimal), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnFreight);
					this.columnShipName = new DataColumn("ShipName", typeof(string), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnShipName);
					this.columnShipAddress = new DataColumn("ShipAddress", typeof(string), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnShipAddress);
					this.columnShipCity = new DataColumn("ShipCity", typeof(string), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnShipCity);
					this.columnShipRegion = new DataColumn("ShipRegion", typeof(string), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnShipRegion);
					this.columnShipPostalCode = new DataColumn("ShipPostalCode", typeof(string), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnShipPostalCode);
					this.columnShipCountry = new DataColumn("ShipCountry", typeof(string), null, System.Data.MappingType.Element);
					this.Columns.Add(this.columnShipCountry);
					this.Constraints.Add(new UniqueConstraint("Constraint1", new DataColumn[] {
																								  this.columnOrderID}, true));
					this.columnOrderID.AutoIncrement = true;
					this.columnOrderID.AllowDBNull = false;
					this.columnOrderID.ReadOnly = true;
					this.columnOrderID.Unique = true;
				}
            
				public OrdersRow NewOrdersRow() 
				{
					return ((OrdersRow)(this.NewRow()));
				}
            
				protected override DataRow NewRowFromBuilder(DataRowBuilder builder) 
				{
					return new OrdersRow(builder);
				}
            
				protected override System.Type GetRowType() 
				{
					return typeof(OrdersRow);
				}
            
				protected override void OnRowChanged(DataRowChangeEventArgs e) 
				{
					base.OnRowChanged(e);
					if ((this.OrdersRowChanged != null)) 
					{
						this.OrdersRowChanged(this, new OrdersRowChangeEvent(((OrdersRow)(e.Row)), e.Action));
					}
				}
            
				protected override void OnRowChanging(DataRowChangeEventArgs e) 
				{
					base.OnRowChanging(e);
					if ((this.OrdersRowChanging != null)) 
					{
						this.OrdersRowChanging(this, new OrdersRowChangeEvent(((OrdersRow)(e.Row)), e.Action));
					}
				}
            
				protected override void OnRowDeleted(DataRowChangeEventArgs e) 
				{
					base.OnRowDeleted(e);
					if ((this.OrdersRowDeleted != null)) 
					{
						this.OrdersRowDeleted(this, new OrdersRowChangeEvent(((OrdersRow)(e.Row)), e.Action));
					}
				}
            
				protected override void OnRowDeleting(DataRowChangeEventArgs e) 
				{
					base.OnRowDeleting(e);
					if ((this.OrdersRowDeleting != null)) 
					{
						this.OrdersRowDeleting(this, new OrdersRowChangeEvent(((OrdersRow)(e.Row)), e.Action));
					}
				}
            
				public void RemoveOrdersRow(OrdersRow row) 
				{
					this.Rows.Remove(row);
				}
			}
        
			[System.Diagnostics.DebuggerStepThrough()]
				public class OrdersRow : DataRow 
			{
            
				private OrdersDataTable tableOrders;
            
				internal OrdersRow(DataRowBuilder rb) : 
					base(rb) 
				{
					this.tableOrders = ((OrdersDataTable)(this.Table));
				}
            
				public int OrderID 
				{
					get 
					{
						return ((int)(this[this.tableOrders.OrderIDColumn]));
					}
					set 
					{
						this[this.tableOrders.OrderIDColumn] = value;
					}
				}
            
				public string CustomerID 
				{
					get 
					{
						try 
						{
							return ((string)(this[this.tableOrders.CustomerIDColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.CustomerIDColumn] = value;
					}
				}
            
				public int EmployeeID 
				{
					get 
					{
						try 
						{
							return ((int)(this[this.tableOrders.EmployeeIDColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.EmployeeIDColumn] = value;
					}
				}
            
				public System.DateTime OrderDate 
				{
					get 
					{
						try 
						{
							return ((System.DateTime)(this[this.tableOrders.OrderDateColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.OrderDateColumn] = value;
					}
				}
            
				public System.DateTime RequiredDate 
				{
					get 
					{
						try 
						{
							return ((System.DateTime)(this[this.tableOrders.RequiredDateColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.RequiredDateColumn] = value;
					}
				}
            
				public System.DateTime ShippedDate 
				{
					get 
					{
						try 
						{
							return ((System.DateTime)(this[this.tableOrders.ShippedDateColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.ShippedDateColumn] = value;
					}
				}
            
				public int ShipVia 
				{
					get 
					{
						try 
						{
							return ((int)(this[this.tableOrders.ShipViaColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.ShipViaColumn] = value;
					}
				}
            
				public System.Decimal Freight 
				{
					get 
					{
						try 
						{
							return ((System.Decimal)(this[this.tableOrders.FreightColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.FreightColumn] = value;
					}
				}
            
				public string ShipName 
				{
					get 
					{
						try 
						{
							return ((string)(this[this.tableOrders.ShipNameColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.ShipNameColumn] = value;
					}
				}
            
				public string ShipAddress 
				{
					get 
					{
						try 
						{
							return ((string)(this[this.tableOrders.ShipAddressColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.ShipAddressColumn] = value;
					}
				}
            
				public string ShipCity 
				{
					get 
					{
						try 
						{
							return ((string)(this[this.tableOrders.ShipCityColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.ShipCityColumn] = value;
					}
				}
            
				public string ShipRegion 
				{
					get 
					{
						try 
						{
							return ((string)(this[this.tableOrders.ShipRegionColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.ShipRegionColumn] = value;
					}
				}
            
				public string ShipPostalCode 
				{
					get 
					{
						try 
						{
							return ((string)(this[this.tableOrders.ShipPostalCodeColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.ShipPostalCodeColumn] = value;
					}
				}
            
				public string ShipCountry 
				{
					get 
					{
						try 
						{
							return ((string)(this[this.tableOrders.ShipCountryColumn]));
						}
						catch (InvalidCastException e) 
						{
							throw new StrongTypingException("Cannot get value because it is DBNull.", e);
						}
					}
					set 
					{
						this[this.tableOrders.ShipCountryColumn] = value;
					}
				}
            
				public bool IsCustomerIDNull() 
				{
					return this.IsNull(this.tableOrders.CustomerIDColumn);
				}
            
				public void SetCustomerIDNull() 
				{
					this[this.tableOrders.CustomerIDColumn] = System.Convert.DBNull;
				}
            
				public bool IsEmployeeIDNull() 
				{
					return this.IsNull(this.tableOrders.EmployeeIDColumn);
				}
            
				public void SetEmployeeIDNull() 
				{
					this[this.tableOrders.EmployeeIDColumn] = System.Convert.DBNull;
				}
            
				public bool IsOrderDateNull() 
				{
					return this.IsNull(this.tableOrders.OrderDateColumn);
				}
            
				public void SetOrderDateNull() 
				{
					this[this.tableOrders.OrderDateColumn] = System.Convert.DBNull;
				}
            
				public bool IsRequiredDateNull() 
				{
					return this.IsNull(this.tableOrders.RequiredDateColumn);
				}
            
				public void SetRequiredDateNull() 
				{
					this[this.tableOrders.RequiredDateColumn] = System.Convert.DBNull;
				}
            
				public bool IsShippedDateNull() 
				{
					return this.IsNull(this.tableOrders.ShippedDateColumn);
				}
            
				public void SetShippedDateNull() 
				{
					this[this.tableOrders.ShippedDateColumn] = System.Convert.DBNull;
				}
            
				public bool IsShipViaNull() 
				{
					return this.IsNull(this.tableOrders.ShipViaColumn);
				}
            
				public void SetShipViaNull() 
				{
					this[this.tableOrders.ShipViaColumn] = System.Convert.DBNull;
				}
            
				public bool IsFreightNull() 
				{
					return this.IsNull(this.tableOrders.FreightColumn);
				}
            
				public void SetFreightNull() 
				{
					this[this.tableOrders.FreightColumn] = System.Convert.DBNull;
				}
            
				public bool IsShipNameNull() 
				{
					return this.IsNull(this.tableOrders.ShipNameColumn);
				}
            
				public void SetShipNameNull() 
				{
					this[this.tableOrders.ShipNameColumn] = System.Convert.DBNull;
				}
            
				public bool IsShipAddressNull() 
				{
					return this.IsNull(this.tableOrders.ShipAddressColumn);
				}
            
				public void SetShipAddressNull() 
				{
					this[this.tableOrders.ShipAddressColumn] = System.Convert.DBNull;
				}
            
				public bool IsShipCityNull() 
				{
					return this.IsNull(this.tableOrders.ShipCityColumn);
				}
            
				public void SetShipCityNull() 
				{
					this[this.tableOrders.ShipCityColumn] = System.Convert.DBNull;
				}
            
				public bool IsShipRegionNull() 
				{
					return this.IsNull(this.tableOrders.ShipRegionColumn);
				}
            
				public void SetShipRegionNull() 
				{
					this[this.tableOrders.ShipRegionColumn] = System.Convert.DBNull;
				}
            
				public bool IsShipPostalCodeNull() 
				{
					return this.IsNull(this.tableOrders.ShipPostalCodeColumn);
				}
            
				public void SetShipPostalCodeNull() 
				{
					this[this.tableOrders.ShipPostalCodeColumn] = System.Convert.DBNull;
				}
            
				public bool IsShipCountryNull() 
				{
					return this.IsNull(this.tableOrders.ShipCountryColumn);
				}
            
				public void SetShipCountryNull() 
				{
					this[this.tableOrders.ShipCountryColumn] = System.Convert.DBNull;
				}
            
				public Order_DetailsRow[] GetOrder_DetailsRows() 
				{
					return ((Order_DetailsRow[])(this.GetChildRows(this.Table.ChildRelations["OrdersOrder_x0020_Details"])));
				}
			}
        
			[System.Diagnostics.DebuggerStepThrough()]
				public class OrdersRowChangeEvent : EventArgs 
			{
            
				private OrdersRow eventRow;
            
				private DataRowAction eventAction;
            
				public OrdersRowChangeEvent(OrdersRow row, DataRowAction action) 
				{
					this.eventRow = row;
					this.eventAction = action;
				}
            
				public OrdersRow Row 
				{
					get 
					{
						return this.eventRow;
					}
				}
            
				public DataRowAction Action 
				{
					get 
					{
						return this.eventAction;
					}
				}
			}
		}
	}
}
