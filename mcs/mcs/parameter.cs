//
// parameter.cs: Parameter definition.
//
// Author: Miguel de Icaza (miguel@gnu.org)
//         Marek Safar (marek.safar@seznam.cz)
//
// Dual licensed under the terms of the MIT X11 or GNU GPL
//
// Copyright 2001-2003 Ximian, Inc (http://www.ximian.com)
// Copyright 2003-2008 Novell, Inc. 
//
//
using System;
using System.Reflection;
using System.Reflection.Emit;
using System.Collections;
using System.Text;

namespace Mono.CSharp {

	/// <summary>
	///   Abstract Base class for parameters of a method.
	/// </summary>
	public abstract class ParameterBase : Attributable {

		protected ParameterBuilder builder;

		protected ParameterBase (Attributes attrs)
			: base (attrs)
		{
		}

		public override void ApplyAttributeBuilder (Attribute a, CustomAttributeBuilder cb, PredefinedAttributes pa)
		{
#if !NET_2_0
			if (a.Type == pa.MarshalAs) {
				UnmanagedMarshal marshal = a.GetMarshal (this);
				if (marshal != null) {
					builder.SetMarshal (marshal);
				}
				return;
			}
#endif
			if (a.HasSecurityAttribute) {
				a.Error_InvalidSecurityParent ();
				return;
			}

			builder.SetCustomAttribute (cb);
		}

		public override bool IsClsComplianceRequired()
		{
			return false;
		}
	}

	/// <summary>
	/// Class for applying custom attributes on the return type
	/// </summary>
	public class ReturnParameter : ParameterBase {
		public ReturnParameter (MethodBuilder mb, Location location):
			base (null)
		{
			try {
				builder = mb.DefineParameter (0, ParameterAttributes.None, "");			
			}
			catch (ArgumentOutOfRangeException) {
				Report.RuntimeMissingSupport (location, "custom attributes on the return type");
			}
		}

		public override void ApplyAttributeBuilder (Attribute a, CustomAttributeBuilder cb, PredefinedAttributes pa)
		{
			if (a.Type == pa.CLSCompliant) {
				Report.Warning (3023, 1, a.Location, "CLSCompliant attribute has no meaning when applied to return types. Try putting it on the method instead");
			}

			// This occurs after Warning -28
			if (builder == null)
				return;

			base.ApplyAttributeBuilder (a, cb, pa);
		}

		public override AttributeTargets AttributeTargets {
			get {
				return AttributeTargets.ReturnValue;
			}
		}

		public override IResolveContext ResolveContext {
			get {
				throw new NotSupportedException ();
			}
		}

		/// <summary>
		/// Is never called
		/// </summary>
		public override string[] ValidAttributeTargets {
			get {
				return null;
			}
		}
	}

	/// <summary>
	/// Class for applying custom attributes on the implicit parameter type
	/// of the 'set' method in properties, and the 'add' and 'remove' methods in events.
	/// </summary>
	/// 
	// TODO: should use more code from Parameter.ApplyAttributeBuilder
	public class ImplicitParameter : ParameterBase {
		public ImplicitParameter (MethodBuilder mb):
			base (null)
		{
			builder = mb.DefineParameter (1, ParameterAttributes.None, "");			
		}

		public override AttributeTargets AttributeTargets {
			get {
				return AttributeTargets.Parameter;
			}
		}

		public override IResolveContext ResolveContext {
			get {
				throw new NotSupportedException ();
			}
		}

		/// <summary>
		/// Is never called
		/// </summary>
		public override string[] ValidAttributeTargets {
			get {
				return null;
			}
		}
	}

	public class ImplicitLambdaParameter : Parameter
	{
		public ImplicitLambdaParameter (string name, Location loc)
			: base (null, name, Modifier.NONE, null, loc)
		{
		}

		public override Type Resolve (IResolveContext ec)
		{
			if (parameter_type == null)
				throw new InternalErrorException ("A type of implicit lambda parameter `{0}' is not set",
					Name);

			return parameter_type;
		}

