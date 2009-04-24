using System.Runtime.InteropServices;

class Dcall
{
	[DllImport ("fp.so")]
	private static extern double fp1 (float x, int i, double y, long l);


	static int Main()
	{
	  float x = 1.1f;
	  double y = 4.4;
	  int i = 7;
	  long l = 8;
	  return (int)(fp1(x,i,y,l));
	}
}
