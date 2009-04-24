using System.Runtime.InteropServices;

class Dcall
{
	[DllImport ("fp.so")]
	private static extern float fp2 (float x, int i, float y, int l);

	static int Main()
	{
	  float x = 1.1f;
	  float y = 4.4f;
	  int i = 7;
	  int l = 8;
	  return (int)(fp2(x,i,y,l));
	}
}
