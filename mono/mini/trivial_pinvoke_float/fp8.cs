using System.Runtime.InteropServices;


class Dcall
{
	[DllImport ("fp.so")]
	private static extern float fp8 (float x, float y, float z, float t, float u, float v, float w);

	static int Main()
	{
	  float a = 1.1f;
	  float b = 2.2f;
	  float c = 3.3f;
	  float d = 4.4f;
	  float e = 5.5f;
	  float f = 6.6f;
	  float g = 7.7f;
	  return (int)(fp8(a,b,c,d,e,f,g));
	}
}
