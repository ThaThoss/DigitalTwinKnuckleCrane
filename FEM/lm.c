#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int formlm( int *connect, int *id, int *lm, int ndof, int npel, int numel ) {
/* Assembly of the lm array (the matrix which gives */
/* the degree of freedom per element and node)      */

	int i, j, k, node;

	for( k = 0; k < numel; k++ ) {
	    for( j = 0; j < npel; j++) {
	      node = *(connect + npel*k + j);
	      for( i = 0; i < ndof; i++ ) {
		      *(lm + ndof*npel*k + ndof*j + i) = *(id + ndof*node + i);
	      }
	   }
	}
	return 1;
}
