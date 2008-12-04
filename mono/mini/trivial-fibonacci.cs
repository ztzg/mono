class Class
{
	static int Fibonacci(int n)
	{
		if (n == 0 || n == 1)
			return n;
		else
			return Fibonacci(n - 1) + Fibonacci(n - 2);
	}

	static int Main(string[] args)
	{
		return Fibonacci(args.Length);
	}
}
