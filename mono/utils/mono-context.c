/*
 * mono-context.c: plat independent machine state definitions
 *
 *
 * Copyright (c) 2011 Novell, Inc (http://www.novell.com)
 */

#include <mono/utils/mono-sigcontext.h>

#ifdef HAVE_UCONTEXT_H
#include <ucontext.h>
#endif

#if (defined(__i386__) && !defined(MONO_CROSS_COMPILE)) || (defined(TARGET_X86))

#include <mono/utils/mono-context.h>

#ifdef HOST_WIN32
#include <windows.h>
#endif

#ifdef __sun
#define REG_EAX EAX
#define REG_EBX EBX
#define REG_ECX ECX
#define REG_EDX EDX
#define REG_EBP EBP
#define REG_ESP ESP
#define REG_ESI ESI
#define REG_EDI EDI
#define REG_EIP EIP
#endif

void
mono_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
#if defined (__native_client__)
	printf("WARNING: mono_arch_sigctx_to_monoctx() called!\n");
	mctx->eax = 0xDEADBEEF;
	mctx->ebx = 0xDEADBEEF;
	mctx->ecx = 0xDEADBEEF;
	mctx->edx = 0xDEADBEEF;
	mctx->ebp = 0xDEADBEEF;
	mctx->esp = 0xDEADBEEF;
	mctx->esi = 0xDEADBEEF;
	mctx->edi = 0xDEADBEEF;
	mctx->eip = 0xDEADBEEF;
#elif defined(MONO_SIGNAL_USE_SIGACTION)
	ucontext_t *ctx = (ucontext_t*)sigctx;
	
	mctx->eax = UCONTEXT_REG_EAX (ctx);
	mctx->ebx = UCONTEXT_REG_EBX (ctx);
	mctx->ecx = UCONTEXT_REG_ECX (ctx);
	mctx->edx = UCONTEXT_REG_EDX (ctx);
	mctx->ebp = UCONTEXT_REG_EBP (ctx);
	mctx->esp = UCONTEXT_REG_ESP (ctx);
	mctx->esi = UCONTEXT_REG_ESI (ctx);
	mctx->edi = UCONTEXT_REG_EDI (ctx);
	mctx->eip = UCONTEXT_REG_EIP (ctx);
#elif defined(HOST_WIN32)
	CONTEXT *context = (CONTEXT*)sigctx;

	mctx->eip = context->Eip;
	mctx->edi = context->Edi;
	mctx->esi = context->Esi;
	mctx->ebx = context->Ebx;
	mctx->edx = context->Edx;
	mctx->ecx = context->Ecx;
	mctx->eax = context->Eax;
	mctx->ebp = context->Ebp;
	mctx->esp = context->Esp;
#else	
	struct sigcontext *ctx = (struct sigcontext *)sigctx;

	mctx->eax = ctx->SC_EAX;
	mctx->ebx = ctx->SC_EBX;
	mctx->ecx = ctx->SC_ECX;
	mctx->edx = ctx->SC_EDX;
	mctx->ebp = ctx->SC_EBP;
	mctx->esp = ctx->SC_ESP;
	mctx->esi = ctx->SC_ESI;
	mctx->edi = ctx->SC_EDI;
	mctx->eip = ctx->SC_EIP;
#endif /* if defined(__native_client__) */
}

