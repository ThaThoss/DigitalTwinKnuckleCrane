#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SendReceiveFemQD.h"
#include "qdconst.h"
#include "qdstruct.h"
#include "qdfunctions.h"


int qdServerInitialReceiver(SIZES* sizes,FORCESTRUCT *forceStruct, int sockfd){
	int check, counter=0;
	int intsToRecive[8] = {0};
	int sofint = sizeof(int32_t);
	ReceiveNIntsNoBlock( sockfd, intsToRecive,  8*sofint);
	/* Receive Number of elements */
	memcpy(&(sizes->numel),intsToRecive,sofint);
	counter++;

	/* Receive Number of Nodal points */
	memcpy(&(sizes->numnp),(intsToRecive+counter),sofint);
	counter++;

/* Receive number of materials */
	memcpy(&(sizes->nmat),(intsToRecive+counter),sofint);
	counter++;

/* Receive number of Fixed nodes in X */
	memcpy(&(sizes->num_fix_x),(intsToRecive+counter),sofint);
	counter++;

/* Receive number of Fixed nodes in Y */
	memcpy(&(sizes->num_fix_y),(intsToRecive+counter),sofint);
	counter++;

/* Receive number of forces */
	memcpy(&(sizes->num_force),(intsToRecive+counter),sofint);
	counter++;

/* Receive plane stres flag */
	memcpy(&(sizes->plane_stress_flag),(intsToRecive+counter),sofint);
	counter++;

/*Copy gravity flag*/
	memcpy(&(forceStruct->gravity_flag),(intsToRecive+counter),sofint);
	
return 0;
}
int qdServerReceiver(BOUND bc, int *connect, double *coord, int *el_matl, FORCESTRUCT forceStruct,
			MATL *matl, double *U,SIZES *sizes, int sockfd){

	int check, i, forceNodeNumber;
	double *dubBuffer;
	dubBuffer = (double *)calloc(ndof+1,sizeof(double));
	
/* Receive Coordinates */
printf("Receive Coordinates\n");
	check = ReceiveNDoubles(sockfd,coord,sizes->sizeOfCoord*sizeof(double));
	if(check){
		printf("Problems with receiving coordinates in qdServerReceiver");
		return 1;
	}
/* Receive Force */
printf("Receive Force\n");
	check = 0;
	forceNodeNumber = 0;
	check = ReceiveNInts( sockfd, bc.force,bc.num_force[0]*sizeof(int));
	
	check = ReceiveNDoubles(sockfd, forceStruct.forces,
							bc.num_force[0]*ndof*sizeof(double));
/*				
	for(i=0;i<bc.num_force[0];i++){
		check += ReceiveInt32(sockfd, &bc.force[forceNodeNumber]);
		check += ReceiveNDoubles(sockfd,force+ndof*bc.force[forceNodeNumber],
					ndof*sizeof(double));
//printf("InitialForce[%d] = [%lf, %lf] \n",bc.force[forceNodeNumber],*(force+ndof*bc.force[forceNodeNumber]),*(force+ndof*bc.force[forceNodeNumber]+1 ));
		forceNodeNumber++;

	}
*/
	if(check){
		printf("problems with receiving forces in qdServerReceiver");
		return 1;
	}

/* Receive Connect Array */
printf("Receive Connect Array\n");
	check = ReceiveNInts(sockfd,connect,sizes->sizeOfConnect*sizeof(int));
	if(check){
		printf("Problems with receiving connect in qdServerReceiver");
		return 1;
	}

/* Receive material properties */
printf("Receive material properties \n");
	check = 0;
	for(i=0;i<sizes->nmat;i++){
		check += ReceiveNDoubles(sockfd,dubBuffer,3*sizeof(double));
		matl[i].E = *dubBuffer;
		matl[i].nu = *(dubBuffer+1);
		matl[i].rho = *(dubBuffer+2);
	}
	if(check){
		printf("Problems with Receiving material properties in qdServerReceiver");
		return 1;
	}

/* Receive Prescribed displacements X */
printf("Receive Prescribed displacements X \n");
	check = 0;
	for(i=0; i<bc.num_fix[0].x;i++){
		check += ReceiveInt32(sockfd,&bc.fix[i].x);
		check += ReceiveNDoubles(sockfd,U+ndof*bc.fix[i].x,sizeof(double));
	}
	if(check){
		printf("Problems with receiving Prescribed Displacements X in qdServerReceiver");
		return 1;
	}

/* Receive Prescribed displacements Y */
printf("Receive Prescribed displacements Y \n");
	check = 0;
	for(i=0; i<bc.num_fix[0].y;i++){
		check += ReceiveInt32(sockfd,&bc.fix[i].y);
		check += ReceiveNDoubles(sockfd,(U+ndof*bc.fix[i].y+1),sizeof(double));
	}
	if(check){
		printf("Problems with receiving Prescribed Displacements Y in qdServerReceiver");
		return 1;
	}

	free(dubBuffer);
printf("Done Receiving\n");
return 0;

}

int printConnect(int* connect,SIZES sizes){
	for(int i=0; i<sizes.numel; i++){
		printf("Connectivety for element (%4d) ",i);
		for(int j=0;j<npel;j++){
			printf("%4d ",*(connect+npel*i+j));
		}
		printf("\n");
	}
return 1;
}

int printCoord(double *coord, SIZES sizes){
	for(int i=0; i<sizes.numnp; i++){
		printf("Node (%d) coordinates ",i);
		for(int j=0; j<nsd; j++){
			printf("%9.4f ", *(coord +nsd*i+j));
		}
		printf("\n");
	}
return 1;
}

int recieveForce(BOUND bc,FORCESTRUCT forceStruct, int sockfd){
	/* Receive Force */
	/*[force x y],gravityx,gravityy,gogo*/
	int check = ReceiveNChars(sockfd, forceStruct.buffer, forceStruct.numBytesofForce);

	if(check){
		printf("problems with receiving forces in qdServerReceiver");
		return 0;
	}
	
	return *(forceStruct.gogo);

}
