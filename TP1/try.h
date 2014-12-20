#ifndef TRY
#define TRY

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAGIC_NUMBER 0xABBADADA

typedef int (func_t) (int);

struct ctx_s {
	void* ctx_esp;
	void* ctx_ebp;
	int ctx_magic_number;
};

int try(struct ctx_s *p_ctx, func_t *f, int arg);
int throw(struct ctx_s *p_ctx, int r);

#endif
