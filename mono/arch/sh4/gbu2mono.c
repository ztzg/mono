/*
  Copyright (c) 2008, STMicroelectronics
  All rights reserved.

  Author: Cedric VINCENT <cedric.vincent@gmail.com>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of the authors nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdlib.h> /* NULL, */
#include <stdio.h>  /* printf(3), */
#include <string.h> /* strdup(3), */
#include <ctype.h>  /* isalnum(3), */
#include <assert.h> /* assert(3), */

#define DEFINE_TABLE /* Used in the following header. */
#include "sh-opc.h"

void printf_checks_imm(int sign_extended, int nb_bits, int factor)
{
	int min = 0;
	int max = 0;

	assert(nb_bits > 0);
	assert(factor > 0);

	if (sign_extended == 0) {
		min = 0;
		max = (1 << nb_bits) - 1;
	} else {
		min = - (1 << (nb_bits - 1));
		max = (1 << (nb_bits - 1)) - 1;
	}

	min *= factor;
	max *= factor;

	printf("	g_assert((int)(imm) >= %d);		\\\n", min);
	printf("	g_assert((int)(imm) <= %d);		\\\n", max);
	if (factor != 1)
		printf("	g_assert(((int)(imm) & 0x%x) == 0);	\\\n", factor - 1);

	return;
}

void printf_checks(const sh_nibble_type nibbles[9], int force_sign)
{
	int i = 0;

	for (i = 0; i < 9; i++) {
		switch (nibbles[i]) {
		case HEX_0:
		case HEX_1:
		case HEX_2:
		case HEX_3:
		case HEX_4:
		case HEX_5:
		case HEX_6:
		case HEX_7:
		case HEX_8:
		case HEX_9:
		case HEX_A:
		case HEX_B:
		case HEX_C:
		case HEX_D:
		case HEX_E:
		case HEX_F:
			break;

		case REG_N:
			printf("	g_assert((int)Rx >= 0);			\\\n");
			printf("	g_assert((int)Rx <= 15);		\\\n");
			break;

		case REG_M:
			printf("	g_assert((int)Ry >= 0);			\\\n");
			printf("	g_assert((int)Ry <= 15);		\\\n");
			break;

		case REG_B:
			printf_checks_imm(0, 3, 1);
			break;

		case IMM0_4:
		case IMM1_4:
			printf_checks_imm(0, 4, 1);
			break;

		case IMM0_4BY2:
		case IMM1_4BY2:
			printf_checks_imm(0, 4, 2);
			break;

		case IMM0_4BY4:
		case IMM1_4BY4:
			printf_checks_imm(0, 4, 4);
			break;

		case IMM0_8:
		case IMM1_8:
			printf_checks_imm(force_sign != 0 ? 1 : 0, 8, 1);
			break;

		case IMM0_8BY2:
		case IMM1_8BY2:
		case PCRELIMM_8BY2:
			printf_checks_imm(0, 8, 2);
			break;

		case IMM0_8BY4:
		case IMM1_8BY4:
		case PCRELIMM_8BY4:
			printf_checks_imm(0, 8, 4);
			break;

		case BRANCH_8:
			printf_checks_imm(1, 8, 2);
			break;

		case BRANCH_12:
			printf_checks_imm(1, 12, 2);
			break;

		default:
			assert(0);
			break;
		}
	}

	return;
}

