/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
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
 */

#ifndef __MONO_SH4_CODEGEN_HEADER_H__
#define __MONO_SH4_CODEGEN_HEADER_H__

#include "glib.h" /* g_assert(), */

/* This variable is defined into mini-sh4.c. If set to non zero with
   an external tool (as a debugger), some extra debug information are
   printed on stderr. */
extern int sh4_extra_debug;

typedef enum {
	sh4_r0 = 0,
	sh4_r1,
	sh4_r2,
	sh4_r3,
	sh4_r4,
	sh4_r5,
	sh4_r6,
	sh4_r7,
	sh4_r8,
	sh4_r9,
	sh4_r10,
	sh4_r11,
	sh4_r12,
	sh4_r13,
	sh4_r14,
	sh4_r15,
	sh4_gp = sh4_r12,
	sh4_fp = sh4_r14,
	sh4_sp = sh4_r15,
	sh4_temp = sh4_r3
} SH4IntRegister;

typedef enum {
	sh4_fr0 = 0,
	sh4_fr1,
	sh4_fr2,
	sh4_fr3,
	sh4_fr4,
	sh4_fr5,
	sh4_fr6,
	sh4_fr7,
	sh4_fr8,
	sh4_fr9,
	sh4_fr10,
	sh4_fr11,
	sh4_fr12,
	sh4_fr13,
	sh4_fr14,
	sh4_fr15,
	sh4_dr0 = sh4_fr0,
	sh4_dr2 = sh4_fr2,
	sh4_dr4 = sh4_fr4,
	sh4_dr6 = sh4_fr6,
	sh4_dr8 = sh4_fr8,
	sh4_dr10 = sh4_fr10,
	sh4_dr12 = sh4_fr12,
	sh4_dr14 = sh4_fr14,
	sh4_fv0 = sh4_fr0,
	sh4_fv4 = sh4_fr4,
	sh4_fv8 = sh4_fr8,
	sh4_fv12 = sh4_fr12
} SH4FloatRegister;

static inline void sh4_emit16(guint8 **address, unsigned int value) 
{
	*((guint16 *)(void *)(* address)) = (guint16) value;
	(*address)+=2;
}

static inline void sh4_emit32(guint8 **address, unsigned int value) 
{
	*((guint32 *)(void *)(* address)) = (guint32) value;
	(*address)+=4;
}

#ifndef INT24_MIN
#  define INT24_MIN (-8388608)
#endif

#ifndef INT24_MAX
#  define INT24_MAX (8388607)
#endif

#ifndef UINT24_MAX
#  define UINT24_MAX (16777216U)
#endif

#define SH4_IS_IMM8(value)   ((value) > INT8_MIN  && (value) < INT8_MAX)
#define SH4_IS_IMM16(value)  ((value) > INT16_MIN && (value) < INT16_MAX)
#define SH4_IS_IMM24(value)  ((value) > INT24_MIN && (value) < INT24_MAX)
#define SH4_IS_IMM32(value)  ((value) > INT32_MIN && (value) < INT32_MAX)
#define SH4_IS_IMM64(value)  ((value) > INT64_MIN && (value) < INT64_MAX)
#define SH4_IS_UIMM8(value)  ((value) > 0 && (value) < UINT8_MAX)
#define SH4_IS_UIMM16(value) ((value) > 0 && (value) < UINT16_MAX)
#define SH4_IS_UIMM24(value) ((value) > 0 && (value) < UINT24_MAX)
#define SH4_IS_UIMM32(value) ((value) > 0 && (value) < UINT32_MAX)
#define SH4_IS_UIMM64(value) ((value) > 0 && (value) < UINT64_MAX)

#define sh4_movl_PCrel(cfg, code, address, Rx)	\
	sh4_movl_dispPC((cfg), (code), (guint32)(address) - (((guint32)(*code) + 4) & ~0x3), Rx)

#define sh4_mova_PCrel_R0(cfg,code,address)     \
	sh4_mova_dispPC_R0((cfg), (code), (guint32)(address) - (((guint32)(*code) + 4) & ~0x3))

#define sh4_die(cfg, code) sh4_ldtlb(cfg, code)

#define SH4_LABEL(from, to) (guint32)((guint32)(to) - (guint32)(from) - 4)

#define SH4_CHECK_RANGE_bra_label(from, to) SH4_CHECK_RANGE_bra(SH4_LABEL((from), (to)))
#define SH4_CHECK_ALIGN_bra_label(from, to) SH4_CHECK_ALIGN_bra(SH4_LABEL((from), (to)))
#define sh4_bra_label(cfg, code, address) sh4_bra(cfg, code, SH4_LABEL(*code, address))

#define SH4_CHECK_RANGE_bt_label(from, to) SH4_CHECK_RANGE_bt(SH4_LABEL((from), (to)))
#define SH4_CHECK_ALIGN_bt_label(from, to) SH4_CHECK_ALIGN_bt(SH4_LABEL((from), (to)))
#define sh4_bt_label(cfg, code, address) sh4_bt(cfg, code, SH4_LABEL(*code, address))

#define SH4_CHECK_RANGE_bf_label(from, to) SH4_CHECK_RANGE_bf(SH4_LABEL((from), (to)))
#define SH4_CHECK_ALIGN_bf_label(from, to) SH4_CHECK_ALIGN_bf(SH4_LABEL((from), (to)))
#define sh4_bf_label(cfg, code, address) sh4_bf(cfg, code, SH4_LABEL(*code, address))

#endif /* __MONO_SH4_CODEGEN_HEADER_H__ */
