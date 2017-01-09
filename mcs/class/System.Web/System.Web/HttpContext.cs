//
// System.Web.HttpContext.cs 
//
// Author:
//	Miguel de Icaza (miguel@novell.com)
//	Gonzalo Paniagua Javier (gonzalo@novell.com)
//

//
// Copyright (C) 2005 Novell, Inc (http://www.novell.com)
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

using System.Collections;
using System.Configuration;
using System.Globalization;
using System.Runtime.Remoting.Messaging;
using System.Security.Permissions;
using System.Security.Principal;
using System.Threading;
using System.Web.Caching;
using System.Web.Configuration;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.Util;
#if NET_2_0
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Resources;
using System.Web.Compilation;
using System.Web.Profile;
using CustomErrorMode = System.Web.Configuration.CustomErrorsMode;
#endif

namespace System.Web {
	// CAS - no InheritanceDemand here as the class is sealed
	[AspNetHostingPermission (SecurityAction.LinkDemand, Level = AspNetHostingPermissionLevel.Minimal)]
	public sealed partial class HttpContext : IServiceProvider {
		internal HttpWorkerRequest WorkerRequest;
		HttpApplication app_instance;
		HttpRequest request;
		HttpResponse response;
		HttpSessionState session_state;
		HttpServerUtility server;
		TraceContext trace_context;
		IHttpHandler handler;
		string error_page;
		bool skip_authorization = false;
		IPrincipal user;
		object errors;
		Hashtable items;
		object config_timeout;
		int timeout_possible;
		DateTime time_stamp = DateTime.UtcNow;
		Timer timer;
		Thread thread;
		bool _isProcessingInclude;

#if NET_2_0
		[ThreadStatic]
		static ResourceProviderFactory provider_factory;
		[ThreadStatic]
		static Dictionary <string, IResourceProvider> resource_providers;
		
#if TARGET_JVM
		const string app_global_res_key = "HttpContext.app_global_res_key";
		internal static Assembly AppGlobalResourcesAssembly {
			get { return (Assembly) AppDomain.CurrentDomain.GetData (app_global_res_key); }
			set { AppDomain.CurrentDomain.SetData (app_global_res_key, value); }
		}
#else
		[ThreadStatic]
		static Dictionary <ResourceManagerCacheKey, ResourceManager> resourceManagerCache;
		internal static Assembly AppGlobalResourcesAssembly;
#endif
		ProfileBase profile = null;
		LinkedList<IHttpHandler> handlers;
#endif
		
		public HttpContext (HttpWorkerRequest wr)
		{
			WorkerRequest = wr;
			request = new HttpRequest (WorkerRequest, this);
			response = new HttpResponse (WorkerRequest, this);
		}

		public HttpContext (HttpRequest request, HttpResponse response)
		{
			this.request = request;
			this.response = response;
			
		}

		internal bool IsProcessingInclude {
			get { return _isProcessingInclude; }
			set { _isProcessingInclude = value; }
		}

		public Exception [] AllErrors {
			get {
				if (errors == null)
					return null;

				if (errors is Exception){
					Exception [] all = new Exception [1];
					all [0] = (Exception) errors;
					return all;
				} 
				return (Exception []) (((ArrayList) errors).ToArray (typeof (Exception)));
			}
		}

		public HttpApplicationState Application {
			get {
				return HttpApplicationFactory.ApplicationState;
			}
		}

		public HttpApplication ApplicationInstance {
			get {
				return app_instance;
			}

			set {
				app_instance = value;
			}
			      
		}

		public Cache Cache {
			get {
				return HttpRuntime.Cache;
			}
		}

		internal Cache InternalCache {
			get {
				return HttpRuntime.InternalCache;
			}
		}
		
		//
		// The "Current" property is set just after we have constructed it with 
		// the 'HttpContext (HttpWorkerRequest)' constructor.
		//
#if !TARGET_JVM // No remoting CallContext support in Grasshopper
		public static HttpContext Current {
			get {
				return (HttpContext) CallContext.GetData ("c");
			}

			set {
				CallContext.SetData ("c", value);
			}
		}
#endif

