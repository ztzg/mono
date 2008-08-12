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

/**
 * Create the designated type of trampoline according to the
 * 'tramp_type' parameter.
 *
 * Technically, the generated trampoline is similar to this pseudo-code :
 *
 * inline void trampoline_code()
 * {
 * 	// Save all registers.
 * 	new_lmf.regs[] = { %R0, ..., %R15 };
 * 
 * 	struct MonoLMF new_lmf;
 * 
 * 	// This trampoline is called with the method's parameters, so the
 * 	// method's address is stored at the end of the previous frame.
 * 	new_lmf.method = %Caller_SP[0];
 * 
 * #if trampoline_type == MONO_TRAMPOLINE_JUMP
 * 	new_lmf.ip = NULL;
 * #else
 * 	new_lmf.ip = %PR;
 * #endif
 * 
 * 	new_lmf.lmf_addr = mono_get_lmf_addr();
 * 
 * 	// Insert the new LMF at the beginning of the LMF list.
 * 	new_lmf.previous_lmf = *(new_lmf.lmf_addr);
 * 	*(new_lmf.lmf_addr) = &new_lmf;
 * 
 * #def trampoline mono_get_trampoline_func(trampoline_type)
 * 
 * 	// Call the actual trampoline, parameters are :
 * 	compiled_methode = trampoline(new_lmf.regs, new_lmf.ip, new_lmf.method, NULL);
 * 
 * 	// Restore the previous LMF list.
 * 	*(new_lmf.lmf_addr) = &(new_lmf.previous_lmf);
 * 
 * 	// Restore almost all registers.
 * 	{ %R1, ..., %R14 } = new_lmf.regs[];
 * 	%PR = %Caller_PR;
 * 
 * 	// Remove the method's address from the previous frame.
 * 	%SP = %SP + 1;
 * 
 * #if trampoline_type != MONO_TRAMPOLINE_CLASS_INIT
 * 	goto compiled_methode;
 * #else
 * 	return;
 * #endif
 * }
 */
