//
// MSBuild.cs: Task that can run .*proj files
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
using System.IO;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace Microsoft.Build.Tasks {

	[MonoTODO]
	public class MSBuild : TaskExtension {
	
		ITaskItem []	projects;
		string []	properties;
		bool		rebaseOutputs;
		bool		runEachTargetSeparately;
		bool		stopOnFirstFailure;
		ITaskItem []	targetOutputs;
		string []	targets;
	
		public MSBuild ()
		{
		}

		public override bool Execute ()
		{
			string filename;
			bool result = true;
			stopOnFirstFailure = false;
			List <ITaskItem > outputItems = new List <ITaskItem> ();
			string currentDirectory = Environment.CurrentDirectory;
			Hashtable outputs;
		
			foreach (ITaskItem project in projects) {
				filename = project.GetMetadata ("FullPath");

				Directory.SetCurrentDirectory (Path.GetDirectoryName (filename));
				outputs = new Hashtable ();

				Dictionary<string, string> global_properties = SplitPropertiesToDictionary ();
				result = BuildEngine.BuildProjectFile (filename, targets, global_properties, outputs);

				if (result) {
					foreach (DictionaryEntry de in outputs) {
						ITaskItem [] array = (ITaskItem []) de.Value;
						foreach (ITaskItem item in array) {
							outputItems.Add (item);
							//FIXME: Correctly rebase output paths to be relative to the
							//	 calling project
							//if (rebaseOutputs)
							//	File.Copy (item.ItemSpec, Path.Combine (currentDirectory, item.ItemSpec), true);
						}
					}
				} else {
					Log.LogError ("Error while building {0}", filename);
					if (stopOnFirstFailure)
						break;
				}

				Directory.SetCurrentDirectory (currentDirectory);
			}

			if (result)
				targetOutputs = outputItems.ToArray ();

			Directory.SetCurrentDirectory (currentDirectory);
			return result;
		}

		[Required]
		public ITaskItem [] Projects {
			get { return projects; }
			set { projects = value; }
		}

		[MonoTODO]
		public string [] Properties {
			get { return properties; }
			set { properties = value; }
		}

		public bool RebaseOutputs {
			get { return rebaseOutputs; }
			set { rebaseOutputs = value; }
		}

		[MonoTODO]
		public bool RunEachTargetSeparately {
			get { return runEachTargetSeparately; }
			set { runEachTargetSeparately = value; }
		}

		public bool StopOnFirstFailure {
			get { return stopOnFirstFailure; }
			set { stopOnFirstFailure = value; }
		}

		[Output]
		public ITaskItem [] TargetOutputs {
			get { return targetOutputs; }
		}

		public string [] Targets {
			get { return targets; }
			set { targets = value; }
		}

		Dictionary<string, string> SplitPropertiesToDictionary ()
		{
			if (properties == null)
				return null;

			Dictionary<string, string> global_properties = new Dictionary<string, string> ();
			foreach (string kvpair in properties) {
				if (String.IsNullOrEmpty (kvpair))
					continue;

				string [] parts = kvpair.Trim ().Split (new char [] {'='}, 2);
				if (parts.Length != 2) {
					Log.LogWarning ("Invalid key/value pairs ({0}) in Properties, ignoring.", kvpair);
					continue;
				}

				global_properties.Add (parts [0], parts [1]);
			}

			return global_properties;
		}

	}
}

#endif
