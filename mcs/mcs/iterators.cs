//
// iterators.cs: Support for implementing iterators
//
// Author:
//   Miguel de Icaza (miguel@ximian.com)
//   Marek Safar (marek.safar@gmail.com)
//
// Dual licensed under the terms of the MIT X11 or GNU GPL
// Copyright 2003 Ximian, Inc.
// Copyright 2003-2008 Novell, Inc.
//

// TODO:
//    Flow analysis for Yield.
//

using System;
using System.Collections;
using System.Reflection;
using System.Reflection.Emit;

namespace Mono.CSharp {

	public class Yield : ResumableStatement {
		Expression expr;
		bool unwind_protect;

		int resume_pc;

		public Yield (Expression expr, Location l)
		{
			this.expr = expr;
			loc = l;
		}

		public static bool CheckContext (EmitContext ec, Location loc, bool isYieldBreak)
		{
			for (Block block = ec.CurrentBlock; block != null; block = block.Parent) {
				if (!block.Unsafe)
					continue;

				Report.Error (1629, loc, "Unsafe code may not appear in iterators");
				return false;
			}

			//
			// We can't use `ec.InUnsafe' here because it's allowed to have an iterator
			// inside an unsafe class.  See test-martin-29.cs for an example.
			//
			if (!ec.CurrentAnonymousMethod.IsIterator) {
				Report.Error (1621, loc,
					      "The yield statement cannot be used inside " +
					      "anonymous method blocks");
				return false;
			}

			return true;
		}

		public override void MutateHoistedGenericType (AnonymousMethodStorey storey)
		{
			expr.MutateHoistedGenericType (storey);
		}
		
		public override bool Resolve (EmitContext ec)
		{
			expr = expr.Resolve (ec);
			if (expr == null)
				return false;

			Report.Debug (64, "RESOLVE YIELD #1", this, ec, expr, expr.GetType (),
				      ec.CurrentAnonymousMethod, ec.CurrentIterator);

			if (!CheckContext (ec, loc, false))
				return false;

			Iterator iterator = ec.CurrentIterator;
			if (expr.Type != iterator.OriginalIteratorType) {
				expr = Convert.ImplicitConversionRequired (
					ec, expr, iterator.OriginalIteratorType, loc);
				if (expr == null)
					return false;
			}

			if (!ec.CurrentBranching.CurrentUsageVector.IsUnreachable)
				unwind_protect = ec.CurrentBranching.AddResumePoint (this, loc, out resume_pc);

			return true;
		}

		protected override void DoEmit (EmitContext ec)
		{
			ec.CurrentIterator.MarkYield (ec, expr, resume_pc, unwind_protect, resume_point);
		}

		protected override void CloneTo (CloneContext clonectx, Statement t)
		{
			Yield target = (Yield) t;

			target.expr = expr.Clone (clonectx);
		}
	}

	public class YieldBreak : ExitStatement {
		public YieldBreak (Location l)
		{
			loc = l;
		}

		public override void Error_FinallyClause ()
		{
			Report.Error (1625, loc, "Cannot yield in the body of a finally clause");
		}

		protected override bool DoResolve (EmitContext ec)
		{
			return Yield.CheckContext (ec, loc, true);
		}

		protected override void DoEmit (EmitContext ec)
		{
			ec.CurrentIterator.EmitYieldBreak (ec.ig, unwind_protect);
		}

		public override void MutateHoistedGenericType (AnonymousMethodStorey storey)
		{
			// nothing to do
		}
	}

	//
	// Wraps method block into iterator wrapper block
	//
	class IteratorStatement : Statement
	{
		Iterator iterator;
		ExplicitBlock original_block;

		public IteratorStatement (Iterator iterator, ExplicitBlock original_block)
		{
			this.iterator = iterator;
			this.original_block = original_block;
			this.loc = iterator.Location;
		}

		public override bool Resolve (EmitContext ec)
		{
			ec.StartFlowBranching (iterator);
			bool ok = original_block.Resolve (ec);
			ec.EndFlowBranching ();
			return ok;
		}

		protected override void DoEmit (EmitContext ec)
		{
			iterator.EmitMoveNext (ec, original_block);
		}

