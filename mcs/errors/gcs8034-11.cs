// CS8034: Contravariant type parameters can only be used as type arguments in contravariant positions
// Line: 11
// Compiler options: -langversion:future

interface A<out T>
{
}

interface B<in T>
{
	A<T> A { get; }
}
