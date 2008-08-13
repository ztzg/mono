/*
 * exceptions-sh4.c: exception support for SH4
 *
 * Authors:
 *   Cedric VINCENT (cedric.vincent@gmail.com)
 *
 * (C) 2008 STMicroelectronics.
 */

#include <glib.h>
#include <ucontext.h> /* ucontext_t, */

#include "mini.h"
#include "mono/arch/sh4/sh4-codegen.h"

MonoJitInfo *mono_arch_find_jit_info(MonoDomain *domain, MonoJitTlsData *jit_tls, MonoJitInfo *res, MonoJitInfo *prev_ji, MonoContext *ctx, MonoContext *new_ctx, char **trace, MonoLMF **lmf, int *native_offset, gboolean *managed)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

gpointer mono_arch_get_call_filter(void)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

/**
 * Returns a pointer to a method which restores a previously saved MonoContext.
 *
 * Technically, the generated method is similar to this pseudo-code :
 *
 * void restore_context(MonoContext *ctx)
 * {
 * 	// Restore almost all registers.
 * 	{ %R1, ..., %R15 } = ctx.regs[];
 * 
 * 	// Jump to the saved IP.
 * 	goto ctx.ip;
 * }
 */
gpointer mono_arch_get_restore_context(void)
{
	static guint8 *code = NULL;
	guint8 *buf = NULL;
	int i = 0;

#define RESTORE_CONTEXT_SIZE 42

	if (code != NULL)
		return code;

	code = buf = mono_global_codeman_reserve(RESTORE_CONTEXT_SIZE);

	/*
	 * Restore almost all registers.
	 */

	/* R0 is now used to point to "ctx.regs[]" (used later). */
	sh4_mov(buf, sh4_r4, sh4_r0);
	sh4_add_imm(buf, offsetof(MonoContext, regs), sh4_r0);

	/* pseudo-code: { %R1, ..., %R15 } = ctx.regs[]; */
	for (i = 1; i <= 15; i++)
		sh4_movl_dispRy(buf, i * 4, sh4_r0, (SH4IntRegister)i);

	/*
	 * Jump to the saved IP.
	 */

	/* pseudo-code: goto ctx.ip; */
	sh4_add_imm(buf, - offsetof(MonoContext, regs) + offsetof(MonoContext, ip), sh4_r0);
	sh4_movl_indRy(buf, sh4_r0, sh4_r0);
	sh4_jmp_indRx(buf, sh4_r0);
	sh4_nop(buf);

	/* Sanity checks. */
	g_assert(buf - code <= RESTORE_CONTEXT_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, RESTORE_CONTEXT_SIZE);

	return code;
}

gpointer mono_arch_get_rethrow_exception(void)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

gpointer mono_arch_get_throw_exception_by_name(void)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

gpointer mono_arch_get_throw_exception(void)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

gboolean mono_arch_handle_exception(void *sigctx, gpointer obj, gboolean test_only)
{
	/* TODO - CV */
	g_assert(0);
	return TRUE;
}

gpointer mono_arch_ip_from_context(void *sigctx)
{
	ucontext_t *ctx = (ucontext_t *)sigctx;
	return (gpointer)ctx->uc_mcontext.pc;
}

void mono_arch_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
	/* TODO - CV */
	g_assert(0);
	return;
}
