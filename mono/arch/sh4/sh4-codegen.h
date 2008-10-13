/**
 * Copyright (C) 2008 STMicroelectronics.
 * This file has been generated automatically, do not modify.
 */

#ifndef __MONO_SH4_CODEGEN_H__
#define __MONO_SH4_CODEGEN_H__

#include "sh4-codegen-header.h"

extern void sh4_cstpool_check(void *cfg, guint8 **code);

#define SH4_CHECK_RANGE_add_imm(imm) ((imm) >= -128 && (imm) <= 127)
static inline void sh4_add_imm(void *cfg, guint8 **code, int imm, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(SH4_CHECK_RANGE_add_imm(imm));
	sh4_emit16(code, (0x7 << 12) | ((Rx & 0xF) << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_add(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_addc(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_addv(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)); \
}

#define SH4_CHECK_RANGE_and_imm_R0(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_and_imm_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_and_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x9 << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_and(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)); \
}

#define SH4_CHECK_RANGE_andb_imm_dispR0GBR(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_andb_imm_dispR0GBR(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_andb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xD << 8) | ((imm & 0xFF) << 0)); \
}

#define SH4_CHECK_RANGE_bra(imm) ((imm) >= -4096 && (imm) <= 4094)
#define SH4_CHECK_ALIGN_bra(imm) (((imm) & 0x1) == 0)
static inline void sh4_bra(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_bra(imm));
	g_assert(SH4_CHECK_ALIGN_bra(imm));
	sh4_emit16(code, (0xA << 12) | (((imm & 0x1FFE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bsr(imm) ((imm) >= -4096 && (imm) <= 4094)
#define SH4_CHECK_ALIGN_bsr(imm) (((imm) & 0x1) == 0)
static inline void sh4_bsr(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_bsr(imm));
	g_assert(SH4_CHECK_ALIGN_bsr(imm));
	sh4_emit16(code, (0xB << 12) | (((imm & 0x1FFE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bt(imm) ((imm) >= -256 && (imm) <= 254)
#define SH4_CHECK_ALIGN_bt(imm) (((imm) & 0x1) == 0)
static inline void sh4_bt(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_bt(imm));
	g_assert(SH4_CHECK_ALIGN_bt(imm));
	sh4_emit16(code, (0x8 << 12) | (0x9 << 8) | (((imm & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bf(imm) ((imm) >= -256 && (imm) <= 254)
#define SH4_CHECK_ALIGN_bf(imm) (((imm) & 0x1) == 0)
static inline void sh4_bf(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_bf(imm));
	g_assert(SH4_CHECK_ALIGN_bf(imm));
	sh4_emit16(code, (0x8 << 12) | (0xB << 8) | (((imm & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bts(imm) ((imm) >= -256 && (imm) <= 254)
#define SH4_CHECK_ALIGN_bts(imm) (((imm) & 0x1) == 0)
static inline void sh4_bts(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_bts(imm));
	g_assert(SH4_CHECK_ALIGN_bts(imm));
	sh4_emit16(code, (0x8 << 12) | (0xD << 8) | (((imm & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_bfs(imm) ((imm) >= -256 && (imm) <= 254)
#define SH4_CHECK_ALIGN_bfs(imm) (((imm) & 0x1) == 0)
static inline void sh4_bfs(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_bfs(imm));
	g_assert(SH4_CHECK_ALIGN_bfs(imm));
	sh4_emit16(code, (0x8 << 12) | (0xF << 8) | (((imm & 0x1FE) >> 1) << 0)); \
}

static inline void sh4_clrmac(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0x8 << 0)); \
}

static inline void sh4_clrs(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x4 << 4) | (0x8 << 0)); \
}

static inline void sh4_clrt(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x8 << 0)); \
}

#define SH4_CHECK_RANGE_cmpeq_imm_R0(imm) ((imm) >= -128 && (imm) <= 127)
static inline void sh4_cmpeq_imm_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_cmpeq_imm_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x8 << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_cmpeq(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)); \
}

static inline void sh4_cmpge(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0)); \
}

static inline void sh4_cmpgt(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_cmphi(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_cmphs(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)); \
}

static inline void sh4_cmppl(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x5 << 0)); \
}

static inline void sh4_cmppz(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x1 << 0)); \
}

static inline void sh4_cmpstr(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_div0s(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_div0u(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x9 << 0)); \
}

static inline void sh4_div1(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_extsb(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_extsw(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)); \
}

static inline void sh4_extub(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_extuw(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_icbi_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xE << 4) | (0x3 << 0)); \
}

static inline void sh4_jmp_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xB << 0)); \
}

static inline void sh4_jsr_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xB << 0)); \
}

