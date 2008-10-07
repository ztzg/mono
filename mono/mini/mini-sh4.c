/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * mini-sh4.c: SH4 backend for the Mono code generator
 *
 * Authors:
 *   Cedric VINCENT (cedric.vincent@gmail.com)
 *
 * (C) 2008 STMicroelectronics.
 */

#include <glib.h>

#include "mini.h"
#include "cpu-sh4.h"

struct arg_info {
	guint32 offset;
	guint16 size;
	SH4IntRegister reg;

	enum {
		integer32,
		integer64,
		float32,
		float64,
		aggregate,
		none,
	} type;

	enum {
		into_register,
		onto_stack,
		nowhere
	} storage;
};

struct call_info {
	int nargs;
	guint32 reg_usage;
	guint32 stack_usage;
	guint32 stack_align_amount;
	struct arg_info ret;
	struct arg_info sig_cookie;
	struct arg_info *args;
};

static inline void add_int32_arg(SH4IntRegister *arg_reg, guint32 *stack_size, struct arg_info *arg_info)
{
	arg_info->size = 4;
	arg_info->type = integer32;
	arg_info->offset = *stack_size;

	SH4_DEBUG("args => %d, %d, %p", *arg_reg, *stack_size, arg_info);

	if (*arg_reg <= MONO_SH4_REG_LAST_ARG) {
		arg_info->storage = into_register;
		arg_info->reg = *arg_reg;
		(*arg_reg)++;
	}
	else {
		arg_info->storage = onto_stack;
		arg_info->reg = sh4_r15;
		(*stack_size) += 4;
	}

	SH4_DEBUG("arg_reg = %d", *arg_reg);
	SH4_DEBUG("stack_size = %d", *stack_size);
}

/**
 * Obtain information about a call according to the calling convention and
 * determine the amount of space required for code and stack. In addition
 * determine starting points for stack-based arguments, and area for
 * aggregates being returned on the stack.
 */
static struct call_info *get_call_info(MonoGenericSharingContext *context, MonoMethodSignature *signature)
{
	struct call_info *call_info = NULL;
	SH4IntRegister arg_reg = MONO_SH4_REG_FIRST_ARG;
	MonoType *basic_type = NULL;
	guint32 stack_size = 0;
	guint32 i = 0;

	SH4_DEBUG("args => %p, %p", context, signature);

	call_info = g_malloc0(sizeof(struct call_info));
	call_info->args = g_malloc0(sizeof(struct arg_info) * (signature->param_count + signature->hasthis));

	/* Determine where the result will be stored. */
	basic_type = mono_type_get_underlying_type(signature->ret);
	basic_type = mini_get_basic_type_from_generic(context, basic_type);

	SH4_DEBUG("basic_type of result = %d", basic_type->type);

	switch (basic_type->type) {
	case MONO_TYPE_BOOLEAN:
	case MONO_TYPE_CHAR:
	case MONO_TYPE_I1:
	case MONO_TYPE_U1:
	case MONO_TYPE_I2:
	case MONO_TYPE_U2:
	case MONO_TYPE_I4:
	case MONO_TYPE_U4:
	case MONO_TYPE_I:
	case MONO_TYPE_U:
	case MONO_TYPE_STRING:
	case MONO_TYPE_PTR:
	case MONO_TYPE_CLASS:
	case MONO_TYPE_ARRAY:
	case MONO_TYPE_FNPTR:
	case MONO_TYPE_OBJECT:
	case MONO_TYPE_SZARRAY:
		call_info->ret.storage = into_register;
		call_info->ret.type = integer32;
		call_info->ret.reg = sh4_r0;
		break;

	case MONO_TYPE_I8:
	case MONO_TYPE_U8:
		NOT_IMPLEMENTED;
		/* call_info->ret.storage = into_register; */
		/* call_info->ret.type = integer64; */
		/* call_info->ret.reg = sh4_r0; */
		break;

	case MONO_TYPE_R4:
		NOT_IMPLEMENTED;
		/* call_info->ret.storage = into_register; */
		/* call_info->ret.type = float32; */
		/* call_info->ret.reg = sh4_fr0; */
		break;

	case MONO_TYPE_R8:
		NOT_IMPLEMENTED;
		/* call_info->ret.storage = into_register; */
		/* call_info->ret.type = float64; */
		/* call_info->ret.reg = sh4_fr0; */
		break;

	case MONO_TYPE_VOID:
		call_info->ret.storage = nowhere;
		call_info->ret.type = none;
		break;

	case MONO_TYPE_GENERICINST:
		if (mono_type_generic_inst_is_valuetype(signature->ret) == 0) {
			call_info->ret.storage = into_register;
			call_info->ret.type = integer32;
			call_info->ret.reg = sh4_r0;
			break;
		}
		/* else fall through. */
	case MONO_TYPE_VALUETYPE:
	case MONO_TYPE_TYPEDBYREF:
		/* Used to return aggregate, for instance. */
		NOT_IMPLEMENTED;
		break;

	default:
		g_error("Can't handle type '0x%x' as return value", signature->ret->type);
		break;
	}

	/* Store the 'this' pointer before regular arguments. */
	if (signature->hasthis)
		add_int32_arg(&arg_reg, &stack_size, &(call_info->args[0]));

