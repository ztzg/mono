class Class
{
        static int test_shl0(int a)
	{	return a<<0;
	}

        static int test_shl1(int a)
	{	return a<<1;
	}

        static int test_shl2(int a)
	{	return a<<2;
	}

        static int test_shl8(int a)
	{	return a<<8;
	}

        static int test_shl16(int a)
	{	return a<<16;
	}

	static int Main(string[] args)
	{
		return (args.Length << 5) 	+
			test_shl0(args.Length)	+
			test_shl1(args.Length)	+
			test_shl2(args.Length)	+
			test_shl8(args.Length)	+
			test_shl16(args.Length)	;
	}
}
