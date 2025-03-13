#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int matX( double *C, double *A, double *B, int n, int m, int p) 
{
/******************************************************************************
*                                   matX                                      *
*                          (Matrix Multiplication)                            *
*                          last updated: 5/4/2011                             *
*                                                                             *
*       This subroutine performs the multiplication of 2 matrices,            *
*       and stores the result in a third matrix.                              *
*                                                                             * 
*                             [A] * [B] = [C]                                 * 
*                                                                             * 
*                             size of [A] = n x p                             * 
*                                                                             * 
*                             size of [B] = p x m                             * 
*                                                                             * 
*                             size of [C] = n x m                             * 
*                                                                             *
*       Input variable definitions:                                           *
*                                                                             *
*                   C = pointer to matrix that will store the new             *
*                       matrix formed by the multiplication.                  *
*                                                                             *
*                   A = pointer to matrix that will multiply a                *
*                       second matrix [B].                                    *
*                                                                             * 
*                   B = pointer to matix that will be multiplied              *
*                       by another matrix [A].                                * 
*                                                                             * 
*                   n = number of rows of matrix [A].                         * 
*                                                                             * 
*                   m = number of columns of matrix [B].                      * 
*                                                                             * 
*                   p = number of coulumns of matrix [A] and                  * 
*                       number of rows of matrix [B].                         *  
*                                                                             * 
*       Sample multiplication of 2 2x2 matrices:                              * 
*                                                                             * 
*                     |A11  A12|   |B11  B12|   |C11  C12|                    * 
*                     |        | X |        | = |        |                    * 
*                     |A21  A22|   |B21  B22|   |C21  C22|                    * 
*                                                                             * 
*                           C11 = A11*B11 + A12*B21                           *
*                           C12 = A11*B12 + A12*B22                           * 
*                           C21 = A21*B11 + A22*B21                           * 
*                           C22 = A21*B12 + A22*B22                           *                       
*                                                                             * 
*                                                                             *
******************************************************************************/

	int i, j, k;

	memset(C, 0, n*m*sizeof(double));
	for( i = 0; i < n; i++) {
	   for( j = 0; j < m; j++ ) {
	      for( k = 0; k < p; k++) {
		 *(C+m*i+j) += *(A+p*i+k) * *(B+m*k+j);
	      }
	   }
	}
	return 1;
}
