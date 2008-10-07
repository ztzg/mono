/* Copyright (C)  2008 STMicroelectronics */
#ifndef __MONO_SH4_CODEGEN_H__
#define __MONO_SH4_CODEGEN_H__

#include "sh4-codegen-header.h"
#include <execinfo.h>


#define SH4_CHECK_RANGE_add_imm(imm) ((int)(imm) >= -128 && (int)(imm) <= 127)
static inline void sh4_add_imm(guint8 **code,int imm,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(SH4_CHECK_RANGE_add_imm(imm));
	sh4_emit16(code, (0x7 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_add(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_addc(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_addv(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
}

#define SH4_CHECK_RANGE_and_imm_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_and_imm_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_and_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x9 << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_and(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
}

#define SH4_CHECK_RANGE_andb_imm_dispR0GBR(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_andb_imm_dispR0GBR(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_andb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xD << 8) | (((imm) & 0xFF) << 0)); \
}

#define SH4_CHECK_RANGE_bra(imm) ((int)(imm) >= -4096 && (int)(imm) <= 4094)
#define SH4_CHECK_ALIGN_bra(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_bra(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_bra(imm));
	g_assert(SH4_CHECK_ALIGN_bra(imm));
	sh4_emit16(code, (0xA << 12) | ((((imm) & 0x1FFE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bsr(imm) ((int)(imm) >= -4096 && (int)(imm) <= 4094)
#define SH4_CHECK_ALIGN_bsr(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_bsr(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_bsr(imm));
	g_assert(SH4_CHECK_ALIGN_bsr(imm));
	sh4_emit16(code, (0xB << 12) | ((((imm) & 0x1FFE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bt(imm) ((int)(imm) >= -256 && (int)(imm) <= 254)
#define SH4_CHECK_ALIGN_bt(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_bt(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_bt(imm));
	g_assert(SH4_CHECK_ALIGN_bt(imm));
	sh4_emit16(code, (0x8 << 12) | (0x9 << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bf(imm) ((int)(imm) >= -256 && (int)(imm) <= 254)
#define SH4_CHECK_ALIGN_bf(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_bf(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_bf(imm));
	g_assert(SH4_CHECK_ALIGN_bf(imm));
	sh4_emit16(code, (0x8 << 12) | (0xB << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bts(imm) ((int)(imm) >= -256 && (int)(imm) <= 254)
#define SH4_CHECK_ALIGN_bts(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_bts(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_bts(imm));
	g_assert(SH4_CHECK_ALIGN_bts(imm));
	sh4_emit16(code, (0x8 << 12) | (0xD << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bfs(imm) ((int)(imm) >= -256 && (int)(imm) <= 254)
#define SH4_CHECK_ALIGN_bfs(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_bfs(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_bfs(imm));
	g_assert(SH4_CHECK_ALIGN_bfs(imm));
	sh4_emit16(code, (0x8 << 12) | (0xF << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
}

static inline void sh4_clrmac(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0x8 << 0)); \
}

static inline void sh4_clrs(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x4 << 4) | (0x8 << 0)); \
}

static inline void sh4_clrt(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x8 << 0)); \
}

#define SH4_CHECK_RANGE_cmpeq_imm_R0(imm) ((int)(imm) >= -128 && (int)(imm) <= 127)
static inline void sh4_cmpeq_imm_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_cmpeq_imm_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x8 << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_cmpeq(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
}

static inline void sh4_cmpge(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x3 << 0)); \
}

static inline void sh4_cmpgt(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_cmphi(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_cmphs(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
}

static inline void sh4_cmppl(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x5 << 0)); \
}

static inline void sh4_cmppz(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x1 << 0)); \
}

static inline void sh4_cmpstr(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_div0s(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_div0u(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x9 << 0)); \
}

static inline void sh4_div1(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_extsb(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_extsw(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
}

static inline void sh4_extub(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_extuw(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_icbi_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xE << 4) | (0x3 << 0)); \
}

static inline void sh4_jmp_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xB << 0)); \
}

static inline void sh4_jsr_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xB << 0)); \
}

static inline void sh4_ldc_SR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_GBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_SGR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xA << 0)); \
}

static inline void sh4_ldc_VBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_SSR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_SPC(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_DBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0xF << 4) | (0xA << 0)); \
}

