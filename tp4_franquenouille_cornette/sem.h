#ifndef SWITCHTO
#define SWITCHTO

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAGIC_NUMBER 0xABBACACA
#define SEM_MAGIC_NUMBER 0x00000069
#define MAXCOUNT 100000000

typedef void func_t (void*);

enum etat_e {CTX_TERMINATED, CTX_ACTIVABLE, CTX_INIT, CTX_BLOCKED};

struct ctx_s {
	void* ctx_esp;
	void* ctx_ebp;
	int ctx_magic_number;
	void* ctx_arg;
	func_t* ctx_func;
	void* ctx_stack;
	enum etat_e ctx_etat;
	struct ctx_s* ctx_next;
	struct ctx_s* ctx_sem_next; /* On pointe sur le prochain contexte bloqué */
};

struct sem_s {
	int sem_compteur;
	struct ctx_s* sem_ctx_blocked; /* Liste des contextes bloqués */
	int sem_magic_number;
};

int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *args);
int create_ctx(int stack_size, func_t f, void *args);

void switch_to_ctx(struct ctx_s *ctx);
void yield();
void start_sched();

void sem_init(struct sem_s *sem, unsigned int val);
void sem_down(struct sem_s *sem);
void sem_up(struct sem_s *sem);

#endif
