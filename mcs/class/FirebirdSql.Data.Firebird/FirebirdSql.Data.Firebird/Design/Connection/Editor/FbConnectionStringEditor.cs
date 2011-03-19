/*
 *	Firebird ADO.NET Data provider for .NET and Mono 
 * 
 *	   The contents of this file are subject to the Initial 
 *	   Developer's Public License Version 1.0 (the "License"); 
 *	   you may not use this file except in compliance with the 
 *	   License. You may obtain a copy of the License at 
 *	   http://www.firebirdsql.org/index.php?op=doc&id=idpl
 *
 *	   Software distributed under the License is distributed on 
 *	   an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either 
 *	   express or implied. See the License for the specific 
 *	   language governing rights and limitations under the License.
 * 
 *	Copyright (c) 2002, 2005 Carlos Guzman Alvarez
 *	All Rights Reserved.
 */

#if	(NET)

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Globalization;
using System.Windows.Forms;

namespace FirebirdSql.Data.Firebird.Design
{
	internal class FbConnectionStringEditor : System.Windows.Forms.Form
	{
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.Button cmdTest;
		private System.Windows.Forms.Button cmdAccept;
		private System.Windows.Forms.Button cmdCancel;
		private System.Windows.Forms.TextBox txtDataSource;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label lblDatabase;
		private System.Windows.Forms.Button cmdGetFile;
		private System.Windows.Forms.OpenFileDialog openFirebirdDB;
		private System.Windows.Forms.GroupBox grbLogin;
		private System.Windows.Forms.TextBox txtUserName;
		private System.Windows.Forms.Label lblUser;
		private System.Windows.Forms.Label lblPassword;
		private System.Windows.Forms.TextBox txtPassword;
		private System.Windows.Forms.GroupBox grbSettings;
		private System.Windows.Forms.TextBox txtPort;
		private System.Windows.Forms.TextBox txtDatabase;
		private System.Windows.Forms.CheckBox chkPooling;
		private System.Windows.Forms.Label lblRole;
		private System.Windows.Forms.TextBox txtRole;
		private System.Windows.Forms.Label lblLifeTime;
		private System.Windows.Forms.NumericUpDown txtLifeTime;
		private System.Windows.Forms.Label lblTimeout;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.NumericUpDown txtTimeout;
		private System.Windows.Forms.ComboBox cboCharset;
		private System.Windows.Forms.Label lblDialect;
		private System.Windows.Forms.ComboBox cboPacketSize;
		private FbConnection connection;
		private System.Windows.Forms.ComboBox cboServerType;
		private System.Windows.Forms.Label lblServerType;
		private System.Windows.Forms.Label lblDataSource;
		private System.Windows.Forms.Label lblCharset;
		private System.Windows.Forms.ComboBox cboDialect;

		public FbConnectionStringEditor()
			: this(new FbConnection())
		{
		}

		public FbConnectionStringEditor(FbConnection connection)
		{
			this.connection = connection;

			InitializeComponent();
			InitializeData();
		}

		private void InitializeData()
		{
			FbConnectionString cs = new FbConnectionString(this.connection.ConnectionString);

			this.txtDataSource.Text = cs.DataSource;
			this.txtPort.Text = cs.Port.ToString(CultureInfo.InvariantCulture);
			this.txtDatabase.Text = cs.Database;
			this.txtUserName.Text = cs.UserID;
			this.txtPassword.Text = cs.Password;
			this.txtRole.Text = cs.Role;
			this.cboPacketSize.Text = cs.PacketSize.ToString(CultureInfo.InvariantCulture);
			this.cboDialect.Text = cs.Dialect.ToString(CultureInfo.InvariantCulture);
			this.txtLifeTime.Value = cs.ConnectionLifeTime;
			this.txtTimeout.Value = cs.ConnectionTimeout;
			this.cboCharset.Text = cs.Charset;
			this.chkPooling.Checked = cs.Pooling;

			switch (cs.ServerType)
			{
				case 0:
					this.cboServerType.Text = "Super/Classic Server";
					break;

				case 1:
					this.cboServerType.Text = "Embedded Server";
					break;
			}
		}

		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
				if (components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}