#define SH4_CHECK_RANGE_ldc_bank(imm) ((int)(imm) >= 0 && (int)(imm) <= 7)
static inline void sh4_ldc_bank(guint8 **code,SH4IntRegister Rx,int imm) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(SH4_CHECK_RANGE_ldc_bank(imm));
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0x7) << 4) | (0xE << 0)); \
}

static inline void sh4_ldcl_incRx_SR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_GBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_VBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_SGR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldcl_incRx_SSR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_SPC(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_DBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0xF << 4) | (0x6 << 0)); \
}

#define SH4_CHECK_RANGE_ldcl_incRx_bank(imm) ((int)(imm) >= 0 && (int)(imm) <= 7)
static inline void sh4_ldcl_incRx_bank(guint8 **code,SH4IntRegister Rx,int imm) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(SH4_CHECK_RANGE_ldcl_incRx_bank(imm));
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0x7) << 4) | (0x7 << 0)); \
}

static inline void sh4_lds_MACH(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xA << 0)); \
}

static inline void sh4_lds_MACL(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xA << 0)); \
}

static inline void sh4_lds_PR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xA << 0)); \
}

static inline void sh4_lds_FPUL(guint8 **code,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0x5 << 4) | (0xA << 0)); \
}

static inline void sh4_lds_FPSCR(guint8 **code,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0x6 << 4) | (0xA << 0)); \
}

static inline void sh4_ldsl_incRx_MACH(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldsl_incRx_MACL(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldsl_incRx_PR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldsl_incRy_FPUL(guint8 **code,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0x5 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldsl_incRy_FPSCR(guint8 **code,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0x6 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldtlb(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x3 << 4) | (0x8 << 0)); \
}

static inline void sh4_macw_incRy_incRx(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
}

#define SH4_CHECK_RANGE_mov_imm(imm) ((int)(imm) >= -128 && (int)(imm) <= 127)
static inline void sh4_mov_imm(guint8 **code,int imm,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(SH4_CHECK_RANGE_mov_imm(imm));
	sh4_emit16(code, (0xE << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_mov(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x3 << 0)); \
}

static inline void sh4_movb_dispR0Rx(guint8 **code,SH4IntRegister Ry,SH4IntRegister  Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_movb_decRx(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_movb_indRx(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
}

#define SH4_CHECK_RANGE_movb_dispRy_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 15)
static inline void sh4_movb_dispRy_R0(guint8 **code,int imm,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movb_dispRy_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x4 << 8) | (((Ry) & 0xF) << 4) | (((imm) & 0xF) << 0)); \
}

#define SH4_CHECK_RANGE_movb_dispGBR_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_movb_dispGBR_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_movb_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x4 << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_movb_dispR0Ry(guint8 **code,SH4IntRegister  Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_movb_incRy(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_movb_indRy(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
}

#define SH4_CHECK_RANGE_movb_R0_dispRy(imm) ((int)(imm) >= 0 && (int)(imm) <= 15)
static inline void sh4_movb_R0_dispRy(guint8 **code,int imm,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movb_R0_dispRy(imm));
	sh4_emit16(code, (0x8 << 12) | (0x0 << 8) | (((Ry) & 0xF) << 4) | (((imm) & 0xF) << 0)); \
}

#define SH4_CHECK_RANGE_movb_R0_dispGBR(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_movb_R0_dispGBR(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_movb_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x0 << 8) | (((imm) & 0xFF) << 0)); \
}

#define SH4_CHECK_RANGE_movl_dispRx(imm) ((int)(imm) >= 0 && (int)(imm) <= 60)
#define SH4_CHECK_ALIGN_movl_dispRx(imm) (((int)(imm) & 0x3) == 0)
static inline void sh4_movl_dispRx(guint8 **code,SH4IntRegister Ry,int imm,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movl_dispRx(imm));
	g_assert(SH4_CHECK_ALIGN_movl_dispRx(imm));
	sh4_emit16(code, (0x1 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | ((((imm) & 0x3C) >> 2) << 0)); \
}

