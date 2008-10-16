/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * mini-sh4.h: SH4 backend for the Mono code generator
 *
 * Authors:
 *   Cedric VINCENT (cedric.vincent@gmail.com)
 *
 * (C) 2008 STMicroelectronics.
 */

#ifndef MONO_SH4_H
#define MONO_SH4_H
#include <mono/arch/sh4/sh4-codegen.h>
#include <glib.h>


/*
 * Bitmask selecting the caller-saved floating-point registers,
 * these are usually used for local allocation. This macro is
 * mandatory even if the soft floating-point support is enabled.
*/
#define MONO_ARCH_CALLEE_FREGS 0xF

/*
 * Bitmask selecting the caller-saved general registers, these
 * are usually used for local allocation. This macro is mandatory.
*/
#define MONO_ARCH_CALLEE_REGS ((1<<sh4_r0) | (1<<sh4_r1) | (1<<sh4_r2) | (1<<sh4_r3) | (1<<sh4_r4) | (1<<sh4_r5) | (1<<sh4_r6) | (1<<sh4_r7))

/*
 * Bitmask selecting the callee-saved floating-point registers,
 * these are usually used for global allocation. This macro is
 * mandatory even if the soft floating-point support is enabled.
*/
#define MONO_ARCH_CALLEE_SAVED_FREGS 0

/*
 * Bitmask selecting the callee-saved general registers, these
 * are usually used for global allocation. This macro is mandatory.
*/
#define MONO_ARCH_CALLEE_SAVED_REGS ((1<<sh4_r8) | (1<<sh4_r9) | (1<<sh4_r10) | (1<<sh4_r11) | (1<<sh4_r12) | (1<<sh4_r13) | (1<<sh4_r14) | (1<<sh4_r15))

/*
 * This macro is not yet used.
*/
#define MONO_ARCH_CODE_ALIGNMENT 32 /* not used */

/*
 * This macro defines the name of the machine description generated
 * by genmdesc from cpu-<ARCH>.md during the build process. Obviously,
 * this macro is mandatory.
*/
#define MONO_ARCH_CPU_SPEC sh4_desc

/*
 * If this macro is defined, the opcode to convert an unsigned 32 bits
 * integer to a double float will be emulated.
*/
#define MONO_ARCH_EMULATE_CONV_R8_UN 1

/*
 * If this macro is defined, the division opcodes will be emulated.
*/
#define MONO_ARCH_EMULATE_DIV 1

/*
 * If this macro is defined, the opcode to convert a double float to
 * a 64 bits integer will be emulated.
*/
#define MONO_ARCH_EMULATE_FCONV_TO_I8 1

/*
 * Not used.
*/
#define MONO_ARCH_EMULATE_FREM 1

/*
 * If this macro is defined, the opcode to convert a 64 bits integer
 * to a float will be emulated.
*/
#define MONO_ARCH_EMULATE_LCONV_TO_R4 1

/*
 * If this macro is defined, the opcode to convert a 64 bits integer
 * to a double float will be emulated.
*/
#define MONO_ARCH_EMULATE_LCONV_TO_R8 1

/*
 * If this macro is defined, the opcode to convert from a unsigned 64
 * bits integer to a float will be emulated.
*/
#define MONO_ARCH_EMULATE_LCONV_TO_R8_UN 1

/*
 * Not used.
*/
#define MONO_ARCH_EMULATE_LMUL 1

/*
 * Not used.
*/
#define MONO_ARCH_EMULATE_LONG_MUL_OPTS 1

/*
 * If this macro is defined, the multiplication and division opcodes
 * will be emulated.
*/
#define MONO_ARCH_EMULATE_MUL_DIV 1

/*
 * This macro defines the size of the floating-point stack.
 * Use the value 0 if no floating-point stack is available.
*/
#define MONO_ARCH_FPSTACK_SIZE 0

/*
 * This macro defined the frame pointer alignment constraint.
*/
#define MONO_ARCH_FRAME_ALIGNMENT 32

/*
 * This macro provides, from the machine-description of an instruction,
 * the fixed hreg corresponding to that value. This macro is mandatory.
*/
#define MONO_ARCH_INST_FIXED_REG(desc) ((desc == '0') ? sh4_r0 : -1)

/*
 * This macro tests if a register denotes a register pair (from
 * the machine-description of an instruction). Usually it looks like
 * (desc == 'l' || desc == 'L'). This macro is mandatory.
*/
#define MONO_ARCH_INST_IS_REGPAIR(a) FALSE

/*
 * This macro provides, from the first register of a pair, the bitmask
 * selecting the hregs which can be used for allocating the second
 * register.
*/
#define MONO_ARCH_INST_REGPAIR_REG2(a, b) (-1)

/*
 * This macro is only used by the ARM support but other
 * architectures define it to "(0)".
*/
#define MONO_ARCH_INST_SREG2_MASK(a) (0)

/*
 * If this macro is defined, the software floating point support will
 * be enabled. This macro is required if your architecture does not
 * have hardware support for floating point.
*/
#define MONO_ARCH_SOFT_FLOAT 1

/*
 * This macro defines whether the architecture uses a floating point
 * stack. Obviously, this macro is mandatory.
*/
#define MONO_ARCH_USE_FPSTACK FALSE

/*
 * This macro defines whether the architecture supports POSIX sigactions.
 * This macro should be defined for all POSIX platforms.
*/
#define MONO_ARCH_USE_SIGACTION 1

