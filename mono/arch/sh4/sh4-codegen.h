/* Copyright (C)  2008 STMicroelectronics */
#ifndef __MONO_SH4_CODEGEN_H__
#define __MONO_SH4_CODEGEN_H__

#include "sh4-codegen-header.h"

#define sh4_add_imm(code, imm, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)(imm) >= -128);		\
	g_assert((int)(imm) <= 127);		\
	sh4_emit16(code, (0x7 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_add(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
} while(0)

#define sh4_addc(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
} while(0)

#define sh4_addv(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
} while(0)

#define sh4_and_imm_R0(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0x9 << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_and(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
} while(0)

#define sh4_andb_imm_dispR0GBR(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0xD << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_bra(code, imm) do {		\
	g_assert((int)(imm) >= -4096);		\
	g_assert((int)(imm) <= 4094);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0xA << 12) | ((((imm) & 0x1FFE) >> 1) << 0)); \
} while(0)

#define sh4_bsr(code, imm) do {		\
	g_assert((int)(imm) >= -4096);		\
	g_assert((int)(imm) <= 4094);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0xB << 12) | ((((imm) & 0x1FFE) >> 1) << 0)); \
} while(0)

#define sh4_bt(code, imm) do {		\
	g_assert((int)(imm) >= -256);		\
	g_assert((int)(imm) <= 254);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x8 << 12) | (0x9 << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_bf(code, imm) do {		\
	g_assert((int)(imm) >= -256);		\
	g_assert((int)(imm) <= 254);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x8 << 12) | (0xB << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_bts(code, imm) do {		\
	g_assert((int)(imm) >= -256);		\
	g_assert((int)(imm) <= 254);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x8 << 12) | (0xD << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_bts(code, imm) do {		\
	g_assert((int)(imm) >= -256);		\
	g_assert((int)(imm) <= 254);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x8 << 12) | (0xD << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_bfs(code, imm) do {		\
	g_assert((int)(imm) >= -256);		\
	g_assert((int)(imm) <= 254);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x8 << 12) | (0xF << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_bfs(code, imm) do {		\
	g_assert((int)(imm) >= -256);		\
	g_assert((int)(imm) <= 254);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x8 << 12) | (0xF << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_clrmac(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_clrs(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x4 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_clrt(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_cmpeq_imm_R0(code, imm) do {		\
	g_assert((int)(imm) >= -128);		\
	g_assert((int)(imm) <= 127);		\
	sh4_emit16(code, (0x8 << 12) | (0x8 << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_cmpeq(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
} while(0)

#define sh4_cmpge(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x3 << 0)); \
} while(0)

#define sh4_cmpgt(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_cmphi(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
} while(0)

#define sh4_cmphs(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
} while(0)

#define sh4_cmppl(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x5 << 0)); \
} while(0)

#define sh4_cmppz(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x1 << 0)); \
} while(0)

#define sh4_cmpstr(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
} while(0)

#define sh4_div0s(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_div0u(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x9 << 0)); \
} while(0)

#define sh4_div1(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
} while(0)

#define sh4_extsb(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
} while(0)

#define sh4_extsw(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
} while(0)

#define sh4_extub(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
} while(0)

#define sh4_extuw(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
} while(0)

#define sh4_icbi_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xE << 4) | (0x3 << 0)); \
} while(0)

#define sh4_jmp_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xB << 0)); \
} while(0)

#define sh4_jsr_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xB << 0)); \
} while(0)

#define sh4_ldc_SR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xE << 0)); \
} while(0)

#define sh4_ldc_GBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xE << 0)); \
} while(0)

#define sh4_ldc_SGR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xA << 0)); \
} while(0)

#define sh4_ldc_VBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xE << 0)); \
} while(0)

#define sh4_ldc_SSR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xE << 0)); \
} while(0)

#define sh4_ldc_SPC(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0xE << 0)); \
} while(0)

#define sh4_ldc_DBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0xF << 4) | (0xA << 0)); \
} while(0)

#define sh4_ldc_bank(code, Rx, imm) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 7);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0x7) << 4) | (0xE << 0)); \
} while(0)

#define sh4_ldcl_incRx_SR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x7 << 0)); \
} while(0)