		public Exception Error {
			get {
				if (errors == null || (errors is Exception))
					return (Exception) errors;
				return (Exception) (((ArrayList) errors) [0]);
			}
		}

		public IHttpHandler Handler {
			get {
				return handler;
			}

			set {
				handler = value;
			}
		}

		public bool IsCustomErrorEnabled {
			get {
				try {
					return IsCustomErrorEnabledUnsafe;
				}
				catch {
					return false;
				}
			}
		}

		internal bool IsCustomErrorEnabledUnsafe {
			get {
#if NET_2_0
				CustomErrorsSection cfg = (CustomErrorsSection) WebConfigurationManager.GetSection ("system.web/customErrors");
#else
				CustomErrorsConfig cfg = null;
				try {
					cfg = (CustomErrorsConfig) GetConfig ("system.web/customErrors");
				} catch {
				}

				if (cfg == null)
					return false;
#endif

				if (cfg.Mode == CustomErrorMode.On)
					return true;

				return (cfg.Mode == CustomErrorMode.RemoteOnly) && !Request.IsLocal;
			}
		}
#if !TARGET_JVM
		public bool IsDebuggingEnabled {
			get {
#if NET_2_0
				CompilationSection section = (CompilationSection) WebConfigurationManager.GetSection ("system.web/compilation");
				return section.Debug;
#else
				try {
					return CompilationConfiguration.GetInstance (this).Debug;
				} catch {
					return false;
				}
#endif
			}
		}
#endif
		public IDictionary Items {
			get {
				if (items == null)
					items = new Hashtable ();
				return items;
			}
		}

		public HttpRequest Request {
			get {
				return request;
			}
		}

		public HttpResponse Response {
			get {
				return response;
			}
		}

		public HttpServerUtility Server {
			get {
				if (server == null)
					server = new HttpServerUtility (this);
				return server;
			}
		}

		public HttpSessionState Session {
			get {
				return session_state;
			}
		}

		public bool SkipAuthorization {
			get {
				return skip_authorization;
			}

			[SecurityPermission (SecurityAction.Demand, ControlPrincipal = true)]
			set {
				skip_authorization = value;
			}
		}

		public DateTime Timestamp {
			get {
				return time_stamp.ToLocalTime ();
			}
		}
		
		public TraceContext Trace {
			get {
				if (trace_context == null)
					trace_context = new TraceContext (this);
				return trace_context;
			}
		}

		public IPrincipal User {
			get {
				return user;
			}

			[SecurityPermission (SecurityAction.Demand, ControlPrincipal = true)]
			set {
				user = value;
			}
		}

#if NET_2_0
		internal bool MapRequestHandlerDone {
			get;
			set;
		}
		
		// The two properties below are defined only when the IIS7 integrated mode is used.
		// They are useless under Mono
		public RequestNotification CurrentNotification {
			get { throw new PlatformNotSupportedException ("This property is not supported on Mono.");  }
		}

		public bool IsPostNotification {
			get { throw new PlatformNotSupportedException ("This property is not supported on Mono."); }
		}
		
		internal void PushHandler (IHttpHandler handler)
		{
			if (handler == null)
				return;
			if (handlers == null)
				handlers = new LinkedList <IHttpHandler> ();
			handlers.AddLast (handler);
		}

		internal void PopHandler ()
		{
			if (handlers == null || handlers.Count == 0)
				return;
			handlers.RemoveLast ();
		}
		
		IHttpHandler GetCurrentHandler ()
		{
			if (handlers == null || handlers.Count == 0)
				return null;
			
			return handlers.Last.Value;
		}

		IHttpHandler GetPreviousHandler ()
		{
			if (handlers == null || handlers.Count <= 1)
				return null;
			LinkedListNode <IHttpHandler> previous = handlers.Last.Previous;
			if (previous != null)
				return previous.Value;
			return null;
		}
		
		public IHttpHandler CurrentHandler {
			get { return GetCurrentHandler (); }
		}

		public IHttpHandler PreviousHandler {
			get { return GetPreviousHandler (); }
		}