		public Type Type {
			set { parameter_type = value; }
		}
	}

	public class ParamsParameter : Parameter {
		public ParamsParameter (FullNamedExpression type, string name, Attributes attrs, Location loc):
			base (type, name, Parameter.Modifier.PARAMS, attrs, loc)
		{
		}

		public override Type Resolve (IResolveContext ec)
		{
			if (base.Resolve (ec) == null)
				return null;

			if (!parameter_type.IsArray || parameter_type.GetArrayRank () != 1) {
				Report.Error (225, Location, "The params parameter must be a single dimensional array");
				return null;
			}

			return parameter_type;
		}

		public override void ApplyAttributes (MethodBuilder mb, ConstructorBuilder cb, int index)
		{
			base.ApplyAttributes (mb, cb, index);
			PredefinedAttributes.Get.ParamArray.EmitAttribute (builder, Location);
		}
	}

	public class ArglistParameter : Parameter {
		// Doesn't have proper type because it's never chosen for better conversion
		public ArglistParameter (Location loc) :
			base (null, String.Empty, Parameter.Modifier.ARGLIST, null, loc)
		{
		}

		public override void  ApplyAttributes (MethodBuilder mb, ConstructorBuilder cb, int index)
		{
			// Nothing to do
		}

		public override bool CheckAccessibility (InterfaceMemberBase member)
		{
			return true;
		}

		public override Type Resolve (IResolveContext ec)
		{
			return typeof (ArglistParameter);
		}

		public override string GetSignatureForError ()
		{
			return "__arglist";
		}
	}

	public interface IParameterData
	{
		bool HasExtensionMethodModifier { get; }
		Parameter.Modifier ModFlags { get; }
		string Name { get; }
	}

	//
	// Parameter information created by parser
	//
	public class Parameter : ParameterBase, IParameterData, ILocalVariable {
		[Flags]
		public enum Modifier : byte {
			NONE    = 0,
			REF     = REFMASK | ISBYREF,
			OUT     = OUTMASK | ISBYREF,
			PARAMS  = 4,
			// This is a flag which says that it's either REF or OUT.
			ISBYREF = 8,
			ARGLIST = 16,
			REFMASK	= 32,
			OUTMASK = 64,
			This	= 128
		}

		static string[] attribute_targets = new string [] { "param" };

		protected FullNamedExpression TypeName;
		readonly Modifier modFlags;
		string name;
		protected Type parameter_type;
		public readonly Location Location;
		int idx;
		public bool HasAddressTaken;

		IResolveContext resolve_context;
		LocalVariableReference expr_tree_variable;
		static TypeExpr parameter_expr_tree_type;

		public HoistedVariable HoistedVariableReference;

		public Parameter (FullNamedExpression type, string name, Modifier mod, Attributes attrs, Location loc)
			: base (attrs)
		{
			if (type == TypeManager.system_void_expr)
				Report.Error (1536, loc, "Invalid parameter type `void'");

			this.name = name;
			modFlags = mod;
			Location = loc;
			TypeName = type;
		}

