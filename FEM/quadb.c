#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "qdconst.h"

int quadB( double *B, double *shg) 
{
/*****************************************************************************************
*                                                                                        * 
*                                           quadB                                        *
*                                  last updated: 5/2/2011                                *
*                                                                                        *
*           This subroutine sets up the Strain-Displacement Matrix B for two-            * 
*           dimensional continuum elements.                                              *
*                                                                                        *
*           Definition of Input Parameters:                                              * 
*           	                                                                         *	
*           		B = pointer to double where components of the strain-            *
*           		    displacement matrix are stored.                              *
*                                                                                        * 
*           	      shg = global shape function matrix.                                * 
*                                                                                        *
*           shg output for quad element:                                                 * 
*                                                                                        * 
*                      |N1,x    0   N2,x    0   N3,x    0   N4,x    0 |                  * 
*                      |                                              |                  * 
*                  B = |  0   N1,y    0   N2,y    0   N3,y    0   N4,y|                  * 
*                      |                                              |                  * 
*                      |N1,y  N1,x  N2,y  N2,x  N3,y  N3,x  N4,y  N4,x|                  * 
*                                                                                        *
*****************************************************************************************/

	int i, i2, i2m1;
	

	for( i = 0; i < npel; i++) {
		
		i2   = ndof*i + 1;
		i2m1 = i2 - 1; 
         
		*(B+i2m1)         = *(shg+i);
		*(B+neqel*1+i2)   = *(shg+npel*1+i);
		*(B+neqel*2+i2m1) = *(shg+npel*1+i);
		*(B+neqel*2+i2)   = *(shg+i);	
	}

	return 1;
}