		internal bool ProfileInitialized {
			get { return profile != null; }
		}

		public ProfileBase Profile {
			get {
				if (profile == null) {
					if (Request.IsAuthenticated)
						profile = ProfileBase.Create (User.Identity.Name);
					else
						profile = ProfileBase.Create (Request.AnonymousID, false);
				}
				return profile;
			}

			internal set {
				profile = value;
			}
		}
#endif

		public void AddError (Exception errorInfo)
		{
			if (errors == null){
				errors = errorInfo;
				return;
			}
			ArrayList l;
			if (errors is Exception){
				l = new ArrayList ();
				l.Add (errors);
				errors = l;
			} else 
				l = (ArrayList) errors;
			l.Add (errorInfo);
		}

		internal void ClearError (Exception e)
		{
			if (errors == e)
				errors = null;
		}

		internal bool HasError (Exception e)
		{
			if (errors == e)
				return true;

			return (errors is ArrayList) ?
				((ArrayList) errors).Contains (e) : false;
		}

		public void ClearError ()
		{
			errors = null;
		}

#if NET_2_0
		[Obsolete ("use WebConfigurationManager.GetWebApplicationSection")]
#endif
		public static object GetAppConfig (string name)
		{
			object o = ConfigurationSettings.GetConfig (name);

			return o;
		}

#if NET_2_0
		[Obsolete ("see GetSection")]
#endif
		public object GetConfig (string name)
		{
#if NET_2_0
			return GetSection (name);
#else
			return WebConfigurationSettings.GetConfig (name, this);
#endif
		}

#if NET_2_0
		static object GetResourceObject (string classKey, string resourceKey, CultureInfo culture, Assembly assembly)
		{
			ResourceManager rm;
			try {
				if (resourceManagerCache == null)
					resourceManagerCache = new Dictionary <ResourceManagerCacheKey, ResourceManager> ();
				
				ResourceManagerCacheKey key = new ResourceManagerCacheKey (classKey, assembly);
				if (!resourceManagerCache.TryGetValue (key, out rm)) {
					rm = new ResourceManager (classKey, assembly);
					rm.IgnoreCase = true;
					resourceManagerCache.Add (key, rm);
				}
				
				return rm.GetObject (resourceKey, culture);
			} catch (MissingManifestResourceException) {
				throw;
			} catch (Exception ex) {
				throw new HttpException ("Failed to retrieve the specified global resource object.", ex);
			}
		}
		
		public static object GetGlobalResourceObject (string classKey, string resourceKey)
		{
			return GetGlobalResourceObject (classKey, resourceKey, Thread.CurrentThread.CurrentUICulture);
		}

		static object GetGlobalObjectFromFactory (string classKey, string resourceKey, CultureInfo culture)
		{
			// FIXME: Retention of data

			if (provider_factory == null) {
				GlobalizationSection gs = WebConfigurationManager.GetSection ("system.web/globalization") as GlobalizationSection;

				if (gs == null)
					return null;

				String rsfTypeName = gs.ResourceProviderFactoryType;
				if (String.IsNullOrEmpty (rsfTypeName))
					return null;
			
				Type rsfType = Type.GetType (rsfTypeName, true);
				ResourceProviderFactory rpf = Activator.CreateInstance (rsfType) as ResourceProviderFactory;
			
				if (rpf == null)
					return null;

				provider_factory = rpf;
			}

			if (resource_providers == null)
				resource_providers = new Dictionary <string, IResourceProvider> ();

			IResourceProvider rp;
			if (!resource_providers.TryGetValue (classKey, out rp)) {
				rp = provider_factory.CreateGlobalResourceProvider (classKey);
				if (rp == null)
					return null;
				resource_providers.Add (classKey, rp);
			}

			return rp.GetObject (resourceKey, culture);
		}
		
		public static object GetGlobalResourceObject (string classKey, string resourceKey, CultureInfo culture)
		{
			object ret = GetGlobalObjectFromFactory (classKey, resourceKey, culture);
			if (ret != null)
				return ret;
			
