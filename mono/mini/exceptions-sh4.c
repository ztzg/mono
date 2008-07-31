/*
 * exceptions-sh4.c: exception support for SH4
 *
 * Authors:
 *   Cedric VINCENT (cedric.vincent@gmail.com)
 *
 * (C) 2008 STMicroelectronics.
 */

#include <glib.h>
#include "mini.h"

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

gpointer mono_arch_get_restore_context(void)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
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
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

void mono_arch_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
	/* TODO - CV */
	g_assert(0);
	return;
}
