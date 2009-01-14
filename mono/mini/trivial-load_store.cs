class Byte
{
	public byte val = 3;
}
/*
class Short
{
	public short val = 8;
}
*/
class Int
{
	public int val = 5;
}

class Class
{
	static int Main()
	{
		Byte  var01 = new Byte();
		//Short var02 = new Short();
		Int   var03 = new Int();

		return var01.val + /* var02.val */ + var03.val;
	}
}
