class Class
{
	public virtual int virtual_method(int v)
	{
		return v;
	}

	static int Main()
	{
		Class test = new Class();

		return test.virtual_method(7) + test.virtual_method(3) + test.virtual_method(3);
	}
}
