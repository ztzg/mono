/**
 * Copyright (C) 2008 STMicroelectronics.
 * This file has been generated automatically, do not modify.
 */

#ifndef __MONO_SH4_CODEGEN_H__
#define __MONO_SH4_CODEGEN_H__

#include "sh4-codegen-header.h"

#define SH4_CHECK_RANGE_add_imm(imm) ((imm) >= -128 && (imm) <= 127)

static inline void sh4_add_imm(guint8 **code, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_add_imm(imm));
	sh4_emit16(code, (0x7 << 12) | ((Rx & 0xF) << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_add_imm(guint16 code, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_add_imm(imm)
		&& code == ((0x7 << 12) | ((Rx & 0xF) << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_Rx_sh4_add_imm(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_add_imm(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	if ((result & 0x80) != 0)
		result |= 0xFFFFFF00;
	g_assert(SH4_CHECK_RANGE_add_imm(result));
	return result;
}

static inline void sh4_add(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0));
}

static inline int is_sh4_add(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)));
}

static inline int get_Rx_sh4_add(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_add(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_addc(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0));
}

static inline int is_sh4_addc(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_addc(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_addc(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_addv(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0));
}

static inline int is_sh4_addv(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)));
}

static inline int get_Rx_sh4_addv(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_addv(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_and_imm_R0(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_and_imm_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_and_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x9 << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_and_imm_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_and_imm_R0(imm)
		&& code == ((0xC << 12) | (0x9 << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_and_imm_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}

static inline void sh4_and(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0));
}

static inline int is_sh4_and(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_and(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_and(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_andb_imm_dispR0GBR(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_andb_imm_dispR0GBR(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_andb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xD << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_andb_imm_dispR0GBR(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_andb_imm_dispR0GBR(imm)
		&& code == ((0xC << 12) | (0xD << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_andb_imm_dispR0GBR(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_bra(imm) ((imm) >= -4096 && (imm) <= 4094)
#define SH4_CHECK_ALIGN_bra(imm) (((imm) & 0x1) == 0)

static inline void sh4_bra(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_bra(imm) && SH4_CHECK_ALIGN_bra(imm));
	sh4_emit16(code, (0xA << 12) | (((imm & 0x1FFE) >> 1) << 0));
}

static inline int is_sh4_bra(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_bra(imm) && SH4_CHECK_ALIGN_bra(imm)
		&& code == ((0xA << 12) | (((imm & 0x1FFE) >> 1) << 0)));
}

static inline int get_imm_sh4_bra(guint16 code)
{
	int result = ((code >> 0) & 0xFFF) << 1;
	g_assert(SH4_CHECK_ALIGN_bra(result));
	return result;
}
#define SH4_CHECK_RANGE_bsr(imm) ((imm) >= -4096 && (imm) <= 4094)
#define SH4_CHECK_ALIGN_bsr(imm) (((imm) & 0x1) == 0)

static inline void sh4_bsr(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_bsr(imm) && SH4_CHECK_ALIGN_bsr(imm));
	sh4_emit16(code, (0xB << 12) | (((imm & 0x1FFE) >> 1) << 0));
}

static inline int is_sh4_bsr(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_bsr(imm) && SH4_CHECK_ALIGN_bsr(imm)
		&& code == ((0xB << 12) | (((imm & 0x1FFE) >> 1) << 0)));
}

static inline int get_imm_sh4_bsr(guint16 code)
{
	int result = ((code >> 0) & 0xFFF) << 1;
	g_assert(SH4_CHECK_ALIGN_bsr(result));
	return result;
}
#define SH4_CHECK_RANGE_bt(imm) ((imm) >= -256 && (imm) <= 254)
#define SH4_CHECK_ALIGN_bt(imm) (((imm) & 0x1) == 0)

static inline void sh4_bt(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_bt(imm) && SH4_CHECK_ALIGN_bt(imm));
	sh4_emit16(code, (0x8 << 12) | (0x9 << 8) | (((imm & 0x1FE) >> 1) << 0));
}

static inline int is_sh4_bt(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_bt(imm) && SH4_CHECK_ALIGN_bt(imm)
		&& code == ((0x8 << 12) | (0x9 << 8) | (((imm & 0x1FE) >> 1) << 0)));
}

static inline int get_imm_sh4_bt(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 1;
	g_assert(SH4_CHECK_ALIGN_bt(result));
	return result;
}
#define SH4_CHECK_RANGE_bf(imm) ((imm) >= -256 && (imm) <= 254)
#define SH4_CHECK_ALIGN_bf(imm) (((imm) & 0x1) == 0)

static inline void sh4_bf(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_bf(imm) && SH4_CHECK_ALIGN_bf(imm));
	sh4_emit16(code, (0x8 << 12) | (0xB << 8) | (((imm & 0x1FE) >> 1) << 0));
}

static inline int is_sh4_bf(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_bf(imm) && SH4_CHECK_ALIGN_bf(imm)
		&& code == ((0x8 << 12) | (0xB << 8) | (((imm & 0x1FE) >> 1) << 0)));
}

static inline int get_imm_sh4_bf(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 1;
	g_assert(SH4_CHECK_ALIGN_bf(result));
	return result;
}
#define SH4_CHECK_RANGE_bts(imm) ((imm) >= -256 && (imm) <= 254)
#define SH4_CHECK_ALIGN_bts(imm) (((imm) & 0x1) == 0)

static inline void sh4_bts(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_bts(imm) && SH4_CHECK_ALIGN_bts(imm));
	sh4_emit16(code, (0x8 << 12) | (0xD << 8) | (((imm & 0x1FE) >> 1) << 0));
}

static inline int is_sh4_bts(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_bts(imm) && SH4_CHECK_ALIGN_bts(imm)
		&& code == ((0x8 << 12) | (0xD << 8) | (((imm & 0x1FE) >> 1) << 0)));
}

static inline int get_imm_sh4_bts(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 1;
	g_assert(SH4_CHECK_ALIGN_bts(result));
	return result;
}
#define SH4_CHECK_RANGE_bfs(imm) ((imm) >= -256 && (imm) <= 254)
#define SH4_CHECK_ALIGN_bfs(imm) (((imm) & 0x1) == 0)

static inline void sh4_bfs(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_bfs(imm) && SH4_CHECK_ALIGN_bfs(imm));
	sh4_emit16(code, (0x8 << 12) | (0xF << 8) | (((imm & 0x1FE) >> 1) << 0));
}

static inline int is_sh4_bfs(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_bfs(imm) && SH4_CHECK_ALIGN_bfs(imm)
		&& code == ((0x8 << 12) | (0xF << 8) | (((imm & 0x1FE) >> 1) << 0)));
}

static inline int get_imm_sh4_bfs(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 1;
	g_assert(SH4_CHECK_ALIGN_bfs(result));
	return result;
}

static inline void sh4_clrmac(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0x8 << 0));
}

static inline int is_sh4_clrmac(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0x8 << 0)));
}


static inline void sh4_clrs(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x4 << 4) | (0x8 << 0));
}

static inline int is_sh4_clrs(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x4 << 4) | (0x8 << 0)));
}


static inline void sh4_clrt(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x8 << 0));
}

static inline int is_sh4_clrt(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x8 << 0)));
}

