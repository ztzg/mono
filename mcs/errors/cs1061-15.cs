// CS1061: Type `int' does not contain a definition for `GetValueOrDefault' and no extension method `GetValueOrDefault' of type `int' could be found. Are you missing an assembly reference?
// Line: 9

class C
{
	static void Main ()
	{
		int? i = 4;
		var m = i?.GetValueOrDefault ();
	}
}