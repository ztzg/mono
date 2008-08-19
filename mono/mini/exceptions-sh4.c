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

MonoJitInfo *mono_arch_find_jit_info(MonoDomain *domain, MonoJitTlsData *jit_tls, MonoJitInfo *res, MonoJitInfo *prev_ji, MonoContext *context, MonoContext *new_context, char **trace, MonoLMF **lmf, int *native_offset, gboolean *managed)
{
	/* TODO - CV */
	g_assert(0);
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
 * 
 * 	// Call the handler.
 * 	handler();
 * 
 *  	// Restore all callee-saved registers from the stack, except the stack pointer.
 * 	{ %R8, ... , %R14 } = registers[];
 * 
 * 	return;
 * }
 */
gpointer mono_arch_get_call_filter(void)
{
	static guint8 *code = NULL;
	guint8 *buffer = NULL;
	int i = 0;

	if (code != NULL)
		return code;

#define CALL_FILTER_SIZE 58

	code = buffer = mono_global_codeman_reserve(CALL_FILTER_SIZE);

	/* Save the return address. */
	sh4_stsl_PR_decRx(buffer, sh4_r15);

	/* pseudo-code: unsigned int registers[8]; */
	sh4_add_imm(buffer, - (7 * 4), sh4_r15);

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
	for (i = 8; i <= 14; i++)
		sh4_movl_dispRx(buffer, (SH4IntRegister)i, (i - 8) * 4, sh4_r15);

	/*
	 * Restore all global registers from the MonoContext, except the stack pointer.
	 */

	/* R4 is now used to point to "context.registers[]". */
	sh4_add_imm(buffer, offsetof(MonoContext, registers), sh4_r4);

	/* pseudo-code: { %R8, ... , %R14 } = context.registers[8..14]; */
	for (i = 8; i <= 13; i++) /* R14 is set into the next delay slot. */
		sh4_movl_dispRy(buffer, i * 4, sh4_r4, (SH4IntRegister)i);

	/*
	 * Call the handler.
	 */

	/* pseudo-code: handler(); */
	sh4_jsr_indRx(buffer, sh4_r5);
	sh4_movl_dispRy(buffer, 14 * 4, sh4_r4, sh4_r14); /* <= Delay slot optimization. */

	/*
	 * Restore all callee-saved registers from the stack.
	 */

	/* pseudo-code: { %R8, ... , %R14 } = registers[]; */
	for (i = 8; i <= 14; i++)
		sh4_movl_dispRy(buffer, (i - 8) * 4, sh4_r15, (SH4IntRegister)i);

	/* pseudo-code: return; */
	sh4_add_imm(buffer, (7 * 4), sh4_r15);
	sh4_ldsl_incRx_PR(buffer, sh4_r15);
	sh4_rts(buffer);
	sh4_nop(buffer);

	/* Sanity checks. */
	g_assert(buffer - code <= CALL_FILTER_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, CALL_FILTER_SIZE);

	return code;
}

/**
 * Returns a pointer to a method which restores a previously saved MonoContext.
 *
 * Technically, the generated method is similar to this pseudo-code :
 *
 * void restore_context(MonoContext *context)
 * {
 * 	// Restore almost all registers.
 * 	{ %R1, ..., %R15 } = context.registers[];
 * 
 * 	// Jump to the saved PC.
 * 	goto context.pc;
 * }
 */
gpointer mono_arch_get_restore_context(void)
{
	static guint8 *code = NULL;
	guint8 *buffer = NULL;
	int i = 0;

	if (code != NULL)
		return code;

#define RESTORE_CONTEXT_SIZE 42

	code = buffer = mono_global_codeman_reserve(RESTORE_CONTEXT_SIZE);

	/*
	 * Restore almost all registers.
	 */

	/* R0 is now used to point to "context.registers[]" (used later). */
	sh4_mov(buffer, sh4_r4, sh4_r0);
	sh4_add_imm(buffer, offsetof(MonoContext, registers), sh4_r0);

	/* pseudo-code: { %R1, ..., %R15 } = context.registers[]; */
	for (i = 1; i <= 15; i++)
		sh4_movl_dispRy(buffer, i * 4, sh4_r0, (SH4IntRegister)i);

	/*
	 * Jump to the saved PC.
	 */

	/* pseudo-code: goto context.pc; */
	sh4_add_imm(buffer, - offsetof(MonoContext, registers) + offsetof(MonoContext, pc), sh4_r0);
	sh4_movl_indRy(buffer, sh4_r0, sh4_r0);
	sh4_jmp_indRx(buffer, sh4_r0);
	sh4_nop(buffer);

	/* Sanity checks. */
	g_assert(buffer - code <= RESTORE_CONTEXT_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, RESTORE_CONTEXT_SIZE);

	return code;
}


