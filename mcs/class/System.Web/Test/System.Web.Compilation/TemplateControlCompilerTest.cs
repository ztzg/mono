using System;
using System.Collections.Generic;
using MonoTests.SystemWeb.Framework;
using NUnit.Framework;
using System.Web;
using System.Web.Compilation;
using System.Web.UI.WebControls;
using System.Reflection;
using System.ComponentModel;
using System.Threading;

namespace MonoTests.System.Web.Compilation {
	public class ReadOnlyPropertyControl:TextBox {
		[Bindable (true)]
		public bool MyProp
		{
			get { return true; }
		}

	}

#if NET_2_0
	public class BindTestDataItem
	{
		int data;
		public int Data {
			get { return data; }
			set { data = value; }
		}

		public BindTestDataItem (int data)
		{
			this.data = data;
		}
	}
	
	public class BindTestDataSource 
	{
		public IList <BindTestDataItem> GetData ()
		{
			return new List <BindTestDataItem> {new BindTestDataItem (0), new BindTestDataItem (1)};
		}
	}
#endif
	
	[TestFixture]
	public class TemplateControlCompilerTest
	{
		[TestFixtureSetUp]
		public void TemplateControlCompiler_Init ()
		{
			WebTest.CopyResource (GetType (), "ReadOnlyPropertyBind.aspx", "ReadOnlyPropertyBind.aspx");
			WebTest.CopyResource (GetType (), "ReadOnlyPropertyControl.ascx", "ReadOnlyPropertyControl.ascx");
			WebTest.CopyResource (GetType (), "TemplateControlParsingTest.aspx", "TemplateControlParsingTest.aspx");
#if NET_2_0
			WebTest.CopyResource (GetType (), "InvalidPropertyBind1.aspx", "InvalidPropertyBind1.aspx");
			WebTest.CopyResource (GetType (), "InvalidPropertyBind2.aspx", "InvalidPropertyBind2.aspx");
			WebTest.CopyResource (GetType (), "InvalidPropertyBind3.aspx", "InvalidPropertyBind3.aspx");
			WebTest.CopyResource (GetType (), "InvalidPropertyBind4.aspx", "InvalidPropertyBind4.aspx");
			WebTest.CopyResource (GetType (), "ValidPropertyBind1.aspx", "ValidPropertyBind1.aspx");
			WebTest.CopyResource (GetType (), "ValidPropertyBind2.aspx", "ValidPropertyBind2.aspx");
			WebTest.CopyResource (GetType (), "ValidPropertyBind3.aspx", "ValidPropertyBind3.aspx");
			WebTest.CopyResource (GetType (), "ValidPropertyBind4.aspx", "ValidPropertyBind4.aspx");
			WebTest.CopyResource (GetType (), "ValidPropertyBind5.aspx", "ValidPropertyBind5.aspx");
			WebTest.CopyResource (GetType (), "ReadWritePropertyControl.ascx", "ReadWritePropertyControl.ascx");
			WebTest.CopyResource (GetType (), "ContentPlaceHolderInTemplate.aspx", "ContentPlaceHolderInTemplate.aspx");
			WebTest.CopyResource (GetType (), "ContentPlaceHolderInTemplate.master", "ContentPlaceHolderInTemplate.master");
#endif
		}
		
        	[Test]
		[NUnit.Framework.Category ("NunitWeb")]
#if !TARGET_JVM
		[NUnit.Framework.Category ("NotWorking")]
#endif
		public void ReadOnlyPropertyBindTest ()
		{
			new WebTest ("ReadOnlyPropertyBind.aspx").Run ();
		}

#if NET_2_0
		// Test for bug #449970
		[Test]
		public void MasterPageContentPlaceHolderInTemplate ()
		{
			new WebTest ("ContentPlaceHolderInTemplate.aspx").Run ();
		}
		
		[Test]
		[ExpectedException (typeof (CompilationException))]
		public void InvalidPropertyBindTest1 ()
		{
			new WebTest ("InvalidPropertyBind1.aspx").Run ();
		}

		[Test]
		[ExpectedException (typeof (HttpParseException))]
		public void InvalidPropertyBindTest2 ()
		{
			new WebTest ("InvalidPropertyBind2.aspx").Run ();
		}

		[Test]
		[ExpectedException (typeof (CompilationException))]
		public void InvalidPropertyBindTest3 ()
		{
			new WebTest ("InvalidPropertyBind3.aspx").Run ();
		}

		[Test]
		[ExpectedException (typeof (HttpParseException))]
		public void InvalidPropertyBindTest4 ()
		{
			new WebTest ("InvalidPropertyBind4.aspx").Run ();
		}

		[Test]
		public void ValidPropertyBindTest1 ()
		{
			new WebTest ("ValidPropertyBind1.aspx").Run ();
		}

		[Test]
		public void ValidPropertyBindTest2 ()
		{
			new WebTest ("ValidPropertyBind2.aspx").Run ();
		}

		[Test]
		public void ValidPropertyBindTest3 ()
		{
			new WebTest ("ValidPropertyBind3.aspx").Run ();
		}

		[Test]
		public void ValidPropertyBindTest4 ()
		{
			new WebTest ("ValidPropertyBind4.aspx").Run ();
		}

		[Test]
		public void ValidPropertyBindTest5 ()
		{
			new WebTest ("ValidPropertyBind5.aspx").Run ();
		}
#endif
		
		[Test]
		public void ChildTemplatesTest ()
		{
			try {
				WebTest.Host.AppDomain.AssemblyResolve += new ResolveEventHandler (ResolveAssemblyHandler);
				new WebTest ("TemplateControlParsingTest.aspx").Run ();
			} finally {
				WebTest.Host.AppDomain.AssemblyResolve -= new ResolveEventHandler (ResolveAssemblyHandler);
			}
		}
		
		[TestFixtureTearDown]
		public void TearDown ()
		{
			Thread.Sleep (100);
			WebTest.Unload ();
		}

		public static Assembly ResolveAssemblyHandler (object sender, ResolveEventArgs e)
		{
			if (e.Name != "System.Web_test")
				return null;

			return Assembly.GetExecutingAssembly ();
		}
	}
}

