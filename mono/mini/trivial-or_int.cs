/* Testing or and xor operators.*/

/*
 * An equivalent "inlined" version is available in
 * trivial-or_int2.cs.
 *
 */
class Class
{
        static int test_or(int a)
	{	return a | 0x3;
	}

        static int test_or2(int a)
	{	return a | 0xFF4;
	}

        static int test_xor(int a)
	{	return a ^ 0x3;
	}

        static int test_xor2(int a)
	{	return a ^ 0xFF4;
	}

	static int Main(string[] args)
	{
		return	test_or(args.Length)    +
			test_or2(args.Length)	+
			test_xor(args.Length)	+
			test_xor2(args.Length)	;
	}
}
