#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "qdconst.h"

int qdshl( double g, double *shl, double *w )
{
/*******************************************************************************
*                                                                              *
*                                   qdshl                                      *
*                         last date updated: 6/2/2011                          *
*                                                                              *
*     This subroutine calculates the local shape functions and their           * 
*     derivatives of a quadrilateral element. It also calculates the           * 
*     Gauss Integration weight factors.                                        * 
*                                                                              * 
*     It is based on the subroutine QDCSHL from the book "The Finite Element   *
*     Method" by Thomas Hughes, page 784.                                      * 
*                                                                              * 
*     In the local element coordinate system x & y are replaced by b and n.    * 
*     In Thomas Hughes' book, b and n are ("XI") and ("ETA") respectively.     *
*                                                                              * 
*     Definition of input parameters:                                          *
*                                                                              *
*             g = location where shape function is to be evaluated.            * 
*                 In order to work properly value should be multiplied         * 
*                 by 2.                                                        *
*                                                                              *
*           shl = pointer to double array where values calculated will         * 
*                 be stored.                                                   *     
*                                                                              *
*             w = pointer to double where the integration weighting            * 
*                 factors will be stored to perform Gauss Integration.         * 
*                                                                              * 
*    Quad Shape Function:                                                      *
*                                                                              * 
*                     Na(b, n) = (1/4)*(1+ba*b)*(1+na*n)                       *
*                                                                              * 
*                 dNa(b,n)/db  = (1/4)*ba*(1+na*n)                             * 
*                                                                              * 
*                 dNa(b,n)/db  = (1/4)*na*(1+ba*n)                             * 
*                                                                              *       
*             	        	a  |  ba  |  na                                * 
*             	       		-----------------                              *	
*             			1  |  -1  |  -1                                * 
*             			2  |   1  |  -1                                * 
*             			3  |   1  |   1                                * 
*             			4  |  -1  |   1                                * 
*                                                                              * 
*     shl output:                                                              * 
*                                                                              *	
*  |dN1(g1)/db ... dN4(g1)/db  dN1(g1)/dn ... dN4(g1)/dn  N1(g1) ... N4(g1)|   * 
*  |                                                                       |   * 
*  |dN1(g2)/db ... dN4(g2)/db  dN1(g2)/dn ... dN4(g2)/dn  N1(g2) ... N4(g2)|   * 
*  |                                                                       |   * 
*  |dN1(g3)/db ... dN4(g3)/db  dN1(g3)/dn ... dN4(g3)/dn  N1(g3) ... N4(g3)|   * 
*  |                                                                       |   * 
*  |dN1(g4)/db ... dN4(g4)/db  dN1(g4)/dn ... dN4(g4)/dn  N1(g4) ... N4(g4)|   * 
*                                                                              *
*******************************************************************************/

	double ra[] = {-0.50,  0.50,  0.50, -0.50};
	double sa[] = {-0.50, -0.50,  0.50,  0.50};
	double temps, tempr, r, s;
	int    i, k;

/* calculating the weights and local dN/db, dn matrix */
	for( k = 0; k < num_int; k++ ) 
	{
		*(w+k) = 1.0;
		r=g*(*(ra+k));      /* b coordinate of evaluation location */	
		s=g*(*(sa+k));      /* n coordinate of evaluation location */ 
		for ( i = 0; i < npel; i++ ) 
		{
                    tempr = pt5 + *(ra+i)*r;
                    temps = pt5 + *(sa+i)*s;
                    *(shl+npel*(nsd+1)*k+i)        = *(ra+i)*temps;
		    *(shl+npel*(nsd+1)*k+npel*1+i) = tempr*(*(sa+i));  
                    *(shl+npel*(nsd+1)*k+npel*2+i) = tempr*temps;
		}
	}
        return 1;
}