/*
 * Get the frame pointer from a Mono context. This macro is
 * mandatory.
*/
#define MONO_CONTEXT_GET_BP(context) ((gpointer)(context)->registers[sh4_fp])

/*
 * Get the instruction pointer from a Mono context. This macro is
 * mandatory.
*/
#define MONO_CONTEXT_GET_IP(context) ((gpointer)(context)->pc)

/*
 * Get the stack pointer from a Mono context. This macro is mandatory.
*/
#define MONO_CONTEXT_GET_SP(context) ((gpointer)(context)->registers[sh4_sp])

/*
 * Set the frame pointer of a Mono context. This macro is
 * mandatory.
*/
#define MONO_CONTEXT_SET_BP(context, vbp) do { (context)->registers[sh4_fp] = (guint32)(vbp); } while (0);

/*
 * Set the instruction pointer of a Mono context. This macro is
 * mandatory.
*/
#define MONO_CONTEXT_SET_IP(context, vpc) do { (context)->pc = (guint32)(vpc); } while (0);

/*
 * Set the stack pointer of a Mono context.
*/
#define MONO_CONTEXT_SET_SP(context, vsp) do { (context)->registers[sh4_fp] = (guint32)(vsp); } while (0);

/*
 * This macro retrieves the Mono context from a function pointer. This
 * macro have to be defined if your architecture does not support
 * MONO_INIT_CONTEXT_FROM_CURRENT.
*/
#define MONO_INIT_CONTEXT_FROM_FUNC(context, func) do {			\
		MONO_CONTEXT_SET_IP((context), (func));			\
		MONO_CONTEXT_SET_BP((context), __builtin_frame_address(0)); \
		MONO_CONTEXT_SET_SP((context), __builtin_frame_address(0)); \
	} while (0)


/*
 * This macro defines the maximum number of floating point registers.
 * Obviously, this macro is mandatory.
*/
#define MONO_MAX_FREGS 16

/*
 * This macro defines the maximum number of integer registers (even ones
 * which have a special meaning). Obviously, this macro is mandatory.
*/
#define MONO_MAX_IREGS 16

/*
 * This macro defines the first argument used in the calling convention.
*/
#define MONO_SH4_REG_FIRST_ARG sh4_r4

/*
 * This macro defines the last argument used in the calling convention.
*/
#define MONO_SH4_REG_LAST_ARG sh4_r7

/* Structure where the arch-specific code can store
 * data during a compilation. */
#define SH4_MAX_CSTPOOL       255 /* Maximum number of constants in a pool */

/* Tackling constant pools */
typedef enum {
  cstpool_undef     = 0,
  cstpool_filling   = 1,   /* The only state where you can add data */
  cstpool_allocated = 2,
  cstpool_patched   = 3,
} CstPool_State;

typedef struct MonoSH4CstPool{
	struct  MonoSH4CstPool *next;
	CstPool_State state    ;
	guint32 pool_off0_start; /* |pc/2|*2 of this first instruction */
				 /* that contributes to the memory pool*/

	guint32  pool_off;       /* Base address of the pool           */
	guint32  pool_nbcst;     /* Number of constants in the pool    */

	/* Memorize constant - Actual type of field "type" is    */
	/* MonoJumpInfoType. It is defined in mini.h that        */
	/* includes mini-arch.h that includes it self mini-sh4.h.*/
	gint16          type    [SH4_MAX_CSTPOOL];
	guint16         reg     [SH4_MAX_CSTPOOL];
	gconstpointer   pool_cst[SH4_MAX_CSTPOOL];
	guint32         off_inst[SH4_MAX_CSTPOOL];

	guint32  pool_nbcst_emitted; /* Temporary -                   */
} MonoSH4CstPool;

typedef struct {
	MonoMemPool    *mempool;
	MonoSH4CstPool *start;
	MonoSH4CstPool *last;

	guint32         nbpool;        /* For statistics and debug only */
	guint32         nbcst;         /* For statistics and debug only */

	guint32         nb_bblocks;
	guint32        *tab_bb_offset;
} MonoSH4CstPool_Env;

typedef struct { 
	gint localloc_size;
	MonoSH4CstPool_Env *poolenv;
 } MonoCompileArch;

/* The execution state of a thread during exception handling
 * is stored in this arch-specific structure. */
typedef struct { 
	guint32 pc;
	guint32 registers[MONO_MAX_IREGS];
 } MonoContext;

/* When managed code needs to call into native code, it does through
 * a managed->native wrapper function, which is generated by the JIT.
 * This wrapper is responsible for saving the machine into the
 * following arch-specific structure. */
struct MonoLMF { 
	gpointer    previous_lmf;
	gpointer    lmf_addr;
	MonoMethod *method;
	guint32     pc;
	guint32     registers[MONO_MAX_IREGS];
	guint32     ebp; /* required by Mini but not used with the SH4. */
 };
#endif /* MONO_SH4_H */

#ifndef NDEBUG
#  define SH4_DEBUG(format, ...) fprintf(stderr, "! %s:%d: " format "\n", __FUNCTION__, __LINE__, __VA_ARGS__)
#  define SH4_CFG_DEBUG(LVL) if (cfg->verbose_level >= LVL)
#  define SH4_EXTRA_DEBUG(format, ...) if (sh4_extra_debug != 0) SH4_DEBUG(format, __VA_ARGS__)
#else
#  define SH4_DEBUG(format, ...) (0)
#  define SH4_CFG_DEBUG(LVL) if (0)
#  define SH4_EXTRA_DEBUG(format, ...) (0)
#endif /* NDEBUG */

