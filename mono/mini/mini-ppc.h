#ifndef __MONO_MINI_PPC_H__
#define __MONO_MINI_PPC_H__

#include <mono/arch/ppc/ppc-codegen.h>
#include <glib.h>

#ifdef __mono_ppc64__
#define MONO_ARCH_CPU_SPEC ppc64_cpu_desc
#else
#define MONO_ARCH_CPU_SPEC ppcg4
#endif

#define MONO_MAX_IREGS 32
#define MONO_MAX_FREGS 32

#define MONO_SAVED_GREGS 19
#define MONO_SAVED_FREGS 18

#define MONO_ARCH_FRAME_ALIGNMENT 16

/* fixme: align to 16byte instead of 32byte (we align to 32byte to get 
 * reproduceable results for benchmarks */
#define MONO_ARCH_CODE_ALIGNMENT 32

void ppc_patch (guchar *code, const guchar *target);
void ppc_patch_full (guchar *code, const guchar *target, gboolean is_fd);

struct MonoLMF {
	gpointer    previous_lmf;
	gpointer    lmf_addr;
	MonoMethod *method;
	gulong     ebp;
	gulong     eip;
	gulong     iregs [MONO_SAVED_GREGS]; /* 13..31 */
	gdouble    fregs [MONO_SAVED_FREGS]; /* 14..31 */
};

/* we define our own structure and we'll copy the data
 * from sigcontext/ucontext/mach when we need it.
 * This also makes us save stack space and time when copying
 * We might also want to add an additional field to propagate
 * the original context from the signal handler.
 */
typedef struct {
	gulong sc_ir;          // pc 
	gulong sc_sp;          // r1
	gulong regs [MONO_SAVED_GREGS];
	double fregs [MONO_SAVED_FREGS];
} MonoContext;

typedef struct MonoCompileArch {
	int fp_conv_var_offset;
} MonoCompileArch;

#ifdef __mono_ppc64__
#define MONO_ARCH_NO_EMULATE_LONG_SHIFT_OPS
#define MONO_ARCH_NO_EMULATE_LONG_MUL_OPTS
#define MONO_ARCH_HAVE_ATOMIC_ADD 1
#define PPC_USES_FUNCTION_DESCRIPTOR
#define MONO_ARCH_HAVE_TLS_GET 1
#define MONO_ARCH_ENABLE_MONITOR_IL_FASTPATH 1
#else /* must be __mono_ppc__ */
#if 0
/* enabling this for PPC32 causes hangs in the thread/delegate tests.
   So disable for now. */
#if defined(__linux__)
#define MONO_ARCH_ENABLE_MONITOR_IL_FASTPATH 1
#endif
#endif

#define MONO_ARCH_HAVE_TLS_GET 1
#define MONO_ARCH_EMULATE_FCONV_TO_I8 1
#define MONO_ARCH_EMULATE_LCONV_TO_R8 1
#define MONO_ARCH_EMULATE_LCONV_TO_R4 1
#endif

#define MONO_ARCH_EMULATE_LCONV_TO_R8_UN 1
#define MONO_ARCH_EMULATE_FREM 1
#define MONO_ARCH_BIGMUL_INTRINS 1
#define MONO_ARCH_HAVE_ATOMIC_CAS 1
//#define MONO_ARCH_ENABLE_EMIT_STATE_OPT 1

/* Parameters used by the register allocator */
#define MONO_ARCH_CALLEE_REGS ((0xff << ppc_r3) | (1 << ppc_r11) | (1 << ppc_r12))
#define MONO_ARCH_CALLEE_SAVED_REGS (0xfffff << ppc_r13) /* ppc_13 - ppc_31 */

#if defined(__APPLE__) || defined(__mono_ppc64__)
#define MONO_ARCH_CALLEE_FREGS (0x1fff << ppc_f1)
#else
#define MONO_ARCH_CALLEE_FREGS (0xff << ppc_f1)
#endif
#define MONO_ARCH_CALLEE_SAVED_FREGS (~(MONO_ARCH_CALLEE_FREGS | 1))

#define MONO_ARCH_USE_FPSTACK FALSE
#define MONO_ARCH_FPSTACK_SIZE 0

#ifdef __mono_ppc64__
#define MONO_ARCH_INST_FIXED_REG(desc) (((desc) == 'a')? ppc_r3:\
					((desc) == 'g'? ppc_f1:-1))
#define MONO_ARCH_INST_IS_REGPAIR(desc) FALSE
#define MONO_ARCH_INST_REGPAIR_REG2(desc,hreg1) (-1)
#else
#define MONO_ARCH_INST_FIXED_REG(desc) (((desc) == 'a')? ppc_r3:\
					((desc) == 'l')? ppc_r4:\
					((desc) == 'g'? ppc_f1:-1))
#define MONO_ARCH_INST_IS_REGPAIR(desc) (desc == 'l')
#define MONO_ARCH_INST_REGPAIR_REG2(desc,hreg1) (desc == 'l' ? ppc_r3 : -1)
#endif

#define MONO_ARCH_INST_SREG2_MASK(ins) (0)

