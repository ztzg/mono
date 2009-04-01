class Dcall
{
	static float f1 (float x, int i, float y, int l)
	{ 
	  return (float)x+(float)i*y-(float)l; 
	}

	static int Main()
	{
	  float x = 1.1f;
	  float y = 4.4f;
	  int i = 7;
	  int l = 8;
	  return (int)(f1(x,i,y,l));
	}
}