#define SH4_CHECK_RANGE_cmpeq_imm_R0(imm) ((imm) >= -128 && (imm) <= 127)

static inline void sh4_cmpeq_imm_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_cmpeq_imm_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x8 << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_cmpeq_imm_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_cmpeq_imm_R0(imm)
		&& code == ((0x8 << 12) | (0x8 << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_cmpeq_imm_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	if ((result & 0x80) != 0)
		result |= 0xFFFFFF00;
	g_assert(SH4_CHECK_RANGE_cmpeq_imm_R0(result));
	return result;
}

static inline void sh4_cmpeq(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0));
}

static inline int is_sh4_cmpeq(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)));
}

static inline int get_Rx_sh4_cmpeq(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_cmpeq(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_cmpge(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0));
}

static inline int is_sh4_cmpge(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_cmpge(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_cmpge(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_cmpgt(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0));
}

static inline int is_sh4_cmpgt(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_cmpgt(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_cmpgt(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_cmphi(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0));
}

static inline int is_sh4_cmphi(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_cmphi(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_cmphi(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_cmphs(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0));
}

static inline int is_sh4_cmphs(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_cmphs(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_cmphs(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_cmppl(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x5 << 0));
}

static inline int is_sh4_cmppl(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_cmppl(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_cmppz(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x1 << 0));
}

static inline int is_sh4_cmppz(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x1 << 0)));
}

static inline int get_Rx_sh4_cmppz(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_cmpstr(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0));
}

static inline int is_sh4_cmpstr(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)));
}

static inline int get_Rx_sh4_cmpstr(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_cmpstr(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_div0s(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0));
}

static inline int is_sh4_div0s(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_div0s(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_div0s(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_div0u(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x9 << 0));
}

static inline int is_sh4_div0u(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x9 << 0)));
}


static inline void sh4_div1(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0));
}

static inline int is_sh4_div1(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)));
}

static inline int get_Rx_sh4_div1(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_div1(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_extsb(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0));
}

static inline int is_sh4_extsb(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_extsb(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_extsb(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_extsw(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0));
}

static inline int is_sh4_extsw(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)));
}

static inline int get_Rx_sh4_extsw(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_extsw(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_extub(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0));
}

static inline int is_sh4_extub(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)));
}

static inline int get_Rx_sh4_extub(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_extub(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_extuw(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0));
}

static inline int is_sh4_extuw(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_extuw(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_extuw(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_icbi_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xE << 4) | (0x3 << 0));
}

static inline int is_sh4_icbi_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0xE << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_icbi_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_jmp_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xB << 0));
}

static inline int is_sh4_jmp_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_jmp_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_jsr_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xB << 0));
}

static inline int is_sh4_jsr_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_jsr_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldc_SR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xE << 0));
}

static inline int is_sh4_ldc_SR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_ldc_SR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldc_GBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xE << 0));
}

static inline int is_sh4_ldc_GBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_ldc_GBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldc_SGR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xA << 0));
}

static inline int is_sh4_ldc_SGR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_ldc_SGR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldc_VBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xE << 0));
}

static inline int is_sh4_ldc_VBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_ldc_VBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldc_SSR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xE << 0));
}

static inline int is_sh4_ldc_SSR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_ldc_SSR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldc_SPC(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xE << 0));
}

static inline int is_sh4_ldc_SPC(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_ldc_SPC(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldc_DBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0xA << 0));
}

static inline int is_sh4_ldc_DBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_ldc_DBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_ldc_bank(imm) ((imm) >= 0 && (imm) <= 7)

static inline void sh4_ldc_bank(guint8 **code, SH4IntRegister Rx, int imm)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_ldc_bank(imm));
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0xE << 0));
}

static inline int is_sh4_ldc_bank(guint16 code, SH4IntRegister Rx, int imm)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_ldc_bank(imm)
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_ldc_bank(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldcl_incRx_SR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x7 << 0));
}

static inline int is_sh4_ldcl_incRx_SR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_ldcl_incRx_SR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldcl_incRx_GBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x7 << 0));
}

static inline int is_sh4_ldcl_incRx_GBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_ldcl_incRx_GBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldcl_incRx_VBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x7 << 0));
}

static inline int is_sh4_ldcl_incRx_VBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_ldcl_incRx_VBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldcl_incRx_SGR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x6 << 0));
}

static inline int is_sh4_ldcl_incRx_SGR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_ldcl_incRx_SGR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldcl_incRx_SSR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x7 << 0));
}

static inline int is_sh4_ldcl_incRx_SSR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_ldcl_incRx_SSR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldcl_incRx_SPC(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x7 << 0));
}

static inline int is_sh4_ldcl_incRx_SPC(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_ldcl_incRx_SPC(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldcl_incRx_DBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0x6 << 0));
}

static inline int is_sh4_ldcl_incRx_DBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_ldcl_incRx_DBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_ldcl_incRx_bank(imm) ((imm) >= 0 && (imm) <= 7)

static inline void sh4_ldcl_incRx_bank(guint8 **code, SH4IntRegister Rx, int imm)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_ldcl_incRx_bank(imm));
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x7 << 0));
}

static inline int is_sh4_ldcl_incRx_bank(guint16 code, SH4IntRegister Rx, int imm)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_ldcl_incRx_bank(imm)
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_ldcl_incRx_bank(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_lds_MACH(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xA << 0));
}

static inline int is_sh4_lds_MACH(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_lds_MACH(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_lds_MACL(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xA << 0));
}

static inline int is_sh4_lds_MACL(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_lds_MACL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_lds_PR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xA << 0));
}

static inline int is_sh4_lds_PR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_lds_PR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_lds_FPUL(guint8 **code, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0x5 << 4) | (0xA << 0));
}

static inline int is_sh4_lds_FPUL(guint16 code, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Ry & 0xF) << 8) | (0x5 << 4) | (0xA << 0)));
}

static inline int get_Ry_sh4_lds_FPUL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_lds_FPSCR(guint8 **code, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0xA << 0));
}

static inline int is_sh4_lds_FPSCR(guint16 code, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0xA << 0)));
}

static inline int get_Ry_sh4_lds_FPSCR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldsl_incRx_MACH(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x6 << 0));
}

static inline int is_sh4_ldsl_incRx_MACH(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_ldsl_incRx_MACH(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldsl_incRx_MACL(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x6 << 0));
}

static inline int is_sh4_ldsl_incRx_MACL(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_ldsl_incRx_MACL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldsl_incRx_PR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x6 << 0));
}

static inline int is_sh4_ldsl_incRx_PR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_ldsl_incRx_PR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldsl_incRy_FPUL(guint8 **code, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0x5 << 4) | (0x6 << 0));
}

static inline int is_sh4_ldsl_incRy_FPUL(guint16 code, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Ry & 0xF) << 8) | (0x5 << 4) | (0x6 << 0)));
}

static inline int get_Ry_sh4_ldsl_incRy_FPUL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldsl_incRy_FPSCR(guint8 **code, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0x6 << 0));
}

static inline int is_sh4_ldsl_incRy_FPSCR(guint16 code, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0x6 << 0)));
}

