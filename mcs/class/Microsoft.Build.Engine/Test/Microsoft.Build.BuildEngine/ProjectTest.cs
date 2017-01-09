//
// ProjectTest.cs:
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

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using Microsoft.Build.BuildEngine;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using NUnit.Framework;
using System.Text;

namespace MonoTests.Microsoft.Build.BuildEngine {

	class TestLogger : Logger {
		int target_started_events = 0;
		int target_finished_events = 0;

		public override void Initialize (IEventSource eventSource)
		{
			eventSource.TargetStarted += new TargetStartedEventHandler(TargetStarted);
			eventSource.TargetFinished += new TargetFinishedEventHandler(TargetFinished);
			eventSource.MessageRaised += new BuildMessageEventHandler(Message);
			eventSource.WarningRaised += new BuildWarningEventHandler(Warning);
		}

		void TargetStarted (object sender, TargetStartedEventArgs args)
		{
			target_started_events++;
		}

		void TargetFinished (object sender, TargetFinishedEventArgs args)
		{
			target_finished_events++;
		}

		void Message (object sender, BuildMessageEventArgs args)
		{
		}
		
		void Warning (object sender, BuildWarningEventArgs args)
		{
		}

		public int TargetStartedEvents { get { return target_started_events; } }

		public int TargetFinishedEvents { get { return target_finished_events; } }
	}

	[TestFixture]
	public class ProjectTest {

		/*
		Import [] GetImports (ImportCollection ic)
		{
			List<Import> list = new List<Import> ();
			foreach (Import i in ic)
				list.Add (i);
			return list.ToArray ();
		}
		*/

		[Test]
		[ExpectedException (typeof (InvalidProjectFileException),
		@"The default XML namespace of the project must be the MSBuild XML namespace." + 
		" If the project is authored in the MSBuild 2003 format, please add " +
		"xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\" to the <Project> element. " +
		"If the project has been authored in the old 1.0 or 1.2 format, please convert it to MSBuild 2003 format.  ")]
		public void TestAssignment1 ()
		{
			Engine engine;
			Project project;
			string documentString =
				"<Project></Project>";
			
			engine = new Engine (Consts.BinPath);
			DateTime time = DateTime.Now;
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (true, project.BuildEnabled, "A1");
			Assert.AreEqual (String.Empty, project.DefaultTargets, "A2");
			Assert.AreEqual (String.Empty, project.FullFileName, "A3");
			Assert.AreEqual (false, project.IsDirty, "A4");
			Assert.AreEqual (false, project.IsValidated, "A5");
			Assert.AreEqual (engine, project.ParentEngine, "A6");
			Assert.IsTrue (time <= project.TimeOfLastDirty, "A7");
			Assert.IsTrue (String.Empty != project.Xml, "A8");
		}

		[Test]
		public void TestAssignment2 ()
		{
			Engine engine;
			Project project;
			string documentString =
				"<Project xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"></Project>";
			
			engine = new Engine (Consts.BinPath);
			DateTime time = DateTime.Now;
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (true, project.BuildEnabled, "A1");
			Assert.AreEqual (String.Empty, project.DefaultTargets, "A2");
			Assert.AreEqual (String.Empty, project.FullFileName, "A3");
			Assert.AreEqual (true, project.IsDirty, "A4");
			Assert.AreEqual (false, project.IsValidated, "A5");
			Assert.AreEqual (engine, project.ParentEngine, "A6");
			Assert.IsTrue (time <= project.TimeOfLastDirty, "A7");
			Assert.IsTrue (String.Empty != project.Xml, "A8");
		}

		[Test]
		[Category ("NotWorking")]
		public void TestAddNewImport1 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<PropertyGroup />
					<ItemGroup />
					<Target Name='a' />
					<Import Project='Test/resources/Import.csproj' />
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.AddNewImport ("a", "true");
			// reevaluation wasn't caused by anything so it has only old import
			Assert.AreEqual (1, project.Imports.Count, "A1");
		}

