/*
 * tramp-sh4.c: JIT trampoline code for SH4
 *
 * Authors:
 *   Cedric VINCENT (cedric.vincent@gmail.com)
 *
 * (C) 2008 STMicroelectronics.
 */

#include <glib.h>
#include "mini.h"

gpointer mono_arch_create_rgctx_lazy_fetch_trampoline(guint32 slot)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

gpointer mono_arch_create_specific_trampoline(gpointer arg1, MonoTrampolineType tramp_type, MonoDomain *domain, guint32 *code_len)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

guchar *mono_arch_create_trampoline_code (MonoTrampolineType tramp_type)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

guint32 mono_arch_get_rgctx_lazy_fetch_offset(gpointer *regs)
{
	/* TODO - CV */
	g_assert(0);
	return 0;
}

gpointer mono_arch_get_unbox_trampoline(MonoGenericSharingContext *gsctx, MonoMethod *m, gpointer addr)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

void mono_arch_nullify_class_init_trampoline(guint8 *code, gssize *regs)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_nullify_plt_entry(guint8 *code)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_patch_callsite(guint8 *method_start, guint8 *orig_code, guint8 *addr)
{
	/* TODO - CV */
	g_assert(0);
	return;
}

void mono_arch_patch_plt_entry(guint8 *code, guint8 *addr)
{
	/* TODO - CV */
	g_assert(0);
	return;
}