static inline int get_Ry_sh4_ldsl_incRy_FPSCR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ldtlb(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x3 << 4) | (0x8 << 0));
}

static inline int is_sh4_ldtlb(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x3 << 4) | (0x8 << 0)));
}


static inline void sh4_macw_incRy_incRx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0));
}

static inline int is_sh4_macw_incRy_incRx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)));
}

static inline int get_Rx_sh4_macw_incRy_incRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_macw_incRy_incRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_mov_imm(imm) ((imm) >= -128 && (imm) <= 127)

static inline void sh4_mov_imm(guint8 **code, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_mov_imm(imm));
	sh4_emit16(code, (0xE << 12) | ((Rx & 0xF) << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_mov_imm(guint16 code, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_mov_imm(imm)
		&& code == ((0xE << 12) | ((Rx & 0xF) << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_Rx_sh4_mov_imm(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_mov_imm(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	if ((result & 0x80) != 0)
		result |= 0xFFFFFF00;
	g_assert(SH4_CHECK_RANGE_mov_imm(result));
	return result;
}

static inline void sh4_mov(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0));
}

static inline int is_sh4_mov(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_mov(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_mov(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movb_dispR0Rx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0));
}

static inline int is_sh4_movb_dispR0Rx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)));
}

static inline int get_Rx_sh4_movb_dispR0Rx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movb_dispR0Rx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movb_decRx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0));
}

static inline int is_sh4_movb_decRx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)));
}

static inline int get_Rx_sh4_movb_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movb_decRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movb_indRx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0));
}

static inline int is_sh4_movb_indRx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)));
}

static inline int get_Rx_sh4_movb_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movb_indRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movb_dispRy_R0(imm) ((imm) >= 0 && (imm) <= 15)

static inline void sh4_movb_dispRy_R0(guint8 **code, int imm, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15 && SH4_CHECK_RANGE_movb_dispRy_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x4 << 8) | ((Ry & 0xF) << 4) | ((imm & 0xF) << 0));
}

static inline int is_sh4_movb_dispRy_R0(guint16 code, int imm, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15 && SH4_CHECK_RANGE_movb_dispRy_R0(imm)
		&& code == ((0x8 << 12) | (0x4 << 8) | ((Ry & 0xF) << 4) | ((imm & 0xF) << 0)));
}

static inline int get_Ry_sh4_movb_dispRy_R0(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_movb_dispRy_R0(guint16 code)
{
	int result = ((code >> 0) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movb_dispGBR_R0(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_movb_dispGBR_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_movb_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x4 << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_movb_dispGBR_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_movb_dispGBR_R0(imm)
		&& code == ((0xC << 12) | (0x4 << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_movb_dispGBR_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}

static inline void sh4_movb_dispR0Ry(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0));
}

static inline int is_sh4_movb_dispR0Ry(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)));
}

static inline int get_Rx_sh4_movb_dispR0Ry(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movb_dispR0Ry(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movb_incRy(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0));
}

static inline int is_sh4_movb_incRy(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)));
}

static inline int get_Rx_sh4_movb_incRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movb_incRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movb_indRy(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0));
}

static inline int is_sh4_movb_indRy(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)));
}

static inline int get_Rx_sh4_movb_indRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movb_indRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movb_R0_dispRx(imm) ((imm) >= 0 && (imm) <= 15)

static inline void sh4_movb_R0_dispRx(guint8 **code, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_movb_R0_dispRx(imm));
	sh4_emit16(code, (0x8 << 12) | (0x0 << 8) | ((Rx & 0xF) << 4) | ((imm & 0xF) << 0));
}

static inline int is_sh4_movb_R0_dispRx(guint16 code, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_movb_R0_dispRx(imm)
		&& code == ((0x8 << 12) | (0x0 << 8) | ((Rx & 0xF) << 4) | ((imm & 0xF) << 0)));
}

static inline int get_Rx_sh4_movb_R0_dispRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_movb_R0_dispRx(guint16 code)
{
	int result = ((code >> 0) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movb_R0_dispGBR(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_movb_R0_dispGBR(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_movb_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x0 << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_movb_R0_dispGBR(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_movb_R0_dispGBR(imm)
		&& code == ((0xC << 12) | (0x0 << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_movb_R0_dispGBR(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movl_dispRx(imm) ((imm) >= 0 && (imm) <= 60)
#define SH4_CHECK_ALIGN_movl_dispRx(imm) (((imm) & 0x3) == 0)

static inline void sh4_movl_dispRx(guint8 **code, SH4IntRegister Ry, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15 && SH4_CHECK_RANGE_movl_dispRx(imm) && SH4_CHECK_ALIGN_movl_dispRx(imm));
	sh4_emit16(code, (0x1 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (((imm & 0x3C) >> 2) << 0));
}

static inline int is_sh4_movl_dispRx(guint16 code, SH4IntRegister Ry, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15 && SH4_CHECK_RANGE_movl_dispRx(imm) && SH4_CHECK_ALIGN_movl_dispRx(imm)
		&& code == ((0x1 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (((imm & 0x3C) >> 2) << 0)));
}

static inline int get_Rx_sh4_movl_dispRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movl_dispRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_movl_dispRx(guint16 code)
{
	int result = ((code >> 0) & 0xF) << 2;
	g_assert(SH4_CHECK_ALIGN_movl_dispRx(result));
	return result;
}

static inline void sh4_movl_dispR0Rx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0));
}

static inline int is_sh4_movl_dispR0Rx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_movl_dispR0Rx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movl_dispR0Rx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movl_decRx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0));
}

static inline int is_sh4_movl_decRx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_movl_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movl_decRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movl_indRx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0));
}

static inline int is_sh4_movl_indRx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_movl_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movl_indRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movl_dispRy(imm) ((imm) >= 0 && (imm) <= 60)
#define SH4_CHECK_ALIGN_movl_dispRy(imm) (((imm) & 0x3) == 0)

static inline void sh4_movl_dispRy(guint8 **code, int imm, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15 && SH4_CHECK_RANGE_movl_dispRy(imm) && SH4_CHECK_ALIGN_movl_dispRy(imm));
	sh4_emit16(code, (0x5 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (((imm & 0x3C) >> 2) << 0));
}

static inline int is_sh4_movl_dispRy(guint16 code, int imm, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15 && SH4_CHECK_RANGE_movl_dispRy(imm) && SH4_CHECK_ALIGN_movl_dispRy(imm)
		&& code == ((0x5 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (((imm & 0x3C) >> 2) << 0)));
}

static inline int get_Rx_sh4_movl_dispRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movl_dispRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_movl_dispRy(guint16 code)
{
	int result = ((code >> 0) & 0xF) << 2;
	g_assert(SH4_CHECK_ALIGN_movl_dispRy(result));
	return result;
}
#define SH4_CHECK_RANGE_movl_dispGBR_R0(imm) ((imm) >= 0 && (imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_dispGBR_R0(imm) (((imm) & 0x3) == 0)

static inline void sh4_movl_dispGBR_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_movl_dispGBR_R0(imm) && SH4_CHECK_ALIGN_movl_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x6 << 8) | (((imm & 0x3FC) >> 2) << 0));
}

