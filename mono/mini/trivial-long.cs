class Class
{
	static long TestMul(long a, long b)
	{
		return a*b;
	}

	static long TestSub(long a, long b)
	{
		return a - b;
	}

	static long TestAdd(long a, long b)
	{
		return a + b;
	}

	static long TestAddImm(long a)
	{
		return a + (long) 55;
	}

	static long TestSubImm(long a)
	{
		return a + (long) 365;
	}

	static long TestNeg(long a)
	{
		return -a;
	}

	static int Main(string[] args)
	{
		long result = TestAdd((long)args.Length, 5);

		result += TestAddImm((long)args.Length);

		result += TestSub((long)args.Length,535);

		result += TestSubImm((long)args.Length);

		result += TestMul((long)args.Length,437);

		result += TestNeg((long)(-150+args.Length));

		return (int)(result);
	}
}
