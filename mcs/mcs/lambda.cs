//
// lambda.cs: support for lambda expressions
//
// Authors: Miguel de Icaza (miguel@gnu.org)
//          Marek Safar (marek.safar@gmail.com)
//
// Dual licensed under the terms of the MIT X11 or GNU GPL
//
// Copyright 2007-2008 Novell, Inc
//

using System;
using System.Collections;
using System.Reflection;
using System.Reflection.Emit;

namespace Mono.CSharp {
	public class LambdaExpression : AnonymousMethodExpression
	{
		readonly bool explicit_parameters;

		//
		// The parameters can either be:
		//    A list of Parameters (explicitly typed parameters)
		//    An ImplicitLambdaParameter
		//
		public LambdaExpression (Parameters parameters, Location loc)
			: base (parameters, loc)
		{
			if (parameters.Count > 0)
				explicit_parameters = !(parameters.FixedParameters [0] is ImplicitLambdaParameter);
		}

		protected override Expression CreateExpressionTree (EmitContext ec, Type delegate_type)
		{
			if (ec.IsInProbingMode)
				return this;
			
			Expression args = Parameters.CreateExpressionTree (ec, loc);
			Expression expr = Block.CreateExpressionTree (ec);
			if (expr == null)
				return null;

			ArrayList arguments = new ArrayList (2);
			arguments.Add (new Argument (expr));
			arguments.Add (new Argument (args));
			return CreateExpressionFactoryCall ("Lambda",
				new TypeArguments (loc, new TypeExpression (delegate_type, loc)),
				arguments);
		}

		public override bool HasExplicitParameters {
			get {
				return explicit_parameters;
			}
		}

		protected override Parameters ResolveParameters (EmitContext ec, TypeInferenceContext tic, Type delegateType)
		{
			if (!TypeManager.IsDelegateType (delegateType))
				return null;

			AParametersCollection d_params = TypeManager.GetDelegateParameters (delegateType);

			if (explicit_parameters) {
				if (!VerifyExplicitParameters (delegateType, d_params, ec.IsInProbingMode))
					return null;

				return Parameters;
			}

			//
			// If L has an implicitly typed parameter list we make implicit parameters explicit
			// Set each parameter of L is given the type of the corresponding parameter in D
			//
			if (!VerifyParameterCompatibility (delegateType, d_params, ec.IsInProbingMode))
				return null;

			Type [] ptypes = new Type [Parameters.Count];
			for (int i = 0; i < d_params.Count; i++) {
				// D has no ref or out parameters
				if ((d_params.FixedParameters [i].ModFlags & Parameter.Modifier.ISBYREF) != 0)
					return null;

				Type d_param = d_params.Types [i];

#if MS_COMPATIBLE
				// Blablabla, because reflection does not work with dynamic types
				if (d_param.IsGenericParameter)
					d_param = delegateType.GetGenericArguments () [d_param.GenericParameterPosition];
#endif
				//
				// When type inference context exists try to apply inferred type arguments
				//
				if (tic != null) {
					d_param = tic.InflateGenericArgument (d_param);
				}

				ptypes [i] = d_param;
				((ImplicitLambdaParameter) Parameters.FixedParameters [i]).Type = d_param;
			}

			Parameters.Types = ptypes;
			return Parameters;
		}

		public override Expression DoResolve (EmitContext ec)
		{
			//
			// Only explicit parameters can be resolved at this point
			//
			if (explicit_parameters) {
				if (!Parameters.Resolve (ec))
					return null;
			}

			eclass = ExprClass.Value;
			type = TypeManager.anonymous_method_type;						
			return this;
		}

		protected override AnonymousMethodBody CompatibleMethodFactory (Type returnType, Type delegateType, Parameters p, ToplevelBlock b)
		{
			return new LambdaMethod (p, b, returnType, delegateType, loc);
		}

		public override string GetSignatureForError ()
		{
			return "lambda expression";
		}
	}

	public class LambdaMethod : AnonymousMethodBody
	{
		public LambdaMethod (Parameters parameters,
					ToplevelBlock block, Type return_type, Type delegate_type,
					Location loc)
			: base (parameters, block, return_type, delegate_type, loc)
		{
		}

		public override string ContainerType {
			get {
				return "lambda expression";
			}
		}

		public override Expression CreateExpressionTree (EmitContext ec)
		{
			Expression args = parameters.CreateExpressionTree (ec, loc);
			Expression expr = Block.CreateExpressionTree (ec);
			if (expr == null)
				return null;

			ArrayList arguments = new ArrayList (2);
			arguments.Add (new Argument (expr));
			arguments.Add (new Argument (args));
			return CreateExpressionFactoryCall ("Lambda",
				new TypeArguments (loc, new TypeExpression (type, loc)),
				arguments);
		}
	}

	//
	// This is a return statement that is prepended lambda expression bodies that happen
	// to be expressions.  Depending on the return type of the delegate this will behave
	// as either { expr (); return (); } or { return expr (); }
	//
	public class ContextualReturn : Return
	{
		public ContextualReturn (Expression expr)
			: base (expr, expr.Location)
		{
		}

		public override Expression CreateExpressionTree (EmitContext ec)
		{
			return Expr.CreateExpressionTree (ec);
		}

		public override void Emit (EmitContext ec)
		{
			if (ec.ReturnType == TypeManager.void_type) {
				((ExpressionStatement) Expr).EmitStatement (ec);
				ec.ig.Emit (OpCodes.Ret);
				return;
			}

			base.Emit (ec);
		}

		protected override bool DoResolve (EmitContext ec)
		{	
			//
			// When delegate returns void, only expression statements can be used
			//
			if (ec.ReturnType == TypeManager.void_type) {
				Expr = Expr.Resolve (ec);
				if (Expr == null)
					return false;
				
				if (Expr is ExpressionStatement)
					return true;
				
				Expr.Error_InvalidExpressionStatement ();
				return false;
			}

			return base.DoResolve (ec);
		}
	}
}