static inline void sh4_movl_dispR0Rx(guint8 **code,SH4IntRegister Ry,SH4IntRegister  Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_movl_decRx(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_movl_indRx(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
}

#define SH4_CHECK_RANGE_movl_dispRy(imm) ((int)(imm) >= 0 && (int)(imm) <= 60)
#define SH4_CHECK_ALIGN_movl_dispRy(imm) (((int)(imm) & 0x3) == 0)
static inline void sh4_movl_dispRy(guint8 **code,int imm,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movl_dispRy(imm));
	g_assert(SH4_CHECK_ALIGN_movl_dispRy(imm));
	sh4_emit16(code, (0x5 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | ((((imm) & 0x3C) >> 2) << 0)); \
}

#define SH4_CHECK_RANGE_movl_dispGBR_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_dispGBR_R0(imm) (((int)(imm) & 0x3) == 0)
static inline void sh4_movl_dispGBR_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_movl_dispGBR_R0(imm));
	g_assert(SH4_CHECK_ALIGN_movl_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x6 << 8) | ((((imm) & 0x3FC) >> 2) << 0)); \
}

#define SH4_CHECK_RANGE_movl_dispPC(imm) ((int)(imm) >= 0 && (int)(imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_dispPC(imm) (((int)(imm) & 0x3) == 0)
static inline void sh4_movl_dispPC(guint8 **code,int imm,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(SH4_CHECK_RANGE_movl_dispPC(imm));
	g_assert(SH4_CHECK_ALIGN_movl_dispPC(imm));
	sh4_emit16(code, (0xD << 12) | (((Rx) & 0xF)  << 8) | ((((imm) & 0x3FC) >> 2) << 0)); \
}

static inline void sh4_movl_dispR0Ry(guint8 **code,SH4IntRegister  Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_movl_incRy(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_movl_indRy(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
}

#define SH4_CHECK_RANGE_movl_R0_dispGBR(imm) ((int)(imm) >= 0 && (int)(imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_R0_dispGBR(imm) (((int)(imm) & 0x3) == 0)
static inline void sh4_movl_R0_dispGBR(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_movl_R0_dispGBR(imm));
	g_assert(SH4_CHECK_ALIGN_movl_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x2 << 8) | ((((imm) & 0x3FC) >> 2) << 0)); \
}

static inline void sh4_movw_dispR0Rx(guint8 **code,SH4IntRegister Ry,SH4IntRegister  Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_movw_decRx(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_movw_indRx(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x1 << 0)); \
}

#define SH4_CHECK_RANGE_movw_dispRy_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 30)
#define SH4_CHECK_ALIGN_movw_dispRy_R0(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_movw_dispRy_R0(guint8 **code,int imm,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movw_dispRy_R0(imm));
	g_assert(SH4_CHECK_ALIGN_movw_dispRy_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x5 << 8) | (((Ry) & 0xF) << 4) | ((((imm) & 0x1E) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_movw_dispGBR_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 510)
#define SH4_CHECK_ALIGN_movw_dispGBR_R0(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_movw_dispGBR_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_movw_dispGBR_R0(imm));
	g_assert(SH4_CHECK_ALIGN_movw_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x5 << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_movw_dispPC(imm) ((int)(imm) >= 0 && (int)(imm) <= 510)
#define SH4_CHECK_ALIGN_movw_dispPC(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_movw_dispPC(guint8 **code,int imm,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(SH4_CHECK_RANGE_movw_dispPC(imm));
	g_assert(SH4_CHECK_ALIGN_movw_dispPC(imm));
	sh4_emit16(code, (0x9 << 12) | (((Rx) & 0xF)  << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
}

static inline void sh4_movw_dispR0Ry(guint8 **code,SH4IntRegister  Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_movw_incRy(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_movw_indRy(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x1 << 0)); \
}

#define SH4_CHECK_RANGE_movw_R0_dispRy(imm) ((int)(imm) >= 0 && (int)(imm) <= 30)
#define SH4_CHECK_ALIGN_movw_R0_dispRy(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_movw_R0_dispRy(guint8 **code,int imm,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movw_R0_dispRy(imm));
	g_assert(SH4_CHECK_ALIGN_movw_R0_dispRy(imm));
	sh4_emit16(code, (0x8 << 12) | (0x1 << 8) | (((Ry) & 0xF) << 4) | ((((imm) & 0x1E) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_movw_R0_dispGBR(imm) ((int)(imm) >= 0 && (int)(imm) <= 510)
#define SH4_CHECK_ALIGN_movw_R0_dispGBR(imm) (((int)(imm) & 0x1) == 0)
static inline void sh4_movw_R0_dispGBR(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_movw_R0_dispGBR(imm));
	g_assert(SH4_CHECK_ALIGN_movw_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x1 << 8) | ((((imm) & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_mova_dispPC_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 1020)
#define SH4_CHECK_ALIGN_mova_dispPC_R0(imm) (((int)(imm) & 0x3) == 0)
static inline void sh4_mova_dispPC_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_mova_dispPC_R0(imm));
	g_assert(SH4_CHECK_ALIGN_mova_dispPC_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x7 << 8) | ((((imm) & 0x3FC) >> 2) << 0)); \
}

static inline void sh4_movcal_R0_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xC << 4) | (0x3 << 0)); \
}

static inline void sh4_movcol_R0_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x7 << 4) | (0x3 << 0)); \
}