static inline void sh4_ldc_SR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_GBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_SGR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xA << 0)); \
}

static inline void sh4_ldc_VBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_SSR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_SPC(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xE << 0)); \
}

static inline void sh4_ldc_DBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0xA << 0)); \
}

#define SH4_CHECK_RANGE_ldc_bank(imm) ((imm) >= 0 && (imm) <= 7)
static inline void sh4_ldc_bank(void *cfg, guint8 **code, SH4IntRegister Rx, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(SH4_CHECK_RANGE_ldc_bank(imm));
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0xE << 0)); \
}

static inline void sh4_ldcl_incRx_SR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_GBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_VBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_SGR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldcl_incRx_SSR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_SPC(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x7 << 0)); \
}

static inline void sh4_ldcl_incRx_DBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0x6 << 0)); \
}

#define SH4_CHECK_RANGE_ldcl_incRx_bank(imm) ((imm) >= 0 && (imm) <= 7)
static inline void sh4_ldcl_incRx_bank(void *cfg, guint8 **code, SH4IntRegister Rx, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(SH4_CHECK_RANGE_ldcl_incRx_bank(imm));
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x7 << 0)); \
}

static inline void sh4_lds_MACH(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xA << 0)); \
}

static inline void sh4_lds_MACL(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xA << 0)); \
}

static inline void sh4_lds_PR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xA << 0)); \
}

static inline void sh4_lds_FPUL(void *cfg, guint8 **code, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0x5 << 4) | (0xA << 0)); \
}

static inline void sh4_lds_FPSCR(void *cfg, guint8 **code, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0xA << 0)); \
}

static inline void sh4_ldsl_incRx_MACH(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldsl_incRx_MACL(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldsl_incRx_PR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldsl_incRy_FPUL(void *cfg, guint8 **code, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0x5 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldsl_incRy_FPSCR(void *cfg, guint8 **code, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0x6 << 0)); \
}

static inline void sh4_ldtlb(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x3 << 4) | (0x8 << 0)); \
}

static inline void sh4_macw_incRy_incRx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)); \
}

#define SH4_CHECK_RANGE_mov_imm(imm) ((imm) >= -128 && (imm) <= 127)
static inline void sh4_mov_imm(void *cfg, guint8 **code, int imm, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(SH4_CHECK_RANGE_mov_imm(imm));
	sh4_emit16(code, (0xE << 12) | ((Rx & 0xF) << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_mov(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0)); \
}

static inline void sh4_movb_dispR0Rx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_movb_decRx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_movb_indRx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)); \
}

#define SH4_CHECK_RANGE_movb_dispRy_R0(imm) ((imm) >= 0 && (imm) <= 15)
static inline void sh4_movb_dispRy_R0(void *cfg, guint8 **code, int imm, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movb_dispRy_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x4 << 8) | ((Ry & 0xF) << 4) | ((imm & 0xF) << 0)); \
}

#define SH4_CHECK_RANGE_movb_dispGBR_R0(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_movb_dispGBR_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_movb_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x4 << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_movb_dispR0Ry(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_movb_incRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_movb_indRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)); \
}

#define SH4_CHECK_RANGE_movb_R0_dispRy(imm) ((imm) >= 0 && (imm) <= 15)
static inline void sh4_movb_R0_dispRy(void *cfg, guint8 **code, int imm, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movb_R0_dispRy(imm));
	sh4_emit16(code, (0x8 << 12) | (0x0 << 8) | ((Ry & 0xF) << 4) | ((imm & 0xF) << 0)); \
}

#define SH4_CHECK_RANGE_movb_R0_dispGBR(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_movb_R0_dispGBR(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_movb_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x0 << 8) | ((imm & 0xFF) << 0)); \
}

