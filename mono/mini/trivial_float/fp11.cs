class Fcall
{
	static float f1 (float x, float y)
	{ 
	  return x+y; 
	}

	static int Main()
	{
	  float a = 1.234f;
	  float b = 4.321f;
	  return (int)f1(a,b);
	}
}
