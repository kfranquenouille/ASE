#include "sem.h"
#include "hw.h"

static struct ctx_s *ctx_courant = NULL;
static struct ctx_s *ctxs = NULL;
struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

struct sem_s sem, vide, plein;

void f_ping(void *arg);
void f_pong(void *arg);	


/* ====================== Contexte ====================== */

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
	ctx->ctx_sem_next=NULL;
	return 0;
}


/* Permet de changer de contexte */
void switch_to_ctx(struct ctx_s *ctx)
{
	assert(ctx->ctx_magic_number == MAGIC_NUMBER); 
	assert(ctx->ctx_etat != CTX_TERMINATED); 
	assert(ctx->ctx_etat != CTX_BLOCKED);

	irq_disable();
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
		irq_enable();
		ctx_courant->ctx_func (ctx_courant->ctx_arg);
		ctx_courant->ctx_etat = CTX_TERMINATED;
		yield();
	} 
	else
	{
		irq_enable();
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


/* ====================== Liste de contexte + yield ====================== */

/* Met a jour la liste des contextes et passe au contexte suivant */
void yield()
{
	struct ctx_s *courant = ctx_courant;
	irq_disable();
	if (ctx_courant)
	{
		struct ctx_s *tmp;
		while (((ctx_courant->ctx_next->ctx_etat == CTX_TERMINATED) || (ctx_courant->ctx_next->ctx_etat == CTX_BLOCKED)) && (ctx_courant->ctx_next != courant))
		{
			if (ctx_courant->ctx_next->ctx_etat == CTX_TERMINATED)
			{
				tmp = ctx_courant->ctx_next;
				ctx_courant->ctx_next = ctx_courant->ctx_next->ctx_next;
				free(tmp->ctx_stack);
				free(tmp);
			}

			if (ctx_courant->ctx_next->ctx_etat == CTX_BLOCKED)
			{
				ctx_courant=ctx_courant->ctx_next;
				printf("I'm blocked\n");
			}
		}
		if ((ctx_courant->ctx_next->ctx_etat != CTX_TERMINATED) && (ctx_courant->ctx_next->ctx_etat != CTX_BLOCKED))
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


/* ====================== Semaphore ====================== */

/* Initialisation du semaphore */
void sem_init(struct sem_s *sem, unsigned int val)
{
	sem->sem_magic_number=SEM_MAGIC_NUMBER;
	sem->sem_compteur=val;
	sem->sem_ctx_blocked=NULL;
}

/* Fonction wait des semaphores */
void sem_down(struct sem_s *sem)
{
	irq_disable();
	assert(sem->sem_magic_number==SEM_MAGIC_NUMBER);
	sem->sem_compteur = sem->sem_compteur - 1;
	if (sem->sem_compteur < 0)
	{
		ctx_courant->ctx_etat = CTX_BLOCKED;
		ctx_courant->ctx_sem_next = sem->sem_ctx_blocked;
		sem->sem_ctx_blocked = ctx_courant;
		irq_enable();
		yield();
	}
	irq_enable();
}

/* Fonction signal des semaphores */
void sem_up(struct sem_s *sem)
{
	struct ctx_s *ctx = NULL;
	irq_disable();
	assert(sem->sem_magic_number==SEM_MAGIC_NUMBER);
	sem->sem_compteur = sem->sem_compteur + 1;
	if (sem->sem_compteur < 1)
	{
		assert(sem->sem_ctx_blocked);
		ctx = sem->sem_ctx_blocked;
		ctx->ctx_etat = CTX_ACTIVABLE;
		sem->sem_ctx_blocked = ctx->ctx_sem_next;
		ctx->ctx_sem_next=NULL;
	}
	irq_enable();
}


/* ====================== Main ====================== */
/*
int main(int argc, char *argv[]) 
{
	create_ctx(16384, f_ping, NULL);
	create_ctx(16384, f_pong, NULL);
	start_sched();

	exit(EXIT_SUCCESS);
}
*/

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