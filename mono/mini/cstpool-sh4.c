/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
 * cstpool-sh4.c: SH4 backend for the Mono code generator
 *
 * Authors:
 *   Yves   JANIN   (yves.janin@st.com)
 *
 * (C) 2008 STMicroelectronics.
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
 * SH4 features constant pools as a way to load immediates into registers.
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
 ...|............
 *  |      0x10             // Beginning of the constant pool
 ...|............
 *  |      0x18 0xdeadbeef  // Value of the loaded constant
 ...|............
 *  -----> 0x30             // Beginning of the next basic block (code).
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
 * void sh4_cstpool_init(MonoCompile *cfg).
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
 * void sh4_cstpool_add(MonoCompile     *cfg,
 *                      guint8          *pcval,    // PC current value
 *                      MonoJumpInfoType type,
 *                      gconstpointer    target,
 *                      guint32          reg)
 *
 *     Generate an instruction that load a 32 bit datum into
 *     a register whose index is given. Update internal representation
 *     and store all necessary information to do further patches.
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
 * void sh4_cstpool_end(MonoCompile *cfg)
 *
 *     Final call. To be called after the last instruction of the
 *     last basic block. Flush the last constant pool if necessary.
 *     Do some patches. Free memory associated to the process...
 *
 * III Algorithms
 * ==============
 *
 * We have devised a simple sub-optimal two pass algorithm.
 * The first pass performs most of the job, while the second
 * one is reserved for patching data pools with labels values.
 * For this second pass, we heavily rely on the Mono patch mechanism.
 *
 * Remark: Since native code buffer can change, it is necessary to store
 *         address pool information (see MonoSH4CstPool) as OFFSETS.
 */

/* Prototype declarations */
static inline void*    sh4_cstpool_malloc(MonoMemPool *pool, guint32 size);
static inline gboolean sh4_is_valid_offset(guint32 offset);

static void         sh4_cstpool_decide_emission(MonoCompile*, gboolean, 
                                               MonoBasicBlock*, guint8 **pcval);
static inline void  sh4_cstpool_addpool(MonoCompile*);
static void         sh4_emit_current_pool(MonoCompile*, MonoSH4CstPool*,
                                          gboolean, guint8 **pcval);

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

/**
 * Adding a constant pool in the environement
 */