/**
 * Raise an exception based on the passed parameters.
 */
static void throw_exception(MonoObject *exception, guint32 pc, guint32 *registers, gboolean rethrow)
{
	static void (* restore_context)(MonoContext *) = NULL;
	MonoContext context;

	if (restore_context == NULL)
		restore_context = mono_arch_get_restore_context();

	memset(&context, 0, sizeof(MonoContext));
	memcpy(&(context.registers), registers, sizeof(context.registers));
	context.pc = pc;

	if (rethrow != 0) {
		MonoObject *object = mono_object_isinst(exception, mono_defaults.exception_class);
		if (object != NULL)
			((MonoException*)exception)->stack_trace = NULL;
	}

	/* Adjust PC to point to the call instruction. */
	mono_handle_exception(&context, exception, (gpointer)pc, FALSE);
	restore_context(&context);

	g_assert_not_reached();
}

/**
 * Returns a pointer to a method which can be used to raise exceptions.
 *
 * void throw_exception_trampoline(MonoObject *exception)
 * {
 * 	// Save all registers onto the stack.
 * 	registers[] = { %R0, ..., %R15 };
 * 
 * 	unsigned int registers[16];
 * 
 * #if rethrow != 0
 * 	goto throw_exception(exception, %PC, %SP, 1);
 * #else
 * 	goto throw_exception(exception, %PC, %SP, 0);
 * #endif
 * }
 */
static gpointer get_throw_exception(gboolean rethrow)
{
	int i = 0;
	guint8 *code   = NULL;
	guint8 *buffer = NULL;
	guint8 *patch  = NULL;

#define THROW_EXCEPTION_SIZE 56

	code = buffer = mono_global_codeman_reserve(THROW_EXCEPTION_SIZE);

	/*
	 * Save all registers onto the stack.
	 */

	/* Adjust SP to allocate the stacked registers[]. */
	sh4_add_imm(buffer, -16 * 4, sh4_r15);

	/* At this point, the stack looks like :
	 *	:             :
	 *	|             | Caller's frame.
	 *	|=============|
	 *	|             | Current frame.
	 *	| registers[] |
	 *	|             |
	 *	|-------------| <- SP
	 *	:             :
	 */

	/* pseudo-code: registers[] = { %R0, ..., %R15 }; */
	for (i = 0; i <= 14; i++)
		sh4_movl_dispRx(buffer, (SH4IntRegister)i, i * 4, sh4_r15);

	/* Compute the previous value of SP before saving into registers[]. */
	sh4_mov(buffer, sh4_r15, sh4_r0);
	sh4_add_imm(buffer, 16 * 4, sh4_r0);
	sh4_movl_dispRx(buffer, sh4_r0, 15 * 4, sh4_r15);

	/*
	 * Jump to throw_exception.
	 */

	/* Fill parameters passed to the throw_exception().
	   The variable 'exception' is already in place in sh4_r4. */
	sh4_sts_PR(buffer, sh4_r5);
	sh4_mov(buffer, sh4_r15, sh4_r6);
	sh4_mov_imm(buffer, (rethrow != 0 ? 1 : 0), sh4_r7);

	/* Patch slot for : sh4_r0 <- throw_exception */
	patch = buffer;
	sh4_sleep(buffer);

	/* pseudo-code: goto throw_exception(exception, pc, sp, 0/1); */
	sh4_jmp_indRx(buffer, sh4_r0);
	sh4_nop(buffer);

	/* Align the constant pool. */
	while (((guint32)buffer % 4) != 0)
		sh4_nop(buffer);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_dispPC(patch, (guint32)buffer - (((guint32)patch + 4) & ~0x3), sh4_r0);
	sh4_emit32(buffer, (guint32)throw_exception);

	/* Sanity checks. */
	g_assert(buffer - code <= THROW_EXCEPTION_SIZE);

	/* Flush instruction cache, since we've generated code. */
	mono_arch_flush_icache(code, THROW_EXCEPTION_SIZE);

	return code;
}

gpointer mono_arch_get_rethrow_exception(void)
{
	static guint8 *code = NULL;

	if (code == NULL)
		code = get_throw_exception(TRUE);

	return code;
}

gpointer mono_arch_get_throw_exception(void)
{
	static guint8 *code = NULL;

	if (code == NULL)
		code = get_throw_exception(FALSE);

	return code;
}

gpointer mono_arch_get_throw_exception_by_name(void)
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

gpointer mono_arch_ip_from_context(void *context)
{
	ucontext_t *ucontext = (ucontext_t *)context;
	return (gpointer)ucontext->uc_mcontext.pc;
}

void mono_arch_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
	/* TODO - CV */
	g_assert(0);
	return;
}
