#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int dotX( double *C, double *A, double *B, int n ) 
{
/******************************************************************************
*                                                                             *
*                                   dotx                                      *
*                          last updated 6/2/2011                              *
*                                                                             *
*       This subroutine calculates the dot product of two vectors.            *
*                                                                             * 
*       Definition of Input Parameters:                                       * 
*                                                                             * 
*       	C = pointer two double where scalar result will be stored.    *  
*                                                                             *
*               A = first vector.                                             *
*                                                                             *
*               B = second vector.                                            *
*                                                                             *
*               n = size of both vectors.                                     *
*                                                                             *
*       Calculation Notes:                                                    * 
*                                                                             *
*               given   a = < a1, a2, a3, ..., an >                           * 
*                       b = < b1, b2, b3, ..., bn >                           * 
*                                                                             *
*                           n                                                 * 
*       	dot(a, b) = E ai*bi + a1*b1 + a2*b2 + ... + an*bn             * 
*                          i=1                                                * 
*                                                                             *
******************************************************************************/ 
	
	int i;

	*C = 0.0;

	for( i = 0; i < n; i++ ) {
		*C += *(A+i) * *(B+i);
	}

	return 1;
}
