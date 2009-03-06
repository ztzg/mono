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

	static long TestBitwiseAnd(long a, long b)
	{
		return a&b;
	}

	static long TestBitwiseAndImm(long a)
	{
		return (long)-126000 & a;
	}

	static long TestBitwiseOr(long a, long b)
	{
		return a | b;
	}

	static long TestBitwiseOrImm(long a)
	{
		return a | (long) 0x555;
	}

	static long TestXor(long a, long b)
	{
		return a^b;
	}

	static long TestXorImm(long a)
	{
		return a^0xFF00;
	}

	static long TestBitwiseNot(long a)
	{
		return (~a) ^ ((long)-1);
	}

	static long TestDiv(long a, long b)
	{
		if(b==(long)0) {
			return 0;
		} else {
			return a/b;
		}
	}

	static long TestMod(long a, long b)
	{
		if(b==(long)0) {
			return 0;
		} else {
			return a%b;
		}
	}

	static int Main(string[] args)
	{
		long result = TestAdd((long)args.Length, 5);

		result += TestAddImm((long)args.Length);

		result += TestSub((long)args.Length,535);

		result += TestSubImm((long)args.Length);

		result += TestMul((long)args.Length,437);

		result += TestNeg((long)(-150+args.Length));

		result += TestXor(5,(long)args.Length);

		result += TestXorImm((long)args.Length);

		result += TestBitwiseAnd((long)args.Length,0x55);

		result += TestBitwiseAndImm((long)args.Length);

		result += TestBitwiseOr((long)args.Length,0xFF);

		result += TestBitwiseOrImm((long)args.Length);

		result += TestBitwiseNot((long)args.Length + 5);

		result += TestDiv(25500,(long)args.Length);

		result += TestMod(25,(long)args.Length);

		return (int)(result);
	}
}
