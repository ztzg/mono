double fp1 (float x, int i, double y, long long l)
{
	return (double)x+(double)i*y-(double)l;
}

float fp2 (float x, int i, float y, int l)
{
	return (float)x+(float)i*y-(float)l;
}

double fp3 (double x, int i, double y, int l)
{
	return x+(double)i*y-(double)l;
}

double fp4 (float x, int i, double y, int l, float x1, int i1, double y1, int l1)
{
	return (double)x+(double)i*y-(double)l+(double)x1/(double)i1-y1*(double)l1;
}

double fp5 (float x, int i, double y, int l, float x1, int i1, double y1, int l1, int l2)
{
	return (double)x+(double)i*y-(double)l+(double)x1/(double)i1-y1*(double)l1+(double)l2;
}

double fp6 (float x1, int i1, long long l1, double y1, float x2, int i2, long long l2, double y2, float x3, int i3, long long l3, double y3)
{
	return x1+(double)i1+(double)l1+(double)y1+x2+(double)i2+(double)l2+(double)y2+x3+(double)i3+(double)l3+(double)y3;
}

double fp7 (double x, double y, double z, double t, double u, double v, double w)
{
	return x+y+z+t+u+v+w;
}

float fp8 (float x, float y, float z, float t, float u, float v, float w)
{
	return x+y+z+t+u+v+w;
}

double fp9 (float x1, int i1, double y1, float x2, int i2, double y2, float x3, int i3, double y3)
{
	return x1+(double)i1+(double)y1+x2+(double)i2+(double)y2+x3+(double)i3+(double)y3;
}

double fp10 (double y, long long l)
{
	return y-(double)l;
}

float fp11 (float x, float y)
{
	return x+y;
}

double fp12 (double x, double y, double z, double t, double u, double v)
{
	//return x+y-z*t/u%v;
	return x+y-z*t/u+v;
}
