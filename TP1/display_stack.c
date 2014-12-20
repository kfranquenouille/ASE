#include <stdlib.h>
#include <stdio.h>

int main()
{
	void* esp;
	void* ebp;

	asm("movl %%esp, %0\n\t" "movl %%ebp, %1"
		:"=r"(esp), "=r"(ebp));

	printf("ESP: %p\n"
		   "EBP: %p\n",
		   esp, ebp);
	
	return EXIT_SUCCESS;
}
