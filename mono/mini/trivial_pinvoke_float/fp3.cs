using System.Runtime.InteropServices;

class Dcall
{
	[DllImport ("fp.so")]
	private static extern double fp3 (double x, int i, double y, int l);

	static int Main()
	{
	  double x = 1.1f;
	  double y = 4.4f;
	  int i = 7;
	  int l = 8;
	  return (int)(fp3(x,i,y,l));
	}
}
