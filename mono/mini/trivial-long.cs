class Class
{
	static long Test(long a, long b)
	{
		return a + b;
	}

	static int Main(string[] args)
	{
		long result = Test((long)args.Length, 5);

		return (int)(result);
	}
}
