#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mi_syscall.h"
#include "hardware.h"

struct tlb_entry_s tlbe;
unsigned int current_process=0;

static int ppage_of_vaddr(int process, unsigned vaddr){
	unsigned int vpage;
	vpage=(vaddr>>12)& 0xFFF;

	if(vaddr > ((int)virtual_memory + VM_SIZE - 1) ){
		return -1;
	}
	if(vpage<0 || vpage>N/2){
		return -1;
	}
	return (process * N/2 +1)+vpage;
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

static void mmuhandler(){
	int ppage, vaddr;

	vaddr=_in(MMU_FAULT_ADDR);
	ppage=ppage_of_vaddr(current_process,vaddr);
	if(ppage==-1){
		fprintf(stderr,"Erreur de segmentation personnelle\n");
		exit(EXIT_FAILURE);
	}else{
		
		tlbe.tlb_vpage= (vaddr >> 12) & 0xFFF;
		tlbe.tlb_ppage=ppage;
		tlbe.tlb_access=7;
		tlbe.tlb_isactive=1;
		_out(TLB_ADD_ENTRY,*((int *)&tlbe));
	}
}
	

void init(){

  if(init_hardware(HW_CONFIG) == 0) {
	fprintf(stderr, "Error in hardware initialization\n");
	exit(EXIT_FAILURE);
    }	
	

	IRQVECTOR[MMU_IRQ] = mmuhandler;
    IRQVECTOR[16] = switch_to_process0;
    IRQVECTOR[17] = switch_to_process1;
}




