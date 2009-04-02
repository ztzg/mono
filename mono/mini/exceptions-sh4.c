/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * exceptions-sh4.c: exception support for SH4
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
 */

#include <glib.h>
#include <ucontext.h> /* ucontext_t, */

#include "mini.h"
#include "mono/arch/sh4/sh4-codegen.h"

/**
 * This function is used to gather information from 'ctx'. It returns
 * the MonoJitInfo of the corresponding function, unwinds one stack
 * frame and stores the resulting context into 'new_ctx'. It modifies
 * the 'lmf' if necessary.
 */
MonoJitInfo *mono_arch_find_jit_info(MonoDomain *domain, MonoJitTlsData *jit_tls, MonoJitInfo *result,
				     MonoJitInfo *previous_jit_info, MonoContext *context, MonoContext *new_context,
				     MonoLMF **lmf, gboolean *managed)
{
	MonoJitInfo *jit_info = NULL;
	gpointer pc = (gpointer)context->pc;

	jit_info = mono_jit_info_table_find(domain, pc);

	if (managed != NULL)
		*managed = FALSE;

	if (jit_info != NULL) {
		int i = 0;
		guint stack_offset = 0;
		guint16 *code = NULL;
		guint32 *registers = NULL;

		*new_context = *context;

		if (managed != NULL &&
		    jit_info->method->wrapper_type == 0)
			*managed = TRUE;

		/*
		 * Unwind one stack frame, here comes a typical one:
		 *	:              :
		 *	|==============|
		 *	|  saved reg.  |
		 *	|--------------|
		 *	|  local var.  |
		 *	|--------------| <- FP
		 *	|    MonoLMF   |
		 *	|--------------| <- SP (only if method->save_lmf is set)
		 *	:              :
		 *
		 * A prologue looks like that on Mono/SH4:
		 *
		 * 	mov.l	r8, @-r15	// if r8 is used
		 * 	mov.l	r9, @-r15	// if r9 is used
		 * 	mov.l	r10, @-r15	// if r10 is used
		 * 	mov.l	r11, @-r15	// if r11 is used
		 * 	mov.l	r12, @-r15	// if r12 is used
		 * 	mov.l	r13, @-r15	// if r13 is used
		 * 	mov.l	r14, @-r15
		 * 	sts.l	pr, @-r15
		 * 	mov	r15, r14
		 * #if stack_offset is small
		 * 	add	-#stack_offset, r14
		 * #else if stack_offset is medium
		 * 	mov	#stack_offset, temp
		 * 	sub	temp, r14
		 * #else if stack_offset is large
		 * 	LOAD	#stack_offset, temp
		 * 	sub	temp, r14
		 * #else if stack_offset is 0
		 * #endif
		 *      [...]
		 * 	mov	r14, r15	// if stack_offset != 0
		 */

		SH4_EXTRA_DEBUG("start: %p", jit_info->code_start);

		/* Walk forward to find the space used by local variables. */
		for (code = jit_info->code_start; !is_sh4_mov(*code, sh4_sp, sh4_fp); code++) {
			/* Sanity check. */
			g_assert((guint8 *)code < (guint8 *)jit_info->code_start + jit_info->code_size);
		}
		code++;

		SH4_EXTRA_DEBUG("code: %p", code);

		/* stack_offset is small? */
		if (stack_offset = get_imm_sh4_add_imm(code[0]),
		    is_sh4_add_imm(code[0], stack_offset, sh4_fp)) {
			/* Remember stack_offset was negated due to
			   the use of "add_imm" instead of "sub_imm". */
			stack_offset = -stack_offset;
		}
		/* stack_offset is medium? */
		else if (stack_offset = get_imm_sh4_mov_imm(code[0]),
			 is_sh4_mov_imm(code[0], stack_offset, sh4_temp) &&
			 is_sh4_sub(code[1], sh4_temp, sh4_fp)) {
			/* stack_offset is already extracted. */;
		}
		/* stack_offset is large? */
		else if (stack_offset = get_imm_sh4_movl_dispPC(code[0]),
			 is_sh4_movl_dispPC(code[0], stack_offset, sh4_temp)) {
			/* The virtual address is formed by calculating PC + 4,
			   clearing the lowest 2 bits, and adding the immediate. */
			guint address = (guint)code;
			address += 4;
			address &= ~0x3;
			stack_offset = *(guint8 *)(address + stack_offset);
		}
		/* stack_offset is 0! */
		else {
			stack_offset = 0;
		}

		/* Now we know exactly where are saved global registers. */
		registers = (guint32 *)(context->registers[sh4_fp] + stack_offset);

		SH4_EXTRA_DEBUG("stack_offset: %d", stack_offset);
		SH4_EXTRA_DEBUG("registers: %p", registers);

		/* Extract the previous value of PC. */
		new_context->pc = *registers;
		registers++;

#if 0
		/* Extract the previous value of global floating-point registers. */
		for (i = MONO_MAX_FREGS - 1; i >= 0; i--) {
			if ((MONO_ARCH_CALLEE_SAVED_FREGS & (1 << i)) != 0 &&
			    (jit_info->used_fregs         & (1 << i)) != 0) {
				new_context->fregisters[i] = *registers;
				registers++;

				SH4_EXTRA_DEBUG("back freg%d: 0x%x -> 0x%x", i, context->registers[i], new_context->registers[i]);
			}
			else
				new_context->registers[i] = context->registers[i];
		}
#endif

		/* Extract the previous value of global registers. */
		/* Restore global registers. */
		for (i = MONO_MAX_IREGS - 1; i >= 0; i--) {
			if ((MONO_ARCH_CALLEE_SAVED_REGS & (1 << i)) != 0 &&
			    (jit_info->used_regs         & (1 << i)) != 0) {
				new_context->registers[i] = *registers;
				registers++;

				SH4_EXTRA_DEBUG("back reg%d: 0x%x -> 0x%x", i, context->registers[i], new_context->registers[i]);
			}
			else
				new_context->registers[i] = context->registers[i];
		}

		SH4_EXTRA_DEBUG("back pc: 0x%x -> 0x%x", context->pc, new_context->pc);

		/* Remove any unused LMF. */
		if (*lmf != NULL &&
		    *lmf != jit_tls->first_lmf &&
		    context->registers[sh4_fp] >= (*lmf)->registers[sh4_fp])
			*lmf = (*lmf)->previous_lmf;

		return jit_info;
	}
	else if (*lmf != NULL) {
		*new_context = *context;

		/* Top LMF entry? */
		if (*lmf == jit_tls->first_lmf)
			return (gpointer)-1;

		/* Check if it is a trampoline LMF. */
		jit_info = mono_jit_info_table_find(domain, (gpointer)(*lmf)->pc);
		if (jit_info == NULL) {
			if ((*lmf)->method == NULL)
				return (gpointer)-1;

			bzero(result, sizeof(MonoJitInfo));
			result->method = (*lmf)->method;
		}

		/* Adjust the new context with information from the LMF. */
		new_context->pc = (*lmf)->pc;
		memcpy(new_context->registers, (*lmf)->registers, sizeof(new_context->registers));

		/* Remove the current LMF. */
		*lmf = (*lmf)->previous_lmf;

		if (jit_info != NULL)
			return jit_info;
		else
			return result;
	}

	return NULL;
}

