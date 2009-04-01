class Class
{
	static double test01(int a)
	{
		return 1.3 * a;
	}

	static float test02(int a)
	{
		return (float)0.3 * a;
	}

	static int Main(string[] args)
	{
		return (int)(test01(args.Length) + (double)test02(args.Length));
	}
}
