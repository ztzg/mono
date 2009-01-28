/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * mini-sh4.c: SH4 backend for the Mono code generator
 *
 * Copyright (c) 2009 STMicroelectronics
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Cedric VINCENT (cedric.vincent@st.com)
 *   Yves JANIN (yves.janin@st.com)
 *   Denis FERRANTI (denis.ferranti@st.com)
 *   Yvan Roux (yvan.roux@st.com)
 *   Julien Villette (julien.villette@st.com)
 */

#include <glib.h>

#include "mini.h"
#include "inssel.h"
#include "cpu-sh4.h"

int sh4_extra_debug = 0;

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

	SH4_EXTRA_DEBUG("args => %d, %d, %p", *arg_reg, *stack_size, arg_info);

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

	SH4_EXTRA_DEBUG("arg_reg = %d", *arg_reg);
	SH4_EXTRA_DEBUG("stack_size = %d", *stack_size);
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

	SH4_EXTRA_DEBUG("args => %p, %p", context, signature);

	call_info = g_malloc0(sizeof(struct call_info));
	call_info->args = g_malloc0(sizeof(struct arg_info) * (signature->param_count + signature->hasthis));

	/* Determine where the result will be stored. */
	basic_type = mono_type_get_underlying_type(signature->ret);
	basic_type = mini_get_basic_type_from_generic(context, basic_type);

	SH4_EXTRA_DEBUG("basic_type of result = %d", basic_type->type);

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

		SH4_EXTRA_DEBUG("basic_type of arg[%d] = %d", i, basic_type->type);

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

	return call_info;
}

/**
 * For variable length argument lists emit a signature cookie.
 */
static inline void emit_signature_cookie(MonoCompile *cfg, MonoCallInst *call, struct arg_info *arg_info)
{
	MonoInst *arg = NULL;
	MonoInst *signature = NULL;

	g_assert(cfg->new_ir == FALSE);

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p, %p, %p", cfg, call, arg_info);

	/* Declare a room where the signature cookie will be stored. */
	MONO_INST_NEW(cfg, signature, OP_ICONST);
	signature->inst_p0 = call->signature;

	/* Create a new argument pointing to the signature cookie. */
	MONO_INST_NEW(cfg, arg, OP_OUTARG);
	arg->inst_left = signature;
	arg->inst_call = call;

	switch (arg_info->storage) {
	case into_register:
		arg->backend.reg3 = arg_info->reg;
		call->used_iregs |= 1 << arg_info->reg;
		break;

	case onto_stack:
		arg->opcode = OP_OUTARG_MEMBASE;
		arg->backend.size = arg_info->size;
		/* Useless because the SH4 uses "mov.l Rx,@-R15" to store parameters.
		arg->backend.arg_info = arg_info->offset; */
		break;

	case nowhere:
	default:
		g_assert_not_reached();
	}

	/* prepend, so they get reversed */
	arg->next = call->out_args;
	call->out_args = arg;

	return;
}

/**
 * Take the arguments and generate the Mono instructions in an
 * arch-specific way to properly call the function.
 */