/**
 * Returns a pointer to a method which calls an exception filter.
 *
 * Technically, the generated method is similar to this pseudo-code :
 *
 * void call_filter(MonoContext *context, rtype (* handler)(void))
 * {
 * 	// Save all callee-saved registers (a.k.a global registers), except the stack pointer.
 * 	registers[] = { %R8, ... , %R14 };
 * 
 * 	unsigned int registers[8];
 * 
 *  	// Restore all global registers from the MonoContext, except the stack pointer.
 * 	{ %R8, ... , %R14 } = context.registers[8..14];
 * 	{ %FR12, ... , %FR15 } = context.fregisters[12..15];
 * 
 * 	// Call the handler.
 * 	handler();
 * 
 *  	// Restore all callee-saved registers from the stack, except the stack pointer.
 * 	{ %FR12, ... , %FR15 } = fregisters[];
 * 	{ %R8, ... , %R14 }    = registers[];
 * 
 * 	return;
 * }
 */
gpointer mono_arch_get_call_filter(void)
{
	static guint8 *code = NULL;
	guint8 *buffer = NULL;
	int i = 0;

	SH4_EXTRA_DEBUG("code = %p", code);

	if (code != NULL)
		return code;

#define CALL_FILTER_SIZE 64

	code = buffer = mono_global_codeman_reserve(CALL_FILTER_SIZE);

	/* At this point, the stack looks like :
	 *	:             :
	 *	|             | Caller's frame.
	 *	|=============|
	 *	|     PR      |
	 *	|-------------|
	 *	|             | Current frame.
	 *	| registers[] |
	 *	|             |
	 *	|-------------| <- SP
	 *	:             :
	 */

	/*
	 * Save all callee-saved registers (a.k.a global registers), except the stack pointer.
	 */

	/* pseudo-code: registers[] = { %R8, %R9, %R10, %R11, %R12, %R13, %R14 }; */
	for (i = 0; i < MONO_MAX_IREGS; i++)
		if ((MONO_ARCH_CALLEE_SAVED_REGS & (1 << i)) != 0)
			sh4_movl_decRx(&buffer, (SH4IntRegister)i, sh4_sp);

#if 0
	/* pseudo-code: fregisters[] = { %FR12, %FR13, %FR14, %FR15 }; */
	for (i = 0; i < MONO_MAX_FREGS; i++)
		if ((MONO_ARCH_CALLEE_SAVED_FREGS & (1 << i)) != 0)
			sh4_fmovl_decRx(&buffer, (SH4FloatRegister)i, sh4_sp);
#endif

	/* Save the return address. */
	sh4_stsl_PR_decRx(&buffer, sh4_sp);

	/*
	 * Restore all global registers from the MonoContext, except the stack pointer.
	 */

	/* R4 is now used to point to "context.registers[]". */
	sh4_add_imm(&buffer, offsetof(MonoContext, registers), sh4_r4);

	/* pseudo-code: { %R8, ... , %R14 } = context.registers[8..14]; */
	for (i = 0; i < MONO_MAX_IREGS; i++)
		if ((MONO_ARCH_CALLEE_SAVED_REGS & (1 << i)) != 0)
			sh4_movl_dispRy(&buffer, i * 4, sh4_r4, (SH4IntRegister)i);

#if 0
	/* pseudo-code: { %FR12, ... , %FR15 } = context.fregisters[12..15]; */
	for (i = 0; i < MONO_MAX_FREGS; i++)
		if ((MONO_ARCH_CALLEE_SAVED_FREGS & (1 << i)) != 0)
			sh4_fmovl_dispRy(&buffer, i * 4, sh4_r4, (SH4FloatRegister)i);
#endif

	/*
	 * Call the handler.
	 */

	/* pseudo-code: handler(); */
	sh4_jsr_indRx(&buffer, sh4_r5);
	sh4_nop(&buffer);

	/* Restore the return address. */
	sh4_ldsl_incRx_PR(&buffer, sh4_sp);

	/*
	 * Restore all callee-saved registers from the stack.
	 */

#if 0
	/* pseudo-code: { %FR12, ... , %FR15 } = fregisters[]; */
	for (i = MONO_MAX_FREGS - 1; i >= 0; i--)
		if ((MONO_ARCH_CALLEE_SAVED_FREGS & (1 << i)) != 0)
			sh4_fmovl_incRy(&buffer, sh4_sp, (SH4FloatRegister)i);
#endif

	/* pseudo-code: { %R8, ... , %R14 } = registers[]; */
	for (i = MONO_MAX_IREGS - 1; i >= 0; i--)
		if ((MONO_ARCH_CALLEE_SAVED_REGS & (1 << i)) != 0)
			sh4_movl_incRy(&buffer, sh4_sp, (SH4IntRegister)i);

	/* pseudo-code: return; */
	sh4_rts(&buffer);
	sh4_nop(&buffer);

	/* Sanity checks. */
	g_assert(buffer - code <= CALL_FILTER_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, CALL_FILTER_SIZE);

	SH4_EXTRA_DEBUG("code = %p", code);

	return code;
}