static inline void
sh4_cstpool_addpool(MonoCompile *cfg)
{
   MonoSH4CstPool_Env *env = cfg->arch.poolenv;
   MonoSH4CstPool     *tmp;

   tmp = sh4_cstpool_malloc(env->mempool,sizeof(MonoSH4CstPool));

   SH4_DEBUG("args => %p -- Create pool %d", cfg, env->nbpool);

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
 * Overall initialization
 */
void
sh4_cstpool_init(MonoCompile *cfg)
{
 
   MonoSH4CstPool_Env *env ;
   MonoMemPool        *mempool;
   int i;

   SH4_DEBUG("args => %p", cfg);

   mempool = mono_mempool_new();

   /* Deal with constant pools.
    */
   cfg->arch.poolenv = sh4_cstpool_malloc(mempool,sizeof(MonoSH4CstPool_Env));
   env = cfg->arch.poolenv;
   env->mempool = mempool;

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
void
sh4_cstpool_end(MonoCompile *cfg)
{
   MonoSH4CstPool_Env *env = cfg->arch.poolenv;

   SH4_DEBUG("args => %p", cfg);

   /* (parts of) this code will be removed when memory pools are used */
   if(env!=NULL) {
     SH4_DEBUG("nb of cst pools %d, overall nb of cst %d",
                env->nbpool, env->nbcst);

     mono_mempool_destroy(env->mempool);
     cfg->arch.poolenv = NULL;
   }

   return;
}

static void
sh4_emit_current_pool(MonoCompile *cfg, MonoSH4CstPool *current,
                      gboolean end_bb, guint8 **pcval)
{

   MonoSH4CstPool *cur_pool = cfg->arch.poolenv->last;

   guint32 index;        /* Index of current item in data pool */
   
/* This piece of code is very temporary. Adapted from Denis' work......*/
   guint8   *patch0;
   guint8   *patch1;
   guint8   *dest;
   guint32   offset;
   guint32   length_max;
   guint32   value;

   index  = cur_pool->pool_nbcst_emitted;
   SH4_DEBUG("emit constant nr %d (cfg %p)",index,cfg);

   /* Evaluate if we have enough space. If not, reassign a buffer.   */
   /* Following sequence generates at most 10 bytes.                 */
   offset    = *pcval - cfg->native_code;
   length_max = offset + 10U;
   sh4_realloc_buf_if_needed(cfg,offset,length_max,pcval);

   patch1 = *pcval;

   sh4_bra(NO_recur,pcval, 0x0); /* 0x0 to be patched   */
   sh4_nop(NO_recur,pcval);      /* delay slot          */

   if(((guint32)*pcval & 0x3)) {
     sh4_nop(NO_recur,pcval);    /* Align constant pool */
   }

   /* constant pool is here. */
   dest = *pcval;

   if(cur_pool->type[index] != MONO_PATCH_INFO_NONE) {
     mono_add_patch_info(cfg,
                        *pcval - cfg->native_code,
                         cur_pool->type[index],
                         cur_pool->pool_cst[index]);
     value = 0U; /* meaningless value */
   } else {
     value = *(guint32*)(cur_pool->pool_cst[index]);
     SH4_DEBUG("constant value 0x%08x\n",value);
   }

   sh4_emit32(pcval,value);  /* constant pool allocation */

   /* patch sh4_movl_dispPC instruction emitted previously. */
   patch0 = cfg->native_code + cur_pool->off_inst[index];
   sh4_movl_PCrel(NO_recur,&patch0, dest, cur_pool->reg[index]);

   /* patch instruction at patch1 */
   sh4_bra_label(NO_recur,&patch1, *pcval);

   (cur_pool->pool_nbcst_emitted)++;
   if(index == SH4_MAX_CSTPOOL) {
     current->state = cstpool_allocated;
   }
/* End of temporary code ...........................................*/

   return;
}

/**
 * Decide whether a constant pool is to be emitted
 * bb(MonoBasicBlock) param. is meaningfull only if end_bb flag is set.
 *
 * There are cases where basic block emission is mandatory
 * For instance, if constant pool is full or if pc range
 * between the first constant carrying instruction and the
 * current pc is big enough, we have no choice but to emit
 * the constant pool. In other cases, we use some heuristics
 */
static void
sh4_cstpool_decide_emission(MonoCompile *cfg,gboolean end_bb,
                            MonoBasicBlock *bb, guint8 **pcval)
{
   MonoSH4CstPool *cur_pool = cfg->arch.poolenv->last;

   /* No pool, no constant pool or pool already emitted */
   if(cur_pool == NULL              || 
      cur_pool->pool_nbcst == 0     ||
      cur_pool->state != cstpool_filling)
        return;

   /* Temporary */
   if(cur_pool->pool_nbcst_emitted==cur_pool->pool_nbcst)
        return;


   /* TODO */
#if 0
     if(cur_pool->pool_nbcst == SH4_MAX_CSTPOOL)
#endif
       sh4_emit_current_pool(cfg, cur_pool,end_bb,pcval);

   return;
}

void
sh4_cstpool_add(MonoCompile *cfg, guint8 **pcval,
                MonoJumpInfoType type, gconstpointer target,
                guint32  reg)
{
   MonoSH4CstPool_Env *env = cfg->arch.poolenv;
   MonoSH4CstPool     *tmp;
   guint32             offset;
   guint32             index;

   offset = *pcval - cfg->native_code;

   /* Add an other pool of constants if necessary*/
   g_assert(env!=NULL);
   if(env->last==NULL ||
      env->last->state!=cstpool_filling) {
        sh4_cstpool_addpool(cfg);
      }
   tmp = env->last;

   /* Fill in data */
   index  = tmp->pool_nbcst;
   tmp->pool_cst[index] = target;
   tmp->type    [index] = (gint16)type;
   tmp->off_inst[index] = offset;
   tmp->reg     [index] = (guint16)reg;
   if(tmp->pool_nbcst==0U) {
    tmp->pool_off0_start = offset & ~0x3U;
   }
   ++(tmp->pool_nbcst);
   ++(env->nbcst);

   /* Check that we have enough space in code buffer */
   sh4_realloc_buf_if_needed(cfg,offset, offset+2U,pcval);

   /* Allocate space in code buffer. We know that emission of the
    * current constant pool might be delayed.
    */
   sh4_movl_dispPC(NO_recur,pcval, 0, reg);   /* 0 to be patched       */

   sh4_cstpool_decide_emission(cfg,
                               FALSE /* not at the end of a bb */,
                               NULL,
                               pcval);

   return;
}

/**
 * WARNING: any change in the prototype of this routine
 * has to be reproduced in header file sh4-codegen.h and thus in
 * gbu2mono!!!!!!!
 */
void
sh4_cstpool_check(void *cfg, guint8 **pcval)
{
   /* Unsafe cast, but we have to avoid circular dependencies in
    * header files.
    */
   MonoCompile *cfg2 = (MonoCompile*) cfg;

   sh4_cstpool_decide_emission(cfg2,
                               FALSE /* not at the end of a bb */,
                               NULL,
                               pcval);
   return;
}

void
sh4_cstpool_check_begin_bb(MonoCompile *cfg, MonoBasicBlock *bb,
                           guint8 **pcval)
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

void
sh4_cstpool_check_end_bb(MonoCompile *cfg, MonoBasicBlock *bb,
                         guint8 **pcval)
{
   sh4_cstpool_decide_emission(cfg,
                               TRUE, /* not at the end of a bb */
                               bb,
                               pcval);
   return;
}

/**
 *   Realloc a buffer if we haven't got enough size. Otherwise
 *   do nothing.
 *
 *   offset     (guint32)  current offset in the native code buffer
 *   length_max (guint32)  length max to be allocated.
 *   pcval      (**guint8) current pointer in the buffer.
 *
 *   Returns TRUE if a buffer has been allocated, and FALSE otherwise.
 *
 *   Note: this routine is not strictly related to constant pools.
 *   It is also called by mono_arch_output_basic_block.
 */
void
sh4_realloc_buf_if_needed(MonoCompile *cfg, guint32 offset,
                          guint32 length_max, guint8 **pcval)
{
#ifndef NDEBUG
   guint8 *oldpcval = *pcval;
#endif

   g_assert(offset<=length_max);

   if(length_max <= cfg->code_size)
      return;

   /* Get the correct size */
   while (length_max > cfg->code_size) {
          cfg->code_size *= 2U;
   }

   cfg->native_code = g_realloc(cfg->native_code, 
                                cfg->code_size);
  *pcval = cfg->native_code + offset;

   SH4_DEBUG("buffer reallocation pcval 0x%08x -> 0x%08x",
             (guint32)oldpcval,(guint32)pcval);
   SH4_DEBUG("current offset %d, required offset %d, new buffer size %d",
              offset, length_max, cfg->code_size);

   mono_jit_stats.code_reallocs++;
   return;
}

/* Return the offset of a bb if known */
gboolean
sh4_cstpool_get_bb_address(MonoCompile *cfg, MonoBasicBlock *bb,
                           guint32 *offset)
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

