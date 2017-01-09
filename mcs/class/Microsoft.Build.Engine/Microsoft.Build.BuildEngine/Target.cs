//
// Target.cs: Represents a target.
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
using System.Xml;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace Microsoft.Build.BuildEngine {
	public class Target : IEnumerable {
	
		TargetBatchingImpl batchingImpl;
		BuildState	buildState;
		Engine		engine;
		ImportedProject	importedProject;
		string		name;
		Project		project;
		XmlElement	targetElement;
		List <XmlElement>	onErrorElements;
		List <BuildTask>	buildTasks;
		
		internal Target (XmlElement targetElement, Project project, ImportedProject importedProject)
		{
			if (project == null)
				throw new ArgumentNullException ("project");
			if (targetElement == null)
				throw new ArgumentNullException ("targetElement");

			this.targetElement = targetElement;
			this.name = targetElement.GetAttribute ("Name");

			this.project = project;
			this.engine = project.ParentEngine;
			this.importedProject = importedProject;

			this.onErrorElements  = new List <XmlElement> ();
			this.buildState = BuildState.NotStarted;
			this.buildTasks = new List <BuildTask> ();
			this.batchingImpl = new TargetBatchingImpl (project, this.targetElement);

			bool onErrorFound = false;
			foreach (XmlNode xn in targetElement.ChildNodes) {
				if (xn is XmlElement) {
					XmlElement xe = (XmlElement) xn;
					if (xe.Name == "OnError") {
						onErrorElements.Add (xe);
						onErrorFound = true;
					} else if (onErrorFound)
						throw new InvalidProjectFileException (
							"The element <OnError> must be last under element <Target>. Found element <Error> instead.");
					else
						buildTasks.Add (new BuildTask (xe, this));
				}
			}
		}
		
		[MonoTODO]
		public BuildTask AddNewTask (string taskName)
		{
			if (taskName == null)
				throw new ArgumentNullException ("taskName");
		
			XmlElement task = project.XmlDocument.CreateElement (taskName, Project.XmlNamespace);
			targetElement.AppendChild (task);
			BuildTask bt = new BuildTask (task, this);
			buildTasks.Add (bt);
			
			return bt;
		}

		public IEnumerator GetEnumerator ()
		{
			foreach (BuildTask bt in buildTasks)
				yield return bt;
		}

		// FIXME: shouldn't we remove it from XML?
		public void RemoveTask (BuildTask buildTask)
		{
			if (buildTask == null)
				throw new ArgumentNullException ("buildTask");
			buildTasks.Remove (buildTask);
		}
		
		// FIXME: log errors instead of throwing exceptions
		internal bool Build ()
		{
			bool deps;
			bool result;

			// log that target is being skipped
			if (!ConditionParser.ParseAndEvaluate (Condition, Project))
				return true;

			try {
				buildState = BuildState.Started;
				deps = BuildDependencies (GetDependencies ());

				result = deps ? DoBuild () : false;

				buildState = BuildState.Finished;
			// FIXME: log it 
			} catch (Exception e) {
				LogError ("Error building target {0}: {1}", Name, e.ToString ());
				return false;
			}

			return result;
		}

		List <Target> GetDependencies ()
		{
			List <Target> list = new List <Target> ();
			Target t;
			string [] targetNames;
			Expression deps;

			if (DependsOnTargets != String.Empty) {
				deps = new Expression ();
				deps.Parse (DependsOnTargets, true);
				targetNames = (string []) deps.ConvertTo (Project, typeof (string []));
				foreach (string name in targetNames) {
					t = project.Targets [name.Trim ()];
					if (t == null)
						throw new InvalidProjectFileException (String.Format ("Target '{0}' not found.", name.Trim ()));
					list.Add (t);
				}
			}
			return list;
		}

		bool BuildDependencies (List <Target> deps)
		{
			foreach (Target t in deps) {
				if (t.BuildState == BuildState.NotStarted)
					if (!t.Build ())
						return false;
				if (t.BuildState == BuildState.Started)
					throw new InvalidProjectFileException ("Cycle in target dependencies detected");
			}

			return true;
		}
		
		bool DoBuild ()
		{
			bool executeOnErrors;
			bool result = true;

			if (BuildTasks.Count == 0)
				// nothing to do
				return true;
		
			try {
				result = batchingImpl.Build (this, out executeOnErrors);
			} catch (Exception e) {
				LogError ("Error building target {0}: {1}", Name, e.ToString ());
				throw;
			}

			if (executeOnErrors == true)
				ExecuteOnErrors ();
				
			return result;
		}
		
		void ExecuteOnErrors ()
		{
			foreach (XmlElement onError in onErrorElements) {
				// FIXME: add condition
				if (onError.GetAttribute ("ExecuteTargets") == String.Empty)
					throw new InvalidProjectFileException ("ExecuteTargets attribute is required in OnError element.");
				string[] targetsToExecute = onError.GetAttribute ("ExecuteTargets").Split (';');
				foreach (string t in targetsToExecute)
					this.project.Targets [t].Build ();
			}
		}

		void LogError (string message, params object [] messageArgs)
		{
			if (message == null)
				throw new ArgumentException ("message");

			BuildErrorEventArgs beea = new BuildErrorEventArgs (
				null, null, null, 0, 0, 0, 0, String.Format (message, messageArgs),
				null, null);
			engine.EventSource.FireErrorRaised (this, beea);
		}
	
		public string Condition {
			get { return targetElement.GetAttribute ("Condition"); }
			set { targetElement.SetAttribute ("Condition", value); }
		}

		public string DependsOnTargets {
			get { return targetElement.GetAttribute ("DependsOnTargets"); }
			set { targetElement.SetAttribute ("DependsOnTargets", value); }
		}

		public bool IsImported {
			get { return importedProject != null; }
		}

		public string Name {
			get { return name; }
		}
		
		internal Project Project {
			get { return project; }
		}

		internal List<BuildTask> BuildTasks {
			get { return buildTasks; }
		}

		internal Engine Engine {
			get { return engine; }
		}
		
		internal BuildState BuildState {
			get { return buildState; }
		}

		internal ITaskItem [] Outputs {
			get {
				string outputs = targetElement.GetAttribute ("Outputs");
				if (outputs == String.Empty)
					return new ITaskItem [0];

				Expression e = new Expression ();
				e.Parse (outputs, true);

				string [] outputFiles = (string []) e.ConvertTo (project, typeof (string []));
				ITaskItem [] outputItems = new ITaskItem [outputFiles.Length];
				for (int i = 0; i < outputFiles.Length; i++)
					outputItems [i] = new TaskItem (outputFiles [i]);
				return outputItems;
			}
		}
	}
	
	internal enum BuildState {
		NotStarted,
		Started,
		Finished,
		Skipped
	}
}

#endif