#define sh4_ldcl_incRx_GBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x7 << 0)); \
} while(0)

#define sh4_ldcl_incRx_VBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x7 << 0)); \
} while(0)

#define sh4_ldcl_incRx_SGR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x6 << 0)); \
} while(0)

#define sh4_ldcl_incRx_SSR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x7 << 0)); \
} while(0)

#define sh4_ldcl_incRx_SPC(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0x7 << 0)); \
} while(0)

#define sh4_ldcl_incRx_DBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0xF << 4) | (0x6 << 0)); \
} while(0)

#define sh4_ldcl_incRx_bank(code, Rx, imm) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 7);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0x7) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_lds_MACH(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xA << 0)); \
} while(0)

#define sh4_lds_MACL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xA << 0)); \
} while(0)

#define sh4_lds_PR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xA << 0)); \
} while(0)

#define sh4_lds_FPUL(code, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0x5 << 4) | (0xA << 0)); \
} while(0)

#define sh4_lds_FPSCR(code, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0x6 << 4) | (0xA << 0)); \
} while(0)

#define sh4_ldsl_incRx_MACH(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x6 << 0)); \
} while(0)

#define sh4_ldsl_incRx_MACL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x6 << 0)); \
} while(0)

#define sh4_ldsl_incRx_PR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x6 << 0)); \
} while(0)

#define sh4_ldsl_incRy_FPUL(code, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0x5 << 4) | (0x6 << 0)); \
} while(0)

#define sh4_ldsl_incRy_FPSCR(code, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0x6 << 4) | (0x6 << 0)); \
} while(0)

#define sh4_ldtlb(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x3 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_macw_incRy_incRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
} while(0)

#define sh4_mov_imm(code, imm, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)(imm) >= -128);		\
	g_assert((int)(imm) <= 127);		\
	sh4_emit16(code, (0xE << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_mov(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x3 << 0)); \
} while(0)

#define sh4_movb_dispR0Rx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
} while(0)

#define sh4_movb_decRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
} while(0)

#define sh4_movb_indRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
} while(0)

#define sh4_movb_dispRy_R0(code, imm, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 15);		\
	sh4_emit16(code, (0x8 << 12) | (0x4 << 8) | (((Ry) & 0xF) << 4) | (((imm) & 0xF) << 0)); \
} while(0)

#define sh4_movb_dispGBR_R0(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0x4 << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_movb_dispR0Ry(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
} while(0)

#define sh4_movb_incRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
} while(0)

#define sh4_movb_indRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
} while(0)

#define sh4_movb_R0_dispRy(code, imm, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 15);		\
	sh4_emit16(code, (0x8 << 12) | (0x0 << 8) | (((Ry) & 0xF) << 4) | (((imm) & 0xF) << 0)); \
} while(0)

#define sh4_movb_R0_dispGBR(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0x0 << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_movl_dispRx(code, Ry, imm, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 60);		\
	g_assert(((int)(imm) & 0x3) == 0);	\
	sh4_emit16(code, (0x1 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | ((((imm) & 0x3C) >> 2) << 0)); \
} while(0)

#define sh4_movl_dispR0Rx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
} while(0)

#define sh4_movl_decRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
} while(0)

#define sh4_movl_indRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
} while(0)

#define sh4_movl_dispRy(code, imm, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 60);		\
	g_assert(((int)(imm) & 0x3) == 0);	\
	sh4_emit16(code, (0x5 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | ((((imm) & 0x3C) >> 2) << 0)); \
} while(0)

#define sh4_movl_dispGBR_R0(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 1020);		\
	g_assert(((int)(imm) & 0x3) == 0);	\
	sh4_emit16(code, (0xC << 12) | (0x6 << 8) | ((((imm) & 0x3FC) >> 2) << 0)); \
} while(0)

#define sh4_movl_dispPC(code, imm, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 1020);		\
	g_assert(((int)(imm) & 0x3) == 0);	\
	sh4_emit16(code, (0xD << 12) | (((Rx) & 0xF)  << 8) | ((((imm) & 0x3FC) >> 2) << 0)); \
} while(0)

#define sh4_movl_dispR0Ry(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
} while(0)

#define sh4_movl_incRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
} while(0)