static inline int is_sh4_movl_dispGBR_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_movl_dispGBR_R0(imm) && SH4_CHECK_ALIGN_movl_dispGBR_R0(imm)
		&& code == ((0xC << 12) | (0x6 << 8) | (((imm & 0x3FC) >> 2) << 0)));
}

static inline int get_imm_sh4_movl_dispGBR_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 2;
	g_assert(SH4_CHECK_ALIGN_movl_dispGBR_R0(result));
	return result;
}
#define SH4_CHECK_RANGE_movl_dispPC(imm) ((imm) >= 0 && (imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_dispPC(imm) (((imm) & 0x3) == 0)

static inline void sh4_movl_dispPC(guint8 **code, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_movl_dispPC(imm) && SH4_CHECK_ALIGN_movl_dispPC(imm));
	sh4_emit16(code, (0xD << 12) | ((Rx & 0xF) << 8) | (((imm & 0x3FC) >> 2) << 0));
}

static inline int is_sh4_movl_dispPC(guint16 code, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_movl_dispPC(imm) && SH4_CHECK_ALIGN_movl_dispPC(imm)
		&& code == ((0xD << 12) | ((Rx & 0xF) << 8) | (((imm & 0x3FC) >> 2) << 0)));
}

static inline int get_Rx_sh4_movl_dispPC(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_movl_dispPC(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 2;
	g_assert(SH4_CHECK_ALIGN_movl_dispPC(result));
	return result;
}

static inline void sh4_movl_dispR0Ry(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0));
}

static inline int is_sh4_movl_dispR0Ry(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_movl_dispR0Ry(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movl_dispR0Ry(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movl_incRy(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0));
}

static inline int is_sh4_movl_incRy(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_movl_incRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movl_incRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movl_indRy(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0));
}

static inline int is_sh4_movl_indRy(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_movl_indRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movl_indRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movl_R0_dispGBR(imm) ((imm) >= 0 && (imm) <= 1020)
#define SH4_CHECK_ALIGN_movl_R0_dispGBR(imm) (((imm) & 0x3) == 0)

static inline void sh4_movl_R0_dispGBR(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_movl_R0_dispGBR(imm) && SH4_CHECK_ALIGN_movl_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x2 << 8) | (((imm & 0x3FC) >> 2) << 0));
}

static inline int is_sh4_movl_R0_dispGBR(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_movl_R0_dispGBR(imm) && SH4_CHECK_ALIGN_movl_R0_dispGBR(imm)
		&& code == ((0xC << 12) | (0x2 << 8) | (((imm & 0x3FC) >> 2) << 0)));
}

static inline int get_imm_sh4_movl_R0_dispGBR(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 2;
	g_assert(SH4_CHECK_ALIGN_movl_R0_dispGBR(result));
	return result;
}

static inline void sh4_movw_dispR0Rx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0));
}

static inline int is_sh4_movw_dispR0Rx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_movw_dispR0Rx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movw_dispR0Rx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movw_decRx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0));
}

static inline int is_sh4_movw_decRx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_movw_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movw_decRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movw_indRx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0));
}

static inline int is_sh4_movw_indRx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0)));
}

static inline int get_Rx_sh4_movw_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movw_indRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movw_dispRy_R0(imm) ((imm) >= 0 && (imm) <= 30)
#define SH4_CHECK_ALIGN_movw_dispRy_R0(imm) (((imm) & 0x1) == 0)

static inline void sh4_movw_dispRy_R0(guint8 **code, int imm, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15 && SH4_CHECK_RANGE_movw_dispRy_R0(imm) && SH4_CHECK_ALIGN_movw_dispRy_R0(imm));
	sh4_emit16(code, (0x8 << 12) | (0x5 << 8) | ((Ry & 0xF) << 4) | (((imm & 0x1E) >> 1) << 0));
}

static inline int is_sh4_movw_dispRy_R0(guint16 code, int imm, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15 && SH4_CHECK_RANGE_movw_dispRy_R0(imm) && SH4_CHECK_ALIGN_movw_dispRy_R0(imm)
		&& code == ((0x8 << 12) | (0x5 << 8) | ((Ry & 0xF) << 4) | (((imm & 0x1E) >> 1) << 0)));
}

static inline int get_Ry_sh4_movw_dispRy_R0(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_movw_dispRy_R0(guint16 code)
{
	int result = ((code >> 0) & 0xF) << 1;
	g_assert(SH4_CHECK_ALIGN_movw_dispRy_R0(result));
	return result;
}
#define SH4_CHECK_RANGE_movw_dispGBR_R0(imm) ((imm) >= 0 && (imm) <= 510)
#define SH4_CHECK_ALIGN_movw_dispGBR_R0(imm) (((imm) & 0x1) == 0)

static inline void sh4_movw_dispGBR_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_movw_dispGBR_R0(imm) && SH4_CHECK_ALIGN_movw_dispGBR_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x5 << 8) | (((imm & 0x1FE) >> 1) << 0));
}

static inline int is_sh4_movw_dispGBR_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_movw_dispGBR_R0(imm) && SH4_CHECK_ALIGN_movw_dispGBR_R0(imm)
		&& code == ((0xC << 12) | (0x5 << 8) | (((imm & 0x1FE) >> 1) << 0)));
}

static inline int get_imm_sh4_movw_dispGBR_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 1;
	g_assert(SH4_CHECK_ALIGN_movw_dispGBR_R0(result));
	return result;
}
#define SH4_CHECK_RANGE_movw_dispPC(imm) ((imm) >= 0 && (imm) <= 510)
#define SH4_CHECK_ALIGN_movw_dispPC(imm) (((imm) & 0x1) == 0)

static inline void sh4_movw_dispPC(guint8 **code, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_movw_dispPC(imm) && SH4_CHECK_ALIGN_movw_dispPC(imm));
	sh4_emit16(code, (0x9 << 12) | ((Rx & 0xF) << 8) | (((imm & 0x1FE) >> 1) << 0));
}

static inline int is_sh4_movw_dispPC(guint16 code, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_movw_dispPC(imm) && SH4_CHECK_ALIGN_movw_dispPC(imm)
		&& code == ((0x9 << 12) | ((Rx & 0xF) << 8) | (((imm & 0x1FE) >> 1) << 0)));
}

static inline int get_Rx_sh4_movw_dispPC(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_movw_dispPC(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 1;
	g_assert(SH4_CHECK_ALIGN_movw_dispPC(result));
	return result;
}

static inline void sh4_movw_dispR0Ry(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0));
}

static inline int is_sh4_movw_dispR0Ry(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_movw_dispR0Ry(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movw_dispR0Ry(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movw_incRy(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0));
}

static inline int is_sh4_movw_incRy(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_movw_incRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movw_incRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_movw_indRy(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0));
}

static inline int is_sh4_movw_indRy(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0)));
}

static inline int get_Rx_sh4_movw_indRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_movw_indRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_movw_R0_dispRx(imm) ((imm) >= 0 && (imm) <= 30)
#define SH4_CHECK_ALIGN_movw_R0_dispRx(imm) (((imm) & 0x1) == 0)

static inline void sh4_movw_R0_dispRx(guint8 **code, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_movw_R0_dispRx(imm) && SH4_CHECK_ALIGN_movw_R0_dispRx(imm));
	sh4_emit16(code, (0x8 << 12) | (0x1 << 8) | ((Rx & 0xF) << 4) | (((imm & 0x1E) >> 1) << 0));
}