void
mono_monoctx_to_sigctx (MonoContext *mctx, void *sigctx)
{
#if defined(__native_client__)
	printf("WARNING: mono_arch_monoctx_to_sigctx() called!\n");
#elif defined(MONO_SIGNAL_USE_SIGACTION)
	ucontext_t *ctx = (ucontext_t*)sigctx;

	UCONTEXT_REG_EAX (ctx) = mctx->eax;
	UCONTEXT_REG_EBX (ctx) = mctx->ebx;
	UCONTEXT_REG_ECX (ctx) = mctx->ecx;
	UCONTEXT_REG_EDX (ctx) = mctx->edx;
	UCONTEXT_REG_EBP (ctx) = mctx->ebp;
	UCONTEXT_REG_ESP (ctx) = mctx->esp;
	UCONTEXT_REG_ESI (ctx) = mctx->esi;
	UCONTEXT_REG_EDI (ctx) = mctx->edi;
	UCONTEXT_REG_EIP (ctx) = mctx->eip;
#elif defined(HOST_WIN32)
	CONTEXT *context = (CONTEXT*)sigctx;

	context->Eip = mctx->eip;
	context->Edi = mctx->edi;
	context->Esi = mctx->esi;
	context->Ebx = mctx->ebx;
	context->Edx = mctx->edx;
	context->Ecx = mctx->ecx;
	context->Eax = mctx->eax;
	context->Ebp = mctx->ebp;
	context->Esp = mctx->esp;
#else
	struct sigcontext *ctx = (struct sigcontext *)sigctx;

	ctx->SC_EAX = mctx->eax;
	ctx->SC_EBX = mctx->ebx;
	ctx->SC_ECX = mctx->ecx;
	ctx->SC_EDX = mctx->edx;
	ctx->SC_EBP = mctx->ebp;
	ctx->SC_ESP = mctx->esp;
	ctx->SC_ESI = mctx->esi;
	ctx->SC_EDI = mctx->edi;
	ctx->SC_EIP = mctx->eip;
#endif /* __native_client__ */
}

#elif (defined(__x86_64__) && !defined(MONO_CROSS_COMPILE)) || (defined(TARGET_AMD64)) /* defined(__i386__) */

#include <mono/utils/mono-context.h>

#ifdef HOST_WIN32
#include <windows.h>
#endif

void
mono_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
#if defined(__native_client_codegen__) || defined(__native_client__)
	printf("WARNING: mono_arch_sigctx_to_monoctx() called!\n");
#endif

#if defined(MONO_SIGNAL_USE_SIGACTION)
	ucontext_t *ctx = (ucontext_t*)sigctx;

	mctx->rax = UCONTEXT_REG_RAX (ctx);
	mctx->rbx = UCONTEXT_REG_RBX (ctx);
	mctx->rcx = UCONTEXT_REG_RCX (ctx);
	mctx->rdx = UCONTEXT_REG_RDX (ctx);
	mctx->rbp = UCONTEXT_REG_RBP (ctx);
	mctx->rsp = UCONTEXT_REG_RSP (ctx);
	mctx->rsi = UCONTEXT_REG_RSI (ctx);
	mctx->rdi = UCONTEXT_REG_RDI (ctx);
	mctx->r8 = UCONTEXT_REG_R8 (ctx);
	mctx->r9 = UCONTEXT_REG_R9 (ctx);
	mctx->r10 = UCONTEXT_REG_R10 (ctx);
	mctx->r11 = UCONTEXT_REG_R11 (ctx);
	mctx->r12 = UCONTEXT_REG_R12 (ctx);
	mctx->r13 = UCONTEXT_REG_R13 (ctx);
	mctx->r14 = UCONTEXT_REG_R14 (ctx);
	mctx->r15 = UCONTEXT_REG_R15 (ctx);
	mctx->rip = UCONTEXT_REG_RIP (ctx);
#elif defined(HOST_WIN32)
	CONTEXT *context = (CONTEXT*)sigctx;

	mctx->rip = context->Rip;
	mctx->rax = context->Rax;
	mctx->rcx = context->Rcx;
	mctx->rdx = context->Rdx;
	mctx->rbx = context->Rbx;
	mctx->rsp = context->Rsp;
	mctx->rbp = context->Rbp;
	mctx->rsi = context->Rsi;
	mctx->rdi = context->Rdi;
	mctx->r8 = context->R8;
	mctx->r9 = context->R9;
	mctx->r10 = context->R10;
	mctx->r11 = context->R11;
	mctx->r12 = context->R12;
	mctx->r13 = context->R13;
	mctx->r14 = context->R14;
	mctx->r15 = context->R15;