		public override void ApplyAttributeBuilder (Attribute a, CustomAttributeBuilder cb, PredefinedAttributes pa)
		{
			if (a.Type == pa.In && ModFlags == Modifier.OUT) {
				Report.Error (36, a.Location, "An out parameter cannot have the `In' attribute");
				return;
			}

			if (a.Type == pa.ParamArray) {
				Report.Error (674, a.Location, "Do not use `System.ParamArrayAttribute'. Use the `params' keyword instead");
				return;
			}

			if (a.Type == PredefinedAttributes.Get.Out && (ModFlags & Modifier.REF) == Modifier.REF &&
			    !OptAttributes.Contains (pa.In)) {
				Report.Error (662, a.Location,
					"Cannot specify only `Out' attribute on a ref parameter. Use both `In' and `Out' attributes or neither");
				return;
			}

			if (a.Type == pa.CLSCompliant) {
				Report.Warning (3022, 1, a.Location, "CLSCompliant attribute has no meaning when applied to parameters. Try putting it on the method instead");
			}

			// TypeManager.default_parameter_value_attribute_type is null if !NET_2_0, or if System.dll is not referenced
			if (a.Type == pa.DefaultParameterValue) {
				object val = a.GetParameterDefaultValue ();
				if (val != null) {
					Type t = val.GetType ();
					if (t.IsArray || TypeManager.IsSubclassOf (t, TypeManager.type_type)) {
						if (parameter_type == TypeManager.object_type) {
							if (!t.IsArray)
								t = TypeManager.type_type;

							Report.Error (1910, a.Location, "Argument of type `{0}' is not applicable for the DefaultValue attribute",
								TypeManager.CSharpName (t));
						} else {
							Report.Error (1909, a.Location, "The DefaultValue attribute is not applicable on parameters of type `{0}'",
								TypeManager.CSharpName (parameter_type)); ;
						}
						return;
					}
				}

				if (parameter_type == TypeManager.object_type ||
				    (val == null && !TypeManager.IsGenericParameter (parameter_type) && TypeManager.IsReferenceType (parameter_type)) ||
				    (val != null && TypeManager.TypeToCoreType (val.GetType ()) == parameter_type))
					builder.SetConstant (val);
				else
					Report.Error (1908, a.Location, "The type of the default value should match the type of the parameter");
				return;
			}

			base.ApplyAttributeBuilder (a, cb, pa);
		}
		
		public virtual bool CheckAccessibility (InterfaceMemberBase member)
		{
			if (parameter_type == null || TypeManager.IsGenericParameter (parameter_type))
				return true;

			return member.IsAccessibleAs (parameter_type);
		}

		public override IResolveContext ResolveContext {
			get {
				return resolve_context;
			}
		}

		// <summary>
		//   Resolve is used in method definitions
		// </summary>
		public virtual Type Resolve (IResolveContext ec)
		{
			// HACK: to resolve attributes correctly
			this.resolve_context = ec;

			if (parameter_type != null)
				return parameter_type;

			TypeExpr texpr = TypeName.ResolveAsTypeTerminal (ec, false);
			if (texpr == null)
				return null;

			parameter_type = texpr.Type;

			if ((modFlags & Parameter.Modifier.ISBYREF) != 0 &&
				TypeManager.IsSpecialType (parameter_type)) {
				Report.Error (1601, Location, "Method or delegate parameter cannot be of type `{0}'",
					GetSignatureForError ());
				return null;
			}

#if GMCS_SOURCE
			if (parameter_type.IsGenericParameter) {
				AbstractPropertyEventMethod accessor = ec as AbstractPropertyEventMethod;
				if (accessor == null || !accessor.IsDummy) {
					if ((parameter_type.GenericParameterAttributes & GenericParameterAttributes.Covariant) != 0) {
						Report.Error (-38, Location, "Covariant type parameters cannot be used as method parameters");
						return null;
					} else if ((ModFlags & Modifier.ISBYREF) != 0 &&
					           (parameter_type.GenericParameterAttributes & GenericParameterAttributes.Contravariant) != 0) {
						Report.Error (-37, Location, "Contravariant type parameters cannot be used in output positions");
						return null;
					}
				}
				return parameter_type;
			}
#endif

			if ((parameter_type.Attributes & Class.StaticClassAttribute) == Class.StaticClassAttribute) {
				Report.Error (721, Location, "`{0}': static types cannot be used as parameters",
					texpr.GetSignatureForError ());
				return parameter_type;
			}

			if ((modFlags & Modifier.This) != 0 && parameter_type.IsPointer) {
				Report.Error (1103, Location, "The type of extension method cannot be `{0}'",
					TypeManager.CSharpName (parameter_type));
			}

			return parameter_type;
		}

		public void ResolveVariable (int idx)
		{
			this.idx = idx;
		}

