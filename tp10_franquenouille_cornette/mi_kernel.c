#include <stdio.h>
#include <stdlib.h>

#include "hw_config.h"
#include "mi_user.h"
#include "swap.h"
#include "mi_handler.h"
#include "mi_syscall.h"
#include "include/hardware.h"

static void ignore(){
	return;
}

static void 
switch_to_process0(void) 
{
    current_process = 0;
    _out(MMU_CMD, MMU_RESET);
}

static void
switch_to_process1(void) 
{
    current_process = 1;
    _out(MMU_CMD, MMU_RESET);
}

int 
main() 
{

    unsigned int i;

    if(init_hardware("etc/hardware.ini") == 0) {
	fprintf(stderr, "Error in hardware initialization\n");
        exit(EXIT_FAILURE);
    }
    
    printf("ATTENTION !\nSi l'on donne une taille de matrice très grande, le processus peut prendre un certain temps\n\n");
    printf("MATRIX_SIZE : %d\n\n", MATRIX_SIZE);
    for(i=0;i<16;i++){
	IRQVECTOR[i] = ignore;
    }

    _out(MMU_PROCESS,1);


    IRQVECTOR[MMU_IRQ] = mmuhandler;
    IRQVECTOR[SYSCALL_SWTCH_0] = switch_to_process0;
    IRQVECTOR[SYSCALL_SWTCH_1] = switch_to_process1;
    _mask(0x1001);

    if(init_swap("./swap_file")==-1){
	printf("erreur\n");
    }
    init();
    return 0;
}
