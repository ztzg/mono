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


void sh4_cstpool_init(MonoCompile *cfg);
void sh4_cstpool_end(MonoCompile *cfg);
void sh4_cstpool_add(MonoCompile *cfg, guint8 **pcval,
                     MonoJumpInfoType type, gconstpointer target,
                     guint32 reg);
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