		public bool HasExtensionMethodModifier {
			get { return (modFlags & Modifier.This) != 0; }
		}

		public Modifier ModFlags {
			get { return modFlags & ~Modifier.This; }
		}

		public string Name {
			get { return name; }
			set { name = value; }
		}

		ParameterAttributes Attributes {
			get { return ParametersCompiled.GetParameterAttribute (modFlags); }
		}

		public override AttributeTargets AttributeTargets {
			get {
				return AttributeTargets.Parameter;
			}
		}

		public virtual string GetSignatureForError ()
		{
			string type_name;
			if (parameter_type != null)
				type_name = TypeManager.CSharpName (parameter_type);
			else
				type_name = TypeName.GetSignatureForError ();

			string mod = GetModifierSignature (modFlags);
			if (mod.Length > 0)
				return String.Concat (mod, " ", type_name);

			return type_name;
		}

		public static string GetModifierSignature (Modifier mod)
		{
			switch (mod) {
			case Modifier.OUT:
				return "out";
			case Modifier.PARAMS:
				return "params";
			case Modifier.REF:
				return "ref";
			case Modifier.This:
				return "this";
			default:
				return "";
			}
		}

		public void IsClsCompliant ()
		{
			if (AttributeTester.IsClsCompliant (parameter_type))
				return;

			Report.Warning (3001, 1, Location, "Argument type `{0}' is not CLS-compliant", GetSignatureForError ());
		}

		public virtual void ApplyAttributes (MethodBuilder mb, ConstructorBuilder cb, int index)
		{
			if (mb == null)
				builder = cb.DefineParameter (index, Attributes, Name);
			else
				builder = mb.DefineParameter (index, Attributes, Name);

			if (OptAttributes != null)
				OptAttributes.Emit ();
		}

		public override string[] ValidAttributeTargets {
			get {
				return attribute_targets;
			}
		}

		public Parameter Clone ()
		{
			Parameter p = (Parameter) MemberwiseClone ();
			if (attributes != null) {
				p.attributes = attributes.Clone ();
				p.attributes.AttachTo (p);
			}

			return p;
		}

		public ExpressionStatement CreateExpressionTreeVariable (EmitContext ec)
		{
			//
			// A parameter is not hoisted when used directly as ET
			//
			HoistedVariableReference = null;

			if ((modFlags & Modifier.ISBYREF) != 0)
				Report.Error (1951, Location, "An expression tree parameter cannot use `ref' or `out' modifier");

			LocalInfo variable = ec.CurrentBlock.AddTemporaryVariable (
				ResolveParameterExpressionType (ec, Location), Location);
			variable.Resolve (ec);

			expr_tree_variable = new LocalVariableReference (
				ec.CurrentBlock, variable.Name, Location, variable, false);

			ArrayList arguments = new ArrayList (2);
			arguments.Add (new Argument (new TypeOf (
				new TypeExpression (parameter_type, Location), Location)));
			arguments.Add (new Argument (new StringConstant (Name, Location)));
			return new SimpleAssign (ExpressionTreeVariableReference (),
				Expression.CreateExpressionFactoryCall ("Parameter", null, arguments, Location));
		}

		public void Emit (EmitContext ec)
		{
			int arg_idx = idx;
			if (!ec.IsStatic)
				arg_idx++;

			ParameterReference.EmitLdArg (ec.ig, arg_idx);
		}

		public void EmitAssign (EmitContext ec)
		{
			int arg_idx = idx;
			if (!ec.IsStatic)
				arg_idx++;

			if (arg_idx <= 255)
				ec.ig.Emit (OpCodes.Starg_S, (byte) arg_idx);
			else
				ec.ig.Emit (OpCodes.Starg, arg_idx);
		}

