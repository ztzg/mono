//
// BuildItem.cs:
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
using System.Collections.Generic;
using System.Collections.Specialized;
using System.IO;
using System.Text;
using System.Xml;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using Mono.XBuild.Utilities;

namespace Microsoft.Build.BuildEngine {
	public class BuildItem {

		BuildItemGroup	child_items;
		XmlElement	itemElement;
		string		finalItemSpec;
		bool		isImported;
		string		itemInclude;
		string		name;
		BuildItemGroup	parent_item_group;
		BuildItem	parent_item;
		//string		recursiveDir;
		IDictionary	evaluatedMetadata;
		IDictionary	unevaluatedMetadata;

		BuildItem ()
		{
		}
		
		public BuildItem (string itemName, ITaskItem taskItem)
		{
			if (taskItem == null)
				throw new ArgumentNullException ("taskItem");

			this.name = itemName;
			this.finalItemSpec = taskItem.ItemSpec;
			this.itemInclude = Utilities.Escape (taskItem.ItemSpec);
			this.evaluatedMetadata = (Hashtable) taskItem.CloneCustomMetadata ();
			this.unevaluatedMetadata = (Hashtable) taskItem.CloneCustomMetadata ();
		}

		public BuildItem (string itemName, string itemInclude)
		{
			if (itemInclude == null)
				throw new ArgumentNullException ("itemInclude");
			if (itemInclude == String.Empty)
				throw new ArgumentException ("Parameter \"itemInclude\" cannot have zero length.");

			name = itemName;
			finalItemSpec = itemInclude;
			this.itemInclude = itemInclude;
			unevaluatedMetadata = CollectionsUtil.CreateCaseInsensitiveHashtable ();
			evaluatedMetadata = CollectionsUtil.CreateCaseInsensitiveHashtable ();
		}
		
		internal BuildItem (XmlElement itemElement, BuildItemGroup parentItemGroup)
		{
			child_items = new BuildItemGroup ();
			isImported = parentItemGroup.IsImported;
			unevaluatedMetadata = CollectionsUtil.CreateCaseInsensitiveHashtable ();
			evaluatedMetadata = CollectionsUtil.CreateCaseInsensitiveHashtable ();
			this.parent_item_group = parentItemGroup;
			
			this.itemElement = itemElement;
			
			if (Include == String.Empty)
				throw new InvalidProjectFileException (String.Format ("The required attribute \"Include\" is missing from element <{0}>.", Name));
		}
		
		BuildItem (BuildItem parent)
		{
			isImported = parent.isImported;
			name = parent.Name;
			parent_item = parent;
			parent_item.child_items.AddItem (this);
			parent_item_group = parent.parent_item_group;
			unevaluatedMetadata = CollectionsUtil.CreateCaseInsensitiveHashtable (parent.unevaluatedMetadata);
			evaluatedMetadata = CollectionsUtil.CreateCaseInsensitiveHashtable (parent.evaluatedMetadata);
		}
		
		public void CopyCustomMetadataTo (BuildItem destinationItem)
		{
			if (destinationItem == null)
				throw new ArgumentNullException ("destinationItem");

			foreach (DictionaryEntry de in unevaluatedMetadata)
				destinationItem.AddMetadata ((string) de.Key, (string) de.Value);
		}
		
		[MonoTODO]
		public BuildItem Clone ()
		{
			return (BuildItem) this.MemberwiseClone ();
		}

		public string GetEvaluatedMetadata (string metadataName)
		{
			if (ReservedNameUtils.IsReservedMetadataName (metadataName)) {
				string metadata = ReservedNameUtils.GetReservedMetadata (FinalItemSpec, metadataName);
				return (metadataName.ToLower () == "fullpath") ? Utilities.Escape (metadata) : metadata;
			}

			if (evaluatedMetadata.Contains (metadataName))
				return (string) evaluatedMetadata [metadataName];
			else
				return String.Empty;
		}

