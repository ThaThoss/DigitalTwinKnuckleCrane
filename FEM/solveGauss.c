#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int solveGauss( double *A, double *f, int *id, int neqn, int dof) {


	int     i, j, k;
	double  factor, sum;


/* Gauss Elimination is used to solve the equation Kd=f */
/* Forward Substitution */
	for( i = 0; i < neqn-1; i++) {
	   for( j = i+1; j < neqn; j++) {
		{	
		    factor = *(A+j*neqn+i) / *(A+i*neqn+i); 
	      	    for( k = i; k < neqn; k++) {
			*(A+j*neqn+k) -= *(A+i*neqn+k) * factor; 
	            }	
		 } 
	      	*(f+j) -= *(f+i) * factor;
	      } 
	   }
	

/* Backward  Substitution */
	*(f+neqn-1) /= *(A+(neqn-1)*neqn+(neqn-1));
	for( i = neqn-2; i > -1; i--) {
            sum = 0.0; 
	    for( j = i+1; j < neqn; j++) {
		 sum += *(A+i*neqn+j) * *(f+j); 	
	    }
	    *(f+i) = (*(f+i) - sum) / *(A+i*neqn+i);
	}	    


	return 1;
}