#else
	MonoContext *ctx = (MonoContext *)sigctx;

	mctx->rax = ctx->rax;
	mctx->rbx = ctx->rbx;
	mctx->rcx = ctx->rcx;
	mctx->rdx = ctx->rdx;
	mctx->rbp = ctx->rbp;
	mctx->rsp = ctx->rsp;
	mctx->rsi = ctx->rsi;
	mctx->rdi = ctx->rdi;
	mctx->r8 = ctx->r8;
	mctx->r9 = ctx->r9;
	mctx->r10 = ctx->r10;
	mctx->r11 = ctx->r11;
	mctx->r12 = ctx->r12;
	mctx->r13 = ctx->r13;
	mctx->r14 = ctx->r14;
	mctx->r15 = ctx->r15;
	mctx->rip = ctx->rip;
#endif
}

void
mono_monoctx_to_sigctx (MonoContext *mctx, void *sigctx)
{
#if defined(__native_client__) || defined(__native_client_codegen__)
  printf("WARNING: mono_arch_monoctx_to_sigctx() called!\n");
#endif

#if defined(MONO_SIGNAL_USE_SIGACTION)
	ucontext_t *ctx = (ucontext_t*)sigctx;

	UCONTEXT_REG_RAX (ctx) = mctx->rax;
	UCONTEXT_REG_RBX (ctx) = mctx->rbx;
	UCONTEXT_REG_RCX (ctx) = mctx->rcx;
	UCONTEXT_REG_RDX (ctx) = mctx->rdx;
	UCONTEXT_REG_RBP (ctx) = mctx->rbp;
	UCONTEXT_REG_RSP (ctx) = mctx->rsp;
	UCONTEXT_REG_RSI (ctx) = mctx->rsi;
	UCONTEXT_REG_RDI (ctx) = mctx->rdi;
	UCONTEXT_REG_R8 (ctx) = mctx->r8;
	UCONTEXT_REG_R9 (ctx) = mctx->r9;
	UCONTEXT_REG_R10 (ctx) = mctx->r10;
	UCONTEXT_REG_R11 (ctx) = mctx->r11;
	UCONTEXT_REG_R12 (ctx) = mctx->r12;
	UCONTEXT_REG_R13 (ctx) = mctx->r13;
	UCONTEXT_REG_R14 (ctx) = mctx->r14;
	UCONTEXT_REG_R15 (ctx) = mctx->r15;
	UCONTEXT_REG_RIP (ctx) = mctx->rip;
#elif defined(HOST_WIN32)
	CONTEXT *context = (CONTEXT*)sigctx;

	context->Rip = mctx->rip;
	context->Rax = mctx->rax;
	context->Rcx = mctx->rcx;
	context->Rdx = mctx->rdx;
	context->Rbx = mctx->rbx;
	context->Rsp = mctx->rsp;
	context->Rbp = mctx->rbp;
	context->Rsi = mctx->rsi;
	context->Rdi = mctx->rdi;
	context->R8 = mctx->r8;
	context->R9 = mctx->r9;
	context->R10 = mctx->r10;
	context->R11 = mctx->r11;
	context->R12 = mctx->r12;
	context->R13 = mctx->r13;
	context->R14 = mctx->r14;
	context->R15 = mctx->r15;
#else
	MonoContext *ctx = (MonoContext *)sigctx;

	ctx->rax = mctx->rax;
	ctx->rbx = mctx->rbx;
	ctx->rcx = mctx->rcx;
	ctx->rdx = mctx->rdx;
	ctx->rbp = mctx->rbp;
	ctx->rsp = mctx->rsp;
	ctx->rsi = mctx->rsi;
	ctx->rdi = mctx->rdi;
	ctx->r8 = mctx->r8;
	ctx->r9 = mctx->r9;
	ctx->r10 = mctx->r10;
	ctx->r11 = mctx->r11;
	ctx->r12 = mctx->r12;
	ctx->r13 = mctx->r13;
	ctx->r14 = mctx->r14;
	ctx->r15 = mctx->r15;
	ctx->rip = mctx->rip;
#endif
}

#elif defined(__s390x__)

#include <mono/utils/mono-context.h>

/*------------------------------------------------------------------*/
/*                                                                  */
/* Name		- mono_arch_sigctx_to_monoctx.                      */
/*                                                                  */
/* Function	- Called from the signal handler to convert signal  */
/*                context to MonoContext.                           */
/*                                                                  */
/*------------------------------------------------------------------*/

