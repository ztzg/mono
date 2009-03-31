
struct Struct
{
	public int a;
	public byte b;
	public short c;
	public long d;
}
 
class Class
{
	static Struct get_simple (int v) {
		Struct r = new Struct ();
		r.a = v;
		r.b = (byte)(v + 1);
		r.c = (short)(v + 2);
		r.d = v + 3;

		return r;
	}

	static int Main(string[] args)
	{
		Struct v = get_simple (1);

		if (v.a != 1)
			return 0;
		if (v.b != 2)
			return 0;
		if (v.c != 3)
			return 0;
		if (v.d != 4)
			return 0;

		Struct w = get_simple (args.Length);

		return 3 + w.a;
	}
}
