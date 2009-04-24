using System.Runtime.InteropServices;

class Fcall
{
	[DllImport ("fp.so")]
	private static extern float fp11 (float x, float y);

	static int Main()
	{
	  float a = 1.234f;
	  float b = 4.321f;
	  return (int)fp11(a,b);
	}
}
