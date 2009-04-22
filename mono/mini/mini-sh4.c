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
 *   Hervé Knochel (herve.knochel@st.com)
 */

#include <asm/cachectl.h> /* CACHEFLUSH_*, */
#include <sys/syscall.h>  /* __NR_cacheflush, */
#include <unistd.h>       /* syscall(2), */
#include <glib.h>

#include "mini.h"
#include "ir-emit.h"
#include "cpu-sh4.h"
#include "cstpool-sh4.h"

#define ALIGN_TO(val, align) ((((guint32)val) + ((align) - 1)) & ~((align) - 1))

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
		valuetype,
		typedbyref,
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


/* Prevent following arguments from being passed into registers */
static inline void force_stack(SH4IntRegister *arg_reg)
{
	*arg_reg = MONO_SH4_REG_LAST_ARG + 1;
	return;
}

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
		(*stack_size) += 4;
	}

	SH4_EXTRA_DEBUG("arg_reg = %d", *arg_reg);
	SH4_EXTRA_DEBUG("stack_size = %d", *stack_size);
}

static inline void add_int64_arg(SH4IntRegister *arg_reg, guint32 *stack_size, struct arg_info *arg_info)
{
	arg_info->size = 8;
	arg_info->type = integer64;
	arg_info->offset = *stack_size;

	SH4_EXTRA_DEBUG("args => %d, %d, %p", *arg_reg, *stack_size, arg_info);

	/* "last_arg - 1" because the argument is either
	   entirely into 2 registers, or entirely onto the stack. */
	if (*arg_reg <= MONO_SH4_REG_LAST_ARG - 1) {
		arg_info->storage = into_register;
		arg_info->reg = *arg_reg;
		(*arg_reg) += 2;
	}
	else {
		if(*arg_reg == MONO_SH4_REG_LAST_ARG)
			force_stack(arg_reg);
		arg_info->storage = onto_stack;
		(*stack_size) += 8;
	}

	SH4_EXTRA_DEBUG("arg_reg = %d", *arg_reg);
	SH4_EXTRA_DEBUG("stack_size = %d", *stack_size);
}

static void add_valuetype(MonoCompile *cfg, MonoGenericSharingContext *context, MonoMethodSignature *signature,
			  MonoType *type, struct arg_info *arg_info, guint32 *stack_size)
{
	guint32 size;
	MonoClass *klass;

	/* Currently, valuetypes are always transmitted onto the stack. */

	klass = mono_class_from_mono_type(type);

	if (signature->pinvoke)
		size = mono_type_native_stack_size(&klass->byval_arg, NULL);
	else
		size = mini_type_stack_size(context, &klass->byval_arg, NULL);

	size = ALIGN_TO(size, 0x4);

	arg_info->type    = valuetype;
	arg_info->storage = onto_stack;
	arg_info->offset  = *stack_size;
	arg_info->size    = size;
	*stack_size       += size;

	SH4_CFG_DEBUG(4)
		SH4_DEBUG("add_valuetype - method %s, size %d, offset %d, stack_size %d",
			  cfg->method->name, size, arg_info->offset, *stack_size);

	return;
}

static inline void add_float64_arg(SH4FloatRegister *arg_reg, guint32 *stack_size, struct arg_info *arg_info)
{
	arg_info->size = 8;
	arg_info->type = float64;
	arg_info->offset = *stack_size;

	SH4_EXTRA_DEBUG("args => %d, %d, %p", *arg_reg, *stack_size, arg_info);

	/* "last_arg - 1" because the argument is either
	   entirely into 2 registers, or entirely onto the stack. */
	if (*arg_reg <= MONO_SH4_FREG_LAST_ARG - 1) {
		arg_info->storage = into_register;
		arg_info->reg = *arg_reg;
		(*arg_reg) += 2;
	}
	else {
#if 0 /* TODO - CV: Maybe we have to do something like that when supporting PInvoke calls. */
		if(*arg_reg == MONO_SH4_FREG_LAST_ARG)
			arg_reg = MONO_SH4_FREG_LAST_ARG + 1;
#endif
		arg_info->storage = onto_stack;
		(*stack_size) += 8;
	}

	SH4_EXTRA_DEBUG("arg_reg = %d", *arg_reg);
	SH4_EXTRA_DEBUG("stack_size = %d", *stack_size);
}