		public override void MutateHoistedGenericType (AnonymousMethodStorey storey)
		{
			original_block.MutateHoistedGenericType (storey);
			iterator.MutateHoistedGenericType (storey);
		}
	}

	public class IteratorStorey : AnonymousMethodStorey
	{
		public readonly Iterator Iterator;

		TypeExpr iterator_type_expr;
		Field pc_field;
		Field current_field;

		TypeExpr enumerator_type;
		TypeExpr enumerable_type;
		TypeArguments generic_args;
		TypeExpr generic_enumerator_type;
#if GMCS_SOURCE		
		TypeExpr generic_enumerable_type;
#endif

		int local_name_idx;

		public IteratorStorey (Iterator iterator)
			: base (iterator.Container.Toplevel, iterator.Host,
			  iterator.OriginalMethod as MemberBase, iterator.GenericMethod, "Iterator")
		{
			this.Iterator = iterator;
			HasHoistedVariables = true;
		}

		public Field PC {
			get { return pc_field; }
		}

		public Field CurrentField {
			get { return current_field; }
		}

		public ArrayList HoistedParameters {
			get { return hoisted_params; }
		}

		public override TypeExpr [] GetClassBases (out TypeExpr base_class)
		{
			iterator_type_expr = new TypeExpression (MutateType (Iterator.OriginalIteratorType), Location);

#if GMCS_SOURCE
			generic_args = new TypeArguments (Location);
			generic_args.Add (iterator_type_expr);
#endif

			ArrayList list = new ArrayList ();
			if (Iterator.IsEnumerable) {
				enumerable_type = new TypeExpression (
					TypeManager.ienumerable_type, Location);
				list.Add (enumerable_type);

#if GMCS_SOURCE
				generic_enumerable_type = new ConstructedType (
					TypeManager.generic_ienumerable_type,
					generic_args, Location);
				list.Add (generic_enumerable_type);
#endif
			}

			enumerator_type = new TypeExpression (
				TypeManager.ienumerator_type, Location);
			list.Add (enumerator_type);

			list.Add (new TypeExpression (TypeManager.idisposable_type, Location));

#if GMCS_SOURCE
			generic_enumerator_type = new ConstructedType (
				TypeManager.generic_ienumerator_type,
				generic_args, Location);
			list.Add (generic_enumerator_type);
#endif

			type_bases = list;

			return base.GetClassBases (out base_class);
		}

		protected override string GetVariableMangledName (LocalInfo local_info)
		{
			return "<" + local_info.Name + ">__" + local_name_idx++.ToString ();
		}

		public void DefineIteratorMembers ()
		{
			pc_field = AddCompilerGeneratedField ("$PC", TypeManager.system_int32_expr);
			current_field = AddCompilerGeneratedField ("$current", iterator_type_expr);

#if GMCS_SOURCE
			Define_Current (true);
#endif
			Define_Current (false);
			new DisposeMethod (this);
			Define_Reset ();

			if (Iterator.IsEnumerable) {
				new GetEnumeratorMethod (this, false);
#if GMCS_SOURCE
				new GetEnumeratorMethod (this, true);
#endif
			}

			DoResolveMembers ();
		}

		void Define_Current (bool is_generic)
		{
			MemberName left;
			TypeExpr type;

			if (is_generic) {
				left = new MemberName (
					"System.Collections.Generic.IEnumerator",
					generic_args, Location);
				type = iterator_type_expr;
			} else {
				left = new MemberName ("System.Collections.IEnumerator", Location);
				type = TypeManager.system_object_expr;
			}

			MemberName name = new MemberName (left, "Current", null, Location);

			ToplevelBlock get_block = new ToplevelBlock (Location);
			get_block.AddStatement (new CurrentBlock (this, is_generic));

			Accessor getter = new Accessor (get_block, 0, null, Location);

			Property current = new Property (
				this, type, Modifiers.DEBUGGER_HIDDEN, false, name, null, getter, null, false);
			AddProperty (current);
		}

		void Define_Reset ()
		{
			Method reset = new Method (
				this, null, TypeManager.system_void_expr,
				Modifiers.PUBLIC | Modifiers.DEBUGGER_HIDDEN,
				false, new MemberName ("Reset", Location),
				Parameters.EmptyReadOnlyParameters, null);
			AddMethod (reset);

			reset.Block = new ToplevelBlock (Location);
			reset.Block.AddStatement (Create_ThrowNotSupported ());
		}

