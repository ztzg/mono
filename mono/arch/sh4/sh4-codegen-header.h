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

#define sh4_emit16(address, value) do {					   \
	*((guint16 *) (void *)(address)) = (guint16)value;		   \
	(address) = (typeof(address))(((guint16 *)(void *)(address)) + 1); \
} while(0)

#define sh4_emit32(address, value) do {					   \
	*((guint32 *) (void *)(address)) = (guint32)value;		   \
	(address) = (typeof(address))(((guint32 *)(void *)(address)) + 1); \
} while(0)

#ifndef INT24_MIN
#  define INT24_MIN (-8388608)
#endif

#ifndef INT24_MAX
#  define INT24_MAX (8388607)
#endif

#ifndef UINT24_MAX
#  define UINT24_MAX (16777216U)
#endif

#define sh4_is_imm8(value)   ((value) > INT8_MIN  && (value) < INT8_MAX)
#define sh4_is_imm16(value)  ((value) > INT16_MIN && (value) < INT16_MAX)
#define sh4_is_imm24(value)  ((value) > INT24_MIN && (value) < INT24_MAX)
#define sh4_is_imm32(value)  ((value) > INT32_MIN && (value) < INT32_MAX)
#define sh4_is_imm64(value)  ((value) > INT64_MIN && (value) < INT64_MAX)
#define sh4_is_uimm8(value)  ((value) > 0 && (value) < UINT8_MAX)
#define sh4_is_uimm16(value) ((value) > 0 && (value) < UINT16_MAX)
#define sh4_is_uimm24(value) ((value) > 0 && (value) < UINT24_MAX)
#define sh4_is_uimm32(value) ((value) > 0 && (value) < UINT32_MAX)
#define sh4_is_uimm64(value) ((value) > 0 && (value) < UINT64_MAX)

#define sh4_build_uint(code, value, Rx)					\
	g_assert(value >= 0);						\
	g_assert(value <= UINT32_MAX);					\
	sh4_xor(buffer, (Rx), (Rx));					\
	if (sh4_is_uimm32((value)) && !sh4_is_uimm24((value))) {	\
		sh4_add_imm(buffer, ((value) & 0xFF000000) >> 24, (Rx)); \
		sh4_shll8(buffer, (Rx));				\
	}								\
	if (sh4_is_uimm24((value)) && !sh4_is_uimm16((value))) {	\
		sh4_add_imm(buffer, ((value) & 0x00FF0000) >> 16, (Rx)); \
		sh4_shll8(buffer, (Rx));				\
	}								\
	if (sh4_is_uimm16((value)) && !sh4_is_uimm8((value))) {		\
		sh4_add_imm(buffer, ((value) & 0x0000FF00) >> 8, (Rx));	\
		sh4_shll8(buffer, (Rx));				\
	}								\
	sh4_add_imm(buffer, (value) & 0x000000FF, (Rx));

#define sh4_movl_PCrel(code, address, Rx)	\
	sh4_movl_dispPC(code, (guint32)address - (((guint32)code + 4) & ~0x3), Rx)

#endif /* __MONO_SH4_CODEGEN_HEADER_H__ */
