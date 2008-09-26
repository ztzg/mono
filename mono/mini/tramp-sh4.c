/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * tramp-sh4.c: JIT trampoline code for SH4
 *
 * Authors:
 *   Cedric VINCENT (cedric.vincent@gmail.com)
 *
 * (C) 2008 STMicroelectronics.
 */

#include <glib.h>

#include "mini.h"
#include "mono/arch/sh4/sh4-codegen.h"

gpointer mono_arch_create_rgctx_lazy_fetch_trampoline(guint32 slot)
{
	/* TODO - CV */
	g_assert(0);
	return NULL;
}

/**
 * Create the stub used to transfer control to a specified trampoline.
 */
gpointer mono_arch_create_specific_trampoline(gpointer methode2compile, MonoTrampolineType trampoline_type, MonoDomain *domain, guint32 *code_length)
{
	int short_branch = 0;
	guint8 *code   = NULL;
	guint8 *buffer = NULL;
	guint8 *patch1 = NULL;
	guint8 *patch2 = NULL;
	guint8 *trampoline = NULL;

	SH4_DEBUG("args => %p, %d, %p, %p", methode2compile, trampoline_type, domain, code_length);

	trampoline = mono_get_trampoline_code(trampoline_type);

#define SPECIFIC_TRAMPOLINE_SIZE 20

	mono_domain_lock(domain);
	code = buffer = mono_code_manager_reserve(domain->code_mp, SPECIFIC_TRAMPOLINE_SIZE);
	mono_domain_unlock(domain);

	/* Patch slot for : sh4_r0 <- methode2compile */
	patch1 = buffer;
	sh4_ldtlb(buffer);

	/* Push the address of the method to compile onto the stack.
	   The trampoline will automatically pop this "hidden" parameter.
	   TODO - CV : May be I could use a caller-saved register instead ? */
	sh4_movl_decRx(buffer, sh4_r0, sh4_r15);

	/* If possible, branch to the trampoline in an optimized way,
	   that is, without the need of the constant pool. */
	if (short_branch != 0) {
		NOT_IMPLEMENTED;
		sh4_bra(buffer, 0 /* Fake value. */);
	} else {
		/* Patch slot for : sh4_r0 <- trampoline */
		patch2 = buffer;
		sh4_ldtlb(buffer);

		sh4_jmp_indRx(buffer, sh4_r0);
	}
	sh4_nop(buffer);

	/* Align the constant pool. */
	while (((guint32)buffer % 4) != 0)
		sh4_nop(buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_PCrel(patch1, buffer, sh4_r0);
	sh4_emit32(buffer, (guint32)methode2compile);

	if (short_branch == 0) {
		sh4_movl_PCrel(patch2, buffer, sh4_r0);
		sh4_emit32(buffer, (guint32)trampoline);
	}

	/* Sanity checks. */
	g_assert(buffer - code <= SPECIFIC_TRAMPOLINE_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, SPECIFIC_TRAMPOLINE_SIZE);

	if (code_length != NULL) {
		*code_length = buffer - code;
		SH4_DEBUG("code_length = %d", *code_length);
	}

	SH4_DEBUG("code = %p", code);

	return code;
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
 * 	new_lmf.registers[] = { %R0, ..., %R15 };
 * 
 * 	struct MonoLMF new_lmf;
 * 
 * 	// This trampoline is called with the method's parameters, so the
 * 	// method's address is stored at the end of the previous frame.
 * 	new_lmf.method = %Caller_SP[0];
 * 
 * #if trampoline_type == MONO_TRAMPOLINE_JUMP
 * 	new_lmf.pc = NULL;
 * #else
 * 	new_lmf.pc = %PR;
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
 * 	compiled_methode = trampoline(new_lmf.registers, new_lmf.pc, new_lmf.method, NULL);
 * 
 * 	// Restore the previous LMF list.
 * 	*(new_lmf.lmf_addr) = &(new_lmf.previous_lmf);
 * 
 * 	// Restore almost all registers.
 * 	{ %R1, ..., %R14 } = new_lmf.registers[];
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
	int i = 0;
	guint8 *code   = NULL;
	guint8 *buffer = NULL;
	guint8 *patch1 = NULL;
	guint8 *patch2 = NULL;

	SH4_DEBUG("args => %d", trampoline_type);

#define TRAMPOLINE_SIZE 148

	code = buffer = mono_global_codeman_reserve(TRAMPOLINE_SIZE);

	/* Save the return address. */
	sh4_stsl_PR_decRx(buffer, sh4_r15);

	/* pseudo-code: struct MonoLMF new_lmf; */
	sh4_add_imm(buffer, -sizeof(MonoLMF) + offsetof(MonoLMF, registers), sh4_r15);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|              |
	 *	|--------------|
	 *	|    method    | Caller's frame.
	 *	|==============|
	 *	|      PR      |
	 *	|--------------|
	 *	| .registers[] | Current frame.
	 *	| - - - - - - -| <- SP
	 *	|   new_lmf    |
	 *	|--------------|
	 *	:              :
	 */

	/*
	 * Save all registers.
	 */

	/* pseudo-code: new_lmf.registers[] = { %R0, ..., %R15 }; */
	for (i = 0; i <= 14; i++)
		sh4_movl_dispRx(buffer, (SH4IntRegister)i, i * 4, sh4_r15);

	/* Compute the previous value of SP before saving into new_lmf.registers[]. */
	sh4_mov(buffer, sh4_r15, sh4_r8);
	sh4_add_imm(buffer, sizeof(MonoLMF) - offsetof(MonoLMF, registers) + 4 /* stacked PR. */, sh4_r8);
	sh4_movl_dispRx(buffer, sh4_r8, 60, sh4_r15);

	/* Adjust SP to finish the local allocation of new_lmf,
	   so now it will be used to point to the local new_lmf.*/
	sh4_add_imm(buffer, -offsetof(MonoLMF, registers), sh4_r15);

	/*
	 * This trampoline is called with the method's parameters, so the
	 * method's address is stored at the end of the previous frame.
	 */

	/* At this point, the stack looks like :
	 *	:              :
	 *	|              |
	 *	|--------------|
	 *	|    method    | Caller's frame.
	 *	|==============|
	 *	|      PR      |
	 *	|--------------|
	 *	| .registers[] | Current frame.
	 *	| - - - - - - -|
	 *	|   new_lmf    |
	 *	|--------------| <- SP
	 *	:              :
	 */

	/* pseudo-code: new_lmf.method = %Caller_SP[0]; */
	sh4_mov(buffer, sh4_r15, sh4_r8);
	sh4_add_imm(buffer, sizeof(MonoLMF) + 4 /* stacked PR. */, sh4_r8);
	sh4_movl_indRy(buffer, sh4_r8, sh4_r9);
	sh4_movl_dispRx(buffer, sh4_r9, offsetof(MonoLMF, method), sh4_r15);

	if (trampoline_type == MONO_TRAMPOLINE_JUMP)
		/* pseudo-code: new_lmf.pc = NULL; */
		sh4_mov_imm(buffer, 0, sh4_r10);
	else
		/* pseudo-code: new_lmf.pc = %PR; */
		sh4_sts_PR(buffer, sh4_r10);

	sh4_movl_dispRx(buffer, sh4_r10, offsetof(MonoLMF, pc), sh4_r15);

	/* Patch slot for : sh4_r8 <- mono_get_lmf_addr */
	patch1 = buffer;
	sh4_ldtlb(buffer);

	/* pseudo-code: new_lmf.lmf_addr = mono_get_lmf_addr(); */
	sh4_jsr_indRx(buffer, sh4_r8);
	sh4_nop(buffer);

	sh4_mov(buffer, sh4_r0, sh4_r11); /* R11 now holds "new_lmf.lmf_addr". */
	sh4_movl_dispRx(buffer, sh4_r11, offsetof(MonoLMF, lmf_addr), sh4_r15);

	/*
	 * Insert the new LMF at the beginning of the LMF list.
	 */

	/* pseudo-code: new_lmf.previous_lmf = *(new_lmf.lmf_addr); */
	sh4_movl_indRy(buffer, sh4_r11, sh4_r1);
	sh4_movl_dispRx(buffer, sh4_r1, offsetof(MonoLMF, previous_lmf), sh4_r15);

	/* pseudo-code: *(new_lmf.lmf_addr) = &new_lmf;  */
	sh4_movl_indRx(buffer, sh4_r15, sh4_r11);

	/* Fill parameters passed to the trampoline. */
	sh4_mov(buffer, sh4_r15, sh4_r4);
	sh4_add_imm(buffer, offsetof(MonoLMF, registers), sh4_r4);
	sh4_mov(buffer, sh4_r10, sh4_r5); /* R10 is currently used as "new_lmf.pc". */
	sh4_mov(buffer, sh4_r9, sh4_r6);  /* R9 is currently used as "new_lmf.method". */
	sh4_mov_imm(buffer, 0, sh4_r7);

	/* Patch slot for : sh4_r8 <- trampoline */
	patch2 = buffer;
	sh4_ldtlb(buffer);

	/* pseudo-code: compiled_methode = trampoline(new_lmf.registers, new_lmf.pc, new_lmf.method, NULL); */
	sh4_jsr_indRx(buffer, sh4_r8);
	sh4_nop(buffer);

	/*
	 * Restore the previous LMF list.
	 */

	/* pseudo-code: *(new_lmf.lmf_addr) = &(new_lmf.previous_lmf); */
	sh4_mov(buffer, sh4_r15, sh4_r8);
	sh4_add_imm(buffer, offsetof(MonoLMF, previous_lmf), sh4_r8);
	sh4_movl_indRx(buffer, sh4_r8, sh4_r11); /* R11 is currently used as "new_lmf.lmf_addr". */

	/*
	 * Restore all registers.
	 */

	sh4_add_imm(buffer, offsetof(MonoLMF, registers), sh4_r15);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|              |
	 *	|--------------|
	 *	|    method    | Caller's frame.
	 *	|==============|
	 *	|      PR      |
	 *	|--------------|
	 *	| .registers[] | Current frame.
	 *	| - - - - - - -| <- SP
	 *	|   new_lmf    |
	 *	|--------------|
	 *	:              :
	 */

	/* pseudo-code: { %R1, ..., %R14 } = new_lmf.registers[]; */
	/* Do not restore R0 and R15 because there are used later. */
	for (i = 1; i <= 14; i++)
		sh4_movl_dispRy(buffer, i * 4, sh4_r15, (SH4IntRegister)i);

	/* pseudo-code: %PR = %Caller_PR; */
	sh4_add_imm(buffer, sizeof(MonoLMF) - offsetof(MonoLMF, registers), sh4_r15);
	sh4_ldsl_incRx_PR(buffer, sh4_r15);

	/*
	 * Remove the method's address from the previous frame.
	 */

	/* pseudo-code: %SP = %SP + 1; */
	sh4_add_imm(buffer, 4, sh4_r15); /* method */

	/* At this point, the stack looks like :
	 *	:              :
	 *	|              |
	 *	|--------------| <- SP
	 *	|    method    | Caller's frame.
	 *	|==============|
	 *	:              : Current frame.
	 */

	if (trampoline_type != MONO_TRAMPOLINE_CLASS_INIT)
		/* pseudo-code: goto compiled_methode; */
		/* R0 is the result from the call to trampoline(). */
		sh4_jmp_indRx(buffer, sh4_r0);
	else
		/* pseudo-code: return; */
		sh4_rts(buffer);
	sh4_nop(buffer);

	/* Align the constant pool. */
	while (((guint32)buffer % 4) != 0)
		sh4_nop(buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_PCrel(patch1, buffer, sh4_r8);
	sh4_emit32(buffer, (guint32)mono_get_lmf_addr);

	sh4_movl_PCrel(patch2, buffer, sh4_r8);
	sh4_emit32(buffer, (guint32)mono_get_trampoline_func(trampoline_type));

	/* Sanity checks. */
	g_assert(buffer - code <= TRAMPOLINE_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, TRAMPOLINE_SIZE);

	SH4_DEBUG("code = %p", code);

	return code;
}

guint32 mono_arch_get_rgctx_lazy_fetch_offset(gpointer *registers)
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

void mono_arch_nullify_class_init_trampoline(guint8 *code, gssize *registers)
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
