#include <stdio.h>
#include <string.h>

#include "mi_handler.h"
#include "mi_user.h"
#include "mi_syscall.h"
#include "include/hardware.h"

typedef unsigned int matrix[MATRIX_SIZE][MATRIX_SIZE];

int 
sum(void *ptr) 
{
    int i;
    int sum = 0;
    for(i = 0; i < PAGE_SIZE * N/2 ; i++){
        sum += ((char*)ptr)[i];
    }
    return sum;
}

/* Multiplication de matrice */
void mulmat(matrix* dest,matrix* mat1,matrix* mat2){
	int i,j,k;
	for(j=0;j<MATRIX_SIZE;j++){
		for(i=0;i<MATRIX_SIZE;i++){
			for(k=0;k<MATRIX_SIZE;k++){
				(*dest)[i][j]+=(*mat1)[i][k]*(*mat2)[k][j];
			}
		}
	}
}

/* Affichage de la matrice */
void print_matrice(matrix* matrice){
	int i,j;
	for(i=0;i<MATRIX_SIZE;i++){
		for(j=0;j<MATRIX_SIZE;j++){
			printf("%d ",(*matrice)[i][j]);
		}
		printf("\n");
	}
}

void init(){
    int i,j;
    matrix* matrix1 = (matrix*)virtual_memory;
    matrix* matrix2 = ((matrix*)virtual_memory)+1;
    matrix* matrix3 = ((matrix*)virtual_memory)+2;

    for(i=0;i<MATRIX_SIZE;i++){
    	for(j=0;j<MATRIX_SIZE;j++){
    		(*matrix1)[i][j]=1;
    		(*matrix2)[i][j]=1;
            /* matrice de sortie */
    		(*matrix3)[i][j]=0;
    	}
    }

    mulmat(matrix3,matrix1,matrix2);

    print_matrice(matrix3);

}
