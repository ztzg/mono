using System.Runtime.InteropServices;

class Dcall
{
	[DllImport ("fp.so")]
	private static extern double fp12 (double x, double y, double z, double t, double u, double v);

	static int Main()
	{
	  double a = 1.234;
	  double b = 5.321;
	  double c = 1.234;
	  double d = 5.321;
	  double e = 1.234;
	  double f = 5.321;
	  return (int)(fp12(a,b,c,d,e,f));
	}
}
