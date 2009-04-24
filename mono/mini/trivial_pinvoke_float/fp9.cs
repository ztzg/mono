using System.Runtime.InteropServices;

class Dcall
{
	[DllImport ("fp.so")]
	private static extern double fp9 (float x1, int i1, double y1, float x2, int i2, double y2, float x3, int i3, double y3);

	static int Main()
	{
	  float x1 = 1.1f;
	  float x2 = 2.2f;
	  float x3 = 3.3f;
	  double y1 = 4.4;
	  double y2 = 5.5;
	  double y3 = 6.6;
	  int i1 = 7;
	  int i2 = 8;
	  int i3 = 9;
	  return (int)(fp9(x1,i1,y1,x2,i2,y2,x3,i3,y3));
	}
}
