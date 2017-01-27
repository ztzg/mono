/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * mini-sh4.h: SH4 backend for the Mono code generator
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

#ifndef MONO_SH4_H
#define MONO_SH4_H
#include <mono/arch/sh4/sh4-codegen.h>
#include <mono/utils/mono-context.h>
#include <glib.h>

/*
 * Bitmask selecting the caller-saved floating-point registers,
 * these are usually used for local allocation. This macro is
 * mandatory even if the soft floating-point support is enabled.
 */
#define MONO_ARCH_CALLEE_FREGS ((1<<sh4_dr0) | (1<<sh4_dr2) | (1<<sh4_dr4) | (1<<sh4_dr6) | (1<<sh4_dr8) | (1<<sh4_dr10) | (1<<sh4_dr12) | (1<<sh4_dr14))

/*
 * Bitmask selecting the caller-saved general registers, these
 * are usually used for local allocation. This macro is mandatory.
 */
#define MONO_ARCH_CALLEE_REGS ((1<<sh4_r0) | (1<<sh4_r1) | (1<<sh4_r2) | (1<<sh4_r4) | (1<<sh4_r5) | (1<<sh4_r6) | (1<<sh4_r7))

/*
 * Bitmask selecting the callee-saved floating-point registers,
 * these are usually used for global allocation. This macro is
 * mandatory even if the soft floating-point support is enabled.
 */
/* Only Mono/AMD64 supports global floating-point register allocation. */
#define MONO_ARCH_CALLEE_SAVED_FREGS 0

/*
 * Bitmask selecting the callee-saved general registers, these
 * are usually used for global allocation. This macro is mandatory.
 */
#define MONO_ARCH_CALLEE_SAVED_REGS ((1<<sh4_r8) | (1<<sh4_r9) | (1<<sh4_r10) | (1<<sh4_r11) | (1<<sh4_r12) | (1<<sh4_r13) | (1<<sh4_r14))

/*
 * This macro is not yet used.
 */
#define MONO_ARCH_CODE_ALIGNMENT 32 /* not used */

/*
 * This macro defines the name of the machine description generated
 * by genmdesc from cpu-<ARCH>.md during the build process. Obviously,
 * this macro is mandatory.
 */
#define MONO_ARCH_CPU_SPEC mono_sh4_desc

/*
 * If this macro is defined, the opcode to convert an unsigned 32 bits
 * integer to a double float will be emulated.
 */
#define MONO_ARCH_EMULATE_CONV_R8_UN 1

/*
 * If this macro is defined, the multiplication/division opcodes will be emulated.
 */
#define MONO_ARCH_EMULATE_MUL_DIV 1
#define MONO_ARCH_NEED_DIV_CHECK 1
#define MONO_ARCH_EMULATE_DIV 1

/*
 * If this macro is defined, the opcode to convert a double float to
 * a 64 bits integer will be emulated. */
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
 * This macro is set when decomposition of some int operators is
 * made in target specific parts of the backend.
 */
#define MONO_ARCH_HAVE_DECOMPOSE_OPTS 1

/*
 * This macro is set when decomposition of long operators is
 * made in target specific parts of the backend.
 *
 * In our case, this decomposition is only partial and we rely
 * on the generic one in decompose.c.
 */
#define MONO_ARCH_HAVE_DECOMPOSE_LONG_OPTS 1

/*
 * TODO: COMMENT
 */
#define MONO_ARCH_HAVE_THROW_CORLIB_EXCEPTION 1

/*
 * This macro defines the size of the floating-point stack.
 * Use the value 0 if no floating-point stack is available.
 */
#define MONO_ARCH_FPSTACK_SIZE 0

/*
 * This macro defined the frame pointer alignment constraint.
 */
#define MONO_ARCH_FRAME_ALIGNMENT 32

/* Not yet supported.
#define MONO_ARCH_HAVE_TLS_GET 1
#define MONO_ARCH_ENABLE_MONO_LMF_VAR 1
*/

/*
 * This macro is used to initialize the top LMF entry.
 * The register used to store the frame pointer has to be initialized to -1.
 */
#define MONO_ARCH_INIT_TOP_LMF_ENTRY(lmf) do { (lmf)->registers[sh4_r14] = (guint32)-1; } while (0);

/*
 * This macro provides, from the machine-description of an instruction,
 * the fixed hreg corresponding to that value. This macro is mandatory.
 */
#define MONO_ARCH_INST_FIXED_REG(desc) ((desc) == 'z' || (desc) == 'Z' ? sh4_r0 : \
					(desc) == 'y' ? sh4_dr0 : -1)

/*
 * This macro tests if a register denotes a register pair (from
 * the machine-description of an instruction). Usually it looks like
 * (desc == 'l' || desc == 'L'). This macro is mandatory.
 */
#define MONO_ARCH_INST_IS_REGPAIR(desc) ((desc) == 'Z')

/*
 * This macro tests if a register denotes a floating point (from
 * the machine-description of an instruction). Usually it looks like
 * (desc == 'f').
 */
#define MONO_ARCH_INST_IS_FLOAT(desc) ((desc == 'f') || (desc == 'y'))

/*
 * This macro provides, from the first register of a pair, the bitmask
 * selecting the hregs which can be used for allocating the second
 * register.
 */
#define MONO_ARCH_INST_REGPAIR_REG2(desc, hreg) (((desc) == 'Z') ? sh4_r1 : -1)

/*
 * This macro is only used by the ARM support but other
 * architectures define it to "(0)".
 */
#define MONO_ARCH_INST_SREG2_MASK(a) (0)

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

/*
 * This macro defines the first float argument used in the calling convention.
 */
#define MONO_SH4_FREG_FIRST_ARG sh4_fr4

/*
 * This macro defines the last float argument used in the calling convention.
 */
#define MONO_SH4_FREG_LAST_ARG sh4_fr11

#define MONO_ARCH_HAVE_IMT 1
#define MONO_ARCH_HAVE_SIGCTX_TO_MONOCTX 1

#define MONO_ARCH_IMT_REG sh4_r1
#define MONO_ARCH_RGCTX_REG MONO_ARCH_IMT_REG
#define MONO_ARCH_VTABLE_REG sh4_r0

/* Structure where the arch-specific code can store
 * data during a compilation. */
typedef struct {
	void *poolenv;
} MonoCompileArch;

/* When managed code needs to call into native code, it does through
 * a managed->native wrapper function, which is generated by the JIT.
 * This wrapper is responsible for saving the machine state into the
 * following arch-specific structure. */
struct MonoLMF {
	gpointer    previous_lmf;
	gpointer    lmf_addr;
	MonoMethod *method;
	guint32     pc;
	guint32     registers[MONO_MAX_IREGS];
	guint32     fregisters[MONO_MAX_FREGS];
};

#ifndef NDEBUG
#  define SH4_DEBUG(format, ...) fprintf(stdout, "! %s:%d: " format "\n", __FUNCTION__, __LINE__, __VA_ARGS__)
#  define SH4_CFG_DEBUG(LVL) if (cfg->verbose_level >= LVL)
#  define SH4_EXTRA_DEBUG(format, ...) if (sh4_extra_debug != 0) SH4_DEBUG(format, __VA_ARGS__)
#else
#  define SH4_DEBUG(format, ...) (0)
#  define SH4_CFG_DEBUG(LVL) if (0)
#  define SH4_EXTRA_DEBUG(format, ...) (0)
#endif /* NDEBUG */

#endif /* MONO_SH4_H */

