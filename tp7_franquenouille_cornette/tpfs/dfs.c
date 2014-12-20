#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "superbloc.h"

extern char* get_hw_config();

extern int load_current_volume();

int main() 
{
	/*char test[32] = "test"; */
	init_mbr();
	list_volumes();
	write_mbr();
    if (load_super(0) == 0)
    {
    	printf("Chargement du superbloc effectué !\n\n");
    }
	display();
	exit(EXIT_SUCCESS);
}