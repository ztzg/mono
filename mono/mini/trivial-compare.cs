class Class
{
	static int Main(string[] args)
	{
		int result = 0;

		if (args.Length < 2)
			result += 1;

		if (args.Length <= 2)
			result += 2;

		if (args.Length > 2)
			result += 4;

		if (args.Length >= 2)
			result += 8;

		return result;
	}
}
