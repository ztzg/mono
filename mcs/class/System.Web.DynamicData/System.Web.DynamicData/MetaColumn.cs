//
// MetaColumn.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2008 Novell Inc. http://novell.com
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
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Globalization;
using System.Reflection;
using System.Security.Permissions;
using System.Security.Principal;
using System.Web.Caching;
using System.Web.DynamicData.ModelProviders;

namespace System.Web.DynamicData
{
	[AspNetHostingPermission (SecurityAction.InheritanceDemand, Level = AspNetHostingPermissionLevel.Minimal)]
	[AspNetHostingPermission (SecurityAction.LinkDemand, Level = AspNetHostingPermissionLevel.Minimal)]
	public class MetaColumn : IFieldFormattingOptions
	{
		internal MetaColumn (MetaTable table, ColumnProvider provider)
		{
			Table = table;
			Provider = provider;
		}

		[MonoTODO]
		public bool ApplyFormatInEditMode { get; private set; }

		[MonoTODO]
		public AttributeCollection Attributes { get; private set; }

		public Type ColumnType {
			get { return Provider.ColumnType; }
		}

		[MonoTODO]
		public bool ConvertEmptyStringToNull { get; private set; }

		[MonoTODO]
		public string DataFormatString { get; private set; }

		[MonoTODO]
		public DataTypeAttribute DataTypeAttribute { get; internal set; }

		[MonoTODO]
		public Object DefaultValue { get; private set; }

		[MonoTODO]
		public string Description { get; private set; }

		[MonoTODO]
		public string DisplayName { get; private set; }

		public PropertyInfo EntityTypeProperty {
			get { return Provider.EntityTypeProperty; }
		}

		[MonoTODO]
		public bool HtmlEncode { get; private set; }

		[MonoTODO]
		public bool IsBinaryData { get; private set; }

		public bool IsCustomProperty {
			get { return Provider.IsCustomProperty; }
		}

		[MonoTODO]
		public bool IsFloatingPoint { get; private set; }

		public bool IsForeignKeyComponent {
			get { return Provider.IsForeignKeyComponent; }
		}

		public bool IsGenerated {
			get { return Provider.IsGenerated; }
		}

		[MonoTODO]
		public bool IsInteger { get; private set; }

		[MonoTODO]
		public bool IsLongString { get; private set; }

		public bool IsPrimaryKey {
			get { return Provider.IsPrimaryKey; }
		}

		[MonoTODO]
		public bool IsReadOnly { get; private set; }

		[MonoTODO]
		public bool IsRequired { get; private set; }

		[MonoTODO]
		public bool IsString { get; private set; }

		public int MaxLength {
			get { return Provider.MaxLength; }
		}

		public MetaModel Model { get; private set; }

		public string Name {
			get { return Provider.Name; }
		}

		[MonoTODO]
		public string NullDisplayText { get; private set; }

		public ColumnProvider Provider { get; private set; }

		[MonoTODO]
		public string RequiredErrorMessage { get; private set; }

		[MonoTODO]
		public bool Scaffold { get; set; }

		[MonoTODO]
		public string SortExpression { get; private set; }

		public MetaTable Table { get; private set; }

		[MonoTODO]
		public TypeCode TypeCode { get; private set; }

		[MonoTODO]
		public string UIHint { get; private set; }


		[MonoTODO]
		public override string ToString ()
		{
			return base.ToString ();
		}
	}
}
