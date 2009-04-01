class Dcall
{
	static double f1 (float x1, int i1, long l1, double y1, float x2, int i2, long l2, double y2, float x3, int i3, long l3, double y3)
	{ 
	  return x1+(double)i1+(double)l1+(double)y1+x2+(double)i2+(double)l2+(double)y2+x3+(double)i3+(double)l3+(double)y3; 
	}

	static int Main()
	{
	  float x1 = 1.1f;
	  float x2 = 2.2f;
	  float x3 = 3.3f;
	  double y1 = 4.4;
	  double y2 = 5.5;
	  double y3 = 6.6;
	  int i1 = 7;
	  int i2 = 8;
	  int i3 = 9;
	  long l1 = 10;
	  long l2 = 11;
	  long l3 = 12;
	  return (int)(f1(x1,i1,l1,y1,x2,i2,l2,y2,x3,i3,l3,y3));
	}
}
