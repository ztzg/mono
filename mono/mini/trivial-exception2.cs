using System;

class Class
{
	static int Test2(int test)
	{
		if (test > 4)
			throw new Exception();
		return test + 4;
	}

	static int Test(int test)
	{
		if (test > 2)
			return Test2(test);
		return test + 2;
	}

	static int Main(string[] args)
	{
		int result = 0;

		try {
			result = Test(args.Length);
		}
		catch {
			return args.Length;
		}

		return result;
	}
}
