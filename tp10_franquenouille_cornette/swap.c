#include <stdio.h>

#include "swap.h"
#include "hw_config.h"
#include "include/hardware.h"

static FILE *swap_file;

/* Initialisation du swap */
char init_swap(const char *path){
	swap_file=fopen(path,"w+");
	if(swap_file==NULL){
		return -1;
	}
	return 0;
}

/* On stocke dans le fichier de swap */
char store_to_swap(int vpage, int ppage){
	if(fseek(swap_file,vpage<<12, SEEK_SET)==0){
		if(fwrite((void*)((ppage<<12)+*((int*)&physical_memory)),PAGE_SIZE,1,swap_file)==PAGE_SIZE){
			return 0;
		}
	}
	return -1;
}

/* On recupere depuis le fichier de swap */
char fetch_from_swap(int vpage, int ppage){
	if(fseek(swap_file,vpage<<12, SEEK_SET)==0){
		if(fread((void*)((ppage<<12)+*((int*)&physical_memory)),PAGE_SIZE,1,swap_file)==PAGE_SIZE){
			return 0;
		}
	}
	return -1;
}
