#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "qdconst.h"
#include "qdstruct.h"

extern int dof, neqn;

int qdformid( BOUND bc, int *id) {
	int i, counter;
	counter = 0;

/* Place a -1 in x position for nodes where x-displacement is prescribed */	
	for( i = 0; i < bc.num_fix[0].x; i++ ) { 
	   *(id + ndof*bc.fix[i].x)     = -1;
	}

/* Place a -1 in y position for nodes where y-displacement is prescribed */
	for( i = 0; i < bc.num_fix[0].y; i++ ) {
	   *(id + ndof*bc.fix[i].y + 1) = -1;
	}
        
/* Start numbering positions not containing a -1 from 0 and up */	
	for( i = 0; i < dof; i++ ) {
	   if( *(id + i) != -1 ) {
	   	*(id + i ) = counter;
     		counter++;
	   }
	}
	neqn = counter;
	return 1;
}
