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

#define sh4_emit16(address, value) do {					   \
	*((guint16 *) (void *)(address)) = (guint16)value;		   \
	(address) = (typeof(address))(((guint16 *)(void *)(address)) + 1); \
} while(0)

#define sh4_emit32(address, value) do {					   \
	*((guint32 *) (void *)(address)) = (guint32)value;		   \
	(address) = (typeof(address))(((guint32 *)(void *)(address)) + 1); \
} while(0)

#endif /* __MONO_SH4_CODEGEN_HEADER_H__ */
