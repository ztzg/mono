/* Copyright (C)  2008 STMicroelectronics */

#ifndef __MONO_SH4_CODEGEN_HEADER_H__
#define __MONO_SH4_CODEGEN_HEADER_H__

#include "glib.h" /* g_assert(), */

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
	sh4_pr
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
	*(guint16*)(*address) = (guint16) value;
	(*address)+=2;
}


static inline void sh4_emit32(guint8 **address, unsigned int value) 
{
	*(guint32*)(*address) = (guint32) value;
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

#define sh4_movl_PCrel(code, address, Rx)	\
	sh4_movl_dispPC((code), (guint32)(address) - (((guint32)(*code) + 4) & ~0x3), Rx)

#endif /* __MONO_SH4_CODEGEN_HEADER_H__ */
