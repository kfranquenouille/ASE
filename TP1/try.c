#include "try.h"

static int val;

/*
 * Fonction try qui permet de simuler un setjump
 */
int try(struct ctx_s *p_ctx, func_t *f, int arg)
{
	p_ctx->ctx_magic_number = MAGIC_NUMBER;

	asm("movl %%esp, %0\n\t" "movl %%ebp, %1"
		:"=r"(p_ctx->ctx_esp), "=r"(p_ctx->ctx_ebp));
	
	return f(arg);
}

/*
 * Fonction throw qui permet de simuler un longjump
 */
int throw(struct ctx_s *p_ctx, int r)
{
	assert(p_ctx->ctx_magic_number == MAGIC_NUMBER);
	val = r;

	asm("movl %0, %%esp\n\t" "movl %1, %%ebp"
		::"r"(p_ctx->ctx_esp), "r"(p_ctx->ctx_ebp));
	
	return val;
}