#define SH4_CHECK_RANGE_movl_dispRx(imm) ((imm) >= 0 && (imm) <= 60)
#define SH4_CHECK_ALIGN_movl_dispRx(imm) (((imm) & 0x3) == 0)
static inline void sh4_movl_dispRx(void *cfg, guint8 **code, SH4IntRegister Ry, int imm, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movl_dispRx(imm));
	g_assert(SH4_CHECK_ALIGN_movl_dispRx(imm));
	sh4_emit16(code, (0x1 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (((imm & 0x3C) >> 2) << 0)); \
}

static inline void sh4_movl_dispR0Rx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_movl_decRx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_movl_indRx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)); \
}

#define SH4_CHECK_RANGE_movl_dispRy(imm) ((imm) >= 0 && (imm) <= 60)
#define SH4_CHECK_ALIGN_movl_dispRy(imm) (((imm) & 0x3) == 0)
static inline void sh4_movl_dispRy(void *cfg, guint8 **code, int imm, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movl_dispRy(imm));
	g_assert(SH4_CHECK_ALIGN_movl_dispRy(imm));
	sh4_emit16(code, (0x5 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (((imm & 0x3C) >> 2) << 0)); \
}

#define SH4_CHECK_RANGE_movl_dispGBR_R0(imm) ((imm) >= 0 && (imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_dispGBR_R0(imm) (((imm) & 0x3) == 0)
static inline void sh4_movl_dispGBR_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_movl_dispGBR_R0(imm));
	g_assert(SH4_CHECK_ALIGN_movl_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x6 << 8) | (((imm & 0x3FC) >> 2) << 0)); \
}

#define SH4_CHECK_RANGE_movl_dispPC(imm) ((imm) >= 0 && (imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_dispPC(imm) (((imm) & 0x3) == 0)
static inline void sh4_movl_dispPC(void *cfg, guint8 **code, int imm, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(SH4_CHECK_RANGE_movl_dispPC(imm));
	g_assert(SH4_CHECK_ALIGN_movl_dispPC(imm));
	sh4_emit16(code, (0xD << 12) | ((Rx & 0xF) << 8) | (((imm & 0x3FC) >> 2) << 0)); \
}

static inline void sh4_movl_dispR0Ry(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_movl_incRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_movl_indRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)); \
}

#define SH4_CHECK_RANGE_movl_R0_dispGBR(imm) ((imm) >= 0 && (imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_R0_dispGBR(imm) (((imm) & 0x3) == 0)
static inline void sh4_movl_R0_dispGBR(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_movl_R0_dispGBR(imm));
	g_assert(SH4_CHECK_ALIGN_movl_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x2 << 8) | (((imm & 0x3FC) >> 2) << 0)); \
}

static inline void sh4_movw_dispR0Rx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_movw_decRx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_movw_indRx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0)); \
}

