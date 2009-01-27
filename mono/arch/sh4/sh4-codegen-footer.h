/* -*- c-set-style: "K&R"; c-basic-offset: 8 -*-
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
 */

#ifndef __MONO_SH4_CODEGEN_FOOTER_H__
#define __MONO_SH4_CODEGEN_FOOTER_H__

static inline void sh4_load(guint8 **code, guint32 value, SH4IntRegister Rx)
{
	guint8 *patch0 = NULL;
	guint8 *patch1 = NULL;

	/* Patch slot for : Rx <- value */
	patch0 = *code;
	sh4_die(NULL, code);

	/* Patch slot for : bra_label "skip_cstpool" */
	patch1 = *code;
	sh4_die(NULL, code);
	sh4_nop(NULL, code);

	/* Align the constant pool. */
	while (((guint32)*code % 4) != 0)
		sh4_nop(NULL, code);

	/* Build the constant pool & patch the corresponding instructions. */
	sh4_movl_PCrel(NULL, &patch0, *code, Rx);
	sh4_emit32(code, (guint32)value);

	sh4_bra_label(NULL, &patch1, *code);
}

/**
 * Checks if a code sequence is a constant-pool load and returns its length.
 *
 * #if strong_align
 *     -8: mov.l @cstpool, rX
 *     -6: bra   cstpool_end
 * #else
 *    -10: mov.l @cstpool, rX
 *     -8: bra   cstpool_end
 *     -6: nop
 * #endif
 *     -4: nop
 * cstpool:
 *     -2: .word 0xXXXX
 *     -0: .word 0xYYYY <- code points here
 */
static inline int is_sh4_load(guint16 *code, SH4IntRegister sh4_rX)
{
	if (!is_sh4_nop(code[-2]))
		return 0;

	if (is_sh4_bra(code[-3], 4) &&
	    is_sh4_movl_dispPC(code[-4], 4, sh4_rX))
		return 5;

	if (is_sh4_nop(code[-3])    &&
	    is_sh4_bra(code[-4], 6) &&
	    is_sh4_movl_dispPC(code[-5], 4, sh4_rX))
		return 6;

	return 0;
}

#endif /* __MONO_SH4_CODEGEN_FOOTER_H__ */