#define sh4_movl_indRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
} while(0)

#define sh4_movl_R0_dispGBR(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 1020);		\
	g_assert(((int)(imm) & 0x3) == 0);	\
	sh4_emit16(code, (0xC << 12) | (0x2 << 8) | ((((imm) & 0x3FC) >> 2) << 0)); \
} while(0)

#define sh4_movw_dispR0Rx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
} while(0)

#define sh4_movw_decRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
} while(0)

#define sh4_movw_indRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x1 << 0)); \
} while(0)

#define sh4_movw_dispRy_R0(code, imm, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 30);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x8 << 12) | (0x5 << 8) | (((Ry) & 0xF) << 4) | ((((imm) & 0x1E) >> 1) << 0)); \
} while(0)

#define sh4_movw_dispGBR_R0(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 510);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0xC << 12) | (0x5 << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_movw_dispPC(code, imm, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 510);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x9 << 12) | (((Rx) & 0xF)  << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_movw_dispR0Ry(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
} while(0)

#define sh4_movw_incRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
} while(0)

#define sh4_movw_indRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x1 << 0)); \
} while(0)

#define sh4_movw_R0_dispRy(code, imm, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 30);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0x8 << 12) | (0x1 << 8) | (((Ry) & 0xF) << 4) | ((((imm) & 0x1E) >> 1) << 0)); \
} while(0)

#define sh4_movw_R0_dispGBR(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 510);		\
	g_assert(((int)(imm) & 0x1) == 0);	\
	sh4_emit16(code, (0xC << 12) | (0x1 << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
} while(0)

#define sh4_mova_dispPC_R0(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 1020);		\
	g_assert(((int)(imm) & 0x3) == 0);	\
	sh4_emit16(code, (0xC << 12) | (0x7 << 8) | ((((imm) & 0x3FC) >> 2) << 0)); \
} while(0)

#define sh4_movcal_R0_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xC << 4) | (0x3 << 0)); \
} while(0)

#define sh4_movcol_R0_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x7 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_movlil_indRy_R0(code, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Ry) & 0xF) << 8) | (0x6 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_movt(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x9 << 0)); \
} while(0)

#define sh4_movual_indRy_R0(code, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0xA << 4) | (0x9 << 0)); \
} while(0)

#define sh4_movual_incRy_R0(code, Ry) do {		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0xE << 4) | (0x9 << 0)); \
} while(0)

#define sh4_mulsw(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
} while(0)

#define sh4_muls(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
} while(0)

#define sh4_mull(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_muluw(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
} while(0)

#define sh4_mulu(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
} while(0)

#define sh4_neg(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
} while(0)

#define sh4_negc(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
} while(0)

#define sh4_nop(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x9 << 0)); \
} while(0)

#define sh4_not(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_ocbi_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x9 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_ocbp_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xA << 4) | (0x3 << 0)); \
} while(0)

#define sh4_ocbwb_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xB << 4) | (0x3 << 0)); \
} while(0)

#define sh4_or_imm_R0(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0xB << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_or(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
} while(0)

#define sh4_orb_imm_dispR0GBR(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0xF << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_pref_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x8 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_prefi_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xD << 4) | (0x3 << 0)); \
} while(0)

#define sh4_rotcl(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x4 << 0)); \
} while(0)

#define sh4_rotcr(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x5 << 0)); \
} while(0)

#define sh4_rotl(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x4 << 0)); \
} while(0)

#define sh4_rotr(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x5 << 0)); \
} while(0)

#define sh4_rte(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0xB << 0)); \
} while(0)

#define sh4_rts(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0xB << 0)); \
} while(0)

#define sh4_sets(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x5 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_sett(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_shad(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
} while(0)

#define sh4_shld(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
} while(0)

#define sh4_shal(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x0 << 0)); \
} while(0)

#define sh4_shar(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x1 << 0)); \
} while(0)

#define sh4_shll(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x0 << 0)); \
} while(0)

#define sh4_shll16(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_shll2(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_shll8(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x8 << 0)); \
} while(0)

#define sh4_shlr(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x1 << 0)); \
} while(0)

#define sh4_shlr16(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x9 << 0)); \
} while(0)