/**
 * Returns a pointer to a method which restores a previously saved MonoContext.
 *
 * Technically, the generated method is similar to this pseudo-code :
 *
 * void restore_context(MonoContext *context)
 * {
 * 	// Mimic a return from an ordinary routine by setting the
 * 	// linkage register PR to the value of the saved PC.
 * 	%PR = context.pc
 *
 * 	// Restore all registers.
 * 	{ %R0, ..., %R15 }   = context.registers[];
 * 	{ %FR0, ..., %FR15 } = context.fregisters[];
 * 
 *      return;
 * }
 *
 * The main assumption that is done here is that PR is stored
 * onto the stack even for a leaf routine. This point is not
 * required by SH4 ABI for a pure C routine (non managed code).
 */
gpointer mono_arch_get_restore_context(void)
{
	static guint8 *code = NULL;
	guint8 *buffer = NULL;
	int i = 0;

	SH4_EXTRA_DEBUG("code = %p", code);

	if (code != NULL)
		return code;

#define RESTORE_CONTEXT_SIZE 44

	code = buffer = mono_global_codeman_reserve(RESTORE_CONTEXT_SIZE);

	/* Rtemp now points to "context.registers[]" (used later). */
	sh4_mov(&buffer, sh4_r4, sh4_temp);
	sh4_add_imm(&buffer, offsetof(MonoContext, registers), sh4_temp);

	/*
	 * Mimic a return from an ordinary routine by setting the
	 * linkage register PR to the value of the saved PC.
	 */

	/* pseudo-code: %PR = context.pc; */
	sh4_movl_dispRy(&buffer, offsetof(MonoContext, pc), sh4_r4, sh4_r4);
	sh4_lds_PR(&buffer, sh4_r4);

	/*
	 * Restore all registers.
	 */

	/* pseudo-code: { %R0, ..., %R15 } = context.registers[]; */
	for (i = 0; i < MONO_MAX_IREGS; i++)
		if (i != sh4_temp)
			sh4_movl_dispRy(&buffer, i * 4, sh4_temp, (SH4IntRegister)i);

#if 0
	/* Rtemp now points to "context.fregisters[]". */
	sh4_add_imm(&buffer, -offsetof(MonoContext, registers) + offsetof(MonoContext, fregisters), sh4_temp);
	for (i = 0; i < MONO_MAX_FREGS; i++)
		sh4_fmovl_dispRy(&buffer, i * 4, sh4_temp, (SH4FloatRegister)i);
#endif

	/* pseudo-code: return; */
	sh4_rts(&buffer);
	sh4_nop(&buffer);

	/* Sanity checks. */
	g_assert(buffer - code <= RESTORE_CONTEXT_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, RESTORE_CONTEXT_SIZE);

	SH4_EXTRA_DEBUG("code = %p", code);

	return code;
}