static inline void add_float32_arg(SH4FloatRegister *arg_reg, guint32 *stack_size, struct arg_info *arg_info)
{
	/* Currently float32 arguments are handled as float64, except in emit_prolog(). */
	arg_info->size = 8;
	arg_info->type = float32;
	arg_info->offset = *stack_size;

	SH4_EXTRA_DEBUG("args => %d, %d, %p", *arg_reg, *stack_size, arg_info);

	/* "last_arg - 1" because the argument is either
	   entirely into 2 registers, or entirely onto the stack. */
	if (*arg_reg <= MONO_SH4_FREG_LAST_ARG - 1) {
		/* TODO - CV: PInvoke support. */
		arg_info->storage = into_register;
		arg_info->reg = *arg_reg;
		(*arg_reg) += 2;
	}
	else {
		arg_info->storage = onto_stack;
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
static struct call_info *get_call_info(MonoCompile *cfg, MonoGenericSharingContext *context, MonoMethodSignature *signature)
{
	struct call_info *call_info = NULL;
	SH4IntRegister arg_reg = MONO_SH4_REG_FIRST_ARG;
	SH4FloatRegister arg_freg = MONO_SH4_FREG_FIRST_ARG;
	MonoType *basic_type = NULL;
	guint32 stack_size = 0;
	guint32 size = 0;
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
		call_info->ret.storage = into_register;
		call_info->ret.type = integer64;
		call_info->ret.reg = sh4_r0;
		break;

	case MONO_TYPE_R4:
		call_info->ret.storage = into_register;
		call_info->ret.type = float32;
		call_info->ret.reg = sh4_fr0;
		break;

	case MONO_TYPE_R8:
		call_info->ret.storage = into_register;
		call_info->ret.type = float64;
		call_info->ret.reg = sh4_dr0;
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
		add_valuetype(cfg, context, signature, signature->ret, &call_info->ret, &size /* unused */);
		/* SH4 ABI specifies register containing the @ of the aggregate. */
		call_info->ret.reg = sh4_r2;
		break;

	case MONO_TYPE_TYPEDBYREF:
		/* Something "à la" S390 looks better than options taken by others backends. */
		size = sizeof(MonoTypedRef);
		call_info->ret.reg     = sh4_r2;
		call_info->ret.type    = typedbyref;
		call_info->ret.size    = size;
		call_info->ret.storage = onto_stack;
		call_info->ret.offset  = stack_size;
		stack_size += ALIGN_TO(size, 4);
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
			add_int64_arg(&arg_reg, &stack_size, arg_info);
			break;

		case MONO_TYPE_R4:
			/* HK: TODO, handle pinvoke case */
			g_assert(signature->pinvoke == 0);
			add_float32_arg(&arg_freg, &stack_size, arg_info);
			break;

		case MONO_TYPE_R8:
			add_float64_arg(&arg_freg, &stack_size, arg_info);
			break;

		case MONO_TYPE_GENERICINST:
			if (mono_type_generic_inst_is_valuetype(signature->params[i]) == 0) {
				add_int32_arg(&arg_reg, &stack_size, arg_info);
				break;
			}
		/* else fall through. */
		case MONO_TYPE_VALUETYPE:
			add_valuetype(cfg, context, signature, signature->params[i], arg_info, &stack_size);
			break;

		case MONO_TYPE_TYPEDBYREF:
			size = sizeof(MonoTypedRef);
			arg_info->type    = typedbyref;
			arg_info->size    = size;
			arg_info->storage = onto_stack;
			arg_info->offset  = stack_size;
			stack_size += ALIGN_TO(size, 4);
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
	call_info->stack_usage = ALIGN_TO(stack_size, 0x4);
	call_info->stack_align_amount = call_info->stack_usage - stack_size;

	return call_info;
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
	call->stack_usage += 4;
	MONO_ADD_INS(cfg->cbb, inst);
}

/**
 * Satellite routine of mono_arch_emit_call for value type and
 * "typedbyref" arguments.
 */
static guint32 mono_emit_call_struct(MonoCompile *cfg, MonoCallInst *call,
				     struct arg_info *arg_info, guint32 index)
{
	MonoMethodSignature *signature;
	MonoInst *in;
	MonoInst *arg;
	guint32 size;
	guint32 align;

	in = call->args[index];
	g_assert (in->klass);

	signature = call->signature;

	if (arg_info->type==typedbyref) {
		size  = sizeof (MonoTypedRef);
		align = sizeof(gpointer);
	} else {
		size = mini_type_stack_size_full(cfg->generic_sharing_context,
						&in->klass->byval_arg, &align,
						signature->pinvoke);
	}

	if(size>0) {
		MONO_INST_NEW(cfg,arg,OP_OUTARG_VT);
		arg->sreg1 = in->dreg;
		arg->klass = in->klass;
		arg->backend.size = size;
		arg->inst_p0 = call;
		arg->inst_p1 = mono_mempool_alloc (cfg->mempool, sizeof (struct arg_info));
		memcpy (arg->inst_p1, arg_info, sizeof (struct arg_info));
		MONO_ADD_INS (cfg->cbb, arg);
	}

	SH4_CFG_DEBUG(4)
		SH4_DEBUG("emit_call_struct - size => %d, ", size);

	return size;
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
	int flag_i = 0;
	int flag_f = 0;
	int i = 0;
	int j = 0;

	SH4_CFG_DEBUG(4)
		SH4_DEBUG("args => %p, %p", cfg, call);

	signature = call->signature;
	arg_count = signature->param_count + signature->hasthis;
	call_info = get_call_info(cfg, cfg->generic_sharing_context, signature);

	if (MONO_TYPE_ISSTRUCT(signature->ret)) {
		MonoInst *inst;

		MONO_INST_NEW(cfg, inst, OP_MOVE);
		inst->sreg1 = call->vret_var->dreg;
		inst->dreg  = mono_alloc_ireg(cfg);
		MONO_ADD_INS(cfg->cbb, inst);
		mono_call_inst_add_outarg_reg(cfg, call, inst->dreg, call_info->ret.reg, FALSE);
	}

	if (signature->pinvoke == 0 &&
	    signature->call_convention == MONO_CALL_VARARG)
		sentinelpos = signature->sentinelpos + (signature->hasthis ? 1 : 0);

	/* Used into mono_arch_output_basic_block():*CALL*
	   to free the space used by parameters after a call. */
	call->stack_usage = 0;

	/* First, put in order parameters passed into registers. */
	for (i = 0; i < arg_count; i++) {
		struct arg_info *arg_info = &(call_info->args[i]);
		MonoInst *arg = NULL;

		/* Emit the signature cookie just before the implicit arguments. */
		if (sentinelpos == i &&
		    signature->pinvoke == 0 &&
		    signature->call_convention == MONO_CALL_VARARG)
			emit_signature_cookie2(cfg, call);

		if (i >= signature->hasthis &&
		    MONO_TYPE_ISSTRUCT(signature->params[i - signature->hasthis])) {
			/* Currently structures are always passed onto the stack */
			continue;
		}

		/* Set flag_i (resp. flag_f) if the number of integer (resp. fp) registers
		   available is reached */

		flag_i |= ((arg_info->type == integer32 || arg_info->type == integer64) &&
			   (arg_info->storage != into_register));

		flag_f |= ((arg_info->type == float32 || arg_info->type == float64) &&
			   (arg_info->storage != into_register));

		/* Check if it is the last parameter passed into registers. */
		if (flag_i && flag_f)
			break;

		/* Arguments passed by reference are already handled in get_call_info(). */
		switch (arg_info->type) {
		case integer32:
			if (!flag_i) {
				MONO_INST_NEW(cfg, arg, OP_MOVE);
				arg->sreg1 = call->args[i]->dreg;
				arg->dreg  = mono_alloc_ireg(cfg);
				mono_call_inst_add_outarg_reg(cfg, call, arg->dreg, arg_info->reg, FALSE);
				MONO_ADD_INS(cfg->cbb, arg);
			}
			break;

		case integer64:
			if (!flag_i) {
				/* A long register N is splitted into two interger registers N+1, N+2. */
				MONO_INST_NEW(cfg, arg, OP_MOVE);
				arg->sreg1 = call->args[i]->dreg + 1;
				arg->dreg  = mono_alloc_ireg(cfg);
				mono_call_inst_add_outarg_reg(cfg, call, arg->dreg, arg_info->reg, FALSE);
				MONO_ADD_INS(cfg->cbb, arg);

				MONO_INST_NEW(cfg, arg, OP_MOVE);
				arg->sreg1 = call->args[i]->dreg + 2;
				arg->dreg  = mono_alloc_ireg(cfg);
				mono_call_inst_add_outarg_reg(cfg, call, arg->dreg, arg_info->reg + 1, FALSE);
				MONO_ADD_INS(cfg->cbb, arg);
			}
			break;

		case float32:
			/* HK: TODO, handle pinvoke case */
			g_assert(signature->pinvoke == 0);
			/* Fall through. */
		case float64:
			if (!flag_f) {
				MONO_INST_NEW(cfg, arg, OP_FMOVE);
				arg->sreg1 = call->args[i]->dreg;
				arg->dreg  = mono_alloc_freg(cfg);
				mono_call_inst_add_outarg_reg(cfg, call, arg->dreg, arg_info->reg, TRUE);
				MONO_ADD_INS(cfg->cbb, arg);
			}
			break;

		default:
			g_assert_not_reached();
			break;
		}
	}

	/*
	 * Then, push in reverse order parameters passed onto the stack.
	 *
	 *	:              :
	 *	|--------------| <- SP before parameter passing
	 *	| stacked arg3 |
	 *	|--------------|
	 *	| stacked arg2 |
	 *	|--------------|
	 *	| stacked arg1 |
	 *	|--------------| <- SP after parameter passing
	 *	:              :
	 */
	flag_i = 0;
	flag_f = 0;
	for (j = arg_count - 1; j >= 0; j--) {
		struct arg_info *arg_info = &(call_info->args[j]);
		MonoInst *arg = NULL;

		/* Set flag_i (resp. flag_f) if the number of integer (resp. fp) parameters
		   passed onto the stacked is reached */

		flag_i |= ((arg_info->type == integer32 || arg_info->type == integer64) &&
			   (arg_info->storage != onto_stack));

		flag_f |= ((arg_info->type == float32 || arg_info->type == float64) &&
			   (arg_info->storage != onto_stack));

		/* Check if it is the last parameter passed  onto the stack. */
		if (flag_i && flag_f)
			break;

		if (j >= signature->hasthis &&
		    MONO_TYPE_ISSTRUCT(signature->params[j - signature->hasthis])) {
			/* Currently structures are always passed onto the stack */
			call->stack_usage += mono_emit_call_struct(cfg,call,arg_info,j);
			continue;
		}

		/* Arguments passed by reference are already handled in get_call_info(). */
		switch (arg_info->type) {
		case integer32:
			if (!flag_i) {
				MONO_INST_NEW(cfg, arg, OP_SH4_PUSH_ARG);
				arg->sreg1 = call->args[j]->dreg;
				MONO_ADD_INS(cfg->cbb, arg);
				call->stack_usage += 4;
			}
			break;

		case integer64:
			if (!flag_i) {
			/* A long register N is splitted into two interger registers N+1, N+2. */
				MONO_INST_NEW(cfg, arg, OP_SH4_PUSH_ARG);
				arg->sreg1 = call->args[j]->dreg + 2;
				MONO_ADD_INS(cfg->cbb, arg);

				MONO_INST_NEW(cfg, arg, OP_SH4_PUSH_ARG);
				arg->sreg1 = call->args[j]->dreg + 1;
				MONO_ADD_INS(cfg->cbb, arg);

				call->stack_usage += 8;
			}
			break;

		case float32:
			/* HK: TODO, handle pinvoke case */
			g_assert(signature->pinvoke == 0);
			if (!flag_f) {
				MONO_INST_NEW(cfg, arg, OP_SH4_PUSH_F32ARG);
				arg->sreg1 = call->args[j]->dreg;
				MONO_ADD_INS(cfg->cbb, arg);
				call->stack_usage += 4;
			}
			break;

		case float64:
			if (!flag_f) {
				MONO_INST_NEW(cfg, arg, OP_SH4_PUSH_F64ARG);
				arg->sreg1 = call->args[j]->dreg;
				MONO_ADD_INS(cfg->cbb, arg);
				call->stack_usage += 8;
			}
			break;

		default:
			g_assert_not_reached();
			break;
		}
	}

	/* Emit the signature cookie in case no implicit arguments are specified. */
	if (signature->pinvoke == 0 &&
	    sentinelpos == arg_count &&
	    signature->call_convention == MONO_CALL_VARARG)
		emit_signature_cookie2(cfg, call);

	/* get_call_info() and emit_call() have to be kept in sync. */
	g_assert(call->stack_usage == call_info->stack_usage);

	g_free(call_info->args);
	g_free(call_info);
	return;
}

/* Emits IR to move its argument to the proper return register(s). */
void mono_arch_emit_setret(MonoCompile *cfg, MonoMethod *method, MonoInst *result)
{
	MonoType *ret = mini_type_get_underlying_type(cfg->generic_sharing_context, mono_method_signature(method)->ret);
	MonoInst *inst = NULL;

	if (ret->byref != 0) {
		NOT_IMPLEMENTED;
	}

	switch (ret->type) {
	case MONO_TYPE_VOID:
		break;

	case MONO_TYPE_I8:
	case MONO_TYPE_U8:
		/* A long register N is splitted into two integer registers N+1, N+2. */
		/* TODO - CV: maybe I can do here something as simple as 2 x OP_MOVE. */
		MONO_INST_NEW(cfg, inst, OP_SETLRET);
		inst->sreg1 = result->dreg + 1;
		inst->sreg2 = result->dreg + 2;
		MONO_ADD_INS(cfg->cbb, inst);
		break;

	case MONO_TYPE_R4:
		g_assert(method->wrapper_type != MONO_WRAPPER_RUNTIME_INVOKE);
		if (method->wrapper_type == MONO_WRAPPER_MANAGED_TO_NATIVE) {
 			MONO_INST_NEW(cfg, inst, OP_SH4_FCNVSD);
			inst->sreg1 = result->dreg;
			inst->dreg = result->dreg;
			MONO_ADD_INS(cfg->cbb, inst);
		}
	/* Fall through */
	case MONO_TYPE_R8:
		MONO_EMIT_NEW_UNALU(cfg, OP_FMOVE, cfg->ret->dreg, result->dreg);
		break;

	default:
		MONO_EMIT_NEW_UNALU(cfg, OP_MOVE, cfg->ret->dreg, result->dreg);
		break;
	}

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
	guint32 locals_alignement = 0;
	gint32 *locals_offsets = NULL;
	int locals_padding = 0;
	int i = 0;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	/* Spill variables slots are allocated from bottom to top. */
	cfg->flags |= MONO_CFG_HAS_SPILLUP;

	signature = mono_method_signature(cfg->method);
	call_info = get_call_info(cfg, cfg->generic_sharing_context, signature);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------| <- SP
	 *	:              :
	 */

	cfg->frame_reg = sh4_fp;
	cfg->used_int_regs |= 1 << sh4_fp;

	/* Compute space used by local variables. */
	locals_offsets = mono_allocate_stack_slots_full(cfg, FALSE,
							(guint32 *)&cfg->stack_offset,
							&locals_alignement);
	if (locals_alignement != 0) {
		locals_padding = ALIGN_TO(locals_padding, locals_alignement);
	}
	cfg->stack_offset += locals_padding;

	SH4_CFG_DEBUG(4) SH4_DEBUG("locals stack size: %d", cfg->stack_offset);
	SH4_CFG_DEBUG(4) SH4_DEBUG("locals stack alignment: %d", locals_alignement);

	/* Specify how to access local variables. */
	for (i = cfg->locals_start; i < cfg->num_varinfo; i++) {
		MonoInst *inst = cfg->varinfo[i];

		/* Nothing to do if the variable is already allocated
		   to a register or if it is unused. */
		if (locals_offsets[i] == -1) {
			SH4_CFG_DEBUG(4) SH4_DEBUG("local '%d' reg = %d", i, inst->dreg);
			continue;
		}

		/* Specify how to access this local variable. */
		inst->opcode = OP_REGOFFSET;
		inst->inst_basereg = cfg->frame_reg;
		inst->inst_offset = locals_padding + locals_offsets[i];

		SH4_CFG_DEBUG(4) SH4_DEBUG("local '%d' stack = %d", i, inst->inst_offset);
	}

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

	/* The locals area size is now fully known so specify where are saved parameters. */
	for (i = 0; i < signature->param_count + signature->hasthis; i++) {
		MonoInst *inst = cfg->args[i];
		guint32 align = 0;
		guint32 size = 0;

		/* Nothing to do if the variable is already allocated to a [global] register. */
		if (inst->opcode == OP_REGVAR) {
			SH4_CFG_DEBUG(4) SH4_DEBUG("arg '%d' reg = %d", i, inst->dreg);
			continue;
		}

		if (signature->hasthis != 0 && i == 0) {
			size = sizeof(gpointer);
			align = sizeof(gpointer);
		}
		else if (signature->pinvoke != 0)
			size = mono_type_native_stack_size(signature->params[i - signature->hasthis], &align);
		else
			size = mono_type_size(signature->params[i - signature->hasthis], (int *)&align);

		g_assert(size != 0);

		/* For the moment we save parameters into a 32 bits slot (at least).
		   If you want to change this behaviour, don't forget to adjust emit_prolog(). */
		if (align < 4)
			align = 4;

		/* Align the access on a `align`-bytes boundary. */
		cfg->stack_offset = ALIGN_TO(cfg->stack_offset, align);

		/* Specify how to access this argument. */
		inst->opcode = OP_REGOFFSET;
		inst->inst_basereg = cfg->frame_reg;
		inst->inst_offset = cfg->stack_offset;

		SH4_CFG_DEBUG(4) SH4_DEBUG("arg '%d' stack = %d, size = %d", i, cfg->stack_offset, size);

		cfg->stack_offset += size;
	}

	/* Align the frame on a 4-bytes boundary to avoid SIGBUS in prologue. */
	cfg->stack_offset = ALIGN_TO(cfg->stack_offset, 0x4);

	SH4_CFG_DEBUG(4) SH4_DEBUG("return type = %d", call_info->ret.type);

	/* Specify how to access the return value, see mono_arch_emit_setret(). */
	switch(call_info->ret.type) {
	case float32:
	case float64:
	case integer64:
	case integer32:
		cfg->ret->opcode  = OP_REGVAR;
		cfg->ret->inst_c0 = call_info->ret.reg;
		cfg->ret->dreg    = call_info->ret.reg;
		break;

	case valuetype:
	case typedbyref:
		/* In the new IR, the cfg->vret_addr variable represents the
		 * vtype return value. */
		cfg->vret_addr->opcode       = OP_REGOFFSET;
		cfg->vret_addr->inst_basereg = cfg->frame_reg;
		cfg->vret_addr->inst_offset  = cfg->stack_offset;
		/* We allocate 4 more bytes in the stack: it's the space
		 * that is needed to store the address of the structure. */
		cfg->stack_offset += sizeof(gpointer);
		break;

	case none: /* Nothing to do. */
		break;

	default:
		g_assert_not_reached();
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
#if 0
	int fpscr = 0;

	__asm__ __volatile__ ("sts fpscr, %0" : : "r"(fpscr));
	fpscr |= (1 << 19); /* Set the precision mode to 'double'. */
	fpscr |= (1 << 20); /* Set the transfer size mode to 'double'. */
	__asm__ __volatile__ ("lds %0, fpscr" : : "r"(fpscr));
#endif
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
	MonoMethodSignature *signature;
	struct call_info *call_info;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	signature = mono_method_signature(cfg->method);

	call_info = get_call_info(cfg, cfg->generic_sharing_context, signature);

	switch (call_info->ret.storage) {
	case into_register:
		cfg->ret_var_is_local = TRUE;
		break;

	case onto_stack:
		if (MONO_TYPE_ISSTRUCT(signature->ret))
			cfg->vret_addr = mono_compile_create_var(cfg, &mono_defaults.int_class->byval_arg, OP_ARG);
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

static inline void sh4_add_offset2base(MonoCompile *cfg, guint8 **buffer, int offset, SH4IntRegister base, SH4FloatRegister new_base)
{
	if (offset == 0) {
		sh4_mov(buffer, base, new_base);
	}
	else if (SH4_CHECK_RANGE_add_imm(offset)) {
		sh4_mov(buffer, base, new_base);
		sh4_add_imm(buffer, offset, new_base);
	}
	/* TODO - CV: optimize using sh4_multi_add_imm() if it
	   produces less instructions than sh4_load(). */
	else {
		if (cfg == NULL)
			sh4_load(buffer, offset, new_base);
		else
			sh4_cstpool_add(cfg, buffer, MONO_PATCH_INFO_NONE, &offset, new_base);
		sh4_add(buffer, base, new_base);
	}
}

static inline void sh4_base_store(MonoCompile *cfg, guint8 **buffer, SH4IntRegister src, int offset, SH4IntRegister base)
{
	if (SH4_CHECK_RANGE_movl_dispRx(offset)) {
		sh4_movl_dispRx(buffer, src, offset, base);
	}
	else {
		sh4_add_offset2base(cfg, buffer, offset, base, sh4_temp);
		sh4_movl_indRx(buffer, src, sh4_temp);
	}
}

static inline void sh4_base_load(MonoCompile *cfg, guint8 **buffer, int offset, SH4IntRegister base, SH4IntRegister dest)
{
	if (SH4_CHECK_RANGE_movl_dispRy(offset)) {
		sh4_movl_dispRy(buffer, offset, base, dest);
	}
	else {
		sh4_add_offset2base(cfg, buffer, offset, base, sh4_temp);
		sh4_movl_indRy(buffer, sh4_temp, dest);
	}
}

static inline void sh4_base_storef32(MonoCompile *cfg, guint8 **buffer, SH4FloatRegister src, int offset, SH4IntRegister base)
{
	sh4_add_offset2base(cfg, buffer, offset, base, sh4_temp);

	/* Convert back to simple precision.  */
	sh4_fcnvds_double_FPUL(buffer, src);
	sh4_fsts_FPUL(buffer, src);

	sh4_fmovs_indRx(buffer, src, sh4_temp);
}

static inline void sh4_base_loadf32(MonoCompile *cfg, guint8 **buffer, int offset, SH4IntRegister base, SH4FloatRegister dest)
{
	sh4_add_offset2base(cfg, buffer, offset, base, sh4_temp);

	sh4_fmovs_indRy(buffer, sh4_temp, dest);

	/* Convert [internally] to double precision, as specified by the ECMA. */
	sh4_flds_FPUL(buffer, dest);
	sh4_fcnvsd_FPUL_double(buffer, dest);
}

static inline void sh4_base_storef64(MonoCompile *cfg, guint8 **buffer, SH4FloatRegister src, int offset, SH4IntRegister base)
{
	sh4_add_offset2base(cfg, buffer, offset + 4, base, sh4_temp);

	sh4_fmovs_indRx(buffer, src + 1, sh4_temp);
	sh4_fmovs_decRx(buffer, src, sh4_temp);
}

static inline void sh4_base_loadf64(MonoCompile *cfg, guint8 **buffer, int offset, SH4IntRegister base, SH4FloatRegister dest)
{
	sh4_add_offset2base(cfg, buffer, offset, base, sh4_temp);

	sh4_fmovs_incRy(buffer, sh4_temp, dest);
	sh4_fmovs_indRy(buffer, sh4_temp, dest + 1);
}

/**
 * Create the instruction sequence for a function prologue.
 * Keep in sync with mono_arch_find_jit_info().
 */
guint8 *mono_arch_emit_prolog(MonoCompile *cfg)
{
	MonoMethodSignature *signature = NULL;
	struct call_info *call_info = NULL;
	guint8 *buffer = NULL;
	int saved_regs_size = 0;
	int i = 0;
	int j;
	guint32 size_struct = 0;
	guint32 nb_struct_on_stack = 0;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	if (cfg->method->wrapper_type == MONO_WRAPPER_NATIVE_TO_MANAGED) {
		NOT_IMPLEMENTED;
	}

	/* Initialize cst pools - for the moment keep running in low perf mode.*/
	sh4_cstpool_init(cfg, cstpool_mode_lowperf);

	signature = mono_method_signature(cfg->method);

	call_info = get_call_info(cfg, cfg->generic_sharing_context, signature);

	/* Allocate buffer memory. Each time you modify this prolog
	 * please modify the code size accordingly, or debugging is
	 * going to be painful!
	 * Note that we're very pessimistic here. */
	cfg->code_size = 0;
	for (i = 0; i < signature->param_count + signature->hasthis; i++) {
		struct arg_info *arg_info = &call_info->args[i];
		MonoInst *inst = cfg->args[i];

		if(inst->opcode != OP_REGVAR &&
		   arg_info->storage == onto_stack &&
		  (arg_info->type == valuetype ||
		   arg_info->type == typedbyref)) {

			size_struct = ALIGN_TO(arg_info->size, 0x4);
			cfg->code_size += (size_struct/4) * 32;
			nb_struct_on_stack++;
		}
	}

	cfg->code_size += 52 + (signature->param_count + signature->hasthis - nb_struct_on_stack) * 64;
	if(cfg->method->save_lmf != 0)
		cfg->code_size += 116;

	buffer = cfg->native_code = g_malloc(cfg->code_size);

	SH4_CFG_DEBUG(4)
		SH4_DEBUG("max prolog size %d, param_count %d, lmf %d, hasthis %d\n",
			cfg->code_size,
			signature->param_count,
			cfg->method->save_lmf,
			signature->hasthis);


	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============| <- SP
	 *	:              : Callee's frame.
	 */

	/* Save global registers. */
	for (i = 0; i < MONO_MAX_IREGS; i++) {
		if ((MONO_ARCH_CALLEE_SAVED_REGS & (1 << i)) != 0 &&
		    (cfg->used_int_regs          & (1 << i)) != 0) {
			sh4_movl_decRx(&buffer, (SH4IntRegister)i, sh4_sp);
			saved_regs_size += 4;
		}
	}

#if 0 /* Mono does not support yet global floating-point register allocation. */
	/* Save global floating-point registers. */
	for (i = 0; i < MONO_MAX_FREGS; i++) {
		if ((MONO_ARCH_CALLEE_SAVED_FREGS & (1 << i)) != 0 &&
		    (cfg->used_float_regs         & (1 << i)) != 0) {
			sh4_fmovl_decRx(&buffer, (SH4FloatRegister)i, sh4_sp);
			saved_regs_size += 4;
		}
	}
#endif

	/* Save the PR. */
	sh4_stsl_PR_decRx(&buffer, sh4_sp);
	saved_regs_size += 4;

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------| <- SP
	 *	:              :
	 */

	/* TODO - CV: optimizations => use SP only if needed, that is,
	 * only if there is a parameted passed onto the stack.
	 * Keep in sync with mono_arch_find_jit_info(). */
	sh4_mov(&buffer, sh4_sp, sh4_fp);

	/* The SH4 is a 16-bits instruction length CPU, so "immediate"
	 * values used for displacement addressing are really-really
	 * small, that's why we keep SP close to stacked parameters. */

	/* The space needed by local variables is computed into mono_arch_allocate_vars(),
	 * and the size of the spill area is computed into mini-codegen.c.
	 * Keep in sync with mono_arch_find_jit_info(). */
	if (cfg->stack_offset != 0) {
		if (SH4_CHECK_RANGE_add_imm(-cfg->stack_offset)) {
			sh4_add_imm(&buffer, -cfg->stack_offset, sh4_fp);
		}
		else if (SH4_CHECK_RANGE_mov_imm(cfg->stack_offset)) {
			sh4_mov_imm(&buffer, cfg->stack_offset, sh4_temp);
			sh4_sub(&buffer, sh4_temp, sh4_fp);
		}
		else {
			sh4_load(&buffer, cfg->stack_offset, sh4_temp);
			sh4_sub(&buffer, sh4_temp, sh4_fp);
		}
	}

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============|
	 *	|  saved reg.  | Callee's frame.
	 *	|--------------| <- SP
	 *	|  local var.  |
	 *	|--------------| <- FP
	 *	:              :
	 *
	 * If you want to load parameters stored in the stack, you have to remember
	 * they are pushed down from "saved_regs_size + call_info->stack_usage".
	 */

	if (call_info->ret.type == valuetype || call_info->ret.type == typedbyref) {
		struct arg_info *ret_info = &call_info->ret;
		MonoInst *inst_ret = cfg->vret_addr;

		SH4_CFG_DEBUG(4)
			SH4_DEBUG("Method %s, prolog ret.type = %s, offset value %d, size %d, return reg %s",
				  cfg->method->name,
				  ret_info->type == valuetype ? "valuetype" : "typedbyref",
				  inst_ret->inst_offset,
				  ret_info->size,
				  mono_arch_regname(ret_info->reg));

		sh4_base_store(NULL, &buffer, ret_info->reg, inst_ret->inst_offset, inst_ret->inst_basereg);
	}

	/* Copy arguments at the expected place : into register or onto the stack. */
	for (i = 0; i < signature->param_count + signature->hasthis; i++) {
		struct arg_info *arg_info = &call_info->args[i];
		MonoInst *inst = cfg->args[i];
		int offset = 0;

		/* The argument ends up into a register... */
		if (inst->opcode == OP_REGVAR) {
			switch (arg_info->storage) {
			/* ... and was already into a register. */
			case into_register:
				switch (arg_info->type) {
				case integer32:
					if (inst->dreg != arg_info->reg)
						sh4_mov(&buffer, arg_info->reg, inst->dreg);
					break;

				case float32:
				case float64:
					if (inst->dreg != arg_info->reg) {
						sh4_fmov(&buffer, arg_info->reg, inst->dreg);
						sh4_fmov(&buffer, arg_info->reg + 1, inst->dreg + 1);
					}
					break;

				case integer64:		/* Fall through - handled as 2 x integer32 */
				case valuetype:		/* Fall through - always on the stack currently */
				case typedbyref:	/* Fall through - always on the stack currently */
				case none:
				default:
					g_assert_not_reached();
					break;
				}
				break;

			/* ... but was onto the stack. */
			case onto_stack:
				offset = saved_regs_size + arg_info->offset;
				switch (arg_info->type) {
				case integer32:
					sh4_base_load(NULL, &buffer, offset, sh4_sp, inst->dreg);
					break;

				case float32:
					sh4_base_loadf32(NULL, &buffer, offset, sh4_sp, inst->dreg);
					break;

				case float64:
					sh4_base_loadf64(NULL, &buffer, offset, sh4_sp, inst->dreg);
					break;

				case integer64:		/* Fall through - handled as 2 x integer32 */
				case typedbyref:	/* Fall through - always on the stack currently */
				case valuetype:		/* Fall through - always on the stack currently */
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
					g_assert(inst->inst_basereg == sh4_fp);
					sh4_base_store(NULL, &buffer, arg_info->reg, inst->inst_offset, sh4_fp);
					break;

				case integer64:
					sh4_base_store(NULL, &buffer, arg_info->reg, inst->inst_offset, sh4_fp);
					sh4_base_store(NULL, &buffer, arg_info->reg + 1, inst->inst_offset + 4, sh4_fp);
					break;

				case float64:
					sh4_base_storef64(NULL, &buffer, arg_info->reg, inst->inst_offset, sh4_fp);
					break;

				case float32:
					sh4_base_storef32(NULL, &buffer, arg_info->reg, inst->inst_offset, sh4_fp);
					break;

				case typedbyref:	/* Always onto the stack currently */
				case valuetype:		/* Always onto the stack currently */
				case none:
				default:
					g_assert_not_reached();
					break;
				}
				break;

			/* ... and was already onto the stack. */
			case onto_stack:
				offset = saved_regs_size + arg_info->offset;

				/* In the following sequences of code, we need a temporary
				 * register. We can't use r3 (aka sh4_temp) since it
				 * might be used in sequences generated by sh4_base_load
				 * and sh4_base_store. We've decided to use sh4_r0.
				 * Since we're in the prolog there's no danger to break
				 * a live range: register allocation hasn't been activated yet.
				 */
				switch (arg_info->type) {
				case integer32:
				case float32:
					sh4_base_load(NULL, &buffer, offset, sh4_sp, sh4_r0);
					sh4_base_store(NULL, &buffer, sh4_r0, inst->inst_offset, sh4_fp);
					break;

				case integer64:
				case float64:
					/* TODO - CV: optimize this case. */
					sh4_base_load(NULL, &buffer, offset, sh4_sp, sh4_r0);
					sh4_base_store(NULL, &buffer, sh4_r0, inst->inst_offset, sh4_fp);
					sh4_base_load(NULL, &buffer, offset + 4, sh4_sp, sh4_r0);
					sh4_base_store(NULL, &buffer, sh4_r0, inst->inst_offset + 4, sh4_fp);
					break;

				case valuetype:
				case typedbyref:
					/* This sequence of code is quite ugly and we should avoid to
					 * generate it. We can think of different ways to get round
					 * the problem but for the time being we heading towards the
					 * the completion of the VM, not its optimization!*/
					size_struct = ALIGN_TO(arg_info->size, 0x4);
					for(j=0;j<size_struct;j+=4) {
						sh4_base_load(NULL,&buffer, offset + j, sh4_sp,sh4_r0);
						sh4_base_store(NULL, &buffer, sh4_r0, inst->inst_offset + j, sh4_fp);
					}
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

	/* Set the stack pointer. */
	if (cfg->stack_offset != 0)
		sh4_mov(&buffer, sh4_fp, sh4_sp);

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

	/* Save the LMF just after "regular" local variables, in fact
	   this structure is "hidden" in the local stack frame. */
	if (cfg->method->save_lmf != 0) {
		guint8 *patch0 = NULL;
		guint8 *patch1 = NULL;
		guint8 *patch2 = NULL;
		guint8 *patch3 = NULL;

		/* About used registers : sh4_r0 is a local register and is
		   declared as clobbered into mono_arch_call_opcode() if
		   "save_lmf" is set. */

		/* Allocate the struct MonoLMF onto the stack. */
		sh4_multi_add_imm(&buffer, -sizeof(MonoLMF), sh4_sp);

		/* pseudo-code: MonoLMF.registers[] = { %R0, ..., %R15 }; */
		sh4_mov(&buffer, sh4_sp, sh4_temp);
		sh4_add_imm(&buffer, offsetof(MonoLMF, registers), sh4_temp);
		for (i = 0; i < MONO_MAX_IREGS; i++)
			sh4_movl_dispRx(&buffer, (SH4IntRegister)i, i * 4, sh4_temp);

		/* pseudo-code: MonoLMF.fregisters[] = { %FR0, ..., %FR15 }; */
		sh4_multi_add_imm(&buffer, -offsetof(MonoLMF, registers) +
					   offsetof(MonoLMF, fregisters) +
					   MONO_MAX_FREGS * sizeof(guint32), sh4_temp);
		for (i = MONO_MAX_FREGS - 1; i >= 0; i--)
			sh4_fmov_decRx(&buffer, (SH4FloatRegister)i, sh4_temp);

		/* Patch slot for : sh4_r0 <- cfg->method */
		patch0 = buffer;
		sh4_die(&buffer);

		/* pseudo-code: MonoLMF.method = cfg->method; */
		sh4_movl_dispRx(&buffer, sh4_r0, offsetof(MonoLMF, method), sh4_sp);

		/* Patch slot for : sh4_r0 <- %PC */
		patch1 = buffer;
		sh4_die(&buffer);

		/* pseudo-code: MonoLMF.pc = %PC; */
		sh4_movl_dispRx(&buffer, sh4_r0, offsetof(MonoLMF, pc), sh4_sp);

		/* Patch slot for : sh4_r0 <- mono_get_lmf_addr */
		patch2 = buffer;
		sh4_die(&buffer);

		/* pseudo-code: MonoLMF.lmf_addr = mono_get_lmf_addr(); */
		sh4_jsr_indRx(&buffer, sh4_r0);
		sh4_nop(&buffer);

		sh4_movl_dispRx(&buffer, sh4_r0, offsetof(MonoLMF, lmf_addr), sh4_sp);

		/*
		 * Insert the new LMF at the beginning of the LMF list.
		 */

		/* pseudo-code: MonoLMF.previous_lmf = *(MonoLMF.lmf_addr); */
		sh4_movl_indRy(&buffer, sh4_r0, sh4_temp);
		sh4_movl_dispRx(&buffer, sh4_temp, offsetof(MonoLMF, previous_lmf), sh4_sp);

		/* pseudo-code: *(MonoLMF.lmf_addr) = &MonoLMF;  */
		sh4_movl_indRx(&buffer, sh4_sp, sh4_r0);

		/* Patch slot for : bra_label "skip_cstpool" */
		patch3 = buffer;
		sh4_die(&buffer);
		sh4_nop(&buffer);

		/* Align the constant pool. */
		while (((guint32)buffer % 4) != 0)
			sh4_nop(&buffer);

		/* Build the constant pool & patch the corresponding instructions. */
		sh4_movl_PCrel(&patch0, buffer, sh4_r0);
		sh4_emit32(&buffer, (guint32)cfg->method);

		sh4_movl_PCrel(&patch1, buffer, sh4_r0);
		patch1 -= (guint32)cfg->native_code;
		mono_add_patch_info(cfg, buffer - cfg->native_code, MONO_PATCH_INFO_IP, patch1);
		sh4_emit32(&buffer, (guint32)0);

		sh4_movl_PCrel(&patch2, buffer, sh4_r0);
		sh4_emit32(&buffer, (guint32)mono_get_lmf_addr);

		sh4_bra_label(&patch3, buffer);

		/* At this point, the stack looks like :
		 *	:              :
		 *	|--------------| Caller's frame.
		 *	|  parameters  |
		 *	|==============|
		 *	|  saved reg.  | Callee's frame.
		 *	|--------------|
		 *	|  local var.  |
		 *	|--------------| <- FP
		 *	|   MonoLMF    |
		 *	|--------------| <- SP
		 *	:              :
		 */
	}

	cfg->code_len = buffer - cfg->native_code;

	/* Sanity checks. */
	g_assert(cfg->code_len < cfg->code_size);

	g_free(call_info->args);
	g_free(call_info);

	return buffer;
}

guint8 *get_code_buffer(MonoCompile *cfg, guint32 size)
{
	if (cfg->code_len + size <= cfg->code_size)
		return cfg->native_code + cfg->code_len;

        while (cfg->code_len + size > cfg->code_size) {
		cfg->code_size *= 2U;
	}

	cfg->native_code = g_realloc(cfg->native_code, cfg->code_size);
	mono_jit_stats.code_reallocs++;
	return cfg->native_code + cfg->code_len;
}

void mono_arch_emit_epilog(MonoCompile *cfg)
{
	guint8 *buffer = NULL;
	guint8 *code   = NULL;
	int i = 0;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

#define EPILOGUE_SIZE 48U

	/* Reallocate enough room to store the SH4 instructions
	   used to implement an epilogue. */
	code = buffer = get_code_buffer(cfg, EPILOGUE_SIZE);

	/* Restore the previous LMF & free the space used by the local one. */
	if (cfg->method->save_lmf != 0) {
		/* Adjust SP to point to the "hidden" LMF. */
		sh4_mov(&buffer, sh4_fp, sh4_sp);
		sh4_multi_add_imm(&buffer, -sizeof(MonoLMF), sh4_sp);
		sh4_mov(&buffer, sh4_sp, sh4_temp);

		/* At this point, the stack looks like :
		 *	:              :
		 *	|--------------| Caller's frame.
		 *	|  parameters  |
		 *	|==============|
		 *	|  saved reg.  | Callee's frame.
		 *	|--------------|
		 *	|  local var.  |
		 *	|--------------| <- FP
		 *	|   MonoLMF    |
		 *	|--------------| <- SP, temp
		 *	:              :
		 */

		/* pseudo-code: *(MonoLMF.lmf_addr) = MonoLMF.previous_lmf; */
		sh4_movl_dispRy(&buffer, offsetof(MonoLMF, previous_lmf), sh4_temp, sh4_temp);
		sh4_movl_dispRy(&buffer, offsetof(MonoLMF, lmf_addr), sh4_sp, sh4_sp);
		sh4_movl_indRx(&buffer, sh4_temp, sh4_sp);
	}

	/* Reset the stack pointer. */
	sh4_mov(&buffer, sh4_fp, sh4_sp);

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
	if (cfg->stack_offset != 0) {
		if (SH4_CHECK_RANGE_add_imm(cfg->stack_offset)) {
			sh4_add_imm(&buffer, cfg->stack_offset, sh4_sp);
		}
		else if (SH4_CHECK_RANGE_mov_imm(cfg->stack_offset)) {
			sh4_mov_imm(&buffer, cfg->stack_offset, sh4_temp);
			sh4_add(&buffer, sh4_temp, sh4_sp);
		}
		else {
			sh4_load(&buffer, cfg->stack_offset, sh4_temp);
			sh4_add(&buffer, sh4_temp, sh4_sp);
		}
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
	sh4_ldsl_incRx_PR(&buffer, sh4_sp);

#if 0   /* Mono does not support yet global floating-point register allocation. */
	/* Restore global floating-point registers. */
	for (i = MONO_MAX_FREGS - 1; i >= 0; i--)
		if ((MONO_ARCH_CALLEE_SAVED_FREGS & (1 << i)) != 0 &&
		    (cfg->used_int_regs           & (1 << i)) != 0)
			sh4_fmovl_incRy(&buffer, sh4_sp, (SH4FloatRegister)i);
#endif

	/* Restore global registers. */
	for (i = MONO_MAX_IREGS - 1; i >= 0; i--)
		if ((MONO_ARCH_CALLEE_SAVED_REGS & (1 << i)) != 0 &&
		    (cfg->used_int_regs          & (1 << i)) != 0)
			sh4_movl_incRy(&buffer, sh4_sp, (SH4IntRegister)i);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|--------------| Caller's frame.
	 *	|  parameters  |
	 *	|==============| <- SP
	 *	:              : Callee's frame.
	 */

	/* At this point, the stack is fully restored (as caller's point of view). */

	sh4_rts(&buffer);
	sh4_nop(&buffer);

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
	code = buffer = get_code_buffer(cfg, exceptions_size);

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
				...
				goto end_loop;
			}
		}
#endif

		/* Patch the constant used to jump to this exception. */
		mono_add_patch_info(cfg, patch_info->ip.i, MONO_PATCH_INFO_IP, (guint8 *)(buffer - cfg->native_code));

		/* Pass parameters to the exception handler:
		      1. type token
		      2. offset between the "throw" site and the current PC. */

		/* Patch slot for : sh4_r4 <- type token */
		patch0 = buffer;
		sh4_die(&buffer);

		/* Patch slot for : sh4_r5 <- current PC - throw PC */
		patch1 = buffer;
		sh4_die(&buffer);

		/* Patch slot for : sh4_temp <- mono_arch_throw_corlib_exception */
		patch2 = buffer;
		sh4_die(&buffer);

		sh4_jsr_indRx(&buffer, sh4_temp);
		sh4_nop(&buffer); /* delay slot */

		/* Should never return. */
		sh4_die(&buffer);

		/* Align the constant pool. */
		while (((guint32)buffer % 4) != 0)
			sh4_nop(&buffer);

		/* Build the constant pool & patch the corresponding instructions. */
		sh4_movl_PCrel(&patch0, buffer, MONO_SH4_REG_FIRST_ARG);
		sh4_emit32(&buffer, (guint32)class->type_token);

		sh4_movl_PCrel(&patch1, buffer, MONO_SH4_REG_FIRST_ARG + 1);
		sh4_emit32(&buffer, (guint32)((buffer - cfg->native_code) - patch_info->ip.i));

		sh4_movl_PCrel(&patch2, buffer, sh4_temp);
		/* Reuse this patch_info to set the jump in mono_arch_patch_code(). */
		patch_info->type = MONO_PATCH_INFO_INTERNAL_METHOD;
		patch_info->data.name = "mono_arch_throw_corlib_exception";
		patch_info->ip.i = buffer - cfg->native_code;
		sh4_emit32(&buffer, (guint32)0);
	}

	cfg->code_len = buffer - cfg->native_code;

	/* Sanity checks. */
	g_assert(buffer - code <= exceptions_size);
	g_assert(cfg->code_len <= cfg->code_size);

	/* Free constant pools. It's safer to do it there
	 * than in function mono_arch_emit_epilog(). */
	sh4_cstpool_end(cfg);

	/* mono_arch_flush_icache() is called into the caller mini.c:mono_codegen(). */

	return;
}

void mono_arch_flush_icache(guint8 *code, gint size)
{
	SH4_EXTRA_DEBUG("args => %p, %d", code, size);
	syscall(__NR_cacheflush, code, size, CACHEFLUSH_D_WB | CACHEFLUSH_I);
	return;
}

void mono_arch_flush_register_windows(void)
{
	/* Not used. */
	return;
}

void mono_arch_free_jit_tls_data(MonoJitTlsData *tls)
{
	return;
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
		MonoInst *ins = cfg->varinfo[i];
		MonoMethodVar *var = MONO_VARINFO(cfg, i);

		/* Skip unused variables. */
		if (var->range.first_use.abs_pos >= var->range.last_use.abs_pos ||
		    (ins->flags & (MONO_INST_IS_DEAD | MONO_INST_VOLATILE | MONO_INST_INDIRECT)) ||
		    (ins->opcode != OP_LOCAL && ins->opcode != OP_ARG))
			continue;

		/* Allocate 32 bit variables only. */
		if (mono_is_regsize_var(ins->inst_vtype)) {
			g_assert(MONO_VARINFO(cfg, i)->reg == -1);
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
 * The registers sh4_fp and sh4_sp are respectively used as the
 * frame pointer and as the stack pointer. Maybe sh4_r12 will be
 * used one day as the global pointer.
 */
GList *mono_arch_get_global_int_regs(MonoCompile *cfg)
{
	GList *regs = NULL;
	int i = 0;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	for (i = 0; i < MONO_MAX_IREGS; i++)
		if (i != sh4_fp && /* Do not use the frame pointer... */
		    i != sh4_sp && /* ... nor the stack pointer ;) */
		    (MONO_ARCH_CALLEE_SAVED_REGS & (1 << i)) != 0)
			regs = g_list_prepend(regs, (gpointer)i);

	return regs;
}

/**
 * Check for opcodes we can handle directly in hardware, but also emits the instructions.
 */
MonoInst *mono_arch_emit_inst_for_method(MonoCompile *cfg, MonoMethod *method,
					 MonoMethodSignature *signature, MonoInst **args)
{
	return NULL;
}

MonoInst *mono_arch_get_thread_intrinsic(MonoCompile* cfg)
{
	/* TODO - CV */
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
 *   // Load the VTable
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
 *           add   rY, rW
 *           mov.l @rW, rX
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
	 *         add   rY, rW
	 *         mov.l @rW, rX
	 */
	sh4_rW = get_Ry_sh4_movl_indRy(code16[-3]);
	if (is_sh4_movl_indRy(code16[-3], sh4_rW, sh4_rX)) {
		sh4_rY = get_Ry_sh4_add(code16[-4]);
		if (!is_sh4_add(code16[-4], sh4_rY, sh4_rW))
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
	/* Trick used in output_basic_block(). */
	next_inst->backend.data = NULL;

	switch (next_inst->opcode) {
	case OP_CEQ:
	case OP_ICEQ:
		inst->opcode = OP_SH4_CMPEQ;
		next_inst->opcode = OP_SH4_MOVT;
		break;

	case OP_CGT:
	case OP_ICGT:
		inst->opcode = OP_SH4_CMPGT;
		next_inst->opcode = OP_SH4_MOVT;
		break;

	case OP_CGT_UN:
	case OP_ICGT_UN:
		inst->opcode = OP_SH4_CMPHI;
		next_inst->opcode = OP_SH4_MOVT;
		break;

	case OP_CLT:
	case OP_ICLT:
		inst->opcode = OP_SH4_NOT_CMPGE;
		next_inst->opcode = OP_SH4_MOVT;
		break;

	case OP_CLT_UN:
	case OP_ICLT_UN:
		inst->opcode = OP_SH4_NOT_CMPHS;
		next_inst->opcode = OP_SH4_MOVT;
		break;

	case OP_COND_EXC_IEQ:
	case OP_COND_EXC_EQ:
		next_inst->backend.data = (gpointer)-1;
	case CEE_BEQ:
	case OP_IBEQ:
		inst->opcode = OP_SH4_CMPEQ;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_COND_EXC_INE_UN:
	case OP_COND_EXC_NE_UN:
		next_inst->backend.data = (gpointer)-1;
	case CEE_BNE_UN:
	case OP_IBNE_UN:
		inst->opcode = OP_SH4_CMPEQ;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_COND_EXC_IGT:
	case OP_COND_EXC_GT:
		next_inst->backend.data = (gpointer)-1;
	case OP_IBGT:
		inst->opcode = OP_SH4_CMPGT;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_COND_EXC_IGT_UN:
	case OP_COND_EXC_GT_UN:
		next_inst->backend.data = (gpointer)-1;
	case OP_IBGT_UN:
		inst->opcode = OP_SH4_CMPHI;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_COND_EXC_ILE_UN:
	case OP_COND_EXC_LE_UN:
		next_inst->backend.data = (gpointer)-1;
	case OP_IBLE_UN:
		inst->opcode = OP_SH4_CMPHI;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_COND_EXC_ILE:
	case OP_COND_EXC_LE:
		next_inst->backend.data = (gpointer)-1;
	case OP_IBLE:
		inst->opcode = OP_SH4_CMPGT;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_COND_EXC_IGE:
	case OP_COND_EXC_GE:
		next_inst->backend.data = (gpointer)-1;
	case OP_IBGE:
		inst->opcode = OP_SH4_CMPGE;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_COND_EXC_IGE_UN:
	case OP_COND_EXC_GE_UN:
		next_inst->backend.data = (gpointer)-1;
	case CEE_BGE_UN:
	case OP_IBGE_UN:
		inst->opcode = OP_SH4_CMPHS;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_COND_EXC_ILT:
	case OP_COND_EXC_LT:
		next_inst->backend.data = (gpointer)-1;
	case OP_IBLT:
		inst->opcode = OP_SH4_CMPGE;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_COND_EXC_ILT_UN:
	case OP_COND_EXC_LT_UN:
		next_inst->backend.data = (gpointer)-1;
	case OP_IBLT_UN:
		inst->opcode = OP_SH4_CMPHS;
		next_inst->opcode = OP_SH4_BF;
		break;

	default:
		/* The conditional branch was removed due to [some] dead-code
		   elimination, so we can replace safely this comparison with a nop. */
		inst->opcode = OP_NOP;
	}
}

static inline void convert_fcomparison_to_sh4(MonoCompile *cfg, MonoBasicBlock *basic_block, MonoInst *inst, MonoInst *next_inst)
{
	MonoInst *new_inst = NULL;
	MonoInst *new_inst2 = NULL;
	MonoInst *new_inst3 = NULL;
	MonoInst *new_inst4 = NULL;
	gint32 tmp_reg;

	/* Trick used in output_basic_block(). */
	next_inst->backend.data = NULL;

	switch (next_inst->opcode) {
	case OP_FBEQ:
		/* (A FBEQ B) <=> (A SH4_CMPEQ B)*/
		inst->opcode = OP_SH4_FCMPEQ;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_FBGE:
		/* (A FBGE B) <=> ((A SH4_CMPEQ B) || (A SH4_CMPGT B)) */
		inst->opcode = OP_SH4_FCMPEQ;
		next_inst->opcode = OP_SH4_BT;

		MONO_INST_NEW(cfg, new_inst, OP_SH4_FCMPGT);
		new_inst->sreg1 = inst->sreg1;
		new_inst->sreg2 = inst->sreg2;
		mono_bblock_insert_after_ins(basic_block, next_inst, new_inst);

		MONO_INST_NEW(cfg, new_inst2, OP_SH4_BT);
		memcpy(new_inst2, next_inst, sizeof(MonoInst));
		mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);
		break;

	case OP_FBGE_UN:
		/* (A FBGE_UN B) <=> !(B SH4_CMPGT A) */
		inst->opcode = OP_SH4_FCMPGT;
		tmp_reg = inst->sreg1;
		inst->sreg1 = inst->sreg2;
		inst->sreg2 = tmp_reg;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_FBGT:
		/* (A FBGT B) <=> (A SH4_CMPGT B) */
		inst->opcode = OP_SH4_FCMPGT;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_FBGT_UN:
		/* (A FBGT_UN B) <=> !((A SH4_CMPEQ B) || (B SH4_CMPGT A))
		                 <=> (!(A SH4_CMPEQ B) && !(B SH4_CMPGT A)) */
		inst->opcode = OP_SH4_FCMPEQ;

		MONO_INST_NEW(cfg, new_inst, OP_SH4_MOVT);
		new_inst->dreg = mono_alloc_ireg(cfg);
		mono_bblock_insert_after_ins(basic_block, inst, new_inst);

		MONO_INST_NEW(cfg, new_inst2, OP_SH4_FCMPGT);
		new_inst2->sreg1 = inst->sreg2;
		new_inst2->sreg2 = inst->sreg1;
		mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);

		MONO_INST_NEW(cfg, new_inst3, OP_SH4_ROTCL);
		new_inst3->sreg1 = new_inst->dreg;
		new_inst3->dreg  = new_inst->dreg;
		mono_bblock_insert_after_ins(basic_block, new_inst2, new_inst3);

		MONO_INST_NEW(cfg, new_inst4, OP_SH4_TST);
		new_inst4->sreg1 = new_inst->dreg;
		new_inst4->sreg2 = new_inst->dreg;
		mono_bblock_insert_after_ins(basic_block, new_inst3, new_inst4);

		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_FBLE:
		/* (A FBLE B) <=> ((A SH4_CMPEQ B) || (B SH4_CMPGT A)) */
		inst->opcode = OP_SH4_FCMPEQ;
		next_inst->opcode = OP_SH4_BT;

		MONO_INST_NEW(cfg, new_inst, OP_SH4_FCMPGT);
		new_inst->sreg1 = inst->sreg2;
		new_inst->sreg2 = inst->sreg1;
		mono_bblock_insert_after_ins(basic_block, next_inst, new_inst);

		MONO_INST_NEW(cfg, new_inst2, OP_SH4_BT);
		memcpy(new_inst2, next_inst, sizeof(MonoInst));
		mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);
		break;

	case OP_FBLE_UN:
		/* (A FBLE_UN B) <=> !(A SH4_CMPGT B) */
		inst->opcode = OP_SH4_FCMPGT;
		next_inst->opcode = OP_SH4_BF;
		break;

	case OP_FBLT:
		/* (A FBLT B) <=> (B SH4_CMPGT A) */
		inst->opcode = OP_SH4_FCMPGT;
		tmp_reg = inst->sreg1;
		inst->sreg1 = inst->sreg2;
		inst->sreg2 = tmp_reg;
		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_FBLT_UN:
		/* (A FBLT B) <=> !((A SH4_CMPEQ B) || (A SH4_CMPGT B))
		              <=> (!(A SH4_CMPEQ B) && !(A SH4_CMPGT B)) */
		inst->opcode = OP_SH4_FCMPEQ;

		MONO_INST_NEW(cfg, new_inst, OP_SH4_MOVT);
		new_inst->dreg = mono_alloc_ireg(cfg);
		mono_bblock_insert_after_ins(basic_block, inst, new_inst);

		MONO_INST_NEW(cfg, new_inst2, OP_SH4_FCMPGT);
		new_inst2->sreg1 = inst->sreg1;
		new_inst2->sreg2 = inst->sreg2;
		mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);

		MONO_INST_NEW(cfg, new_inst3, OP_SH4_ROTCL);
		new_inst3->sreg1 = new_inst->dreg;
		new_inst3->dreg  = new_inst->dreg;
		mono_bblock_insert_after_ins(basic_block, new_inst2, new_inst3);

		MONO_INST_NEW(cfg, new_inst4, OP_SH4_TST);
		new_inst4->sreg1 = new_inst->dreg;
		new_inst4->sreg2 = new_inst->dreg;
		mono_bblock_insert_after_ins(basic_block, new_inst3, new_inst4);

		next_inst->opcode = OP_SH4_BT;
		break;

	case OP_FBNE_UN:
		/* (A FBNE_UN B) <=> !(A SH4_CMPEQ B) */
		inst->opcode = OP_SH4_FCMPEQ;
		next_inst->opcode = OP_SH4_BF;
		break;

	default:
		fprintf(stderr, "unimplemented (yet) next_inst->opcode %s (0x%x) in %s()\n",
			mono_inst_name(next_inst->opcode), next_inst->opcode, __FUNCTION__);
		g_assert_not_reached();
		break;
	}
}
static inline void decompose_op_offset2base(MonoCompile *cfg, MonoBasicBlock *basic_block, MonoInst *inst, int is_store)
{
	MonoInst *new_inst  = NULL;
	MonoInst *new_inst2 = NULL;

	/* Load the offset in new register. */
	MONO_INST_NEW(cfg, new_inst, OP_ICONST);
	new_inst->inst_c0 = inst->inst_offset;
	new_inst->dreg = mono_alloc_ireg(cfg);

	/* Add the base register to this new register,
	   remember dest == sreg1 for OP_IADD on SH4. */
	MONO_INST_NEW(cfg, new_inst2, OP_IADD);
	new_inst2->dreg = new_inst2->sreg1 = new_inst->dreg;
	if (is_store != 0)
		new_inst2->sreg2 = inst->inst_destbasereg;
	else
		new_inst2->sreg2 = inst->inst_basereg;

	mono_bblock_insert_before_ins(basic_block, inst, new_inst2);
	mono_bblock_insert_before_ins(basic_block, new_inst2, new_inst);

	/* Adjust the original opcode. */
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
	new_inst->dreg = mono_alloc_ireg(cfg);
	mono_bblock_insert_before_ins(basic_block, inst, new_inst);
	inst->sreg1 = new_inst->dreg;
}

static void mono_sh4_decompose_localloc(MonoCompile *cfg, MonoBasicBlock *basic_block, MonoInst *inst,
					SH4IntRegister dreg, SH4IntRegister size_reg)
{
	MonoInst *new_inst = NULL;

	/* Substract the requested & aligned size to the SP. */
	inst->opcode = OP_ISUB;
	inst->sreg2  = size_reg;
	inst->sreg1  = sh4_sp;
	inst->dreg   = sh4_sp;

	if ((inst->flags & MONO_INST_INIT) != 0) {
		MONO_INST_NEW(cfg, new_inst, OP_SH4_BZERO4);
		new_inst->inst_basereg  = sh4_sp;
		new_inst->inst_indexreg = size_reg;
		new_inst->dreg          = dreg;
	}
	else {
		MONO_INST_NEW(cfg, new_inst, OP_MOVE);
		new_inst->sreg1 = sh4_sp;
		new_inst->dreg  = dreg;
	}
	mono_bblock_insert_after_ins(basic_block, inst, new_inst);
}

static SH4IntRegister mono_sh4_decompose_align(MonoCompile *cfg, MonoBasicBlock *basic_block, MonoInst *inst,
					       SH4IntRegister reg, int alignement)
{
	MonoInst *new_inst  = NULL;
	MonoInst *new_inst2 = NULL;
	MonoInst *new_inst3 = NULL;
	MonoInst *new_inst4 = NULL;
	MonoInst *new_inst5 = NULL;

	MONO_INST_NEW(cfg, new_inst, OP_MOVE);
	new_inst->sreg1 = reg;
	new_inst->dreg  = mono_alloc_ireg(cfg);
	mono_bblock_insert_before_ins(basic_block, inst, new_inst);

	MONO_INST_NEW(cfg, new_inst2, OP_ICONST);
	new_inst2->inst_c0 = (alignement - 1);
	new_inst2->dreg    = mono_alloc_ireg(cfg);
	mono_bblock_insert_before_ins(basic_block, inst, new_inst2);

	MONO_INST_NEW(cfg, new_inst3, OP_IADD);
	new_inst3->sreg1 = new_inst->dreg;
	new_inst3->sreg2 = new_inst2->dreg;
	new_inst3->dreg  = new_inst->dreg;
	mono_bblock_insert_before_ins(basic_block, inst, new_inst3);

	MONO_INST_NEW(cfg, new_inst4, OP_ICONST);
	new_inst4->inst_c0 = ~(alignement - 1);
	new_inst4->dreg    = mono_alloc_ireg(cfg);
	mono_bblock_insert_before_ins(basic_block, inst, new_inst4);

	MONO_INST_NEW(cfg, new_inst5, OP_IAND);
	new_inst5->sreg1 = new_inst->dreg;
	new_inst5->sreg2 = new_inst4->dreg;
	new_inst5->dreg  = new_inst->dreg;
	mono_bblock_insert_before_ins(basic_block, inst, new_inst5);

	return new_inst->dreg;
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

	if (cfg->verbose_level >= 3) {
		printf("BEFORE LOWERING BLOCK %d:\n", basic_block->block_num);
		MONO_BB_FOR_EACH_INS(basic_block, inst)
			mono_print_ins(inst);
	}

#define register_not_assigned(reg) ((reg) < 0 || (reg) >= MONO_MAX_IREGS)

	MONO_BB_FOR_EACH_INS_SAFE(basic_block, next_inst, inst) {
		MonoInst *new_inst  = NULL;
		MonoInst *new_inst2 = NULL;
		MonoInst *new_inst3 = NULL;
		MonoInst *new_inst4 = NULL;
		MonoInst *new_inst5 = NULL;
		MonoInst *new_inst6 = NULL;
		MonoInst *new_inst7 = NULL;
		gint32 tmp_reg;

		switch (inst->opcode) {
		case OP_COMPARE:
		case OP_ICOMPARE:
			next_inst = inst->next;
			g_assert(next_inst != NULL);
			convert_comparison_to_sh4(inst, next_inst);
			break;

		case OP_FCOMPARE:
			next_inst = inst->next;
			g_assert(next_inst != NULL);
			convert_fcomparison_to_sh4(cfg, basic_block, inst, next_inst);
			break;

		case OP_FCEQ:
			inst->opcode = OP_SH4_FCMPEQ;
			MONO_INST_NEW(cfg, new_inst, OP_SH4_MOVT);
			new_inst->dreg = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, inst, new_inst);
			break;

		case OP_FCGT:
			inst->opcode = OP_SH4_FCMPGT;
			MONO_INST_NEW(cfg, new_inst, OP_SH4_MOVT);
			new_inst->dreg = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, inst, new_inst);
			break;

		case OP_FCGT_UN:
			inst->opcode = OP_SH4_FCMPEQ;
			MONO_INST_NEW(cfg, new_inst, OP_SH4_MOVT);
			new_inst->dreg = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, inst, new_inst);

			MONO_INST_NEW(cfg, new_inst2, OP_SH4_FCMPGT);
			new_inst2->sreg1 = inst->sreg2;
			new_inst2->sreg2 = inst->sreg1;
			mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);

			MONO_INST_NEW(cfg, new_inst3, OP_SH4_ROTCL);
			new_inst3->sreg1 = inst->dreg;
			new_inst3->dreg  = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst2, new_inst3);

			MONO_INST_NEW(cfg, new_inst4, OP_SH4_TST);
			new_inst4->sreg1 = inst->dreg;
			new_inst4->sreg2 = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst3, new_inst4);

			MONO_INST_NEW(cfg, new_inst5, OP_SH4_MOVT);
			new_inst5->dreg = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst4, new_inst5);
			break;

		case OP_FCLT:
			inst->opcode = OP_SH4_FCMPGT;
			tmp_reg = inst->sreg1;
			inst->sreg1 = inst->sreg2;
			inst->sreg2 = tmp_reg;
			MONO_INST_NEW(cfg, new_inst, OP_SH4_MOVT);
			new_inst->dreg = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, inst, new_inst);
			break;

		case OP_FCLT_UN:
			inst->opcode = OP_SH4_FCMPEQ;
			MONO_INST_NEW(cfg, new_inst, OP_SH4_MOVT);
			new_inst->dreg = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, inst, new_inst);

			MONO_INST_NEW(cfg, new_inst2, OP_SH4_FCMPGT);
			new_inst2->sreg1 = inst->sreg1;
			new_inst2->sreg2 = inst->sreg2;
			mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);

			MONO_INST_NEW(cfg, new_inst3, OP_SH4_ROTCL);
			new_inst3->sreg1 = inst->dreg;
			new_inst3->dreg  = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst2, new_inst3);

			MONO_INST_NEW(cfg, new_inst4, OP_SH4_TST);
			new_inst4->sreg1 = inst->dreg;
			new_inst4->sreg2 = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst3, new_inst4);

			MONO_INST_NEW(cfg, new_inst5, OP_SH4_MOVT);
			new_inst5->dreg = inst->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst4, new_inst5);
			break;

		case OP_COMPARE_IMM:
		case OP_ICOMPARE_IMM:
			next_inst = inst->next;
			g_assert(next_inst != NULL);

			convert_comparison_to_sh4(inst, next_inst);

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
				new_inst->dreg = mono_alloc_ireg(cfg);
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

		case OP_ADDCC_IMM:	/* Fall through */
		case OP_ADC_IMM:	/* Fall through */
		case OP_SUBCC_IMM:	/* Fall through */
		case OP_ISBB_IMM:	/* Fall through */
		case OP_SBB_IMM:
			mono_decompose_op_imm(cfg, basic_block, inst);
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
			/* mono_decompose_op_imm() knows OP_ISHL_IMM. */
			inst->opcode = OP_ISHL_IMM;
		case OP_ISHL_IMM:
			switch (inst->inst_imm) {
			case 0:
				inst->opcode = OP_MOVE;
				break;
			case 1:
				inst->opcode = OP_SH4_SHLL1;
				break;
			case 2:
				inst->opcode = OP_SH4_SHLL2;
				break;
			case 8:
				inst->opcode = OP_SH4_SHLL8;
				break;
			case 16:
				inst->opcode = OP_SH4_SHLL16;
				break;
			default:
				mono_decompose_op_imm(cfg, basic_block, inst);
				break;
			}
			break;

		case OP_SHR_UN_IMM:
			/* mono_decompose_op_imm() knows OP_ISHR_UN_IMM. */
			inst->opcode = OP_ISHR_UN_IMM;
		case OP_ISHR_UN_IMM:
			switch (inst->inst_imm) {
			case 0:
				inst->opcode = OP_MOVE;
				break;
			case 1:
				inst->opcode = OP_SH4_SHLR1;
				break;
			case 2:
				inst->opcode = OP_SH4_SHLR2;
				break;
			case 8:
				inst->opcode = OP_SH4_SHLR8;
				break;
			case 16:
				inst->opcode = OP_SH4_SHLR16;
				break;
			default:
				mono_decompose_op_imm(cfg, basic_block, inst);
				break;
			}
			break;

		case OP_SHR_IMM:
			/* mono_decompose_op_imm() knows OP_ISHR_IMM. */
			inst->opcode = OP_ISHR_IMM;
		case OP_ISHR_IMM:
			switch (inst->inst_imm) {
			case 0:
				inst->opcode = OP_MOVE;
				break;
			case 1:
				inst->opcode = OP_SH4_SHAR1;
				break;
			default:
				mono_decompose_op_imm(cfg, basic_block, inst);
				break;
			}
			break;

		case OP_MUL_IMM:
			/* mono_decompose_op_imm() knows OP_IMUL_IMM. */
			inst->opcode = OP_IMUL_IMM;
		case OP_IMUL_IMM:
			switch (inst->inst_imm) {
			case 0:
				inst->opcode = OP_ICONST;
				inst->inst_c0= 0;
				break;
			case 1:
				inst->opcode = OP_MOVE;
				break;
			case 2:
				inst->opcode = OP_SH4_SHLL1;
				break;
			case 4:
				inst->opcode = OP_SH4_SHLL2;
				break;
			case 256:
				inst->opcode = OP_SH4_SHLL8;
				break;
			case 65536:
				inst->opcode = OP_SH4_SHLL16;
				break;
			default:
				mono_decompose_op_imm(cfg, basic_block, inst);
				break;
			}
			break;

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

		case OP_FCALL_MEMBASE:
		case OP_VCALL_MEMBASE:
		case OP_VCALL2_MEMBASE:
		case OP_VOIDCALL_MEMBASE:
		case OP_LCALL_MEMBASE:
		case OP_CALL_MEMBASE:
			/* The opcodes call*_membase are splitted into
			   load_membase + call*_reg. */
			MONO_INST_NEW(cfg, new_inst, OP_LOAD_MEMBASE);
			mono_bblock_insert_before_ins(basic_block, inst, new_inst);

			/* Link the new opcodes together. */
			new_inst->inst_basereg = inst->inst_basereg;
			new_inst->inst_offset = inst->inst_offset;
			new_inst->dreg = mono_alloc_ireg(cfg);

			inst->inst_offset = 0;
			inst->sreg1 = new_inst->dreg;

			/* Convert call*_membase to call*_reg. */
			/* TODO - CV: replace this ugly code with something like:
			   inst->opcode = call_membase2reg[inst->opcode]; */
			switch (inst->opcode) {
			case OP_VOIDCALL_MEMBASE:
			case OP_VCALL_MEMBASE:
			case OP_VCALL2_MEMBASE:
				inst->opcode = OP_VOIDCALL_REG;
				break;
			case OP_LCALL_MEMBASE:
				inst->opcode = OP_LCALL_REG;
				break;
			case OP_FCALL_MEMBASE:
				inst->opcode = OP_FCALL_REG;
				break;
			default:
				inst->opcode = OP_CALL_REG;
				break;
			}

			/* See comment about OP_SH4_LOAD*. */
			if (SH4_CHECK_RANGE_movl_dispRy(new_inst->inst_offset)) {
				new_inst->opcode = OP_SH4_LOADI4_MEMBASE;
			}
			else {
				decompose_op_offset2base(cfg, basic_block, new_inst, 0);
				new_inst->opcode = OP_SH4_LOADI4;
			}
			break;

		case OP_COND_EXC_IC:
		case OP_COND_EXC_C:
			/* The T-bit is [already] the carry. */
			inst->backend.data = (gpointer)-1;
			inst->opcode = OP_SH4_BT;
			break;

		case OP_SH4_IADD_OVF:
		case OP_SH4_IADC_OVF:
			/*
			 * Implement OVF condition for signed addition:
			 *     overflow = sign(~(x ^ y) & (s ^ y)), with s = x + y
			 * cf. Hacker's Delight, p.28
			 */

			if (inst->opcode == OP_SH4_IADD_OVF)
				inst->opcode = OP_IADD;
			else
				inst->opcode = OP_IADC;

			MONO_INST_NEW(cfg, new_inst, OP_IXOR);
			new_inst->sreg1 = inst->sreg1;
			new_inst->sreg2 = inst->sreg2;
			new_inst->dreg  = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, inst, new_inst);

			MONO_INST_NEW(cfg, new_inst2, OP_INOT);
			new_inst2->sreg1 = new_inst->dreg;
			new_inst2->dreg  = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);

			MONO_INST_NEW(cfg, new_inst3, OP_IXOR);
			new_inst3->sreg1 = inst->dreg;
			new_inst3->sreg2 = inst->sreg2;
			new_inst3->dreg  = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, new_inst2, new_inst3);

			MONO_INST_NEW(cfg, new_inst4, OP_IAND);
			new_inst4->sreg1 = new_inst2->dreg;
			new_inst4->sreg2 = new_inst3->dreg;
			new_inst4->dreg  = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, new_inst3, new_inst4);

			MONO_INST_NEW(cfg, new_inst5, OP_SH4_SHLL1);
			new_inst5->sreg1 = new_inst4->dreg;
			new_inst5->dreg  = new_inst4->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst4, new_inst5);
			break;

		case OP_SH4_ISUB_OVF:
		case OP_SH4_ISBB_OVF:
			/*
			 * Implement OVF condition for signed subtraction:
			 *     overflow = sign((x ^ y) & (d ^ x)), with d = x - y
			 * cf. Hacker's Delight, p.28
			 */

			if (inst->opcode == OP_SH4_ISUB_OVF)
				inst->opcode = OP_ISUB;
			else
				inst->opcode = OP_ISBB;

			MONO_INST_NEW(cfg, new_inst, OP_IXOR);
			new_inst->sreg1 = inst->sreg1;
			new_inst->sreg2 = inst->sreg2;
			new_inst->dreg  = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, inst, new_inst);

			MONO_INST_NEW(cfg, new_inst2, OP_IXOR);
			new_inst2->sreg1 = inst->dreg;
			new_inst2->sreg2 = inst->sreg1;
			new_inst2->dreg  = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);

			MONO_INST_NEW(cfg, new_inst3, OP_IAND);
			new_inst3->sreg1 = new_inst->dreg;
			new_inst3->sreg2 = new_inst2->dreg;
			new_inst3->dreg  = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, new_inst2, new_inst3);

			MONO_INST_NEW(cfg, new_inst4, OP_SH4_SHLL1);
			new_inst4->sreg1 = new_inst3->dreg;
			new_inst4->dreg  = new_inst3->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst3, new_inst4);
			break;

		case OP_LCONV_TO_OVF_I4_2:
			/*
			 * Implement OVF condition for int64 to int32 conversion:
			 * if l = (high,low)
			 *     overflow = (low >> 31) != high
			 */
			inst->opcode = OP_MOVE;

			MONO_INST_NEW(cfg, new_inst, OP_ICONST);
			new_inst->inst_c0 = (-1) << 5;
			new_inst->dreg    = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, inst, new_inst);

			MONO_INST_NEW(cfg, new_inst2, OP_SH4_SHAD);
			new_inst2->sreg2 = new_inst->dreg;
			new_inst2->sreg1 = inst->dreg;
			new_inst2->dreg  = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, new_inst, new_inst2);

			MONO_INST_NEW(cfg, new_inst3, OP_SH4_CMPEQ);
			new_inst3->sreg1 = new_inst2->dreg;
			new_inst3->sreg2 = inst->sreg2;
			mono_bblock_insert_after_ins(basic_block, new_inst2, new_inst3);

			MONO_INST_NEW(cfg, new_inst4, OP_SH4_MOVT);
			new_inst4->dreg = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, new_inst3, new_inst4);

			MONO_INST_NEW(cfg, new_inst5, OP_ICONST);
			new_inst5->inst_c0 = 0;
			new_inst5->dreg    = mono_alloc_ireg(cfg);
			mono_bblock_insert_after_ins(basic_block, new_inst4, new_inst5);

			MONO_INST_NEW(cfg, new_inst6, OP_SH4_CMPEQ);
			new_inst6->sreg1 = new_inst4->dreg;
			new_inst6->sreg2 = new_inst5->dreg;
			mono_bblock_insert_after_ins(basic_block, new_inst5, new_inst6);

			MONO_INST_NEW(cfg, new_inst7, OP_SH4_BT);
			new_inst7->backend.data = (gpointer)-1;
			new_inst7->inst_p1      = (void *)"OverflowException";
			mono_bblock_insert_after_ins(basic_block, new_inst6, new_inst7);
			break;

		case OP_LOCALLOC_IMM:
			inst->inst_imm = ALIGN_TO(inst->inst_imm, 4);
			decompose_op_imm2reg(cfg, basic_block, inst);
			mono_sh4_decompose_localloc(cfg, basic_block, inst, inst->dreg, inst->sreg1);
			break;

		case OP_LOCALLOC:
			tmp_reg = mono_sh4_decompose_align(cfg, basic_block, inst, inst->sreg1, 4);
			mono_sh4_decompose_localloc(cfg, basic_block, inst, inst->dreg, tmp_reg);
			break;

		default:
			break;
		}
	}

	/* Save the state of the virtual-register allocator. */
	basic_block->max_vreg = cfg->next_vreg;

	if (cfg->verbose_level >= 3) {
		printf("AFTER LOWERING BLOCK %d:\n", basic_block->block_num);
		MONO_BB_FOR_EACH_INS(basic_block, inst)
			mono_print_ins(inst);
	}

	return;
}

