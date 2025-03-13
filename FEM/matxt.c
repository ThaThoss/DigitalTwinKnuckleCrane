#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int matXT( double *C, double *A, double *B, int n, int m, int p) {

	int i, j, k;

	memset(C, 0, n*m*sizeof(double));
	for( i = 0; i < n; i++) {
	   for( j = 0; j < m; j++) {
	      for( k = 0; k < p; k++) {
		      *(C+m*i+j) += *(A+n*k+i) * *(B+m*k+j);
	      }
	   }
	}

	return 1;
}
