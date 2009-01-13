class Class
{
	static int OutArg(int reg1, int reg2, int reg3, int reg4, int stack1, int stack2, int stack3, int stack4, int stack5, int stack6, int stack7)
	{
		if (stack7 != 13) {
			OutArg(reg1, reg2, reg3, reg4, stack1, stack2, stack3, stack4, stack5, stack6, 13);
		}

		return reg1 + reg2 + reg3 + reg4 + stack1 + stack2 + stack3 + stack4 + stack5 + stack6 + stack7;
	}

	static int Main(string[] args)
	{
		return OutArg(args.Length + 1, args.Length + 2, args.Length + 3, args.Length + 4, args.Length + 5, args.Length + 6, args.Length + 7, args.Length + 8, args.Length + 9, args.Length + 10, 15);
	}
}
