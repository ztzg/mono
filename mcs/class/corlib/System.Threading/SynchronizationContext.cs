// 
// System.Threading.SynchronizationContext.cs
//
// Author:
//   Lluis Sanchez (lluis@novell.com)
//
// Copyright (C) 2004-2005 Novell, Inc (http://www.novell.com)
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

#if NET_2_0

using System.Runtime.ConstrainedExecution;
using System.Runtime.InteropServices;

namespace System.Threading 
{
	public class SynchronizationContext
	{
		bool notification_required;

		[ThreadStatic]
		static SynchronizationContext currentContext;
		
		public SynchronizationContext ()
		{
		}

		internal SynchronizationContext (SynchronizationContext context)
		{
			currentContext = context;
		}
		
		public static SynchronizationContext Current
		{
			get {
#if NET_2_1
				if (currentContext == null)
					currentContext = new SynchronizationContext ();
#endif
				return currentContext;
			}
		}

		public virtual SynchronizationContext CreateCopy ()
		{
			return new SynchronizationContext (this);
		}

		public bool IsWaitNotificationRequired ()
		{
			return notification_required;
		}

		public virtual void OperationCompleted ()
		{
		}

		public virtual void OperationStarted ()
		{
		}
		
		public virtual void Post (SendOrPostCallback d, object state)
		{
#if NET_2_1
			// WARNING: This is a horrible hack that short-circuites the SynchronizationContext
			// logic for moonlight until we finish our Dispatcher implementation.  This is only
			// put in place so that we can run moon-unit in the interim.
			Console.WriteLine ("WARNING: SynchronizationContext short-circuited to UI thread hack.");
			callback = (GSourceFunc) delegate (IntPtr ctx) { d(state); return false; };
			g_idle_add (callback, IntPtr.Zero);
#else
			ThreadPool.QueueUserWorkItem (new WaitCallback (d), state);
#endif
		}
		
		public virtual void Send (SendOrPostCallback d, object state)
		{
			d (state);
		}
		
		public static void SetSynchronizationContext (SynchronizationContext syncContext)
		{
			currentContext = syncContext;
		}

		[MonoTODO]
		protected void SetWaitNotificationRequired ()
		{
			notification_required = true;
			throw new NotImplementedException ();
		}

		[CLSCompliant (false)]
		[PrePrepareMethod ()]
		public virtual int Wait (IntPtr[] waitHandles, bool waitAll, int millisecondsTimeout)
		{
			return WaitHelper (waitHandles, waitAll, millisecondsTimeout);
		}

		[MonoTODO]
		[CLSCompliant (false)]
		[ReliabilityContract (Consistency.WillNotCorruptState, Cer.MayFail)]
		[PrePrepareMethod ()]
		protected static int WaitHelper (IntPtr[] waitHandles, bool waitAll, int millisecondsTimeout)
		{
			throw new NotImplementedException ();
		}

#if NET_2_1
		GSourceFunc callback;

                public delegate bool GSourceFunc (IntPtr data);

                [DllImport ("moon")]
                static extern uint g_idle_add (GSourceFunc callback, IntPtr data);
#endif
	}
}

#endif