/**
 * Raise an exception based on the passed parameters.
 */
static void throw_exception(MonoObject *exception, guint32 pc, guint32 *registers, gboolean rethrow)
{
	static void (* restore_context)(MonoContext *) = NULL;
	MonoContext context;

	SH4_EXTRA_DEBUG("args => %p, %d, %p, %d", exception, pc, registers, rethrow);

	if (restore_context == NULL)
		restore_context = mono_arch_get_restore_context();

	memset(&context, 0, sizeof(MonoContext));
	memcpy(&(context.registers), registers, sizeof(context.registers));
#if 0
	memcpy(&(context.fregisters), registers, sizeof(context.fregisters));
#endif
	context.pc = pc;

	if (rethrow != 0) {
		MonoObject *object = mono_object_isinst(exception, mono_defaults.exception_class);
		if (object != NULL)
			((MonoException*)exception)->stack_trace = NULL;
	}

	mono_handle_exception(&context, exception, (gpointer)pc, FALSE);
	restore_context(&context);

	g_assert_not_reached();
}

/**
 * Returns a pointer to a method which can be used to raise exceptions.
 *
 * void throw_exception_trampoline(void *exception)
 * {
 * 	// Save all registers onto the stack.
 * 	registers[]  = { %R0, ..., %R15 };
 * 	fregisters[] = { %FR0, ..., %FR15 };
 * 
 * 	unsigned int registers[16];
 * 	unsigned int fregisters[16];
 * 
 * #if by_name != 0
 * 	exception = mono_exception_from_name(mono_defaults.corlib, "System", exception);
 * #endif
 * 
 * 	goto throw_exception(exception, %PC, %SP, rethrow);
 * }
 */