		Statement Create_ThrowNotSupported ()
		{
			TypeExpr ex_type = new TypeLookupExpression ("System.NotSupportedException");

			return new Throw (new New (ex_type, null, Location), Location);
		}

		protected class GetEnumeratorMethod : Method
		{
			public IteratorStorey Host;

			static MemberName GetMemberName (IteratorStorey host, bool is_generic)
			{
				MemberName left;
				if (is_generic) {
					left = new MemberName (
						"System.Collections.Generic.IEnumerable",
						host.generic_args, host.Location);
				} else {
					left = new MemberName (
						"System.Collections.IEnumerable", host.Location);
				}

				return new MemberName (left, "GetEnumerator", host.Location);
			}

			public GetEnumeratorMethod (IteratorStorey host, bool is_generic)
				: base (host, null, is_generic ?
					host.generic_enumerator_type : host.enumerator_type,
					Modifiers.DEBUGGER_HIDDEN, false, GetMemberName (host, is_generic),
					Parameters.EmptyReadOnlyParameters, null)
			{
				this.Host = host;

				host.AddMethod (this);

				Block = new ToplevelBlock (host.Iterator.Container.Toplevel, null, Location);
				Block.AddStatement (new GetEnumeratorStatement (host, type_name));
			}

			public override EmitContext CreateEmitContext (DeclSpace tc, ILGenerator ig)
			{
				EmitContext ec = new EmitContext (
					this, tc, this.ds, Location, ig, MemberType, ModFlags, false);

				ec.CurrentAnonymousMethod = Host.Iterator;
				return ec;
			}

			protected class GetEnumeratorStatement : Statement
			{
				IteratorStorey host;
				Expression type;

				Expression cast;

				public GetEnumeratorStatement (IteratorStorey host, Expression type)
				{
					this.host = host;
					this.type = type;
					loc = host.Location;
				}

				public override bool Resolve (EmitContext ec)
				{
					type = type.ResolveAsTypeTerminal (ec, false);
					if ((type == null) || (type.Type == null))
						return false;

					TypeExpression storey_type_expr = new TypeExpression (host.TypeBuilder, loc);
					Expression new_storey;
					ArrayList init = null;
					if (host.hoisted_this != null) {
						init = new ArrayList (host.hoisted_params == null ? 1 : host.HoistedParameters.Count + 1);
						HoistedThis ht = host.hoisted_this;
						FieldExpr from = new FieldExpr (ht.Field.FieldBuilder, loc);
						from.InstanceExpression = CompilerGeneratedThis.Instance;
						init.Add (new ElementInitializer (ht.Field.Name, from, loc));
					}

					if (host.hoisted_params != null) {
						if (init == null)
							init = new ArrayList (host.HoistedParameters.Count);

						foreach (HoistedParameter hp in host.HoistedParameters) {
							FieldExpr from = new FieldExpr (hp.Field.FieldBuilder, loc);
							from.InstanceExpression = CompilerGeneratedThis.Instance;
							init.Add (new ElementInitializer (hp.Field.Name, from, loc));
						}
					}

					if (init != null) {
						new_storey = new NewInitialize (storey_type_expr, new ArrayList (0),
							new CollectionOrObjectInitializers (init, loc), loc);
					} else {
						new_storey = new New (storey_type_expr, new ArrayList (0), loc);
					}

					new_storey = new_storey.Resolve (ec);
					if (new_storey != null)
						cast = Convert.ImplicitConversionRequired (ec, new_storey, type.Type, loc);

					if (TypeManager.int_interlocked_compare_exchange == null) {
						Type t = TypeManager.CoreLookupType ("System.Threading", "Interlocked", Kind.Class, true);
						if (t != null) {
							TypeManager.int_interlocked_compare_exchange = TypeManager.GetPredefinedMethod (
								t, "CompareExchange", loc, TypeManager.GetReferenceType (TypeManager.int32_type),
								TypeManager.int32_type, TypeManager.int32_type);
						}
					}

					ec.CurrentBranching.CurrentUsageVector.Goto ();
					return true;
				}

