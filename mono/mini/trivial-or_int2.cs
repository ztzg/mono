/* Testing or and xor operators. */

/*
 * trivial-or_int2.cs is equivalent to trivial-or_int.cs. The main
 * difference comes from the fact that code is already inlined.
 *
 */
class Class
{
	static int Main(string[] args)
	{
		return	(args.Length | 0x3)     +
			(args.Length | 0xFF4)	+
			(args.Length ^ 0x3)	+
			(args.Length ^ 0xFF4)	;
	}
}