static gpointer get_throw_exception(gboolean by_name, gboolean rethrow)
{
	int i = 0;
	int size = 58;

	guint8 *code   = NULL;
	guint8 *buffer = NULL;
	guint8 *patch0 = NULL;
	guint8 *patch1 = NULL;
	guint8 *patch2 = NULL;
	guint8 *patch3 = NULL;

	SH4_EXTRA_DEBUG("args => %d, %d", by_name, rethrow);

	if (by_name != 0)
		size += 28;

	code = buffer = mono_global_codeman_reserve(size);

	/*
	 * Save all registers onto the stack.
	 */

	/* pseudo-code: registers[] = { %R0, ..., %R15 }; */
	for (i = MONO_MAX_IREGS - 1; i >= 0; i--)
		sh4_movl_decRx(&buffer, (SH4IntRegister)i, sh4_sp);

#if 0
	/* pseudo-code: fregisters[] = { %FR0, ..., %FR15 }; */
	for (i = MONO_MAX_FREGS - 1; i >= 0; i--)
		sh4_fmovl_decRx(&buffer, (SH4FloatRegister)i, sh4_sp);
#endif

	/* At this point, the stack looks like :
	 *	:              :
	 *	|              | Caller's frame.
	 *	|==============|
	 *	|              | Current frame.
	 *	| registers[]  |
	 *	|              |
	 *	|--------------|
	 *	|              |
	 *	| fregisters[] |
	 *	|              |
	 *	|--------------| <- SP
	 *	:              :
	 */

	if (by_name != 0) {
		/* The current return address have to be preserved through
		   the next call because it is used later. */
		sh4_sts_PR(&buffer, sh4_r8);

		/* Currently, sh4_r4 holds the name of the exception. */
		sh4_mov(&buffer, sh4_r4, sh4_r6);

		/* Patch slot for : sh4_r4 <- mono_defaults.corlib */
		patch1 = buffer;
		sh4_die(&buffer);

		/* Patch slot for : sh4_r5 <- "System" */
		patch2 = buffer;
		sh4_die(&buffer);

		/* Patch slot for : sh4_r0 <- mono_exception_from_name */
		patch3 = buffer;
		sh4_die(&buffer);

		/* pseudo-code: exception = mono_exception_from_name(mono_defaults.corlib, "System", exception); */
		sh4_jsr_indRx(&buffer, sh4_r0);
		sh4_nop(&buffer);

		/* Overwrite the variable 'exception'. */
		sh4_mov(&buffer, sh4_r0, sh4_r4);

		sh4_lds_PR(&buffer, sh4_r8);
	}

	/*
	 * Jump to throw_exception.
	 */

	/* Fill parameters passed to the throw_exception(),
	   sh4_r4 already holds the variable 'exception'. */
	sh4_sts_PR(&buffer, sh4_r5);
	sh4_mov(&buffer, sh4_sp, sh4_r6);
	sh4_mov_imm(&buffer, (rethrow != 0 ? 1 : 0), sh4_r7);

	/* Patch slot for : sh4_r0 <- throw_exception */
	patch0 = buffer;
	sh4_die(&buffer);

	/* pseudo-code: goto throw_exception(exception, pc, sp, rethrow); */
	sh4_jmp_indRx(&buffer, sh4_r0);
	sh4_nop(&buffer);

	/* Align the constant pool. */
	while (((guint32)buffer % 4) != 0)
		sh4_nop(&buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_PCrel(&patch0, buffer, sh4_r0);
	sh4_emit32(&buffer, (guint32)throw_exception);

	if (by_name != 0) {
		sh4_movl_PCrel(&patch1, buffer, sh4_r4);
		sh4_emit32(&buffer, (guint32)mono_defaults.corlib);
		sh4_movl_PCrel(&patch2, buffer, sh4_r5);
		sh4_emit32(&buffer, (guint32)"System");
		sh4_movl_PCrel(&patch3, buffer, sh4_r0);
		sh4_emit32(&buffer, (guint32)mono_exception_from_name);
	}

	/* Sanity checks. */
	g_assert(buffer - code <= size);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, size);

	SH4_EXTRA_DEBUG("code = %p", code);

	return code;
}

