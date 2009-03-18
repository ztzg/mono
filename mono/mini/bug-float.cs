class Class
{
	static int Main(string[] args)
	{
		/* Works fine if '(float)' is removed. */
		return (int)(args.Length + (float)0.7 * args.Length);
	}
}