		[Test]
		[Ignore ("Too detailed probably (implementation specific)")]
		public void TestAddNewItem1 ()
		{
			Engine engine;
			Project project;
			BuildItemGroup [] groups = new BuildItemGroup [1];

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			BuildItem item = project.AddNewItem ("A", "B");

			Assert.AreEqual (1, project.ItemGroups.Count, "A1");
			project.ItemGroups.CopyTo (groups, 0);
			Assert.AreEqual (1, groups [0].Count, "A2");
			Assert.AreEqual ("B", groups [0] [0].Include, "A3");
			Assert.AreEqual ("B", groups [0] [0].FinalItemSpec, "A4");
			Assert.AreEqual ("A", groups [0] [0].Name, "A5");
			//Assert.AreNotSame (item, groups [0] [0], "A6");
			Assert.IsFalse (object.ReferenceEquals (item, groups [0] [0]), "A6");

			Assert.AreEqual (1, project.EvaluatedItems.Count, "A7");
			Assert.AreEqual ("B", project.EvaluatedItems [0].Include, "A8");
			Assert.AreEqual ("B", project.EvaluatedItems [0].FinalItemSpec, "A9");
			Assert.AreEqual ("A", project.EvaluatedItems [0].Name, "A10");
			//Assert.AreNotSame (item, project.EvaluatedItems [0], "A11");
			Assert.IsFalse (object.ReferenceEquals (item, project.EvaluatedItems [0]), "A11");
		}

		[Test]
		[Category ("NotWorking")]
		public void TestAddNewItem2 ()
		{
			Engine engine;
			Project project;

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();

			BuildItem item = project.AddNewItem ("A", "a;b;c");
			Assert.AreEqual ("a;b;c", item.Include, "A1");
			Assert.AreEqual ("a", item.FinalItemSpec, "A2");

			Assert.AreEqual (3, project.EvaluatedItems.Count, "A3");
		}

