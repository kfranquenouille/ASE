#include "switch_to.h"

static struct ctx_s *ctx_courant = NULL;
struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void f_ping(void *arg);
void f_pong(void *arg);

int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *args)
{
	assert((ctx->ctx_stack=malloc(stack_size)));
	ctx->ctx_func=f;
	ctx->ctx_arg=args;
	ctx->ctx_etat=CTX_INIT;
	ctx->ctx_magic_number=MAGIC_NUMBER;
	ctx->ctx_ebp=(char*)ctx->ctx_stack+stack_size-sizeof(void*);
	ctx->ctx_esp=(char*)ctx->ctx_stack+stack_size-sizeof(void*);
	return 0;
}

void switch_to_ctx(struct ctx_s *ctx)
{
	assert((ctx->ctx_magic_number == MAGIC_NUMBER) && (ctx->ctx_etat != CTX_TERMINATED));

	/* On sauvegarde %%esp et %%ebp */
	if (ctx_courant != NULL)
	{
		asm("movl %%esp, %0\n\t" "movl %%ebp, %1"
		:"=r"(ctx_courant->ctx_esp), "=r"(ctx_courant->ctx_ebp));

	}

	ctx_courant=ctx;

	/* On restaure %%esp et %%ebp */
	asm("movl %0, %%esp\n\t" "movl %1, %%ebp"
		::"r"(ctx_courant->ctx_esp), "r"(ctx_courant->ctx_ebp));

	if (ctx_courant->ctx_etat == CTX_INIT)
	{
		ctx_courant->ctx_etat = CTX_ACTIVABLE;
		ctx_courant->ctx_func (ctx_courant->ctx_arg);
		ctx_courant->ctx_etat = CTX_TERMINATED;
		while(1);
	} 
	else
	{
		return;
	}
}

int main(int argc, char *argv[]) {

	init_ctx(&ctx_ping, 16384, f_ping, NULL);
	init_ctx(&ctx_pong, 16384, f_pong, NULL);
	switch_to_ctx(&ctx_ping);

	exit(EXIT_SUCCESS);
}

void f_ping(void *args) {
	while(1) {
		printf("A");
		switch_to_ctx(&ctx_pong);
		printf("B");
		switch_to_ctx(&ctx_pong);
		printf("C");
		switch_to_ctx(&ctx_pong);
	}
}

void f_pong(void *args) {
	while(1) {
		printf("1");
		switch_to_ctx(&ctx_ping);
		printf("2");
		switch_to_ctx(&ctx_ping);
	}
}