				protected override void DoEmit (EmitContext ec)
				{
					ILGenerator ig = ec.ig;
					Label label_init = ig.DefineLabel ();

					ig.Emit (OpCodes.Ldarg_0);
					ig.Emit (OpCodes.Ldflda, host.PC.FieldBuilder);
					ig.Emit (OpCodes.Ldc_I4, (int) Iterator.State.Start);
					ig.Emit (OpCodes.Ldc_I4, (int) Iterator.State.Uninitialized);
					ig.Emit (OpCodes.Call, TypeManager.int_interlocked_compare_exchange);

					ig.Emit (OpCodes.Ldc_I4, (int) Iterator.State.Uninitialized);
					ig.Emit (OpCodes.Bne_Un, label_init);

					ig.Emit (OpCodes.Ldarg_0);
					ig.Emit (OpCodes.Ret);

					ig.MarkLabel (label_init);

					cast.Emit (ec);
					ig.Emit (OpCodes.Ret);
				}

				public override void MutateHoistedGenericType (AnonymousMethodStorey storey)
				{
					throw new NotSupportedException ();
				}
			}
		}

		protected class DisposeMethod : Method
		{
			readonly IteratorStorey Host;

			public DisposeMethod (IteratorStorey host)
				: base (host, null, TypeManager.system_void_expr,
					Modifiers.PUBLIC | Modifiers.DEBUGGER_HIDDEN | Modifiers.COMPILER_GENERATED,
					false, new MemberName ("Dispose", host.Location),
					Parameters.EmptyReadOnlyParameters, null)
			{
				this.Host = host;

				host.AddMethod (this);

				Block = new ToplevelBlock (host.Iterator.Container, null, Location);
				Block.AddStatement (new DisposeMethodStatement (Host.Iterator));

				Report.Debug (64, "DISPOSE METHOD", host, Block);
			}

			public override EmitContext CreateEmitContext (DeclSpace tc, ILGenerator ig)
			{
				EmitContext ec = new EmitContext (
					this, tc, this.ds, Location, ig, MemberType, ModFlags, false);

				ec.CurrentAnonymousMethod = Host.Iterator;
				return ec;
			}

			//public override void Emit ()
			//{
			//    if (Parent.MemberName.IsGeneric)
			//        block.MutateHoistedGenericType (Host.Iterator.Storey);

			//    base.Emit ();
			//}

			protected class DisposeMethodStatement : Statement
			{
				Iterator iterator;

				public DisposeMethodStatement (Iterator iterator)
				{
					this.iterator = iterator;
					this.loc = iterator.Location;
				}

				public override bool Resolve (EmitContext ec)
				{
					return true;
				}

				protected override void DoEmit (EmitContext ec)
				{
					iterator.EmitDispose (ec);
				}

				public override void MutateHoistedGenericType (AnonymousMethodStorey storey)
				{
					throw new NotSupportedException ();
				}
			}
		}

		protected class CurrentBlock : Statement {
			IteratorStorey host;
			bool is_generic;

			public CurrentBlock (IteratorStorey host, bool is_generic)
			{
				this.host = host;
				this.is_generic = is_generic;
				loc = host.Location;
			}

			public override bool Resolve (EmitContext ec)
			{
				// We emit a 'ret', so prevent the enclosing TopLevelBlock from emitting one too
				ec.CurrentBranching.CurrentUsageVector.Goto ();
				return true;
			}

			protected override void DoEmit (EmitContext ec)
			{
				ILGenerator ig = ec.ig;

				ig.Emit (OpCodes.Ldarg_0);
				ig.Emit (OpCodes.Ldfld, host.CurrentField.FieldBuilder);
				if (!is_generic)
					ig.Emit (OpCodes.Box, host.CurrentField.MemberType);
				ig.Emit (OpCodes.Ret);
			}

			public override void MutateHoistedGenericType (AnonymousMethodStorey storey)
			{
				throw new NotSupportedException ();
			}
		}
	}

	//
	// Iterators are implemented as hidden anonymous block
	//
	public class Iterator : AnonymousExpression {
		public readonly IMethodData OriginalMethod;

		public readonly bool IsEnumerable;

