
struct Struct
{
        public int a;
        public byte b;
        public short c;
        public long d;
}

class Class
{
	static int pass_simple1 (Struct a, int b) {
 
                return a.a + (int)a.b + (int) a.c + (int) a.d + b;
        }

	static int pass_simple2 (int b, int c, int d, int e, Struct a) {


		return a.a + (int)a.b + (int) a.c + (int) a.d + b + c + d + e;
	}
 
        static int Main(string[] args)
        {
                Struct v = new Struct();
 
		v.a = 1;
		v.b = 2;
		v.c = 3;
		v.d = 4;

		return 	pass_simple1(v,args.Length) +
			pass_simple2(args.Length,1,2,3,v);
        }
}