#define SH4_CHECK_RANGE_movw_dispRy_R0(imm) ((imm) >= 0 && (imm) <= 30)
#define SH4_CHECK_ALIGN_movw_dispRy_R0(imm) (((imm) & 0x1) == 0)
static inline void sh4_movw_dispRy_R0(void *cfg, guint8 **code, int imm, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movw_dispRy_R0(imm));
	g_assert(SH4_CHECK_ALIGN_movw_dispRy_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x5 << 8) | ((Ry & 0xF) << 4) | (((imm & 0x1E) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_movw_dispGBR_R0(imm) ((imm) >= 0 && (imm) <= 510)
#define SH4_CHECK_ALIGN_movw_dispGBR_R0(imm) (((imm) & 0x1) == 0)
static inline void sh4_movw_dispGBR_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_movw_dispGBR_R0(imm));
	g_assert(SH4_CHECK_ALIGN_movw_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x5 << 8) | (((imm & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_movw_dispPC(imm) ((imm) >= 0 && (imm) <= 510)
#define SH4_CHECK_ALIGN_movw_dispPC(imm) (((imm) & 0x1) == 0)
static inline void sh4_movw_dispPC(void *cfg, guint8 **code, int imm, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(SH4_CHECK_RANGE_movw_dispPC(imm));
	g_assert(SH4_CHECK_ALIGN_movw_dispPC(imm));
	sh4_emit16(code, (0x9 << 12) | ((Rx & 0xF) << 8) | (((imm & 0x1FE) >> 1) << 0)); \
}

static inline void sh4_movw_dispR0Ry(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_movw_incRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_movw_indRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0)); \
}

#define SH4_CHECK_RANGE_movw_R0_dispRy(imm) ((imm) >= 0 && (imm) <= 30)
#define SH4_CHECK_ALIGN_movw_R0_dispRy(imm) (((imm) & 0x1) == 0)
static inline void sh4_movw_R0_dispRy(void *cfg, guint8 **code, int imm, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(SH4_CHECK_RANGE_movw_R0_dispRy(imm));
	g_assert(SH4_CHECK_ALIGN_movw_R0_dispRy(imm));
	sh4_emit16(code, (0x8 << 12) | (0x1 << 8) | ((Ry & 0xF) << 4) | (((imm & 0x1E) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_movw_R0_dispGBR(imm) ((imm) >= 0 && (imm) <= 510)
#define SH4_CHECK_ALIGN_movw_R0_dispGBR(imm) (((imm) & 0x1) == 0)
static inline void sh4_movw_R0_dispGBR(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_movw_R0_dispGBR(imm));
	g_assert(SH4_CHECK_ALIGN_movw_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x1 << 8) | (((imm & 0x1FE) >> 1) << 0)); \
}

#define SH4_CHECK_RANGE_mova_dispPC_R0(imm) ((imm) >= 0 && (imm) <= 1020)
#define SH4_CHECK_ALIGN_mova_dispPC_R0(imm) (((imm) & 0x3) == 0)
static inline void sh4_mova_dispPC_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_mova_dispPC_R0(imm));
	g_assert(SH4_CHECK_ALIGN_mova_dispPC_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x7 << 8) | (((imm & 0x3FC) >> 2) << 0)); \
}

static inline void sh4_movcal_R0_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xC << 4) | (0x3 << 0)); \
}

static inline void sh4_movcol_R0_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x7 << 4) | (0x3 << 0)); \
}

static inline void sh4_movlil_indRy_R0(void *cfg, guint8 **code, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0x3 << 0)); \
}

static inline void sh4_movt(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x9 << 0)); \
}

static inline void sh4_movual_indRy_R0(void *cfg, guint8 **code, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0xA << 4) | (0x9 << 0)); \
}

static inline void sh4_movual_incRy_R0(void *cfg, guint8 **code, SH4IntRegister Ry)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0xE << 4) | (0x9 << 0)); \
}

static inline void sh4_mulsw(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)); \
}

static inline void sh4_muls(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)); \
}

static inline void sh4_mull(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_muluw(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_mulu(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_neg(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_negc(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_nop(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x9 << 0)); \
}

static inline void sh4_not(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_ocbi_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x9 << 4) | (0x3 << 0)); \
}

static inline void sh4_ocbp_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xA << 4) | (0x3 << 0)); \
}

static inline void sh4_ocbwb_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xB << 4) | (0x3 << 0)); \
}

#define SH4_CHECK_RANGE_or_imm_R0(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_or_imm_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_or_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0xB << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_or(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)); \
}

#define SH4_CHECK_RANGE_orb_imm_dispR0GBR(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_orb_imm_dispR0GBR(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_orb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xF << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_pref_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x8 << 4) | (0x3 << 0)); \
}

static inline void sh4_prefi_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xD << 4) | (0x3 << 0)); \
}

static inline void sh4_rotcl(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x4 << 0)); \
}

static inline void sh4_rotcr(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x5 << 0)); \
}

static inline void sh4_rotl(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x4 << 0)); \
}

static inline void sh4_rotr(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x5 << 0)); \
}

static inline void sh4_rte(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0xB << 0)); \
}

static inline void sh4_rts(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0xB << 0)); \
}

static inline void sh4_sets(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x5 << 4) | (0x8 << 0)); \
}

static inline void sh4_sett(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x8 << 0)); \
}

static inline void sh4_shad(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_shld(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_shal(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x0 << 0)); \
}

static inline void sh4_shar(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x1 << 0)); \
}

static inline void sh4_shll(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x0 << 0)); \
}

static inline void sh4_shll16(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x8 << 0)); \
}

static inline void sh4_shll2(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x8 << 0)); \
}

static inline void sh4_shll8(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x8 << 0)); \
}

static inline void sh4_shlr(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x1 << 0)); \
}

static inline void sh4_shlr16(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x9 << 0)); \
}