static inline int is_sh4_movw_R0_dispRx(guint16 code, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_movw_R0_dispRx(imm) && SH4_CHECK_ALIGN_movw_R0_dispRx(imm)
		&& code == ((0x8 << 12) | (0x1 << 8) | ((Rx & 0xF) << 4) | (((imm & 0x1E) >> 1) << 0)));
}

static inline int get_Rx_sh4_movw_R0_dispRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
static inline int get_imm_sh4_movw_R0_dispRx(guint16 code)
{
	int result = ((code >> 0) & 0xF) << 1;
	g_assert(SH4_CHECK_ALIGN_movw_R0_dispRx(result));
	return result;
}
#define SH4_CHECK_RANGE_movw_R0_dispGBR(imm) ((imm) >= 0 && (imm) <= 510)
#define SH4_CHECK_ALIGN_movw_R0_dispGBR(imm) (((imm) & 0x1) == 0)

static inline void sh4_movw_R0_dispGBR(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_movw_R0_dispGBR(imm) && SH4_CHECK_ALIGN_movw_R0_dispGBR(imm));
	sh4_emit16(code, (0xC << 12) | (0x1 << 8) | (((imm & 0x1FE) >> 1) << 0));
}

static inline int is_sh4_movw_R0_dispGBR(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_movw_R0_dispGBR(imm) && SH4_CHECK_ALIGN_movw_R0_dispGBR(imm)
		&& code == ((0xC << 12) | (0x1 << 8) | (((imm & 0x1FE) >> 1) << 0)));
}

static inline int get_imm_sh4_movw_R0_dispGBR(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 1;
	g_assert(SH4_CHECK_ALIGN_movw_R0_dispGBR(result));
	return result;
}
#define SH4_CHECK_RANGE_mova_dispPC_R0(imm) ((imm) >= 0 && (imm) <= 1020)
#define SH4_CHECK_ALIGN_mova_dispPC_R0(imm) (((imm) & 0x3) == 0)

static inline void sh4_mova_dispPC_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_mova_dispPC_R0(imm) && SH4_CHECK_ALIGN_mova_dispPC_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x7 << 8) | (((imm & 0x3FC) >> 2) << 0));
}

static inline int is_sh4_mova_dispPC_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_mova_dispPC_R0(imm) && SH4_CHECK_ALIGN_mova_dispPC_R0(imm)
		&& code == ((0xC << 12) | (0x7 << 8) | (((imm & 0x3FC) >> 2) << 0)));
}

static inline int get_imm_sh4_mova_dispPC_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 2;
	g_assert(SH4_CHECK_ALIGN_mova_dispPC_R0(result));
	return result;
}

static inline void sh4_movcal_R0_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xC << 4) | (0x3 << 0));
}

static inline int is_sh4_movcal_R0_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0xC << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_movcal_R0_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_movcol_R0_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x7 << 4) | (0x3 << 0));
}

static inline int is_sh4_movcol_R0_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x7 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_movcol_R0_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_movlil_indRy_R0(guint8 **code, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0x3 << 0));
}

static inline int is_sh4_movlil_indRy_R0(guint16 code, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Ry & 0xF) << 8) | (0x6 << 4) | (0x3 << 0)));
}

static inline int get_Ry_sh4_movlil_indRy_R0(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_movt(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x9 << 0));
}

static inline int is_sh4_movt(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_movt(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_movual_indRy_R0(guint8 **code, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0xA << 4) | (0x9 << 0));
}

static inline int is_sh4_movual_indRy_R0(guint16 code, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Ry & 0xF) << 8) | (0xA << 4) | (0x9 << 0)));
}

static inline int get_Ry_sh4_movual_indRy_R0(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_movual_incRy_R0(guint8 **code, SH4IntRegister Ry)
{
	g_assert(1 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Ry & 0xF) << 8) | (0xE << 4) | (0x9 << 0));
}

static inline int is_sh4_movual_incRy_R0(guint16 code, SH4IntRegister Ry)
{
	return (1 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Ry & 0xF) << 8) | (0xE << 4) | (0x9 << 0)));
}

static inline int get_Ry_sh4_movual_incRy_R0(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_mulsw(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0));
}

static inline int is_sh4_mulsw(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)));
}

static inline int get_Rx_sh4_mulsw(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_mulsw(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_muls(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0));
}

static inline int is_sh4_muls(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)));
}

static inline int get_Rx_sh4_muls(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_muls(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_mull(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0));
}

static inline int is_sh4_mull(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_mull(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_mull(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_muluw(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0));
}

static inline int is_sh4_muluw(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_muluw(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_muluw(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_mulu(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0));
}

static inline int is_sh4_mulu(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_mulu(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_mulu(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_neg(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0));
}

static inline int is_sh4_neg(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_neg(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_neg(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_negc(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0));
}

static inline int is_sh4_negc(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_negc(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_negc(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_nop(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x9 << 0));
}

static inline int is_sh4_nop(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0x9 << 0)));
}


static inline void sh4_not(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0));
}

static inline int is_sh4_not(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_not(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_not(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_ocbi_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x9 << 4) | (0x3 << 0));
}

static inline int is_sh4_ocbi_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x9 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_ocbi_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ocbp_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xA << 4) | (0x3 << 0));
}

static inline int is_sh4_ocbp_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0xA << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_ocbp_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ocbwb_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xB << 4) | (0x3 << 0));
}

static inline int is_sh4_ocbwb_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0xB << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_ocbwb_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_or_imm_R0(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_or_imm_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_or_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0xB << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_or_imm_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_or_imm_R0(imm)
		&& code == ((0xC << 12) | (0xB << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_or_imm_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}

static inline void sh4_or(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0));
}

static inline int is_sh4_or(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_or(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_or(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_orb_imm_dispR0GBR(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_orb_imm_dispR0GBR(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_orb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xF << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_orb_imm_dispR0GBR(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_orb_imm_dispR0GBR(imm)
		&& code == ((0xC << 12) | (0xF << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_orb_imm_dispR0GBR(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}

static inline void sh4_pref_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x8 << 4) | (0x3 << 0));
}

static inline int is_sh4_pref_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x8 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_pref_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_prefi_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xD << 4) | (0x3 << 0));
}

static inline int is_sh4_prefi_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0xD << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_prefi_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_rotcl(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x4 << 0));
}

static inline int is_sh4_rotcl(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x4 << 0)));
}

static inline int get_Rx_sh4_rotcl(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_rotcr(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x5 << 0));
}

static inline int is_sh4_rotcr(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_rotcr(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_rotl(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x4 << 0));
}

static inline int is_sh4_rotl(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x4 << 0)));
}

static inline int get_Rx_sh4_rotl(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_rotr(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x5 << 0));
}

static inline int is_sh4_rotr(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_rotr(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_rte(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0xB << 0));
}

static inline int is_sh4_rte(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x2 << 4) | (0xB << 0)));
}


static inline void sh4_rts(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0xB << 0));
}

static inline int is_sh4_rts(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x0 << 4) | (0xB << 0)));
}


static inline void sh4_sets(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x5 << 4) | (0x8 << 0));
}

