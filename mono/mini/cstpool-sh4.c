/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * cstpool-sh4.c: constant pool service for the SH4 backend.
 *
 * Copyright (c) 2009 STMicroelectronics
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Yves JANIN (yves.janin@st.com)
 *   Denis FERRANTI (denis.ferranti@st.com)
 *   Cedric VINCENT (cedric.vincent@st.com)
 */

#include <glib.h>
#include "cstpool-sh4.h"

/**
 * Constant pool management
 *
 *
 * I Introduction
 *   ============
 *
 * sh4 features constant pools as a way to load immediates into registers.
 * The sequence of code for loading an 32 bit immediate from a
 * constant pool typically looks:
 *
 *    MOV.L @(disp,PC),Rn
 *
 * The effective address in @() addressing mode is calculated as follows:
 *
 * addr = disp * 4 + (PC & ~0x3)
 *
 * where disp is an 8 bit unsigned immediate. The first two bits of the
 * PC are cleared so as to garantee correct alignments when 32-bit
 * data are loaded.
 *
 * The following diagram depicts what happens on a simple example 
 * where constant 0xdeadbeef is loaded into register R1.
 *
 * Addr    0x00
 *  -----  0x02 MOV.L @(0x6,PC),R1   <-  current value of PC = 0x02
 *  |      0x04
 *  |      0x06
 *  |      0x08
 *  |      0x0A
 *  |      0x0C
 * ...|............
 *  |      0x10             // Beginning of the constant pool
 * ...|............
 *  |----> 0x18 0xdeadbeef  // Value of the loaded constant
 * ................
 *         0x30             // Beginning of the next basic block (code).
 *
 * Loading a constant is an operation that can be followed by
 * several others operations including branches. In such a case,
 * code is typically:
 *
 *    MOV.L (@disp,PC),Rn
 *    BSRF  Rn             // PC <- PC + 4 + Rn   call of a subroutine 
 *
 * or
 *    MOV.L (@disp,PC),Rn
 *    BRAF  Rn             // PC <- PC + 4 + Rn   unconditional branch instruction.
 *
 * Note that these two sequences of instructions are to be used only when
 * immediate versions of conditional/unconditionals branch and jump
 * instructions can't be used (*1).
 *
 * Warning: The constant pool should not be emitted between an
 * instruction and its delay slot.
 *
 * Float and double constants
 * --------------------------
 *
 * The case of float and double constants is somewhat special since sh4
 * architecture defines float registers (FRi where i=[0..15]). Double
 * registers (DRj where j=[0..14]) are made up of 2 float registers. For
 * instance DR2 is composed of FR2 and FR3.
 *
 * (*1) currently, immediate jump and branch instructions aren't generated,
 *      But they could be in a near (well not so near :-) future.
 *
 * II Constant pool management as a service
 * ========================================
 *
 * Constant pool management is to be seen a dedicated service by the rest of
 * the code selector. Arguments in favor of such a separation are:
 *
 * -1) since constant pool management may be fairly complex, we don't want to
 *     bother code selection writer with this task.
 *
 * -2) separating constant pool management from the rest of the
 *     code should ease maintainability on both sides.
 *
 * -3) in order to test and try various strategies for constant pools, we need
 *     to have a well isolated piece of code.
 *
 * API definition
 * ==============
 *
 * The following functions (or macros) are also defined:
 *
 * void sh4_cstpool_init(MonoCompile *cfg, CstPool_Mode mode).
 *
 *     Initialization steps.
 *
 * void sh4_cstpool_check(MonoCompile *cfg,
 *                        guint8      *pcval) // PC current value
 *
 *     To be called for each generated (SH4) instruction whatever the
 *     instruction.
 *
 *     In most cases, this routine just updates some internal
 *     offset counter. For end block instructions, it may generate a
 *     constant pool if required.
 *
 *     In case generation of a constant pool is necessary within
 *     a basic block (things that may happen for large basic blocks),
 *     it also generates the constant pool as well as the necessary
 *     jump above the generated constant pool.
 *
 * void sh4_cstpool_check_end_bb(MonoCompile *cfg,
 *                               guint8      *pcval)
 *                               
 *      To be called at the end of each basic block.                               
 *
 * void sh4_cstpool_check_begin_bb(MonoCompile *cfg,
 *                                 guint8      *pcval)
 *
 *      To be called at the beginning of each basic block.
 *
 * void sh4_cstpool_add(MonoCompile *cfg, guint8 **pcval, MonoJumpInfoType type, gconstpointer target, SH4IntRegister reg);
 *
 *     Generate an instruction that loads a 32 bit datum into
 *     a register whose index is given. Update internal representation
 *     and store all necessary information to do further patches.
 *
 * void sh4_cstpool_addf(MonoCompile      *cfg,
 *                       guint8          **pcval,  // PC current value
 *                       float             f);
 *     Generate a mova instruction that enables the next instruction
 *     to load a float constant. Note that register r0 is clobbered
 *     in this routine.
 *
 * void sh4_cstpool_addd(MonoCompile      *cfg,
 *                       guint8          **pcval,  // PC current value
 *                       double            d);
 *     Generate a mova instruction that enables next instructions
 *     to load a double constant. Note that register r0 is clobbered
 *     in this routine.
 *
 * gboolean sh4_cstpool_get_bb_address(MonoCompile *cfg,
 *                                     guint32      bb_id
 *                                     guint32     &offset)
 *
 *      For a given basic block whose identifier is given, returns TRUE
 *      if offset (relative address) of this basic block is known. Returns
 *      FALSE otherwise.
 *      This function should help some decisions in emit basic block routines.
 *
 * gboolean sh4_cstpool_decide_emission(MonoCompile *cfg,gboolean end_bb,
 *					MonoBasicBlock *bb, guint32 *size)
 *
 *	Decide whether current pool is to be emitted right away. Returns TRUE
 *	if so, otherwise returns FALSE. If constant pool is to be emitted,
 *	returns the size of the memory required by constant pool emission.
 *
 * void sh4_cstpool_end(MonoCompile *cfg)
 *
 *     Final call. To be called after the last instruction of the
 *     last basic block. Flush the last constant pool if necessary.
 *     Do some patches. Free memory associated to the process...
 *
 * III Algorithms
 * ==============
 *
 * TODO
 *
 * Remark: Since native code buffer can change, it is necessary to store
 *         address pool information (see MonoSH4CstPool) as OFFSETS.
 */