		public string GetMetadata (string metadataName)
		{
			if (ReservedNameUtils.IsReservedMetadataName (metadataName)) {
				string metadata = ReservedNameUtils.GetReservedMetadata (FinalItemSpec, metadataName);
				return (metadataName.ToLower () == "fullpath") ? Utilities.Escape (metadata) : metadata;
			} else if (unevaluatedMetadata.Contains (metadataName))
				return (string) unevaluatedMetadata [metadataName];
			else
				return String.Empty;
		}
		
		public bool HasMetadata (string metadataName)
		{
			if (ReservedNameUtils.IsReservedMetadataName (metadataName))
				return true;
			else
				return evaluatedMetadata.Contains (metadataName);
		}

		public void RemoveMetadata (string metadataName)
		{
			if (metadataName == null)
				throw new ArgumentNullException ("metadataName");
			
			if (ReservedNameUtils.IsReservedMetadataName (metadataName))
				throw new ArgumentException (String.Format ("\"{0}\" is a reserved item meta-data, and cannot be modified or deleted.",
					metadataName));

			if (FromXml) {
				if (unevaluatedMetadata.Contains (metadataName)) {
					XmlNode node = itemElement [metadataName];
					itemElement.RemoveChild (node);
				}
			} else if (HasParent) {
				if (parent_item.child_items.Count > 1)
					SplitParentItem ();
				parent_item.RemoveMetadata (metadataName);
			} 
			
			DeleteMetadata (metadataName);
		}

		public void SetMetadata (string metadataName,
					 string metadataValue)
		{
			SetMetadata (metadataName, metadataValue, false);
		}
		
		public void SetMetadata (string metadataName,
					 string metadataValue,
					 bool treatMetadataValueAsLiteral)
		{
			if (metadataName == null)
				throw new ArgumentNullException ("metadataName");
			
			if (metadataValue == null)
				throw new ArgumentNullException ("metadataValue");
			
			if (ReservedNameUtils.IsReservedMetadataName (metadataName))
				throw new ArgumentException (String.Format ("\"{0}\" is a reserved item meta-data, and cannot be modified or deleted.",
					metadataName));

			if (treatMetadataValueAsLiteral && !HasParent)
				metadataValue = Utilities.Escape (metadataValue);

			if (FromXml) {
				XmlElement element = itemElement [metadataName];
				if (element == null) {
					element = itemElement.OwnerDocument.CreateElement (metadataName, Project.XmlNamespace);
					element.InnerText = metadataValue;
					itemElement.AppendChild (element);
				} else
					element.InnerText = metadataValue;
			} else if (HasParent) {
				if (parent_item.child_items.Count > 1)
					SplitParentItem ();
				parent_item.SetMetadata (metadataName, metadataValue, treatMetadataValueAsLiteral);
			}
			if (FromXml || HasParent) {
				parent_item_group.ParentProject.MarkProjectAsDirty ();
				parent_item_group.ParentProject.NeedToReevaluate ();
			}
			
			DeleteMetadata (metadataName);
			AddMetadata (metadataName, metadataValue);
		}

		void AddMetadata (string name, string value)
		{
			if (parent_item_group != null) {
				Expression e = new Expression ();
				e.Parse (value, true);
				evaluatedMetadata.Add (name, (string) e.ConvertTo (parent_item_group.ParentProject, typeof (string)));
			} else
				evaluatedMetadata.Add (name, Utilities.Unescape (value));
				
				unevaluatedMetadata.Add (name, value);
		}

		void DeleteMetadata (string name)
		{
			if (evaluatedMetadata.Contains (name))
				evaluatedMetadata.Remove (name);
			
			if (unevaluatedMetadata.Contains (name))
				unevaluatedMetadata.Remove (name);
		}

