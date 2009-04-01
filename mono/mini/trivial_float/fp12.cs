class Dcall
{
	static double f1 (double x, double y, double z, double t, double u, double v)
	{ 
	  return x+y-z*t/u%v; 
	}

	static int Main()
	{
	  double a = 1.234;
	  double b = 5.321;
	  double c = 1.234;
	  double d = 5.321;
	  double e = 1.234;
	  double f = 5.321;
	  return (int)(f1(a,b,c,d,e,f));
	}
}