		//
		// The state as we generate the iterator
		//
		Label move_next_ok, move_next_error;
		LocalBuilder skip_finally, current_pc;

		public LocalBuilder SkipFinally {
			get { return skip_finally; }
		}

		public LocalBuilder CurrentPC {
			get { return current_pc; }
		}

		public Block Container {
			get { return OriginalMethod.Block; }
		}

		public GenericMethod GenericMethod {
			get { return OriginalMethod.GenericMethod; }
		}

		public readonly Type OriginalIteratorType;

		readonly IteratorStorey IteratorHost;

		public enum State {
			Running = -3, // Used only in CurrentPC, never stored into $PC
			Uninitialized = -2,
			After = -1,
			Start = 0
		}

		public override void AddStoreyReference (AnonymousMethodStorey storey)
		{
			// do nothing
		}

		public void EmitYieldBreak (ILGenerator ig, bool unwind_protect)
		{
			ig.Emit (unwind_protect ? OpCodes.Leave : OpCodes.Br, move_next_error);
		}

		void EmitMoveNext_NoResumePoints (EmitContext ec, Block original_block)
		{
			ILGenerator ig = ec.ig;

			ig.Emit (OpCodes.Ldarg_0);
			ig.Emit (OpCodes.Ldfld, IteratorHost.PC.FieldBuilder);

			ig.Emit (OpCodes.Ldarg_0);
			IntConstant.EmitInt (ig, (int) State.After);
			ig.Emit (OpCodes.Stfld, IteratorHost.PC.FieldBuilder);

			// We only care if the PC is zero (start executing) or non-zero (don't do anything)
			ig.Emit (OpCodes.Brtrue, move_next_error);

			SymbolWriter.StartIteratorBody (ec.ig);
			original_block.Emit (ec);
			SymbolWriter.EndIteratorBody (ec.ig);

			ig.MarkLabel (move_next_error);
			ig.Emit (OpCodes.Ldc_I4_0);
			ig.Emit (OpCodes.Ret);
		}

		internal void EmitMoveNext (EmitContext ec, Block original_block)
		{
			ILGenerator ig = ec.ig;

			move_next_ok = ig.DefineLabel ();
			move_next_error = ig.DefineLabel ();

			if (resume_points == null) {
				EmitMoveNext_NoResumePoints (ec, original_block);
				return;
			}

			current_pc = ec.GetTemporaryLocal (TypeManager.uint32_type);
			ig.Emit (OpCodes.Ldarg_0);
			ig.Emit (OpCodes.Ldfld, IteratorHost.PC.FieldBuilder);
			ig.Emit (OpCodes.Stloc, current_pc);

			// We're actually in state 'running', but this is as good a PC value as any if there's an abnormal exit
			ig.Emit (OpCodes.Ldarg_0);
			IntConstant.EmitInt (ig, (int) State.After);
			ig.Emit (OpCodes.Stfld, IteratorHost.PC.FieldBuilder);

			Label [] labels = new Label [1 + resume_points.Count];
			labels [0] = ig.DefineLabel ();

			bool need_skip_finally = false;
			for (int i = 0; i < resume_points.Count; ++i) {
				ResumableStatement s = (ResumableStatement) resume_points [i];
				need_skip_finally |= s is ExceptionStatement;
				labels [i+1] = s.PrepareForEmit (ec);
			}

			if (need_skip_finally) {
				skip_finally = ec.GetTemporaryLocal (TypeManager.bool_type);
				ig.Emit (OpCodes.Ldc_I4_0);
				ig.Emit (OpCodes.Stloc, skip_finally);
			}

			SymbolWriter.StartIteratorDispatcher (ec.ig);
			ig.Emit (OpCodes.Ldloc, current_pc);
			ig.Emit (OpCodes.Switch, labels);

			ig.Emit (OpCodes.Br, move_next_error);
			SymbolWriter.EndIteratorDispatcher (ec.ig);

			ig.MarkLabel (labels [0]);

			SymbolWriter.StartIteratorBody (ec.ig);
			original_block.Emit (ec);
			SymbolWriter.EndIteratorBody (ec.ig);

			SymbolWriter.StartIteratorDispatcher (ec.ig);

			ig.Emit (OpCodes.Ldarg_0);
			IntConstant.EmitInt (ig, (int) State.After);
			ig.Emit (OpCodes.Stfld, IteratorHost.PC.FieldBuilder);

			ig.MarkLabel (move_next_error);
			ig.Emit (OpCodes.Ldc_I4_0);
			ig.Emit (OpCodes.Ret);

			ig.MarkLabel (move_next_ok);
			ig.Emit (OpCodes.Ldc_I4_1);
			ig.Emit (OpCodes.Ret);

			SymbolWriter.EndIteratorDispatcher (ec.ig);
		}