void
mono_sigctx_to_monoctx (void *ctx, MonoContext *mctx)
{
	memcpy (mctx, ctx, sizeof(MonoContext));
}

/*========================= End of Function ========================*/

/*------------------------------------------------------------------*/
/*                                                                  */
/* Name		- mono_arch_monoctx_to_sigctx.                      */
/*                                                                  */
/* Function	- Convert MonoContext structure to signal context.  */
/*                                                                  */
/*------------------------------------------------------------------*/

void
mono_monoctx_to_sigctx (MonoContext *mctx, void *ctx)
{
	memcpy (ctx, mctx, sizeof(MonoContext));
}

/*========================= End of Function ========================*/

#elif (defined(__arm__) && !defined(MONO_CROSS_COMPILE)) || (defined(TARGET_ARM))

#include <mono/utils/mono-context.h>
#include <mono/arch/arm/arm-codegen.h>
#include <mono/arch/arm/arm-vfp-codegen.h>

void
mono_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
#ifdef MONO_CROSS_COMPILE
	g_assert_not_reached ();
#elif defined(__native_client__)
	g_assert_not_reached ();
#else
	arm_ucontext *my_uc = sigctx;

	mctx->pc = UCONTEXT_REG_PC (my_uc);
	mctx->regs [ARMREG_SP] = UCONTEXT_REG_SP (my_uc);
	mctx->cpsr = UCONTEXT_REG_CPSR (my_uc);
	memcpy (&mctx->regs, &UCONTEXT_REG_R0 (my_uc), sizeof (mgreg_t) * 16);
#ifdef UCONTEXT_REG_VFPREGS
	memcpy (&mctx->fregs, UCONTEXT_REG_VFPREGS (my_uc), sizeof (double) * 16);
#endif
#endif
}

void
mono_monoctx_to_sigctx (MonoContext *mctx, void *ctx)
{
#ifdef MONO_CROSS_COMPILE
	g_assert_not_reached ();
#elif defined(__native_client__)
	g_assert_not_reached ();
#else
	arm_ucontext *my_uc = ctx;

	UCONTEXT_REG_PC (my_uc) = mctx->pc;
	UCONTEXT_REG_SP (my_uc) = mctx->regs [ARMREG_SP];
	UCONTEXT_REG_CPSR (my_uc) = mctx->cpsr;
	/* The upper registers are not guaranteed to be valid */
	memcpy (&UCONTEXT_REG_R0 (my_uc), &mctx->regs, sizeof (mgreg_t) * 12);
#ifdef UCONTEXT_REG_VFPREGS
	memcpy (UCONTEXT_REG_VFPREGS (my_uc), &mctx->fregs, sizeof (double) * 16);
#endif
#endif
}

#elif (defined(__aarch64__) && !defined(MONO_CROSS_COMPILE)) || (defined(TARGET_ARM64))

#include <mono/utils/mono-context.h>

void
mono_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
#ifdef MONO_CROSS_COMPILE
	g_assert_not_reached ();
#else
	memcpy (mctx->regs, UCONTEXT_GREGS (sigctx), sizeof (mgreg_t) * 31);
	mctx->pc = UCONTEXT_REG_PC (sigctx);
	mctx->regs [ARMREG_SP] = UCONTEXT_REG_SP (sigctx);
	/*
	 * We don't handle fp regs, this is not currrently a
	 * problem, since we don't allocate them globally.
	 */
#endif
}

void
mono_monoctx_to_sigctx (MonoContext *mctx, void *sigctx)
{
#ifdef MONO_CROSS_COMPILE
	g_assert_not_reached ();
#else
	memcpy (UCONTEXT_GREGS (sigctx), mctx->regs, sizeof (mgreg_t) * 31);
	UCONTEXT_REG_PC (sigctx) = mctx->pc;
	UCONTEXT_REG_SP (sigctx) = mctx->regs [ARMREG_SP];
#endif
}

#elif (defined(__mips__) && !defined(MONO_CROSS_COMPILE)) || (defined(TARGET_MIPS))

#include <mono/utils/mono-context.h>
#include <mono/arch/mips/mips-codegen.h>

