using System;
using System.Reflection;


public class TestDriver {

	static public int RunTests (Type type, string[] args) {
		int failed = 0, ran = 0;
		int result, expected, elen;
		int i, j;
		string name;
		MethodInfo[] methods;
		bool do_timings = false;
		bool verbose = false;
		int tms = 0;

		if (args != null && args.Length > 0) {
			for (j = 0; j < args.Length; j++) {
				bool found = false;
				if (args [j] == "--time") {
					do_timings = true;
					found = true;
				}
				if ((args [j] == "-v") || (args [j] == "--verbose")) {
					verbose = true;
					found = true;
				}

				if (found) {
					string[] new_args = new string [args.Length - 1];
					for (i = 0; i < j; ++i)
						new_args [i] = args [i];
					j++;
					for (; j < args.Length; ++i, ++j)
						new_args [i] = args [j];
					args = new_args;
					break;
				}
			}
		}
		methods = type.GetMethods (BindingFlags.Public|BindingFlags.NonPublic|BindingFlags.Static);
		for (i = 0; i < methods.Length; ++i) {
			name = methods [i].Name;
			if (!name.StartsWith ("test_"))
				continue;
			if (args != null && args.Length > 0) {
				bool found = false;
				for (j = 0; j < args.Length; j++) {
					if (name.EndsWith (args [j])) {
						found = true;
						break;
					}
				}
				if (!found)
					continue;
			}
			for (j = 5; j < name.Length; ++j)
				if (!Char.IsDigit (name [j]))
					break;

			Console.Write ("Running '{0}': ", name);
			expected = Int32.Parse (name.Substring (5, j - 5));
			result = (int)methods [i].Invoke (null, null);
			ran++;
			if (result != expected) {
				failed++;
				//Console.WriteLine ("{0} failed: got {1}, expected {2}", name, result, expected);
				Console.WriteLine ("failed");
			}
			else {
				Console.WriteLine ("succeed");
			}
		}
		//Console.WriteLine ("Regression tests: {0} ran, {1} failed in {2}", ran, failed, type);
		//Console.WriteLine ("Regression tests: {0} ran, {1} failed in [{2}]{3}", ran, failed, type.Assembly.GetName().Name, type);
		return failed;
	}
	static public int RunTests (Type type) {
		return RunTests (type, null);
	}
}

