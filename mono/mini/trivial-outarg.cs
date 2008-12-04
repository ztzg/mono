class Class
{
	static int OutArg(int reg1, int reg2, int reg3, int reg4, int stack1, int stack2)
	{
		if (stack2 != 13) {
			OutArg(reg1, reg2, reg3, reg4, stack1, stack2);
		}

		return reg1 + reg2 + reg3 + reg4 + stack1 + stack2;
	}

	static int Main(string[] args)
	{
		return OutArg(args.Length + 1, args.Length + 2, args.Length + 3, args.Length + 4, args.Length + 5, 13);
	}
}