static inline void sh4_movlil_indRy_R0(guint8 **code,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Ry) & 0xF) << 8) | (0x6 << 4) | (0x3 << 0)); \
}

static inline void sh4_movt(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x9 << 0)); \
}

static inline void sh4_movual_indRy_R0(guint8 **code,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0xA << 4) | (0x9 << 0)); \
}

static inline void sh4_movual_incRy_R0(guint8 **code,SH4IntRegister Ry) 
{
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Ry) & 0xF) << 8) | (0xE << 4) | (0x9 << 0)); \
}

static inline void sh4_mulsw(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
}

static inline void sh4_muls(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
}

static inline void sh4_mull(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_muluw(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_mulu(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_neg(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_negc(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_nop(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x9 << 0)); \
}

static inline void sh4_not(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_ocbi_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x9 << 4) | (0x3 << 0)); \
}

static inline void sh4_ocbp_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xA << 4) | (0x3 << 0)); \
}

static inline void sh4_ocbwb_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xB << 4) | (0x3 << 0)); \
}

#define SH4_CHECK_RANGE_or_imm_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_or_imm_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_or_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0xB << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_or(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
}

#define SH4_CHECK_RANGE_orb_imm_dispR0GBR(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_orb_imm_dispR0GBR(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_orb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xF << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_pref_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x8 << 4) | (0x3 << 0)); \
}

static inline void sh4_prefi_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xD << 4) | (0x3 << 0)); \
}

static inline void sh4_rotcl(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x4 << 0)); \
}

static inline void sh4_rotcr(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x5 << 0)); \
}

static inline void sh4_rotl(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x4 << 0)); \
}

static inline void sh4_rotr(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x5 << 0)); \
}

static inline void sh4_rte(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0xB << 0)); \
}

static inline void sh4_rts(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0xB << 0)); \
}

static inline void sh4_sets(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x5 << 4) | (0x8 << 0)); \
}

static inline void sh4_sett(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x8 << 0)); \
}

static inline void sh4_shad(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_shld(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_shal(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x0 << 0)); \
}

static inline void sh4_shar(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x1 << 0)); \
}

static inline void sh4_shll(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x0 << 0)); \
}

static inline void sh4_shll16(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x8 << 0)); \
}

static inline void sh4_shll2(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x8 << 0)); \
}

static inline void sh4_shll8(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x8 << 0)); \
}

static inline void sh4_shlr(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x1 << 0)); \
}

static inline void sh4_shlr16(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x9 << 0)); \
}

static inline void sh4_shlr2(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x9 << 0)); \
}

static inline void sh4_shlr8(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x9 << 0)); \
}

static inline void sh4_sleep(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0xB << 0)); \
}

static inline void sh4_stc_SR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_GBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_VBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_SSR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_SPC(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_SGR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xA << 0)); \
}

static inline void sh4_stc_DBR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0xF << 4) | (0xA << 0)); \
}

#define SH4_CHECK_RANGE_stc_bank(imm) ((int)(imm) >= 0 && (int)(imm) <= 7)
static inline void sh4_stc_bank(guint8 **code,int imm,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(SH4_CHECK_RANGE_stc_bank(imm));
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0x7) << 4) | (0x2 << 0)); \
}

static inline void sh4_stcl_SR_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_VBR_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_SSR_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_SPC_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_GBR_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_SGR_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0x2 << 0)); \
}

