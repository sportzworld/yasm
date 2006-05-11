/*
 * ORG bytecode
 *
 *  Copyright (C) 2005-2006  Peter Johnson
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
#define YASM_LIB_INTERNAL
#include "util.h"
/*@unused@*/ RCSID("$Id$");

#include "coretype.h"
#include "file.h"

#include "errwarn.h"
#include "intnum.h"
#include "expr.h"
#include "value.h"

#include "bytecode.h"

#include "bc-int.h"


typedef struct bytecode_org {
    unsigned long start;	/* target starting offset within section */
} bytecode_org;

static void bc_org_destroy(void *contents);
static void bc_org_print(const void *contents, FILE *f, int indent_level);
static void bc_org_finalize(yasm_bytecode *bc, yasm_bytecode *prev_bc);
static yasm_bc_resolve_flags bc_org_resolve
    (yasm_bytecode *bc, int save, yasm_calc_bc_dist_func calc_bc_dist);
static int bc_org_tobytes(yasm_bytecode *bc, unsigned char **bufp, void *d,
			  yasm_output_value_func output_value,
			  /*@null@*/ yasm_output_reloc_func output_reloc);

static const yasm_bytecode_callback bc_org_callback = {
    bc_org_destroy,
    bc_org_print,
    bc_org_finalize,
    bc_org_resolve,
    bc_org_tobytes,
    0
};


static void
bc_org_destroy(void *contents)
{
    yasm_xfree(contents);
}

static void
bc_org_print(const void *contents, FILE *f, int indent_level)
{
    const bytecode_org *org = (const bytecode_org *)contents;
    fprintf(f, "%*s_Org_\n", indent_level, "");
    fprintf(f, "%*sStart=%lu\n", indent_level, "", org->start);
}

static void
bc_org_finalize(yasm_bytecode *bc, yasm_bytecode *prev_bc)
{
}

static yasm_bc_resolve_flags
bc_org_resolve(yasm_bytecode *bc, int save,
	       yasm_calc_bc_dist_func calc_bc_dist)
{
    bytecode_org *org = (bytecode_org *)bc->contents;

    /* Check for overrun */
    if (bc->offset > org->start) {
	yasm_error_set(YASM_ERROR_GENERAL,
		       N_("ORG overlap with already existing data"));
	return YASM_BC_RESOLVE_ERROR | YASM_BC_RESOLVE_UNKNOWN_LEN;
    }

    /* Generate space to start offset */
    bc->len = org->start - bc->offset;
    return YASM_BC_RESOLVE_MIN_LEN;
}

static int
bc_org_tobytes(yasm_bytecode *bc, unsigned char **bufp, void *d,
	       yasm_output_value_func output_value,
	       /*@unused@*/ yasm_output_reloc_func output_reloc)
{
    bytecode_org *org = (bytecode_org *)bc->contents;
    unsigned long len, i;

    /* Sanity check for overrun */
    if (bc->offset > org->start) {
	yasm_error_set(YASM_ERROR_GENERAL,
		       N_("ORG overlap with already existing data"));
	return 1;
    }
    len = org->start - bc->offset;
    for (i=0; i<len; i++)
	YASM_WRITE_8(*bufp, 0);
    return 0;
}

yasm_bytecode *
yasm_bc_create_org(unsigned long start, unsigned long line)
{
    bytecode_org *org = yasm_xmalloc(sizeof(bytecode_org));

    org->start = start;

    return yasm_bc_create_common(&bc_org_callback, org, line);
}