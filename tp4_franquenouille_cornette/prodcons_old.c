#include "sem.h"
#define N 100 						/* nombre de places dans le tampon */

struct sem_s mutex, vide, plein;

static int cpt = 0;

void mettre()
{
	cpt++;
}

void enlever()
{
	cpt--;
}

void producteur (void *arg)
{
	while (1) 
	{
		sem_down(&vide); 			/* dec. nb places libres */
		sem_down(&mutex); 			/* entree en section critique */
		mettre();
		printf("On produit l'objet %d\n", cpt); 	/* produire l’objet suivant */
      	/*printf("On met\n");*/
      	/*for(i=0;i<MAXCOUNT;i++)
      	;
		printf("l'objet\n");*/ 		/* mettre l’objet dans le tampon */
		sem_up(&mutex); 			/* sortie de section critique */
		sem_up(&plein); 			/* inc. nb place occupees */
	}
}

void consommateur (void *arg)
{	
	while (1) 
	{
		sem_down(&plein); 			/* dec. nb emplacements occupes */
		sem_down(&mutex); 			/* entree section critique */
		printf("On retire l'objet %d\n", cpt); 	/* retire un objet du tampon */
		enlever();
		/*
      	printf("On utilise ");
		printf("l'objet\n"); */		/* utiliser l’objet */
		sem_up(&mutex); 			/* sortie de la section critique */
		sem_up(&vide); 				/* inc. nb emplacements libres */
	}
}

int main(int argc, char *argv[]) 
{
	sem_init(&mutex, 1); 				/* controle d’acces au tampon */
	sem_init(&vide, N); 				/* nb de places libres */
	sem_init(&plein, 0); 				/* nb de places occupees */	
	create_ctx(16384, producteur, NULL);
	create_ctx(16384, consommateur, NULL);
	start_sched();

	exit(EXIT_SUCCESS);
}