	/* Determine where the arguments will be stored. */
	for (i = 0; i < signature->param_count; i++) {
		struct arg_info *arg_info = &(call_info->args[signature->hasthis + i]);

		/* Emit the signature cookie just before the implicit arguments. */
		if (signature->pinvoke == 0 &&
		    signature->sentinelpos == i &&
		    signature->call_convention == MONO_CALL_VARARG)
			add_int32_arg(&arg_reg, &stack_size, &(call_info->sig_cookie));

		if (signature->params[i]->byref) {
			add_int32_arg(&arg_reg, &stack_size, arg_info);
			continue;
		}

		basic_type = mono_type_get_underlying_type(signature->params[i]);
		basic_type = mini_get_basic_type_from_generic(context, basic_type);

		SH4_DEBUG("basic_type of arg[%d] = %d", i, basic_type->type);

		switch (basic_type->type) {
		case MONO_TYPE_END:
		case MONO_TYPE_VOID:
		case MONO_TYPE_BOOLEAN:
		case MONO_TYPE_CHAR:
		case MONO_TYPE_I1:
		case MONO_TYPE_U1:
		case MONO_TYPE_I2:
		case MONO_TYPE_U2:
		case MONO_TYPE_I4:
		case MONO_TYPE_U4:
		case MONO_TYPE_I:
		case MONO_TYPE_U:
		case MONO_TYPE_STRING:
		case MONO_TYPE_PTR:
		case MONO_TYPE_CLASS:
		case MONO_TYPE_ARRAY:
		case MONO_TYPE_FNPTR:
		case MONO_TYPE_OBJECT:
		case MONO_TYPE_SZARRAY:
			add_int32_arg(&arg_reg, &stack_size, arg_info);
			break;

		case MONO_TYPE_I8:
		case MONO_TYPE_U8:
			NOT_IMPLEMENTED;
			/* add_int64_arg(&arg_reg, &stack_size, arg_info); */
			break;

		case MONO_TYPE_R4:
			NOT_IMPLEMENTED;
			/* add_float32_arg(&arg_reg, &stack_size, arg_info); */
			break;

		case MONO_TYPE_R8:
			NOT_IMPLEMENTED;
			/* add_float64_arg(&arg_reg, &stack_size, arg_info); */
			break;

		case MONO_TYPE_GENERICINST:
		if (mono_type_generic_inst_is_valuetype(signature->params[i]) == 0) {
			add_int32_arg(&arg_reg, &stack_size, arg_info);
			break;
		}
		/* else fall through. */
		case MONO_TYPE_VALUETYPE:
		case MONO_TYPE_TYPEDBYREF:
			NOT_IMPLEMENTED;
			break;

		default:
			g_error("Can't handle type '0x%x' as argument", signature->params[i]->type);
			break;
		}
	}

	/* Emit the signature cookie in case no implicit arguments are specified. */
	if (signature->pinvoke == 0 &&
	    signature->sentinelpos == signature->param_count &&
	    signature->call_convention == MONO_CALL_VARARG)
		add_int32_arg(&arg_reg, &stack_size, &(call_info->sig_cookie));

	/* Align the stack frame on a 4-bytes boundary. */
	call_info->stack_usage = (stack_size + 0x3) & ~0x3;
	call_info->stack_align_amount = call_info->stack_usage - stack_size;
	call_info->reg_usage = arg_reg;

	return call_info;
}

/**
 * For variable length argument lists emit a signature cookie.
 */
static inline void emit_signature_cookie(MonoCompile *compile_unit, MonoCallInst *call, struct arg_info *arg_info)
{
	MonoInst *arg = NULL;
	MonoInst *signature = NULL;

	SH4_DEBUG("args => %p, %p, %p", compile_unit, call, arg_info);

	/* Declare a room where the signature cookie will be stored. */
	MONO_INST_NEW(compile_unit, signature, OP_ICONST);
	signature->inst_p0 = call->signature;

	/* Create a new argument pointing to the signature cookie. */
	MONO_INST_NEW(compile_unit, arg, OP_OUTARG);
	arg->inst_left = signature;
	arg->inst_call = call;

	switch (arg_info->storage) {
	case into_register:
		arg->backend.reg3 = arg_info->reg;
		/* CV : why is this not done into the register allocation phase ? */
		call->used_iregs |= 1 << arg_info->reg;
		break;

	case onto_stack:
		arg->opcode = OP_OUTARG_MEMBASE;
		arg->backend.size = arg_info->size;
		arg->backend.arg_info = arg_info->offset;
		break;

	case nowhere:
	default:
		g_assert_not_reached();
	}

	MONO_INST_LIST_ADD_TAIL(&arg->node, &call->out_args);

	return;
}

/**
 * Take the arguments and generate the Mono instructions in an
 * arch-specific way to properly call the function.
 */
