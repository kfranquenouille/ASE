#include <stdlib.h>
#include <stdio.h>

#define SYSCALL_SWITCH_0 16
#define SYSCALL_SWITCH_1 17

#define HW_CONFIG "hardware.ini"

#define PAGE_SIZE 4096
#define PM_PAGES 256		
#define VM_PAGES 4096
#define PM_SIZE PM_PAGES * PAGE_SIZE	
#define VM_SIZE VM_PAGES * PAGE_SIZE

#define N (PM_PAGES-2)

#define MMU_CMD 0x66
#define MMU_FAULT_ADDR 0xCD
#define MMU_IRQ		 13				
			
#define TLB_ADD_ENTRY	 0xCE								
#define TLB_DEL_ENTRY	 0xDE			
#define TLB_SIZE		32			
#define TLB_ENTRIES		 0x800		

extern unsigned int current_process;

struct tlb_entry_s{
	unsigned tlb_unused:8;
	unsigned tlb_vpage:12;
	unsigned tlb_ppage:8;
	unsigned tlb_access:3;
	unsigned tlb_isactive:1;
};


void init();
