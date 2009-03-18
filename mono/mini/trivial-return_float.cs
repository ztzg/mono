class Class
{
	static double test01(int a)
	{
		return 1.3 * a;
	}

	static float test02(int a)
	{
		return (float)0.7 * a;
	}

	static int Main(string[] args)
	{
		return (int)(test01(args.Length) + test02(args.Length));
	}
}