MonoCallInst *mono_arch_call_opcode(MonoCompile *compile_unit, MonoBasicBlock* bb, MonoCallInst *call, int is_virtual)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;
	int sentinelpos = -1;
	int arg_count = 0;
	int i = 0;

	SH4_DEBUG("args => %p, %p, %p, %d", compile_unit, bb, call, is_virtual);

	signature = call->signature;
	arg_count = signature->param_count + signature->hasthis;
	call_info = get_call_info(compile_unit->generic_sharing_context, signature);

	if (call_info->ret.type == aggregate)
		NOT_IMPLEMENTED;

	if (signature->pinvoke == 0 &&
	    signature->call_convention == MONO_CALL_VARARG)
		sentinelpos = signature->sentinelpos + (is_virtual ? 1 : 0);

	for (i = 0; i < arg_count; i++) {
		struct arg_info *arg_info = &(call_info->args[i]);
		MonoInst *arg = NULL;

		/* Emit the signature cookie just before the implicit arguments. */
		if (sentinelpos == i &&
		    signature->pinvoke == 0 &&
		    signature->call_convention == MONO_CALL_VARARG)
			emit_signature_cookie(compile_unit, call, &(call_info->sig_cookie));

		MONO_INST_NEW(compile_unit, arg, OP_OUTARG);
		arg->cil_code  = call->args[i]->cil_code;
		arg->type      = call->args[i]->type;
		arg->inst_left = call->args[i];
		arg->inst_call = call;

		SH4_DEBUG("type of arg[%d] = %d", i, arg_info->type);

		switch (arg_info->type) {
		case integer64:
			if (arg_info->storage == into_register)
				call->used_iregs |= 1 << (arg_info->reg + 1);
			/* Fall through. */
		case integer32:
			switch (arg_info->storage) {
			case into_register:
				arg->backend.reg3 = arg_info->reg;
				/* CV : why is this not done into the register allocation phase ? */
				call->used_iregs |= 1 << arg_info->reg;
				break;

			case onto_stack:
				arg->opcode = OP_OUTARG_MEMBASE;
				arg->backend.size = arg_info->size;
				arg->backend.arg_info = arg_info->offset;
				break;

			case nowhere:
			default:
				g_assert_not_reached();
			}
			break;

		case float32:
			NOT_IMPLEMENTED;
			break;

		case float64:
			NOT_IMPLEMENTED;
			break;

		case aggregate:
			NOT_IMPLEMENTED;
			break;

		default:
			g_assert_not_reached();
			break;
		}

		MONO_INST_LIST_ADD_TAIL(&arg->node, &call->out_args);
	}

	/* Emit the signature cookie in case no implicit arguments are specified. */
	if (signature->pinvoke == 0 &&
	    sentinelpos == arg_count &&
	    signature->call_convention == MONO_CALL_VARARG)
		emit_signature_cookie(compile_unit, call, &(call_info->sig_cookie));

	if (call_info->stack_align_amount != 0) {
		; /* TODO - CV */
	}

	call->stack_usage = call_info->stack_usage;
	compile_unit->param_area = MAX(compile_unit->param_area, call->stack_usage);
	compile_unit->flags |= MONO_CFG_HAS_CALLS;

	g_free(call_info->args);
	g_free(call_info);

	return call;
}

/**
 * Allocate space onto the stack for variables/parameters/...
 * according to the SH4 ABI, and specify how to access them.
 */
void mono_arch_allocate_vars(MonoCompile *compile_unit)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;
	int regsave_offset = 0;
	int locals_offset = 0;
	int i = 0;

	SH4_DEBUG("args => %p", compile_unit);

	signature = mono_method_signature(compile_unit->method);
	call_info = get_call_info(compile_unit->generic_sharing_context, signature);

	/* Allocate space to save the LMF just below the stack pointer. */
	if (compile_unit->method->save_lmf != 0) {
		NOT_IMPLEMENTED;
		compile_unit->stack_offset += sizeof(struct MonoLMF);
	}

	/* Allocate space for parameters, computed into mono_arch_call_opcode(). */
	compile_unit->stack_offset += compile_unit->param_area;

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============| <- SP
	 *	:              : Callee's frame.
	 */

	/* Allocate space to save global registers (sh4_r8 -> sh4_r13). */
	for (i = sh4_r8; i <= sh4_r13; i++)
		if ((compile_unit->used_int_regs & (SH4IntRegister)i) != 0)
			regsave_offset += 4;

	compile_unit->stack_offset += regsave_offset;

	/* Allocate space to save the previous frame pointer (sh4_r14). */
	compile_unit->stack_offset += 4;

	/* Allocate space to save the PR. */
	compile_unit->stack_offset += 4;

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------| <- SP
	 *	:              :
	 */

	compile_unit->frame_reg = sh4_r14;
	compile_unit->used_int_regs |= 1 << sh4_r14;

	/* Compute space used by local variables and specify how to access them. */
	for (i = compile_unit->locals_start; i < compile_unit->num_varinfo; i++) {
		MonoInst *inst = compile_unit->varinfo[i];
		guint32 align = 0;
		guint32 size = 0;

		/* Nothing to do if the variable is already allocated
		   to a register or if it is unused. */
		if (inst->opcode == OP_REGVAR || (inst->flags & MONO_INST_IS_DEAD) != 0)
			continue;

		/* inst->backend.is_pinvoke indicates native sized value types,
		   this is used by the pinvoke wrappers when they call functions
		   returning structures. */
		if (inst->backend.is_pinvoke != 0 && MONO_TYPE_ISSTRUCT(inst->inst_vtype))
			size = mono_class_native_size(inst->inst_vtype->data.klass, &align);
		else
			size = mono_type_size(inst->inst_vtype, (int *)&align);

		/* Align the access on a `align`-bytes boundary. */
		locals_offset += align - 1;
		locals_offset &= ~(align - 1);

		/* Specify how to access this local variable. */
		inst->opcode = OP_REGOFFSET;
		inst->inst_basereg = compile_unit->frame_reg;
		inst->inst_offset = locals_offset;

		SH4_DEBUG("local '%d' size = %d", i, size);
		SH4_DEBUG("local '%d' offset = %d", i, locals_offset);

		locals_offset += size;
	}

	/* Allocate space for local variables. */
	compile_unit->stack_offset += locals_offset;

	/* Align the stack frame on a 4-bytes boundary. */
	compile_unit->stack_offset = (compile_unit->stack_offset + 0x3) & ~0x3;

	/* Record the amount of space needed by local variables, for mono_arch_emit_prolog(). */
	compile_unit->arch.localloc_size = compile_unit->stack_offset - locals_offset;

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------|
	 *	|  local var.  |
	 *	|--------------| <- FP, SP
	 *	:              :
	 */

	if (signature->pinvoke != 0 &&
	    signature->call_convention == MONO_CALL_VARARG) {
		/* Currently, the SH4 backend is able to store the signature
		   cookie into a register, however it appears the generic
		   part of Mono can not handle this because only the field
		   "gint32 sig_cookie" into MonoCompile is available. I don't
		   like the idea to do like other backend, so I'm waiting for
		   another solution. CV */
		NOT_IMPLEMENTED;
	}

	/* The stack size is now fully known so specify how to access parameters. */
	for (i = 0; i < signature->param_count + signature->hasthis; i++) {
		MonoInst *inst = compile_unit->varinfo[i];
		struct arg_info *arg_info = &call_info->args[i];

		/* Nothing to do if the variable is already allocated to a register.
		   TODO - CV : When does this happen ? */
		if (inst->opcode == OP_REGVAR)
			continue;

		switch (arg_info->storage) {
		case into_register:
			inst->opcode = OP_REGVAR;
			inst->dreg = arg_info->reg;

			SH4_DEBUG("arg '%d' reg = %d", i, arg_info->reg);
			break;

		case onto_stack:
			inst->opcode = OP_REGOFFSET;
			inst->inst_basereg = compile_unit->frame_reg;
			inst->inst_offset = arg_info->offset;

			SH4_DEBUG("arg '%d' offset = %d", i, arg_info->offset);

			/* The parameter area is before local variables and
			   global registers are stored (despite the stack
			   grows to low address, offsets are positively
			   computed). */
			inst->inst_offset -= locals_offset + regsave_offset;
			break;

		case nowhere:
		default:
			g_assert_not_reached();
		}
	}

	SH4_DEBUG("return type = %d", call_info->ret.type);

	/* Specify how to access the return value. */
	switch (call_info->ret.type) {
	case aggregate:
		NOT_IMPLEMENTED;
		/* The caller set sh4_r2 to point to already allocated
		   space where the return aggregate will be hold. */
		compile_unit->ret->opcode = OP_REGVAR;
		compile_unit->ret->inst_c0 = sh4_r2;
		break;

	case integer32:
	case integer64:
	case float32:
	case float64:
		compile_unit->ret->opcode = OP_REGVAR;
		compile_unit->ret->inst_c0 = call_info->ret.reg; /* sh4_r0 or sh4_fr0 */
		break;

	case none: /* void */
		break;

	default:
		g_assert_not_reached();
		break;
	}

	g_free(call_info->args);
	g_free(call_info);
}

