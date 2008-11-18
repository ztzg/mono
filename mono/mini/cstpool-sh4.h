/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * cstpool-sh4.h: SH4 backend for the Mono code generator
 *
 * Authors:
 *   Yves   JANIN   (yves.janin@st.com)
 *
 * (C) 2008 STMicroelectronics.
 */

#include "mini.h"

#ifndef CSTPOOL_SH4_H
#define CSTPOOL_SH4_H

#define SH4_MAX_CSTPOOL       255 /* Maximum number of constants in a pool */

/* Tackling constant pools */
typedef enum {
	cstpool_undef     = 0,
	cstpool_filling   = 1,   /* The only state where you can add data */
	cstpool_allocated = 2,
	cstpool_patched   = 3,
} CstPool_State;

/* Float and double values add some complexity */
enum CstPool_Const_Type {
	cstpool_type_int     = 0,     /* Integer */
	cstpool_type_float   = 1,     /* Float   */
	cstpool_type_double  = 2,     /* Double: 32 low  bits */
};

/* This structure should only be used in file cstpool-sh4.c
 * where elementary functions to manipulate its fields
 * are defined.
 */
typedef struct{
	guint16  const_type;          /* CstPool_Const_Type */
	union {

		union {
			float   f;
			guint32 i;
		}u2;

		union {
			double  d;
			guint32 tabint[2];
		}u3;

	} u1;
} CstPool_Const_Values;

typedef struct MonoSH4CstPool{
	struct  MonoSH4CstPool *next;
	CstPool_State state    ;
	guint32 pool_off0_start; /* |pc/2|*2 of this first instruction */
				 /* that contributes to the memory pool*/

	guint32  pool_off;       /* Base address of the pool           */
	guint32  pool_nbcst;     /* Number of constants in the pool    */

	/* Memorize constant - Actual type of field "type" is    */
	/* MonoJumpInfoType. It is defined in mini.h that        */
	/* includes mini-arch.h that includes it self mini-sh4.h.*/
	gint16          type    [SH4_MAX_CSTPOOL];
	guint16         reg     [SH4_MAX_CSTPOOL];
	gconstpointer   pool_cst[SH4_MAX_CSTPOOL];
	guint32         off_inst[SH4_MAX_CSTPOOL];

	guint32  pool_nbcst_emitted; /* Temporary -                   */
} MonoSH4CstPool;

typedef struct {
	MonoMemPool    *mempool;
	MonoSH4CstPool *start;
	MonoSH4CstPool *last;

	guint32         nbpool;        /* For statistics and debug only */
	guint32         nbcst;         /* For statistics and debug only */

	guint32         nb_bblocks;
	guint32        *tab_bb_offset;
} MonoSH4CstPool_Env;

void sh4_cstpool_init(MonoCompile *cfg);
void sh4_cstpool_end(MonoCompile *cfg);
void sh4_cstpool_add(MonoCompile *cfg, guint8 **pcval,
                     MonoJumpInfoType type, gconstpointer target,
                     guint32 reg);
void sh4_cstpool_addf(MonoCompile *cfg, guint8 **pcval, float f);
void sh4_cstpool_addd(MonoCompile *cfg, guint8 **pcval, double d);
void sh4_cstpool_check(void *cfg, guint8 **pcval);
void sh4_cstpool_check_end_bb(MonoCompile *cfg, MonoBasicBlock *bb,
                              guint8 **pcval);
void sh4_cstpool_check_begin_bb(MonoCompile *cfg, MonoBasicBlock *bb,
				guint8 **pcval);
gboolean sh4_cstpool_get_bb_address(MonoCompile *cfg, MonoBasicBlock *bb,
                                    guint32 *offset);

void sh4_realloc_buf_if_needed(MonoCompile *cfg, guint32 offset,
                               guint32 length_max,guint8 **pcval);

#endif  /* CSTPOOL_SH4_H  */
