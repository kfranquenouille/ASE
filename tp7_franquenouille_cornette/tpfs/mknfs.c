#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "superbloc.h"

int main() 
{
	char test[32] = "test";
	init_mbr();
	create_volume(0, 1, 11, VOL_BASE);
	list_volumes();
    write_mbr();
	init_super(0, 10, test);
/*	while (new_bloc() > 0)
	{
		printf("Création d'un nouveau bloc\n");
	}*/
/*	free_bloc(2);
	printf("Free du bloc 2\n");
	free_bloc(3);
	printf("Free du bloc 3\n");*/
	new_bloc();
	new_bloc();
	save_super();
	printf("Superbloc rempli de 2 blocs !\n");
	printf("Taux d'occupation : %f/100\n", taux_occupation(0));
	/*free_bloc(2); */
	/*printf("Free du bloc numéro 2\n"); */
	save_super();	
	exit(EXIT_SUCCESS);
}