/**
 * Cleanup architecture specific code.
 */
void mono_arch_cleanup(void)
{
	return;
}

gpointer mono_arch_context_get_int_reg(MonoContext *ctx, int reg)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

/**
 * Initialize the cpu to execute managed code.
 */
void mono_arch_cpu_init(void)
{
	return;
}

/**
 * This function returns the optimizations supported on this cpu.
 */
guint32 mono_arch_cpu_optimizazions(guint32 *exclude_mask)
{
	/* No SH4-specific optimizations yet. */
	*exclude_mask = 0;
	return 0;
}

void mono_arch_create_vars(MonoCompile *compile_unit)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;

	SH4_DEBUG("args => %p", compile_unit);

	signature = mono_method_signature(compile_unit->method);

	call_info = get_call_info(compile_unit->generic_sharing_context, signature);

	switch (call_info->ret.storage) {
	case into_register:
		compile_unit->ret_var_is_local = TRUE;
		break;

	case onto_stack:
		if (MONO_TYPE_ISSTRUCT(signature->ret)) {
			NOT_IMPLEMENTED;
			compile_unit->vret_addr = mono_compile_create_var(compile_unit, &mono_defaults.int_class->byval_arg, OP_ARG);
		}
		break;

	case nowhere:
		/* Nothing to do. */
		break;

	default:
		g_assert_not_reached();
	}

	g_free(call_info->args);
	g_free(call_info);

	return;
}

/**
 * Create the instruction sequence for a function prologue.
 */