			if (AppGlobalResourcesAssembly == null)
				return null;

			return GetResourceObject ("Resources." + classKey, resourceKey, culture, AppGlobalResourcesAssembly);
		}

		public static object GetLocalResourceObject (string virtualPath, string resourceKey)
		{
			return GetLocalResourceObject (virtualPath, resourceKey, Thread.CurrentThread.CurrentUICulture);
		}

		public static object GetLocalResourceObject (string virtualPath, string resourceKey, CultureInfo culture)
		{
			if (!VirtualPathUtility.IsAbsolute (virtualPath))
				throw new ArgumentException ("The specified virtualPath was not rooted.");
			
			string path = Path.GetDirectoryName (virtualPath);
			Assembly asm = AppResourcesCompiler.GetCachedLocalResourcesAssembly (path);
			if (asm == null) {
				AppResourcesCompiler ac = new AppResourcesCompiler (path);
				asm = ac.Compile ();
				if (asm == null)
					throw new MissingManifestResourceException ("A resource object was not found at the specified virtualPath.");
			}
			
			path = Path.GetFileName (virtualPath);
			return GetResourceObject (path, resourceKey, culture, asm);
		}

		public object GetSection (string name)
		{
			return WebConfigurationManager.GetSection (name);
		}

		sealed class ResourceManagerCacheKey
		{
			readonly string _name;
			readonly Assembly _asm;

			public ResourceManagerCacheKey (string name, Assembly asm)
			{
				_name = name;
				_asm = asm;
			}

			public override bool Equals (object obj)
			{
				if (!(obj is ResourceManagerCacheKey))
					return false;
				ResourceManagerCacheKey key = (ResourceManagerCacheKey) obj;
				return key._asm == _asm && _name.Equals (key._name, StringComparison.Ordinal);
			}

			public override int GetHashCode ()
			{
				return _name.GetHashCode () + _asm.GetHashCode ();
			}
		}
#endif
		object IServiceProvider.GetService (Type service)
		{
			if (service == typeof (HttpWorkerRequest))
				return WorkerRequest;

			//
			// We return everything out of properties in case
			// they are dynamically computed in some form in the future.
			//
			if (service == typeof (HttpApplication))
				return ApplicationInstance;

			if (service == typeof (HttpRequest))
				return Request;

			if (service == typeof (HttpResponse))
				return Response;

			if (service == typeof (HttpSessionState))
				return Session;

			if (service == typeof (HttpApplicationState))
				return Application;

			if (service == typeof (IPrincipal))
				return User;

			if (service == typeof (Cache))
				return Cache;

			if (service == typeof (HttpContext))
				return Current;

			if (service == typeof (IHttpHandler))
				return Handler;

			if (service == typeof (HttpServerUtility))
				return Server;
			
			if (service == typeof (TraceContext))
				return Trace;
			
			return null;
		}

#if NET_2_0
		public void RemapHandler (IHttpHandler handler)
		{
			if (MapRequestHandlerDone)
				throw new InvalidOperationException ("The RemapHandler method was called after the MapRequestHandler event occurred.");
			Handler = handler;
		}
#endif
		
		public void RewritePath (string path)
		{
#if NET_2_0
			RewritePath (path, true);
#else
			RewritePath (path, false);
#endif
		}