static inline int is_sh4_sets(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x5 << 4) | (0x8 << 0)));
}


static inline void sh4_sett(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x8 << 0));
}

static inline int is_sh4_sett(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0x8 << 0)));
}


static inline void sh4_shad(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0));
}

static inline int is_sh4_shad(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)));
}

static inline int get_Rx_sh4_shad(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_shad(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_shld(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0));
}

static inline int is_sh4_shld(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_shld(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_shld(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_shal(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x0 << 0));
}

static inline int is_sh4_shal(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x0 << 0)));
}

static inline int get_Rx_sh4_shal(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shar(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x1 << 0));
}

static inline int is_sh4_shar(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x1 << 0)));
}

static inline int get_Rx_sh4_shar(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shll(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x0 << 0));
}

static inline int is_sh4_shll(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x0 << 0)));
}

static inline int get_Rx_sh4_shll(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shll16(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x8 << 0));
}

static inline int is_sh4_shll16(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_shll16(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shll2(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x8 << 0));
}

static inline int is_sh4_shll2(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_shll2(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shll8(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x8 << 0));
}

static inline int is_sh4_shll8(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_shll8(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shlr(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x1 << 0));
}

static inline int is_sh4_shlr(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x1 << 0)));
}

static inline int get_Rx_sh4_shlr(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shlr16(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x9 << 0));
}

static inline int is_sh4_shlr16(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_shlr16(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shlr2(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x9 << 0));
}

static inline int is_sh4_shlr2(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_shlr2(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_shlr8(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x9 << 0));
}

static inline int is_sh4_shlr8(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_shlr8(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_sleep(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0xB << 0));
}

static inline int is_sh4_sleep(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0x1 << 4) | (0xB << 0)));
}


static inline void sh4_stc_SR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x2 << 0));
}

static inline int is_sh4_stc_SR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stc_SR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stc_GBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x2 << 0));
}

static inline int is_sh4_stc_GBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stc_GBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stc_VBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x2 << 0));
}

static inline int is_sh4_stc_VBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stc_VBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stc_SSR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x2 << 0));
}

static inline int is_sh4_stc_SSR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stc_SSR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stc_SPC(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x2 << 0));
}

static inline int is_sh4_stc_SPC(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stc_SPC(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stc_SGR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xA << 0));
}

static inline int is_sh4_stc_SGR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_stc_SGR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stc_DBR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0xA << 0));
}

static inline int is_sh4_stc_DBR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_stc_DBR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_stc_bank(imm) ((imm) >= 0 && (imm) <= 7)

static inline void sh4_stc_bank(guint8 **code, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_stc_bank(imm));
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x2 << 0));
}

static inline int is_sh4_stc_bank(guint16 code, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_stc_bank(imm)
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stc_bank(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stcl_SR_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x3 << 0));
}

static inline int is_sh4_stcl_SR_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_stcl_SR_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stcl_VBR_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x3 << 0));
}

static inline int is_sh4_stcl_VBR_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_stcl_VBR_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stcl_SSR_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x3 << 0));
}

static inline int is_sh4_stcl_SSR_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_stcl_SSR_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stcl_SPC_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x3 << 0));
}

static inline int is_sh4_stcl_SPC_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_stcl_SPC_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stcl_GBR_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x3 << 0));
}

static inline int is_sh4_stcl_GBR_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_stcl_GBR_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stcl_SGR_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x2 << 0));
}

static inline int is_sh4_stcl_SGR_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stcl_SGR_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stcl_DBR_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0x2 << 0));
}

static inline int is_sh4_stcl_DBR_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stcl_DBR_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_stcl_bank_decRx(imm) ((imm) >= 0 && (imm) <= 7)

static inline void sh4_stcl_bank_decRx(guint8 **code, int imm, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_stcl_bank_decRx(imm));
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x3 << 0));
}

static inline int is_sh4_stcl_bank_decRx(guint16 code, int imm, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && SH4_CHECK_RANGE_stcl_bank_decRx(imm)
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | ((imm & 0x7) << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_stcl_bank_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_sts_MACH(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xA << 0));
}

static inline int is_sh4_sts_MACH(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_sts_MACH(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_sts_MACL(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xA << 0));
}

static inline int is_sh4_sts_MACL(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_sts_MACL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_sts_PR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xA << 0));
}

static inline int is_sh4_sts_PR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_sts_PR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_sts_FPUL(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xA << 0));
}

static inline int is_sh4_sts_FPUL(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_sts_FPUL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_sts_FPSCR(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xA << 0));
}

static inline int is_sh4_sts_FPSCR(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_sts_FPSCR(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stsl_MACH_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x2 << 0));
}

static inline int is_sh4_stsl_MACH_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stsl_MACH_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stsl_MACL_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x2 << 0));
}

static inline int is_sh4_stsl_MACL_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stsl_MACL_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stsl_PR_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x2 << 0));
}

static inline int is_sh4_stsl_PR_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stsl_PR_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stsl_FPUL_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0x2 << 0));
}

static inline int is_sh4_stsl_FPUL_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stsl_FPUL_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_stsl_FPSCR_decRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0x2 << 0));
}

static inline int is_sh4_stsl_FPSCR_decRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_stsl_FPSCR_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_sub(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0));
}

