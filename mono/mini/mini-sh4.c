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

void mono_arch_allocate_vars(MonoCompile *cfg)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

MonoCallInst *mono_arch_call_opcode(MonoCompile *cfg, MonoBasicBlock* bb, MonoCallInst *call, int is_virtual)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
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

void mono_arch_emit_call(MonoCompile *cfg, MonoCallInst *call)
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

MonoInst *mono_arch_emit_inst_for_method(MonoCompile *cfg, MonoMethod *cmethod, MonoMethodSignature *fsig, MonoInst **args)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

void mono_arch_emit_outarg_vt(MonoCompile *cfg, MonoInst *ins, MonoInst *src)
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

void mono_arch_emit_setret(MonoCompile *cfg, MonoMethod *method, MonoInst *val)
{
	/* TODO - CV */
	g_assert(0);
	return;
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
#endif
	mono_memory_barrier();

#if defined (__SH4A__)
	while (addr <= end) {
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

int mono_arch_get_argument_info(MonoMethodSignature *csig, int param_count, MonoJitArgumentInfo *arg_info)
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
	g_assert(0);
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
