/*
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
		structure
	} type;

	enum {
		into_register,
		onto_stack,
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

void mono_arch_allocate_vars(MonoCompile *cfg)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

static inline void add_int32_arg(SH4IntRegister *arg_reg, guint32 *stack_size, struct arg_info *arg_info)
{
	arg_info->size = 4;
	arg_info->type = integer32;
	arg_info->offset = *stack_size;

	if (*arg_reg <= MONO_SH4_REG_LAST_ARG) {
		arg_info->storage = into_register;
		(*arg_reg)++;
	}
	else {
		arg_info->storage = onto_stack;
		(*stack_size) += 4;
	}
}

/**
 * Obtain information about a call according to the calling convention and
 * determine the amount of space required for code and stack. In addition
 * determine starting points for stack-based arguments, and area for
 * structures being returned on the stack.
 */
static struct call_info *get_call_info(MonoGenericSharingContext *context, MonoMethodSignature *signature)
{
	struct call_info *call_info = NULL;
	SH4IntRegister arg_reg = MONO_SH4_REG_FIRST_ARG;
	MonoType *basic_type = NULL;
	guint32 stack_size = 0;
	guint32 i = 0;

	call_info = g_malloc0(sizeof(struct call_info));
	call_info->args = g_malloc0(sizeof(struct arg_info) * (signature->param_count + signature->hasthis));

	/* Determine where the result will be stored. */
	basic_type = mono_type_get_underlying_type(signature->ret);
	basic_type = mini_get_basic_type_from_generic(context, basic_type);
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
		/* Nothing to do */
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
		/* Used to return structure, for instance. */
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
static inline void emit_signature_cookie(MonoCompile *cfg, MonoCallInst *call, struct arg_info *arg_info)
{
	MonoInst *arg = NULL;
	MonoInst *signature = NULL;

	/* Declare a room where the signature cookie will be stored. */
	MONO_INST_NEW(cfg, signature, OP_ICONST);
	signature->inst_p0 = call->signature;

	/* Create a new argument pointing to the signature cookie. */
	MONO_INST_NEW(cfg, arg, OP_OUTARG);
	arg->inst_left = signature;
	arg->inst_call = call;

	if (arg_info->storage == into_register) {
		arg->backend.reg3 = arg_info->reg;
	}
	else { /* arg_info->storage == onto_stack */
		arg->opcode = OP_OUTARG_MEMBASE;
		arg->backend.size = arg_info->size;
		arg->backend.arg_info = arg_info->offset;
	}

	MONO_INST_LIST_ADD_TAIL(&arg->node, &call->out_args);

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

	signature = call->signature;
	arg_count = signature->param_count + signature->hasthis;
	call_info = get_call_info(cfg->generic_sharing_context, signature);

	if (call_info->ret.type == structure)
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
			emit_signature_cookie(cfg, call, &(call_info->sig_cookie));

		MONO_INST_NEW(cfg, arg, OP_OUTARG);
		arg->cil_code  = call->args[i]->cil_code;
		arg->type      = call->args[i]->type;
		arg->inst_left = call->args[i];
		arg->inst_call = call;

		switch (arg_info->type) {
		case integer64:
			if (arg_info->storage == into_register)
				call->used_iregs |= 1 << (arg_info->reg + 1);
			/* Fall through. */
		case integer32:
			if (arg_info->storage == into_register) {
				arg->backend.reg3 = arg_info->reg;
				call->used_iregs |= 1 << arg_info->reg;
			}
			else { /* arg_info->storage == onto_stack */
				arg->opcode = OP_OUTARG_MEMBASE;
				arg->backend.size = arg_info->size;
				arg->backend.arg_info = arg_info->offset;
			}
			break;

		case float32:
			if (arg_info->storage == into_register) {
				NOT_IMPLEMENTED;
			}
			else { /* arg_info->storage == onto_stack */
				NOT_IMPLEMENTED;
			}
			break;

		case float64:
			if (arg_info->storage == into_register) {
				NOT_IMPLEMENTED;
			}
			else { /* arg_info->storage == onto_stack */
				NOT_IMPLEMENTED;
			}
			break;

		case structure:
			if (arg_info->storage == into_register) {
				NOT_IMPLEMENTED;
			}
			else { /* arg_info->storage == onto_stack */
				NOT_IMPLEMENTED;
			}
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
		emit_signature_cookie(cfg, call, &(call_info->sig_cookie));

	if (call_info->stack_align_amount != 0) {
		; /* TODO - CV */
	}

	call->stack_usage = call_info->stack_usage;
	cfg->param_area = MAX(cfg->param_area, call->stack_usage);
	cfg->flags |= MONO_CFG_HAS_CALLS;

	g_free(call_info->args);
	g_free(call_info);

	return call;
}

void mono_arch_cleanup(void)
{
	/* TODO - CV */
	g_assert(0);
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
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_emit_epilog(MonoCompile *cfg)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_emit_exceptions(MonoCompile *cfg)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

guint8 *mono_arch_emit_prolog(MonoCompile *cfg)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

void mono_arch_emit_this_vret_args(MonoCompile *cfg, MonoCallInst *inst, int this_reg, int this_type, int vt_reg)
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
	/* TODO - CV */
	g_assert(0);
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

GList *mono_arch_get_allocatable_int_vars(MonoCompile *cfg)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
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

GList *mono_arch_get_global_int_regs(MonoCompile *cfg)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

MonoInst *mono_arch_get_inst_for_method(MonoCompile *cfg, MonoMethod *cmethod, MonoMethodSignature *fsig, MonoInst **args)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

MonoInst *mono_arch_get_thread_intrinsic(MonoCompile* cfg)
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
	/* TODO - CV */
	g_assert(0);
	return 0;
}

void mono_arch_lowering_pass(MonoCompile *cfg, MonoBasicBlock *bb)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_output_basic_block(MonoCompile *cfg, MonoBasicBlock *bb)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_patch_code(MonoMethod *method, MonoDomain *domain, guint8 *code, MonoJumpInfo *ji, gboolean run_cctors)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_peephole_pass_1(MonoCompile *cfg, MonoBasicBlock *bb)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_peephole_pass_2(MonoCompile *cfg, MonoBasicBlock *bb)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

gboolean mono_arch_print_tree(MonoInst *tree, int arity)
{
	/* TODO - CV */
	g_assert(0);
	return 0;
}

guint32 mono_arch_regalloc_cost(MonoCompile *cfg, MonoMethodVar *vmv)
{
	/* TODO - CV */
	g_assert(0);
	return 0;
}

void mono_arch_register_lowlevel_calls(void)
{
	/* TODO - CV */
	return;
}

const char *mono_arch_regname(int reg)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

void mono_arch_setup_jit_tls_data(MonoJitTlsData *tls)
{
	/* TODO - CV */
	return;
}
