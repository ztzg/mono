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
using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace FirebirdSql.Data.Firebird.Design
{
	internal class FbConnectionStringUIEditor : UITypeEditor
	{
		#region Fields

		private IWindowsFormsEditorService edSvc;

		#endregion

		#region Protected Methods

		protected virtual void SetEditorProps(FbConnection editingInstance, FbConnection editor)
		{
			if (editingInstance != null &&
				editingInstance.ConnectionString != null &&
				editingInstance.ConnectionString.Length > 0)
			{
				editor.ConnectionString = editingInstance.ConnectionString;
			}
		}

		#endregion

		#region Methods

		public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
		{
			if (context != null && context.Instance != null &&
				provider != null)
			{
				edSvc = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));

				if (edSvc != null)
				{
					FbConnection connection = new FbConnection();
					SetEditorProps((FbConnection)context.Instance, connection);

					FbConnectionStringEditor editor = new FbConnectionStringEditor(connection);
					edSvc.ShowDialog(editor);

					if (editor.DialogResult == DialogResult.OK)
					{
						value = connection.ConnectionString;
					}
				}
			}

			return value;
		}

		public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
		{
			if (context != null && context.Instance != null)
			{
				return UITypeEditorEditStyle.Modal;
			}
			return base.GetEditStyle(context);
		}

		public override bool GetPaintValueSupported(ITypeDescriptorContext context)
		{
			return false;
		}

		#endregion
	}
}

#endif