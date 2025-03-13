#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX(x, y) (((x)<(y))?(x):(y))

int globalKassemble( double *A, double *K_el, int *lm, int neqel, int neqn) {

	int i, j;
	int lmi, lmj;

	for( i = 0; i < neqel; i++){
		if( *(lm+i) > -1 ) {
		   for( j = 0; j < neqel; j++) {
		       if( *(lm+j) > -1) {
			   lmi = *(lm+i);
			   lmj = *(lm+j);
		      	   *(A+neqn*lmi+lmj) += *(K_el+neqel*i+j);
		       }
		    }
		}	

	}

}