		internal void Evaluate (Project project, bool evaluatedTo)
		{
			// FIXME: maybe make Expression.ConvertTo (null, ...) work as Utilities.Unescape ()?
			if (project == null) {
				this.finalItemSpec = Utilities.Unescape (Include);
				return;
			}
			
			foreach (XmlElement xe in itemElement.ChildNodes)
				AddMetadata (xe.Name, xe.InnerText);

			DirectoryScanner directoryScanner;
			Expression includeExpr, excludeExpr;
			string includes, excludes;

			includeExpr = new Expression ();
			includeExpr.Parse (Include, true);
			excludeExpr = new Expression ();
			excludeExpr.Parse (Exclude, true);
			
			includes = (string) includeExpr.ConvertTo (project, typeof (string));
			excludes = (string) excludeExpr.ConvertTo (project, typeof (string));

			this.finalItemSpec = includes;
			
			directoryScanner = new DirectoryScanner ();
			
			directoryScanner.Includes = includes;
			directoryScanner.Excludes = excludes;

			if (project.FullFileName != String.Empty)
				directoryScanner.BaseDirectory = new DirectoryInfo (Path.GetDirectoryName (project.FullFileName));
			else
				directoryScanner.BaseDirectory = new DirectoryInfo (Directory.GetCurrentDirectory ());
			
			directoryScanner.Scan ();
			
			foreach (string matchedFile in directoryScanner.MatchedFilenames)
				AddEvaluatedItem (project, evaluatedTo, matchedFile);
		}
		
		void AddEvaluatedItem (Project project, bool evaluatedTo, string itemSpec)
		{
			BuildItemGroup big;			
			BuildItem bi = new BuildItem (this);
			bi.finalItemSpec = itemSpec;

			if (evaluatedTo) {
				project.EvaluatedItems.AddItem (bi);
	
				if (!project.EvaluatedItemsByName.ContainsKey (bi.Name)) {
					big = new BuildItemGroup (null, project, null, true);
					project.EvaluatedItemsByName.Add (bi.Name, big);
				} else {
					big = project.EvaluatedItemsByName [bi.Name];
				}

				big.AddItem (bi);
			}

			if (!project.EvaluatedItemsByNameIgnoringCondition.ContainsKey (bi.Name)) {
				big = new BuildItemGroup (null, project, null, true);
				project.EvaluatedItemsByNameIgnoringCondition.Add (bi.Name, big);
			} else {
				big = project.EvaluatedItemsByNameIgnoringCondition [bi.Name];
			}

			big.AddItem (bi);
		}
		
		internal string ConvertToString (Expression transform)
		{
			return GetItemSpecFromTransform (transform);
		}
		
		internal ITaskItem ConvertToITaskItem (Expression transform)
		{
			TaskItem taskItem;
			taskItem = new TaskItem (GetItemSpecFromTransform (transform), evaluatedMetadata);
			return taskItem;
		}

		internal void Detach ()
		{
			if (FromXml)
				itemElement.ParentNode.RemoveChild (itemElement);
			else if (HasParent) {
				if (parent_item.child_items.Count > 1)
					SplitParentItem ();
				parent_item.Detach ();
			}
		}

		string GetItemSpecFromTransform (Expression transform)
		{
			StringBuilder sb;
		
			if (transform == null)
				return finalItemSpec;
			else {
				sb = new StringBuilder ();
				foreach (object o in transform.Collection) {
					if (o is string) {
						sb.Append ((string)o);
					} else if (o is PropertyReference) {
						sb.Append (((PropertyReference)o).ConvertToString (parent_item_group.ParentProject));
					} else if (o is ItemReference) {
						sb.Append (((ItemReference)o).ConvertToString (parent_item_group.ParentProject));
					} else if (o is MetadataReference) {
						sb.Append (GetMetadata (((MetadataReference)o).MetadataName));
					}
				}
				return sb.ToString ();
			}
		}

