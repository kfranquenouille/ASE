#define VM_PAGES 4096
#define VM_SIZE (VM_PAGES*PAGE_SIZE)
#define PM_PAGES 256

#define N 20

unsigned int current_process;

int ppage_of_vaddr( unsigned vaddr);
void mmuhandler();