#define sh4_shlr2(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x9 << 0)); \
} while(0)

#define sh4_shlr8(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x9 << 0)); \
} while(0)

#define sh4_sleep(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0xB << 0)); \
} while(0)

#define sh4_stc_SR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stc_GBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stc_VBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stc_SSR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stc_SPC(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stc_SGR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xA << 0)); \
} while(0)

#define sh4_stc_DBR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xF << 4) | (0xA << 0)); \
} while(0)

#define sh4_stc_bank(code, imm, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 7);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0x7) << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stcl_SR_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_stcl_VBR_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_stcl_SSR_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_stcl_SPC_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_stcl_GBR_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_stcl_SGR_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stcl_DBR_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0xF << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stcl_bank_decRx(code, imm, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 7);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0x7) << 4) | (0x3 << 0)); \
} while(0)

#define sh4_sts_MACH(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xA << 0)); \
} while(0)

#define sh4_sts_MACL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xA << 0)); \
} while(0)

#define sh4_sts_PR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xA << 0)); \
} while(0)

#define sh4_sts_FPUL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x5 << 4) | (0xA << 0)); \
} while(0)

#define sh4_sts_FPSCR(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x6 << 4) | (0xA << 0)); \
} while(0)

#define sh4_stsl_MACH_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stsl_MACL_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stsl_PR_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stsl_FPUL_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x5 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_stsl_FPSCR_decRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x6 << 4) | (0x2 << 0)); \
} while(0)

#define sh4_sub(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
} while(0)

#define sh4_subc(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
} while(0)

#define sh4_subv(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
} while(0)

#define sh4_swapb(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
} while(0)

#define sh4_swapw(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
} while(0)

#define sh4_synco(code) do {		\
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0xA << 4) | (0xB << 0)); \
} while(0)

#define sh4_tasb_indRx(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xB << 0)); \
} while(0)

#define sh4_trapa_imm(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0x3 << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_tst_imm_R0(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0x8 << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_tst(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
} while(0)

#define sh4_tstb_imm_dispR0GBR(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0xC << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_xor_imm_R0(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0xA << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_xor(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
} while(0)

#define sh4_xorb_imm_dispR0GBR(code, imm) do {		\
	g_assert((int)(imm) >= 0);		\
	g_assert((int)(imm) <= 255);		\
	sh4_emit16(code, (0xC << 12) | (0xE << 8) | (((imm) & 0xFF) << 0)); \
} while(0)

#define sh4_xtrct(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
} while(0)

#define sh4_dt(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x0 << 0)); \
} while(0)

#define sh4_dmulsl(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
} while(0)

#define sh4_dmulul(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
} while(0)

#define sh4_macl_incRy_incRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
} while(0)

#define sh4_braf(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_bsrf(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x3 << 0)); \
} while(0)

#define sh4_fabs(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x5 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fabs_double(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x5 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fadd(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
} while(0)

#define sh4_fadd_double(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert(!(Ry & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
} while(0)

#define sh4_fcmpeq(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
} while(0)

#define sh4_fcmpeq_double(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert(!(Ry & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
} while(0)

#define sh4_fcmpgt(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
} while(0)

#define sh4_fcmpgt_double(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert(!(Ry & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
} while(0)

#define sh4_fcnvds_double_FPUL(code, Rx) do {		\
	g_assert(!(Rx & 0x1));		\
	sh4_emit16(code, (0xF << 12) | ((((Rx) & 0xF) << 1)  << 8) | (0xB << 4) | (0xD << 0)); \
} while(0)

#define sh4_fcnvsd_FPUL_double(code, Rx) do {		\
	g_assert(!(Rx & 0x1));		\
	sh4_emit16(code, (0xF << 12) | ((((Rx) & 0xF) << 1)  << 8) | (0xA << 4) | (0xD << 0)); \
} while(0)

#define sh4_fdiv(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x3 << 0)); \
} while(0)

#define sh4_fdiv_double(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert(!(Ry & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x3 << 0)); \
} while(0)

#define sh4_fipr(code, Ry, Rx) do {		\
	g_assert(!((Rx & 0x3) || (Ry & 0x3)));		\
	sh4_emit16(code, (0xF << 12) | (((((Rx) & 0xF) << 2) | (((Ry) & 0xF) >> 2))  << 8) | (0xE << 4) | (0xD << 0)); \
} while(0)