/* Prototype declarations */
static inline void*    sh4_cstpool_malloc(MonoMemPool *pool, guint32 size);
static inline gboolean sh4_is_valid_offset(guint32 offset);

static inline guint32  sh4_cst_type(CstPool_Const_Values *g);
static inline gboolean sh4_cst_is_double(CstPool_Const_Values *g);

static inline guint32  sh4_get_int(CstPool_Const_Values *g);
static inline guint32  sh4_get_int_from_float(CstPool_Const_Values *g);
static inline guint32  sh4_get_32low_from_double(CstPool_Const_Values* g);
static inline guint32  sh4_get_32high_from_double(CstPool_Const_Values* g);

static inline CstPool_Const_Values *sh4_build_cst_int(MonoMemPool *pool, guint32 i);
static inline CstPool_Const_Values *sh4_build_cst_float(MonoMemPool *pool, float f);
static inline CstPool_Const_Values *sh4_build_cst_double(MonoMemPool *pool, double d);

static inline void  sh4_cstpool_addpool(MonoCompile*);

static void         sh4_emit_pool_lowperf(MonoCompile*, gboolean, guint8 **pcval);
static void         sh4_emit_pool_fullperf(MonoCompile*, gboolean, guint8 **pcval);
static void         sh4_cstpool_add_internal(MonoCompile *cfg,
                                             guint8 **pcval,
				             MonoJumpInfoType type,
                                             gconstpointer target,
                                             guint32  reg,
                                             CstPool_Const_Values *g);

/* We don't want to call recursively sh4_cstpool_check in
 * routines that actually generate code for constant pool.
 */
#define NO_recur NULL

/* Beginning of code .....................................................*/

/**
 * If pool is NULL, we call the classical malloc.
 * If not, we allocate memory in the pool.
 * In any cases, allocated memory is cleared.
 *
 */
static inline void*
sh4_cstpool_malloc(MonoMemPool *pool, guint32 size)
{
	if(pool==NULL)
		return g_malloc0(size);
	else
		return mono_mempool_alloc0(pool,size);
}

static inline gboolean
sh4_is_valid_offset(guint32 offset)
{
	return (offset&0x1)==0 ? TRUE : FALSE;
}

/* Access routines for CstPool_Const_Values structure. */
static inline guint32 sh4_cst_type(CstPool_Const_Values *g)
{
	return g->const_type;
}

