//
// System.Runtime.Remoting.Metadata.W3cXsd2001.SoapNormalizedString
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
	public sealed class SoapNormalizedString : ISoapXsd
	{
		string _value;
		
		public SoapNormalizedString()
		{
		}
		
		public SoapNormalizedString (string value)
		{
			_value = SoapHelper.Normalize (value);
		}
		
		public string Value {
			get { return _value; } 
			set { _value = value; }
		}

		public static string XsdType {
			get { return "normalizedString"; }
		}

		public string GetXsdType()
		{
			return XsdType;
		}
		
		public static SoapNormalizedString Parse (string value)
		{
			return new SoapNormalizedString (value);
		}

		public override string ToString()
		{
			return _value;
		}
	}
}