		public void EmitAddressOf (EmitContext ec)
		{
			int arg_idx = idx;

			if (!ec.IsStatic)
				arg_idx++;

			bool is_ref = (ModFlags & Modifier.ISBYREF) != 0;
			if (is_ref) {
				ParameterReference.EmitLdArg (ec.ig, arg_idx);
			} else {
				if (arg_idx <= 255)
					ec.ig.Emit (OpCodes.Ldarga_S, (byte) arg_idx);
				else
					ec.ig.Emit (OpCodes.Ldarga, arg_idx);
			}
		}

		public Expression ExpressionTreeVariableReference ()
		{
			return expr_tree_variable;
		}

		//
		// System.Linq.Expressions.ParameterExpression type
		//
		public static TypeExpr ResolveParameterExpressionType (EmitContext ec, Location location)
		{
			if (parameter_expr_tree_type != null)
				return parameter_expr_tree_type;

			Type p_type = TypeManager.parameter_expression_type;
			if (p_type == null) {
				p_type = TypeManager.CoreLookupType ("System.Linq.Expressions", "ParameterExpression", Kind.Class, true);
				TypeManager.parameter_expression_type = p_type;
			}

			parameter_expr_tree_type = new TypeExpression (p_type, location).
				ResolveAsTypeTerminal (ec, false);

			return parameter_expr_tree_type;
		}
	}

	//
	// Imported or resolved parameter information
	//
	public class ParameterData : IParameterData
	{
		readonly string name;
		readonly Parameter.Modifier modifiers;

		public ParameterData (string name, Parameter.Modifier modifiers)
		{
			this.name = name;
			this.modifiers = modifiers;
		}

		#region IParameterData Members

		public bool HasExtensionMethodModifier {
			get { return (modifiers & Parameter.Modifier.This) != 0; }
		}

		public Parameter.Modifier ModFlags {
			get { return modifiers & ~Parameter.Modifier.This; }
		}

		public string Name {
			get { return name; }
		}

		#endregion
	}

	public abstract class AParametersCollection
	{
		protected bool has_arglist;
		protected bool has_params;

		// Null object pattern
		protected IParameterData [] parameters;
		protected Type [] types;

		public int Count {
			get { return parameters.Length; }
		}

		public Type ExtensionMethodType {
			get {
				if (Count == 0)
					return null;

				return FixedParameters [0].HasExtensionMethodModifier ?
					types [0] : null;
			}
		}

		public IParameterData [] FixedParameters {
			get {
				return parameters;
			}
		}

		public static ParameterAttributes GetParameterAttribute (Parameter.Modifier modFlags)
		{
			return (modFlags & Parameter.Modifier.OUT) == Parameter.Modifier.OUT ?
				ParameterAttributes.Out : ParameterAttributes.None;
		}

		public Type [] GetEmitTypes ()
		{
			Type [] types = null;
			if (has_arglist) {
				if (Count == 1)
					return Type.EmptyTypes;

				types = new Type [Count - 1];
				Array.Copy (Types, types, types.Length);
			}

			for (int i = 0; i < Count; ++i) {
				if ((FixedParameters [i].ModFlags & Parameter.Modifier.ISBYREF) == 0)
					continue;

				if (types == null)
					types = (Type []) Types.Clone ();

				types [i] = TypeManager.GetReferenceType (types [i]);
			}

			if (types == null)
				types = Types;

			return types;
		}

		public string GetSignatureForError ()
		{
			StringBuilder sb = new StringBuilder ("(");
			for (int i = 0; i < Count; ++i) {
				if (i != 0)
					sb.Append (", ");
				sb.Append (ParameterDesc (i));
			}
			sb.Append (')');
			return sb.ToString ();
		}

		public bool HasArglist {
			get { return has_arglist; }
		}

		public bool HasExtensionMethodType {
			get {
				if (Count == 0)
					return false;

				return FixedParameters [0].HasExtensionMethodModifier;
			}
		}

		public bool HasParams {
			get { return has_params; }
		}

		public bool IsEmpty {
			get { return parameters.Length == 0; }
		}