static inline CstPool_Const_Values *sh4_build_cst_int(MonoMemPool *pool, guint32 i)
{
	CstPool_Const_Values *g;

	g = sh4_cstpool_malloc(pool, sizeof(CstPool_Const_Values));
	g->const_type = cstpool_type_int;
	g->u1.u2.i = i;

	return g;
}

static inline CstPool_Const_Values *sh4_build_cst_float(MonoMemPool *pool, float f)
{
	CstPool_Const_Values *g;

	g = sh4_cstpool_malloc(pool, sizeof(CstPool_Const_Values));
	g->const_type = cstpool_type_float;
	g->u1.u2.f = f;

	return g;
}

static inline CstPool_Const_Values *sh4_build_cst_double(MonoMemPool *pool, double d)
{
	CstPool_Const_Values *g;

	g = sh4_cstpool_malloc(pool, sizeof(CstPool_Const_Values));
	g->const_type = cstpool_type_double;
	g->u1.u3.d = d;

	return g;
}

static inline gboolean sh4_cst_is_double(CstPool_Const_Values *g)
{
	return g->const_type == cstpool_type_double ? TRUE : FALSE;
}

static inline guint32 sh4_get_int(CstPool_Const_Values* g)
{
	return g->u1.u2.i;
}

static inline guint32 sh4_get_int_from_float(CstPool_Const_Values* g)
{
	return g->u1.u2.i;
}

/* Warning, depends on endianness! */
static inline guint32 sh4_get_32low_from_double(CstPool_Const_Values* g)
{
	return g->u1.u3.tabint[0];
}

/* Warning, depends on endianness! */
static inline guint32 sh4_get_32high_from_double(CstPool_Const_Values* g)
{
	return g->u1.u3.tabint[1];
}

/**
 * Adding a constant pool in the environement
 */
static inline void sh4_cstpool_addpool(MonoCompile *cfg)
{
	MonoSH4CstPool_Env *env = cfg->arch.poolenv;
	MonoSH4CstPool     *tmp;

	tmp = sh4_cstpool_malloc(env->mempool,sizeof(MonoSH4CstPool));

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p -- Create pool %d", cfg, env->nbpool);

	/* Initialize pool */
	tmp->next            = NULL;
	tmp->state           = cstpool_filling;
	tmp->pool_nbcst      = 0;
	tmp->pool_off        = 0xFFFFFFFF;
	tmp->pool_off0_start = 0xFFFFFFFF;
	tmp->pool_nbcst_emitted = 0;

	/* Initialize linked list */
	env->last = tmp;
	if(env->start==NULL) {
		env->start=tmp;
	}

	++(env->nbpool);   /* One more pool */
	return;
}

/**
 * Overall initialization.
 */
void sh4_cstpool_init(MonoCompile *cfg, CstPool_Mode mode)
{
	MonoSH4CstPool_Env *env ;
	MonoMemPool        *mempool;
	int i;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	mempool = mono_mempool_new();

	/* Deal with constant pools.
	 */
	cfg->arch.poolenv = sh4_cstpool_malloc(mempool,sizeof(MonoSH4CstPool_Env));
	env = cfg->arch.poolenv;
	env->mempool = mempool;

	env->mode = (mode == cstpool_mode_fullperf ? mode : cstpool_mode_lowperf);

	env->nbcst = 0;
	env->nbpool= 0;
	env->start = env->last = NULL;

	/* Deal with basic blocks */
	env->nb_bblocks = cfg->num_bblocks;
	if(env->nb_bblocks) {
		guint32 size = sizeof(guint32) * env->nb_bblocks;
		env->tab_bb_offset = sh4_cstpool_malloc(mempool,size);

		for(i=0;i<env->nb_bblocks;++i) {
			env->tab_bb_offset[i] = 0xFFFFFFFF;   /* Invalid offset */
		}
	} else {
		env->tab_bb_offset = NULL;
	}

	return;
}

/**
 * Called at the end of each compilation unit.
 */
void sh4_cstpool_end(MonoCompile *cfg)
{
	MonoSH4CstPool_Env *env = cfg->arch.poolenv;

	SH4_CFG_DEBUG(4) SH4_DEBUG("args => %p", cfg);

	/* (parts of) this code will be removed when memory pools are used */
	if(env!=NULL) {
		SH4_CFG_DEBUG(4) SH4_DEBUG("nb of cst pools %d, overall nb of cst %d",
					   env->nbpool, env->nbcst);

		mono_mempool_destroy(env->mempool);
		cfg->arch.poolenv = NULL;
	}

	return;
}

