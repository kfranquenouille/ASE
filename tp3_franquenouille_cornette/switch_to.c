#include "switch_to.h"
#include "hw.h"

static struct ctx_s *ctx_courant = NULL;
static struct ctx_s *ctxs = NULL;
struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void f_ping(void *arg);
void f_pong(void *arg);	

/* Initialise un contexte */
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

/* Permet de changer de contexte */
void switch_to_ctx(struct ctx_s *ctx)
{
	assert((ctx->ctx_magic_number == MAGIC_NUMBER) && (ctx->ctx_etat != CTX_TERMINATED));

	irq_disable();
	/* On sauvegarde %%esp et %%ebp */
	if (ctx_courant != NULL)
	{
		asm("movl %%esp, %0\n\t" "movl %%ebp, %1"
		:"=r"(ctx_courant->ctx_esp), "=r"(ctx_courant->ctx_ebp));

	}

	ctx_courant=ctx;

	irq_enable();
	
	/* On restaure %%esp et %%ebp */
	asm("movl %0, %%esp\n\t" "movl %1, %%ebp"
		::"r"(ctx_courant->ctx_esp), "r"(ctx_courant->ctx_ebp));

	if (ctx_courant->ctx_etat == CTX_INIT)
	{
		ctx_courant->ctx_etat = CTX_ACTIVABLE;
		ctx_courant->ctx_func (ctx_courant->ctx_arg);
		ctx_courant->ctx_etat = CTX_TERMINATED;
		yield();
	} 
	else
	{
		return;
	}
}

/* Creation du contexte et ajout dans la liste des contexte */
int create_ctx(int stack_size, func_t f, void *args)
{
	struct ctx_s* alloc;
	alloc = (struct ctx_s*)malloc(sizeof(struct ctx_s));
	init_ctx(alloc, stack_size, f, args);
	if (ctxs)
	{
		alloc->ctx_next = ctxs->ctx_next;
		ctxs->ctx_next = alloc;
	}
	else 
	{
		ctxs = alloc;
		ctxs->ctx_next = alloc;
	}
	return 1;
}

/* Met a jour la liste des contextes et passe au contexte suivant */
void yield()
{
	if (ctx_courant)
	{
		struct ctx_s *tmp;
		while ((ctx_courant->ctx_next->ctx_etat == CTX_TERMINATED) && (ctx_courant->ctx_next != ctx_courant))
		{
			tmp = ctx_courant->ctx_next;
			ctx_courant->ctx_next = ctx_courant->ctx_next->ctx_next;
			free(tmp->ctx_stack);
			free(tmp);
		}
		if (ctx_courant->ctx_next->ctx_etat != CTX_TERMINATED)
		{
			switch_to_ctx(ctx_courant->ctx_next);
		}
		else
		{
			exit(0);
		}
	}
	else
	{
		assert(ctxs);
		switch_to_ctx(ctxs);
	}
}

void start_sched()
{
	setup_irq(TIMER_IRQ,yield);
	start_hw();
	yield();
}

int main(int argc, char *argv[]) 
{
	create_ctx(16384, f_ping, NULL);
	create_ctx(16384, f_pong, NULL);
	start_sched();

	exit(EXIT_SUCCESS);
}

void f_ping(void *args) {
	while(1) {
		printf("A");
		printf("B");
		printf("C");
	}
}

void f_pong(void *args) {
	while(1) {
		printf("1");
		printf("2");
	}
}