		public string ParameterDesc (int pos)
		{
			if (types == null || types [pos] == null)
				return ((Parameter)FixedParameters [pos]).GetSignatureForError ();

			string type = TypeManager.CSharpName (types [pos]);
			if (FixedParameters [pos].HasExtensionMethodModifier)
				return "this " + type;

			Parameter.Modifier mod = FixedParameters [pos].ModFlags & ~Parameter.Modifier.ARGLIST;
			if (mod == 0)
				return type;

			return Parameter.GetModifierSignature (mod) + " " + type;
		}

		public Type[] Types {
			get { return types; }
			set { types = value; }
		}

#if MS_COMPATIBLE
		public AParametersCollection InflateTypes (Type[] genArguments, Type[] argTypes)
		{
			AParametersCollection p = (AParametersCollection) MemberwiseClone (); // Clone ();

			for (int i = 0; i < Count; ++i) {
				if (types[i].IsGenericType) {
					Type[] gen_arguments_open = new Type[types[i].GetGenericTypeDefinition ().GetGenericArguments ().Length];
					Type[] gen_arguments = types[i].GetGenericArguments ();
					for (int ii = 0; ii < gen_arguments_open.Length; ++ii) {
						if (gen_arguments[ii].IsGenericParameter) {
							Type t = argTypes[gen_arguments[ii].GenericParameterPosition];
							gen_arguments_open[ii] = t;
						} else
							gen_arguments_open[ii] = gen_arguments[ii];
					}

					p.types[i] = types[i].GetGenericTypeDefinition ().MakeGenericType (gen_arguments_open);
					continue;
				}

				if (types[i].IsGenericParameter) {
					Type gen_argument = argTypes[types[i].GenericParameterPosition];
					p.types[i] = gen_argument;
					continue;
				}
			}

			return p;
		}
#endif
	}

	//
	// A collection of imported or resolved parameters
	//
	public class ParametersImported : AParametersCollection
	{
		ParametersImported (AParametersCollection param, Type[] types)
		{
			this.parameters = param.FixedParameters;
			this.types = types;
			has_arglist = param.HasArglist;
			has_params = param.HasParams;
		}

		ParametersImported (IParameterData [] parameters, Type [] types, MethodBase method, bool hasParams)
		{
			this.parameters = parameters;
			this.types = types;
			has_arglist = (method.CallingConvention & CallingConventions.VarArgs) != 0;
			if (has_arglist) {
				this.parameters = new IParameterData [parameters.Length + 1];
				parameters.CopyTo (this.parameters, 0);
				this.parameters [parameters.Length] = new ArglistParameter (Location.Null);
				this.types = new Type [types.Length + 1];
				types.CopyTo (this.types, 0);
				this.types [types.Length] = TypeManager.arg_iterator_type;
			}
			has_params = hasParams;
		}

		public ParametersImported (IParameterData [] param, Type[] types)
		{
			this.parameters = param;
			this.types = types;
		}

		public static AParametersCollection Create (MethodBase method)
		{
			return Create (method.GetParameters (), method);
		}

		//
		// Generic method parameters importer, param is shared between all instances
		//
		public static AParametersCollection Create (AParametersCollection param, MethodBase method)
		{
			if (param.IsEmpty)
				return param;

			ParameterInfo [] pi = method.GetParameters ();
			Type [] types = new Type [pi.Length];
			for (int i = 0; i < types.Length; i++) {
				Type t = pi [i].ParameterType;
				if (t.IsByRef)
					t = TypeManager.GetElementType (t);

				types [i] = TypeManager.TypeToCoreType (t);
			}

			return new ParametersImported (param, types);
		}