		[Test]
		public void TestAddNewItem3 ()
		{
			Engine engine;
			Project project;
			BuildItemGroup [] groups = new BuildItemGroup [4];

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<ItemGroup />
					<ItemGroup>
						<A Include='a'/>
					</ItemGroup>
					<ItemGroup>
						<B Include='a'/>
					</ItemGroup>
					<ItemGroup>
						<B Include='a'/>
					</ItemGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.AddNewItem ("B", "b");

			project.ItemGroups.CopyTo (groups, 0);
			Assert.AreEqual (0, groups [0].Count, "A1");
			Assert.AreEqual (1, groups [1].Count, "A2");
			Assert.AreEqual (1, groups [2].Count, "A3");
			Assert.AreEqual (2, groups [3].Count, "A4");
		}
		[Test]
		public void TestAddNewItemGroup ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			BuildItemGroup big = project.AddNewItemGroup ();
			Assert.IsNotNull (big, "A1");
			Assert.AreEqual (String.Empty, big.Condition, "A2");
			Assert.AreEqual (0, big.Count, "A3");
			Assert.AreEqual (false, big.IsImported, "A4");
			Assert.IsTrue (project.IsDirty, "A5");
		}

		[Test]
		public void TestAddNewPropertyGroup ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			BuildPropertyGroup bpg = project.AddNewPropertyGroup (false);
			Assert.IsNotNull (bpg, "A1");
			Assert.AreEqual (String.Empty, bpg.Condition, "A2");
			Assert.AreEqual (0, bpg.Count, "A3");
			Assert.AreEqual (false, bpg.IsImported, "A4");
			Assert.IsTrue (project.IsDirty, "A5");
		}

		[Test]
		public void TestBuild0 ()
		{
			Engine engine;
			Project project;
			IDictionary hashtable = new Hashtable ();

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<Target 
						Name='Main'
						Inputs='a;b;c'
						Outputs='d;e;f'
					>
					</Target>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (true, project.Build (new string [] { "Main" }, hashtable), "A1");
			Assert.AreEqual (1, hashtable.Count, "A2");

			IDictionaryEnumerator e = hashtable.GetEnumerator ();
			e.MoveNext ();

			string name = (string) e.Key;
			Assert.AreEqual ("Main", name, "A3");
			ITaskItem [] arr = (ITaskItem []) e.Value;

			Assert.AreEqual (3, arr.Length, "A4");
			Assert.AreEqual ("d", arr [0].ItemSpec, "A5");
			Assert.AreEqual ("e", arr [1].ItemSpec, "A6");
			Assert.AreEqual ("f", arr [2].ItemSpec, "A7");
		}

		[Test]
		public void TestBuild1 ()
		{
			Engine engine;
			Project project;
			IDictionary hashtable = new Hashtable ();
			
			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<Target Name='Main'>
						<Microsoft.Build.Tasks.Message Text='Text' />
					</Target>
				</Project>
			";
			
			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (true, project.Build (new string[] { "Main" }, hashtable), "A1");
			Assert.AreEqual (1, hashtable.Count, "A2");

			IDictionaryEnumerator e = hashtable.GetEnumerator ();
			e.MoveNext ();

			string name = (string) e.Key;
			Assert.AreEqual ("Main", name, "A3");
			Assert.IsNotNull ((ITaskItem []) e.Value, "A4");
		}

		[Test]
		public void TestBuild2 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<Target Name='T' Inputs='Test\resources\TestTasks.cs' Outputs='Test\resources\TestTasks.dll'>
						<Message Text='text' />
					</Target>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			TestLogger tl = new TestLogger ();
			engine.RegisterLogger (tl);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.Build ("T");
			project.Build ("T");

			Assert.AreEqual (2, tl.TargetStartedEvents, "A1");
			Assert.AreEqual (2, tl.TargetFinishedEvents, "A2");
		}

		[Test]
		public void TestBuild3 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<Target Name='T' Inputs='Test\resources\TestTasks.cs' Outputs='Test\resources\TestTasks.dll'>
						<Message Text='text' />
					</Target>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			TestLogger tl = new TestLogger ();
			engine.RegisterLogger (tl);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.Build (new string [1] { "T" }, null, BuildSettings.None);
			project.Build (new string [1] { "T" }, null, BuildSettings.None);

			Assert.AreEqual (2, tl.TargetStartedEvents, "A1");
			Assert.AreEqual (2, tl.TargetFinishedEvents, "A2");
		}

		[Test]
		[Category ("NotWorking")]
		public void TestBuild4 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<Target Name='T' Inputs='Test\resources\TestTasks.cs' Outputs='Test\resources\TestTasks.dll'>
						<Message Text='text' />
					</Target>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			TestLogger tl = new TestLogger ();
			engine.RegisterLogger (tl);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.Build (new string [1] { "T" }, null, BuildSettings.DoNotResetPreviouslyBuiltTargets);
			project.Build (new string [1] { "T" }, null, BuildSettings.DoNotResetPreviouslyBuiltTargets);

			Assert.AreEqual (1, tl.TargetStartedEvents, "A1");
			Assert.AreEqual (1, tl.TargetFinishedEvents, "A2");
		}

		[Test]
		public void TestBuild5 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.IsFalse (project.Build ("target_that_doesnt_exist"));
		}

		[Test]
		public void TestEvaluatedItems1 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<ItemGroup>
						<A Include='a' />
						<B Include='b' Condition='false' />
					</ItemGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (1, project.EvaluatedItems.Count, "A1");

			BuildItem bi = project.EvaluatedItems [0];

			bi.Name = "C";
			bi.Include = "c";

			BuildItemGroup [] big = new BuildItemGroup [1];
			project.ItemGroups.CopyTo (big, 0);
			Assert.AreEqual ("C", big [0] [0].Name, "A2");
			Assert.AreEqual ("c", big [0] [0].Include, "A3");
		}

		[Test]
		public void TestEvaluatedItems2 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<ItemGroup>
						<A Include='a;b;c' />
					</ItemGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			BuildItemGroup [] big = new BuildItemGroup [1];
			project.ItemGroups.CopyTo (big, 0);

			Assert.AreEqual (3, project.EvaluatedItems.Count, "A1");
			Assert.AreEqual ("a;b;c", big [0] [0].Include, "A2");
			Assert.AreEqual (1, big [0].Count, "A3");

			BuildItem bi = project.EvaluatedItems [0];

			bi.Include = "d";

			Assert.AreEqual (3, big [0].Count, "A4");
			Assert.AreEqual ("d", big [0] [0].Include, "A5");
			Assert.AreEqual ("b", big [0] [1].Include, "A6");
			Assert.AreEqual ("c", big [0] [2].Include, "A7");
		}

		[Test]
		[Category ("NotWorking")]
		public void TestGetConditionedPropertyValues ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<PropertyGroup Condition='true'>
						<A>A</A>
						<B Condition='true'>A</B>
					</PropertyGroup>
					<PropertyGroup>
						<C Condition='true'>A</C>
						<C Condition='false'>B</C>
						<C Condition='!false'>C</C>
						<D>A</D>
						<E Condition="" '$(C)' == 'A' "">E</E>
					</PropertyGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (0, project.GetConditionedPropertyValues ("A").Length, "A1");
			Assert.AreEqual (0, project.GetConditionedPropertyValues ("B").Length, "A2");
			Assert.AreEqual (1, project.GetConditionedPropertyValues ("C").Length, "A3");
			Assert.AreEqual (0, project.GetConditionedPropertyValues ("D").Length, "A4");
			Assert.AreEqual (0, project.GetConditionedPropertyValues ("E").Length, "A5");
			Assert.AreEqual ("A", project.GetConditionedPropertyValues ("C") [0], "A6");
		}

		[Test]
		[ExpectedException (typeof (ArgumentNullException))]
		public void TestGetEvaluatedItemsByName1 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.GetEvaluatedItemsByName (null);
		}

		[Test]
		public void TestGetEvaluatedItemsByName2 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<ItemGroup>
						<A Include='1' />
						<B Include='2' Condition='true' />
						<C Include='3' Condition='false' />
					</ItemGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			BuildItemGroup big;

			big = project.GetEvaluatedItemsByName (String.Empty);

			Assert.AreEqual (0, big.Count, "A1");

			big = project.GetEvaluatedItemsByName ("A");

			Assert.AreEqual (1, big.Count, "A2");
			Assert.AreEqual ("1", big [0].FinalItemSpec, "A3");

			big = project.GetEvaluatedItemsByName ("B");

			Assert.AreEqual (1, big.Count, "A4");
			Assert.AreEqual ("2", big [0].FinalItemSpec, "A5");

			big = project.GetEvaluatedItemsByName ("C");

			Assert.AreEqual (0, big.Count, "A6");
		}

		[Test]
		[ExpectedException (typeof (ArgumentNullException))]
		public void TestGetEvaluatedItemsByNameIgnoringCondition1 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.GetEvaluatedItemsByNameIgnoringCondition (null);
		}

		[Test]
		public void TestGetEvaluatedItemsByNameIgnoringCondition2 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<ItemGroup>
						<A Include='1' />
						<B Include='2' Condition='true' />
						<C Include='3' Condition='false' />
					</ItemGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			BuildItemGroup big;

			big = project.GetEvaluatedItemsByNameIgnoringCondition (String.Empty);

			Assert.AreEqual (0, big.Count, "A1");

			big = project.GetEvaluatedItemsByNameIgnoringCondition ("A");

			Assert.AreEqual (1, big.Count, "A2");
			Assert.AreEqual ("1", big [0].FinalItemSpec, "A3");

			big = project.GetEvaluatedItemsByNameIgnoringCondition ("B");

			Assert.AreEqual (1, big.Count, "A4");
			Assert.AreEqual ("2", big [0].FinalItemSpec, "A5");

			big = project.GetEvaluatedItemsByNameIgnoringCondition ("C");

			Assert.AreEqual (1, big.Count, "A6");
			Assert.AreEqual ("3", big [0].FinalItemSpec, "A7");
		}

		[Test]
		[ExpectedException (typeof (ArgumentNullException))]
		public void TestGetEvaluatedProperty1 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.GetEvaluatedProperty (null);
		}
		[Test]
		public void TestGetEvaluatedProperty2 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<PropertyGroup>
						<A>1</A>
						<B Condition='true'>2</B>
						<C Condition='false'>3</C>
					</PropertyGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual ("1", project.GetEvaluatedProperty ("A"), "A1");
			Assert.AreEqual ("2", project.GetEvaluatedProperty ("B"), "A2");
			Assert.IsNull (project.GetEvaluatedProperty ("C"), "A3");
		}

		[Test]
		public void TestGetProjectExtensions ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<ProjectExtensions>
						<Node>Text</Node>
					</ProjectExtensions>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (String.Empty, project.GetProjectExtensions (null), "A1");
			Assert.AreEqual (String.Empty, project.GetProjectExtensions (String.Empty), "A2");
			Assert.AreEqual (String.Empty, project.GetProjectExtensions ("something"), "A3");
			Assert.AreEqual ("Text", project.GetProjectExtensions ("Node"), "A4");
		}

		[Test]
		public void TestGlobalProperties1 ()
		{
			Engine engine;
			Project project;
			
			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";
			
			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (0, project.GlobalProperties.Count, "A1");
		}

		[Test]
		public void TestGlobalProperties2 ()
		{
			Engine engine;
			Project project;
			
			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";
			
			engine = new Engine (Consts.BinPath);
			engine.GlobalProperties.SetProperty ("Property", "Value");
			
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.AreEqual (1, project.GlobalProperties.Count, "A1");
			Assert.AreEqual ("Property", project.GlobalProperties ["Property"].Name, "A2");
			Assert.AreEqual ("Value", project.GlobalProperties ["Property"].Value, "A3");
			Assert.AreEqual ("Value", project.GlobalProperties ["Property"].FinalValue, "A4");
			Assert.AreEqual ("Property", project.EvaluatedProperties ["Property"].Name, "A2");
			Assert.AreEqual ("Value", project.EvaluatedProperties ["Property"].Value, "A3");
			Assert.AreEqual ("Value", project.EvaluatedProperties ["Property"].FinalValue, "A4");
		}

		[Test]
		[Category ("NotDotNet")]
		[ExpectedException (typeof (ArgumentNullException))]
		public void TestGlobalProperties3 ()
		{
			Engine engine;
			Project project;
			
			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";
			
			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.GlobalProperties = null;
		}

		[Test]
		[Ignore ("needs rewriting")]
		public void TestGlobalProperties4 ()
		{
			Engine engine;
			Project project;
			
			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<PropertyGroup>
						<Property>a</Property>
					</PropertyGroup>
				</Project>
			";
			
			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			BuildPropertyGroup[] groups = new BuildPropertyGroup [1];
			project.PropertyGroups.CopyTo (groups, 0);

			project.GlobalProperties = groups [0];
			project.GlobalProperties = project.EvaluatedProperties;
		}

		[Test]
		[Category ("NotDotNet")]
		[ExpectedException (typeof (InvalidOperationException))]
		public void TestGlobalProperties5 ()
		{
			Engine engine;
			Project project;
			
			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<PropertyGroup>
						<Property>a</Property>
					</PropertyGroup>
				</Project>
			";
			
			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			BuildPropertyGroup[] groups = new BuildPropertyGroup [1];
			project.PropertyGroups.CopyTo (groups, 0);
			project.GlobalProperties = groups [0];
		}

		[Test]
		[ExpectedException (typeof (InvalidProjectFileException))]
		public void TestLoad1 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<PropertyGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);
		}

		[Test]
		[ExpectedException (typeof (InvalidProjectFileException))]
		public void TestLoad2 ()
		{
			Engine engine;
			Project project;

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml ("project_file_that_doesnt_exist");
		}

		[Test]
		public void TestParentEngine ()
		{
			Engine engine;
			Project project;
			
			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();

			Assert.AreEqual (engine, project.ParentEngine, "A1");
		}

		[Test]
		[Category ("NotDotNet")]
		[ExpectedException (typeof (ArgumentNullException))]
		public void TestRemoveItemGroup1 ()
		{
			Engine engine;
			Project p1;

			engine = new Engine (Consts.BinPath);
			p1 = engine.CreateNewProject ();

			p1.RemoveItemGroup (null);
		}

		[Test]
		[ExpectedException (typeof (InvalidOperationException),
					"The \"BuildItemGroup\" object specified does not belong to the correct \"Project\" object.")]
		[Category ("NotWorking")]
		public void TestRemoveItemGroup2 ()
		{
			Engine engine;
			Project p1;
			Project p2;
			BuildItemGroup [] groups  = new BuildItemGroup [1];

			engine = new Engine (Consts.BinPath);
			p1 = engine.CreateNewProject ();
			p2 = engine.CreateNewProject ();

			p1.AddNewItem ("A", "B");
			p1.ItemGroups.CopyTo (groups, 0);

			p2.RemoveItemGroup (groups [0]);
		}

		[Test]
		[Category ("NotDotNet")]
		[ExpectedException (typeof (ArgumentNullException))]
		public void TestRemoveItem1 ()
		{
			Engine engine;
			Project project;

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();

			project.RemoveItem (null);
		}

		[Test]
		[ExpectedException (typeof (InvalidOperationException),
			"The object passed in is not part of the project.")]
		public void TestRemoveItem2 ()
		{
			Engine engine;
			Project project;

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();

			project.RemoveItem (new BuildItem ("name", "include"));
		}

		[Test]
		[ExpectedException (typeof (InvalidOperationException),
					"The \"BuildItemGroup\" object specified does not belong to the correct \"Project\" object.")]
		public void TestRemoveItem3 ()
		{
			Engine engine;
			Project p1;
			Project p2;

			engine = new Engine (Consts.BinPath);
			p1 = engine.CreateNewProject ();
			p2 = engine.CreateNewProject ();

			p1.AddNewItem ("A", "B");

			p2.RemoveItem (p1.EvaluatedItems [0]);
		}

		[Test]
		[Category ("NotDotNet")]
		[ExpectedException (typeof (InvalidOperationException))]
		public void TestRemoveItem4 ()
		{
			Engine engine;
			Project p1;
			Project p2;

			engine = new Engine (Consts.BinPath);
			p1 = engine.CreateNewProject ();
			p2 = engine.CreateNewProject ();

			p1.AddNewItem ("A", "B");
			p1.AddNewItem ("A", "C");

			p2.RemoveItem (p1.EvaluatedItems [0]);
		}

		[Test]
		public void TestRemoveItem5 ()
		{
			Engine engine;
			Project project;
			BuildItemGroup [] groups = new BuildItemGroup [1];

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<ItemGroup>
						<A Include='a'/>
					</ItemGroup>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.RemoveItem (project.EvaluatedItems [0]);
			Assert.AreEqual (0, project.EvaluatedItems.Count, "A1");
			project.ItemGroups.CopyTo (groups, 0);
			Assert.IsNull (groups [0], "A2");
			Assert.AreEqual (0, project.ItemGroups.Count, "A3");
		}

		[Test]
		[Category ("NotWorking")]
		public void TestResetBuildStatus ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<Target Name='T' Inputs='Test\resources\TestTasks.cs' Outputs='Test\resources\TestTasks.dll'>
						<Message Text='text' />
					</Target>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			TestLogger tl = new TestLogger ();
			engine.RegisterLogger (tl);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.Build ("T");
			project.ResetBuildStatus ();
			project.Build (new string [1] { "T" }, null, BuildSettings.DoNotResetPreviouslyBuiltTargets);

			Assert.AreEqual (2, tl.TargetStartedEvents, "A1");
			Assert.AreEqual (2, tl.TargetFinishedEvents, "A1");
		}
		
		[Test]
		public void TestSchemaFile ()
		{
			Engine engine;
			Project project;
			
			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";
			
			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			Assert.IsNull (project.SchemaFile, "A1");
		}
		[Test]
		[Category ("NotDotNet")]
		[ExpectedException (typeof (ArgumentNullException))]
		public void TestSetProjectExtensions1 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.SetProjectExtensions (null, null);
		}

		[Test]
		public void TestSetProjectExtensions2 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.SetProjectExtensions ("name", "1");
			Assert.AreEqual ("1", project.GetProjectExtensions ("name"), "A1");
			project.SetProjectExtensions ("name", "2");
			Assert.AreEqual ("2", project.GetProjectExtensions ("name"), "A2");
			Assert.IsTrue (project.IsDirty, "A3");
		}

		[Test]
		public void TestSetProjectExtensions3 ()
		{
			Engine engine;
			Project project;

			string documentString = @"
				<Project xmlns='http://schemas.microsoft.com/developer/msbuild/2003'>
					<ProjectExtensions>
					</ProjectExtensions>
				</Project>
			";

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);

			project.SetProjectExtensions ("name", "1");
			Assert.AreEqual ("1", project.GetProjectExtensions ("name"), "A1");
			Assert.IsTrue (project.IsDirty, "A2");
		}

		[Test]
		public void TestBuildProjectError1 ()
		{
			Engine engine = new Engine (Consts.BinPath);
			Project project = engine.CreateNewProject ();

			Assert.IsFalse (project.Build ((string) null), "A1");
			Assert.IsFalse (project.Build ((string[]) null), "A2");
			Assert.IsFalse (project.Build ((string []) null, null), "A3");
			Assert.IsFalse (project.Build ((string []) null, null, BuildSettings.None), "A4");
		}

		[Test]
		public void TestBuildProjectError2 ()
		{
			Engine engine = new Engine (Consts.BinPath);
			Project project = engine.CreateNewProject ();

			try {
				project.Build (new string [] { null });
			} catch {
				return;
			}
			Assert.Fail ("Expected exception for project.Build, null string in targetNames []");
		}

		[Test]
		public void TestBuildProjectFile1 ()
		{
			Project project = CreateAndLoadProject ("foo.proj", false, new string [] { "1", "2" }, new bool [] { true, true }, "TBPF1");
			CheckProjectBuild (project, new string [] { "main" }, true, new string [] { "main" }, "TBPF1");
		}

		[Test]
		public void TestBuildProjectFileXml1 ()
		{
			Project project = CreateAndLoadProject (null, false, new string [] { "1", "2" }, new bool [] { true, true }, "TBPFX1");
			CheckProjectBuild (project, new string [] { "main" }, true, new string [] { "main" }, "TBPFX1");
		}

		[Test]
		public void TestBuildProjectFile2 ()
		{
			Project project = CreateAndLoadProject ("foo.proj", false, new string [] { "1", "2", "3" }, new bool [] { true, false, true }, "TBPF2");
			CheckProjectBuild (project, new string [] { "main" }, false, new string [0], "TBPF2");
		}

		[Test]
		public void TestBuildProjectFileXml2 ()
		{
			Project project = CreateAndLoadProject (null, false, new string [] { "1", "2", "3" }, new bool [] { true, false, true }, "TBPFX2");
			CheckProjectBuild (project, new string [] { "main" }, false, new string [0], "TBPFX2");
		}

		[Test]
		public void TestBuildProjectFile3 ()
		{
			Project project = CreateAndLoadProject ("foo.proj", false, new string [] { "1", "2", "3" }, new bool [] { true, true, true }, "TBPF3");
			CheckProjectBuild (project, new string [] { "1", "2" }, true, new string [] { "1", "2" }, "TBPF3");
		}

		[Test]
		public void TestBuildProjectFileXml3 ()
		{
			Project project = CreateAndLoadProject (null, false, new string [] { "1", "2", "3" }, new bool [] { true, true, true }, "TBPFX3");
			CheckProjectBuild (project, new string [] { "1", "2" }, true, new string [] { "1", "2" }, "TBPFX3");
		}

		[Test]
		public void TestBuildProjectFile4 ()
		{
			Project project = CreateAndLoadProject ("foo.proj", false, new string [] { "1", "2", "3" }, new bool [] { true, false, true }, "TBPF4");
			CheckProjectBuild (project, new string [] { "main" }, false, new string [0], "TBPF4");
		}

		[Test]
		public void TestBuildProjectFileXml4 ()
		{
			Project project = CreateAndLoadProject (null, false, new string [] { "1", "2", "3" }, new bool [] { true, false, true }, "TBPFX4");
			CheckProjectBuild (project, new string [] { "main" }, false, new string [0], "TBPFX4");
		}

		//Run separate tests

		//Run single target
		[Test]
		public void TestBuildProjectFile5 ()
		{
			Project project = CreateAndLoadProject ("foo.proj", true, new string [] { "1", "2", "3" }, new bool [] { true, false, true }, "TBPF5");
			CheckProjectBuild (project, new string [] { "main" }, false, new string [0], "TBPF5");
		}

		[Test]
		public void TestBuildProjectFileXml5 ()
		{
			Project project = CreateAndLoadProject (null, true, new string [] { "1", "2", "3" }, new bool [] { true, false, true }, "TBPFX5");
			CheckProjectBuild (project, new string [] { "main" }, false, new string [0], "TBPFX5");
		}

		[Test]
		public void TestBuildProjectFile6 ()
		{
			Project project = CreateAndLoadProject ("foo.proj", true, new string [] { "1", "2", "3" }, new bool [] { true, true, true }, "TBPF6");
			CheckProjectBuild (project, new string [] { "main" }, true, new string [] { "main" }, "TBPF6");
		}

		[Test]
		public void TestBuildProjectFileXml6 ()
		{
			Project project = CreateAndLoadProject (null, true, new string [] { "1", "2", "3" }, new bool [] { true, true, true }, "TBPFX6");
			CheckProjectBuild (project, new string [] { "main" }, true, new string [] { "main" }, "TBPFX6");
		}

		// run multiple targets
		[Test]
		public void TestBuildProjectFile7 ()
		{
			Project project = CreateAndLoadProject ("foo.proj", true, new string [] { "1", "2", "3" }, new bool [] { true, true, true }, "TBPF7");
			CheckProjectBuild (project, new string [] { "1", "2", "3" }, true, new string [] { "1", "2", "3" }, "TBPF7");
		}

		[Test]
		public void TestBuildProjectFileXml7 ()
		{
			Project project = CreateAndLoadProject (null, true, new string [] { "1", "2", "3" }, new bool [] { true, true, true }, "TBPFX7");
			CheckProjectBuild (project, new string [] { "1", "2", "3" }, true, new string [] { "1", "2", "3" }, "TBPFX7");
		}

		[Test]
		public void TestBuildProjectFile8 ()
		{
			Project project = CreateAndLoadProject ("foo.proj", true, new string [] { "1", "2", "3" }, new bool [] { true, true, false }, "TBPF8");
			CheckProjectBuild (project, new string [] { "1", "2", "3" }, false, new string [] { "1", "2"}, "TBPF8");
		}

		[Test]
		public void TestBuildProjectFileXml8 ()
		{
			Project project = CreateAndLoadProject (null, true, new string [] { "1", "2", "3" }, new bool [] { true, true, false }, "TBPFX8");
			CheckProjectBuild (project, new string [] { "1", "2", "3" }, false, new string [] { "1", "2"}, "TBPFX8");
		}

		void CheckProjectBuild (Project project, string [] targetNames, bool result, string [] outputNames, string prefix)
		{
			IDictionary targetOutputs = new Hashtable ();

			Assert.AreEqual (result, project.Build (targetNames, targetOutputs), prefix + "A1");
			Assert.AreEqual (outputNames.Length, targetOutputs.Keys.Count, prefix + "A2");

			foreach (string outputName in outputNames) {
				Assert.IsTrue (targetOutputs.Contains (outputName), prefix + " A3: target " + outputName);

				object o = targetOutputs [outputName];
				Assert.IsTrue (typeof (ITaskItem []).IsAssignableFrom (o.GetType ()), prefix + " A4: target " + outputName);

				ITaskItem [] items = (ITaskItem [])o;
				Assert.AreEqual (0, items.Length, prefix + "A5: target " + outputName);
			}
		}

		string CreateProjectString (bool run_separate, string [] targets, bool [] results, string prefix)
		{
			StringBuilder sb = new StringBuilder ();
			sb.Append (@"<Project xmlns=""http://schemas.microsoft.com/developer/msbuild/2003"">");
			sb.AppendFormat ("<Target Name = \"{0}\"><Message Text = \"#Target {1}:{0} called\" />", "main", prefix);

			sb.AppendFormat ("<CallTarget Targets=\"");
			for (int i = 0; i < targets.Length; i++)
				sb.AppendFormat ("{0};", targets [i]);
			sb.AppendFormat ("\" ");

			if (run_separate)
				sb.AppendFormat (" RunEachTargetSeparately=\"true\" ");
			sb.AppendFormat ("/></Target>\n");

			for (int i = 0; i < targets.Length; i++) {
				sb.AppendFormat ("<Target Name = \"{0}\"><Message Text = \"#Target {1}:{0} called\" />", targets [i], prefix);
				if (!results [i])
					sb.AppendFormat ("<Error Text = \"#Error message for target {0}:{1}\"/>", prefix, targets [i]);
				sb.Append ("</Target>\n");
			}

			sb.Append ("</Project>");

			return sb.ToString ();
		}

		void CreateProjectFile (string fname, bool run_separate, string [] targets, bool [] results, string prefix)
		{
			using (StreamWriter sw = new StreamWriter (fname))
				sw.Write (CreateProjectString (run_separate, targets, results, prefix));
		}

		Project CreateAndLoadProject (string fname, bool run_separate, string [] targets, bool [] results, string prefix)
		{
			Engine engine = new Engine (Consts.BinPath);
			Project project = engine.CreateNewProject ();

			string projectXml = CreateProjectString (run_separate, targets, results, prefix);
			if (fname == null) {
				project.LoadXml (projectXml);
			} else {
				using (StreamWriter sw = new StreamWriter (fname))
					sw.Write (projectXml);
				project.Load (fname);
				File.Delete (project.FullFileName);
			}

			return project;
		}
	}
}
