#include "stdio.h"
#include "stdlib.h"

#include "swap.h"
#include "hw_config.h"
#include "mi_handler.h"
#include "include/hardware.h"

struct tlb_entry_s{
	unsigned entry_rfu:8;
	unsigned entry_vpage:12;
	unsigned entry_ppage:8;
	unsigned entry_exec:1;
	unsigned entry_write:1;
	unsigned entry_read:1;
	unsigned entry_use:1;
};

struct ppage_map{
	unsigned ppage_vpage:12;
	unsigned ppage_mapper:1;
};

struct vpage_map{
	unsigned vpage_ppage:8;
	unsigned vpage_mapper:1;
};

unsigned next_page=1;

struct ppage_map map_ppage[PM_PAGES];
struct vpage_map map_vpage[VM_PAGES];

int ppage_of_vaddr( unsigned vaddr){
	unsigned int vpage;
	struct tlb_entry_s* tlb;
	unsigned entries = _in(TLB_ENTRIES);
	int i;
	if(vaddr < ((unsigned int)&virtual_memory) || vaddr > ((unsigned int)&virtual_memory)+VM_SIZE-1){
		return -1;
	}
	vpage=((vaddr>>12) & 0xFFF);
	if(vpage<N/2){
		tlb = (struct tlb_entry_s*)(&entries);
		for (i=0; i<TLB_SIZE; i++) {
			if (tlb[i].entry_vpage == vpage) {
				return tlb[i].entry_ppage;
			}
		}
    }
    return -1;
}

void mmuhandler(){
	struct tlb_entry_s tlb;
	unsigned addr,vpage;
	tlb.entry_read = 1;
	tlb.entry_write = 1;
	tlb.entry_exec = 1;
	addr=_in(MMU_FAULT_ADDR);
	vpage=(addr>>12) & 0xFFF;

	/* Si la page virtuelle est déjà mappée */
	if (map_vpage[vpage].vpage_mapper) {
		/* On change les valeur dans l'entrée de la tlb */
        tlb.entry_vpage = vpage;        
        tlb.entry_ppage = map_vpage[vpage].vpage_ppage;
		tlb.entry_use=1;
        _out(TLB_ADD_ENTRY, *(int*)&tlb);
	}
	else{
		/* si la prochaine page page dans le mappage des pages physique est deja mappée */
		if(map_ppage[next_page].ppage_mapper){
			/* On stocke dans le swap */
			store_to_swap(map_ppage[next_page].ppage_vpage, next_page);
			map_vpage[map_ppage[next_page].ppage_vpage].vpage_mapper=0;
			tlb.entry_vpage = map_ppage[next_page].ppage_vpage;
			tlb.entry_ppage = next_page;
			_out(TLB_DEL_ENTRY, *((int*)&tlb));
		} 

		if(fetch_from_swap(vpage, next_page)==-1){
			/*printf("fetch erreur\n");*/
		}

		/* on pointe sur la prochaine page que l'on peut mapper */
		map_vpage[vpage].vpage_ppage = next_page;
		map_vpage[vpage].vpage_mapper = 1;
		map_ppage[next_page].ppage_vpage = vpage;
		map_ppage[next_page].ppage_mapper = 1;

		/* MAJ dans l'entrée de la TLB */
		tlb.entry_vpage = vpage;        
		tlb.entry_ppage = next_page;
		tlb.entry_use=1;

		_out(TLB_ADD_ENTRY, *(int*)&tlb);

		/* MAJ de la prochaine page */
		next_page=(next_page%(PM_PAGES-1))+1;
	}
}
