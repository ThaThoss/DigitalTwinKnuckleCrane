#include <string.h>
#include <math.h>
#include "qdconst.h"

extern int sof;

int dotX( double *, double *, double *, int);

int qdstress_shg( double *det, int el, double *shl_node2, double *shg, double *xl) {

/* 

....  CALCULATE GLOBAL DERIVATIVES OF SHAPE FUNCTIONS AND
      JACOBIAN DETERMINANTS FOR AN EIGTH-NODE HEXAHEDRAL ELEMENT

  *(xl+npel*(0,1,2)+*(node_xi+2*k+(0,1))) = GLOBAL COORDINATES CORRESPONDING TO 
  					    NONZERO SHAPE FUNCTION dN/dxi

  *(xl+npel*(0,1,2)+*(node_eta+2*k+(0,1))) = GLOBAL COORDINATES CORRESPONDING TO
 				     	     NONZERO SHAPE FUNCTION dN/deta

      *(det+k)  = JACOBIAN DETERMINANT
      *(shl_node2+2*nsd*k+i) = LOCAL ("XI") DERIVATIVE OF SHAPE FUNCTION
      *(shl_node2+2*nsd*k+2*1+i) = LOCAL ("ETA") DERIVATIVE OF SHAPE FUNCTION
      *(shl_node2+2*nsd*k+2*2+i) = LOCAL SHAPE FUNCTION

      2 has replaced "npel" because there are only 2 non-zero shape funtion
      derivatives when evaluating at a node. 

      *(shg+npel*(nsd+1)*k+i) = X-DERIVATIVE OF SHAPE FUNCTION
      *(shg+npel*(nsd+1)*k+npel*1+i) = Y-DERIVATIVE OF SHAPE FUNCTION
      *(shg+npel*(nsd+1)*k+npel*2+i) = shl(npel*(nsd+1)*k+npel*3+i)
      *(xs+nsd*j+i) = JACOBIAN MATRIX
         i    = LOCAL NODE NUMBER OR GLOBAL COORDINATE NUMBER 
	 j    = GLOBAL COORDINATE NUMBER
	 k    = INTEGRATION-POINT NUMBER
      num_int = NUMBER OF INTEGRATION POINTS, EQ.1 OR 8

      shl_node2  = derivative of shape funtion dN/dc evaluated at nodal point
      node_xi[]  = only nonzero shape function number at particular node
                   for dN/dxi
      node_eta[] = only nonzero shape function number at particular node 
                   for dN/deta 

*/      

	int node_xi[]  = {0, 1, 0, 1, 2, 3, 2, 3}; 
	int node_eta[] = {0, 3, 1, 2, 1, 2, 0, 3};
	int check, i, j, k;

	double xs[4], temp;
       	
	memset(shg, 0, sosh*sof);

	for( k = 0; k < npel; k++) {
		
/* The jacobian, dx/dc, is calculated below */
	   for( j = 0; j < nsd; j++)
	}	
}