void mono_arch_decompose_long_opts(MonoCompile *cfg, MonoInst *long_inst)
{
	/* Use of cfg->cbb in mono_decompose_long_opts is tricky and
	 * explains why the macros hereafter work fine.
	 */
	if (long_inst->opcode == OP_LNEG) {
		MonoInst *new_inst;

		MONO_INST_NEW(cfg, new_inst, OP_SH4_CLRT);	/* Reset carry */
		MONO_ADD_INS(cfg->cbb, new_inst);
		MONO_EMIT_NEW_UNALU(cfg, OP_SH4_NEGC, long_inst->dreg + 1, long_inst->sreg1 + 1);
		MONO_EMIT_NEW_UNALU(cfg, OP_SH4_NEGC, long_inst->dreg + 2, long_inst->sreg1 + 2);

		NULLIFY_INS(long_inst);
	}
	return;
}

/* Free the space used by parameters, computed into mono_arch_call_opcode(). */
static inline void free_args_area(MonoCompile *cfg, guint8 **buffer, MonoCallInst *call)
{
	if (SH4_CHECK_RANGE_add_imm(call->stack_usage))
		sh4_add_imm(buffer, call->stack_usage, sh4_sp);
	else {
		/* sh4_temp belongs to clobbered registers during a call,
		   so we can reuse it here. */
		sh4_cstpool_add(cfg, buffer, MONO_PATCH_INFO_NONE, &call->stack_usage, sh4_temp);
		sh4_add(buffer, sh4_temp, sh4_sp);
	}
}

