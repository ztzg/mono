class Class
{
	static int Main(string[] args)
	{
		switch (args.Length) {
		case 1:
			return 3 - args.Length;
		case 2:
			return 3 + args.Length;
		case 3:
			return 3 * args.Length;
		default:
			return 3;
		}
	}
}
