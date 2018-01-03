/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * tramp-sh4.c: JIT trampoline code for SH4
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
 *   Denis FERRANTI (denis.ferranti@st.com)
 *   Yvan Roux (yvan.roux@st.com)
 */

#include <glib.h>

#include <mono/metadata/abi-details.h>

#include "mini.h"

#include "mono/arch/sh4/sh4-codegen.h"

guint8*
mono_sh4_emit_load_aotconst (guint8 *start, guint8 *code,
			     MonoJumpInfo **ji, MonoJumpInfoType tramp_type,
			     gconstpointer target, SH4IntRegister dest);

/**
 * Create the stub used to transfer control to a specified trampoline.
 */
gpointer 
mono_arch_create_specific_trampoline(gpointer methode2compile, MonoTrampolineType trampoline_type, MonoDomain *domain, guint32 *code_length)
{
	int short_branch = 0;
	guint8 *code   = NULL;
	guint8 *buffer = NULL;
	guint8 *patch1 = NULL;
	guint8 *patch2 = NULL;
	guint8 *trampoline = NULL;

	SH4_EXTRA_DEBUG("args => %p, %d, %p, %p", methode2compile, trampoline_type, domain, code_length);

	trampoline = mono_get_trampoline_code(trampoline_type);

#define SPECIFIC_TRAMPOLINE_SIZE 22

	mono_domain_lock(domain);
	code = buffer = mono_code_manager_reserve(domain->code_mp, SPECIFIC_TRAMPOLINE_SIZE);
	mono_domain_unlock(domain);

	/* Patch slot for : sh4_temp <- methode2compile */
	patch1 = buffer;
	sh4_die(&buffer);

	/* Push the address of the method to compile onto the stack.
	   The trampoline will automatically pop this "hidden" parameter. */
	sh4_movl_decRx(&buffer, sh4_temp, sh4_sp);

	/* If possible, branch to the trampoline in an optimized way,
	   that is, without the need of the constant pool. */
	if (short_branch != 0) {
		g_assert_not_reached();
		sh4_bra(&buffer, 0 /* Fake value. */);
	} else {
		/* Patch slot for : sh4_temp <- trampoline */
		patch2 = buffer;
		sh4_die(&buffer);

		sh4_jmp_indRx(&buffer, sh4_temp);
	}
	sh4_nop(&buffer);

	/* Should return elsewhere. */
	sh4_die(&buffer);

	/* Align the constant pool. */
	while (((guint32)buffer % 4) != 0)
		sh4_nop(&buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_PCrel(&patch1, buffer, sh4_temp);
	sh4_emit32(&buffer, (guint32)methode2compile);

	if (short_branch == 0) {
		sh4_movl_PCrel(&patch2, buffer, sh4_temp);
		sh4_emit32(&buffer, (guint32)trampoline);
	}

	/* Sanity checks. */
	g_assert(buffer - code <= SPECIFIC_TRAMPOLINE_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, SPECIFIC_TRAMPOLINE_SIZE);

	if (code_length != NULL) {
		*code_length = buffer - code;
		SH4_EXTRA_DEBUG("code_length = %d", *code_length);
	}

	SH4_EXTRA_DEBUG("code = %p", code);

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
 * 	new_lmf.registers[]  = { %R0, ..., %R15 };
 * 	new_lmf.fregisters[] = { %FR0, ..., %FR15 };
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
 * 	*(new_lmf.lmf_addr) = new_lmf.previous_lmf;
 * 
 * 	// Restore almost all registers.
 * 	{ %FR0, ..., %FR15 } = new_lmf.fregisters[];
 * 	{ %R1, ..., %R14 }   = new_lmf.registers[];
 * 	%PR = %Caller_PR;
 * 
 * 	// Remove the method's address from the previous frame.
 * 	%SP = %SP + 1;
 * 
 * 	goto compiled_methode;
 * }
 */
guchar * 
mono_arch_create_generic_trampoline (MonoTrampolineType trampoline_type, MonoTrampInfo **info, gboolean aot)
{
	int i = 0;
	guint8 *code   = NULL;
	guint8 *buffer = NULL;
	guint8 *patch1 = NULL;
	guint8 *patch2 = NULL;
	MonoJumpInfo *ji = NULL;

	SH4_EXTRA_DEBUG("args => %d", trampoline_type);

	if (info)
		*info = NULL;

#define TRAMPOLINE_SIZE 220

	code = buffer = mono_global_codeman_reserve(TRAMPOLINE_SIZE);

	/* Save the return address. */
	sh4_stsl_PR_decRx(&buffer, sh4_sp);

	/* pseudo-code: struct MonoLMF new_lmf; */
	sh4_multi_add_imm(&buffer, -sizeof(MonoLMF), sh4_sp);

	/* At this point, the stack looks like :
	 *	:              :
	 *	|              |
	 *	|--------------|
	 *	|    method    | Caller's frame.
	 *	|==============|
	 *	|      PR      |
	 *	|--------------| Current frame.
	 *	|              |
	 *	|   new_lmf    |
	 *	|              |
	 *	|--------------| <- SP
	 *	:              :
	 */

	/*
	 * Save all registers.
	 */

	sh4_mov(&buffer, sh4_sp, sh4_temp);
	sh4_add_imm(&buffer, offsetof(MonoLMF, registers), sh4_temp);
	/* pseudo-code: new_lmf.registers[] = { %R0, ..., %R15 }; */
	for (i = 0; i < MONO_MAX_IREGS; i++)
		if (i != sh4_temp)
			sh4_movl_dispRx(&buffer, (SH4IntRegister)i, i * 4, sh4_temp);

	sh4_multi_add_imm(&buffer, -offsetof(MonoLMF, registers) +
				   offsetof(MonoLMF, fregisters) +
				   MONO_MAX_FREGS * sizeof(guint32), sh4_temp);
	/* pseudo-code: new_lmf.fregisters[] = { %FR0, ..., %FR15 }; */
	for (i = MONO_MAX_FREGS - 1; i >= 0; i--)
		sh4_fmov_decRx(&buffer, (SH4FloatRegister)i, sh4_temp);

	/*
	 * This trampoline is called with the method's parameters, so the
	 * method's address is stored at the end of the previous frame.
	 */

	/* pseudo-code: new_lmf.method = %Caller_SP[0]; */
	sh4_mov(&buffer, sh4_sp, sh4_r8);
	sh4_multi_add_imm(&buffer, sizeof(MonoLMF) + 4 /* stacked PR. */, sh4_r8);
	sh4_movl_indRy(&buffer, sh4_r8, sh4_r9);
	sh4_movl_dispRx(&buffer, sh4_r9, offsetof(MonoLMF, method), sh4_sp);

	if (trampoline_type == MONO_TRAMPOLINE_JUMP)
		/* pseudo-code: new_lmf.pc = NULL; */
		sh4_mov_imm(&buffer, 0, sh4_r10);
	else
		/* pseudo-code: new_lmf.pc = %PR; */
		sh4_sts_PR(&buffer, sh4_r10);

	sh4_movl_dispRx(&buffer, sh4_r10, offsetof(MonoLMF, pc), sh4_sp);

	/* Patch slot for : sh4_r8 <- mono_get_lmf_addr */
	patch1 = buffer;
	sh4_die(&buffer);

	/* pseudo-code: new_lmf.lmf_addr = mono_get_lmf_addr(); */
	sh4_jsr_indRx(&buffer, sh4_r8);
	sh4_nop(&buffer);

	sh4_mov(&buffer, sh4_r0, sh4_r11); /* R11 now holds "new_lmf.lmf_addr". */
	sh4_movl_dispRx(&buffer, sh4_r11, offsetof(MonoLMF, lmf_addr), sh4_sp);

	/*
	 * Insert the new LMF at the beginning of the LMF list.
	 */

	/* pseudo-code: new_lmf.previous_lmf = *(new_lmf.lmf_addr); */
	sh4_movl_indRy(&buffer, sh4_r11, sh4_r1);
	sh4_movl_dispRx(&buffer, sh4_r1, offsetof(MonoLMF, previous_lmf), sh4_sp);

	/* pseudo-code: *(new_lmf.lmf_addr) = &new_lmf;  */
	sh4_movl_indRx(&buffer, sh4_sp, sh4_r11);

	/* Fill parameters passed to the trampoline. */
	sh4_mov(&buffer, sh4_sp, sh4_r4);
	sh4_add_imm(&buffer, offsetof(MonoLMF, registers), sh4_r4);
#if 0 /* Mono trampolines does not need floating-point registers. */
	sh4_mov(&buffer, sh4_sp, sh4_r5);
	sh4_add_imm(&buffer, offsetof(MonoLMF, fregisters), sh4_r5);
	sh4_mov(&buffer, sh4_r10, sh4_r6); /* R10 is currently used as "new_lmf.pc". */
	sh4_mov(&buffer, sh4_r9, sh4_r7);  /* R9 is currently used as "new_lmf.method". */
#else
	sh4_mov(&buffer, sh4_r10, sh4_r5); /* R10 is currently used as "new_lmf.pc". */
	sh4_mov(&buffer, sh4_r9, sh4_r6);  /* R9 is currently used as "new_lmf.method". */
#endif

	/* The last parameter is in fact not used, however it eases debug
	   session when breaking on mono_magic_trampoline(). */
	sh4_mova_dispPC_R0(&buffer, 0);
	sh4_mov(&buffer, sh4_r0, sh4_r7);

	/* Patch slot for : sh4_r8 <- trampoline */
	patch2 = buffer;
	sh4_die(&buffer);

	/* pseudo-code: compiled_methode = trampoline(new_lmf.registers, new_lmf.pc, new_lmf.method, NULL); */
	sh4_jsr_indRx(&buffer, sh4_r8);
	sh4_nop(&buffer);

	/*
	 * Restore the previous LMF list.
	 */

	/* pseudo-code: *(new_lmf.lmf_addr) = new_lmf.previous_lmf; */
	sh4_movl_dispRy(&buffer, offsetof(MonoLMF, previous_lmf), sh4_sp, sh4_r8);
	sh4_movl_indRx(&buffer, sh4_r8, sh4_r11); /* R11 is currently used as "new_lmf.lmf_addr". */

	/*
	 * Restore all registers.
	 */

	/* At this point, the stack looks like :
	 *	:              :
	 *	|              |
	 *	|--------------|
	 *	|    method    | Caller's frame.
	 *	|==============|
	 *	|      PR      |
	 *	|--------------| Current frame.
	 *	|              |
	 *	|    new_lmf   |
	 *	|              |
	 *	|--------------| <- SP
	 *	:              :
	 */

	sh4_mov(&buffer, sh4_sp, sh4_temp);
	sh4_add_imm(&buffer, offsetof(MonoLMF, registers), sh4_temp);

	/* pseudo-code: { %R1, ..., %R14 } = new_lmf.registers[]; */
	/* R15 is retored later and R0 holds the result of trampoline(). */
	for (i = 1; i < MONO_MAX_IREGS - 1; i++)
		if (i != sh4_temp)
			sh4_movl_dispRy(&buffer, i * 4, sh4_temp, (SH4IntRegister)i);

	sh4_add_imm(&buffer, -offsetof(MonoLMF, registers) + offsetof(MonoLMF, fregisters), sh4_temp);
	/* pseudo-code: { %FR0, ..., %FR15 } = new_lmf.fregisters[]; */
	for (i = 0; i < MONO_MAX_FREGS; i++)
		sh4_fmov_incRy(&buffer, sh4_temp, (SH4FloatRegister)i);

	/* pseudo-code: %PR = %Caller_PR; */
	sh4_multi_add_imm(&buffer, sizeof(MonoLMF), sh4_sp);
	sh4_ldsl_incRx_PR(&buffer, sh4_sp);

	/*
	 * Remove the method's address from the previous frame.
	 */

	/* pseudo-code: %SP = %SP + 1; */
	sh4_add_imm(&buffer, 4, sh4_sp); /* method */

	/* At this point, the stack looks like :
	 *	:              :
	 *	|              |
	 *	|--------------| <- SP
	 *	|    method    | Caller's frame.
	 *	|==============|
	 *	:              : Current frame.
	 */

	if (MONO_TRAMPOLINE_TYPE_MUST_RETURN (trampoline_type)) {
		sh4_rts (&buffer);
	} else {
		/* pseudo-code: goto compiled_methode; */
		/* Rtemp is the result from the call to trampoline(). */
		sh4_jmp_indRx(&buffer, sh4_r0);
	}
	sh4_nop(&buffer);

	/* Align the constant pool. */
	while (((guint32)buffer % 4) != 0)
		sh4_nop(&buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_PCrel(&patch1, buffer, sh4_r8);
	if (aot) {
		buffer = mono_arch_emit_load_aotconst (code, buffer, &ji, MONO_PATCH_INFO_JIT_ICALL_ADDR, "mono_get_lmf_addr");
	} else {
		sh4_emit32(&buffer, (guint32)mono_get_lmf_addr);
	}

	sh4_movl_PCrel(&patch2, buffer, sh4_r8);
	if (aot) {
		buffer = mono_arch_emit_load_aotconst (code, buffer, &ji, MONO_PATCH_INFO_JIT_ICALL_ADDR, g_strdup_printf ("trampoline_func_%d", trampoline_type));
	} else {
		sh4_emit32(&buffer, (guint32)mono_get_trampoline_func(trampoline_type));
	}

	/* Sanity checks. */
	g_assert(buffer - code <= TRAMPOLINE_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, TRAMPOLINE_SIZE);

	SH4_EXTRA_DEBUG("code = %p", code);

	return code;
}

/**
 * When value type methods are called through the vtable we need to
 * unbox the this argument. This method returns a pointer to a
 * trampoline which does unboxing before calling the method. Remember
 * this trampoline executes in the context of the caller.
 */
gpointer 
mono_arch_get_unbox_trampoline(MonoMethod *method, gpointer address)
{
	guint8 *code   = NULL;
	guint8 *buffer = NULL;
	guint8 *patch0 = NULL;
	MonoDomain *domain = mono_domain_get();

#define UNBOX_TRAMPOLINE_SIZE 14

	mono_domain_lock(domain);
	code = buffer = mono_code_manager_reserve(domain->code_mp, UNBOX_TRAMPOLINE_SIZE);
	mono_domain_unlock(domain);

	/* Adjust 'this' by the size of MonoObject. */
	sh4_add_imm(&buffer, sizeof(MonoObject), MONO_SH4_REG_FIRST_ARG);

	/* TODO - CV : optimize with sh4_bsr if possible. */

	/* Patch slot for : sh4_temp <- address */
	patch0 = buffer;
	sh4_die(&buffer);

	/* Finally, call the method. */
	sh4_jmp_indRx(&buffer, sh4_temp);
	sh4_nop(&buffer);

	/* Align the constant pool. */
	while (((guint32)buffer % 4) != 0)
		sh4_nop(&buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_PCrel(&patch0, buffer, sh4_temp);
	sh4_emit32(&buffer, (guint32)address);

	/* Sanity checks. */
	g_assert(buffer - code <= UNBOX_TRAMPOLINE_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, UNBOX_TRAMPOLINE_SIZE);

	return code;
}

/*
 * mono_arch_get_static_rgctx_trampoline:
 *
 *   Create a trampoline which sets RGCTX_REG to MRGCTX, then jumps to ADDR.
 */
gpointer
mono_arch_get_static_rgctx_trampoline (MonoMethod *m, MonoMethodRuntimeGenericContext *mrgctx, gpointer addr)
{
	guint8 *code, *start, *patch_load_mrgctx, *patch_load_addr;
	/* TODO(ddiederen): DWARF. */
	/* GSList *unwind_ops; */
	int buf_len = 18;
	MonoDomain *domain = mono_domain_get ();

	start = code = mono_domain_code_reserve (domain, buf_len);

	/* 0: MONO_ARCH_RGCTX_REG <- mrgctx */
	patch_load_mrgctx = code;
	sh4_die (&code);

	/* 2: sh4_temp <- addr */
	patch_load_addr = code;
	sh4_die (&code);

	/* 4: Jump */
	sh4_jmp_indRx (&code, sh4_temp);
	sh4_nop (&code);

	/* 8: Align the constant pool. */
	while (((guint32)code % 4) != 0)
		sh4_nop (&code);

	/* 8-10: mrgctx */
	sh4_movl_PCrel (&patch_load_mrgctx, code, MONO_ARCH_RGCTX_REG);
	sh4_emit32 (&code, (guint32)mrgctx);

	/* 12-14: addr */
	sh4_movl_PCrel (&patch_load_addr, code, sh4_temp);
	sh4_emit32 (&code, (guint32)addr);

	/* 16-18: end */
	g_assert ((code - start) < buf_len);

	mono_arch_flush_icache (start, code - start);

	mono_profiler_code_buffer_new (start, code - start, MONO_PROFILER_CODE_BUFFER_GENERICS_TRAMPOLINE, NULL);

	mono_tramp_info_register (mono_tramp_info_create (NULL, start, code - start, NULL, NULL), domain);

	return start;
}

gpointer
mono_arch_create_rgctx_lazy_fetch_trampoline (guint32 slot, MonoTrampInfo **info, gboolean aot)
{
	guint8 *tramp;
	guint8 *code, *buf;
	guint8 *patch_load_offset = NULL, *patch_load_tramp = NULL;
	guint8 **rgctx_null_jumps;
	int tramp_size;
	int depth, index;
	int i, njumps, offset;
	gboolean mrgctx;
	MonoJumpInfo *ji = NULL;
	/* TODO(ddiederen): DWARF. */
	GSList *unwind_ops = NULL;
	SH4IntRegister chain_reg;

	mrgctx = MONO_RGCTX_SLOT_IS_MRGCTX (slot);
	index = MONO_RGCTX_SLOT_INDEX (slot);
	if (mrgctx)
		index += MONO_SIZEOF_METHOD_RUNTIME_GENERIC_CONTEXT / sizeof (gpointer);
	for (depth = 0; ; ++depth) {
		int size = mono_class_rgctx_get_array_size (depth, mrgctx);

		if (index < size - 1)
			break;
		index -= size - 1;
	}

	tramp_size = 6 + 6 * depth + 14 + 8 + 2 * 4;

	code = buf = mono_global_codeman_reserve (tramp_size);

	rgctx_null_jumps = g_malloc (sizeof (guint8*) * (depth + 2));
	njumps = 0;

	if (mrgctx) {
		/* get mrgctx ptr */
		chain_reg = MONO_SH4_REG_FIRST_ARG;
	} else {
		/* load rgctx ptr from vtable */
		g_assert (SH4_CHECK_RANGE_movl_dispRy (MONO_STRUCT_OFFSET (MonoVTable, runtime_generic_context)));
		sh4_movl_dispRy (&code, (MONO_STRUCT_OFFSET (MonoVTable, runtime_generic_context)), MONO_SH4_REG_FIRST_ARG, sh4_r0);
		chain_reg = sh4_r0;
		/* is the rgctx ptr null? */
		sh4_tst (&code, sh4_r0, sh4_r0);
		/* if yes, jump to actual trampoline */
		rgctx_null_jumps [njumps ++] = code;
		sh4_die (&code);
	}

	for (i = 0; i < depth; ++i) {
		/* load ptr to next array */
		if (mrgctx && i == 0) {
			g_assert (SH4_CHECK_RANGE_movl_dispRy (MONO_SIZEOF_METHOD_RUNTIME_GENERIC_CONTEXT));
			sh4_movl_dispRy (&code, (MONO_SIZEOF_METHOD_RUNTIME_GENERIC_CONTEXT), chain_reg, sh4_r0);
		} else {
			sh4_movl_indRy (&code, chain_reg, sh4_r0);
		}
		chain_reg = sh4_r0;
		/* is the ptr null? */
		sh4_tst (&code, sh4_r0, sh4_r0);
		/* if yes, jump to actual trampoline */
		rgctx_null_jumps [njumps ++] = code;
		sh4_die (&code);
	}

	/* fetch slot */
	offset = sizeof (gpointer) * (index + 1);
	if (SH4_CHECK_RANGE_movl_dispRy (offset)) {
		sh4_movl_dispRy (&code, offset, chain_reg, sh4_r0);
	} else {
		patch_load_offset = code;
		sh4_die (&code);
		sh4_add (&code, chain_reg, sh4_temp);
		sh4_movl_indRy (&code, sh4_temp, sh4_r0);
	}
	chain_reg = sh4_r0;
	/* is the slot null? */
	sh4_tst (&code, sh4_r0, sh4_r0);
	/* if yes, jump to actual trampoline */
	rgctx_null_jumps [njumps ++] = code;
	sh4_die (&code);
	/* otherwise return, result is in r0 */
	sh4_rts (&code);
	sh4_nop (&code); /* Delay slot */

	g_assert (njumps <= depth + 2);
	for (i = 0; i < njumps; ++i)
		sh4_bt_label (&rgctx_null_jumps [i], code);

	g_free (rgctx_null_jumps);

	/* move the rgctx pointer to the VTABLE register */
	sh4_mov (&code, MONO_SH4_REG_FIRST_ARG, MONO_ARCH_VTABLE_REG);

	if (aot) {
		code = mono_sh4_emit_load_aotconst (buf, code, &ji, MONO_PATCH_INFO_JIT_ICALL_ADDR, g_strdup_printf ("specific_trampoline_lazy_fetch_%u", slot), sh4_temp);

		sh4_jmp_indRx (&code, sh4_temp);
		sh4_nop (&code);
	} else {
		tramp = mono_arch_create_specific_trampoline (GUINT_TO_POINTER (slot), MONO_TRAMPOLINE_RGCTX_LAZY_FETCH, mono_get_root_domain (), NULL);

		patch_load_tramp = code;
		sh4_die (&code);
		sh4_jmp_indRx (&code, sh4_temp);
		sh4_nop (&code);
	}

	if (patch_load_offset || patch_load_tramp) {
		/* Align the constant pool. */
		while (((guint32)code % 4) != 0)
			sh4_nop (&code);

		if (patch_load_offset) {
			sh4_movl_PCrel (&patch_load_offset, code, sh4_temp);
			sh4_emit32 (&code, (guint32)mrgctx);
		}

		if (patch_load_tramp) {
			sh4_movl_PCrel (&patch_load_tramp, code, sh4_temp);
			sh4_emit32 (&code, (guint32)tramp);
		}
	}

	mono_arch_flush_icache (buf, code - buf);
	mono_profiler_code_buffer_new (buf, code - buf, MONO_PROFILER_CODE_BUFFER_GENERICS_TRAMPOLINE, NULL);

	g_assert (code - buf <= tramp_size);

	char *name = mono_get_rgctx_fetch_trampoline_name (slot);
	*info = mono_tramp_info_create (name, buf, code - buf, ji, unwind_ops);
	g_free (name);

	return buf;
}

/**
 * Search for and patch the calling sequence pointed to by 'code'
 * so it calls 'address'.
 */
void 
mono_arch_patch_callsite(guint8 *method, guint8 *code, guint8 *address)
{
	guint8 *constant_address = NULL;

	SH4_EXTRA_DEBUG("args => %p, %p, %p", method, code, address);

	constant_address = get_imm_sh4_call_site((void *)code);
	if(constant_address == NULL) {
		char name[] = "call site in patch_callsite";
		mono_disassemble_code(NULL, code - 20, 20, name);
		g_assert_not_reached();
	}

	/* Patch the address. */
	sh4_emit32(&constant_address, (guint32)address);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(constant_address - 4, 4);

	return;
}

void 
mono_arch_patch_plt_entry(guint8 *code, gpointer *got, mgreg_t *regs, guint8 *addr)
{
	/* TODO - CV */
}

guint8*
mono_arch_get_call_target (guint8 *code)
{
	guint32 *target = NULL;
	guint8	opcode;
	gint8	disp;
	
int i;
for (i=-8; i<12; i++) {
 if (i==0) fprintf(stderr,"*");
 fprintf(stderr,"%02x ",code[i]);
}
fprintf(stderr, "\n");
	opcode = code[3] & 0xf0;
	if (opcode == 0xd0) {			// Direct call
		disp = (code[2] + 1) * 4;
		target = code + disp;
fprintf(stderr,"1. call *target: %p ",target);
		target = (guint32 *) *target;
fprintf(stderr,"target: %p\n",target);
	} else {
		opcode = code[-4];
		if (opcode == 0x0b) {		// PLT call
			target = &code[-8];
fprintf(stderr,"2. call *target: %p ",target);
			target = (guint32 *) *target;
fprintf(stderr,"target: %p\n",target);
		} else { 
	fprintf(stderr, "opcode: %02x\n", opcode);
			g_assert(0);
		}
	}
	return target;
}

guint32
mono_arch_get_plt_info_offset (guint8 *plt_entry, mgreg_t *regs, guint8 *code)
{
	/* The offset is stored as the 2nd word of the plt entry */
fprintf (stderr,"plt_off: %08x\n",((guint32*)plt_entry)[4]);
	return ((guint32*)plt_entry) [4];
}
