#include "try.h"

static struct ctx_s ctx;

static int mul(int depth)
{
	int i;
	int retour = 0;
	switch (scanf("%d", &i)){
		case EOF:
			return 1; /* neutral element*/
		case 0:
			return mul(depth+1); /* erroneous read */
		case 1:
			if (i)
				return i * mul(depth+1);
			else
				throw(&ctx, retour);
	}

	/* Non atteint mais obligatoire selon compilateur */
	return 0;
}

int main()
{
	int product;

	printf("A list of int, please\n");		
	product = try(&ctx, mul, 0);
	printf("product = %d\n", product);

	return EXIT_SUCCESS;
}
