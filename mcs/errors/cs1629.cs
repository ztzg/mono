// CS1629: Unsafe code may not appear in iterators
// Line: 8
// Compiler options: -unsafe

using System.Collections;

class X {
	static unsafe IEnumerator GetIt ()
	{
	    yield return 1;
	}
	
}