		//
		// Imports SRE parameters
		//
		public static AParametersCollection Create (ParameterInfo [] pi, MethodBase method)
		{
			if (pi.Length == 0) {
				if (method != null && (method.CallingConvention & CallingConventions.VarArgs) != 0)
					return new ParametersImported (new IParameterData [0], Type.EmptyTypes, method, false);

				return ParametersCompiled.EmptyReadOnlyParameters;
			}

			Type [] types = new Type [pi.Length];
			IParameterData [] par = new IParameterData [pi.Length];
			bool is_params = false;
			PredefinedAttribute extension_attr = PredefinedAttributes.Get.Extension;
			PredefinedAttribute param_attr = PredefinedAttributes.Get.ParamArray;
			for (int i = 0; i < types.Length; i++) {
				types [i] = TypeManager.TypeToCoreType (pi [i].ParameterType);

				ParameterInfo p = pi [i];
				Parameter.Modifier mod = 0;
				if (types [i].IsByRef) {
					if ((p.Attributes & (ParameterAttributes.Out | ParameterAttributes.In)) == ParameterAttributes.Out)
						mod = Parameter.Modifier.OUT;
					else
						mod = Parameter.Modifier.REF;

					//
					// Strip reference wrapping
					//
					types [i] = TypeManager.GetElementType (types [i]);
				} else if (i == 0 && extension_attr.IsDefined && method != null && method.IsStatic &&
			        (method.DeclaringType.Attributes & Class.StaticClassAttribute) == Class.StaticClassAttribute &&
					method.IsDefined (extension_attr.Type, false)) {
					mod = Parameter.Modifier.This;
				} else if (i >= pi.Length - 2 && types [i].IsArray) {
					if (p.IsDefined (param_attr.Type, false)) {
						mod = Parameter.Modifier.PARAMS;
						is_params = true;
					}
				}

				par [i] = new ParameterData (p.Name, mod);
			}

			return method != null ?
				new ParametersImported (par, types, method, is_params) :
				new ParametersImported (par, types);
		}
	}

	/// <summary>
	///   Represents the methods parameters
	/// </summary>
	public class ParametersCompiled : AParametersCollection
	{
		public static readonly ParametersCompiled EmptyReadOnlyParameters = new ParametersCompiled ();
		
		// Used by C# 2.0 delegates
		public static readonly ParametersCompiled Undefined = new ParametersCompiled ();

		private ParametersCompiled ()
		{
			parameters = new Parameter [0];
			types = Type.EmptyTypes;
		}

		private ParametersCompiled (Parameter [] parameters, Type [] types)
		{
			this.parameters = parameters;
		    this.types = types;
		}
		
		public ParametersCompiled (params Parameter[] parameters)
		{
			if (parameters == null)
				throw new ArgumentException ("Use EmptyReadOnlyParameters");

			this.parameters = parameters;
			int count = parameters.Length;

			if (count == 0)
				return;

			if (count == 1) {
				has_params = (parameters [0].ModFlags & Parameter.Modifier.PARAMS) != 0;
				return;
			}

			for (int i = 0; i < count; i++){
				string base_name = parameters [i].Name;
				has_params |= (parameters [i].ModFlags & Parameter.Modifier.PARAMS) != 0;

				for (int j = i + 1; j < count; j++){
					if (base_name != parameters [j].Name)
						continue;

					ErrorDuplicateName (parameters [i]);
					i = j;
				}
			}
		}

		public ParametersCompiled (Parameter [] parameters, bool has_arglist) :
			this (parameters)
		{
			this.has_arglist = has_arglist;
		}
		
		public static ParametersCompiled CreateFullyResolved (Parameter p, Type type)
		{
			return new ParametersCompiled (new Parameter [] { p }, new Type [] { type });
		}
		
		public static ParametersCompiled CreateFullyResolved (Parameter[] parameters, Type[] types)
		{
			return new ParametersCompiled (parameters, types);
		}

		public static ParametersCompiled MergeGenerated (ParametersCompiled userParams, bool checkConflicts, Parameter compilerParams, Type compilerTypes)
		{
			return MergeGenerated (userParams, checkConflicts,
				new Parameter [] { compilerParams },
				new Type [] { compilerTypes });
		}

