//
// DynamicDataExtensions.cs
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
using System.Globalization;
using System.Security.Permissions;
using System.Security.Principal;
using System.Web.Caching;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.DynamicData.ModelProviders;

namespace System.Web.DynamicData
{
	[AspNetHostingPermission (SecurityAction.LinkDemand, Level = AspNetHostingPermissionLevel.Minimal)]
	public static class DynamicDataExtensions
	{
		[MonoTODO]
		public static object ConvertEditedValue (this IFieldFormattingOptions formattingOptions, string value)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static void EnablePersistedSelection (this BaseDataBoundControl dataBoundControl)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static void ExpandDynamicWhereParameters (this IDynamicDataSource dataSource)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static IDynamicDataSource FindDataSourceControl (this Control current)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static Control FindFieldTemplate (this Control control, string columnName)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static MetaTable FindMetaTable (this Control current)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static string FormatEditValue (this IFieldFormattingOptions formattingOptions, object fieldValue)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static string FormatValue (this IFieldFormattingOptions formattingOptions, object fieldValue)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static MetaTable GetTable (this IDynamicDataSource dataSource)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public static void LoadWithForeignKeys (this LinqDataSource dataSource, Type rowType)
		{
			throw new NotImplementedException ();
		}
	}
}