guint8 *mono_arch_emit_prolog(MonoCompile *compile_unit)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;
	guint8 *buffer = NULL;
	int localloc_size = 0;
	int i = 0;

	SH4_DEBUG("args => %p", compile_unit);

	if (compile_unit->method->save_lmf != 0) {
		NOT_IMPLEMENTED;
	}

	if (compile_unit->method->wrapper_type == MONO_WRAPPER_NATIVE_TO_MANAGED) {
		NOT_IMPLEMENTED;
	}

	/* Re-align compile_unit->stack_offset if needed,
	   due to the spilling of variables in mini-codegen.c. */
	/* compile_unit->stack_offset = (stack_size + 0x3) & ~0x3; */

	signature = mono_method_signature(compile_unit->method);

	compile_unit->code_size = 40 + signature->param_count * 10;
	buffer = compile_unit->native_code = g_malloc(compile_unit->code_size);

	call_info = get_call_info(compile_unit->generic_sharing_context, signature);

	if (call_info->ret.type == aggregate)
		NOT_IMPLEMENTED;

	compile_unit->arch.argalloc_size = 0;

	/* Save arguments at the right place : into register or onto the stack. */
	for (i = 0; i < signature->param_count + signature->hasthis; i++) {
		struct arg_info *arg_info = &call_info->args[i];
		MonoInst *inst = compile_unit->args[i];

		/* Sanity checks. */
		g_assert(inst->opcode == OP_REGVAR && arg_info->storage == into_register);

		SH4_DEBUG("arg[%d] is on stack ? %d", i, (int)(inst->opcode != OP_REGVAR));

		switch (arg_info->type) {

		case integer32:
			if (inst->opcode == OP_REGVAR) {
				if (inst->dreg != arg_info->reg)
					sh4_mov(&buffer, inst->dreg, arg_info->reg);
			}
			else {
				sh4_movl_decRx(&buffer, inst->dreg, sh4_r15);
				compile_unit->arch.argalloc_size += 4;
			}
			break;

		case integer64:
			NOT_IMPLEMENTED;
			if (inst->opcode == OP_REGVAR) {
				if (inst->dreg != arg_info->reg) {
					/* TODO - CV : check the order. */
					sh4_mov(&buffer, inst->dreg + 1, arg_info->reg + 1);
					sh4_mov(&buffer, inst->dreg, arg_info->reg);
				}
			}
			else {
				/* TODO - CV : check the order. */
				sh4_movl_decRx(&buffer, inst->dreg + 1, sh4_r15);
				sh4_movl_decRx(&buffer, inst->dreg, sh4_r15);
				compile_unit->arch.argalloc_size += 8;
			}
			break;

		case float64:
		case float32:
		case aggregate:
			NOT_IMPLEMENTED;
			break;

		case none:
		default:
			g_assert_not_reached();
			break;
		}
	}

	/* Sanity checks. */
	g_assert(compile_unit->param_area == compile_unit->arch.argalloc_size);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============| <- SP
	 *	:              : Callee's frame.
	 */

	/* Save global registers (sh4_r8 -> sh4_r13). */
	for (i = sh4_r8; i <= sh4_r13; i++)
		if (compile_unit->used_int_regs & (1 << i))
			sh4_movl_decRx(&buffer, (SH4IntRegister)i, sh4_r15);

	/* Save the previous frame pointer (sh4_r14). */
	sh4_movl_decRx(&buffer, sh4_r14, sh4_r15);

	/* Save the PR. */
	sh4_stsl_PR_decRx(&buffer, sh4_r15);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------| <- SP
	 *	:              :
	 */

	/* The space needed by local variables is computed into mono_arch_allocate_vars(). */
	localloc_size = compile_unit->arch.localloc_size;
	if (localloc_size != 0) {
		if (SH4_CHECK_RANGE_add_imm(localloc_size))
			sh4_add_imm(&buffer, -localloc_size, sh4_r15);
		else {
			/* R14 can be used to increment the stack size (that is, used
			   to decrement R15) because it was saved previously and will
			   be overwritten later. */
			NOT_IMPLEMENTED;
#if 0
			/* Patch slot for : sh4_r14 <-localloc _size */
			patch = buffer;
			sh4_ldtlb(&buffer);

			sh4_sub(&buffer, sh4_r14, sh4_r15);
#endif
		}
	}

	SH4_DEBUG("localloc_size = %d", localloc_size);

	/* Set the frame pointer. */
	sh4_mov(&buffer, sh4_r15, sh4_r14);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------|
	 *	|  local var.  |
	 *	|--------------| <- FP, SP
	 *	:              :
	 */

	compile_unit->code_len = buffer - compile_unit->native_code;

	/* Sanity checks. */
	g_assert(compile_unit->code_len < compile_unit->code_size);

	g_free(call_info->args);
	g_free(call_info);

	return buffer;
}