static void
sh4_emit_pool_lowperf(MonoCompile *cfg, gboolean end_bb, guint8 **pcval)
{
	MonoSH4CstPool *cur_pool = ((MonoSH4CstPool_Env *)cfg->arch.poolenv)->last;

	guint32 index;        /* Index of current item in data pool */
	CstPool_Const_Values *g;

	guint8   *patch0;
	guint8   *patch1;
	guint8   *dest;
	guint32   value1, value2;
	guint32   nb_int_const;
	gboolean  is_float_or_double;

	/* We generate a sequence of 6 + n*4 bytes (n beeing the nb of 	*/
	/* variables emitted). In the following, we track the overall 	*/
	/* code size allocated (see sz "metavariable"). 		*/
	patch1 = *pcval;

	sh4_bra(NO_recur,pcval, 0x0); /* 0x0 to be patched. sz = 2   */
	sh4_nop(NO_recur,pcval);      /* delay slot.        sz = 4   */

	if(((guint32)*pcval & 0x3)) {                     /* sz<=6   */
		sh4_nop(NO_recur,pcval);      /* Align constant pool */
	}

	while(cur_pool->pool_nbcst_emitted < cur_pool->pool_nbcst) {
		index  = cur_pool->pool_nbcst_emitted;
		SH4_CFG_DEBUG(4) SH4_DEBUG("emit constant nr %d (cfg %p)",index,cfg);

		/* constant is here. */
		dest = *pcval;

		is_float_or_double = FALSE;       /* Default value */

		if(cur_pool->type[index] != MONO_PATCH_INFO_NONE) {
			mono_add_patch_info(cfg,
					    *pcval - cfg->native_code,
					    cur_pool->type[index],
					    cur_pool->pool_cst[index]);
			sh4_emit32(pcval,0U); /* constant pool allocation  - patched later*/
						/* sz <= 10 */
			nb_int_const = 1U;
		} else {
			g = (CstPool_Const_Values*) (cur_pool->pool_cst[index]);

			g_assert(g!=NULL);

			switch(sh4_cst_type(g)) {
			case cstpool_type_int:
				value1 = sh4_get_int(g);
				sh4_emit32(pcval,value1);   /* constant pool allocation - sz <= 6 + n*4*/
				nb_int_const = 1U;

				SH4_CFG_DEBUG(4) SH4_DEBUG("constant value 0x%08x\n",value1);
			break;

			case cstpool_type_float:
				value1 = sh4_get_int_from_float(g);
				sh4_emit32(pcval,value1);    /* constant pool allocation - sz <= 6 + n*4*/
				nb_int_const = 1U;
				is_float_or_double = TRUE;   /* Default value */

				SH4_CFG_DEBUG(4) SH4_DEBUG("constant float value 0x%08x\n",value1);
			break;

			case cstpool_type_double:
				value1 = sh4_get_32low_from_double(g);
				value2 = sh4_get_32high_from_double(g);
				sh4_emit32(pcval,value1);    /* constant pool allocation - sz <= 6 + n*4*/
				sh4_emit32(pcval,value2);    /* constant pool allocation - sz <= 6 + n*4*/
				nb_int_const = 2U;
				is_float_or_double = TRUE;

				SH4_CFG_DEBUG(4) SH4_DEBUG("constant double value 0x%08x%08x\n",
							   value2,value1);
			break;

			default:			/* Should never happen */
				g_assert(FALSE);
				nb_int_const = 0U;	/* Prevent compilation warnings */
			break;
			}
		}
	/* sz <= 6 + n*4*/

	/* patch sh4_movl_dispPC instruction emitted previously. */
		patch0 = cfg->native_code + cur_pool->off_inst[index];
		if(is_float_or_double) {
			sh4_mova_PCrel_R0(NO_recur,&patch0,dest);
		} else {
			sh4_movl_PCrel(NO_recur,&patch0, dest, cur_pool->reg[index]);
		}

	/* A double requires two inputs in tables (second entry is a
	 * dummy one). We also have sequences where more than two consecutives
	 * calls in emit_basic_block are emitted.
	 */
		cur_pool->pool_nbcst_emitted += nb_int_const;

	}  /* End while */

	/* patch instruction at patch1 */
	sh4_bra_label(NO_recur,&patch1, *pcval);

	if(cur_pool->pool_nbcst_emitted > SH4_MAX_CSTPOOL -2U) {
		cur_pool->state = cstpool_allocated;
	}

	return;
}

