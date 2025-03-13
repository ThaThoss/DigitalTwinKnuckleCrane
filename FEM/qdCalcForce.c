#include "qdstruct.h"
#include "qdconst.h"
#include "string.h"

extern int dof;
extern int numnp;
int qdCalcForce(double * force, double *tempForce,double *nodalMass, BOUND bc,FORCESTRUCT *forceStruct, int *id){

    memcpy(force,tempForce,dof*sizeof(double));

	for(int i=0;i<bc.num_force[0];i++){

		*(force +ndof*bc.force[i] + 0 ) += *(forceStruct->forces +i*ndof + 0); 
		*(force +ndof*bc.force[i] + 1 ) += *(forceStruct->forces +i*ndof + 1); 

	}

	//Density and gravity constant are already in nodalMass, done in qdkassembl.
	if(forceStruct->gravity_flag){
		for(int i=0;i<numnp;i++){
			
			*(force + ndof*i) += *(nodalMass + i )*(forceStruct->gravity_X[0]);
			*(force + ndof*i + 1) += *(nodalMass + i )*(forceStruct->gravity_Y[0]);

		}
	}

	//contract force vector based on id array.
	int counter = 0;	
	for( int j = 0; j < dof; j++) {
		if( *(id + j) > -1) {
			*(force + counter) = *(force + j);
			counter++;
		}
	}


return 0;
}