void mono_arch_emit_epilog(MonoCompile *compile_unit)
{
	guint8 *buffer = NULL;
	guint8 *code   = NULL;
	guint8 *patch1 = NULL;
	guint8 *patch2 = NULL;
	int localloc_size = 0;
	int argalloc_size = 0;
	int i = 0;

	SH4_DEBUG("args => %p", compile_unit);

	if (compile_unit->method->save_lmf != 0) {
		NOT_IMPLEMENTED;
	}

#define EPILOGUE_SIZE 44

	/* Reallocate enough room to store the SH4 instructions
	   used to implement an epilogue. */
	while (compile_unit->code_len + EPILOGUE_SIZE > compile_unit->code_size) {
		compile_unit->code_size *= 2;
		compile_unit->native_code = g_realloc(compile_unit->native_code, compile_unit->code_size);

		mono_jit_stats.code_reallocs++;
	}

	code = buffer = compile_unit->native_code + compile_unit->code_len;

	/* Reset the stack pointer. */
	sh4_mov(&buffer, sh4_r14, sh4_r15);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------|
	 *	|  local var.  |
	 *	|--------------| <- FP, SP
	 *	:              :
	 */

	/* Free the space used by local variables. */
	localloc_size = compile_unit->arch.localloc_size;
	if (localloc_size != 0) {
		if (SH4_CHECK_RANGE_add_imm(localloc_size))
			sh4_add_imm(&buffer, localloc_size, sh4_r15);
		else {
			/* R14 can be used to increment the stack size (that is, used
			   to decrement R15) because it was saved previously and will
			   be overwritten later. */

			/* Patch slot for : sh4_r14 <- localloc_size */
			patch1 = buffer;
			sh4_ldtlb(&buffer);

			sh4_add(&buffer, sh4_r14, sh4_r15);
		}
	}

	SH4_DEBUG("localloc_size = %d", localloc_size);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------| <- SP
	 *	:              :
	 */

	/* Restore the PR. */
	sh4_ldsl_incRx_PR(&buffer, sh4_r15);

	/* Restore the previous frame pointer (sh4_r14). */
	sh4_movl_incRy(&buffer, sh4_r15, sh4_r14);

	/* Restore scratch registers (sh4_r8 -> sh4_r13). */
	for (i = sh4_r8; i <= sh4_r13; i++)
		if (compile_unit->used_int_regs & (1 << i))
			sh4_movl_incRy(&buffer, sh4_r15, (SH4IntRegister)i);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============| <- SP
	 *	:              : Callee's frame.
	 */

	/* Free the space used by parameters. */
	argalloc_size = compile_unit->arch.argalloc_size;
	if (argalloc_size != 0) {
		if (SH4_CHECK_RANGE_add_imm(argalloc_size))
			sh4_add_imm(&buffer, argalloc_size, sh4_r15);
		else {
			NOT_IMPLEMENTED;
#if 0
			/* I don't know yet if I can use a local register
			   (a.k.a scratch register). */

			/* Patch slot for : sh4_rXX <- argalloc_size */
			patch2 = buffer;
			sh4_ldtlb(&buffer);

			sh4_add(&buffer, sh4_rXX, sh4_r15);
#endif
		}
	}

	/* At this point, the stack is fully restored (as caller's point of view). */

	sh4_rts(&buffer);
	sh4_nop(&buffer);

	/* Align the constant pool. */
	if (patch1 != NULL || patch2 != NULL)
		while (((guint32)buffer % 4) != 0)
			sh4_nop(&buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	if (patch1 != NULL) {
		sh4_movl_PCrel(&patch1, buffer, sh4_r8);
		sh4_emit32(&buffer, (guint32)localloc_size);
	}
	if (patch2 != NULL) {
		sh4_movl_PCrel(&patch2, buffer, sh4_r8);
		sh4_emit32(&buffer, (guint32)argalloc_size);
	}

	compile_unit->code_len = buffer - compile_unit->native_code;

	/* Sanity checks. */
	g_assert(buffer - code <= EPILOGUE_SIZE);
	g_assert(compile_unit->code_len < compile_unit->code_size);

	/* mono_arch_flush_icache() is called into the caller mini.c:mono_codegen(). */

	return;
}

void mono_arch_emit_exceptions(MonoCompile *compile_unit)
{
	MonoJumpInfo *patch_info = NULL;
	int exceptions_size = 0;
	guint8 *buffer = NULL;
	guint8 *code   = NULL;

	SH4_DEBUG("args => %p", compile_unit);

	/* Compute the space needed by exceptions infos. */
	for (patch_info = compile_unit->patch_info; patch_info != NULL; patch_info = patch_info->next) {
		if (patch_info->type != MONO_PATCH_INFO_EXC)
			continue;

		/* TODO : replace '0' with the size of the actual code. */
		exceptions_size += 0;
	}

	SH4_DEBUG("exceptions_size = %d", exceptions_size);

	/* Reallocate enough room to store the SH4 instructions
	   used to implement an epilogue. */
	while (compile_unit->code_len + exceptions_size > compile_unit->code_size) {
		compile_unit->code_size *= 2;
		compile_unit->native_code = g_realloc(compile_unit->native_code, compile_unit->code_size);

		mono_jit_stats.code_reallocs++;
	}

	code = buffer = compile_unit->native_code + compile_unit->code_len;

	/* Patch code to raise exceptions. */
	for (patch_info = compile_unit->patch_info; patch_info != NULL; patch_info = patch_info->next) {
		if (patch_info->type != MONO_PATCH_INFO_EXC)
			continue;

		/* TODO - CV */
		NOT_IMPLEMENTED;
	}

	compile_unit->code_len = buffer - compile_unit->native_code;

	/* Sanity checks. */
	g_assert(buffer - code <= exceptions_size);
	g_assert(compile_unit->code_len < compile_unit->code_size);

	/* mono_arch_flush_icache() is called into the caller mini.c:mono_codegen(). */

	return;
}

void mono_arch_emit_this_vret_args(MonoCompile *compile_unit, MonoCallInst *inst, int this_reg, int this_type, int vt_reg)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_flush_icache(guint8 *code, gint size)
{
#if defined (__SH4A__)
	guint32 addr = (guint32)code & ~0xF;
	guint32 end  = (guint32)code + size;

	SH4_DEBUG("args => %p, %d", code, size);

	while (addr <= end) {
		__asm__ __volatile__ ("ocbp @%0" : : "r"(addr));
		__asm__ __volatile__ ("icbi @%0" : : "r"(addr));
		addr += 32;
	}
#endif
	return;
}

void mono_arch_flush_register_windows(void)
{
	/* Not used. */
	return;
}

void mono_arch_free_jit_tls_data(MonoJitTlsData *tls)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

const char *mono_arch_fregname(int reg)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

/**
 * Return a list of variables that can be allocated to
 * the integer registers in the current architecture.
 */
GList *mono_arch_get_allocatable_int_vars(MonoCompile *compile_unit)
{
	int i = 0;
	GList *vars = NULL;

	SH4_DEBUG("args => %p", compile_unit);

	for (i = 0; i < compile_unit->num_varinfo; i++) {
		MonoInst *ins = compile_unit->varinfo [i];
		MonoMethodVar *var = MONO_VARINFO(compile_unit, i);

		/* Skip unused variables. */
		if (var->range.first_use.abs_pos >= var->range.last_use.abs_pos ||
		    (ins->flags & (MONO_INST_IS_DEAD | MONO_INST_VOLATILE | MONO_INST_INDIRECT)) ||
		    (ins->opcode != OP_LOCAL && ins->opcode != OP_ARG))
			continue;

		/* Allocate 32 bit variables only. */
		if (mono_is_regsize_var(ins->inst_vtype)) {
			g_assert(MONO_VARINFO (compile_unit, i)->reg == -1);
			g_assert(i == var->idx);

			SH4_DEBUG("var '%p' allocated to a register", var);

			vars = g_list_prepend(vars, var);
		}
	}

	vars = mono_varlist_sort(compile_unit, vars, 0);

	return vars;
}

int mono_arch_get_argument_info(MonoMethodSignature *csig, int arg_count, MonoJitArgumentInfo *arg_info)
{
	/* TODO - CV */
	g_assert(0);
	return 0;
}

MonoInst *mono_arch_get_domain_intrinsic(MonoCompile* compile_unit)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

/**
 * Return a list of callee-saved registers (a.k.a global registers)
 * that can be used to allocate variables in the current method.
 *
 * The registers sh4_r14 and sh4_r15 are respectively used as the
 * frame pointer and as the stack pointer. Maybe sh4_r12 will be
 * used one day as the global pointer.
 */
GList *mono_arch_get_global_int_regs(MonoCompile *compile_unit)
{
	GList *regs = NULL;

	SH4_DEBUG("args => %p", compile_unit);

	regs = g_list_prepend(regs, (gpointer)sh4_r8);
	regs = g_list_prepend(regs, (gpointer)sh4_r9);
	regs = g_list_prepend(regs, (gpointer)sh4_r10);
	regs = g_list_prepend(regs, (gpointer)sh4_r11);
	regs = g_list_prepend(regs, (gpointer)sh4_r12);
	regs = g_list_prepend(regs, (gpointer)sh4_r13);

	return regs;
}

/**
 * Check for opcodes we can handle directly in hardware.     
 */
MonoInst *mono_arch_get_inst_for_method(MonoCompile *compile_unit, MonoMethod *method, MonoMethodSignature *signature, MonoInst **args)
{
	return NULL;
}

MonoInst *mono_arch_get_thread_intrinsic(MonoCompile* compile_unit)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

gpointer *mono_arch_get_vcall_slot_addr(guint8 *code, gpointer *regs)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

/**
 * Initialize architecture specific code.
 */
void mono_arch_init(void)
{
	/* TODO - CV : InitializeCriticalSection (&mini_arch_mutex); */
	return;
}

void *mono_arch_instrument_epilog(MonoCompile *compile_unit, void *func, void *p, gboolean enable_arguments)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

void *mono_arch_instrument_prolog(MonoCompile *compile_unit, void *func, void *p, gboolean enable_arguments)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

gboolean mono_arch_is_inst_imm(gint64 imm)
{
	/* TODO - CV */
	g_assert(0);
	return 0;
}

/**
 * Decompose some generic opcodes to architecture-specific ones.
 *
 * This opcode decomposition is really helpful because it gives to
 * Mono a better estimation of used resources (registers and length).
 *
 * For instance, the opcode "icompare_imm" is not easily translatable
 * from MonoInst to SH4 code because it only works with R0, moreover
 * the global register allocator and the "call" handler have already
 * done some register assignment. This lowering pass allows to do
 * something like this :
 *
 *     if ins->sreg1 is not already assigned to a physical register and
 *        ins->inst_imm is in the range of "cmpeq_imm_R0" then
 *         replace OP_ICOMPARE_IMM with OP_SH4_ICOMPARE_IMM_R0
 *     else
 *         replace OP_ICOMPARE_IMM with OP_ICOMPARE
 *
 * Now, the machine description file should be adapted to specify R0
 * as a "fixed" register for this new architecture-specific opcode :
 *
 *     sh4_icompare_imm: src1:0 len:2
 */
void mono_arch_lowering_pass(MonoCompile *compile_unit, MonoBasicBlock *basic_block)
{
	MonoInst *inst = NULL;

#define register_not_assigned(reg) ((reg) < 0 || (reg) >= MONO_MAX_IREGS)

	/* Setup the virtual-register allocator. */
	if (basic_block->max_vreg > compile_unit->rs->next_vreg)
		compile_unit->rs->next_vreg = basic_block->max_vreg;

	MONO_BB_FOR_EACH_INS(basic_block, inst) {
		switch (inst->opcode) {

		case OP_ICOMPARE_IMM:
			if (register_not_assigned(inst->sreg1) &&
			    SH4_CHECK_RANGE_cmpeq_imm_R0(inst->inst_imm))
				inst->opcode = OP_SH4_ICOMPARE_IMM_R0;
			else
				mono_decompose_op_imm(compile_unit, inst);
			break;

		default:
			break;
		}
	}

	/* Save the state of the virtual-register allocator. */
	basic_block->max_vreg = compile_unit->rs->next_vreg;

	return;
}

void mono_arch_output_basic_block(MonoCompile *compile_unit, MonoBasicBlock *basic_block)
{
	MonoInst *inst = NULL;
	guint8 *buffer = NULL;

	SH4_DEBUG("args => %p, %p", compile_unit, basic_block);
	SH4_DEBUG("method: %s", compile_unit->method->name);

	/* A chunk of code memory was previously allocated (native_code)
	   then partially used (code_len) into mono_arch_emit_prolog
	   and/or into this function. */
	buffer = compile_unit->native_code + compile_unit->code_len;

	mono_debug_open_block(compile_unit, basic_block, compile_unit->code_len);

	MONO_BB_FOR_EACH_INS(basic_block, inst) {
		guint offset   = 0;
		int length_max = 0;
		int length     = 0;
		guint8 *code   = NULL;

		offset = buffer - compile_unit->native_code;

		/* Get the 'len' field of this opcode, specify into cpu-sh4.md. */
		length_max = *((guint8 *)ins_get_spec(inst->opcode) + MONO_INST_LEN);

		SH4_DEBUG("inst => %s", mono_inst_name(inst->opcode));
		SH4_DEBUG("inst_len => %d", length_max);

		/* Reallocate enough room to store the SH4 instructions
		   used to implement the current opcode. */
		while (offset + length_max > compile_unit->code_size) {
			compile_unit->code_size *= 2;
			compile_unit->native_code = g_realloc(compile_unit->native_code, compile_unit->code_size);

			buffer = compile_unit->native_code + offset;

			mono_jit_stats.code_reallocs++;
		}

		mono_debug_record_line_number(compile_unit, inst, offset);

		/* Save the start address to make sanity checks later. */
		code = buffer;

		switch (inst->opcode) {

		case OP_ICOMPARE_IMM:
			g_assert_not_reached();
			break;

		case OP_SH4_ICOMPARE_IMM_R0:
			/* MD: sh4_icompare_imm_R0: src1:0 len:2 */
			g_assert(inst->sreg1 == sh4_r0);
			sh4_cmpeq_imm_R0(&buffer, inst->inst_imm);
			break;

		default:
			/* The following opcodes are not yet supported, however
			   I need them to pass some trivial examples. */
			/* MD: int_beq: */
			/* MD: store_membase_imm: */
			/* MD: iconst: */
			/* MD: loadu4_membase: */
			/* MD: voidcall: */
			/* MD: move: */
			/* MD: load_membase: */
			/* MD: voidcall_reg: */
			/* MD: br: */
			/* MD: start_handler: */
			/* MD: int_cgt_un: */
			/* MD: endfilter: */
			/* MD: store_membase_reg: */
			/* MD: compare_imm: */
			/* MD: beq: */
			/* MD: bne.un: */
			/* MD: label: */
			/* MD: icompare: */
			/* MD: storei4_membase_imm: */
			/* MD: call_reg: */
			/* MD: call: */
			/* MD: storei4_membase_reg: */
			/* MD: loadi4_membase: */

			g_warning("unknown opcode %s\n", mono_inst_name(inst->opcode));
			g_assert_not_reached();
		}

		/* Sanity checks. */
		length = buffer - code;
		if (length > length_max) {
			g_warning("max length of the opcode %s is at least %d, not %d",
				  mono_inst_name(inst->opcode), length, length_max);
			g_assert_not_reached();
		}
	}

	compile_unit->code_len = buffer - compile_unit->native_code;

	return;
}

void mono_arch_patch_code(MonoMethod *method, MonoDomain *domain, guint8 *code, MonoJumpInfo *patch_info, gboolean run_cctors)
{
	SH4_DEBUG("args => %p, %p, %p, %p, %d", method, domain, code, patch_info, run_cctors);

	for (; patch_info != NULL; patch_info = patch_info->next){
		NOT_IMPLEMENTED;
	}

	return;
}

void mono_arch_peephole_pass_1(MonoCompile *compile_unit, MonoBasicBlock *bb)
{
	/* TODO - CV */
	return;
}

void mono_arch_peephole_pass_2(MonoCompile *compile_unit, MonoBasicBlock *bb)
{
	/* TODO - CV */
	return;
}

gboolean mono_arch_print_tree(MonoInst *tree, int arity)
{
	/* TODO - CV */
	g_assert(0);
	return 0;
}

/**
 * Return the cost, in number of memory references, of the action of
 * allocating the variable VMV into a register during global register
 * allocation.
 */
guint32 mono_arch_regalloc_cost(MonoCompile *compile_unit, MonoMethodVar *vmv)
{
	/* TODO - CV */
	return 0;
}

void mono_arch_register_lowlevel_calls(void)
{
	/* TODO - CV */
	return;
}

const char *mono_arch_regname(int reg)
{
	switch (reg) {
	case sh4_r0: return "sh4_r0";
	case sh4_r1: return "sh4_r1";
	case sh4_r2: return "sh4_r2";
	case sh4_r3: return "sh4_r3";
	case sh4_r4: return "sh4_r4";
	case sh4_r5: return "sh4_r5";
	case sh4_r6: return "sh4_r6";
	case sh4_r7: return "sh4_r7";
	case sh4_r8: return "sh4_r8";
	case sh4_r9: return "sh4_r9";
	case sh4_r10: return "sh4_r10";
	case sh4_r11: return "sh4_r11";
	case sh4_r12: return "sh4_r12";
	case sh4_r13: return "sh4_r13";
	case sh4_r14: return "sh4_r14";
	case sh4_r15: return "sh4_r15";
	}
	return "unknown";
}

void mono_arch_setup_jit_tls_data(MonoJitTlsData *tls)
{
	/* TODO - CV */
	return;
}
