class Class
{
	static int Main(string[] args)
	{
		double value = (double)args.Length;

		value += 3.2 * value;
		value -= 5.7 * value;
		value = -value;

		return (int)value;
	}
}