		void SplitParentItem ()
		{
			BuildItem parent = parent_item;
			List <BuildItem> list = new List <BuildItem> ();
			XmlElement insertAfter = parent.itemElement;
			foreach (BuildItem bi in parent.child_items) {
				BuildItem added = InsertElementAfter (parent, bi, insertAfter);
				insertAfter = added.itemElement;
				list.Add (added);
			}
			parent.parent_item_group.ReplaceWith (parent, list);
			parent.itemElement.ParentNode.RemoveChild (parent.itemElement);			
		}

		static BuildItem InsertElementAfter (BuildItem parent, BuildItem child, XmlElement insertAfter)
		{
			BuildItem newParent;

			XmlDocument doc = parent.itemElement.OwnerDocument;
			XmlElement newElement = doc.CreateElement (child.Name, Project.XmlNamespace);
			newElement.SetAttribute ("Include", child.FinalItemSpec);
			if (parent.itemElement.HasAttribute ("Condition"))
				newElement.SetAttribute ("Condition", parent.itemElement.GetAttribute ("Condition"));
			foreach (XmlElement xe in parent.itemElement)
				newElement.AppendChild (xe.Clone ());
			parent.itemElement.ParentNode.InsertAfter (newElement, insertAfter);

			newParent = new BuildItem (newElement, parent.parent_item_group);
			newParent.child_items.AddItem (child);
			child.parent_item = newParent;

			return newParent;
		}

		public string Condition {
			get {
				if (FromXml)
					return itemElement.GetAttribute ("Condition");
				else
					return String.Empty;
			}
			set {
				if (FromXml)
					itemElement.SetAttribute ("Condition", value);
				else if (!HasParent)
					throw new InvalidOperationException ("Cannot set a condition on an object not represented by an XML element in the project file.");
			}
		}

		public string Exclude {
			get {
				if (FromXml)
					return itemElement.GetAttribute ("Exclude");
				else
					return String.Empty;
			}
			set {
				if (FromXml)
					itemElement.SetAttribute ("Exclude", value);
				else
					throw new InvalidOperationException ("Assigning the \"Exclude\" attribute of a virtual item is not allowed.");
			}
		}

		public string FinalItemSpec {
			get { return finalItemSpec; }
		}

		public string Include {
			get {
				if (FromXml)
					return itemElement.GetAttribute ("Include");
				else if (HasParent)
					return parent_item.Include;
				else
					return itemInclude;
			}
			set {
				if (FromXml)
					itemElement.SetAttribute ("Include", value);
				else if (HasParent) {
					if (parent_item.child_items.Count > 1)
						SplitParentItem ();
					parent_item.Include = value;
				} else
					itemInclude = value;
			}
		}

		public bool IsImported {
			get { return isImported; }
		}

		public string Name {
			get {
				if (FromXml)
					return itemElement.Name;
				else if (HasParent)
					return parent_item.Name;
				else
					return name;
			}
			set {
				if (FromXml) {
					XmlElement newElement = itemElement.OwnerDocument.CreateElement (value, Project.XmlNamespace);
					newElement.SetAttribute ("Include", itemElement.GetAttribute ("Include"));
					newElement.SetAttribute ("Condition", itemElement.GetAttribute ("Condition"));
					foreach (XmlElement xe in itemElement)
						newElement.AppendChild (xe.Clone ());
					itemElement.ParentNode.ReplaceChild (newElement, itemElement);
					itemElement = newElement;
				} else if (HasParent) {
					if (parent_item.child_items.Count > 1)
						SplitParentItem ();
					parent_item.Name = value;
				} else
					name = value;
			}
		}
		
		internal bool FromXml {
			get { return itemElement != null; }
		}
		
		internal bool HasParent {
			get { return parent_item != null; }
		}

		internal BuildItem ParentItem {
			get { return parent_item; }
		}

		internal BuildItemGroup ParentItemGroup {
			get { return parent_item_group; }
		}
	}
}

#endif