		public void EmitDispose (EmitContext ec)
		{
			ILGenerator ig = ec.ig;

			Label end = ig.DefineLabel ();

			Label [] labels = null;
			int n_resume_points = resume_points == null ? 0 : resume_points.Count;
			for (int i = 0; i < n_resume_points; ++i) {
				ResumableStatement s = (ResumableStatement) resume_points [i];
				Label ret = s.PrepareForDispose (ec, end);
				if (ret.Equals (end) && labels == null)
					continue;
				if (labels == null) {
					labels = new Label [resume_points.Count + 1];
					for (int j = 0; j <= i; ++j)
						labels [j] = end;
				}
				labels [i+1] = ret;
			}

			if (labels != null) {
				current_pc = ec.GetTemporaryLocal (TypeManager.uint32_type);
				ig.Emit (OpCodes.Ldarg_0);
				ig.Emit (OpCodes.Ldfld, IteratorHost.PC.FieldBuilder);
				ig.Emit (OpCodes.Stloc, current_pc);
			}

			ig.Emit (OpCodes.Ldarg_0);
			IntConstant.EmitInt (ig, (int) State.After);
			ig.Emit (OpCodes.Stfld, IteratorHost.PC.FieldBuilder);

			if (labels != null) {
				//SymbolWriter.StartIteratorDispatcher (ec.ig);
				ig.Emit (OpCodes.Ldloc, current_pc);
				ig.Emit (OpCodes.Switch, labels);
				//SymbolWriter.EndIteratorDispatcher (ec.ig);

				foreach (ResumableStatement s in resume_points)
					s.EmitForDispose (ec, this, end, true);
			}

			ig.MarkLabel (end);
		}


		ArrayList resume_points;
		public int AddResumePoint (ResumableStatement stmt, Location loc)
		{
			if (resume_points == null)
				resume_points = new ArrayList ();
			resume_points.Add (stmt);
			return resume_points.Count;
		}

		//
		// Called back from Yield
		//
		public void MarkYield (EmitContext ec, Expression expr, int resume_pc, bool unwind_protect, Label resume_point)
		{
			ILGenerator ig = ec.ig;

			// Store the new current
			ig.Emit (OpCodes.Ldarg_0);
			expr.Emit (ec);
			ig.Emit (OpCodes.Stfld, IteratorHost.CurrentField.FieldBuilder);

			// store resume program-counter
			ig.Emit (OpCodes.Ldarg_0);
			IntConstant.EmitInt (ig, resume_pc);
			ig.Emit (OpCodes.Stfld, IteratorHost.PC.FieldBuilder);

			// mark finally blocks as disabled
			if (unwind_protect && skip_finally != null) {
				ig.Emit (OpCodes.Ldc_I4_1);
				ig.Emit (OpCodes.Stloc, skip_finally);
			}

			// Return ok
			ig.Emit (unwind_protect ? OpCodes.Leave : OpCodes.Br, move_next_ok);

			ig.MarkLabel (resume_point);
		}

		public override string ContainerType {
			get { return "iterator"; }
		}

		public override bool IsIterator {
			get { return true; }
		}

		public override AnonymousMethodStorey Storey {
			get { return IteratorHost; }
		}

		//
		// Our constructor
		//
		private Iterator (IMethodData method, TypeContainer host, Type iterator_type, bool is_enumerable)
			: base (host,
				new ToplevelBlock (method.Block, Parameters.EmptyReadOnlyParameters, method.Block.StartLocation),
				TypeManager.bool_type,
				method.Location)
		{
			this.OriginalMethod = method;
			this.OriginalIteratorType = iterator_type;
			this.IsEnumerable = is_enumerable;

			IteratorHost = Block.ChangeToIterator (this, method.Block);
		}