void
sh4_emit_pool_fullperf(MonoCompile *cfg, gboolean end_bb, guint8 **pcval)
{
	NOT_IMPLEMENTED;
	return;
}

void
sh4_emit_pool(MonoCompile *cfg, gboolean end_bb, guint8 **pcval)
{
	MonoSH4CstPool_Env *env = (MonoSH4CstPool_Env*)(cfg->arch.poolenv);

	if(env->mode == cstpool_mode_fullperf) {
		sh4_emit_pool_fullperf(cfg, end_bb, pcval);
	} else {
		sh4_emit_pool_lowperf(cfg, end_bb, pcval);
	}
	return;
}

/**
 * Decide whether a constant pool is to be emitted
 * Param. bb(MonoBasicBlock) is meaningfull only if flag end_bb is set.
 *
 * There are cases where basic block emission is mandatory
 * For instance, if constant pool is full or if pc range
 * between the first constant carrying instruction and the
 * current pc is big enough, we have no choice but to emit
 * the constant pool. In other cases, we use some heuristics
 */
gboolean
sh4_cstpool_decide_emission(MonoCompile *cfg,gboolean end_bb,
			MonoBasicBlock *bb, guint32 *size)
{
	MonoSH4CstPool_Env *env = (MonoSH4CstPool_Env*)(cfg->arch.poolenv);
	MonoSH4CstPool *cur_pool = env->last;

	*size = 0U;			/* Default value */

	/* No pool, no constant pool or pool already emitted */
	if(cur_pool == NULL              ||
	   cur_pool->pool_nbcst == 0     ||
	   cur_pool->state != cstpool_filling)
		return FALSE;

	/* Low performance mode. If constants are to be emitted,*/
	/* do the job and exit early.				*/
	if(env->mode == cstpool_mode_lowperf) {
		gint32 pool_nbcst;

		pool_nbcst = cur_pool->pool_nbcst - cur_pool->pool_nbcst_emitted;
		if(pool_nbcst > 0) {
			/* See function sh4_emit_pool_lowperf */
			*size = 6U + (pool_nbcst * 4U);
			return TRUE;
		}
		return FALSE;
	}

	if(cur_pool->pool_nbcst > SH4_MAX_CSTPOOL - 2U) {
		NOT_IMPLEMENTED;
		return TRUE;
	}

	return FALSE;
}

/* API routine for float constants */
void sh4_cstpool_addf(MonoCompile *cfg, guint8 **pcval, float f)
{
	MonoSH4CstPool_Env   *env = cfg->arch.poolenv;
	CstPool_Const_Values *g;

	g_assert(env!=NULL);
	g = sh4_build_cst_float(env->mempool, f);

	sh4_cstpool_add_internal(cfg, pcval, MONO_PATCH_INFO_NONE,
				 NULL,sh4_r0,g);
	return;
}

/* API routine for double constants */
void sh4_cstpool_addd(MonoCompile *cfg, guint8 **pcval, double d)
{
	MonoSH4CstPool_Env   *env = cfg->arch.poolenv;
	CstPool_Const_Values *g;

	g_assert(env!=NULL);
	g = sh4_build_cst_double(env->mempool, d);

	sh4_cstpool_add_internal(cfg, pcval, MONO_PATCH_INFO_NONE,
				 NULL, sh4_r0, g);

	return;
}

void sh4_cstpool_add(MonoCompile *cfg, guint8 **pcval, MonoJumpInfoType type, gconstpointer target, SH4IntRegister reg)
{
	MonoSH4CstPool_Env 	*env = cfg->arch.poolenv;
	CstPool_Const_Values 	*g;       /* Default value */
	guint32			 imm_value;

	g_assert(env!=NULL);

	if(type == MONO_PATCH_INFO_NONE) {
		imm_value = *(guint32*)target;

		/* A special case: constant is tiny enough to be encoded */
		/* directly. If so, generate a mov instruction and exit. */
		if(SH4_CHECK_RANGE_mov_imm(imm_value)) {
			sh4_mov_imm(cfg,pcval,imm_value,reg);
			return;
		}

		g = sh4_build_cst_int(env->mempool,imm_value);
	} else {
		g = NULL;
	}

	sh4_cstpool_add_internal(cfg, pcval, type, target, reg, g); 
	return;
}