static inline void sh4_stcl_DBR_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0xF << 4) | (0x2 << 0)); \
}

#define SH4_CHECK_RANGE_stcl_bank_decRx(imm) ((int)(imm) >= 0 && (int)(imm) <= 7)
static inline void sh4_stcl_bank_decRx(guint8 **code,int imm,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(SH4_CHECK_RANGE_stcl_bank_decRx(imm));
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (((imm) & 0x7) << 4) | (0x3 << 0)); \
}

static inline void sh4_sts_MACH(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xA << 0)); \
}

static inline void sh4_sts_MACL(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xA << 0)); \
}

static inline void sh4_sts_PR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xA << 0)); \
}

static inline void sh4_sts_FPUL(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x5 << 4) | (0xA << 0)); \
}

static inline void sh4_sts_FPSCR(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x6 << 4) | (0xA << 0)); \
}

static inline void sh4_stsl_MACH_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x2 << 0)); \
}

static inline void sh4_stsl_MACL_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x2 << 0)); \
}

static inline void sh4_stsl_PR_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x2 << 0)); \
}

static inline void sh4_stsl_FPUL_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x5 << 4) | (0x2 << 0)); \
}

static inline void sh4_stsl_FPSCR_decRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x6 << 4) | (0x2 << 0)); \
}

static inline void sh4_sub(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_subc(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_subv(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_swapb(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_swapw(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_synco(guint8 **code) 
{
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0xA << 4) | (0xB << 0)); \
}

static inline void sh4_tasb_indRx(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xB << 0)); \
}

#define SH4_CHECK_RANGE_trapa_imm(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_trapa_imm(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_trapa_imm(imm));
	sh4_emit16(code, (0xC << 12) | (0x3 << 8) | (((imm) & 0xFF) << 0)); \
}

#define SH4_CHECK_RANGE_tst_imm_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_tst_imm_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_tst_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x8 << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_tst(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
}

#define SH4_CHECK_RANGE_tstb_imm_dispR0GBR(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_tstb_imm_dispR0GBR(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_tstb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xC << 8) | (((imm) & 0xFF) << 0)); \
}

#define SH4_CHECK_RANGE_xor_imm_R0(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_xor_imm_R0(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_xor_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0xA << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_xor(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
}

#define SH4_CHECK_RANGE_xorb_imm_dispR0GBR(imm) ((int)(imm) >= 0 && (int)(imm) <= 255)
static inline void sh4_xorb_imm_dispR0GBR(guint8 **code,int imm) 
{
	g_assert(SH4_CHECK_RANGE_xorb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xE << 8) | (((imm) & 0xFF) << 0)); \
}

static inline void sh4_xtrct(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_dt(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0x0 << 0)); \
}

static inline void sh4_dmulsl(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_dmulul(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_macl_incRy_incRx(guint8 **code,SH4IntRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xF << 0)); \
}

static inline void sh4_braf(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0x3 << 0)); \
}

static inline void sh4_bsrf(guint8 **code,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0x3 << 0)); \
}

static inline void sh4_fabs(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x5 << 4) | (0xD << 0)); \
}

static inline void sh4_fabs_double(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((((Rx) & 0xF) << 1)  << 8) | (0x5 << 4) | (0xD << 0)); \
}

static inline void sh4_fadd(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
}

static inline void sh4_fadd_double(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x0 << 0)); \
}

static inline void sh4_fcmpeq(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_fcmpeq_double(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_fcmpgt(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_fcmpgt_double(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_fcnvds_double_FPUL(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((((Rx) & 0xF) << 1)  << 8) | (0xB << 4) | (0xD << 0)); \
}

static inline void sh4_fcnvsd_FPUL_double(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((((Rx) & 0xF) << 1)  << 8) | (0xA << 4) | (0xD << 0)); \
}

static inline void sh4_fdiv(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x3 << 0)); \
}

static inline void sh4_fdiv_double(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x3 << 0)); \
}

static inline void sh4_fipr(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert(!((Rx & 0x3) || (Ry & 0x3)));
	sh4_emit16(code, (0xF << 12) | (((((Rx) & 0xF) << 2) | (((Ry) & 0xF) >> 2))  << 8) | (0xE << 4) | (0xD << 0)); \
}