		public void RewritePath (string filePath, string pathInfo, string queryString)
		{
			RewritePath (filePath, pathInfo, queryString, false);
		}

#if NET_2_0
		public
#else
		internal
#endif
		void RewritePath (string path, bool rebaseClientPath)
		{
			int qmark = path.IndexOf ('?');
			if (qmark != -1)
				RewritePath (path.Substring (0, qmark), "", path.Substring (qmark + 1), rebaseClientPath);
			else
				RewritePath (path, null, null, rebaseClientPath);
		}

#if NET_2_0
		public
#else
		internal
#endif
		void RewritePath (string filePath, string pathInfo, string queryString, bool setClientFilePath)
		{
			if (filePath == null)
				throw new ArgumentNullException ("filePath");
			if (!VirtualPathUtility.IsValidVirtualPath (filePath))
				throw new HttpException ("'" + HttpUtility.HtmlEncode (filePath) + "' is not a valid virtual path.");

			bool pathRelative = VirtualPathUtility.IsAppRelative (filePath);
			bool pathAbsolute = pathRelative ? false : VirtualPathUtility.IsAbsolute (filePath);
			if (pathRelative || pathAbsolute) {
				bool needSubstring = false;

				if (pathRelative && filePath.Length > 1)
					needSubstring = true;

				string bvd = Request.BaseVirtualDir;
				if (bvd.Length > 1)
					bvd += "/";

				string canonizedFilePath = VirtualPathUtility.Canonize (filePath);
				filePath = VirtualPathUtility.Combine (bvd, needSubstring ? canonizedFilePath.Substring (2) : canonizedFilePath);
			} else 
				filePath = VirtualPathUtility.Combine (VirtualPathUtility.GetDirectory (Request.FilePath), filePath);
			
			if (!StrUtils.StartsWith (filePath, HttpRuntime.AppDomainAppVirtualPath))
				throw new HttpException (404, "The virtual path '" + HttpUtility.HtmlEncode (filePath) + "' maps to another application.", filePath);
			
			Request.SetCurrentExePath (filePath);
			if (setClientFilePath)
				Request.SetFilePath (filePath);
			
			// A null pathInfo or queryString is ignored and previous values remain untouched
			if (pathInfo != null)
				Request.SetPathInfo (pathInfo);

			if (queryString != null)
				Request.QueryStringRaw = queryString;
		}

#region internals
		
		internal void SetSession (HttpSessionState state)
		{
			session_state = state;
		}

		// URL of a page used for error redirection.
		internal string ErrorPage {
			get {
				return error_page;
			}

			set {
				error_page = value;
			}
		}

		internal TimeSpan ConfigTimeout {
			get {
				if (config_timeout == null) {
#if NET_2_0
					HttpRuntimeSection section = (HttpRuntimeSection)WebConfigurationManager.GetSection ("system.web/httpRuntime");
					config_timeout = section.ExecutionTimeout;
#else
					HttpRuntimeConfig config = (HttpRuntimeConfig)
								GetConfig ("system.web/httpRuntime");
					config_timeout = new TimeSpan (0, 0, config.ExecutionTimeout);
#endif
				}

				return (TimeSpan) config_timeout;
			}

			set {
				config_timeout = value;
#if !TARGET_J2EE
				if (timer != null) {
					TimeSpan remaining = value - (DateTime.UtcNow - time_stamp);
					long remaining_ms = Math.Max ((long)remaining.TotalMilliseconds, 0);

					// See http://msdn2.microsoft.com/en-us/library/7hs7492w.aspx
					if (remaining_ms > 4294967294)
						remaining_ms = 4294967294;
					
					timer.Change (remaining_ms, (long)Timeout.Infinite);
				}
#endif
			}
		}

#if !TARGET_J2EE
		void TimeoutReached(object state) {
			if (Interlocked.CompareExchange (ref timeout_possible, 0, 0) == 0) {
				timer.Change(2000, 0);
				return;			
			}
			StopTimeoutTimer();
			
			thread.Abort (new StepTimeout ());
		}
		
		internal void StartTimeoutTimer() {
			thread = Thread.CurrentThread;
			timer = new Timer (TimeoutReached, null, (int)ConfigTimeout.TotalMilliseconds, Timeout.Infinite);
		}
		
		internal void StopTimeoutTimer() {
			if(timer != null) {
				timer.Dispose ();
				timer = null;
			}
		}

		internal bool TimeoutPossible {
			get { return (Interlocked.CompareExchange (ref timeout_possible, 1, 1) == 1); }
		}

		internal void BeginTimeoutPossible ()
		{
			timeout_possible = 1;
		}

		internal void EndTimeoutPossible ()
		{
			Interlocked.CompareExchange (ref timeout_possible, 0, 1);
		}
#endif
#endregion
	}
	
	class StepTimeout
	{
	}
}
