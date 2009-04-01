class Dcall
{
	static double f1 (double y, long l)
	{ 
	  return y-(double)l; 
	}

	static int Main()
	{
	  double y = 14.4;
	  long l = 8;	  
	  return (int)(f1(y,l));
	}
}