static inline void sh4_fldi0(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x8 << 4) | (0xD << 0)); \
}

static inline void sh4_fldi1(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x9 << 4) | (0xD << 0)); \
}

static inline void sh4_flds_FPUL(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x1 << 4) | (0xD << 0)); \
}

static inline void sh4_float_FPUL(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xD << 0)); \
}

static inline void sh4_float_FPUL_double(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x2 << 4) | (0xD << 0)); \
}

static inline void sh4_fmac(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_fmov(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_fmov_Xdouble_Xdouble(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_fmov_indRy(guint8 **code,SH4IntRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_fmov_indRy_Xdouble(guint8 **code,SH4IntRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_fmov_indRx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_fmov_Xdouble_indRx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_fmov_incRy(guint8 **code,SH4IntRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_fmov_incRy_Xdouble(guint8 **code,SH4IntRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_fmov_decRx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_fmov_Xdouble_decRx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_fmov_dispR0Ry(guint8 **code,SH4IntRegister  Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_fmov_dispR0Ry_Xdouble(guint8 **code,SH4IntRegister  Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_fmov_dispR0Rx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister  Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_fmov_Xdouble_dispR0Rx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister  Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_fmovd_indRy_Xdouble(guint8 **code,SH4IntRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_fmovd_Xdouble_indRx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_fmovd_incRy_Xdouble(guint8 **code,SH4IntRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_fmovd_Xdouble_decRx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_fmovd_dispR0Ry_Xdouble(guint8 **code,SH4IntRegister  Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_fmovd_Xdouble_dispR0Rx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister  Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_fmovs_indRy(guint8 **code,SH4IntRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_fmovs_indRx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_fmovs_incRy(guint8 **code,SH4IntRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_fmovs_decRx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_fmovs_dispR0Ry(guint8 **code,SH4IntRegister  Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_fmovs_dispR0Rx(guint8 **code,SH4FloatRegister Ry,SH4IntRegister  Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_fmul(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
}

static inline void sh4_fmul_double(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x2 << 0)); \
}

static inline void sh4_fneg(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0xD << 0)); \
}

static inline void sh4_fneg_double(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x4 << 4) | (0xD << 0)); \
}

static inline void sh4_fpchg(guint8 **code) 
{
	sh4_emit16(code, (0xF << 12) | (0x7 << 8) | (0xF << 4) | (0xD << 0)); \
}

static inline void sh4_frchg(guint8 **code) 
{
	sh4_emit16(code, (0xF << 12) | (0xB << 8) | (0xF << 4) | (0xD << 0)); \
}

static inline void sh4_fsca_FPUL_double(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((((Rx) & 0xF) << 1)  << 8) | (0xF << 4) | (0xD << 0)); \
}

static inline void sh4_fschg(guint8 **code) 
{
	sh4_emit16(code, (0xF << 12) | (0x3 << 8) | (0xF << 4) | (0xD << 0)); \
}

static inline void sh4_fsqrt(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x6 << 4) | (0xD << 0)); \
}

static inline void sh4_fsqrt_double(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x6 << 4) | (0xD << 0)); \
}

static inline void sh4_fsrra(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x7 << 4) | (0xD << 0)); \
}

static inline void sh4_fsts_FPUL(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x0 << 4) | (0xD << 0)); \
}

static inline void sh4_fsub(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x1 << 0)); \
}

static inline void sh4_fsub_double(guint8 **code,SH4FloatRegister Ry,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert((int)Ry >= 0);
	g_assert((int)Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (((Ry) & 0xF) << 4) | (0x1 << 0)); \
}

static inline void sh4_ftrc_FPUL(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xD << 0)); \
}

static inline void sh4_ftrc_double_FPUL(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert((int)Rx >= 0);
	g_assert((int)Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx) & 0xF)  << 8) | (0x3 << 4) | (0xD << 0)); \
}

static inline void sh4_ftrv(guint8 **code,SH4FloatRegister Rx) 
{
	g_assert(!(Rx & 0x3));
	sh4_emit16(code, (0xF << 12) | (((((Rx) & 0xF) << 2) | 0x1)  << 8) | (0xF << 4) | (0xD << 0)); \
}


#endif /* __MONO_SH4_CODEGEN_H__ */

