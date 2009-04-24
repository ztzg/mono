using System.Runtime.InteropServices;

class Dcall
{
	[DllImport ("fp.so")]
	private static extern double fp7 (double x, double y, double z, double t, double u, double v, double w);

	static int Main()
	{
	  double a = 1.1;
	  double b = 2.2;
	  double c = 3.3;
	  double d = 4.4;
	  double e = 5.5;
	  double f = 6.6;
	  double g = 7.7;
	  return (int)(fp7(a,b,c,d,e,f,g));
	}
}
