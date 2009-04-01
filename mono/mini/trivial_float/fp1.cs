class Dcall
{
	static double f1 (float x, int i, double y, long l)
	{ 
	  return (double)x+(double)i*y-(double)l; 
	}

	static int Main()
	{
	  float x = 1.1f;
	  double y = 4.4;
	  int i = 7;
	  long l = 8;
	  return (int)(f1(x,i,y,l));
	}
}