gpointer mono_arch_get_rethrow_exception(void)
{
	static guint8 *code = NULL;

	if (code == NULL)
		code = get_throw_exception(FALSE, TRUE);

	return code;
}

gpointer mono_arch_get_throw_exception(void)
{
	static guint8 *code = NULL;

	if (code == NULL)
		code = get_throw_exception(FALSE, FALSE);

	return code;
}

gpointer mono_arch_get_throw_exception_by_name(void)
{
	static guint8 *code = NULL;

	if (code == NULL)
		code = get_throw_exception(TRUE, FALSE);

	return code;
}

/**
 * Returns a function pointer which can be used to raise corlib exceptions.
 *
 * void throw_corlib_exception(guint32 token, guint32 offset)
 * {
 *      // Get the exception object.
 *      MonoException *exception = mono_exception_from_token(
 *                                         mono_defaults.exception_class->image,
 *                                         MONO_TOKEN_TYPE_DEF + token);
 *
 *
 *      // Adjust the caller IP to get the IP of the throw.
 *      %PR = %PR - offset;
 *
 * #def throw_exception = mono_arch_get_throw_exception();
 *      goto throw_exception(exception);
 * }
 *
 * Here, offset is the offset which needs to be substracted from the
 * caller IP to get the IP of the throw. Passing the offset has the
 * advantage that it needs no relocations in the caller.
 */