		//
		// Use this method when you merge compiler generated parameters with user parameters
		//
		public static ParametersCompiled MergeGenerated (ParametersCompiled userParams, bool checkConflicts, Parameter[] compilerParams, Type[] compilerTypes)
		{
			Parameter[] all_params = new Parameter [userParams.Count + compilerParams.Length];
			userParams.FixedParameters.CopyTo(all_params, 0);

			Type [] all_types;
			if (userParams.types != null) {
				all_types = new Type [all_params.Length];
				userParams.Types.CopyTo (all_types, 0);
			} else {
				all_types = null;
			}

			int last_filled = userParams.Count;
			int index = 0;
			foreach (Parameter p in compilerParams) {
				for (int i = 0; i < last_filled; ++i) {
					while (p.Name == all_params [i].Name) {
						if (checkConflicts && i < userParams.Count) {
							Report.Error (316, userParams [i].Location,
								"The parameter name `{0}' conflicts with a compiler generated name", p.Name);
						}
						p.Name = '_' + p.Name;
					}
				}
				all_params [last_filled] = p;
				if (all_types != null)
					all_types [last_filled] = compilerTypes [index++];
				++last_filled;
			}
			
			ParametersCompiled parameters = new ParametersCompiled (all_params, all_types);
			parameters.has_params = userParams.has_params;
			return parameters;
		}

		protected virtual void ErrorDuplicateName (Parameter p)
		{
			Report.Error (100, p.Location, "The parameter name `{0}' is a duplicate", p.Name);
		}

		/// <summary>
		///    Returns the parameter information based on the name
		/// </summary>
		public int GetParameterIndexByName (string name)
		{
			for (int idx = 0; idx < Count; ++idx) {
				if (parameters [idx].Name == name)
					return idx;
			}

			return -1;
		}

		public bool Resolve (IResolveContext ec)
		{
			if (types != null)
				return true;
			
			types = new Type [Count];
			
			bool ok = true;
			Parameter p;
			for (int i = 0; i < FixedParameters.Length; ++i) {
				p = this [i];
				Type t = p.Resolve (ec);
				if (t == null) {
					ok = false;
					continue;
				}

				types [i] = t;
			}

			return ok;
		}

		public void ResolveVariable ()
		{
			for (int i = 0; i < FixedParameters.Length; ++i) {
				this [i].ResolveVariable (i);
			}
		}

		public CallingConventions CallingConvention
		{
			get {
				if (HasArglist)
					return CallingConventions.VarArgs;
				else
					return CallingConventions.Standard;
			}
		}

		// Define each type attribute (in/out/ref) and
		// the argument names.
		public void ApplyAttributes (MethodBase builder)
		{
			if (Count == 0)
				return;

			MethodBuilder mb = builder as MethodBuilder;
			ConstructorBuilder cb = builder as ConstructorBuilder;

			for (int i = 0; i < Count; i++) {
				this [i].ApplyAttributes (mb, cb, i + 1);
			}
		}

		public void VerifyClsCompliance ()
		{
			foreach (Parameter p in FixedParameters)
				p.IsClsCompliant ();
		}

		public Parameter this [int pos] {
			get { return (Parameter) parameters [pos]; }
		}

		public Expression CreateExpressionTree (EmitContext ec, Location loc)
		{
			ArrayList initializers = new ArrayList (Count);
			foreach (Parameter p in FixedParameters) {
				//
				// Each parameter expression is stored to local variable
				// to save some memory when referenced later.
				//
				StatementExpression se = new StatementExpression (p.CreateExpressionTreeVariable (ec));
				if (se.Resolve (ec))
					ec.CurrentBlock.AddScopeStatement (se);
				
				initializers.Add (p.ExpressionTreeVariableReference ());
			}

			return new ArrayCreation (
				Parameter.ResolveParameterExpressionType (ec, loc),
				"[]", initializers, loc);
		}

		public ParametersCompiled Clone ()
		{
			ParametersCompiled p = (ParametersCompiled) MemberwiseClone ();

			p.parameters = new IParameterData [parameters.Length];
			for (int i = 0; i < Count; ++i)
				p.parameters [i] = this [i].Clone ();

			return p;
		}
	}
}
