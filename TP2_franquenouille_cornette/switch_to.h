#ifndef SWITCHTO
#define SWITCHTO

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAGIC_NUMBER 0xABBACACA

typedef void func_t (void*);

enum etat_e {CTX_TERMINATED, CTX_ACTIVABLE, CTX_INIT};

struct ctx_s {
	void* ctx_esp;
	void* ctx_ebp;
	int ctx_magic_number;
	void* ctx_arg;
	func_t* ctx_func;
	void* ctx_stack;
	enum etat_e ctx_etat;
};

void switch_to_ctx(struct ctx_s *ctx);
int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *args);

#endif