void printf_nibbles(const sh_nibble_type nibbles[9])
{
	int i = 0;
	int length = 0;
	int value = 0;

	printf("	sh4_emit16(code, ");

	for (i = 0; i < 9; i++) {
		switch (nibbles[i]) {
		case HEX_0:
		case HEX_1:
		case HEX_2:
		case HEX_3:
		case HEX_4:
		case HEX_5:
		case HEX_6:
		case HEX_7:
		case HEX_8:
		case HEX_9:
		case HEX_A:
		case HEX_B:
		case HEX_C:
		case HEX_D:
		case HEX_E:
		case HEX_F:
			value = (int)nibbles[i] - (int)HEX_0;
			printf("(0x%X << %d)", value, 12 - length);
			length += 4;
			break;

		case REG_N:
			printf("(((Rx) & 0xF)  << %d)", 12 - length);
			length += 4;
			break;

		case REG_M:
			printf("(((Ry) & 0xF) << %d)", 12 - length);
			length += 4;
			break;

		case REG_B:
			printf("(((imm) & 0x7) << %d)", 12 - length);
			length += 4;
			break;

		case IMM0_4:
		case IMM1_4:
			printf("(((imm) & 0xF) << %d)", 12 - length);
			length += 4;
			break;

		case IMM0_4BY2:
		case IMM1_4BY2:
			printf("((((imm) & 0x1E) >> 1) << %d)", 12 - length);
			length += 4;
			break;

		case IMM0_4BY4:
		case IMM1_4BY4:
			printf("((((imm) & 0x3C) >> 2) << %d)", 12 - length);
			length += 4;
			break;

		case IMM0_8:
		case IMM1_8:
			printf("(((imm) & 0xFF) << %d)", 8 - length);
			length += 8;
			break;

		case BRANCH_8:
		case IMM0_8BY2:
		case IMM1_8BY2:
		case PCRELIMM_8BY2:
			printf("((((imm) & 0x1FE) >> 1) << %d)", 8 - length);
			length += 8;
			break;

		case IMM0_8BY4:
		case IMM1_8BY4:
		case PCRELIMM_8BY4:
			printf("((((imm) & 0x3FC) >> 2) << %d)", 8 - length);
			length += 8;
			break;

		case BRANCH_12:
			printf("((((imm) & 0x1FFE) >> 1) << %d)", 4 - length);
			length += 12;
			break;

		default:
			assert(0);
			break;
		}

		if (length == 16)
			break;
		else
			printf(" | ");
	}

	printf("); \\\n");
	assert(length == 16);

	return;
}

void printf_args(const sh_arg_type args[4])
{
	int i = 0;

	for (i = 0; i < 4; i++) {
		if (args[i] == A_END)
			break;

		switch (args[i]) {
		case A_DEC_N:
			printf("_decRx");
			break;

		case A_DISP_GBR:
			printf("_dispGBR");
			break;

		case A_DISP_PC:
			printf("_dispPC");
			break;

		case A_DISP_REG_M:
			printf("_dispRy");
			break;

		case A_DISP_REG_N:
			printf("_dispRx");
			break;

		case A_GBR:
			printf("_GBR");
			break;

		case A_IMM:
			printf("_imm");
			break;

		case A_INC_M:
			printf("_incRy");
			break;

		case A_INC_N:
			printf("_incRx");
			break;

		case A_IND_M:
			printf("_indRy");
			break;

		case A_IND_N:
			printf("_indRx");
			break;

		case A_IND_R0_REG_M:
			printf("_dispR0Ry");
			break;

		case A_IND_R0_REG_N:
			printf("_dispR0Rx");
			break;

		case A_MACH:
			printf("_MACH");
			break;

		case A_MACL:
			printf("_MACL");
			break;

		case A_PR:
			printf("_PR");
			break;

		case A_R0:
			printf("_R0");
			break;

		case A_R0_GBR:
			printf("_dispR0GBR");
			break;

		case A_REG_M:
			break;

		case A_REG_N:
			break;

		case A_REG_B:
			printf("_bank");
			break;

		case A_SR:
			printf("_SR");
			break;

		case A_VBR:
			printf("_VBR");
			break;

		case A_SSR:
			printf("_SSR");
			break;

		case A_SPC:
			printf("_SPC");
			break;

		case A_SGR:
			printf("_SGR");
			break;

		case A_DBR:
			printf("_DBR");
			break;

		case A_BDISP12:
			break;

		case A_BDISP8:
			break;

		default:
			assert(0);
			break;
		}
	}

	printf("(code");

	for (i = 0; i < 4; i++) {
		if (args[i] == A_END)
			break;

		switch (args[i]) {
		case A_DISP_REG_M:
			printf(", imm");
			printf(", Ry");
			break;

		case A_DISP_REG_N:
			printf(", imm");
			printf(", Rx");
			break;

		case A_DISP_GBR:
		case A_DISP_PC:
		case A_BDISP8:
			printf(", imm");
			break;

		case A_BDISP12:
			printf(", imm");
			break;

		case A_DEC_N:
		case A_REG_N:
		case A_INC_N:
		case A_IND_N:
			printf(", Rx");
			break;

		case A_INC_M:
		case A_IND_M:
		case A_REG_M:
			printf(", Ry");
			break;

		case A_GBR:
			break;

		case A_IMM:
			printf(", imm");
			break;

		case A_IND_R0_REG_M:
			printf(", Ry");
			break;

		case A_IND_R0_REG_N:
			printf(", Rx");
			break;

		case A_MACH:
		case A_MACL:
		case A_PR:
		case A_R0_GBR:
		case A_SR:
		case A_VBR:
		case A_SSR:
		case A_SPC:
		case A_SGR:
		case A_DBR:
		case A_R0:
			break;

		case A_REG_B:
			printf(", imm");
			break;

		default:
			assert(0);
			break;
		}
	}

	printf(") ");
}