static inline int is_sh4_sub(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_sub(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_sub(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_subc(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0));
}

static inline int is_sh4_subc(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_subc(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_subc(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_subv(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0));
}

static inline int is_sh4_subv(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_subv(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_subv(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_swapb(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0));
}

static inline int is_sh4_swapb(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_swapb(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_swapb(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_swapw(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0));
}

static inline int is_sh4_swapw(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x6 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_swapw(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_swapw(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_synco(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0x0 << 12) | (0x0 << 8) | (0xA << 4) | (0xB << 0));
}

static inline int is_sh4_synco(guint16 code)
{
	return (1
		&& code == ((0x0 << 12) | (0x0 << 8) | (0xA << 4) | (0xB << 0)));
}


static inline void sh4_tasb_indRx(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xB << 0));
}

static inline int is_sh4_tasb_indRx(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_tasb_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_trapa_imm(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_trapa_imm(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_trapa_imm(imm));
	sh4_emit16(code, (0xC << 12) | (0x3 << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_trapa_imm(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_trapa_imm(imm)
		&& code == ((0xC << 12) | (0x3 << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_trapa_imm(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_tst_imm_R0(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_tst_imm_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_tst_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0x8 << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_tst_imm_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_tst_imm_R0(imm)
		&& code == ((0xC << 12) | (0x8 << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_tst_imm_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}

static inline void sh4_tst(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0));
}

static inline int is_sh4_tst(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_tst(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_tst(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_tstb_imm_dispR0GBR(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_tstb_imm_dispR0GBR(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_tstb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xC << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_tstb_imm_dispR0GBR(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_tstb_imm_dispR0GBR(imm)
		&& code == ((0xC << 12) | (0xC << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_tstb_imm_dispR0GBR(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_xor_imm_R0(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_xor_imm_R0(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_xor_imm_R0(imm));
	sh4_emit16(code, (0xC << 12) | (0xA << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_xor_imm_R0(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_xor_imm_R0(imm)
		&& code == ((0xC << 12) | (0xA << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_xor_imm_R0(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}

static inline void sh4_xor(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0));
}

static inline int is_sh4_xor(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_xor(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_xor(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}
#define SH4_CHECK_RANGE_xorb_imm_dispR0GBR(imm) ((imm) >= 0 && (imm) <= 255)

static inline void sh4_xorb_imm_dispR0GBR(guint8 **code, int imm)
{
	g_assert(1 && SH4_CHECK_RANGE_xorb_imm_dispR0GBR(imm));
	sh4_emit16(code, (0xC << 12) | (0xE << 8) | ((imm & 0xFF) << 0));
}

static inline int is_sh4_xorb_imm_dispR0GBR(guint16 code, int imm)
{
	return (1 && SH4_CHECK_RANGE_xorb_imm_dispR0GBR(imm)
		&& code == ((0xC << 12) | (0xE << 8) | ((imm & 0xFF) << 0)));
}

static inline int get_imm_sh4_xorb_imm_dispR0GBR(guint16 code)
{
	int result = ((code >> 0) & 0xFF) << 0;
	return result;
}

static inline void sh4_xtrct(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0));
}

static inline int is_sh4_xtrct(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x2 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_xtrct(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_xtrct(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_dt(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x0 << 0));
}

static inline int is_sh4_dt(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x4 << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0x0 << 0)));
}

static inline int get_Rx_sh4_dt(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_dmulsl(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0));
}

static inline int is_sh4_dmulsl(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_dmulsl(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_dmulsl(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_dmulul(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0));
}

static inline int is_sh4_dmulul(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x3 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_dmulul(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_dmulul(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_macl_incRy_incRx(guint8 **code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0));
}

static inline int is_sh4_macl_incRy_incRx(guint16 code, SH4IntRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xF << 0)));
}

static inline int get_Rx_sh4_macl_incRy_incRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_macl_incRy_incRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_braf(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x3 << 0));
}

static inline int is_sh4_braf(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_braf(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_bsrf(guint8 **code, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x3 << 0));
}

static inline int is_sh4_bsrf(guint16 code, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0x0 << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_bsrf(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fabs(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xD << 0));
}

static inline int is_sh4_fabs(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fabs(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fabs_double(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xD << 0));
}

static inline int is_sh4_fabs_double(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x5 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fabs_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fadd(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0));
}

static inline int is_sh4_fadd(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)));
}

static inline int get_Rx_sh4_fadd(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fadd(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fadd_double(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0));
}

static inline int is_sh4_fadd_double(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x0 << 0)));
}

static inline int get_Rx_sh4_fadd_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fadd_double(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fcmpeq(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0));
}

static inline int is_sh4_fcmpeq(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)));
}

static inline int get_Rx_sh4_fcmpeq(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fcmpeq(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fcmpeq_double(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0));
}

static inline int is_sh4_fcmpeq_double(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x4 << 0)));
}

static inline int get_Rx_sh4_fcmpeq_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fcmpeq_double(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fcmpgt(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0));
}

static inline int is_sh4_fcmpgt(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_fcmpgt(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fcmpgt(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fcmpgt_double(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0));
}

static inline int is_sh4_fcmpgt_double(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x5 << 0)));
}

static inline int get_Rx_sh4_fcmpgt_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fcmpgt_double(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fcnvds_double_FPUL(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && !(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0xB << 4) | (0xD << 0));
}

static inline int is_sh4_fcnvds_double_FPUL(guint16 code, SH4FloatRegister Rx)
{
	return (1 && !(Rx & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0xB << 4) | (0xD << 0)));
}


static inline void sh4_fcnvsd_FPUL_double(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && !(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0xA << 4) | (0xD << 0));
}

static inline int is_sh4_fcnvsd_FPUL_double(guint16 code, SH4FloatRegister Rx)
{
	return (1 && !(Rx & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0xA << 4) | (0xD << 0)));
}


static inline void sh4_fdiv(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0));
}

static inline int is_sh4_fdiv(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_fdiv(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fdiv(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fdiv_double(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0));
}

static inline int is_sh4_fdiv_double(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x3 << 0)));
}

static inline int get_Rx_sh4_fdiv_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fdiv_double(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fipr(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && !((Rx & 0x3) || (Ry & 0x3)));
	sh4_emit16(code, (0xF << 12) | ((((Rx & 0xF) << 2) | ((Ry & 0xF) >> 2)) << 8) | (0xE << 4) | (0xD << 0));
}

static inline int is_sh4_fipr(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && !((Rx & 0x3) || (Ry & 0x3))
		&& code == ((0xF << 12) | ((((Rx & 0xF) << 2) | ((Ry & 0xF) >> 2)) << 8) | (0xE << 4) | (0xD << 0)));
}


static inline void sh4_fldi0(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x8 << 4) | (0xD << 0));
}

static inline int is_sh4_fldi0(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x8 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fldi0(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fldi1(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x9 << 4) | (0xD << 0));
}

static inline int is_sh4_fldi1(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x9 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fldi1(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_flds_FPUL(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xD << 0));
}

static inline int is_sh4_flds_FPUL(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x1 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_flds_FPUL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_float_FPUL(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xD << 0));
}

static inline int is_sh4_float_FPUL(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_float_FPUL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_float_FPUL_double(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xD << 0));
}

static inline int is_sh4_float_FPUL_double(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x2 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_float_FPUL_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fmac(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0));
}

static inline int is_sh4_fmac(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xE << 0)));
}

static inline int get_Rx_sh4_fmac(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmac(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0));
}

static inline int is_sh4_fmov(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)));
}

static inline int get_Rx_sh4_fmov(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_Xdouble_Xdouble(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0));
}

static inline int is_sh4_fmov_Xdouble_Xdouble(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xC << 0)));
}

static inline int get_Rx_sh4_fmov_Xdouble_Xdouble(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_Xdouble_Xdouble(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_indRy(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0));
}

static inline int is_sh4_fmov_indRy(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_fmov_indRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_indRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_indRy_Xdouble(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0));
}

static inline int is_sh4_fmov_indRy_Xdouble(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_fmov_indRy_Xdouble(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_indRy_Xdouble(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_indRx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0));
}

static inline int is_sh4_fmov_indRx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_fmov_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_indRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_Xdouble_indRx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0));
}

static inline int is_sh4_fmov_Xdouble_indRx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_fmov_Xdouble_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_Xdouble_indRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_incRy(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0));
}

static inline int is_sh4_fmov_incRy(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_fmov_incRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_incRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_incRy_Xdouble(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0));
}

static inline int is_sh4_fmov_incRy_Xdouble(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_fmov_incRy_Xdouble(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_incRy_Xdouble(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_decRx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0));
}

static inline int is_sh4_fmov_decRx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_fmov_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_decRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_Xdouble_decRx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0));
}

static inline int is_sh4_fmov_Xdouble_decRx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_fmov_Xdouble_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_Xdouble_decRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_dispR0Ry(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0));
}

static inline int is_sh4_fmov_dispR0Ry(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_fmov_dispR0Ry(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_dispR0Ry(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_dispR0Ry_Xdouble(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0));
}

static inline int is_sh4_fmov_dispR0Ry_Xdouble(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_fmov_dispR0Ry_Xdouble(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_dispR0Ry_Xdouble(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_dispR0Rx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0));
}

static inline int is_sh4_fmov_dispR0Rx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_fmov_dispR0Rx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_dispR0Rx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmov_Xdouble_dispR0Rx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0));
}

