//
// System.Runtime.Remoting.Metadata.W3cXsd2001.SoapIdref
//
// Authors:
//      Martin Willemoes Hansen (mwh@sysrq.dk)
//      Lluis Sanchez Gual (lluis@ximian.com)
//
// (C) 2003 Martin Willemoes Hansen
//

using System;

namespace System.Runtime.Remoting.Metadata.W3cXsd2001 
{
	[Serializable]
	public sealed class SoapIdref : ISoapXsd
	{
		string _value;
		
		public SoapIdref()
		{
		}
		
		public SoapIdref (string value)
		{
			_value = SoapHelper.Normalize (value);
		}
		
		public string Value {
			get { return _value; } 
			set { _value = value; }
		}

		public static string XsdType {
			get { return "IDREF"; }
		}

		public string GetXsdType()
		{
			return XsdType;
		}
		
		public static SoapIdref Parse (string value)
		{
			return new SoapIdref (value);
		}

		public override string ToString()
		{
			return _value;
		}
	}
}