static inline void sh4_shlr2(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x9 << 0)); \
}

static inline void sh4_shlr8(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x9 << 0)); \
}

static inline void sh4_sleep(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0xB << 0)); \
}

static inline void sh4_stc_SR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_GBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_VBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_SSR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_SPC(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x2 << 0)); \
}

static inline void sh4_stc_SGR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xA << 0)); \
}

static inline void sh4_stc_DBR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0xA << 0)); \
}

#define SH4_CHECK_RANGE_stc_bank(imm) ((imm) >= 0 && (imm) <= 7)
static inline void sh4_stc_bank(void *cfg, guint8 **code, int imm, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(SH4_CHECK_RANGE_stc_bank(imm));
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x2 << 0)); \
}

static inline void sh4_stcl_SR_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_VBR_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_SSR_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_SPC_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_GBR_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x3 << 0)); \
}

static inline void sh4_stcl_SGR_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x2 << 0)); \
}

static inline void sh4_stcl_DBR_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0x2 << 0)); \
}

#define SH4_CHECK_RANGE_stcl_bank_decRx(imm) ((imm) >= 0 && (imm) <= 7)
static inline void sh4_stcl_bank_decRx(void *cfg, guint8 **code, int imm, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(SH4_CHECK_RANGE_stcl_bank_decRx(imm));
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x3 << 0)); \
}

static inline void sh4_sts_MACH(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xA << 0)); \
}

static inline void sh4_sts_MACL(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xA << 0)); \
}

static inline void sh4_sts_PR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xA << 0)); \
}

static inline void sh4_sts_FPUL(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xA << 0)); \
}

static inline void sh4_sts_FPSCR(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xA << 0)); \
}

static inline void sh4_stsl_MACH_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x2 << 0)); \
}

static inline void sh4_stsl_MACL_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x2 << 0)); \
}

static inline void sh4_stsl_PR_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x2 << 0)); \
}

static inline void sh4_stsl_FPUL_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0x2 << 0)); \
}

static inline void sh4_stsl_FPSCR_decRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0x2 << 0)); \
}

static inline void sh4_sub(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_subc(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_subv(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_swapb(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_swapw(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_synco(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0xA << 4) | (0xB << 0)); \
}

static inline void sh4_tasb_indRx(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xB << 0)); \
}

#define SH4_CHECK_RANGE_trapa_imm(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_trapa_imm(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_trapa_imm(imm));
	sh4_emit16(code, (0xC << 12) | (0x3 << 8) | ((imm & 0xFF) << 0)); \
}

#define SH4_CHECK_RANGE_tst_imm_R0(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_tst_imm_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_tst_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x8 << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_tst(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)); \
}

#define SH4_CHECK_RANGE_tstb_imm_dispR0GBR(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_tstb_imm_dispR0GBR(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_tstb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xC << 8) | ((imm & 0xFF) << 0)); \
}

#define SH4_CHECK_RANGE_xor_imm_R0(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_xor_imm_R0(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_xor_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0xA << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_xor(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)); \
}

#define SH4_CHECK_RANGE_xorb_imm_dispR0GBR(imm) ((imm) >= 0 && (imm) <= 255)
static inline void sh4_xorb_imm_dispR0GBR(void *cfg, guint8 **code, int imm)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(SH4_CHECK_RANGE_xorb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xE << 8) | ((imm & 0xFF) << 0)); \
}

static inline void sh4_xtrct(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_dt(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x0 << 0)); \
}

static inline void sh4_dmulsl(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)); \
}

static inline void sh4_dmulul(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_macl_incRy_incRx(void *cfg, guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)); \
}

static inline void sh4_braf(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x3 << 0)); \
}

static inline void sh4_bsrf(void *cfg, guint8 **code, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x3 << 0)); \
}

static inline void sh4_fabs(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xD << 0)); \
}

static inline void sh4_fabs_double(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xD << 0)); \
}

static inline void sh4_fadd(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)); \
}

static inline void sh4_fadd_double(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)); \
}

static inline void sh4_fcmpeq(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_fcmpeq_double(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)); \
}

static inline void sh4_fcmpgt(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_fcmpgt_double(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)); \
}

static inline void sh4_fcnvds_double_FPUL(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx & 0xF) << 1) << 8) | (0xB << 4) | (0xD << 0)); \
}