static void sh4_cstpool_add_internal(MonoCompile *cfg, guint8 **pcval,
				     MonoJumpInfoType type, gconstpointer target,
				     guint32  reg, CstPool_Const_Values *g)
{
	MonoSH4CstPool_Env *env = cfg->arch.poolenv;
	MonoSH4CstPool     *tmp;
	guint32             offset;
	guint32             index;
	guint32             nb_int_const;  /* 8 bytes ~= 2 int constants for a double */ 

	offset = *pcval - cfg->native_code;

	/* Add an other pool of constants if necessary*/
	if(env->last==NULL ||
	   env->last->state!=cstpool_filling) {
		sh4_cstpool_addpool(cfg);
	}
	tmp = env->last;

	/* Fill in data */
	index  = tmp->pool_nbcst;
	if(index==0U) {
		tmp->pool_off0_start = offset & ~0x3U;
	}

	/* For actual constants, we refer to the CstPool_Const_Values.
	 * Since a double takes the place of two integers, we fill in
	 * two entries in tables for a single double constant. This makes
	 * it possible to maintain a linear relation between the number
	 * of constants and the size of the memory pool.
	 */
	nb_int_const = 1U;    /* Default value */
	if (type == MONO_PATCH_INFO_NONE) {
		g_assert(g!=NULL);
		tmp->pool_cst[index] = (gpointer)g;
		if(sh4_cst_is_double(g)) {
			nb_int_const = 2U;
		}
	} else {
		g_assert(target != NULL);
		tmp->pool_cst[index] = target;
	}

	tmp->type    [index] = (gint16)type;
	tmp->off_inst[index] = offset;
	tmp->reg     [index] = (guint16)reg;

	if(nb_int_const==2U) {   /* Double: add a dummy entry */
		tmp->pool_cst[index+1U] = (gpointer)g;
		tmp->type    [index+1U] = (gint16)type;
		tmp->off_inst[index+1U] = offset;
		tmp->reg     [index+1U] = (guint16)reg;
	}

	tmp->pool_nbcst += nb_int_const;
	env->nbcst      += nb_int_const;

	/* Print a dummy instruction in code buffer. We know that
	 * emission of the current constant pool might be delayed.
	 * Note that we don't care if the real instruction is a
	 * movl_disPC or a mova instruction (for float and double): 
	 * in both cases, we assume that current buffer is big enough
         * to make it possible to allocate 2 more bytes.
	 */
	sh4_movl_dispPC(NO_recur,pcval, 0, reg);   /* 0 to be patched       */
	return;
}

/**
 * WARNING: any change in the prototype of this routine
 * has to be reproduced in header file sh4-codegen.h and thus in
 * gbu2mono!!!!!!!
 */
void sh4_cstpool_check(void *cfg, guint8 **pcval)
{
	/* Unsafe cast, but we have to avoid circular dependencies in
	 * header files.
	 */
	return;
}

void sh4_cstpool_check_begin_bb(MonoCompile *cfg, MonoBasicBlock *bb, guint8 **pcval)
{
	MonoSH4CstPool_Env *env = cfg->arch.poolenv;
	guint32             bb_id = bb->block_num;
	guint32             offset;

	g_assert(env!=NULL);

	if(env->nb_bblocks!=0 && bb_id<env->nb_bblocks) {
		offset = *pcval - cfg->native_code;

		g_assert(sh4_is_valid_offset(offset));

		env->tab_bb_offset[bb_id] = offset;
	}

	return;
}

/* Called at the end of a basic block */
void sh4_cstpool_check_end_bb(MonoCompile *cfg, MonoBasicBlock *bb)
{
	guint32 size;
	guint8 *buffer;

	if(sh4_cstpool_decide_emission(cfg,TRUE,bb,&size)) {
		buffer = get_code_buffer(cfg,size);
		sh4_emit_pool(cfg, TRUE /* end of bb */, &buffer);
		cfg->code_len = buffer - cfg->native_code;
	}
	return;
}

/* Return the offset of a bb if known */
gboolean
sh4_cstpool_get_bb_address(MonoCompile *cfg, MonoBasicBlock *bb, guint32 *offset)
{
	MonoSH4CstPool_Env *env = cfg->arch.poolenv;
	guint32             bb_id = bb->block_num;
	gboolean            ret = FALSE;    /* Pessimistic by default */

	g_assert(env!=NULL);

	if(env->nb_bblocks!=0 && bb_id<env->nb_bblocks) {
		guint32 rel_add = env->tab_bb_offset[bb_id];

		if(sh4_is_valid_offset(rel_add)) {
			ret    = TRUE;
			*offset = rel_add;
		}
	}

	if(ret==FALSE) {
		*offset = 0xFFFFFFFF;
	}

	return ret;
}