MonoCallInst *mono_arch_call_opcode(MonoCompile *cfg, MonoBasicBlock* bb, MonoCallInst *call, int is_virtual)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;
	int sentinelpos = -1;
	int arg_count = 0;
	int i = 0;

	g_assert(cfg->new_ir == FALSE);

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p, %p, %p, %d", cfg, bb, call, is_virtual);

	signature = call->signature;
	arg_count = signature->param_count + signature->hasthis;
	call_info = get_call_info(cfg->generic_sharing_context, signature);

	/* The register sh4_r0 is required to save a LMF. */
	if (cfg->method->save_lmf != 0)
		call->used_iregs |= 1 << sh4_r0;

	if (call_info->ret.type == aggregate)
		NOT_IMPLEMENTED;

	if (signature->pinvoke == 0 &&
	    signature->call_convention == MONO_CALL_VARARG)
		sentinelpos = signature->sentinelpos + (is_virtual ? 1 : 0);

	/* Used into mono_arch_output_basic_block():*CALL*
	   to free the space used by parameters after a call. */
	cfg->arch.argalloc_size = 0;

	for (i = 0; i < arg_count; i++) {
		struct arg_info *arg_info = &(call_info->args[i]);
		MonoInst *arg = NULL;

		/* Emit the signature cookie just before the implicit arguments. */
		if (sentinelpos == i &&
		    signature->pinvoke == 0 &&
		    signature->call_convention == MONO_CALL_VARARG)
			emit_signature_cookie(cfg, call, &(call_info->sig_cookie));

		if (is_virtual != 0 && i == 0) {
			/* The argument will be attached to the call instruction. */
			arg = call->args[0]; /* <= Definitively useless! But similar to other backends... */
			call->used_iregs |= 1 << arg_info->reg;
			continue;
		}

		MONO_INST_NEW(cfg, arg, OP_OUTARG);
		arg->cil_code  = call->args[i]->cil_code;
		arg->type      = call->args[i]->type;
		arg->inst_left = call->args[i];
		arg->inst_call = call;

		SH4_CFG_DEBUG(4) SH4_DEBUG("type of arg[%d] = %d", i, arg_info->type);

		switch (arg_info->type) {
		case integer64:
			if (arg_info->storage == into_register)
				call->used_iregs |= 1 << (arg_info->reg + 1);
			/* Fall through. */
		case integer32:
			switch (arg_info->storage) {
			case into_register:
				arg->backend.reg3 = arg_info->reg;
				call->used_iregs |= 1 << arg_info->reg;
				break;

			case onto_stack:
				arg->opcode = OP_OUTARG_MEMBASE;
				arg->backend.size = arg_info->size;
				/* Useless because the SH4 uses "mov.l Rx,@-R15" to store parameters.
				arg->backend.arg_info = arg_info->offset; */

				cfg->arch.argalloc_size += 4;
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

		/* prepend, so they get reversed */
		arg->next = call->out_args;
		call->out_args = arg;
	}

	/* Emit the signature cookie in case no implicit arguments are specified. */
	if (signature->pinvoke == 0 &&
	    sentinelpos == arg_count &&
	    signature->call_convention == MONO_CALL_VARARG)
		emit_signature_cookie(cfg, call, &(call_info->sig_cookie));

	if (call_info->stack_align_amount != 0) {
		; /* TODO - CV */
	}

	/* Reverse the call->out_args list. */
	{
		MonoInst *prev = NULL, *list = call->out_args, *next;
		while (list) {
			next = list->next;
			list->next = prev;
			prev = list;
			list = next;
		}
		call->out_args = prev;
	}

	call->stack_usage = call_info->stack_usage;
	cfg->flags |= MONO_CFG_HAS_CALLS;

	g_free(call_info->args);
	g_free(call_info);

	return call;
}

/**
 * For variable length argument lists emit a signature cookie.
 * Pass a different signature because mono_ArgIterator_Setup assumes
 * the signature cookie is passed first and all the arguments which
 * were before it are passed on the stack after the signature.
 */
static inline void emit_signature_cookie2(MonoCompile *cfg, MonoCallInst *call)
{
	MonoMethodSignature *new_signature = NULL;
	MonoInst *signature = NULL;
	MonoInst *inst = NULL;

	g_assert(cfg->new_ir == TRUE);

	/* AOT not yet supported. */
	cfg->disable_aot = TRUE;

	new_signature = mono_metadata_signature_dup(call->signature);
	new_signature->param_count -= call->signature->sentinelpos;
	new_signature->sentinelpos = 0;

	memcpy(new_signature->params,
	       call->signature->params + call->signature->sentinelpos,
	       new_signature->param_count * sizeof(MonoType*));

	/* Declare a room where the signature cookie will be stored. */
	MONO_INST_NEW(cfg, signature, OP_ICONST);
	signature->inst_c0 = (guint32)new_signature;
	signature->dreg    = mono_alloc_ireg(cfg);
	MONO_ADD_INS(cfg->cbb, signature);

	/* Create a new argument pointing to the signature cookie. */
	MONO_INST_NEW(cfg, inst, OP_SH4_PUSH_ARG);
	inst->sreg1 = signature->dreg;
	cfg->arch.argalloc_size += 4;
	MONO_ADD_INS(cfg->cbb, inst);
}

/**
 * Take the arguments and generate the Mono instructions in an
 * arch-specific way to properly call the function.
 *
 * Same as mono_arch_call_opcode(), but emits IR for pushing arguments
 * to the stack. All the stuff in mono_arch_emit_this_vret_args()
 * should be done in emit_call() too.
 */
void mono_arch_emit_call(MonoCompile *cfg, MonoCallInst *call)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;
	int sentinelpos = -1;
	int arg_count = 0;
	int i = 0;

	SH4_CFG_DEBUG(4)
		SH4_DEBUG("args => %p, %p", cfg, call);

	g_assert(cfg->new_ir == TRUE);

	signature = call->signature;
	arg_count = signature->param_count + signature->hasthis;
	call_info = get_call_info(cfg->generic_sharing_context, signature);

	if (call_info->ret.type == aggregate)
		NOT_IMPLEMENTED;

	if (signature->pinvoke == 0 &&
	    signature->call_convention == MONO_CALL_VARARG)
		sentinelpos = signature->sentinelpos + (signature->hasthis ? 1 : 0);

	/* Used into mono_arch_output_basic_block():*CALL*
	   to free the space used by parameters after a call. */
	cfg->arch.argalloc_size = 0;

	for (i = 0; i < arg_count; i++) {
		struct arg_info *arg_info = &(call_info->args[i]);
		MonoInst *arg = NULL;

		/* Emit the signature cookie just before the implicit arguments. */
		if (sentinelpos == i &&
		    signature->pinvoke == 0 &&
		    signature->call_convention == MONO_CALL_VARARG)
			emit_signature_cookie2(cfg, call);

		MONO_INST_NEW(cfg, arg, OP_NOP);
		arg->cil_code  = call->args[i]->cil_code;
		arg->type      = call->args[i]->type;
		arg->sreg1     = call->args[i]->dreg;

		g_assert(arg->sreg1 != -1);

		switch (arg_info->type) {
		case integer64:
			NOT_IMPLEMENTED;
			/* Fall through. */
		case integer32:
			switch (arg_info->storage) {
			case into_register:
				arg->opcode = OP_MOVE;
				arg->dreg   = mono_alloc_ireg(cfg);
				mono_call_inst_add_outarg_reg(cfg, call, arg->dreg, arg_info->reg, FALSE);
				break;

			case onto_stack:
				arg->opcode = OP_SH4_PUSH_ARG;
				cfg->arch.argalloc_size += 4;
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
		MONO_ADD_INS(cfg->cbb, arg);
	}

	/* Emit the signature cookie in case no implicit arguments are specified. */
	if (signature->pinvoke == 0 &&
	    sentinelpos == arg_count &&
	    signature->call_convention == MONO_CALL_VARARG)
		emit_signature_cookie2(cfg, call);

	call->stack_usage = call_info->stack_usage;

	g_free(call_info->args);
	g_free(call_info);
	return;
}

/* Emits IR to move its argument to the proper return register. */
void mono_arch_emit_setret(MonoCompile *cfg, MonoMethod *method, MonoInst *result)
{
	g_assert(cfg->new_ir == TRUE);

	/* MD: setret: dest:z src1:i len:2 */

	/* Complex return types (as aggregates) not yet supported. */
	MONO_EMIT_NEW_UNALU(cfg, OP_MOVE, cfg->ret->dreg, result->dreg);
	return;
}

/**
 * Allocate space onto the stack for variables/parameters/...
 * according to the SH4 ABI, and specify how to access them.
 */
void mono_arch_allocate_vars(MonoCompile *cfg)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;
	int locals_offset = 0;
	int i = 0;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	/* Spill var slots are allocated from bottom to top. */
	cfg->flags |= MONO_CFG_HAS_SPILLUP;

	signature = mono_method_signature(cfg->method);
	call_info = get_call_info(cfg->generic_sharing_context, signature);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============| <- SP
	 *	:              : Callee's frame.
	 */

	/* Record the amount of space needed to save registers, for mono_arch_emit_prolog(). */
	cfg->arch.regsave_size = 0;

	/* Allocate space to save global registers (sh4_r8 -> sh4_r13). */
	for (i = sh4_r8; i <= sh4_r13; i++)
		if ((cfg->used_int_regs & (1 << i)) != 0)
			cfg->arch.regsave_size += 4;

	/* Allocate space to save the previous frame pointer (sh4_r14). */
	cfg->arch.regsave_size += 4;

	/* Allocate space to save the PR. */
	cfg->arch.regsave_size += 4;

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------| <- SP
	 *	:              :
	 */

	cfg->frame_reg = sh4_r14;
	cfg->used_int_regs |= 1 << sh4_r14;

	/* Allocate space to save the LMF just before "regular" local variables. */
	if (cfg->method->save_lmf != 0)
		;

	/* Compute space used by local variables and specify how to access them. */
	for (i = cfg->locals_start; i < cfg->num_varinfo; i++) {
		MonoInst *inst = cfg->varinfo[i];
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
		inst->inst_basereg = cfg->frame_reg;
		inst->inst_offset = locals_offset;

		locals_offset += size;

		SH4_CFG_DEBUG(4) SH4_DEBUG("local '%d' size = %d", i, size);
		SH4_CFG_DEBUG(4) SH4_DEBUG("local '%d' offset = %d", i, locals_offset);
	}

	/* Record the amount of space needed by local variables, for mono_arch_emit_prolog(). */
	cfg->arch.localloc_size = locals_offset;

	/* Allocate space for local variables. */
	;

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
		MonoInst *inst = cfg->args[i];
		struct arg_info *arg_info = &call_info->args[i];

		/* Nothing to do if the variable is already allocated to a register.
		   TODO - CV : When does this happen ? */
		if (inst->opcode == OP_REGVAR)
			continue;

		switch (arg_info->storage) {
		case into_register:
			inst->opcode = OP_REGVAR;
			inst->dreg = arg_info->reg;

			SH4_CFG_DEBUG(4) SH4_DEBUG("arg '%d' reg = %d", i, arg_info->reg);
			break;

		case onto_stack:
			inst->opcode = OP_REGOFFSET;
			inst->inst_basereg = cfg->frame_reg;
			inst->inst_offset = arg_info->offset;

			/* The parameter area is before local variables and
			   saved registers area (the stack grows to low address,
			   offsets are positively computed). */
			inst->inst_offset += locals_offset + cfg->arch.regsave_size;

			SH4_CFG_DEBUG(4) SH4_DEBUG("arg '%d' offset = %d", i, arg_info->offset);
			break;

		case nowhere:
		default:
			g_assert_not_reached();
		}
	}

	SH4_CFG_DEBUG(4) SH4_DEBUG("return type = %d", call_info->ret.type);

	/* Specify how to access the return value, see mono_arch_emit_setret().
	   Complex return types (as aggregates) not yet supported. */
	if (call_info->ret.type != none) {
		cfg->ret->opcode  = OP_REGVAR;
		cfg->ret->inst_c0 = call_info->ret.reg;
		cfg->ret->dreg    = call_info->ret.reg;
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

void mono_arch_create_vars(MonoCompile *cfg)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	signature = mono_method_signature(cfg->method);

	call_info = get_call_info(cfg->generic_sharing_context, signature);

	switch (call_info->ret.storage) {
	case into_register:
		cfg->ret_var_is_local = TRUE;
		break;

	case onto_stack:
		if (MONO_TYPE_ISSTRUCT(signature->ret)) {
			NOT_IMPLEMENTED;
			cfg->vret_addr = mono_compile_create_var(cfg, &mono_defaults.int_class->byval_arg, OP_ARG);
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
guint8 *mono_arch_emit_prolog(MonoCompile *cfg)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;
	guint8 *buffer = NULL;
	int localloc_size = 0;
	int i = 0;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	if (cfg->method->wrapper_type == MONO_WRAPPER_NATIVE_TO_MANAGED) {
		NOT_IMPLEMENTED;
	}

	signature = mono_method_signature(cfg->method);

	cfg->code_size = 46 + 80 + signature->param_count * 10;
	buffer = cfg->native_code = g_malloc(cfg->code_size);

	call_info = get_call_info(cfg->generic_sharing_context, signature);

	if (call_info->ret.type == aggregate)
		NOT_IMPLEMENTED;

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============| <- SP
	 *	:              : Callee's frame.
	 */

	/* Save global registers (sh4_r8 -> sh4_r13). */
	for (i = sh4_r8; i <= sh4_r13; i++)
		if (cfg->used_int_regs & (1 << i))
			sh4_movl_decRx(NULL, &buffer, (SH4IntRegister)i, sh4_r15);

	/* Save the previous frame pointer (sh4_r14). */
	sh4_movl_decRx(NULL, &buffer, sh4_r14, sh4_r15);

	/* Save the PR. */
	sh4_stsl_PR_decRx(NULL, &buffer, sh4_r15);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------| <- SP
	 *	:              :
	 */

	/* TODO - CV : Should the LMF be saved here ? */

	/* Copy arguments at the expected place : into register or onto the stack. */
	for (i = 0; i < signature->param_count + signature->hasthis; i++) {
		struct arg_info *arg_info = &call_info->args[i];
		MonoInst *inst = cfg->args[i];

		/* The argument ends up into a register... */
		if (inst->opcode == OP_REGVAR) {
			switch (arg_info->storage) {
			/* ... and was already into a register. */
			case into_register:
				switch (arg_info->type) {
				case integer32:
					if (inst->dreg != arg_info->reg)
						sh4_mov(NULL, &buffer, arg_info->reg, inst->dreg);
					break;

				case integer64:
					NOT_IMPLEMENTED;
					if (inst->dreg != arg_info->reg) {
						/* TODO - CV : check the order. */
						sh4_mov(NULL, &buffer, arg_info->reg + 1, inst->dreg + 1);
						sh4_mov(NULL, &buffer, arg_info->reg, inst->dreg);
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
				break;

			/* ... but was onto the stack. */
			case onto_stack:
				switch (arg_info->type) {
				case integer32: {
					int offset = cfg->arch.regsave_size + arg_info->offset;
					if (SH4_CHECK_RANGE_movl_dispRy(offset))
						sh4_movl_dispRy(NULL, &buffer, offset, sh4_r15, inst->dreg);
					else
						NOT_IMPLEMENTED;
					break;
				}
				case integer64:
					NOT_IMPLEMENTED;
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
				break;

			/* ... but was nowhere ! */
			default:
				g_assert_not_reached();
				break;
			}
		}
		/* The argument ends up onto the stack... */
		else {
			switch (arg_info->storage) {
			/* ... but was into a register. */
			case into_register:
				switch (arg_info->type) {
				case integer32:
					g_assert(inst->inst_basereg == sh4_r15);
					g_assert_not_reached(); /* Not yet tested. */
					sh4_movl_dispRx(NULL, &buffer, arg_info->reg, inst->inst_offset, inst->inst_basereg);
					break;

				case integer64:
					NOT_IMPLEMENTED;
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
				break;

			/* ... and was already onto the stack. */
			case onto_stack:
				switch (arg_info->type) {
				case integer32:
					/* Nothing because it is already correctly set in allocate_vars(). */
					break;

				case integer64:
					NOT_IMPLEMENTED;
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
				break;

			/* ... but was nowhere ! */
			default:
				g_assert_not_reached();
				break;
			}
		}
	}

	/* Save the LMF just before "regular" local variables, in fact
	   this structure is "hidden" in the local stack frame. */
	if (cfg->method->save_lmf != 0) {
		guint8 *patch0 = NULL;
		guint8 *patch1 = NULL;
		guint8 *patch2 = NULL;
		guint8 *patch3 = NULL;

		/* About used registers : sh4_r0 is a local register and is
		   declared as clobbered into mono_arch_call_opcode() if
		   "save_lmf" is set. sh4_r14 was saved previously and will
		   be overwritten later. */

		/* pseudo-code: MonoLMF.registers[] = { %R0, ..., %R15 }; */
		sh4_add_imm(NULL, &buffer, -sizeof(MonoLMF) + offsetof(MonoLMF, registers), sh4_r15);
		for (i = 0; i <= 15; i++)
			sh4_movl_dispRx(NULL, &buffer, (SH4IntRegister)i, i * 4, sh4_r15);

		/* Patch slot for : sh4_r0 <- cfg->method */
		patch0 = buffer;
		sh4_die(NULL, &buffer);

		sh4_add_imm(NULL, &buffer, -offsetof(MonoLMF, registers), sh4_r15);

		/* pseudo-code: MonoLMF.method = cfg->method; */
		sh4_movl_dispRx(NULL, &buffer, sh4_r0, offsetof(MonoLMF, method), sh4_r15);

		/* Patch slot for : sh4_r0 <- %PC */
		patch1 = buffer;
		sh4_die(NULL, &buffer);

		/* pseudo-code: MonoLMF.pc = %PC; */
		sh4_movl_dispRx(NULL, &buffer, sh4_r0, offsetof(MonoLMF, pc), sh4_r15);

		/* Patch slot for : sh4_r0 <- mono_get_lmf_addr */
		patch2 = buffer;
		sh4_die(NULL, &buffer);

		/* pseudo-code: MonoLMF.lmf_addr = mono_get_lmf_addr(); */
		sh4_jsr_indRx(NULL, &buffer, sh4_r0);
		sh4_nop(NULL, &buffer);

		sh4_movl_dispRx(NULL, &buffer, sh4_r0, offsetof(MonoLMF, lmf_addr), sh4_r15);

		/*
		 * Insert the new LMF at the beginning of the LMF list.
		 */

		/* pseudo-code: MonoLMF.previous_lmf = *(MonoLMF.lmf_addr); */
		sh4_movl_indRy(NULL, &buffer, sh4_r0, sh4_r14);
		sh4_movl_dispRx(NULL, &buffer, sh4_r14, offsetof(MonoLMF, previous_lmf), sh4_r15);

		/* pseudo-code: *(MonoLMF.lmf_addr) = &MonoLMF;  */
		sh4_movl_indRx(NULL, &buffer, sh4_r15, sh4_r0);

		/* Patch slot for : bra_label "skip_cstpool" */
		patch3 = buffer;
		sh4_die(NULL, &buffer);
		sh4_nop(NULL, &buffer);

		/* Align the constant pool. */
		while (((guint32)buffer % 4) != 0)
			sh4_nop(NULL, &buffer);

		/* Build the constant pool & patch the corresponding instructions. */
		sh4_movl_PCrel(NULL, &patch0, buffer, sh4_r0);
		sh4_emit32(&buffer, (guint32)cfg->method);

		sh4_movl_PCrel(NULL, &patch1, buffer, sh4_r0);
		patch1 -= (guint32)cfg->native_code;
		mono_add_patch_info(cfg, buffer - cfg->native_code, MONO_PATCH_INFO_IP, patch1);
		sh4_emit32(&buffer, (guint32)0);

		sh4_movl_PCrel(NULL, &patch2, buffer, sh4_r0);
		sh4_emit32(&buffer, (guint32)mono_get_lmf_addr);

		sh4_bra_label(NULL, &patch3, buffer);
	}

	/* The space needed by local variables is computed into mono_arch_allocate_vars(),
	   and the size of the spill area (cfg->stack_offset) is computed into mini-codegen.c. */
	localloc_size = cfg->arch.localloc_size + cfg->stack_offset;
	if (localloc_size != 0) {
		if (SH4_CHECK_RANGE_add_imm(-localloc_size)) {
			sh4_add_imm(NULL, &buffer, -localloc_size, sh4_r15);
		}
		else if (SH4_CHECK_RANGE_mov_imm(localloc_size)) {
			/* R14 can be used to increment the stack size (that is, used
			   to decrement R15) because it was saved previously and will
			   be overwritten later. */
			sh4_mov_imm(NULL, &buffer, localloc_size, sh4_r14);
			sh4_sub(NULL, &buffer, sh4_r14, sh4_r15);
		}
		else {
			guint8 *patch0 = NULL;
			guint8 *patch1 = NULL;

			/* Patch slot for : sh4_r0 <- cfg->method */
			patch0 = buffer;
			sh4_die(NULL, &buffer);

			sh4_sub(NULL, &buffer, sh4_r14, sh4_r15);

			/* Patch slot for : bra_label "skip_cstpool" */
			patch1 = buffer;
			sh4_die(NULL, &buffer);
			sh4_nop(NULL, &buffer);

			/* Align the constant pool. */
			while (((guint32)buffer % 4) != 0)
				sh4_nop(NULL, &buffer);

			/* Build the constant pool & patch the corresponding instruction. */
			sh4_movl_PCrel(NULL, &patch0, buffer, sh4_r14);
			sh4_emit32(&buffer, (guint32)localloc_size);

			sh4_bra_label(NULL, &patch1, buffer);
		}
	}

	SH4_CFG_DEBUG(4) SH4_DEBUG("%s::localloc_size = %d", cfg->method->name, localloc_size);

	/* Set the frame pointer. */
	sh4_mov(NULL, &buffer, sh4_r15, sh4_r14);

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

	cfg->code_len = buffer - cfg->native_code;

	/* Sanity checks. */
	g_assert(cfg->code_len < cfg->code_size);

	g_free(call_info->args);
	g_free(call_info);

	return buffer;
}

void mono_arch_emit_epilog(MonoCompile *cfg)
{
	guint8 *buffer = NULL;
	guint8 *code   = NULL;
	guint8 *patch1 = NULL;
	int localloc_size = 0;
	int i = 0;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

#define EPILOGUE_SIZE 48U

	/* Reallocate enough room to store the SH4 instructions
	   used to implement an epilogue. */
        while (cfg->code_len + EPILOGUE_SIZE > cfg->code_size) {
                cfg->code_size *= 2;
		cfg->native_code = g_realloc(cfg->native_code, cfg->code_size);
		mono_jit_stats.code_reallocs++;
	}
	buffer = cfg->native_code + cfg->code_len;
	code = buffer;

	/* Reset the stack pointer. */
	sh4_mov(NULL, &buffer, sh4_r14, sh4_r15);

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

	/* Free the space used by local variables and the spill area. */
	localloc_size = cfg->arch.localloc_size + cfg->stack_offset;
	if (localloc_size != 0) {
		if (SH4_CHECK_RANGE_add_imm(localloc_size))
			sh4_add_imm(NULL, &buffer, localloc_size, sh4_r15);
		else {
			/* R14 can be used here because it was saved previously
			   and will be overwritten/restored later. */

			/* Patch slot for : sh4_r14 <- localloc_size */
			patch1 = buffer;
			sh4_die(NULL, &buffer);

			sh4_add(NULL, &buffer, sh4_r14, sh4_r15);
		}
	}

	SH4_CFG_DEBUG(4) SH4_DEBUG("localloc_size = %d", localloc_size);

	/* Restore the previous LMF & free the space used by the local one. */
	if (cfg->method->save_lmf != 0) {
		/* R14 can be used here because it was saved previously
		   and will be overwritten/restored later. */
		if (localloc_size != 0)
			sh4_mov(NULL, &buffer, sh4_r15, sh4_r14);

		/* At this point, the stack looks like :
		 *	:              :
		 *	|--------------| Caller's frame.
		 *	|  parameters  |
		 *	|==============|
		 *	|  saved reg.  | Callee's frame.
		 *	|--------------|
		 *	|    MonoLMF   |
		 *	|--------------| <- sh4_r15, sh4_r14
		 *	:              :
		 */

		/* pseudo-code: *(MonoLMF.lmf_addr) = MonoLMF.previous_lmf; */
		sh4_movl_dispRy(NULL, &buffer, offsetof(MonoLMF, previous_lmf), sh4_r14, sh4_r4);
		sh4_movl_dispRx(NULL, &buffer, sh4_r14, offsetof(MonoLMF, lmf_addr), sh4_r15);

		/* Adjust sh4_r15 to free of the "hidden" LMF. */
		sh4_add_imm(NULL, &buffer, sizeof(MonoLMF), sh4_r15);
	}

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
	sh4_ldsl_incRx_PR(NULL, &buffer, sh4_r15);

	/* Restore the previous frame pointer (sh4_r14). */
	sh4_movl_incRy(NULL, &buffer, sh4_r15, sh4_r14);

	/* Restore global registers (sh4_r13 -> sh4_r8). */
	for (i = sh4_r13; i >= sh4_r8; i--)
		if (cfg->used_int_regs & (1 << i))
			sh4_movl_incRy(NULL, &buffer, sh4_r15, (SH4IntRegister)i);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============| <- SP
	 *	:              : Callee's frame.
	 */

	/* At this point, the stack is fully restored (as caller's point of view). */

	sh4_rts(NULL, &buffer);
	sh4_nop(NULL, &buffer);

	/* Align & build the constant pool & patch the corresponding instructions. */
	if (patch1 != NULL) {
		while (((guint32)buffer % 4) != 0)
			sh4_nop(NULL, &buffer);

		sh4_movl_PCrel(NULL, &patch1, buffer, sh4_r14);
		sh4_emit32(&buffer, (guint32)localloc_size);
	}

	cfg->code_len = buffer - cfg->native_code;

	/* Sanity checks. */
	g_assert(buffer - code <= EPILOGUE_SIZE);
	g_assert(cfg->code_len < cfg->code_size);

	/* mono_arch_flush_icache() is called into the caller mini.c:mono_codegen(). */

	return;
}

void mono_arch_emit_exceptions(MonoCompile *cfg)
{
	MonoJumpInfo *patch_info = NULL;
	int exceptions_count = 0;
	int exceptions_size = 0;
	guint8 *buffer = NULL;
	guint8 *code   = NULL;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	/* Compute the space needed by exceptions infos. */
	for (patch_info = cfg->patch_info; patch_info != NULL; patch_info = patch_info->next) {
		if (patch_info->type != MONO_PATCH_INFO_EXC)
			continue;

		exceptions_size += 26;
		exceptions_count++;
	}

	SH4_CFG_DEBUG(4) SH4_DEBUG("exceptions_size = %d", exceptions_size);

	/* Reallocate enough room to store the SH4 instructions
	   used to implement exceptions. */
        while (cfg->code_len + exceptions_size > cfg->code_size) {
                cfg->code_size *= 2;
		cfg->native_code = g_realloc(cfg->native_code, cfg->code_size);
		mono_jit_stats.code_reallocs++;
	}
	buffer = cfg->native_code + cfg->code_len;
	code = buffer;

	/* Patch code to raise exceptions. */
	for (patch_info = cfg->patch_info; patch_info != NULL; patch_info = patch_info->next) {
		MonoClass *class = NULL;
		guint8 *patch0 = NULL;
		guint8 *patch1 = NULL;
		guint8 *patch2 = NULL;

		if (patch_info->type != MONO_PATCH_INFO_EXC)
			continue;

		class = mono_class_from_name(mono_defaults.corlib, "System", patch_info->data.name);
		g_assert(class != NULL);

#if 0 /* TODO - CV */
		for (i = 0; i < exceptions_count; i++) {
			/* Reuse a throw sequence for the same exception class. */
			if (classes[i] == class) {
				NOT_IMPLEMENTED;
				goto end_loop;
			}
		}
#endif

		/* Pass parameters to the exception handler:
		      1. type token
		      2. offset between the "throw" site and the current PC. */

		/* Patch slot for : sh4_r4 <- type token */
		patch0 = buffer;
		sh4_die(NULL, &buffer);

		/* Patch slot for : sh4_r5 <- current PC - throw PC */
		patch1 = buffer;
		sh4_die(NULL, &buffer);

		/* Patch slot for : sh4_temp <- mono_arch_throw_exception_by_name */
		patch2 = buffer;
		sh4_die(NULL, &buffer);

		sh4_jsr_indRx(NULL, &buffer, sh4_temp);
		sh4_nop(NULL, &buffer); /* delay slot */

		/* Should never return. */
		sh4_die(NULL, &buffer);

		/* Align the constant pool. */
		while (((guint32)buffer % 4) != 0)
			sh4_nop(NULL, &buffer);

		/* Build the constant pool & patch the corresponding instructions. */
		sh4_movl_PCrel(NULL, &patch0, buffer, MONO_SH4_REG_FIRST_ARG);
		sh4_emit32(&buffer, (guint32)(class->type_token - MONO_TOKEN_TYPE_DEF));

		sh4_movl_PCrel(NULL, &patch1, buffer, MONO_SH4_REG_FIRST_ARG + 1);
		sh4_emit32(&buffer, (guint32)((buffer - cfg->native_code) - patch_info->ip.i - 6));

		sh4_movl_PCrel(NULL, &patch2, buffer, sh4_temp);
		/* Reuse this path_info to set the jump in mono_arch_patch_code(). */
		patch_info->type = MONO_PATCH_INFO_INTERNAL_METHOD;
		patch_info->data.name = "mono_arch_throw_exception_by_name";
		patch_info->ip.i = buffer - cfg->native_code;
		sh4_emit32(&buffer, (guint32)0);
	}

	cfg->code_len = buffer - cfg->native_code;

	/* Sanity checks. */
	g_assert(buffer - code <= exceptions_size);
	g_assert(cfg->code_len <= cfg->code_size);

	/* mono_arch_flush_icache() is called into the caller mini.c:mono_codegen(). */

	return;
}

void mono_arch_emit_this_vret_args(MonoCompile *cfg, MonoCallInst *inst, int this_reg, int this_type, int vt_reg)
{
	int this_dreg = -1;

	g_assert(cfg->new_ir == FALSE);

	/* Keep in sync with get_call_info(). */
	if (vt_reg == -1)
		this_dreg = sh4_r4;
	else
		this_dreg = sh4_r5;

	/* Add the "this" argument. */
	if (this_reg != -1) {
		MonoInst *this;
		MONO_INST_NEW(cfg, this, OP_MOVE);
		this->type  = this_type;
		this->sreg1 = this_reg;
		this->dreg  = mono_regstate_next_int(cfg->rs);
		mono_bblock_add_inst(cfg->cbb, this);
		mono_call_inst_add_outarg_reg(cfg, inst, this->dreg, this_dreg, FALSE);
	}

	/* Add the "vt" argument. */
	if (vt_reg != -1) {
		MonoInst *vtarg;
		MONO_INST_NEW(cfg, vtarg, OP_MOVE);
		vtarg->sreg1 = vt_reg;
		vtarg->dreg  = mono_regstate_next_int(cfg->rs);
		mono_bblock_add_inst(cfg->cbb, vtarg);
		mono_call_inst_add_outarg_reg(cfg, inst, vtarg->dreg, sh4_r4, FALSE);
	}

	return;
}

void mono_arch_flush_icache(guint8 *code, gint size)
{
#if defined (__SH4A__)
	guint32 addr = (guint32)code & ~0xF;
	guint32 end  = (guint32)code + size;

	SH4_EXTRA_DEBUG("args => %p, %d", code, size);

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
GList *mono_arch_get_allocatable_int_vars(MonoCompile *cfg)
{
	int i = 0;
	GList *vars = NULL;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	for (i = 0; i < cfg->num_varinfo; i++) {
		MonoInst *ins = cfg->varinfo [i];
		MonoMethodVar *var = MONO_VARINFO(cfg, i);

		/* Skip unused variables. */
		if (var->range.first_use.abs_pos >= var->range.last_use.abs_pos ||
		    (ins->flags & (MONO_INST_IS_DEAD | MONO_INST_VOLATILE | MONO_INST_INDIRECT)) ||
		    (ins->opcode != OP_LOCAL && ins->opcode != OP_ARG))
			continue;

		/* Allocate 32 bit variables only. */
		if (mono_is_regsize_var(ins->inst_vtype)) {
			g_assert(MONO_VARINFO (cfg, i)->reg == -1);
			g_assert(i == var->idx);

			SH4_CFG_DEBUG(4) SH4_DEBUG("var '%p' allocated to a register", var);

			vars = g_list_prepend(vars, var);
		}
	}

	vars = mono_varlist_sort(cfg, vars, 0);

	return vars;
}

int mono_arch_get_argument_info(MonoMethodSignature *csig, int arg_count, MonoJitArgumentInfo *arg_info)
{
	/* TODO - CV */
	g_assert(0);
	return 0;
}

MonoInst *mono_arch_get_domain_intrinsic(MonoCompile* cfg)
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
GList *mono_arch_get_global_int_regs(MonoCompile *cfg)
{
	GList *regs = NULL;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

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
MonoInst *mono_arch_get_inst_for_method(MonoCompile *cfg, MonoMethod *method, MonoMethodSignature *signature, MonoInst **args)
{
	g_assert(cfg->new_ir == FALSE);
	return NULL;
}

/**
 * Check for opcodes we can handle directly in hardware, but also emits the instructions.
 */
MonoInst *mono_arch_emit_inst_for_method(MonoCompile *cfg, MonoMethod *method,
					 MonoMethodSignature *signature, MonoInst **args)
{
	g_assert(cfg->new_ir == TRUE);
	return NULL;
}

MonoInst *mono_arch_get_thread_intrinsic(MonoCompile* cfg)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

/**
 * Determine the address of the vtable slot used by the [virtual] call
 * which invoked the [magic] trampoline.
 *
 * Technically, a virtual call is a "call VTable[index]" where the
 * "call" opcode is a "call_membase". Remember the lowering pass does
 * the following transformation:
 *
 *     call_membase => load_membase + call_reg
 *
 * We have to analyse the following calling sequence backward because
 * we don't known [yet] if a constant-pool is required:
 *
 *   // Load the VTable:
 *
 *           mov.l @(rZ, 0), rY
 *
 *   // Load the method address, that is, VTable[method_index] (load_membase):
 *
 *       #if index is small
 *           mov.l @(small_index, rY), rX
 *       #else
 *           #if index is medium
 *               mov  medium_index, rW
 *           #else
 *               LOAD big_index, rW
 *           #endif
 *           add   rW, rY
 *           mov.l @rY, rX
 *       #endif
 *
 *   // Jump to the method (call_reg):
 *
 *           jsr  @rX
 *           nop
 *             <- code points here
 */
gpointer *mono_arch_get_vcall_slot_addr(guint8 *code, gpointer *regs)
{
	guint16 *code16 = (void *)code;
	SH4IntRegister sh4_rW = sh4_r0;
	SH4IntRegister sh4_rX = sh4_r0;
	SH4IntRegister sh4_rY = sh4_r0;
	SH4IntRegister sh4_rZ = sh4_r0;
	int offset = 0;
	int index = 0;

	/* Check if it is not a jump to the method (call_reg):
	 *
	 *         jsr  @rX
	 *         nop
	 */
	sh4_rX = get_Rx_sh4_jsr_indRx(code16[-2]);
	if (!is_sh4_nop(code16[-1]) ||
	    !is_sh4_jsr_indRx(code16[-2], sh4_rX))
		return NULL;

	/* Check if it is not a load of the method address, that is,
	 * VTable[method_index] (load_membase) with a medium/big index:
	 *
	 *         #if index is medium
	 *             mov  medium_index, rW
	 *         #else
	 *             LOAD big_index, rW
	 *         #endif
	 *         add   rW, rY
	 *         mov.l @rY, rX
	 */
	sh4_rY = get_Ry_sh4_movl_indRy(code16[-3]);
	if (is_sh4_movl_indRy(code16[-3], sh4_rY, sh4_rX)) {
		sh4_rW = get_Ry_sh4_add(code16[-4]);
		if (!is_sh4_add(code16[-4], sh4_rW, sh4_rY))
			return NULL;

		index = get_imm_sh4_mov_imm(code16[-5]);
		if (is_sh4_mov_imm(code16[-5], index, sh4_rW)) {
			offset = -6;
		}
		else {
			offset = is_sh4_load(&code16[-5], sh4_rW);
			if (offset == 0)
				return NULL;

			offset = -offset - 5;
		}
	}
	else {
	/* Check if it is not a load of the method address, that is,
	 * VTable[method_index] (load_membase) with a small index:
	 *
	 *         mov.l @(small_index, rY), rX
	 */
		sh4_rY = get_Ry_sh4_movl_dispRy(code16[-3]);
		index  = get_imm_sh4_movl_dispRy(code16[-3]);
		if (!is_sh4_movl_dispRy(code16[-3], index, sh4_rY, sh4_rX))
			return NULL;

		offset = -4;
	}

	/* Check if it is not a load of the VTable. */
	sh4_rZ = get_Ry_sh4_movl_dispRy(code16[offset]);
	if (!is_sh4_movl_dispRy(code16[offset], 0, sh4_rZ, sh4_rY))
		return NULL;

	/* So far, so good! */
	return (gpointer*)(regs[sh4_rX]);
}

/**
 * Initialize architecture specific code.
 */
void mono_arch_init(void)
{
	/* TODO - CV : InitializeCriticalSection (&mini_arch_mutex); */
	return;
}

void *mono_arch_instrument_epilog(MonoCompile *cfg, void *func, void *p, gboolean enable_arguments)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

void *mono_arch_instrument_prolog(MonoCompile *cfg, void *func, void *p, gboolean enable_arguments)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

gboolean mono_arch_is_inst_imm(gint64 imm)
{
	/* The lowering pass will take care of it. */
	return TRUE;
}

/**
 * Convert Mono comparison & conditional branch opcodes to SH4 ones.
 *
 * As you may noticed, "compare" is paired with opcodes like "beq",
 * "cgt.un", ... Currently, the signature of "compare" is "src1:X
 * src2:X" and the the signature of "beq" and "cgt.un" is respectively
 * "" and "dest:X", that means we have to use a "status" register to
 * known the result of the previous comparison. It works like a charm
 * on X86 and/or SPARC but it is not suitable for SH4 because its
 * "status" register only keep the equalness, not other possible tests
 * ("<", ">=", ...).
 *
 * A first solution is to test the next opcode to decide which kind of
 * comparison & conditional branch we have to do. Another solution is
 * to adjust the BURG to emit a custom code sequence. Personally, I
 * prefer the first solution because I do not feel happy with
 * auto-generated code (as with BURG) when a simple solution exists.
 */
static inline void convert_comparison_to_sh4(MonoInst *inst, MonoInst *next_inst)
{
	next_inst->backend.data = NULL;

	switch (next_inst->opcode) {
	case OP_ICEQ:
		inst->opcode = OP_SH4_CMPEQ;
		next_inst->opcode = OP_SH4_MOVT;
		break;

	case OP_ICGT:
		inst->opcode = OP_SH4_CMPGT;
		next_inst->opcode = OP_SH4_MOVT;
		break;

	case OP_ICGT_UN:
		inst->opcode = OP_SH4_CMPHI;
		next_inst->opcode = OP_SH4_MOVT;
		break;

	case CEE_BEQ:
	case OP_IBEQ:
		inst->opcode = OP_SH4_CMPEQ;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_COND_EXC_NE_UN:
		/* Trick used in output_basic_block(). */
		next_inst->backend.data = (gpointer)-1;
	case CEE_BNE_UN:
	case OP_IBNE_UN:
		inst->opcode = OP_SH4_CMPEQ;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_IBGT:
		inst->opcode = OP_SH4_CMPGT;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_IBGT_UN:
		inst->opcode = OP_SH4_CMPHI;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_COND_EXC_LE_UN:
		/* Trick used in output_basic_block(). */
		next_inst->backend.data = (gpointer)-1;
	case OP_IBLE_UN:
		inst->opcode = OP_SH4_CMPHI;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_IBLE:
		inst->opcode = OP_SH4_CMPGT;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_IBGE:
		inst->opcode = OP_SH4_CMPGE;
		next_inst->opcode = OP_SH4_BT;
		break;

	case CEE_BGE_UN:
	case OP_IBGE_UN:
		inst->opcode = OP_SH4_CMPHS;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_IBLT:
		inst->opcode = OP_SH4_CMPGE;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_IBLT_UN:
		inst->opcode = OP_SH4_CMPHS;
		next_inst->opcode = OP_SH4_BF;
		break;

	default:
		g_warning("unsupported next_inst->opcode %s (0x%x) in %s()\n",
			  mono_inst_name(next_inst->opcode), next_inst->opcode, __FUNCTION__);
		NOT_IMPLEMENTED;
	}
}

/**
 * Helper routine: returns true if sh4 has got a shift
 * instruction that shifts values by the considered
 * immediate.
 */
static inline gboolean sh4_has_shift_inst_imm(guint32 immediate)
{
	gboolean ret = FALSE;

	switch(immediate) {
		case 1U:	/* Fall through */
		case 2U:	/* Fall through */
		case 8U:	/* Fall through */
		case 16U:
			ret = TRUE;
		break;

		default:
			ret = FALSE;
			break;
	}

	return ret;
}

static inline void decompose_op_offset2base(MonoCompile *cfg, MonoBasicBlock *basic_block, MonoInst *inst, int is_store)
{
	MonoInst *new_inst  = NULL;
	MonoInst *new_inst2 = NULL;

	/* Load the offset in new register. */
	MONO_INST_NEW(cfg, new_inst, OP_ICONST);
	new_inst->inst_c0 = inst->inst_offset;
	new_inst->dreg = mono_regstate_next_int(cfg->rs);

	/* Add the base register to this new register (dest == sreg1 for the SH4). */
	MONO_INST_NEW(cfg, new_inst2, OP_IADD);
	new_inst2->sreg2 = new_inst->dreg;
	if (is_store != 0) {
		new_inst2->dreg  = inst->inst_destbasereg;
		new_inst2->sreg1 = inst->inst_destbasereg;
	}
	else {
		new_inst2->dreg  = inst->inst_basereg;
		new_inst2->sreg1 = inst->inst_basereg;
	}

	mono_bblock_insert_before_ins(basic_block, inst, new_inst2);
	mono_bblock_insert_before_ins(basic_block, new_inst2, new_inst);

	inst->inst_offset = 0;
	if (is_store != 0)
		inst->inst_destbasereg = new_inst2->dreg;
	else
		inst->inst_basereg = new_inst2->dreg;

	return;
}

static inline void decompose_op_imm2reg(MonoCompile *cfg, MonoBasicBlock *basic_block, MonoInst *inst)
{
	MonoInst *new_inst = NULL;

	MONO_INST_NEW(cfg, new_inst, OP_ICONST);
	new_inst->inst_c0 = inst->inst_imm;
	new_inst->dreg = mono_regstate_next_int(cfg->rs);
	mono_bblock_insert_before_ins(basic_block, inst, new_inst);
	inst->sreg1 = new_inst->dreg;
}

/**
 * Decompose some generic opcodes to architecture-specific ones.
 *
 * This opcode decomposition is really helpful because it gives to
 * Mono a better estimation of used resources (registers and length).
 *
 * For instance, the opcode "ceq_imm" is not easily translatable
 * from MonoInst to SH4 code because it only works with R0, moreover
 * the global register allocator and the "call" handler have already
 * done some register assignment. This lowering pass allows to do
 * something like this :
 *
 *     if ins->sreg1 is not already assigned to a physical register and
 *        ins->inst_imm is in the range of "cmpeq_imm_R0" then
 *         replace OP_CMPEQ_IMM with OP_SH4_CMPEQ_IMM_R0
 *     else
 *         replace OP_CMPEQ_IMM with OP_CMPEQ
 *
 * Now, the machine description file should be adapted to specify R0
 * as a "fixed" register for this new architecture-specific opcode :
 *
 *     sh4_cmpeq_imm_R0: src1:z len:2
 */
void mono_arch_lowering_pass(MonoCompile *cfg, MonoBasicBlock *basic_block)
{
	MonoInst *inst = NULL;
	MonoInst *next_inst = NULL;

#define register_not_assigned(reg) ((reg) < 0 || (reg) >= MONO_MAX_IREGS)

	/* Setup the virtual-register allocator. */
	if (basic_block->max_vreg > cfg->rs->next_vreg)
		cfg->rs->next_vreg = basic_block->max_vreg;

	MONO_BB_FOR_EACH_INS_SAFE(basic_block, next_inst, inst) {
		switch (inst->opcode) {
		case OP_COMPARE:
		case OP_ICOMPARE:
		case OP_COMPARE_IMM:
		case OP_ICOMPARE_IMM:
			next_inst = inst->next;
			g_assert(next_inst != NULL);

			convert_comparison_to_sh4(inst, next_inst);

			if (inst->opcode == OP_COMPARE ||
			    inst->opcode == OP_ICOMPARE)
				break;

			/* Optimize if possible. */
			if ((inst->sreg1 == sh4_r0 || register_not_assigned(inst->sreg1)) &&
			    SH4_CHECK_RANGE_cmpeq_imm_R0(inst->inst_imm) &&
			    inst->opcode == OP_SH4_CMPEQ) {
				inst->opcode = OP_SH4_CMPEQ_IMM_R0;
			}
			else {
				MonoInst *new_inst = NULL;

				MONO_INST_NEW(cfg, new_inst, OP_ICONST);
				new_inst->inst_c0 = inst->inst_imm;
				new_inst->dreg = mono_regstate_next_int(cfg->rs);
				mono_bblock_insert_before_ins(basic_block, inst, new_inst);
				inst->sreg2 = new_inst->dreg;
			}
			break;

		case OP_ADD_IMM:
		case OP_IADD_IMM:
			if(!SH4_CHECK_RANGE_add_imm(inst->inst_imm)) {
				mono_decompose_op_imm(cfg, basic_block, inst);
			}
			break;

		case OP_SUB_IMM:
		case OP_ISUB_IMM:
			if(SH4_CHECK_RANGE_add_imm(-inst->inst_imm)) {
				inst->opcode = OP_IADD_IMM;
				inst->inst_imm = -inst->inst_imm;
			}
			else {
				/* 32 bits target: SUB_IMM(pointer) == ISUB_IMM(int)
				 * This case is not handled in mono_op_imm_to_op()
				 * called by mono_decompose_op_imm(). */
				inst->opcode = OP_ISUB_IMM;
				mono_decompose_op_imm(cfg, basic_block, inst);
			}
			break;

		case OP_OR_IMM:
		case OP_IOR_IMM:
			if((inst->sreg1 == sh4_r0 || register_not_assigned(inst->sreg1)) &&
			    SH4_CHECK_RANGE_or_imm_R0(inst->inst_imm)) {
				inst->opcode = OP_SH4_OR_IMM_R0;
			}
			else {
				/* See previous comment. */
				inst->opcode = OP_IOR_IMM;
				mono_decompose_op_imm(cfg, basic_block, inst);
			}
			break;

		case OP_XOR_IMM:
		case OP_IXOR_IMM:
			if((inst->sreg1 == sh4_r0 || register_not_assigned(inst->sreg1)) &&
			   SH4_CHECK_RANGE_xor_imm_R0(inst->inst_imm)) {
				inst->opcode = OP_SH4_XOR_IMM_R0;
			}
			else {
				/* See previous comment. */
				inst->opcode = OP_IXOR_IMM;
				mono_decompose_op_imm(cfg, basic_block, inst);
			}
			break;

		case OP_AND_IMM:
		case OP_IAND_IMM:
			if ((inst->sreg1 == sh4_r0 || register_not_assigned(inst->sreg1)) &&
			    SH4_CHECK_RANGE_and_imm_R0(inst->inst_imm)) {
				inst->opcode = OP_SH4_AND_IMM_R0;
			}
			else {
				mono_decompose_op_imm(cfg, basic_block, inst);
			}
			break;

		case OP_SHL_IMM:
		case OP_ISHL_IMM:
			if(inst->inst_imm==0) {
				inst->opcode = OP_MOVE;
			}
			else if(!sh4_has_shift_inst_imm(inst->inst_imm)) {
				/* See previous comment. */
				inst->opcode = OP_ISHL_IMM;
				mono_decompose_op_imm(cfg, basic_block, inst);
			}
			break;

		case OP_MUL_IMM:
		case OP_IMUL_IMM: {
			MonoInst *new_inst;
			guint32 imm8;

			/* An obvious case */
			if(inst->inst_imm == 0) {
				inst->opcode = OP_ICONST;
				inst->inst_c0= 0;
				break;  /* Early exit */
			}

			/* The case where immediate is 1 may already have
			 * been reduced by CIL compilers. */
			if(inst->inst_imm == 1) {
				inst->opcode = OP_MOVE;
				break;  /* Early exit */
			}

			/* If immediate is a power of 2, we perform some elementary
			 * operator strengh reductions in a limited number of cases:
			 *
			 *  - imm8==1 --> the goal is to map instruction shll.
			 *  - imm8==2 --> the goal is to map instruction shll2
			 *  - imm8==8 --> the goal is to map instruction shll8.
			 *  - imm8==16--> the goal is to map instruction shll16.
			 */
			imm8 = mono_is_power_of_two(inst->inst_imm);
			if(sh4_has_shift_inst_imm(imm8)) {
				inst->opcode = OP_ISHL_IMM;
				inst->inst_imm = imm8;
				break;   /* Early exit */
			}

			/* Now, the general case */
			MONO_INST_NEW(cfg,new_inst,OP_ICONST);
			new_inst->inst_c0 = inst->inst_imm;
			new_inst->dreg = mono_regstate_next_int(cfg->rs);
			mono_bblock_insert_before_ins(basic_block, inst, new_inst);
			inst->opcode = OP_IMUL;
			inst->sreg2 = new_inst->dreg;
			break;
		}

		/* The SH4 can't store immediate into memory, so split
		   store*_membase_imm into: iconst + store*_membase_reg. */
		case OP_STOREI1_MEMBASE_IMM:
			decompose_op_imm2reg(cfg, basic_block, inst);
			/* Fall through. */
		case OP_STOREI1_MEMBASE_REG:
			if ((inst->sreg1 == sh4_r0 || register_not_assigned(inst->sreg1)) &&
			    SH4_CHECK_RANGE_movb_R0_dispRx(inst->inst_offset)) {
				inst->opcode = OP_SH4_STOREI1_MEMBASE_R0;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, inst, 1);
				inst->opcode = OP_SH4_STOREI1;
			}
			break;

		/* See previous comment. */
		case OP_STOREI2_MEMBASE_IMM:
			decompose_op_imm2reg(cfg, basic_block, inst);
			/* Fall through. */
		case OP_STOREI2_MEMBASE_REG:
			if ((inst->sreg1 == sh4_r0 || register_not_assigned(inst->sreg1)) &&
			    SH4_CHECK_RANGE_movw_R0_dispRx(inst->inst_offset)) {
				inst->opcode = OP_SH4_STOREI2_MEMBASE_R0;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, inst, 1);
				inst->opcode = OP_SH4_STOREI2;
			}
			break;

		/* See previous comment. */
		case OP_STORE_MEMBASE_IMM:
		case OP_STOREI4_MEMBASE_IMM:
			decompose_op_imm2reg(cfg, basic_block, inst);
			/* Fall through. */
		case OP_STORE_MEMBASE_REG:
		case OP_STOREI4_MEMBASE_REG:
			if (SH4_CHECK_RANGE_movl_dispRx(inst->inst_offset)) {
				inst->opcode = OP_SH4_STOREI4_MEMBASE;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, inst, 1);
				inst->opcode = OP_SH4_STOREI4;
			}
			break;

		case OP_LOADI1_MEMBASE:
			/* See previous comment. */
			if ((inst->dreg == sh4_r0 || register_not_assigned(inst->dreg)) &&
			    SH4_CHECK_RANGE_movb_dispRy_R0(inst->inst_offset)) {
				inst->opcode = OP_SH4_LOADI1_MEMBASE_R0;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, inst, 0);
				inst->opcode = OP_SH4_LOADI1;
			}
			break;

		case OP_LOADI2_MEMBASE:
			/* See previous comment. */
			if ((inst->dreg == sh4_r0 || register_not_assigned(inst->dreg)) &&
			    SH4_CHECK_RANGE_movw_dispRy_R0(inst->inst_offset)) {
				inst->opcode = OP_SH4_LOADI2_MEMBASE_R0;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, inst, 0);
				inst->opcode = OP_SH4_LOADI2;
			}
			break;

		case OP_LOADU1_MEMBASE:
			/* See previous comment. */
			if ((inst->dreg == sh4_r0 || register_not_assigned(inst->dreg)) &&
			    SH4_CHECK_RANGE_movb_dispRy_R0(inst->inst_offset)) {
				inst->opcode = OP_SH4_LOADU1_MEMBASE_R0;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, inst, 0);
				inst->opcode = OP_SH4_LOADU1;
			}
			break;

		case OP_LOADU2_MEMBASE:
			/* See previous comment. */
			if ((inst->dreg == sh4_r0 || register_not_assigned(inst->dreg)) &&
			    SH4_CHECK_RANGE_movw_dispRy_R0(inst->inst_offset)) {
				inst->opcode = OP_SH4_LOADU2_MEMBASE_R0;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, inst, 0);
				inst->opcode = OP_SH4_LOADU2;
			}
			break;

		case OP_LOAD_MEMBASE:
		case OP_LOADI4_MEMBASE:
		case OP_LOADU4_MEMBASE:
			/* See previous comment. */
			if (SH4_CHECK_RANGE_movl_dispRy(inst->inst_offset)) {
				inst->opcode = OP_SH4_LOADI4_MEMBASE;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, inst, 0);
				inst->opcode = OP_SH4_LOADI4;
			}
			break;

		case OP_VOIDCALL_MEMBASE:
		case OP_CALL_MEMBASE: {
			MonoInst *new_inst = NULL;

			/* The opcodes call*_membase are splitted into
			   load_membase + call*_reg. */
			MONO_INST_NEW(cfg, new_inst, OP_LOAD_MEMBASE);
			mono_bblock_insert_before_ins(basic_block, inst, new_inst);

			/* Link the new opcodes together. */
			new_inst->inst_basereg = inst->inst_basereg;
			new_inst->inst_offset = inst->inst_offset;
			new_inst->dreg = mono_regstate_next_int(cfg->rs);

			inst->inst_offset = 0;
			inst->sreg1 = new_inst->dreg;

			/* Convert call*_membase to call*_reg. */
			if (inst->opcode == OP_VOIDCALL_MEMBASE)
				inst->opcode = OP_VOIDCALL_REG;
			else
				inst->opcode = OP_CALL_REG;

			/* See comment about OP_SH4_LOAD*. */
			if (SH4_CHECK_RANGE_movl_dispRy(new_inst->inst_offset)) {
				new_inst->opcode = OP_SH4_LOADI4_MEMBASE;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, new_inst, 0);
				new_inst->opcode = OP_SH4_LOADI4;
			}

			break;
		}

		default:
			break;
		}
	}

	/* Save the state of the virtual-register allocator. */
	basic_block->max_vreg = cfg->rs->next_vreg;

	return;
}

/* Free the space used by parameters, computed into mono_arch_call_opcode(). */
static inline void free_args_area(MonoCompile *cfg, guint8 **buffer)
{
	if (cfg->arch.argalloc_size != 0) {
		if (SH4_CHECK_RANGE_add_imm(cfg->arch.argalloc_size))
			sh4_add_imm(cfg, buffer, cfg->arch.argalloc_size, sh4_sp);
		else {
			/* sh4_temp belongs to clobbered registers during a call,
			   so we can reuse it here. */
			sh4_load(buffer, cfg->arch.argalloc_size, sh4_temp);
			sh4_add(cfg, buffer, sh4_temp, sh4_sp);
		}
	}
}

void mono_arch_output_basic_block(MonoCompile *cfg, MonoBasicBlock *basic_block)
{
	MonoInst *inst = NULL;
	guint8 *buffer = NULL;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p, %p", cfg, basic_block);
	SH4_CFG_DEBUG(4) SH4_DEBUG("method: %s", cfg->method->name);

	mono_debug_open_block(cfg, basic_block, cfg->code_len);

	MONO_BB_FOR_EACH_INS(basic_block, inst) {
		guint32 length_max = 0;
		int length   = 0;
		guint8 *code = NULL;

		/* Get the 'len' field of this opcode, specify into cpu-sh4.md. */
		length_max = *((guint8 *)ins_get_spec(inst->opcode) + MONO_INST_LEN);

		/* Reallocate enough room to store the SH4 instructions
		   used to implement the current opcode. */
		while (cfg->code_len + length_max > cfg->code_size) {
			cfg->code_size *= 2;
			cfg->native_code = g_realloc(cfg->native_code, cfg->code_size);
			mono_jit_stats.code_reallocs++;
		}

		/* A chunk of code memory was previously allocated (native_code)
		   then partially used (code_len) into mono_arch_emit_prolog
		   and/or into this function. */
		buffer = cfg->native_code + cfg->code_len;
		code   = buffer; /* Save the start address to make sanity checks later. */

		mono_debug_record_line_number(cfg, inst, buffer - cfg->native_code);

		SH4_CFG_DEBUG(4)
			SH4_DEBUG("opcode '%s':\n"
				  "	length	= %d\n"
				  "	sreg1	= %d\n"
				  "	sreg2	= %d\n"
				  "	dreg	= %d\n"
				  "	offset	= %d\n"
				  "	imm	= %d",
				  mono_inst_name(inst->opcode),
				  (int)length_max,
				  (int)inst->sreg1,
				  (int)inst->sreg2,
				  (int)inst->dreg,
				  (int)inst->inst_offset,
				  (int)inst->inst_imm);

		switch (inst->opcode) {
		case OP_ADD_IMM:
		case OP_IADD_IMM:
			/* MD: int_add_imm: clob:1 dest:i src1:i len:2 */
			/* MD: add_imm: clob:1 dest:i src1:i len:2 */
			g_assert(SH4_CHECK_RANGE_add_imm(inst->inst_imm));
			sh4_add_imm(cfg, &buffer, inst->inst_imm, inst->dreg);
			break;

		case CEE_ADD:
		case OP_IADD:
			/* MD: int_add: clob:1 dest:i src1:i src2:i len:2 */
			/* MD: add: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_add(cfg, &buffer, inst->sreg2, inst->dreg);
			break;

		case OP_ISUB:
			/* MD: int_sub: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_sub(cfg, &buffer, inst->sreg2, inst->dreg);
			break;

		case OP_SH4_OR_IMM_R0:
			/* MD: sh4_or_imm_R0: clob:1 dest:z src1:z len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			g_assert(inst->sreg1 == sh4_r0);
			sh4_or_imm_R0(cfg, &buffer, inst->inst_imm);
			break;

		case OP_IOR:
			/* MD: int_or: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_or(cfg, &buffer, inst->sreg2, inst->dreg);
			break;

		case OP_SH4_XOR_IMM_R0:
			/* MD: sh4_xor_imm_R0: clob:1 dest:z src1:z len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			g_assert(inst->sreg1 == sh4_r0);
			sh4_xor_imm_R0(cfg, &buffer, inst->inst_imm);
			break;

		case OP_IXOR:
			/* MD: int_xor: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_xor(cfg, &buffer, inst->sreg2, inst->dreg);
			break;

		case OP_SH4_AND_IMM_R0:
			/* MD: sh4_and_imm_R0: clob:1 dest:z src1:z len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			g_assert(inst->sreg1 == sh4_r0);
			sh4_and_imm_R0(cfg, &buffer, inst->inst_imm);
			break;

		case OP_IAND:
			/* MD: int_and: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_and(cfg, &buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_ISHL_IMM:
		case OP_SHL_IMM:
			/* MD: int_shl_imm: clob:1 dest:i src1:i len:2 */
			/* MD: shl_imm: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			g_assert(sh4_has_shift_inst_imm(inst->inst_imm));
			if(inst->inst_imm == 1) {
				sh4_shll(cfg,&buffer,inst->dreg);
			}
			else if (inst->inst_imm == 2) {
				sh4_shll2(cfg,&buffer,inst->dreg);
			}
			else if (inst->inst_imm == 8) {
				sh4_shll8(cfg,&buffer,inst->dreg);
			}
			else if (inst->inst_imm == 16) {
				sh4_shll16(cfg,&buffer,inst->dreg);
			}
			else {
				g_assert_not_reached();
			}
			break;

		case OP_ISHL:
			/* Note: shld Rm,Rn instruction performs a logical shift
			 * where "the dynamic shift direction" is indicated by
			 * Rm. Clearly "if Rm is less than zero, [instruction] is
			 * a logical right shift".
			 * ECMA-335 standard (June 2006) does not specify what
			 * happens if second argument of shl instruction is negative
			 * (see p. 92 section III).
			 * We're going to suppose that behaviour is unspecified,
			 * which from a practical point of view means that we can
			 * map shld instruction directly.
			 *
			 * By comparison, note that ISO/IEC 1989 standard that defines
			 * the C language states in this case that "behavior is
			 * undefined".
			 */

			/* MD: int_shl: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shld(cfg,&buffer,inst->sreg2,inst->dreg);
			break;

		case OP_IMUL:
			/* MD: int_mul: dest:i src1:i src2:i len:4 */
			/* 32*32 bit multiplication -> result in MACL reg.
			 * transfer MACL into destination reg. */
			sh4_mull(cfg,&buffer,inst->sreg1,inst->sreg2);
			sh4_sts_MACL(cfg,&buffer,inst->dreg);
			break;

		case OP_SH4_CMPEQ:
			/* MD: sh4_cmpeq: src1:i src2:i len:2 */
			sh4_cmpeq(cfg, &buffer, inst->sreg1, inst->sreg2);
			break;

		case OP_SH4_CMPEQ_IMM_R0:
			/* MD: sh4_cmpeq_imm_R0: src1:z len:2 */
			g_assert(inst->sreg1 == sh4_r0);
			sh4_cmpeq_imm_R0(cfg, &buffer, inst->inst_imm);
			break;

		case OP_SH4_CMPGT:
			/* MD: sh4_cmpgt: src1:i src2:i len:2 */
			sh4_cmpgt(cfg, &buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_CMPHI:
			/* MD: sh4_cmphi: src1:i src2:i len:2 */
			sh4_cmphi(cfg, &buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_CMPGE:
			/* MD: sh4_cmpge: src1:i src2:i len:6 */
			sh4_cmpge(cfg, &buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_CMPHS:
			/* MD: sh4_cmphs: src1:i src2:i len:6 */
			sh4_cmphs(cfg, &buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_MOVT:
			/* MD: sh4_movt: dest:i len:2 */
			sh4_movt(cfg, &buffer, inst->dreg);
			break;

		case OP_SH4_PUSH_ARG:
			/* MD: sh4_push_arg: src1:i len:2 */
		case OP_OUTARG_MEMBASE:
			/* MD: outarg_membase: src1:i len:2 */
			sh4_movl_decRx(cfg, &buffer, inst->sreg1, sh4_sp);
			break;

		case OP_SH4_STOREI1_MEMBASE_R0:
			/* MD: sh4_storei1_membase_R0: src1:z dest:b len:2 */
			g_assert(inst->sreg1 == sh4_r0);
			sh4_movb_R0_dispRx(cfg, &buffer, inst->inst_offset, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI1:
			/* MD: sh4_storei1: src1:i dest:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movb_indRx(cfg, &buffer, inst->sreg1, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI2_MEMBASE_R0:
			/* MD: sh4_storei2_membase_R0: src1:z dest:b len:2 */
			g_assert(inst->sreg1 == sh4_r0);
			sh4_movw_R0_dispRx(cfg, &buffer, inst->inst_offset, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI2:
			/* MD: sh4_storei2: src1:i dest:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movw_indRx(cfg, &buffer, inst->sreg1, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI4_MEMBASE:
			/* MD: sh4_storei4_membase: src1:i dest:b len:2 */
			sh4_movl_dispRx(cfg, &buffer, inst->sreg1, inst->inst_offset, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI4:
			/* MD: sh4_storei4: src1:i dest:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movl_indRx(cfg, &buffer, inst->sreg1, inst->inst_destbasereg);
			break;

		case OP_SH4_LOADI1_MEMBASE_R0:
			/* MD: sh4_loadi1_membase_R0: dest:z src1:b len:2 */
			g_assert(inst->dreg == 0);
			sh4_movb_dispRy_R0(cfg, &buffer, inst->inst_offset, inst->inst_basereg);
			break;

		case OP_SH4_LOADI1:
			/* MD: sh4_loadi1: dest:i src1:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movb_indRy(cfg, &buffer, inst->inst_basereg, inst->dreg);
			break;

		case OP_SH4_LOADI2_MEMBASE_R0:
			/* MD: sh4_loadi2_membase_R0: dest:z src1:b len:2 */
			g_assert(inst->dreg == 0);
			sh4_movw_dispRy_R0(cfg, &buffer, inst->inst_offset, inst->inst_basereg);
			break;

		case OP_SH4_LOADI2:
			/* MD: sh4_loadi2: dest:i src1:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movw_indRy(cfg, &buffer, inst->inst_basereg, inst->dreg);
			break;

		case OP_SH4_LOADU1_MEMBASE_R0:
			/* MD: sh4_loadu1_membase_R0: dest:z src1:b len:4 */
			g_assert(inst->dreg == 0);
			sh4_movb_dispRy_R0(cfg, &buffer, inst->inst_offset, inst->inst_basereg);
			sh4_extub(cfg, &buffer, sh4_r0, sh4_r0); /* Adjust the sign. */
			break;

		case OP_SH4_LOADU1:
			/* MD: sh4_loadu1: dest:i src1:b len:4 */
			g_assert(inst->inst_offset == 0);
			sh4_movb_indRy(cfg, &buffer, inst->inst_basereg, inst->dreg);
			sh4_extub(cfg, &buffer, inst->dreg, inst->dreg); /* Adjust the sign. */
			break;

		case OP_SH4_LOADU2_MEMBASE_R0:
			/* MD: sh4_loadu2_membase_R0: dest:z src1:b len:4 */
			g_assert(inst->dreg == 0);
			sh4_movw_dispRy_R0(cfg, &buffer, inst->inst_offset, inst->inst_basereg);
			sh4_extuw(cfg, &buffer, sh4_r0, sh4_r0); /* Adjust the sign. */
			break;

		case OP_SH4_LOADU2:
			/* MD: sh4_loadu2: dest:i src1:b len:4 */
			g_assert(inst->inst_offset == 0);
			sh4_movw_indRy(cfg, &buffer, inst->inst_basereg, inst->dreg);
			sh4_extuw(cfg, &buffer, inst->dreg, inst->dreg); /* Adjust the sign. */
			break;

		case OP_SH4_LOADI4_MEMBASE:
			/* MD: sh4_loadi4_membase: dest:i src1:b len:2 */
			sh4_movl_dispRy(cfg, &buffer, inst->inst_offset, inst->inst_basereg, inst->dreg);
			break;

		case OP_SH4_LOADI4:
			/* MD: sh4_loadi4: dest:i src1:i len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movl_indRy(cfg, &buffer, inst->inst_basereg, inst->dreg);
			break;

		case OP_ICONST:
			/* MD: iconst: dest:i len:12 */
			if (SH4_CHECK_RANGE_mov_imm(inst->inst_c0))
				sh4_mov_imm(cfg, &buffer, inst->inst_c0, inst->dreg);
			else
				sh4_load(&buffer, inst->inst_c0, inst->dreg);
			break;

		case OP_VOIDCALL:
			/* MD: voidcall: clob:c len:30 */
		case OP_CALL: {
			/* MD: call: dest:z clob:c len:30 */
			MonoCallInst *call = (MonoCallInst*)inst;
			MonoJumpInfoType type;
			gpointer target = NULL;

			/* patch cst-pool with call destination */
			if (inst->flags & MONO_INST_HAS_METHOD) {
				type = MONO_PATCH_INFO_METHOD;
				target = call->method;
			}
			else {
				type = MONO_PATCH_INFO_ABS;
				target = (gpointer)call->fptr;
			}

			/* Please, update mono_arch_patch_callsite() according
			   to any changes made on this code-generation. CV */

			/* TODO - CV : optimize with sh4_bsr if possible. */

			sh4_load(&buffer, 0, sh4_temp);
			mono_add_patch_info(cfg, buffer - 4 - cfg->native_code, type, target);

			sh4_jsr_indRx(cfg, &buffer, sh4_temp);
			sh4_nop(cfg, &buffer); /* delay slot */

			free_args_area(cfg, &buffer);
			break;
		}

		case OP_VOIDCALL_REG:
			/* MD: voidcall_reg: src1:i clob:c len:18 */
		case OP_CALL_REG:
			/* MD: call_reg: dest:z src1:i clob:c len:18 */
			sh4_jsr_indRx(cfg, &buffer, inst->sreg1);
			sh4_nop(cfg, &buffer); /* delay slot */

			free_args_area(cfg, &buffer);
			break;

		case OP_CALL_HANDLER:
			/* MD: call_handler: len:16 */
			sh4_load(&buffer, 0, sh4_temp);
			mono_add_patch_info(cfg, buffer - 4 - cfg->native_code, MONO_PATCH_INFO_BB, inst->inst_target_bb);

			sh4_jsr_indRx(cfg, &buffer, sh4_temp);
			sh4_nop(cfg, &buffer); /* delay slot */
			break;

		case OP_THROW:
			/* MD: throw: src1:i len:20 */
			if (inst->sreg1 != MONO_SH4_REG_FIRST_ARG)
				sh4_mov(cfg, &buffer, inst->sreg1, MONO_SH4_REG_FIRST_ARG);

			sh4_load(&buffer, 0, sh4_temp);
			mono_add_patch_info(cfg, buffer - 4 - cfg->native_code, MONO_PATCH_INFO_INTERNAL_METHOD, (gpointer)"mono_arch_throw_exception");

			sh4_jsr_indRx(cfg, &buffer, sh4_temp);
			sh4_nop(cfg, &buffer); /* delay slot */

			/* Should never return and help the unwinder. */
			sh4_die(cfg, &buffer);
			break;

		case OP_MOVE:
			/* MD: move: dest:i src1:i len:2 */
			if (inst->sreg1 != inst->dreg)
				sh4_mov(cfg, &buffer, inst->sreg1, inst->dreg);
			break;

		/**
		 * The START_HANDLER instruction marks the beginning of a handler
		 * block. It is called using a call instruction, so sh4_pr contains
		 * the return address. Since the handler executes in the same stack
		 * frame as the method itself, we can't use save/restore to save
		 * the return address. Instead, we save it into a dedicated
		 * variable.
		 */
		case OP_START_HANDLER: {
			/* MD: start_handler: len:6 */
			MonoInst *spvar = mono_find_spvar_for_region(cfg, basic_block->region);

			/* We enforce Rtemp here because the SH4 instruction used to save
			   the return address clobbered the destination register. */
			if (spvar->inst_basereg != sh4_temp)
				sh4_mov(cfg, &buffer, spvar->inst_basereg, sh4_temp);

			if (SH4_CHECK_RANGE_add_imm(spvar->inst_offset))
				sh4_add_imm(cfg, &buffer, spvar->inst_offset, sh4_temp);
			else {
				NOT_IMPLEMENTED;
			}

			sh4_stsl_PR_decRx(cfg, &buffer, sh4_temp);

			break;
		}

		/* Restore the return address saved with the opcode "start_handler",
		 * and return the value in "sreg1" if it is an "endfilter". */
		case OP_ENDFILTER:
			/* MD: endfilter: src1:z len:10 */
			/* The local allocator will put the result into sh4_r0. */
		case OP_ENDFINALLY: {
			/* MD: endfinally: len:10 */
			MonoInst *spvar = mono_find_spvar_for_region(cfg, basic_block->region);

			/* We enforce Rtemp here because the SH4 instruction used to save
			   the return address clobbered the source register. */
			if (spvar->inst_basereg != sh4_temp)
				sh4_mov(cfg, &buffer, spvar->inst_basereg, sh4_temp);

			if (SH4_CHECK_RANGE_add_imm(spvar->inst_offset))
				sh4_add_imm(cfg, &buffer, spvar->inst_offset, sh4_temp);
			else {
				NOT_IMPLEMENTED;
			}

			sh4_ldsl_incRx_PR(cfg, &buffer, sh4_temp);

			sh4_rts(cfg, &buffer);
			sh4_nop(cfg, &buffer);

			break;
		}

		case OP_BR: {
			/* MD: br: len:16 */
			MonoJumpInfoType type;
			gpointer target = NULL;
			guint8 *address = NULL;
			int displace = 0;

			if (inst->flags & MONO_INST_BRLABEL) {
				type = MONO_PATCH_INFO_LABEL;
				target = inst->inst_i0;
				displace = inst->inst_i0->inst_c0;
			}
			else {
				type = MONO_PATCH_INFO_BB;
				target = inst->inst_target_bb;
				displace = inst->inst_target_bb->native_offset;
			}

			address = cfg->native_code + displace;

			/* Use the optimal instruction if possible. */
			if (displace != 0 && SH4_CHECK_RANGE_bra_label(buffer, address)) {
				sh4_bra_label(cfg, &buffer, address);
				sh4_nop(cfg, &buffer);
				break;
			}

			sh4_load(&buffer, 0, sh4_temp);
			mono_add_patch_info(cfg, buffer - 4 - cfg->native_code, type, target);

			sh4_jmp_indRx(cfg, &buffer, sh4_temp);
			sh4_nop(cfg, &buffer);

			break;
		}

		case OP_BR_REG:
			/* MD: br_reg: src1:i len:4 */
			sh4_jmp_indRx(cfg, &buffer, inst->sreg1);
			sh4_nop(cfg, &buffer);
			break;

		case OP_SH4_BT: {
			MonoJumpInfoType type;
			gpointer target = NULL;
			guint8 *address = NULL;
			guint8 *patch = NULL;
			int displace = 0;
			/* MD: sh4_bt: len:18 */
		case OP_SH4_BF:
			/* MD: sh4_bf: len:18 */

			/* Find which kind of relocation should be used. */
			if (inst->backend.data == (gpointer)-1) {
				type = MONO_PATCH_INFO_EXC;
				target = inst->inst_p1;
				displace = 0; /* Not used for exceptions. */
			}
			else if (inst->flags & MONO_INST_BRLABEL) {
				type = MONO_PATCH_INFO_LABEL;
				target = inst->inst_i0;
				displace = inst->inst_i0->inst_c0;
			}
			else {
				type = MONO_PATCH_INFO_BB;
				target = inst->inst_true_bb;
				displace = inst->inst_true_bb->native_offset;
			}

			address = cfg->native_code + displace;

			/* Use the optimal instruction if possible.
			   Note: CHECK_RANGE(bt) and CHECK_RANGE(bf) are equivalent. */
			if (displace != 0 && SH4_CHECK_RANGE_bt_label(buffer, address)) {
				if (inst->opcode == OP_SH4_BT) {
					sh4_bt_label(cfg, &buffer, address);
				}
				else {
					sh4_bf_label(cfg, &buffer, address);
				}
				break;
			}

			/* Reverse the test to skip the unconditional jump. */
			patch = buffer;
			sh4_die(cfg, &buffer); /* patch slot for : bf/t_label "skip_jump" */

			sh4_load(&buffer, 0, sh4_temp);
			mono_add_patch_info(cfg, buffer - 4 - cfg->native_code, type, target);

			sh4_jmp_indRx(cfg, &buffer, sh4_temp);
			sh4_nop(cfg, &buffer);

			/* Back patch the reversed test. */
			if (inst->opcode == OP_SH4_BT) {
				sh4_bf_label(NULL, &patch, buffer);
			}
			else {
				sh4_bt_label(NULL, &patch, buffer);
			}

			break;
		}

		case OP_LABEL: {
			/* MD: label: len:0 */
			inst->inst_c0 = code - cfg->native_code;
			break;
		}

		case OP_CHECK_THIS: {
			/* MD: checkthis: src1:i len:2 */
			/* Trig an exception if sreg1 can not be dereferenced,
			   might be misaligned in case of vtypes so use a byte load. */
			sh4_movb_indRy(cfg, &buffer, inst->sreg1, sh4_temp);
			break;
		}

			/* Support for spilled variables. */
		case OP_STORE_MEMBASE_REG:
			/* MD: store_membase_reg: len:2 */
			g_assert(inst->inst_destbasereg == sh4_fp);
			if (SH4_CHECK_RANGE_movl_dispRx(inst->inst_offset)) {
				sh4_movl_dispRx(cfg, &buffer, inst->sreg1, inst->inst_offset, inst->inst_destbasereg);
			}
			else {
				NOT_IMPLEMENTED;
				/* decompose_op_offset2base(cfg, basic_block, inst, 1);
				   inst->opcode = OP_SH4_STOREI4; */
			}
			break;

			/* Support for spilled variables. */
		case OP_LOAD_MEMBASE:
			/* MD: load_membase: len:2 */
			g_assert(inst->inst_basereg == sh4_fp);
			if (SH4_CHECK_RANGE_movl_dispRy(inst->inst_offset)) {
				sh4_movl_dispRy(cfg, &buffer, inst->inst_offset, inst->inst_basereg, inst->dreg);
			}
			else {
				NOT_IMPLEMENTED;
				/* decompose_op_offset2base(cfg, basic_block, inst, 0);
				   inst->opcode = OP_SH4_LOADI4; */
			}
			break;

		case OP_NOP:		/* MD: nop: len:0 */
		case OP_DUMMY_USE:	/* MD: dummy_use: len:0 */
		case OP_DUMMY_STORE:	/* MD: dummy_store: len:0 */
		case OP_NOT_REACHED:	/* MD: not_reached: len:0 */
		case OP_NOT_NULL:	/* MD: not_null: len:0 */
			break;

		case OP_JUMP_TABLE:
			/* MD: jump_table: dest:i len:12 */
			sh4_load(&buffer, 0, inst->dreg);
			mono_add_patch_info(cfg, buffer - 4 - cfg->native_code,
					    (MonoJumpInfoType)inst->inst_i1, inst->inst_p0);
			break;

		default:
			g_warning("unknown opcode %s (0x%x)\n", mono_inst_name(inst->opcode), inst->opcode);
			SH4_DEBUG("opcode '%s':\n"
				  "	sreg1	= %d\n"
				  "	sreg2	= %d\n"
				  "	dreg	= %d\n"
				  "	offset	= %d\n"
				  "	imm	= %d",
				  mono_inst_name(inst->opcode),
				  (int)inst->sreg1,
				  (int)inst->sreg2,
				  (int)inst->dreg,
				  (int)inst->inst_offset,
				  (int)inst->inst_imm);
			g_assert_not_reached();
		}

		/* Sanity checks. */
		length = buffer - code;
		if (length > length_max) {
			g_warning("max length of the opcode %s is at least %d, not %d\n",
				  mono_inst_name(inst->opcode), length, length_max);
			g_assert_not_reached();
		}

		cfg->code_len = buffer - cfg->native_code;
	}

	/* mono_arch_flush_icache() is called into the caller mini.c:mono_codegen(). */

	return;
}

/**
 * Process the patch data created during the instruction build process,
 * i.e. in mono_arch_output_basic_block(). This resolves jumps, calls,
 * variables, ...
 *
 * Patching branches is too much tricky because the constant pool
 * (to be patched) is not always at the same offset from the branch
 * instruction. Typically, you have two possibilities :
 *
 *     movl @cstpool, R0
 *     braf r0
 *     nop
 *     .word ! to be patched
 *     .word ! to be patched
 *
 * or :
 *
 *     movl @cstpool, R0
 *     braf r0
 *     nop
 *     nop
 *     .word ! to be patched
 *     .word ! to be patched
 *
 * That's why all resolutions are absolute.
 */
void mono_arch_patch_code(MonoMethod *method, MonoDomain *domain, guint8 *code, MonoJumpInfo *patch_info, gboolean run_cctors)
{
	SH4_EXTRA_DEBUG("args => %p, %p, %p, %p, %d", method, domain, code, patch_info, run_cctors);

	for (; patch_info != NULL; patch_info = patch_info->next){
		guint8 *patch = NULL;
		guint8 *target = NULL;

		patch = patch_info->ip.i + code;

		SH4_EXTRA_DEBUG("type = %d", patch_info->type);

		switch (patch_info->type) {

		case MONO_PATCH_INFO_SWITCH:
		case MONO_PATCH_INFO_LABEL:
		case MONO_PATCH_INFO_BB:
		case MONO_PATCH_INFO_METHOD:
		case MONO_PATCH_INFO_ABS:
		case MONO_PATCH_INFO_INTERNAL_METHOD:
		case MONO_PATCH_INFO_CLASS_INIT:
			/* Absolute. */
			target = mono_resolve_patch_target(method, domain, code, patch_info, run_cctors);
			break;

		case MONO_PATCH_INFO_IP:
			target = code + (guint32)patch_info->data.target;
			break;

		default:
			NOT_IMPLEMENTED;
			break;
		}

		SH4_EXTRA_DEBUG("*0x%x = 0x%x", (guint32)patch, (guint32)target);

		sh4_emit32(&patch, (guint32)target);

		/* mono_arch_flush_icache() is called into the caller mini.c:mono_codegen(). */
	}

	return;
}

void mono_arch_peephole_pass_1(MonoCompile *cfg, MonoBasicBlock *bb)
{
	/* TODO - CV */
	return;
}

void mono_arch_peephole_pass_2(MonoCompile *cfg, MonoBasicBlock *bb)
{
	/* TODO - CV */
	return;
}

/**
 * Print platform-specific opcode details.
 */
gboolean mono_arch_print_tree(MonoInst *tree, int arity)
{
	gboolean done = 0;

	switch (tree->opcode) {
	case OP_SH4_CMPEQ_IMM_R0:
		printf("[%s,0x%x]", mono_arch_regname(sh4_r0), tree->inst_imm);
		done = 1;
		break;

	case OP_SH4_CMPEQ:
	case OP_SH4_CMPGT:
		printf("[%s,%s]", mono_arch_regname(tree->sreg1), mono_arch_regname(tree->sreg2));
		done = 1;
		break;

	default:
		done = 0;
		break;
	}

	return done;
}

/**
 * Return the cost, in number of memory references, of the action of
 * allocating the variable VMV into a register during global register
 * allocation.
 */
guint32 mono_arch_regalloc_cost(MonoCompile *cfg, MonoMethodVar *vmv)
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

/* Emits IR to push a vtype to the stack. */
void mono_arch_emit_outarg_vt(MonoCompile *cfg, MonoInst *inst, MonoInst *src)
{
	/* TODO - CV */
	NOT_IMPLEMENTED;
	return;
}