		public override string GetSignatureForError ()
		{
			return OriginalMethod.GetSignatureForError ();
		}

		public override Expression DoResolve (EmitContext ec)
		{
			method = new AnonymousMethodMethod (Storey,
				this, Storey, null, TypeManager.system_boolean_expr,
				Modifiers.PUBLIC, OriginalMethod.GetSignatureForError (),
				new MemberName ("MoveNext", Location),
				Parameters.EmptyReadOnlyParameters);

			if (!Compatible (ec))
				return null;

			IteratorHost.DefineIteratorMembers ();

			eclass = ExprClass.Value;
			type = ec.ReturnType;
			return this;
		}

		public override void Emit (EmitContext ec)
		{
			//
			// Load Iterator storey instance
			//
			method.Storey.Instance.Emit (ec);

			//
			// Initialize iterator PC when it's unitialized
			//
			if (IsEnumerable) {
				ILGenerator ig = ec.ig;
				ig.Emit (OpCodes.Dup);
				IntConstant.EmitInt (ig, (int)State.Uninitialized);

				FieldInfo field = IteratorHost.PC.FieldBuilder;
#if GMCS_SOURCE
				if (Storey.MemberName.IsGeneric)
					field = TypeBuilder.GetField (Storey.Instance.Type, field);
#endif
				ig.Emit (OpCodes.Stfld, field);
			}
		}

		public override Expression CreateExpressionTree (EmitContext ec)
		{
			throw new NotSupportedException ("ET");
		}

		public static void CreateIterator (IMethodData method, TypeContainer parent, int modifiers)
		{
			bool is_enumerable;
			Type iterator_type;

			Type ret = method.ReturnType;
			if (ret == null)
				return;

			if (!CheckType (ret, out iterator_type, out is_enumerable)) {
				Report.Error (1624, method.Location,
					      "The body of `{0}' cannot be an iterator block " +
					      "because `{1}' is not an iterator interface type",
					      method.GetSignatureForError (),
					      TypeManager.CSharpName (ret));
				return;
			}

			Parameters parameters = method.ParameterInfo;
			for (int i = 0; i < parameters.Count; i++) {
				Parameter p = parameters [i];
				Parameter.Modifier mod = p.ModFlags;
				if ((mod & Parameter.Modifier.ISBYREF) != 0) {
					Report.Error (1623, p.Location,
						"Iterators cannot have ref or out parameters");
					return;
				}

				if ((mod & Parameter.Modifier.ARGLIST) != 0) {
					Report.Error (1636, method.Location,
						"__arglist is not allowed in parameter list of iterators");
					return;
				}

				if (p.ParameterType.IsPointer) {
					Report.Error (1637, p.Location,
							  "Iterators cannot have unsafe parameters or " +
							  "yield types");
					return;
				}
			}

			if ((modifiers & Modifiers.UNSAFE) != 0) {
				Report.Error (1629, method.Location, "Unsafe code may not appear in iterators");
				return;
			}

			Iterator iter = new Iterator (method, parent, iterator_type, is_enumerable);
			iter.Storey.DefineType ();
		}

		static bool CheckType (Type ret, out Type original_iterator_type, out bool is_enumerable)
		{
			original_iterator_type = null;
			is_enumerable = false;

			if (ret == TypeManager.ienumerable_type) {
				original_iterator_type = TypeManager.object_type;
				is_enumerable = true;
				return true;
			}
			if (ret == TypeManager.ienumerator_type) {
				original_iterator_type = TypeManager.object_type;
				is_enumerable = false;
				return true;
			}

			if (!TypeManager.IsGenericType (ret))
				return false;

			Type[] args = TypeManager.GetTypeArguments (ret);
			if (args.Length != 1)
				return false;

			Type gt = TypeManager.DropGenericTypeArguments (ret);
			if (gt == TypeManager.generic_ienumerable_type) {
				original_iterator_type = args [0];
				is_enumerable = true;
				return true;
			}
			
			if (gt == TypeManager.generic_ienumerator_type) {
				original_iterator_type = args [0];
				is_enumerable = false;
				return true;
			}

			return false;
		}
	}
}