guchar *mono_arch_create_trampoline_code(MonoTrampolineType trampoline_type)
{
	guint8 * patch1 = 0;
	guint8 * patch2 = 0;
	int i = 0;

	guint8 *code = NULL;
	guint8 *buf = NULL;

#define TRAMPOLINE_SIZE 148

	code = buf = mono_global_codeman_reserve(TRAMPOLINE_SIZE);

	/* Save the return address. */
	sh4_stsl_PR_decRx(buf, sh4_r15);

	/* pseudo-code: struct MonoLMF new_lmf; */
	sh4_add_imm(buf, -sizeof(MonoLMF) + offsetof(MonoLMF, regs), sh4_r15);

	/* At this point, the stack looks like :
	 *	:          :
	 *	|          |
	 *	|----------|
	 *	|  method  | Caller's frame.
	 *	|==========|
	 *	|    PR    |
	 *	|----------|
	 *	| .regs[]  | Current frame.
	 *	| - - - - -| <- SP
	 *	| new_lmf  |
	 *	|----------|
	 *	:          :
	 */

	/*
	 * Save all registers.
	 */

	/* pseudo-code: new_lmf.regs[] = { %R0, ..., %R15 }; */
	for (i = 0; i < 15; i++)
		sh4_movl_dispRx(buf, (SH4IntRegister)i, i * 4, sh4_r15);

	/* Compute the previous SP value before saving into new_lmf.regs[]. */
	sh4_mov(buf, sh4_r15, sh4_r8);
	sh4_add_imm(buf, sizeof(MonoLMF) - offsetof(MonoLMF, regs) + 4 /* stacked PR. */, sh4_r8);
	sh4_movl_dispRx(buf, sh4_r8, 60, sh4_r15);

	/* Adjust SP to finish the local allocation of new_lmf,
	   so now it will be used to point to the local new_lmf.*/
	sh4_add_imm(buf, -offsetof(MonoLMF, regs), sh4_r15);

	/*
	 * This trampoline is called with the method's parameters, so the
	 * method's address is stored at the end of the previous frame.
	 */

	/* At this point, the stack looks like :
	 *	:          :
	 *	|          |
	 *	|----------|
	 *	|  method  | Caller's frame.
	 *	|==========|
	 *	|    PR    |
	 *	|----------|
	 *	| .regs[]  | Current frame.
	 *	| - - - - -|
	 *	| new_lmf  |
	 *	|----------| <- SP
	 *	:          :
	 */

	/* pseudo-code: new_lmf.method = %Caller_SP[0]; */
	sh4_mov(buf, sh4_r15, sh4_r8);
	sh4_add_imm(buf, sizeof(MonoLMF) + 4 /* stacked PR. */, sh4_r8);
	sh4_movl_indRy(buf, sh4_r8, sh4_r9);
	sh4_movl_dispRx(buf, sh4_r9, offsetof(MonoLMF, method), sh4_r15);

	if (trampoline_type == MONO_TRAMPOLINE_JUMP)
		/* pseudo-code: new_lmf.ip = NULL; */
		sh4_mov_imm(buf, 0, sh4_r10);
	else
		/* pseudo-code: new_lmf.ip = %PR; */
		sh4_sts_PR(buf, sh4_r10);

	sh4_movl_dispRx(buf, sh4_r10, offsetof(MonoLMF, ip), sh4_r15);

	/* The address of the constant pointing to mono_get_lmf_addr() is not
	   known yet, so use a fake instruction. */
	patch1 = buf;
	sh4_sleep(buf);

	/* pseudo-code: new_lmf.lmf_addr = mono_get_lmf_addr(); */
	sh4_jsr_indRx(buf, sh4_r8);
	sh4_nop(buf);

	sh4_mov(buf, sh4_r0, sh4_r11); /* R11 now holds "new_lmf.lmf_addr". */
	sh4_movl_dispRx(buf, sh4_r11, offsetof(MonoLMF, lmf_addr), sh4_r15);

	/*
	 * Insert the new LMF at the beginning of the LMF list.
	 */

	/* pseudo-code: new_lmf.previous_lmf = *(new_lmf.lmf_addr); */
	sh4_movl_indRy(buf, sh4_r11, sh4_r1);
	sh4_movl_dispRx(buf, sh4_r1, offsetof(MonoLMF, previous_lmf), sh4_r15);

	/* pseudo-code: *(new_lmf.lmf_addr) = &new_lmf;  */
	sh4_movl_indRx(buf, sh4_r15, sh4_r11);

	/* Fill parameters passed to the trampoline. */
	sh4_mov(buf, sh4_r15, sh4_r4);
	sh4_add_imm(buf, offsetof(MonoLMF, regs), sh4_r4);
	sh4_mov(buf, sh4_r10, sh4_r5); /* R10 is currently used as "new_lmf.ip". */
	sh4_mov(buf, sh4_r9, sh4_r6);  /* R9 is currently used as "new_lmf.method". */
	sh4_mov_imm(buf, 0, sh4_r7);

	/* The address of the constant pointing to trampoline() is not
	   known yet, so use a fake instruction. */
	patch2 = buf;
	sh4_sleep(buf);

	/* pseudo-code: compiled_methode = trampoline(new_lmf.regs, new_lmf.ip, new_lmf.method, NULL); */
	sh4_jsr_indRx(buf, sh4_r8);
	sh4_nop(buf);

	/*
	 * Restore the previous LMF list.
	 */

	/* pseudo-code: *(new_lmf.lmf_addr) = &(new_lmf.previous_lmf); */
	sh4_mov(buf, sh4_r15, sh4_r8);
	sh4_add_imm(buf, offsetof(MonoLMF, previous_lmf), sh4_r8);
	sh4_movl_indRx(buf, sh4_r8, sh4_r11); /* R11 is currently used as "new_lmf.lmf_addr". */

	/*
	 * Restore all registers.
	 */

	sh4_add_imm(buf, offsetof(MonoLMF, regs), sh4_r15);

	/* At this point, the stack looks like :
	 *	:          :
	 *	|          |
	 *	|----------|
	 *	|  method  | Caller's frame.
	 *	|==========|
	 *	|    PR    |
	 *	|----------|
	 *	| .regs[]  | Current frame.
	 *	| - - - - -| <- SP
	 *	| new_lmf  |
	 *	|----------|
	 *	:          :
	 */

	/* pseudo-code: { %R1, ..., %R14 } = new_lmf.regs[]; */
	/* Do not restore R0 and R15 because there are used later. */
	for (i = 1; i < 15; i++)
		sh4_movl_dispRy(buf, i * 4, sh4_r15, (SH4IntRegister)i);

	/* pseudo-code: %PR = %Caller_PR; */
	sh4_add_imm(buf, sizeof(MonoLMF) - offsetof(MonoLMF, regs), sh4_r15);
	sh4_ldsl_incRx_PR(buf, sh4_r15);

	/*
	 * Remove the method's address from the previous frame.
	 */

	/* pseudo-code: %SP = %SP + 1; */
	sh4_add_imm(buf, 4, sh4_r15); /* method */

	/* At this point, the stack looks like :
	 *	:          :
	 *	|          |
	 *	|----------| <- SP
	 *	|  method  | Caller's frame.
	 *	|==========|
	 *	:          : Current frame.
	 */

	if (trampoline_type != MONO_TRAMPOLINE_CLASS_INIT)
		/* pseudo-code: goto compiled_methode; */
		/* R0 is the result from the call to trampoline(). */
		sh4_jmp_indRx(buf, sh4_r0);
	else
		/* pseudo-code: return; */
		sh4_rts(buf);
	sh4_nop(buf);

	/* Align the constant pool. */
	while (((guint32)buf % 4) != 0)
		sh4_nop(buf);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_dispPC(patch1, (guint32)buf - (((guint32)patch1 + 4) & ~0x3), sh4_r8);
	sh4_emit32(buf, (int)mono_get_lmf_addr);

	sh4_movl_dispPC(patch2, (guint32)buf - (((guint32)patch2 + 4) & ~0x3), sh4_r8);
	sh4_emit32(buf, (int)mono_get_trampoline_func(trampoline_type));

	/* Sanity checks. */
	g_assert(buf - code <= TRAMPOLINE_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, TRAMPOLINE_SIZE);

	return code;
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
