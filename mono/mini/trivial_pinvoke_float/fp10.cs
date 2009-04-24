using System.Runtime.InteropServices;

class Dcall
{
	[DllImport ("fp.so")]
	private static extern double fp10 (double y, long l);

	static int Main()
	{
	  double y = 14.4;
	  long l = 8;	  
	  return (int)(fp10(y,l));
	}
}
