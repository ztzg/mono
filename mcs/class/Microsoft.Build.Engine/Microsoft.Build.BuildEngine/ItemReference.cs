//
// ItemReference.cs: Represents "@(Reference)" in expression.
//
// Author:
//   Marek Sieradzki (marek.sieradzki@gmail.com)
// 
// (C) 2005 Marek Sieradzki
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

#if NET_2_0

using System;
using System.Collections;
using Microsoft.Build.Framework;

namespace Microsoft.Build.BuildEngine {
	internal class ItemReference : IReference {
	
		string		itemName;
		Expression	transform;
		Expression	separator;
		int		start;
		int		length;
		
		public ItemReference (string itemName, string transform, string separator, int start, int length)
		{
			this.itemName = itemName;
			this.start = start;
			this.length = length;

			if (transform != null) {
				this.transform = new Expression ();
				this.transform.Parse (transform, false);
			}

			if (separator != null) {
				this.separator = new Expression ();
				this.separator.Parse (separator, false);
			}
		}
		
		public string ConvertToString (Project project)
		{
			BuildItemGroup group;
			if (project.TryGetEvaluatedItemByNameBatched (itemName, out group))
				return group.ConvertToString (transform, separator);
			else
				return String.Empty;
		}
		
		public ITaskItem [] ConvertToITaskItemArray (Project project)
		{
			BuildItemGroup group;
			if (project.TryGetEvaluatedItemByNameBatched (itemName, out group))
				return group.ConvertToITaskItemArray (transform);
			else
				return null;

		}

		public string ItemName {
			get { return itemName; }
		}

		public Expression Transform {
			get { return transform; }
		}

		public Expression Separator {
			get { return separator; }
		}

		public int Start {
			get { return start; }
		}

		public int End {
			get { return start + length - 1; }
		}

		public override string ToString ()
		{
			//FIXME: transform
			return "@" + itemName;
		}
	}
}

#endif