#define MONO_ARCH_INST_IS_FLOAT(desc) ((desc == 'f') || (desc == 'g'))

/* deal with some of the ABI differences here */
#ifdef __APPLE__
#define PPC_RET_ADDR_OFFSET 8
#define PPC_STACK_PARAM_OFFSET 24
#define PPC_MINIMAL_STACK_SIZE 24
#define PPC_MINIMAL_PARAM_AREA_SIZE 0
#define PPC_FIRST_ARG_REG ppc_r3
#define PPC_LAST_ARG_REG ppc_r10
#define PPC_FIRST_FPARG_REG ppc_f1
#define PPC_LAST_FPARG_REG ppc_f13
#define PPC_PASS_STRUCTS_BY_VALUE 1
#else
/* Linux */
#ifdef __mono_ppc64__
#define PPC_RET_ADDR_OFFSET 16
#define PPC_STACK_PARAM_OFFSET 48
#define PPC_MINIMAL_STACK_SIZE 48
#define PPC_MINIMAL_PARAM_AREA_SIZE 64
#define PPC_LAST_FPARG_REG ppc_f13
#define PPC_PASS_STRUCTS_BY_VALUE 1
#define PPC_SMALL_RET_STRUCT_IN_REG 0
#define PPC_THREAD_PTR_REG ppc_r13
#else
#define PPC_RET_ADDR_OFFSET 4
#define PPC_STACK_PARAM_OFFSET 8
#define PPC_MINIMAL_STACK_SIZE 8
#define PPC_MINIMAL_PARAM_AREA_SIZE 0
#define PPC_LAST_FPARG_REG ppc_f8
#define PPC_PASS_STRUCTS_BY_VALUE 0
#define PPC_SMALL_RET_STRUCT_IN_REG 1
#define PPC_THREAD_PTR_REG ppc_r2
#endif
#define PPC_FIRST_ARG_REG ppc_r3
#define PPC_LAST_ARG_REG ppc_r10
#define PPC_FIRST_FPARG_REG ppc_f1
#endif

#if defined(HAVE_WORKING_SIGALTSTACK) && !defined(__APPLE__)
#define MONO_ARCH_SIGSEGV_ON_ALTSTACK 1
#define MONO_ARCH_SIGNAL_STACK_SIZE (12 * 1024)
#endif /* HAVE_WORKING_SIGALTSTACK */

#define MONO_ARCH_HAVE_CREATE_TRAMPOLINE_FROM_TOKEN
#define MONO_ARCH_HAVE_CREATE_DELEGATE_TRAMPOLINE
#define MONO_ARCH_HAVE_IMT 1
#define MONO_ARCH_IMT_REG ppc_r12
#define MONO_ARCH_COMMON_VTABLE_TRAMPOLINE 1

#define MONO_ARCH_VTABLE_REG	ppc_r12
#define MONO_ARCH_RGCTX_REG	ppc_r12

#define MONO_ARCH_NO_IOV_CHECK 1
#define MONO_ARCH_HAVE_DECOMPOSE_OPTS 1

#define MONO_ARCH_HAVE_GENERALIZED_IMT_THUNK 1

#define MONO_ARCH_NEED_DIV_CHECK 1

#define PPC_NUM_REG_ARGS (PPC_LAST_ARG_REG-PPC_FIRST_ARG_REG+1)
#define PPC_NUM_REG_FPARGS (PPC_LAST_FPARG_REG-PPC_FIRST_FPARG_REG+1)

/* we have the stack pointer, not the base pointer in sigcontext */
#define MONO_CONTEXT_SET_IP(ctx,ip) do { (ctx)->sc_ir = (gulong)ip; } while (0);
/* FIXME: should be called SET_SP */
#define MONO_CONTEXT_SET_BP(ctx,bp) do { (ctx)->sc_sp = (gulong)bp; } while (0);

#define MONO_CONTEXT_GET_IP(ctx) ((gpointer)((ctx)->sc_ir))
#define MONO_CONTEXT_GET_BP(ctx) ((gpointer)((ctx)->regs [ppc_r31-13]))
#define MONO_CONTEXT_GET_SP(ctx) ((gpointer)((ctx)->sc_sp))

#ifdef __APPLE__

typedef struct {
	unsigned long sp;
	unsigned long unused1;
	unsigned long lr;
} MonoPPCStackFrame;

#define MONO_INIT_CONTEXT_FROM_FUNC(ctx,start_func) do {	\
		gpointer r1;					\
		__asm__ volatile("mr   %0,r1" : "=r" (r1));	\
		MONO_CONTEXT_SET_BP ((ctx), r1);		\
		MONO_CONTEXT_SET_IP ((ctx), (start_func));	\
	} while (0)

#else

typedef struct {
	unsigned long sp;
#ifdef __mono_ppc64__
	unsigned long cr;
#endif
	unsigned long lr;
} MonoPPCStackFrame;

#ifdef G_COMPILER_CODEWARRIOR
#define MONO_INIT_CONTEXT_FROM_FUNC(ctx,start_func) do {	\
		register gpointer r1_var;					\
		asm { mr r1_var, r1 };	\
		MONO_CONTEXT_SET_BP ((ctx), r1);		\
		MONO_CONTEXT_SET_IP ((ctx), (start_func));	\
	} while (0)

