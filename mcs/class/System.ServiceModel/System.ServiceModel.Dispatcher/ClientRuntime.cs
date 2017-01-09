//
// ClientRuntime.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2005-2006 Novell, Inc.  http://www.novell.com
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
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;

namespace System.ServiceModel.Dispatcher
{
	public sealed class ClientRuntime
	{
		Type callback, contract;
		SynchronizedCollection<IChannelInitializer> channel_initializers
			= new SynchronizedCollection<IChannelInitializer> ();
		SynchronizedCollection<IInteractiveChannelInitializer> interactive_channel_initializers
			= new SynchronizedCollection<IInteractiveChannelInitializer> ();
#if !NET_2_1
		DispatchRuntime dispatch;
#endif
		SynchronizedCollection<IClientMessageInspector> inspectors
			= new SynchronizedCollection<IClientMessageInspector> ();
		ClientOperation.ClientOperationCollection operations
			= new ClientOperation.ClientOperationCollection ();
		IClientOperationSelector selector;
		Uri via;
		bool validate, manual_addressing;
		string contract_name, contract_ns;
		int max_fault_size = 0x10000; // FIXME: not verified.

#if !NET_2_1
		// .ctor() for DispatchRuntime.CallbackClientRuntime
		internal ClientRuntime (DispatchRuntime parent)
		{
			// maybe it could be null, for non-duplex channels.
			this.dispatch = parent;
			contract_name = dispatch.EndpointDispatcher.ContractName;
			contract_ns = dispatch.EndpointDispatcher.ContractNamespace;
		}
#endif

		// .ctor() for Clients
		internal ClientRuntime (ServiceEndpoint endpoint)
		{
			contract_name = endpoint.Contract.Name;
			contract_ns = endpoint.Contract.Namespace;
			contract = endpoint.Contract.ContractType;
		}

		public Type CallbackClientType {
			get { return callback; }
			set { callback = value; }
		}

		public SynchronizedCollection<IChannelInitializer> ChannelInitializers {
			get { return channel_initializers; }
		}

		public SynchronizedCollection<IInteractiveChannelInitializer> InteractiveChannelInitializers {
			get { return interactive_channel_initializers;}
		}

		public string ContractName {
			get { return contract_name; }
		}

		public string ContractNamespace {
			get { return contract_ns; }
		}

		
		public Type ContractClientType {
			get { return contract; }
			set { contract = value; }
		}

#if !NET_2_1
		public DispatchRuntime CallbackDispatchRuntime {
			get { return dispatch; }
		}
#endif

		public SynchronizedCollection<IClientMessageInspector> MessageInspectors {
			get { return inspectors; }
		}

		public SynchronizedKeyedCollection<string,ClientOperation> Operations {
			get { return operations; }
		}

		public bool ManualAddressing {
			get { return manual_addressing; }
			set { manual_addressing = value; }
		}

		public int MaxFaultSize {
			get { return max_fault_size; }
			set { max_fault_size = value; }
		}

		public IClientOperationSelector OperationSelector {
			get { return selector; }
			set { selector = value; }
		}

		public bool ValidateMustUnderstand {
			get { return validate; }
			set { validate = value; }
		}

		public Uri Via {
			get { return via; }
			set { via = value; }
		}

		[MonoTODO]
		public ClientOperation UnhandledClientOperation {
			get { throw new NotImplementedException (); }
		}
	}
}