gpointer mono_arch_get_throw_corlib_exception(void)
{
	guint8 *code   = NULL;
	guint8 *buffer = NULL;
	guint8 *patch0 = NULL;
	guint8 *patch1 = NULL;
	guint8 *patch2 = NULL;

#define THROW_CORLIB_EXCEPTION_SIZE 46

	code = buffer = mono_global_codeman_reserve(THROW_CORLIB_EXCEPTION_SIZE);

	/* Save the parameter 'offset' and the return address onto the stack. */
	sh4_movl_decRx(&buffer, sh4_r5, sh4_sp);
	sh4_stsl_PR_decRx(&buffer, sh4_sp);

	/*
	 * Get the exception object.
	 */

	sh4_mov(&buffer, sh4_r4, sh4_r5);

	/* Patch slot for : sh4_r4 <- mono_defaults.exception_class->image */
	patch0 = buffer;
	sh4_die(&buffer);

	/* Patch slot for : sh4_temp <- mono_exception_from_token */
	patch1 = buffer;
	sh4_die(&buffer);

	/* pseudo-code: exception = mono_exception_from_token(... */
	sh4_jsr_indRx(&buffer, sh4_temp);
	sh4_nop(&buffer);

	sh4_mov(&buffer, sh4_r0, sh4_r4);

	/*
	 * Adjust the caller IP to get the IP of the throw.
	 */

	/* Restore the parameter 'offset' from the stack. */
	sh4_movl_incRy(&buffer, sh4_sp, sh4_temp);
	sh4_movl_incRy(&buffer, sh4_sp, sh4_r5);

	/* pseudo-code: %PR = %PR - offset; */
	sh4_sub(&buffer, sh4_r5, sh4_temp);
	sh4_lds_PR(&buffer, sh4_temp);

	/* Patch slot for : sh4_temp <- throw_exception */
	patch2 = buffer;
	sh4_die(&buffer);

	/* pseudo-code: goto throw_exception(exception); */
	sh4_jmp_indRx(&buffer, sh4_temp);
	sh4_nop(&buffer);

	/* Should never return. */
	sh4_die(&buffer);

	/* Align the constant pool. */
	while (((guint32)buffer % 4) != 0)
		sh4_nop(&buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_PCrel(&patch0, buffer, sh4_r4);
	sh4_emit32(&buffer, (guint32)mono_defaults.exception_class->image);
	sh4_movl_PCrel(&patch1, buffer, sh4_temp);
	sh4_emit32(&buffer, (guint32)mono_exception_from_token);
	sh4_movl_PCrel(&patch2, buffer, sh4_temp);
	sh4_emit32(&buffer, (guint32)mono_arch_get_throw_exception());

	/* Sanity checks. */
	g_assert(buffer - code <= THROW_CORLIB_EXCEPTION_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, THROW_CORLIB_EXCEPTION_SIZE);

	SH4_EXTRA_DEBUG("code = %p", code);

	return code;
}

void mono_arch_sigctx_to_monoctx(void *context, MonoContext *mono_context)
{
	ucontext_t *ucontext = context;
	int i = 0;

	mono_context->pc = ucontext->uc_mcontext.pc;

	for (i = 0; i < MONO_MAX_IREGS; i++)
		mono_context->registers[i] = ucontext->uc_mcontext.gregs[i];

#if 0
	for (i = 0; i < MONO_MAX_FREGS; i++)
		mono_context->fregisters[i] = ucontext->uc_mcontext.fregs[i];
#endif

	return;
}

void mono_arch_monoctx_to_sigctx(MonoContext *mono_context, void *context)
{
	ucontext_t *ucontext = context;
	int i = 0;

	ucontext->uc_mcontext.pc = mono_context->pc;

	for (i = 0; i < MONO_MAX_IREGS; i++)
		ucontext->uc_mcontext.gregs[i] = mono_context->registers[i];

#if 0
	for (i = 0; i < MONO_MAX_FREGS; i++)
		ucontext->uc_mcontext.fregs[i] = mono_context->fregisters[i];
#endif

	return;
}

/**
 * This is the function called from the signal handler
 */
gboolean mono_arch_handle_exception(void *ucontext, gpointer object, gboolean test_only)
{
	MonoContext mono_context;

	mono_arch_sigctx_to_monoctx(ucontext, &mono_context);

	mono_handle_exception(&mono_context, object, (gpointer)mono_context.pc, test_only);

	/* Restore the context so that returning from the signal handler
	   will invoke the catch clause. */
	mono_arch_monoctx_to_sigctx(&mono_context, ucontext);

	return TRUE;
}

gpointer mono_arch_ip_from_context(void *context)
{
	ucontext_t *ucontext = (ucontext_t *)context;
	return (gpointer)ucontext->uc_mcontext.pc;
}
