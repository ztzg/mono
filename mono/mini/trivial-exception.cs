using System;

class Class
{
	static int Test(int test)
	{
		if (test > 3)
			throw new Exception();
		return test + 3;
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