void printf_header()
{
	printf("/* Copyright (C)  2008 STMicroelectronics\n");
	printf("   for testing do the following: ./test | as -o test.o */\n");
	printf("\n");
	printf("#ifndef __MONO_SH4_CODEGEN_H__\n");
	printf("#define __MONO_SH4_CODEGEN_H__\n");
	printf("\n");
	printf("#include \"glib.h\" /* g_assert(), */\n");
	printf("\n");
	printf("typedef enum {\n");
	printf("	sh4_r0 = 0,\n");
	printf("	sh4_r1,\n");
	printf("	sh4_r2,\n");
	printf("	sh4_r3,\n");
	printf("	sh4_r4,\n");
	printf("	sh4_r5,\n");
	printf("	sh4_r6,\n");
	printf("	sh4_r7,\n");
	printf("	sh4_r8,\n");
	printf("	sh4_r9,\n");
	printf("	sh4_r10,\n");
	printf("	sh4_r11,\n");
	printf("	sh4_r12,\n");
	printf("	sh4_r13,\n");
	printf("	sh4_r14,\n");
	printf("	sh4_r15,\n");
	printf("	sh4_gp = sh4_r12,\n");
	printf("	sh4_fp = sh4_r14,\n");
	printf("	sh4_sp = sh4_r15,\n");
	printf("	sh4_pr\n");
	printf("} SH4IntRegister;\n");
	printf("\n");
	printf("#define sh4_emit16(address, value) do {					   \\\n");
	printf("	*((guint16 *) (void *)(address)) = (guint16)value;		   \\\n");
	printf("	(address) = (typeof(address))(((guint16 *)(void *)(address)) + 1); \\\n");
	printf("} while(0)\n");
	printf("\n");
	printf("#define sh4_emit32(address, value) do {					   \\\n");
	printf("	*((guint32 *) (void *)(address)) = (guint32)value;		   \\\n");
	printf("	(address) = (typeof(address))(((guint32 *)(void *)(address)) + 1); \\\n");
	printf("} while(0)\n");
	printf("\n");

	return;
}

void printf_footer()
{
	printf("\n");
	printf("#endif /* __MONO_SH4_CODEGEN_H__ */\n");

	return;
}

int main(void)
{
	int i = 0;
	int j = 0;

	printf_header();

	for (i = 0; sh_table[i].name != (char *)0; i++) {
		int force_sign = 0;

		if ((sh_table[i].arch & arch_sh4a_nofpu) != arch_sh4a_nofpu)
			continue;

		if ((strcmp(sh_table[i].name, "add") == 0 &&
		     sh_table[i].arg[0] == A_IMM &&
		     sh_table[i].arg[1] == A_REG_N) ||
		    (strcmp(sh_table[i].name, "cmp/eq") == 0 &&
		     sh_table[i].arg[0] == A_IMM &&
		     sh_table[i].arg[1] == A_R0) ||
		    (strcmp(sh_table[i].name, "mov") == 0 &&
		     sh_table[i].arg[0] == A_IMM &&
		     sh_table[i].arg[1] == A_REG_N))
			force_sign = 1;

		printf("#define sh4_");
		for (j = 0; sh_table[i].name[j] != '\0'; j++)
			if (isalnum(sh_table[i].name[j]) != 0)
				printf("%c", sh_table[i].name[j]);
		printf_args(sh_table[i].arg);
		printf("do {		\\\n");
		printf_checks(sh_table[i].nibbles, force_sign);
		printf_nibbles(sh_table[i].nibbles);
		printf("} while(0)\n");
		printf("\n");
	}

	printf_footer();

	return 0;
}