#else
#define MONO_INIT_CONTEXT_FROM_FUNC(ctx,start_func) do {	\
		gpointer r1;					\
		__asm__ volatile("mr   %0,1" : "=r" (r1));	\
		MONO_CONTEXT_SET_BP ((ctx), r1);		\
		MONO_CONTEXT_SET_IP ((ctx), (start_func));	\
	} while (0)

#endif
#endif

typedef struct {
	gint8 reg;
	gint8 size;
	int vtsize;
	int offset;
} MonoPPCArgInfo;

#ifdef PPC_USES_FUNCTION_DESCRIPTOR
typedef struct {
	gpointer code;
	gpointer toc;
	gpointer env;
} MonoPPCFunctionDescriptor;

#define PPC_FTNPTR_SIZE			sizeof (MonoPPCFunctionDescriptor)
extern guint8* mono_ppc_create_pre_code_ftnptr (guint8 *code) MONO_INTERNAL;
#else
#define PPC_FTNPTR_SIZE			0
#define mono_ppc_create_pre_code_ftnptr(c)	c
#endif

#if defined(__linux__)
	typedef struct ucontext os_ucontext;

#define MONO_ARCH_USE_SIGACTION 1

#ifdef __mono_ppc64__
	#define UCONTEXT_REG_Rn(ctx, n)   ((ctx)->uc_mcontext.gp_regs [(n)])
	#define UCONTEXT_REG_FPRn(ctx, n) ((ctx)->uc_mcontext.fp_regs [(n)])
	#define UCONTEXT_REG_NIP(ctx)     ((ctx)->uc_mcontext.gp_regs [PT_NIP])
	#define UCONTEXT_REG_LNK(ctx)     ((ctx)->uc_mcontext.gp_regs [PT_LNK])
#else
	#define UCONTEXT_REG_Rn(ctx, n)   ((ctx)->uc_mcontext.uc_regs->gregs [(n)])
	#define UCONTEXT_REG_FPRn(ctx, n) ((ctx)->uc_mcontext.uc_regs->fpregs.fpregs [(n)])
	#define UCONTEXT_REG_NIP(ctx)     ((ctx)->uc_mcontext.uc_regs->gregs [PT_NIP])
	#define UCONTEXT_REG_LNK(ctx)     ((ctx)->uc_mcontext.uc_regs->gregs [PT_LNK])
#endif
#elif defined (__APPLE__) && defined (_STRUCT_MCONTEXT)
#define MONO_ARCH_USE_SIGACTION 1
	typedef struct __darwin_ucontext os_ucontext;

	#define UCONTEXT_REG_Rn(ctx, n)   ((&(ctx)->uc_mcontext->__ss.__r0) [(n)])
	#define UCONTEXT_REG_FPRn(ctx, n) ((ctx)->uc_mcontext->__fs.__fpregs [(n)])
	#define UCONTEXT_REG_NIP(ctx)     ((ctx)->uc_mcontext->__ss.__srr0)
	#define UCONTEXT_REG_LNK(ctx)     ((ctx)->uc_mcontext->__ss.__lr)
#elif defined (__APPLE__) && !defined (_STRUCT_MCONTEXT)
#define MONO_ARCH_USE_SIGACTION 1
	typedef struct ucontext os_ucontext;

	#define UCONTEXT_REG_Rn(ctx, n)   ((&(ctx)->uc_mcontext->ss.r0) [(n)])
	#define UCONTEXT_REG_FPRn(ctx, n) ((ctx)->uc_mcontext->fs.fpregs [(n)])
	#define UCONTEXT_REG_NIP(ctx)     ((ctx)->uc_mcontext->ss.srr0)
	#define UCONTEXT_REG_LNK(ctx)     ((ctx)->uc_mcontext->ss.lr)
#elif defined(__NetBSD__)
#define MONO_ARCH_USE_SIGACTION 1
	typedef ucontext_t os_ucontext;

	#define UCONTEXT_REG_Rn(ctx, n)   ((ctx)->uc_mcontext.__gregs [(n)])
	#define UCONTEXT_REG_FPRn(ctx, n) ((ctx)->uc_mcontext.__fpregs.__fpu_regs [(n)])
	#define UCONTEXT_REG_NIP(ctx)     _UC_MACHINE_PC(ctx)
	#define UCONTEXT_REG_LNK(ctx)     ((ctx)->uc_mcontext.__gregs [_REG_LR])
#else
/* For other operating systems, we pull the definition from an external file */
#include "mini-ppc-os.h"
#endif

#ifdef __mono_ppc64__
#define MONO_PPC_32_64_CASE(c32,c64)	c64
extern void mono_ppc_emitted (guint8 *code, ssize_t length, const char *format, ...);
#else
#define MONO_PPC_32_64_CASE(c32,c64)	c32
#endif

extern gboolean mono_ppc_is_direct_call_sequence (guint32 *code);

#endif /* __MONO_MINI_PPC_H__ */
