
class Class
{
    static int Main(string[] args)
    {
	return 	(byte)args.Length  + (sbyte)args.Length +
		(short)args.Length + (ushort)args.Length;
    }
}
