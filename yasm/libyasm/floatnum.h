/* $IdPath$
 * Floating point number functions header file.
 *
 *  Copyright (C) 2001  Peter Johnson
 *
 *  Based on public-domain x86 assembly code by Randall Hyde (8/28/91).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND OTHER CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR OTHER CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef YASM_FLOATNUM_H
#define YASM_FLOATNUM_H

void yasm_floatnum_initialize(void);
/* Clean up internal allocations */
void yasm_floatnum_cleanup(void);

/*@only@*/ yasm_floatnum *yasm_floatnum_new(const char *str);
/*@only@*/ yasm_floatnum *yasm_floatnum_copy(const yasm_floatnum *flt);
void yasm_floatnum_delete(/*@only@*/ yasm_floatnum *flt);

/* calculation function: acc = acc op operand */
void yasm_floatnum_calc(yasm_floatnum *acc, yasm_expr_op op,
			yasm_floatnum *operand, unsigned long lindex);

/* The get functions return nonzero if flt can't fit into that size format:
 * -1 if underflow occurred, 1 if overflow occurred.
 */

/* Essentially a convert to single-precision and return as 32-bit value.
 * The 32-bit value is a "standard" C value (eg, of unknown endian).
 */
int yasm_floatnum_get_int(const yasm_floatnum *flt,
			  /*@out@*/ unsigned long *ret_val);

/* ptr will point to the Intel-format little-endian byte string after a
 * successful call (eg, [0] should be the first byte output to the file).
 */
int yasm_floatnum_get_sized(const yasm_floatnum *flt,
			    /*@out@*/ unsigned char *ptr, size_t size);

/* Basic check to see if size is even valid for flt conversion (doesn't
 * actually check for underflow/overflow but rather checks for size=4,8,10).
 * Returns 1 if valid, 0 if not.
 */
int yasm_floatnum_check_size(const yasm_floatnum *flt, size_t size);

void yasm_floatnum_print(FILE *f, const yasm_floatnum *flt);

#endif