		#region Windows	Form Designer generated	code
		private void InitializeComponent()
		{
			this.cmdTest = new System.Windows.Forms.Button();
			this.cmdAccept = new System.Windows.Forms.Button();
			this.cmdCancel = new System.Windows.Forms.Button();
			this.txtDataSource = new System.Windows.Forms.TextBox();
			this.panel1 = new System.Windows.Forms.Panel();
			this.lblDataSource = new System.Windows.Forms.Label();
			this.cboDialect = new System.Windows.Forms.ComboBox();
			this.lblLifeTime = new System.Windows.Forms.Label();
			this.grbSettings = new System.Windows.Forms.GroupBox();
			this.cboPacketSize = new System.Windows.Forms.ComboBox();
			this.txtLifeTime = new System.Windows.Forms.NumericUpDown();
			this.txtTimeout = new System.Windows.Forms.NumericUpDown();
			this.lblTimeout = new System.Windows.Forms.Label();
			this.chkPooling = new System.Windows.Forms.CheckBox();
			this.label3 = new System.Windows.Forms.Label();
			this.cboServerType = new System.Windows.Forms.ComboBox();
			this.lblServerType = new System.Windows.Forms.Label();
			this.grbLogin = new System.Windows.Forms.GroupBox();
			this.lblRole = new System.Windows.Forms.Label();
			this.txtRole = new System.Windows.Forms.TextBox();
			this.lblPassword = new System.Windows.Forms.Label();
			this.txtPassword = new System.Windows.Forms.TextBox();
			this.lblUser = new System.Windows.Forms.Label();
			this.txtUserName = new System.Windows.Forms.TextBox();
			this.cmdGetFile = new System.Windows.Forms.Button();
			this.txtDatabase = new System.Windows.Forms.TextBox();
			this.lblDatabase = new System.Windows.Forms.Label();
			this.txtPort = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.lblDialect = new System.Windows.Forms.Label();
			this.lblCharset = new System.Windows.Forms.Label();
			this.cboCharset = new System.Windows.Forms.ComboBox();
			this.openFirebirdDB = new System.Windows.Forms.OpenFileDialog();
			this.panel1.SuspendLayout();
			this.grbSettings.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.txtLifeTime)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.txtTimeout)).BeginInit();
			this.grbLogin.SuspendLayout();
			this.SuspendLayout();
			// 
			// cmdTest
			// 
			this.cmdTest.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.cmdTest.Location = new System.Drawing.Point(7, 240);
			this.cmdTest.Name = "cmdTest";
			this.cmdTest.TabIndex = 2;
			this.cmdTest.Text = "&Test";
			this.cmdTest.Click += new System.EventHandler(this.cmdTest_Click);
			// 
			// cmdAccept
			// 
			this.cmdAccept.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.cmdAccept.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.cmdAccept.Location = new System.Drawing.Point(334, 240);
			this.cmdAccept.Name = "cmdAccept";
			this.cmdAccept.TabIndex = 3;
			this.cmdAccept.Text = "&Accept";
			this.cmdAccept.Click += new System.EventHandler(this.cmdAccept_Click);
			// 
			// cmdCancel
			// 
			this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cmdCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.cmdCancel.Location = new System.Drawing.Point(409, 240);
			this.cmdCancel.Name = "cmdCancel";
			this.cmdCancel.Size = new System.Drawing.Size(72, 23);
			this.cmdCancel.TabIndex = 4;
			this.cmdCancel.Text = "&Cancel";
			// 
			// txtDataSource
			// 
			this.txtDataSource.Location = new System.Drawing.Point(8, 24);
			this.txtDataSource.Name = "txtDataSource";
			this.txtDataSource.Size = new System.Drawing.Size(136, 20);
			this.txtDataSource.TabIndex = 0;
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.lblDataSource);
			this.panel1.Controls.Add(this.cboDialect);
			this.panel1.Controls.Add(this.lblLifeTime);
			this.panel1.Controls.Add(this.grbSettings);
			this.panel1.Controls.Add(this.grbLogin);
			this.panel1.Controls.Add(this.cmdGetFile);
			this.panel1.Controls.Add(this.txtDatabase);
			this.panel1.Controls.Add(this.lblDatabase);
			this.panel1.Controls.Add(this.txtPort);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Controls.Add(this.lblDialect);
			this.panel1.Controls.Add(this.txtDataSource);
			this.panel1.Controls.Add(this.lblCharset);
			this.panel1.Controls.Add(this.cboCharset);
			this.panel1.Location = new System.Drawing.Point(8, 8);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(472, 224);
			this.panel1.TabIndex = 0;
			// 
			// lblDataSource
			// 
			this.lblDataSource.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblDataSource.Location = new System.Drawing.Point(8, 8);
			this.lblDataSource.Name = "lblDataSource";
			this.lblDataSource.Size = new System.Drawing.Size(64, 16);
			this.lblDataSource.TabIndex = 13;
			this.lblDataSource.Text = "Data Source";
			// 
			// cboDialect
			// 
			this.cboDialect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboDialect.Items.AddRange(new object[] {
            "1",
            "3"});
			this.cboDialect.Location = new System.Drawing.Point(248, 24);
			this.cboDialect.Name = "cboDialect";
			this.cboDialect.Size = new System.Drawing.Size(63, 21);
			this.cboDialect.TabIndex = 4;
			// 
			// lblLifeTime
			// 
			this.lblLifeTime.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblLifeTime.Location = new System.Drawing.Point(224, 120);
			this.lblLifeTime.Name = "lblLifeTime";
			this.lblLifeTime.Size = new System.Drawing.Size(48, 16);
			this.lblLifeTime.TabIndex = 12;
			this.lblLifeTime.Text = "Life Time";
			// 
			// grbSettings
			// 
			this.grbSettings.Controls.Add(this.cboPacketSize);
			this.grbSettings.Controls.Add(this.txtLifeTime);
			this.grbSettings.Controls.Add(this.txtTimeout);
			this.grbSettings.Controls.Add(this.lblTimeout);
			this.grbSettings.Controls.Add(this.chkPooling);
			this.grbSettings.Controls.Add(this.label3);
			this.grbSettings.Controls.Add(this.cboServerType);
			this.grbSettings.Controls.Add(this.lblServerType);
			this.grbSettings.Location = new System.Drawing.Point(208, 96);
			this.grbSettings.Name = "grbSettings";
			this.grbSettings.Size = new System.Drawing.Size(256, 120);
			this.grbSettings.TabIndex = 11;
			this.grbSettings.TabStop = false;
			this.grbSettings.Text = "Connection\tSettings";
			// 
			// cboPacketSize
			// 
			this.cboPacketSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboPacketSize.Items.AddRange(new object[] {
            "1024",
            "2048",
            "4096",
            "8192",
            "16384"});
			this.cboPacketSize.Location = new System.Drawing.Point(176, 54);
			this.cboPacketSize.Name = "cboPacketSize";
			this.cboPacketSize.Size = new System.Drawing.Size(64, 21);
			this.cboPacketSize.TabIndex = 5;
			// 
			// txtLifeTime
			// 
			this.txtLifeTime.Location = new System.Drawing.Point(72, 24);
			this.txtLifeTime.Name = "txtLifeTime";
			this.txtLifeTime.Size = new System.Drawing.Size(48, 20);
			this.txtLifeTime.TabIndex = 0;
			this.txtLifeTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// txtTimeout
			// 
			this.txtTimeout.Location = new System.Drawing.Point(184, 22);
			this.txtTimeout.Name = "txtTimeout";
			this.txtTimeout.Size = new System.Drawing.Size(56, 20);
			this.txtTimeout.TabIndex = 2;
			this.txtTimeout.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// lblTimeout
			// 
			this.lblTimeout.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblTimeout.Location = new System.Drawing.Point(136, 24);
			this.lblTimeout.Name = "lblTimeout";
			this.lblTimeout.Size = new System.Drawing.Size(48, 16);
			this.lblTimeout.TabIndex = 1;
			this.lblTimeout.Text = "Timeout";
			// 
			// chkPooling
			// 
			this.chkPooling.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.chkPooling.Location = new System.Drawing.Point(16, 56);
			this.chkPooling.Name = "chkPooling";
			this.chkPooling.Size = new System.Drawing.Size(96, 14);
			this.chkPooling.TabIndex = 3;
			this.chkPooling.Text = "Enable pooling";
			// 
			// label3
			// 
			this.label3.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.label3.Location = new System.Drawing.Point(112, 56);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(59, 16);
			this.label3.TabIndex = 4;
			this.label3.Text = "Packet size";
			// 
			// cboServerType
			// 
			this.cboServerType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboServerType.Items.AddRange(new object[] {
            "Super/Classic Server",
            "Embedded Server"});
			this.cboServerType.Location = new System.Drawing.Point(96, 88);
			this.cboServerType.Name = "cboServerType";
			this.cboServerType.Size = new System.Drawing.Size(144, 21);
			this.cboServerType.TabIndex = 13;
			// 
			// lblServerType
			// 
			this.lblServerType.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblServerType.Location = new System.Drawing.Point(16, 88);
			this.lblServerType.Name = "lblServerType";
			this.lblServerType.Size = new System.Drawing.Size(64, 16);
			this.lblServerType.TabIndex = 13;
			this.lblServerType.Text = "Server Type";
			// 
			// grbLogin
			// 
			this.grbLogin.Controls.Add(this.lblRole);
			this.grbLogin.Controls.Add(this.txtRole);
			this.grbLogin.Controls.Add(this.lblPassword);
			this.grbLogin.Controls.Add(this.txtPassword);
			this.grbLogin.Controls.Add(this.lblUser);
			this.grbLogin.Controls.Add(this.txtUserName);
			this.grbLogin.Location = new System.Drawing.Point(8, 96);
			this.grbLogin.Name = "grbLogin";
			this.grbLogin.Size = new System.Drawing.Size(192, 120);
			this.grbLogin.TabIndex = 10;
			this.grbLogin.TabStop = false;
			this.grbLogin.Text = "Login";
			// 
			// lblRole
			// 
			this.lblRole.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblRole.Location = new System.Drawing.Point(8, 86);
			this.lblRole.Name = "lblRole";
			this.lblRole.Size = new System.Drawing.Size(48, 14);
			this.lblRole.TabIndex = 4;
			this.lblRole.Text = "Role";
			// 
			// txtRole
			// 
			this.txtRole.Location = new System.Drawing.Point(64, 88);
			this.txtRole.Name = "txtRole";
			this.txtRole.Size = new System.Drawing.Size(112, 20);
			this.txtRole.TabIndex = 5;
			// 
			// lblPassword
			// 
			this.lblPassword.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblPassword.Location = new System.Drawing.Point(8, 55);
			this.lblPassword.Name = "lblPassword";
			this.lblPassword.Size = new System.Drawing.Size(48, 16);
			this.lblPassword.TabIndex = 2;
			this.lblPassword.Text = "Password";
			// 
			// txtPassword
			// 
			this.txtPassword.Location = new System.Drawing.Point(64, 55);
			this.txtPassword.Name = "txtPassword";
			this.txtPassword.PasswordChar = '*';
			this.txtPassword.Size = new System.Drawing.Size(112, 20);
			this.txtPassword.TabIndex = 3;
			// 
			// lblUser
			// 
			this.lblUser.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblUser.Location = new System.Drawing.Point(8, 27);
			this.lblUser.Name = "lblUser";
			this.lblUser.Size = new System.Drawing.Size(48, 14);
			this.lblUser.TabIndex = 0;
			this.lblUser.Text = "User";
			// 
			// txtUserName
			// 
			this.txtUserName.Location = new System.Drawing.Point(64, 24);
			this.txtUserName.Name = "txtUserName";
			this.txtUserName.Size = new System.Drawing.Size(112, 20);
			this.txtUserName.TabIndex = 1;
			// 
			// cmdGetFile
			// 
			this.cmdGetFile.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.cmdGetFile.Location = new System.Drawing.Point(411, 64);
			this.cmdGetFile.Name = "cmdGetFile";
			this.cmdGetFile.Size = new System.Drawing.Size(24, 23);
			this.cmdGetFile.TabIndex = 9;
			this.cmdGetFile.Text = "...";
			this.cmdGetFile.Click += new System.EventHandler(this.cmdGetFile_Click);
			// 
			// txtDatabase
			// 
			this.txtDatabase.Location = new System.Drawing.Point(8, 64);
			this.txtDatabase.Name = "txtDatabase";
			this.txtDatabase.Size = new System.Drawing.Size(392, 20);
			this.txtDatabase.TabIndex = 8;
			// 
			// lblDatabase
			// 
			this.lblDatabase.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblDatabase.Location = new System.Drawing.Point(8, 48);
			this.lblDatabase.Name = "lblDatabase";
			this.lblDatabase.Size = new System.Drawing.Size(48, 16);
			this.lblDatabase.TabIndex = 7;
			this.lblDatabase.Text = "Database";
			// 
			// txtPort
			// 
			this.txtPort.Location = new System.Drawing.Point(152, 24);
			this.txtPort.Name = "txtPort";
			this.txtPort.Size = new System.Drawing.Size(40, 20);
			this.txtPort.TabIndex = 2;
			// 
			// label1
			// 
			this.label1.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.label1.Location = new System.Drawing.Point(152, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(88, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "Data Source Port";
			// 
			// lblDialect
			// 
			this.lblDialect.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblDialect.Location = new System.Drawing.Point(248, 8);
			this.lblDialect.Name = "lblDialect";
			this.lblDialect.Size = new System.Drawing.Size(48, 16);
			this.lblDialect.TabIndex = 3;
			this.lblDialect.Text = "Dialect";
			// 
			// lblCharset
			// 
			this.lblCharset.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.lblCharset.Location = new System.Drawing.Point(320, 8);
			this.lblCharset.Name = "lblCharset";
			this.lblCharset.Size = new System.Drawing.Size(48, 16);
			this.lblCharset.TabIndex = 5;
			this.lblCharset.Text = "Charset";
			// 
			// cboCharset
			// 
			this.cboCharset.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cboCharset.Items.AddRange(new object[] {
            "NONE",
            "ASCII",
            "BIG_5Big5",
            "DOS437",
            "DOS850",
            "DOS860",
            "DOS861",
            "DOS863",
            "DOS865",
            "EUCJ_0208",
            "GB_2312",
            "ISO8859_1",
            "ISO8859_2",
            "KSC_5601",
            "ISO2022-JP",
            "SJIS_0208",
            "UNICODE_FSS",
            "WIN1250",
            "WIN1251",
            "WIN1252",
            "WIN1253",
            "WIN1254",
            "WIN1257"});
			this.cboCharset.Location = new System.Drawing.Point(320, 24);
			this.cboCharset.Name = "cboCharset";
			this.cboCharset.TabIndex = 6;
			// 
			// openFirebirdDB
			// 
			this.openFirebirdDB.Filter = "Firebird Database(*.FDB)|*.FDB|Firebird/Interbase\tDatabase (*.GDB)|*.GDB";
			// 
			// FbConnectionStringEditor
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(490, 269);
			this.Controls.Add(this.cmdCancel);
			this.Controls.Add(this.cmdAccept);
			this.Controls.Add(this.cmdTest);
			this.Controls.Add(this.panel1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FbConnectionStringEditor";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Connection\tString Editor";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.grbSettings.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.txtLifeTime)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.txtTimeout)).EndInit();
			this.grbLogin.ResumeLayout(false);
			this.grbLogin.PerformLayout();
			this.ResumeLayout(false);

		}
		#endregion

		private void cmdAccept_Click(object sender, System.EventArgs e)
		{
			try
			{
				this.connection.ConnectionString = this.GetConnectionString();
			}
			catch
			{
				MessageBox.Show("Invalid Connection String.");
			}
		}

		private void cmdTest_Click(object sender, System.EventArgs e)
		{
			try
			{
				connection.ConnectionString = this.GetConnectionString();
				connection.Open();

				MessageBox.Show("Connection successful.", "Connection String Editor");
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);
			}
			finally
			{
				connection.Close();
			}
		}

		private void cmdGetFile_Click(object sender, System.EventArgs e)
		{
			DialogResult result = this.openFirebirdDB.ShowDialog();

			if (result == DialogResult.OK)
			{
				this.txtDatabase.Text = this.openFirebirdDB.FileName;
			}
		}

		public string GetConnectionString()
		{
			System.Text.StringBuilder cs = new System.Text.StringBuilder();
			string csTemplate = "User={0};Password={1};Database={2};DataSource={3};Port={4};Dialect={5};Charset={6};Role={7};Connection lifetime={8};Connection timeout={9};Pooling={10};Packet Size={11};Server Type={12}";

			int serverType = 0;

			if (this.cboServerType.Text == "Embedded Server")
			{
				serverType = 1;
			}

			cs.AppendFormat(
				CultureInfo.CurrentCulture,
				csTemplate,
				this.txtUserName.Text,
				this.txtPassword.Text,
				this.txtDatabase.Text,
				this.txtDataSource.Text,
				this.txtPort.Text,
				this.cboDialect.Text,
				this.cboCharset.Text,
				this.txtRole.Text,
				this.txtLifeTime.Text,
				this.txtTimeout.Text,
				this.chkPooling.Checked.ToString(),
				this.cboPacketSize.Text,
				serverType);

			return cs.ToString();
		}
	}
}

#endif