static inline void sh4_fcnvsd_FPUL_double(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx & 0xF) << 1) << 8) | (0xA << 4) | (0xD << 0)); \
}

static inline void sh4_fdiv(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0)); \
}

static inline void sh4_fdiv_double(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0)); \
}

static inline void sh4_fipr(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(!((Rx & 0x3) || (Ry & 0x3)));
	sh4_emit16(code, (0xF << 12) | ((((Rx & 0xF) << 2) | ((Ry & 0xF) >> 2)) << 8) | (0xE << 4) | (0xD << 0)); \
}

static inline void sh4_fldi0(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x8 << 4) | (0xD << 0)); \
}

static inline void sh4_fldi1(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x9 << 4) | (0xD << 0)); \
}

static inline void sh4_flds_FPUL(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xD << 0)); \
}

static inline void sh4_float_FPUL(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xD << 0)); \
}

static inline void sh4_float_FPUL_double(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xD << 0)); \
}

static inline void sh4_fmac(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)); \
}

static inline void sh4_fmov(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_fmov_Xdouble_Xdouble(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)); \
}

static inline void sh4_fmov_indRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_fmov_indRy_Xdouble(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_fmov_indRx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_fmov_Xdouble_indRx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_fmov_incRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_fmov_incRy_Xdouble(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_fmov_decRx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_fmov_Xdouble_decRx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_fmov_dispR0Ry(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_fmov_dispR0Ry_Xdouble(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_fmov_dispR0Rx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_fmov_Xdouble_dispR0Rx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_fmovd_indRy_Xdouble(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_fmovd_Xdouble_indRx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_fmovd_incRy_Xdouble(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_fmovd_Xdouble_decRx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_fmovd_dispR0Ry_Xdouble(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_fmovd_Xdouble_dispR0Rx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_fmovs_indRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)); \
}

static inline void sh4_fmovs_indRx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)); \
}

static inline void sh4_fmovs_incRy(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)); \
}

static inline void sh4_fmovs_decRx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)); \
}

static inline void sh4_fmovs_dispR0Ry(void *cfg, guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)); \
}

static inline void sh4_fmovs_dispR0Rx(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)); \
}

static inline void sh4_fmul(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)); \
}

static inline void sh4_fmul_double(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)); \
}

static inline void sh4_fneg(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xD << 0)); \
}

static inline void sh4_fneg_double(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xD << 0)); \
}

static inline void sh4_fpchg(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0xF << 12) | (0x7 << 8) | (0xF << 4) | (0xD << 0)); \
}

static inline void sh4_frchg(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0xF << 12) | (0xB << 8) | (0xF << 4) | (0xD << 0)); \
}

static inline void sh4_fsca_FPUL_double(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | (((Rx & 0xF) << 1) << 8) | (0xF << 4) | (0xD << 0)); \
}

static inline void sh4_fschg(void *cfg, guint8 **code)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	sh4_emit16(code, (0xF << 12) | (0x3 << 8) | (0xF << 4) | (0xD << 0)); \
}

static inline void sh4_fsqrt(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xD << 0)); \
}

static inline void sh4_fsqrt_double(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xD << 0)); \
}

static inline void sh4_fsrra(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x7 << 4) | (0xD << 0)); \
}

static inline void sh4_fsts_FPUL(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xD << 0)); \
}

static inline void sh4_fsub(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0)); \
}

static inline void sh4_fsub_double(void *cfg, guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	g_assert(Ry >= 0);
	g_assert(Ry <= 15);
	g_assert(!(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0)); \
}

static inline void sh4_ftrc_FPUL(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xD << 0)); \
}

static inline void sh4_ftrc_double_FPUL(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(Rx >= 0);
	g_assert(Rx <= 15);
	g_assert(!(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xD << 0)); \
}

static inline void sh4_ftrv(void *cfg, guint8 **code, SH4FloatRegister Rx)
{
	if (cfg != NULL)
		sh4_cstpool_check(cfg, code);
	g_assert(!(Rx & 0x3));
	sh4_emit16(code, (0xF << 12) | ((((Rx & 0xF) << 2) | 0x1) << 8) | (0xF << 4) | (0xD << 0)); \
}


#endif /* __MONO_SH4_CODEGEN_H__ */

