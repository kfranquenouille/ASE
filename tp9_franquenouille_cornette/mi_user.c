#include <string.h>
#include "mi_syscall.h"
#include "hardware.h"


static int 
sum(void *ptr) 
{
    int i;
    int sum = 0;
    
    for(i = 0; i < PAGE_SIZE * N/2 ; i++)
        sum += ((char*)ptr)[i];
    return sum;
}


int 
main(int argc, char **argv) 
{
    void *ptr;
    int res,i;

    init();

    _mask(0x1001);

    ptr = virtual_memory;
    for(i=0; i<= (N/4);i++){
	    _int(SYSCALL_SWITCH_0);
	    memset(ptr, 1, PAGE_SIZE * (i));
	     printf("Current process : %d \n",current_process);
	    _int(SYSCALL_SWITCH_1);
	    memset(ptr, 3, PAGE_SIZE * (i));
	    printf("Current process : %d \n",current_process);
     }

    _int(SYSCALL_SWITCH_0);
    res = sum(ptr);
    printf("Resultat du processus 0 : %d\n",res);
    _int(SYSCALL_SWITCH_1);
    res = sum(ptr);
    printf("Resultat processus 1 : %d\n",res);
	return 0;
}
