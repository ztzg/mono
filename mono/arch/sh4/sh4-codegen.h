/* Copyright (C)  2008 STMicroelectronics
   for testing do the following: ./test | as -o test.o */

#ifndef __MONO_SH4_CODEGEN_H__
#define __MONO_SH4_CODEGEN_H__

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

/* TODO - CV */

#endif /* __MONO_SH4_CODEGEN_H__ */