int qdshg( double *det, int el, double *shl, double *shg, double *xl) 
{
/*******************************************************************************
*                                                                              *
*                                   qdshg                                      *
*                           last updated 6/2/2011                              *
*                                                                              *      
*     This subroutine calculates the global shape function derivatives and     * 
*     the jacobian determinants for a quad element. The calculation can        * 
*     occur at either the gauss pts or nodal pts. Passing in *shl will         * 
*     calculate at gauss pts, *shl_node will calculate at nodes                * 
*                                                                              *
*     It is a C translation of the subroutine QDSHG from the book              *
*      "The Finite Element Method" by Thomas Hughes, page 783.                 * 
*                                                                              * 
*     In the local element coordinate system x & y are replaced by b and n.    * 
*     In Thomas Hughes' book, b and n are ("XI") and ("ETA") respectively.     *
*                                                                              * 
*     Definition of input parameters:                                          *
*                                                                              *
*           det = pointer to double where jacobian determinant of the          *
*                 element will be stored.                                      * 
*                                                                              * 
*            el = global element node number.                                  * 
*                                                                              *
*           shl = contains the local shape function and its derivatives        * 
*                 for a quad element.                                          *
*                                                                              * 
*           shg = pointer to double where element global shape function        *
*                 and derivatives will be stored.                              * 
*                                                                              *
*            xl = coordinates of nodal points of the element. i loops          *
*                 over x, y.                                                   * 
*                                                                              * 
*     Definition of locale parameters:                                         *
*                                                                              * 
*            xs = contains the jacobian matrix.                                * 
*                                                                              * 
*     Na,x/Na,y calculations:                                                  *
*                                                                              * 
*                      <Na,x Na,y> = <Na,b Na,n> |b,x  b,y|                    * 
*                                                |        |                    * 
*                                                |n,x  n,y|                    * 
*                                                                              * 
*          Can calculate Na,b and Na,n but can not directly compute            *
*          b,x / b,y / n,x / n,y. Need to use the definition of                * 
*          x(b, n) and y(b, n) to calculate b,x / b,y / n,x / n,y.             *                                
*                                                                              * 
*                                     n_en                                     * 
*                          x(b, n)  =  E   Na(b,n)*x_a                         * 
*                                     a=1                                      * 
*                                                                              * 
*                                     n_en                                     * 
*                          y(b, n)  =  E   Na(b,n)*y_a                         * 
*                                     a=1                                      * 
*                                                                              * 
*                                      |x,b  x,n|                              * 
*                              x,b  =  |        |                              *  
*                                      |y,b  y,n|                              * 
*                                                                              * 
*                   |b,x  b,y|              1 | y,n  -x,n|                     * 
*                   |        | = (x,b)^-1 = - |          |                     *
*                   |n,x  n,y|              j |-y,b   x,b|                     *               
*                                                                              * 
*                         j = det(x,b) = x,b y,n  - x,n y,b                    * 
*                                                                              * 
*          using the above relationships the global derivatives of the         * 
*          shape functions become:                                             * 
*                                                                              * 
*          	    <Na,x Na,y> = <Na,b Na,n> * 1 | y,n  -x,n|                 * 
*          	                                - |          |                 * 
*          	                                j |-y,b   x,b|                 * 
*     shg output matrix:                                                       * 
*                                                                              * 
*   | N1,x(g1) ... N4,x(g1) ... N1,y(g1) ... N4,y(g1) ... N1(g1) ... N4(g1)|   *
*   |                                                                      |   *
*   | N1,x(g2) ... N4,x(g2) ... N1,y(g2) ... N4,y(g2) ... N1(g2) ... N4(g2)|   *
*   |                                                                      |   * 
*   | N1,x(g3) ... N4,x(g3) ... N1,y(g3) ... N4,y(g3) ... N1(g3) ... N4(g3)|   *
*   |                                                                      |   *
*   | N1,x(g4) ... N4,x(g4) ... N1,y(g4) ... N4,y(g4) ... N1(g4) ... N4(g4)|   * 
*                                                                              * 
*******************************************************************************/ 
 
	double xs[nsd*nsd], temp;
	int    check, i, j, k;

	memcpy(shg, shl, sosh*sizeof(double));

	for( k = 0; k < num_int; k++ ) {

/* The jacobian matrix (x,b) is calculated below */
		for( j = 0; j < nsd; j++ ) {
	        for( i =0; i < nsd; i++ ) {
				check = dotX((xs+nsd*i+j), (shg+npel*(nsd+1)*k+npel*j),
				(xl+npel*i), npel);  
		     }
		}
	       
	       	*(det + k) = *(xs) * (*(xs+3)) - (*(xs+2)) * (*(xs+1));
		if(*(det+k) <= 0.0) {
			printf("The element (%d) is inverted; det:%f; integ pt: %d\n", 
				el, *(det+k), k);
	        return 0;
		}	

		/* The inverse of the jacobian matrix, (x,b)^-1, is calculated below */
		temp     = *(xs);
		*(xs)    = *(xs+3)/(*(det+k));
		*(xs+1) *= -1./(*(det+k));
		*(xs+2) *= -1./(*(det+k));
		*(xs+3)  = temp/(*(det+k));		

		for(i = 0; i < npel; i++ ) {
		
		    *(shg+npel*(nsd+1)*k+i) = *(xs) * *(shl+npel*(nsd+1)*k+i) + 
			   *(xs+2) * *(shl+npel*(nsd+1)*k+npel*1+i);
		    *(shg+npel*(nsd+1)*k+npel*1+i) = *(xs+1) * *(shl+npel*(nsd+1)*k+i) +
			   *(xs+3) * *(shl+npel*(nsd+1)*k+npel*1+i); 
		}		
	}
}