static inline int is_sh4_fmov_Xdouble_dispR0Rx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_fmov_Xdouble_dispR0Rx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmov_Xdouble_dispR0Rx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovd_indRy_Xdouble(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0));
}

static inline int is_sh4_fmovd_indRy_Xdouble(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_fmovd_indRy_Xdouble(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovd_indRy_Xdouble(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovd_Xdouble_indRx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0));
}

static inline int is_sh4_fmovd_Xdouble_indRx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_fmovd_Xdouble_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovd_Xdouble_indRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovd_incRy_Xdouble(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0));
}

static inline int is_sh4_fmovd_incRy_Xdouble(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_fmovd_incRy_Xdouble(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovd_incRy_Xdouble(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovd_Xdouble_decRx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0));
}

static inline int is_sh4_fmovd_Xdouble_decRx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_fmovd_Xdouble_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovd_Xdouble_decRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovd_dispR0Ry_Xdouble(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0));
}

static inline int is_sh4_fmovd_dispR0Ry_Xdouble(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_fmovd_dispR0Ry_Xdouble(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovd_dispR0Ry_Xdouble(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovd_Xdouble_dispR0Rx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0));
}

static inline int is_sh4_fmovd_Xdouble_dispR0Rx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_fmovd_Xdouble_dispR0Rx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovd_Xdouble_dispR0Rx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovs_indRy(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0));
}

static inline int is_sh4_fmovs_indRy(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x8 << 0)));
}

static inline int get_Rx_sh4_fmovs_indRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovs_indRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovs_indRx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0));
}

static inline int is_sh4_fmovs_indRx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xA << 0)));
}

static inline int get_Rx_sh4_fmovs_indRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovs_indRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovs_incRy(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0));
}

static inline int is_sh4_fmovs_incRy(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x9 << 0)));
}

static inline int get_Rx_sh4_fmovs_incRy(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovs_incRy(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovs_decRx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0));
}

static inline int is_sh4_fmovs_decRx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0xB << 0)));
}

static inline int get_Rx_sh4_fmovs_decRx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovs_decRx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovs_dispR0Ry(guint8 **code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0));
}

static inline int is_sh4_fmovs_dispR0Ry(guint16 code, SH4IntRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x6 << 0)));
}

static inline int get_Rx_sh4_fmovs_dispR0Ry(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovs_dispR0Ry(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmovs_dispR0Rx(guint8 **code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0));
}

static inline int is_sh4_fmovs_dispR0Rx(guint16 code, SH4FloatRegister Ry, SH4IntRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x7 << 0)));
}

static inline int get_Rx_sh4_fmovs_dispR0Rx(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmovs_dispR0Rx(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmul(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0));
}

static inline int is_sh4_fmul(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_fmul(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmul(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fmul_double(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0));
}

static inline int is_sh4_fmul_double(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x2 << 0)));
}

static inline int get_Rx_sh4_fmul_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fmul_double(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fneg(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xD << 0));
}

static inline int is_sh4_fneg(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fneg(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fneg_double(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xD << 0));
}

static inline int is_sh4_fneg_double(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x4 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fneg_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fpchg(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0xF << 12) | (0x7 << 8) | (0xF << 4) | (0xD << 0));
}

static inline int is_sh4_fpchg(guint16 code)
{
	return (1
		&& code == ((0xF << 12) | (0x7 << 8) | (0xF << 4) | (0xD << 0)));
}


static inline void sh4_frchg(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0xF << 12) | (0xB << 8) | (0xF << 4) | (0xD << 0));
}

static inline int is_sh4_frchg(guint16 code)
{
	return (1
		&& code == ((0xF << 12) | (0xB << 8) | (0xF << 4) | (0xD << 0)));
}


static inline void sh4_fsca_FPUL_double(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && !(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0xD << 0));
}

static inline int is_sh4_fsca_FPUL_double(guint16 code, SH4FloatRegister Rx)
{
	return (1 && !(Rx & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0xF << 4) | (0xD << 0)));
}


static inline void sh4_fschg(guint8 **code)
{
	g_assert(1);
	sh4_emit16(code, (0xF << 12) | (0x3 << 8) | (0xF << 4) | (0xD << 0));
}

static inline int is_sh4_fschg(guint16 code)
{
	return (1
		&& code == ((0xF << 12) | (0x3 << 8) | (0xF << 4) | (0xD << 0)));
}


static inline void sh4_fsqrt(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xD << 0));
}

static inline int is_sh4_fsqrt(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fsqrt(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fsqrt_double(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xD << 0));
}

static inline int is_sh4_fsqrt_double(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x6 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fsqrt_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fsrra(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x7 << 4) | (0xD << 0));
}

static inline int is_sh4_fsrra(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x7 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fsrra(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fsts_FPUL(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xD << 0));
}

static inline int is_sh4_fsts_FPUL(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x0 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_fsts_FPUL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_fsub(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0));
}

static inline int is_sh4_fsub(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && Ry >= 0 && Ry <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0)));
}

static inline int get_Rx_sh4_fsub(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fsub(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_fsub_double(guint8 **code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0));
}

static inline int is_sh4_fsub_double(guint16 code, SH4FloatRegister Ry, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1) && Ry >= 0 && Ry <= 15 && !(Ry & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | ((Ry & 0xF) << 4) | (0x1 << 0)));
}

static inline int get_Rx_sh4_fsub_double(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}
static inline int get_Ry_sh4_fsub_double(guint16 code)
{
	int result = ((code >> 4) & 0xF) << 0;
	return result;
}

static inline void sh4_ftrc_FPUL(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15);
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xD << 0));
}

static inline int is_sh4_ftrc_FPUL(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_ftrc_FPUL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ftrc_double_FPUL(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1));
	sh4_emit16(code, (0xF << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xD << 0));
}

static inline int is_sh4_ftrc_double_FPUL(guint16 code, SH4FloatRegister Rx)
{
	return (1 && Rx >= 0 && Rx <= 15 && !(Rx & 0x1)
		&& code == ((0xF << 12) | ((Rx & 0xF) << 8) | (0x3 << 4) | (0xD << 0)));
}

static inline int get_Rx_sh4_ftrc_double_FPUL(guint16 code)
{
	int result = ((code >> 8) & 0xF) << 0;
	return result;
}

static inline void sh4_ftrv(guint8 **code, SH4FloatRegister Rx)
{
	g_assert(1 && !(Rx & 0x3));
	sh4_emit16(code, (0xF << 12) | ((((Rx & 0xF) << 2) | 0x1) << 8) | (0xF << 4) | (0xD << 0));
}

static inline int is_sh4_ftrv(guint16 code, SH4FloatRegister Rx)
{
	return (1 && !(Rx & 0x3)
		&& code == ((0xF << 12) | ((((Rx & 0xF) << 2) | 0x1) << 8) | (0xF << 4) | (0xD << 0)));
}

#include "sh4-codegen-footer.h"

#endif /* __MONO_SH4_CODEGEN_H__ */