#define sh4_fldi0(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x8 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fldi1(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x9 << 4) | (0xD << 0)); \
} while(0)

#define sh4_flds_FPUL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xD << 0)); \
} while(0)

#define sh4_float_FPUL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xD << 0)); \
} while(0)

#define sh4_float_FPUL_double(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fmac(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
} while(0)

#define sh4_fmov(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
} while(0)

#define sh4_fmov_Xdouble_Xdouble(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
} while(0)

#define sh4_fmov_indRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
} while(0)

#define sh4_fmov_indRy_Xdouble(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
} while(0)

#define sh4_fmov_indRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
} while(0)

#define sh4_fmov_Xdouble_indRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
} while(0)

#define sh4_fmov_incRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
} while(0)

#define sh4_fmov_incRy_Xdouble(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
} while(0)

#define sh4_fmov_decRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
} while(0)

#define sh4_fmov_Xdouble_decRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
} while(0)

#define sh4_fmov_dispR0Ry(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
} while(0)

#define sh4_fmov_dispR0Ry_Xdouble(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
} while(0)

#define sh4_fmov_dispR0Rx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_fmov_Xdouble_dispR0Rx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_fmovd_indRy_Xdouble(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
} while(0)

#define sh4_fmovd_Xdouble_indRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
} while(0)

#define sh4_fmovd_incRy_Xdouble(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
} while(0)

#define sh4_fmovd_Xdouble_decRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
} while(0)

#define sh4_fmovd_dispR0Ry_Xdouble(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
} while(0)

#define sh4_fmovd_Xdouble_dispR0Rx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_fmovs_indRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
} while(0)

#define sh4_fmovs_indRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
} while(0)

#define sh4_fmovs_incRy(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
} while(0)

#define sh4_fmovs_decRx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
} while(0)

#define sh4_fmovs_dispR0Ry(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
} while(0)

#define sh4_fmovs_dispR0Rx(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
} while(0)

#define sh4_fmul(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
} while(0)

#define sh4_fmul_double(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert(!(Ry & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
} while(0)

#define sh4_fneg(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fneg_double(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fpchg(code) do {		\
	sh4_emit16(code, (0xF << 12) | (0x7 << 8) | (0xF << 4) | (0xD << 0)); \
} while(0)

#define sh4_frchg(code) do {		\
	sh4_emit16(code, (0xF << 12) | (0xB << 8) | (0xF << 4) | (0xD << 0)); \
} while(0)

#define sh4_fsca_FPUL_double(code, Rx) do {		\
	g_assert(!(Rx & 0x1));		\
	sh4_emit16(code, (0xF << 12) | ((((Rx) & 0xF) << 1)  << 8) | (0xF << 4) | (0xD << 0)); \
} while(0)

#define sh4_fschg(code) do {		\
	sh4_emit16(code, (0xF << 12) | (0x3 << 8) | (0xF << 4) | (0xD << 0)); \
} while(0)

#define sh4_fsqrt(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x6 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fsqrt_double(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x6 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fsrra(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x7 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fsts_FPUL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xD << 0)); \
} while(0)

#define sh4_fsub(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x1 << 0)); \
} while(0)

#define sh4_fsub_double(code, Ry, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	g_assert((int)Ry >= 0);			\
	g_assert((int)Ry <= 15);		\
	g_assert(!(Ry & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x1 << 0)); \
} while(0)

#define sh4_ftrc_FPUL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xD << 0)); \
} while(0)

#define sh4_ftrc_double_FPUL(code, Rx) do {		\
	g_assert((int)Rx >= 0);			\
	g_assert((int)Rx <= 15);		\
	g_assert(!(Rx & 0x1));		\
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xD << 0)); \
} while(0)

#define sh4_ftrv(code, Rx) do {		\
	g_assert(!(Rx & 0x3));		\
	sh4_emit16(code, (0xF << 12) | (((((Rx) & 0xF) << 2) | 0x1)  << 8) | (0xF << 4) | (0xD << 0)); \
} while(0)


#endif /* __MONO_SH4_CODEGEN_H__ */

