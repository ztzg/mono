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

#define sh4_emit(offset, ...) do {			\
		printf("\t");				\
		printf(__VA_ARGS__);			\
		printf("\t!; @%d\n", offset);		\
		offset+=2;				\
	} while(0)

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
	g_assert(0);
#if 0
	int offset = 0;
	int patch1 = 0;
	int patch2 = 0;
	int i = 0;

	/* WIP guint8 *code = NULL; guint8 *buf = NULL; */
	/* WIP code = buf = mono_global_codeman_reserve(XXX); */

	printf("\t.text\n");
	printf("\t.align 1\n");
	printf("\t.global trampoline_code\n");
	printf("trampoline_code:\n");

	/* Save the return address. */
	sh4_emit(offset, "sts.l	pr, @-r15");

	/* pseudo-code: struct MonoLMF new_lmf; */
	sh4_emit(offset, "add	#%d, r15", -sizeof(MonoLMF) + offsetof(MonoLMF, regs));

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
		sh4_emit(offset, "mov.l	r%d, @(%d, r15)", i, i * 4);

	/* Compute the previous SP value before saving into new_lmf.regs[]. */
	sh4_emit(offset, "mov	r15, r8");
	sh4_emit(offset, "add	#%d, r8", sizeof(MonoLMF) - offsetof(MonoLMF, regs) + 4 /* stacked PR. */);
	sh4_emit(offset, "mov.l	r8, @(60, r15)");

	/* Adjust SP to finish the local allocation of new_lmf,
	   so now it will be used to point to the local new_lmf.*/
	sh4_emit(offset, "add	#%d, r15", -offsetof(MonoLMF, regs));

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
	sh4_emit(offset, "mov	r15, r8");
	sh4_emit(offset, "add	#%d, r8", sizeof(MonoLMF) + 4 /* stacked PR. */);
	sh4_emit(offset, "mov.l	@r8, r9");
	sh4_emit(offset, "mov.l	r9, @(%d, r15)", offsetof(MonoLMF, method));

	if (trampoline_type == MONO_TRAMPOLINE_JUMP) {
		/* pseudo-code: new_lmf.ip = NULL; */
		sh4_emit(offset, "mov	#0, r8");
	} else {
		/* pseudo-code: new_lmf.ip = %PR; */
		sh4_emit(offset, "sts	pr, r8");
	}
	sh4_emit(offset, "mov.l	r8, @(%d, r15)", offsetof(MonoLMF, ip));

	/* The address of the constant pointing to mono_get_lmf_addr() is not
	   known yet, so use a fake instruction. */
	patch1 = offset;
	sh4_emit(offset, "sleep");

	/* pseudo-code: new_lmf.lmf_addr = mono_get_lmf_addr(); */
	sh4_emit(offset, "jsr	@r8");
	sh4_emit(offset, "nop");

	sh4_emit(offset, "mov.l	r0, @(%d, r15)", offsetof(MonoLMF, lmf_addr));

	/*
	 * Insert the new LMF at the beginning of the LMF list.
	 */

	/* pseudo-code: new_lmf.previous_lmf = *(new_lmf.lmf_addr); */
	sh4_emit(offset, "mov.l	@r0, r1");
	sh4_emit(offset, "mov.l	r1, @(%d, r15)", offsetof(MonoLMF, previous_lmf));

	/* pseudo-code: *(new_lmf.lmf_addr) = &new_lmf;  */
	sh4_emit(offset, "mov.l	r15, @r0");

	/* Fill parameters passed to the trampoline. */
	sh4_emit(offset, "mov	r15, r4");
	sh4_emit(offset, "add	#%d, r4", offsetof(MonoLMF, regs));
	sh4_emit(offset, "mov.l	@(%d, r15), r5", offsetof(MonoLMF, ip));
	sh4_emit(offset, "mov.l	@(%d, r15), r6", offsetof(MonoLMF, method));
	sh4_emit(offset, "mov	#0, r7");

	/* The address of the constant pointing to trampoline() is not
	   known yet, so use a fake instruction. */
	patch2 = offset;
	sh4_emit(offset, "sleep");

	/* pseudo-code: compiled_methode = trampoline(new_lmf.regs, new_lmf.ip, new_lmf.method, NULL); */
	sh4_emit(offset, "jsr	@r8");
	sh4_emit(offset, "nop");

	/*
	 * Restore the previous LMF list.
	 */

	/* pseudo-code: *(new_lmf.lmf_addr) = &(new_lmf.previous_lmf); */
	sh4_emit(offset, "mov	r15, r8");
	sh4_emit(offset, "add	#%d, r8", offsetof(MonoLMF, previous_lmf));
	sh4_emit(offset, "mov.l	@(%d, r15), r9", offsetof(MonoLMF, lmf_addr));
	sh4_emit(offset, "mov.l	r8, @r9");

	/*
	 * Restore all registers.
	 */

	sh4_emit(offset, "add	#%d, r15", offsetof(MonoLMF, regs));

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
		sh4_emit(offset, "mov.l	@(%d, r15), r%d", i * 4, i);

	/* pseudo-code: %PR = %Caller_PR; */
	sh4_emit(offset, "add	#%d, r15", sizeof(MonoLMF) - offsetof(MonoLMF, regs));
	sh4_emit(offset, "lds.l	@r15+,pr");

	/*
	 * Remove the method's address from the previous frame.
	 */

	/* pseudo-code: %SP = %SP + 1; */
	sh4_emit(offset, "add	#4, r15"); /* method */

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
		sh4_emit(offset, "jmp @r0");
	else
		/* pseudo-code: return; */
		sh4_emit(offset, "rts");
	sh4_emit(offset, "nop");

	/* Align the constant pool. */
	while ((offset % 4) != 0)
		sh4_emit(offset, "nop");

	/* Build the constant pool & patch the corresponding instructions. */
	assert(offset - patch1 < (2 << 8) - 1);
	sh4_emit(patch1, "!; mov.l	@(%d, pc), r8", offset - patch1);
	sh4_emit(offset, ".long	mono_get_lmf_addr"); offset+=2;

	assert(offset - patch2 < (2 << 8) - 1);
	sh4_emit(patch2, "!; mov.l	@(%d, pc), r8", offset - patch2);
	sh4_emit(offset, ".long	trampoline_func"); offset+=2;

	/* Sanity checks. */
	/* WIP g_assert((buf - code) <= TRAMPOLINE_SIZE); */

	/* Flush instruction cache, since we've generated code. */
	/* WIP mono_arch_flush_icache(code, buf - code); */
#endif
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