void mono_arch_output_basic_block(MonoCompile *cfg, MonoBasicBlock *basic_block)
{
	MonoInst *inst = NULL;
	guint8 *buffer = NULL;
	guint8 *loop   = NULL;
	MonoCallInst *call = NULL;
	MonoJumpInfoType type;
	gpointer target = NULL;
	MonoInst *spvar = NULL;
	guint8 *address = NULL;
	guint8 *patch = NULL;
	int displace = 0;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p, %p", cfg, basic_block);
	SH4_CFG_DEBUG(4) SH4_DEBUG("method: %s", cfg->method->name);

	mono_debug_open_block(cfg, basic_block, cfg->code_len);

	sh4_cstpool_check_begin_bb(cfg, basic_block, &buffer);

	MONO_BB_FOR_EACH_INS(basic_block, inst) {
		guint32 length_max = 0;
		guint32 size_cstpool;
		gboolean emit_cstpool;
		int length   = 0;
		guint8 *code = NULL;

		/* Get the 'len' field of this opcode, specify into cpu-sh4.md. */
		length_max = *((guint8 *)ins_get_spec(inst->opcode) + MONO_INST_LEN);

		/* Check if the constant pool has to be emitted right now. */
		emit_cstpool = sh4_cstpool_decide_emission(cfg, FALSE, NULL, &size_cstpool);
		if (emit_cstpool)
			length_max += size_cstpool;

		/* Reallocate enough room to store the SH4 instructions (and possibly
		   constant pool) used to implement the current opcode. */
		buffer = get_code_buffer(cfg, length_max);
		if (emit_cstpool)
			sh4_emit_pool(cfg, FALSE, &buffer);
		code = buffer;

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
			sh4_add_imm(&buffer, inst->inst_imm, inst->dreg);
			break;

		case CEE_ADD:
		case OP_IADD:
			/* MD: int_add: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_add(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_ADDCC:
			/* MD: addcc: clob:1 dest:i src1:i src2:i len:4 */
			/* Fall through */
		case OP_IADDCC:
			/* MD: int_addcc: clob:1 dest:i src1:i src2:i len:4 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_clrt(&buffer);
			sh4_addc(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_ADC:
			/* MD: adc: clob:1 dest:i src1:i src2:i len:2 */
			/* Fall through */
		case OP_IADC:
			/* MD: int_adc: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_addc(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_ISUB:
			/* MD: int_sub: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_sub(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_SUBCC:
			/* MD: subcc: clob:1 dest:i src1:i src2:i len:4 */
			/* Fall through */
		case OP_ISUBCC:
			/* MD: int_subcc: clob:1 dest:i src1:i src2:i len:4 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_clrt(&buffer);
			sh4_subc(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_SBB:
			/* MD: sbb: clob:1 dest:i src1:i src2:i len:2 */
			/* Fall through */
		case OP_ISBB:
			/* MD: int_sbb: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_subc(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_SH4_OR_IMM_R0:
			/* MD: sh4_or_imm_R0: clob:1 dest:z src1:z len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			g_assert(inst->sreg1 == sh4_r0);
			sh4_or_imm_R0(&buffer, inst->inst_imm);
			break;

		case OP_IOR:
			/* MD: int_or: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_or(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_SH4_XOR_IMM_R0:
			/* MD: sh4_xor_imm_R0: clob:1 dest:z src1:z len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			g_assert(inst->sreg1 == sh4_r0);
			sh4_xor_imm_R0(&buffer, inst->inst_imm);
			break;

		case OP_IXOR:
			/* MD: int_xor: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_xor(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_SH4_AND_IMM_R0:
			/* MD: sh4_and_imm_R0: clob:1 dest:z src1:z len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			g_assert(inst->sreg1 == sh4_r0);
			sh4_and_imm_R0(&buffer, inst->inst_imm);
			break;

		case OP_IAND:
			/* MD: int_and: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_and(&buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_SHLL1:
			/* MD: sh4_shll1: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shll(&buffer, inst->dreg);
			break;

		case OP_SH4_SHLL2:
			/* MD: sh4_shll2: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shll2(&buffer, inst->dreg);
			break;

		case OP_SH4_SHLL8:
			/* MD: sh4_shll8: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shll8(&buffer, inst->dreg);
			break;

		case OP_SH4_SHLL16:
			/* MD: sh4_shll16: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shll16(&buffer, inst->dreg);
			break;

		case OP_SH4_SHAR1:
			/* MD: sh4_shar1: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shar(&buffer, inst->dreg);
			break;

		case OP_SH4_SHLR1:
			/* MD: sh4_shlr1: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shlr(&buffer, inst->dreg);
			break;

		case OP_SH4_SHLR2:
			/* MD: sh4_shlr2: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shlr2(&buffer, inst->dreg);
			break;

		case OP_SH4_SHLR8:
			/* MD: sh4_shlr8: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shlr8(&buffer, inst->dreg);
			break;

		case OP_SH4_SHLR16:
			/* MD: sh4_shlr16: clob:1 dest:i src1:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shlr16(&buffer, inst->dreg);
			break;

		case OP_SH4_SHAD:
			/* MD: sh4_shad: clob:1 dest:i src1:i src2:i len:2 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_shad(&buffer, inst->sreg2, inst->dreg);
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
			sh4_shld(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_ISHR_UN:
			/* MD: int_shr_un: clob:1 dest:i src1:i src2:i len:4 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_neg(&buffer, inst->sreg2, sh4_temp);
			sh4_shld(&buffer, sh4_temp, inst->dreg);
			break;

		case OP_ISHR:
			/* MD: int_shr: clob:1 dest:i src1:i src2:i len:4 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_neg(&buffer, inst->sreg2, sh4_temp);
			sh4_shad(&buffer, sh4_temp, inst->dreg);
			break;

		case OP_IMUL:
			/* MD: int_mul: dest:i src1:i src2:i len:4 */
			/* 32*32 bit multiplication -> result in MACL reg.
			 * transfer MACL into destination reg. */
			sh4_mull(&buffer, inst->sreg1, inst->sreg2);
			sh4_sts_MACL(&buffer, inst->dreg);
			break;

		case OP_SH4_CMPEQ:
			/* MD: sh4_cmpeq: src1:i src2:i len:2 */
			sh4_cmpeq(&buffer, inst->sreg1, inst->sreg2);
			break;

		case OP_SH4_CMPEQ_IMM_R0:
			/* MD: sh4_cmpeq_imm_R0: src1:z len:2 */
			g_assert(inst->sreg1 == sh4_r0);
			sh4_cmpeq_imm_R0(&buffer, inst->inst_imm);
			break;

		case OP_SH4_CMPGT:
			/* MD: sh4_cmpgt: src1:i src2:i len:2 */
			sh4_cmpgt(&buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_CMPHI:
			/* MD: sh4_cmphi: src1:i src2:i len:2 */
			sh4_cmphi(&buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_CMPGE:
			/* MD: sh4_cmpge: src1:i src2:i len:2 */
			sh4_cmpge(&buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_CMPHS:
			/* MD: sh4_cmphs: src1:i src2:i len:2 */
			sh4_cmphs(&buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_NOT_CMPGE:
			/* MD: sh4_not_cmpge: src1:i src2:i len:2 */
			sh4_cmpge(&buffer, inst->sreg1, inst->sreg2);
			break;

		case OP_SH4_NOT_CMPHS:
			/* MD: sh4_not_cmphs: src1:i src2:i len:2 */
			sh4_cmphs(&buffer, inst->sreg1, inst->sreg2);
			break;

		case OP_SH4_FCMPEQ:
			/* MD: sh4_fcmpeq: src1:f src2:f len:2 */
			sh4_fcmpeq_double(&buffer, inst->sreg1, inst->sreg2);
			break;

		case OP_SH4_FCMPGT:
			/* MD: sh4_fcmpgt: src1:f src2:f len:2 */
			sh4_fcmpgt_double(&buffer, inst->sreg2, inst->sreg1);
			break;

		case OP_SH4_MOVT:
			/* MD: sh4_movt: dest:i len:2 */
			sh4_movt(&buffer, inst->dreg);
			break;

		case OP_SH4_PUSH_ARG:
			/* MD: sh4_push_arg: src1:i len:2 */
			sh4_movl_decRx(&buffer, inst->sreg1, sh4_sp);
			break;

		case OP_SH4_PUSH_F32ARG:
			/* MD: sh4_push_f32arg: src1:f len:6 */
			sh4_fcnvds_double_FPUL(&buffer, inst->sreg1);
			sh4_fsts_FPUL(&buffer, inst->sreg1);
			sh4_fmovs_decRx(&buffer, inst->sreg1, sh4_sp);
			break;

		case OP_SH4_PUSH_F64ARG:
			/* MD: sh4_push_f64arg: src1:f len:4 */
			sh4_fmovs_decRx(&buffer, inst->sreg1 + 1, sh4_sp);
			sh4_fmovs_decRx(&buffer, inst->sreg1, sh4_sp);
			break;

		case OP_SH4_STOREI1_MEMBASE_R0:
			/* MD: sh4_storei1_membase_R0: src1:z dest:b len:2 */
			g_assert(inst->sreg1 == sh4_r0);
			sh4_movb_R0_dispRx(&buffer, inst->inst_offset, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI1:
			/* MD: sh4_storei1: src1:i dest:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movb_indRx(&buffer, inst->sreg1, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI2_MEMBASE_R0:
			/* MD: sh4_storei2_membase_R0: src1:z dest:b len:2 */
			g_assert(inst->sreg1 == sh4_r0);
			sh4_movw_R0_dispRx(&buffer, inst->inst_offset, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI2:
			/* MD: sh4_storei2: src1:i dest:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movw_indRx(&buffer, inst->sreg1, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI4_MEMBASE:
			/* MD: sh4_storei4_membase: src1:i dest:b len:2 */
			sh4_movl_dispRx(&buffer, inst->sreg1, inst->inst_offset, inst->inst_destbasereg);
			break;

		case OP_SH4_STOREI4:
			/* MD: sh4_storei4: src1:i dest:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movl_indRx(&buffer, inst->sreg1, inst->inst_destbasereg);
			break;

		case OP_SH4_LOADI1_MEMBASE_R0:
			/* MD: sh4_loadi1_membase_R0: dest:z src1:b len:2 */
			g_assert(inst->dreg == 0);
			sh4_movb_dispRy_R0(&buffer, inst->inst_offset, inst->inst_basereg);
			break;

		case OP_SH4_LOADI1:
			/* MD: sh4_loadi1: dest:i src1:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movb_indRy(&buffer, inst->inst_basereg, inst->dreg);
			break;

		case OP_SH4_LOADI2_MEMBASE_R0:
			/* MD: sh4_loadi2_membase_R0: dest:z src1:b len:2 */
			g_assert(inst->dreg == 0);
			sh4_movw_dispRy_R0(&buffer, inst->inst_offset, inst->inst_basereg);
			break;

		case OP_SH4_LOADI2:
			/* MD: sh4_loadi2: dest:i src1:b len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movw_indRy(&buffer, inst->inst_basereg, inst->dreg);
			break;

		case OP_SH4_LOADU1_MEMBASE_R0:
			/* MD: sh4_loadu1_membase_R0: dest:z src1:b len:4 */
			g_assert(inst->dreg == 0);
			sh4_movb_dispRy_R0(&buffer, inst->inst_offset, inst->inst_basereg);
			sh4_extub(&buffer, sh4_r0, sh4_r0); /* Adjust the sign. */
			break;

		case OP_SH4_LOADU1:
			/* MD: sh4_loadu1: dest:i src1:b len:4 */
			g_assert(inst->inst_offset == 0);
			sh4_movb_indRy(&buffer, inst->inst_basereg, inst->dreg);
			sh4_extub(&buffer, inst->dreg, inst->dreg); /* Adjust the sign. */
			break;

		case OP_SH4_LOADU2_MEMBASE_R0:
			/* MD: sh4_loadu2_membase_R0: dest:z src1:b len:4 */
			g_assert(inst->dreg == 0);
			sh4_movw_dispRy_R0(&buffer, inst->inst_offset, inst->inst_basereg);
			sh4_extuw(&buffer, sh4_r0, sh4_r0); /* Adjust the sign. */
			break;

		case OP_SH4_LOADU2:
			/* MD: sh4_loadu2: dest:i src1:b len:4 */
			g_assert(inst->inst_offset == 0);
			sh4_movw_indRy(&buffer, inst->inst_basereg, inst->dreg);
			sh4_extuw(&buffer, inst->dreg, inst->dreg); /* Adjust the sign. */
			break;

		case OP_SH4_LOADI4_MEMBASE:
			/* MD: sh4_loadi4_membase: dest:i src1:b len:2 */
			sh4_movl_dispRy(&buffer, inst->inst_offset, inst->inst_basereg, inst->dreg);
			break;

		case OP_SH4_LOADI4:
			/* MD: sh4_loadi4: dest:i src1:i len:2 */
			g_assert(inst->inst_offset == 0);
			sh4_movl_indRy(&buffer, inst->inst_basereg, inst->dreg);
			break;

		case OP_ICONST:
			/* MD: iconst: dest:i len:12 */
			if (SH4_CHECK_RANGE_mov_imm(inst->inst_c0))
				sh4_mov_imm(&buffer, inst->inst_c0, inst->dreg);
			else
				sh4_cstpool_add(cfg, &buffer, MONO_PATCH_INFO_NONE, &(inst->inst_c0), inst->dreg);
			break;

		case OP_FCALL:
			/* MD: fcall: dest:y clob:c len:30 */
		case OP_VOIDCALL:
			/* MD: voidcall: clob:c len:30 */
		case OP_VCALL:
			/* MD: vcall: clob:c len:30 */
		case OP_VCALL2:
			/* MD: vcall2: clob:c len:30 */
		case OP_LCALL:
			/* MD: lcall: dest:Z clob:c len:30 */
		case OP_CALL:
			/* MD: call: dest:z clob:c len:30 */
			call = (MonoCallInst*)inst;

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

			sh4_cstpool_add(cfg, &buffer, type, target, sh4_temp);

			sh4_jsr_indRx(&buffer, sh4_temp);
			sh4_nop(&buffer); /* delay slot */

			if (call->stack_usage != 0)
				free_args_area(cfg, &buffer, call);
			break;

		case OP_FCALL_REG:
			/* MD: fcall_reg: dest:y src1:i clob:c len:18 */
		case OP_VOIDCALL_REG:
			/* MD: voidcall_reg: src1:i clob:c len:18 */
		case OP_LCALL_REG:
			/* MD: lcall_reg: dest:Z src1:i clob:c len:18 */
		case OP_CALL_REG:
			/* MD: call_reg: dest:z src1:i clob:c len:18 */
			call = (MonoCallInst*)inst;

			sh4_jsr_indRx(&buffer, inst->sreg1);
			sh4_nop(&buffer); /* delay slot */

			if (call->stack_usage != 0)
				free_args_area(cfg, &buffer, call);
			break;

		case OP_CALL_HANDLER:
			/* MD: call_handler: len:16 */
			sh4_cstpool_add(cfg, &buffer, MONO_PATCH_INFO_BB, inst->inst_target_bb, sh4_temp);

			sh4_jsr_indRx(&buffer, sh4_temp);
			sh4_nop(&buffer); /* delay slot */
			break;

		case OP_THROW:
			/* MD: throw: src1:i len:18 */
			if (inst->sreg1 != MONO_SH4_REG_FIRST_ARG)
				sh4_mov(&buffer, inst->sreg1, MONO_SH4_REG_FIRST_ARG);

			sh4_cstpool_add(cfg, &buffer, MONO_PATCH_INFO_INTERNAL_METHOD, (gpointer)"mono_arch_throw_exception", sh4_temp);

			sh4_jsr_indRx(&buffer, sh4_temp);
			sh4_nop(&buffer); /* delay slot */
			break;

		case OP_RETHROW:
			/* MD: rethrow: src1:i len:18 */
			if (inst->sreg1 != MONO_SH4_REG_FIRST_ARG)
				sh4_mov(&buffer, inst->sreg1, MONO_SH4_REG_FIRST_ARG);

			sh4_cstpool_add(cfg, &buffer, MONO_PATCH_INFO_INTERNAL_METHOD, (gpointer)"mono_arch_rethrow_exception", sh4_temp);

			sh4_jsr_indRx(&buffer, sh4_temp);
			sh4_nop(&buffer); /* delay slot */
			break;

		case OP_ICONV_TO_I4:
			/* MD: int_conv_to_i4: dest:i src1:i len:2 */
		case OP_ICONV_TO_U4:
			/* MD: int_conv_to_u4: dest:i src1:i len:2 */
		case OP_MOVE:
			/* MD: move: dest:i src1:i len:2 */
			if (inst->sreg1 != inst->dreg)
				sh4_mov(&buffer, inst->sreg1, inst->dreg);
			break;

		case OP_ICONV_TO_I1:
			/* MD: int_conv_to_i1: dest:i src1:i len:2 */
			sh4_extsb(&buffer, inst->sreg1, inst->dreg);
			break;

		case OP_ICONV_TO_U1:
			/* MD: int_conv_to_u1: dest:i src1:i len:2 */
			sh4_extub(&buffer, inst->sreg1, inst->dreg);
			break;

		case OP_ICONV_TO_I2:
			/* MD: int_conv_to_i2: dest:i src1:i len:2 */
			sh4_extsw(&buffer, inst->sreg1, inst->dreg);
			break;

		case OP_ICONV_TO_U2:
			/* MD: int_conv_to_u2: dest:i src1:i len:2 */
			sh4_extuw(&buffer, inst->sreg1, inst->dreg);
			break;

		/**
		 * The START_HANDLER instruction marks the beginning of a handler
		 * block. It is called using a call instruction, so sh4_pr contains
		 * the return address. Since the handler executes in the same stack
		 * frame as the method itself, we can't use save/restore to save
		 * the return address. Instead, we save it into a dedicated
		 * variable.
		 */
		case OP_START_HANDLER:
			/* MD: start_handler: len:8 */
			sh4_stsl_PR_decRx(&buffer, sh4_sp);

			spvar = mono_find_spvar_for_region(cfg, basic_block->region);
			g_assert(spvar->inst_basereg == sh4_fp);

			sh4_base_store(cfg, &buffer, sh4_sp, spvar->inst_offset, spvar->inst_basereg);

			break;

		/* Restore the return address saved with the opcode "start_handler",
		 * and return the value in "sreg1" if it is an "endfilter". */
		case OP_ENDFILTER:
			/* MD: endfilter: src1:z len:12 */
			/* The local allocator will put the result into sh4_r0. */
		case OP_ENDFINALLY:
			/* MD: endfinally: len:12 */
			spvar = mono_find_spvar_for_region(cfg, basic_block->region);
			g_assert(spvar->inst_basereg == sh4_fp);

			sh4_base_load(cfg, &buffer, spvar->inst_offset, spvar->inst_basereg, sh4_sp);

			sh4_ldsl_incRx_PR(&buffer, sh4_sp);

			sh4_rts(&buffer);
			sh4_nop(&buffer);

			break;

		case OP_BR:
			/* MD: br: len:16 */
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
				sh4_bra_label(&buffer, address);
				sh4_nop(&buffer);
				break;
			}

			sh4_cstpool_add(cfg, &buffer, type, target, sh4_temp);

			sh4_jmp_indRx(&buffer, sh4_temp);
			sh4_nop(&buffer);

			break;

		case OP_BR_REG:
			/* MD: br_reg: src1:i len:4 */
			sh4_jmp_indRx(&buffer, inst->sreg1);
			sh4_nop(&buffer);
			break;

		case OP_SH4_BT:
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
					sh4_bt_label(&buffer, address);
				}
				else {
					sh4_bf_label(&buffer, address);
				}
				break;
			}

			/* Reverse the test to skip the unconditional jump. */
			patch = buffer;
			sh4_die(&buffer); /* patch slot for : bf/t_label "skip_jump" */

			sh4_cstpool_add(cfg, &buffer, type, target, sh4_temp);

			sh4_jmp_indRx(&buffer, sh4_temp);
			sh4_nop(&buffer);

			/* Back patch the reversed test. */
			if (inst->opcode == OP_SH4_BT) {
				sh4_bf_label(&patch, buffer);
			}
			else {
				sh4_bt_label(&patch, buffer);
			}

			break;

		case OP_SH4_CLRT:
			/* MD: sh4_clrt: len:2 */
			sh4_clrt(&buffer);
			break;

		case OP_SH4_NEGC:
			/* MD: sh4_negc: dest:i src1:i len:2 */
			sh4_negc(&buffer, inst->sreg1, inst->dreg);
			break;

		case OP_SH4_ROTCL:
			/* MD: sh4_rotcl: dest:i src1:i len:2 clob:1 */
			g_assert(inst->sreg1 == inst->dreg);
			sh4_rotcl(&buffer, inst->dreg);
			break;

		case OP_SH4_TST:
			/* MD: sh4_tst: src1:i src2:i len:2 */
			sh4_tst(&buffer, inst->sreg1, inst->sreg2);
			break;

		case OP_INOT:
			/* MD: int_not: dest:i src1:i len:2 */
			sh4_not(&buffer, inst->sreg1, inst->dreg);
			break;

		case OP_INEG:
			/* MD: int_neg: dest:i src1:i len:2 */
			sh4_neg(&buffer, inst->sreg1, inst->dreg);
			break;

		case OP_LABEL:
			/* MD: label: len:0 */
			inst->inst_c0 = code - cfg->native_code;
			break;

		case OP_CHECK_THIS:
			/* MD: checkthis: src1:b len:2 */
			/* Trig an exception if sreg1 can not be dereferenced,
			   might be misaligned in case of vtypes so use a byte load. */
			sh4_movb_indRy(&buffer, inst->sreg1, sh4_temp);
			break;

			/* Support for spilled variables. */
		case OP_STORE_MEMBASE_REG:
			/* MD: store_membase_reg: dest:b src1:i len:16 */
			g_assert(inst->inst_destbasereg == sh4_fp);
			sh4_base_store(cfg, &buffer, inst->sreg1, inst->inst_offset, inst->inst_destbasereg);
			break;

			/* Support for spilled variables. */
		case OP_LOAD_MEMBASE:
			/* MD: load_membase: dest:i src1:b len:16 */
			g_assert(inst->inst_basereg == sh4_fp);
			sh4_base_load(cfg, &buffer, inst->inst_offset, inst->inst_basereg, inst->dreg);
			break;

		case OP_NOP:		/* MD: nop: len:0 */
		case OP_DUMMY_USE:	/* MD: dummy_use: len:0 */
		case OP_DUMMY_STORE:	/* MD: dummy_store: len:0 */
		case OP_NOT_NULL:	/* MD: not_null: len:0 */
			break;

		case OP_NOT_REACHED:	/* MD: not_reached: len:2 */
			sh4_die(&buffer);
			break;

		case OP_MEMORY_BARRIER:
			/* MD: memory_barrier: len:2 */
#ifdef __SH4A__
			sh4_synco(&buffer);
#endif
			break;

		case OP_BREAK:
			/* MD: break: len:2 */
			sh4_trapa_imm(&buffer, 0x40);
			break;

		case OP_JUMP_TABLE:
			/* MD: jump_table: dest:i len:12 */
			sh4_cstpool_add(cfg, &buffer, (MonoJumpInfoType)inst->inst_i1, inst->inst_p0, inst->dreg);
			break;

		case OP_SETLRET:
			/* MD: setlret: src1:i src2:i len:0 */
			g_assert(inst->sreg1 == sh4_r0);
			g_assert(inst->sreg2 == sh4_r1);
			break;

		case OP_SH4_BZERO4:
			/* MD: sh4_bzero4: dest:i src1:b src2:i len:18 */
			g_assert(inst->dreg != inst->inst_basereg);

			/* Setup the iterator at the top of the block to clean. */
			sh4_mov(&buffer, inst->inst_basereg, sh4_temp);
			sh4_add(&buffer, inst->inst_indexreg, sh4_temp);
			sh4_mov(&buffer, sh4_temp, inst->dreg);

			sh4_mov_imm(&buffer, 0, sh4_temp);

			/* Iterate down until dreg <= basereg */
			loop = buffer;
			sh4_cmphi(&buffer, inst->inst_basereg, inst->dreg);
			sh4_bf_label(&buffer, buffer + 6); /* dreg > basereg ? */
				sh4_movl_decRx(&buffer, sh4_temp, inst->dreg);
			sh4_bra_label(&buffer, loop);
			sh4_nop(&buffer);
			break;

		case OP_FMOVE:
			/* MD: fmove: dest:f src1:f len:4 */
			if (inst->sreg1 != inst->dreg) {
				sh4_fmov(&buffer, inst->sreg1, inst->dreg);
				sh4_fmov(&buffer, inst->sreg1 + 1, inst->dreg + 1);
			}
			break;

		case OP_R8CONST:
			/* MD: r8const: dest:f clob:z len:24 */
			/* TODO - CV: decompose in the lowering pass when value == '0.0' || value == '1.0'. */
			/* TODO - CV: sh4_cstpool_add(cfg, &buffer, MONO_PATCH_INFO_R8, inst->inst_p0, inst->dreg); */
			sh4_cstpool_addd(cfg, &buffer, *(double *)inst->inst_p0);
			sh4_fmovs_incRy(&buffer, sh4_r0, inst->dreg + 1);
			sh4_fmovs_indRy(&buffer, sh4_r0, inst->dreg);
			break;

		case OP_R4CONST:
			/* MD: r4const: dest:f clob:z len:20 */
			/* TODO - CV: decompose in the lowering pass when value == '0.0' || value == '1.0'. */
			/* TODO - CV: sh4_cstpool_add(cfg, &buffer, MONO_PATCH_INFO_R4, inst->inst_p0, inst->dreg); */
			sh4_cstpool_addf(cfg, &buffer, *(float *)inst->inst_p0);
			sh4_fmovs_indRy(&buffer, sh4_r0, inst->dreg);

			/* Convert [internally] to double precision, as specified by the ECMA. */
			sh4_flds_FPUL(&buffer, inst->dreg);
			sh4_fcnvsd_FPUL_double(&buffer, inst->dreg);
			break;

		case OP_ICONV_TO_R4:
			/* MD: int_conv_to_r4: dest:f src1:i len:4 */
			sh4_lds_FPUL(&buffer, inst->sreg1);
			sh4_float_FPUL(&buffer, inst->dreg);
			break;

		case OP_ICONV_TO_R8:
			/* MD: int_conv_to_r8: dest:f src1:i len:4 */
			sh4_lds_FPUL(&buffer, inst->sreg1);
			sh4_float_FPUL_double(&buffer, inst->dreg);
			break;

		case OP_FCONV_TO_I:
			/* MD: float_conv_to_i: dest:i src1:f len:4 */
		case OP_FCONV_TO_I4:
			/* MD: float_conv_to_i4: dest:i src1:f len:4 */
			sh4_ftrc_double_FPUL(&buffer, inst->sreg1);
			sh4_sts_FPUL(&buffer, inst->dreg);
			break;

		case OP_FCONV_TO_I2: /* MD: float_conv_to_i2: dest:i src1:f len:6 */
			sh4_ftrc_double_FPUL(&buffer, inst->sreg1);
			sh4_sts_FPUL(&buffer, inst->dreg);
			sh4_extsw(&buffer, inst->dreg, inst->dreg);
			break;

		case OP_FCONV_TO_U2: /* MD: float_conv_to_u2: dest:i src1:f len:6 */
			sh4_ftrc_double_FPUL(&buffer, inst->sreg1);
			sh4_sts_FPUL(&buffer, inst->dreg);
			sh4_extuw(&buffer, inst->dreg, inst->dreg);
			break;

		case OP_FCONV_TO_I1: /* MD: float_conv_to_i1: dest:i src1:f len:6 */
			sh4_ftrc_double_FPUL(&buffer, inst->sreg1);
			sh4_sts_FPUL(&buffer, inst->dreg);
			sh4_extsb(&buffer, inst->dreg, inst->dreg);
			break;

		case OP_FCONV_TO_U1: /* MD: float_conv_to_u1: dest:i src1:f len:6 */
			sh4_ftrc_double_FPUL(&buffer, inst->sreg1);
			sh4_sts_FPUL(&buffer, inst->dreg);
			sh4_extub(&buffer, inst->dreg, inst->dreg);
			break;

		case OP_SH4_FCNVSD: /* MD: sh4_fcnvsd: dest:f src1:f len:4 */
			sh4_flds_FPUL(&buffer, inst->dreg);
			sh4_fcnvsd_FPUL_double(&buffer, inst->dreg);
			break;

		case OP_FCONV_TO_R4: /* MD: float_conv_to_r4: dest:f src1:f len:0 */
		case OP_FCONV_TO_R8: /* MD: float_conv_to_r8: dest:f src1:f len:0 */
			break;

		case OP_FADD:
			/* MD: float_add: clob:1 dest:f src1:f src2:f len:2 */
			sh4_fadd_double(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_FSUB:
			/* MD: float_sub: clob:1 dest:f src1:f src2:f len:2 */
			sh4_fsub_double(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_FMUL:
			/* MD: float_mul: clob:1 dest:f src1:f src2:f len:2 */
			sh4_fmul_double(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_FDIV:
			/* MD: float_div: clob:1 dest:f src1:f src2:f len:2 */
			sh4_fdiv_double(&buffer, inst->sreg2, inst->dreg);
			break;

		case OP_FNEG:
			/* MD: float_neg: dest:f src1:f len:2 */
			sh4_fneg_double(&buffer, inst->dreg);
			break;

			/* Support for spilled floating-point variables. */
		case OP_STORER8_MEMBASE_REG:
			/* MD: storer8_membase_reg: dest:b src1:f len:16 */
			sh4_base_storef64(cfg, &buffer, inst->sreg1, inst->inst_offset, inst->inst_destbasereg);
			break;

			/* Support for spilled floating-point variables. */
		case OP_LOADR8_MEMBASE:
			/* MD: loadr8_membase: dest:f src1:b len:16 */
			sh4_base_loadf64(cfg, &buffer, inst->inst_offset, inst->inst_basereg, inst->dreg);
			break;

		case OP_STORER4_MEMBASE_REG:
			/* MD: storer4_membase_reg: dest:b src1:f len:18 */
			/* TODO - CV: decomposable in the lowering pass? */
			sh4_base_storef32(cfg, &buffer, inst->sreg1, inst->inst_offset, inst->inst_destbasereg);
			break;

		case OP_LOADR4_MEMBASE:
			/* MD: loadr4_membase: dest:f src1:b len:18 */
			/* TODO - CV: decomposable in the lowering pass? */
			sh4_base_loadf32(cfg, &buffer, inst->inst_offset, inst->inst_basereg, inst->dreg);
			break;

		/* These opcodes are missing for iltests.il. */
		case OP_CKFINITE:	 /* MD: ckfinite: dest:f src1:f len:0 */
		case OP_JMP:	 	 /* MD: jmp: len:0 */

			fprintf(stderr, "Method %s:%s opcode %s (0x%x) not yet implemented\n",
				cfg->method->klass->name,
				cfg->method->name,
				mono_inst_name(inst->opcode), inst->opcode);
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

	sh4_cstpool_check_end_bb(cfg, basic_block);

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
			g_assert_not_reached();
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
	case sh4_fp: return "sh4_fp";
	case sh4_sp: return "sh4_sp";
	}
	return "unknown";
}

const char *mono_arch_fregname(int reg)
{
	switch (reg) {
	case sh4_dr0: return "sh4_dr0";
	case sh4_fr1: return "sh4_fr1";
	case sh4_dr2: return "sh4_dr2";
	case sh4_fr3: return "sh4_fr3";
	case sh4_dr4: return "sh4_dr4";
	case sh4_fr5: return "sh4_fr5";
	case sh4_dr6: return "sh4_dr6";
	case sh4_fr7: return "sh4_fr7";
	case sh4_dr8: return "sh4_dr8";
	case sh4_fr9: return "sh4_fr9";
	case sh4_dr10: return "sh4_dr10";
	case sh4_fr11: return "sh4_fr11";
	case sh4_dr12: return "sh4_dr12";
	case sh4_fr13: return "sh4_fr13";
	case sh4_dr14: return "sh4_dr14";
	case sh4_fr15: return "sh4_fr15";
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
	guint32 size = inst->backend.size;

	/* To be optimized later on.*/
	MONO_EMIT_NEW_BIALU_IMM(cfg, OP_SUB_IMM, sh4_sp, sh4_sp, size);
	mini_emit_memcpy(cfg, sh4_sp, 0, src->dreg, 0, size, 4);

	return;
}

void mono_arch_decompose_opts (MonoCompile *cfg, MonoInst *ins)
{
	/* The macros MONO_EMIT_NEW_* use internally a variable named "inst",
	   so do not use something like "inst->XXX" as parameter. */

        switch (ins->opcode) {
        case OP_IADD_OVF:
                ins->opcode = OP_SH4_IADD_OVF;
                MONO_EMIT_NEW_COND_EXC(cfg, IC, "OverflowException");
                break;

        case OP_ISUB_OVF:
                ins->opcode = OP_SH4_ISUB_OVF;
                MONO_EMIT_NEW_COND_EXC(cfg, IC, "OverflowException");
                break;

        case OP_LADD_OVF:
		/* A long register N is splitted into two integer registers N+1, N+2. */
                MONO_EMIT_NEW_BIALU(cfg, OP_IADDCC, ins->dreg + 1, ins->sreg1 + 1, ins->sreg2 + 1);
                MONO_EMIT_NEW_BIALU(cfg, OP_SH4_IADC_OVF, ins->dreg + 2, ins->sreg1 + 2, ins->sreg2 + 2);
                MONO_EMIT_NEW_COND_EXC(cfg, IC, "OverflowException");
                ins->opcode = OP_NOP;
                break;

        case OP_LSUB_OVF:
		/* A long register N is splitted into two integer registers N+1, N+2. */
                MONO_EMIT_NEW_BIALU(cfg, OP_ISUBCC, ins->dreg + 1, ins->sreg1 + 1, ins->sreg2 + 1);
                MONO_EMIT_NEW_BIALU(cfg, OP_SH4_ISBB_OVF, ins->dreg + 2, ins->sreg1 + 2, ins->sreg2 + 2);
                MONO_EMIT_NEW_COND_EXC(cfg, IC, "OverflowException");
                ins->opcode = OP_NOP;
                break;

        default:
                return;
        }
}