void
mono_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
	int i;

	mctx->sc_pc = UCONTEXT_REG_PC (sigctx);
	for (i = 0; i < 32; ++i) {
		mctx->sc_regs[i] = UCONTEXT_GREGS (sigctx) [i];
		mctx->sc_fpregs[i] = UCONTEXT_FPREGS (sigctx) [i];
	}
}

void
mono_monoctx_to_sigctx (MonoContext *mctx, void *sigctx)
{
	int i;

	UCONTEXT_REG_PC (sigctx) = mctx->sc_pc;
	for (i = 0; i < 32; ++i) {
		UCONTEXT_GREGS (sigctx) [i] = mctx->sc_regs[i];
		UCONTEXT_FPREGS (sigctx) [i] = mctx->sc_fpregs[i];
	}
}

#elif (((defined(__ppc__) || defined(__powerpc__) || defined(__ppc64__)) && !defined(MONO_CROSS_COMPILE))) || (defined(TARGET_POWERPC))

#include <mono/utils/mono-context.h>
#include <mono/mini/mini-ppc.h>

void
mono_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
	os_ucontext *uc = sigctx;

	mctx->sc_ir = UCONTEXT_REG_NIP(uc);
	mctx->sc_sp = UCONTEXT_REG_Rn(uc, 1);
	memcpy (&mctx->regs, &UCONTEXT_REG_Rn(uc, 13), sizeof (mgreg_t) * MONO_SAVED_GREGS);
	memcpy (&mctx->fregs, &UCONTEXT_REG_FPRn(uc, 14), sizeof (double) * MONO_SAVED_FREGS);
}

void
mono_monoctx_to_sigctx (MonoContext *mctx, void *sigctx)
{
	os_ucontext *uc = sigctx;

	UCONTEXT_REG_NIP(uc) = mctx->sc_ir;
	UCONTEXT_REG_Rn(uc, 1) = mctx->sc_sp;
	memcpy (&UCONTEXT_REG_Rn(uc, 13), &mctx->regs, sizeof (mgreg_t) * MONO_SAVED_GREGS);
	memcpy (&UCONTEXT_REG_FPRn(uc, 14), &mctx->fregs, sizeof (double) * MONO_SAVED_FREGS);
}

#elif defined(__SH4__)

#include <mono/utils/mono-context.h>
#include <mono/arch/sh4/sh4-codegen.h>

void
mono_sigctx_to_monoctx (void *sigctx, MonoContext *mctx)
{
#ifdef MONO_CROSS_COMPILE
	g_assert_not_reached ();
#else
	ucontext_t *ctx = (ucontext_t*)sigctx;

	mctx->pc = ctx->uc_mcontext.pc;
	g_assert(sizeof(ctx->uc_mcontext.gregs) == sizeof(mctx->registers));
	memcpy(&mctx->registers, &ctx->uc_mcontext.gregs, sizeof(mctx->registers));
	g_assert(sizeof(ctx->uc_mcontext.fpregs) == sizeof(mctx->fregisters));
	memcpy(&mctx->fregisters, &ctx->uc_mcontext.fpregs, sizeof(mctx->fregisters));
#endif
}

void
mono_monoctx_to_sigctx (MonoContext *mctx, void *sigctx)
{
#ifdef MONO_CROSS_COMPILE
	g_assert_not_reached ();
#else
	ucontext_t *ctx = (ucontext_t*)sigctx;

	ctx->uc_mcontext.pc = mctx->pc;
	ctx->uc_stack.ss_sp = (void*)mctx->registers[sh4_sp];

	g_assert(sizeof(ctx->uc_mcontext.gregs) == sizeof(mctx->registers));
	memcpy(&ctx->uc_mcontext.gregs, &mctx->registers, sizeof(ctx->uc_mcontext.gregs));
	g_assert(sizeof(ctx->uc_mcontext.fpregs) == sizeof(mctx->fregisters));
	memcpy(&ctx->uc_mcontext.fpregs, &mctx->fregisters, sizeof(ctx->uc_mcontext.fpregs));
#endif
}

#endif /* #if defined(__i386__) */
