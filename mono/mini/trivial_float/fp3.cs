class Dcall
{
	static double f1 (double x, int i, double y, int l)
	{ 
	  return x+(double)i*y-(double)l; 
	}

	static int Main()
	{
	  double x = 1.1f;
	  double y = 4.4f;
	  int i = 7;
	  int l = 8;
	  return (int)(f1(x,i,y,l));
	}
}
