//
// ParallelExecuter.cs
//
// Author:
//       Jérémie "Garuma" Laval <jeremie.laval@gmail.com>
//
// Copyright (c) 2010 Jérémie "Garuma" Laval
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#if NET_4_0
using System;
using System.Threading;
using System.Threading.Tasks;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq.Parallel.QueryNodes;

namespace System.Linq.Parallel
{
	internal static class ParallelExecuter
	{
		internal static QueryOptions CheckQuery<T> (QueryBaseNode<T> startingNode)
		{
			return CheckQuery<T> (startingNode, false);
		}

		internal static QueryOptions CheckQuery<T> (QueryBaseNode<T> startingNode, bool blocking)
		{
			return CheckQuery (startingNode, GetBestWorkerNumber (blocking));
		}

		internal static QueryOptions CheckQuery<T> (QueryBaseNode<T> startingNode, int partitionCount)
		{
			QueryCheckerVisitor visitor = new QueryCheckerVisitor (partitionCount);
			startingNode.Visit (visitor);

			return visitor.Options;
		}

		internal static CancellationToken Chain (this CancellationToken self, CancellationTokenSource other)
		{
			CancellationTokenSource linked = CancellationTokenSource.CreateLinkedTokenSource (self, other.Token);
			return linked.Token;
		}

		internal static bool IsOrdered<TSource> (this QueryBaseNode<TSource> source)
		{
			QueryIsOrderedVisitor visitor = new QueryIsOrderedVisitor ();
			source.Visit (visitor);

			return visitor.BehindOrderGuard;
		}

		internal static int GetBestWorkerNumber ()
		{
			return GetBestWorkerNumber (false);
		}

		internal static int GetBestWorkerNumber (bool blocking)
		{
			return blocking ? Environment.ProcessorCount + 1 : Environment.ProcessorCount;
		}

		internal static Task[] Process<TSource, TElement> (QueryBaseNode<TSource> node,
		                                                   Action<TElement, CancellationToken> call,
		                                                   Func<QueryBaseNode<TSource>, QueryOptions, IList<IEnumerable<TElement>>> acquisitionFunc,
		                                                   QueryOptions options)
		{
			return Process<TSource, TElement> (node, call, acquisitionFunc, null, options);
		}

		internal static Task[] Process<TSource, TElement> (QueryBaseNode<TSource> node,
		                                                   Action<TElement, CancellationToken> call,
		                                                   Func<QueryBaseNode<TSource>, QueryOptions, IList<IEnumerable<TElement>>> acquisitionFunc,
		                                                   Action endAction,
		                                                   QueryOptions options)
		{
			IList<IEnumerable<TElement>> enumerables = acquisitionFunc (node, options);

			Task[] tasks = new Task[enumerables.Count];
			CancellationTokenSource src
				= CancellationTokenSource.CreateLinkedTokenSource (options.ImplementerToken, options.Token);

			for (int i = 0; i < tasks.Length; i++) {
				int index = i;
				tasks[i] = Task.Factory.StartNew (() => {
					foreach (TElement item in enumerables[index]) {
						// This is from specific operators
						if (options.ImplementerToken.IsCancellationRequested)
							break;
						if (options.Token.IsCancellationRequested)
							throw new OperationCanceledException (options.Token);

						call (item, src.Token);
					}
					if (endAction != null)
						endAction ();
				  }, options.Token);
			}

			return tasks;
		}

		internal static void ProcessAndBlock<T> (QueryBaseNode<T> node, Action<T, CancellationToken> call)
		{
			QueryOptions options = CheckQuery (node, true);

			Task[] tasks = Process (node, call, (n, o) => n.GetEnumerables (o), options);
			Task.WaitAll (tasks, options.Token);
		}

		internal static Action ProcessAndCallback<T> (QueryBaseNode<T> node, Action<T, CancellationToken> call,
		                                              Action callback, QueryOptions options)
		{
			Task[] tasks = Process (node, call, (n, o) => n.GetEnumerables (o), options);
			if (callback != null)
				Task.Factory.ContinueWhenAll (tasks,  (_) => callback ());

			return () => Task.WaitAll (tasks, options.Token);
		}

		internal static Action ProcessAndCallback<T> (QueryBaseNode<T> node, Action<KeyValuePair<long, T>, CancellationToken> call,
		                                              Action endAction,
		                                              Action callback, QueryOptions options)
		{
			Task[] tasks = Process (node, call, (n, o) => n.GetOrderedEnumerables (o), endAction, options);
			if (callback != null)
				Task.Factory.ContinueWhenAll (tasks,  (_) => callback ());

			return () => Task.WaitAll (tasks, options.Token);
		}

		internal static void ProcessAndAggregate<T, U> (QueryBaseNode<T> node,
		                                                Func<U> seedFunc,
		                                                Func<U, T, U> localCall,
		                                                Action<IList<U>> call)
		{
			QueryOptions options = CheckQuery (node, true);

			IList<IEnumerable<T>> enumerables = node.GetEnumerables (options);
			U[] locals = new U[enumerables.Count];
			Task[] tasks = new Task[enumerables.Count];

			if (seedFunc != null) {
				for (int i = 0; i < locals.Length; i++)
					locals[i] = seedFunc ();
			}

			for (int i = 0; i < tasks.Length; i++) {
				int index = i;
				bool firstRun = true;

				tasks[i] = Task.Factory.StartNew (() => {
					foreach (T item in enumerables[index]) {
						// This is from specific operators
						if (options.ImplementerToken.IsCancellationRequested)
							break;
						if (options.Token.IsCancellationRequested)
							throw new OperationCanceledException (options.Token);

						if (firstRun && seedFunc == null) {
							firstRun = false;
							// HACK: TODO: omgwtfitsuckssomuch
							locals[index] = (U)(object)item;
							continue;
						}
						
						U acc = locals[index];
						locals[index] = localCall (acc, item);
					}
				}, options.Token);
			}

			Task.WaitAll (tasks, options.Token);

			if (call != null)
				call (locals);
		}
	}
}
#endif