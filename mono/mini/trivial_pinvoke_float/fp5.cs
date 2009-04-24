using System.Runtime.InteropServices;

class Dcall
{
	[DllImport ("fp.so")]
	private static extern double fp5 (float x, int i, double y, int l, float x1, int i1, double y1, int l1, int l2);

	static int Main()
	{
	  float x = 1.1f;
	  double y = 4.4;
	  int i = 7;
	  int l = 8;
	  float x1 = 44.1f;
	  double y1 = 11.1;
	  int i1 = 7;
	  int l1 = 2;
	  int l2 = 20;
	  return (int)(fp5(x,i,y,l,x1,i1,y1,l1,l2));
	}
}
