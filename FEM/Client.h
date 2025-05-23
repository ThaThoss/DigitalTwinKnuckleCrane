#ifndef FEMCLIENT
#define FEMCLIENT

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <semaphore.h>

#include "./../lib/sendReceive.h"
#include "./../lib/mySharedMemory.h"
//#include "./SendReceiveFemQD.h"

#define PORT "8080"
#define numSpatialDim 2
#define numDegOfFreedom 2
#define nodesPerElement 4

typedef struct {
	int *allTheInts;
	int *connect;
	int *el_matl;
	int *nodesWithForce;
	int *forceGroup;
	int *fixedNodesX;
	int *fixedNodesY;
} INTEGERS;

typedef struct {
	double *allTheDoubles;
	double *coord;
	double *force;
	double *U;
	double *materialProperties;
	double *displacedNodesX;
	double *displacedNodesY;
} DOUBLES;


typedef struct {
	DOUBLES doubles;
	int sizeOfMemDoubles;
	INTEGERS integers;
	int degOfFreedom;
	int sizeOfMemIntegers;
	int numFixx;
	int numFixy;
	int numForce;
	int numEl;
	int numNodPnt;
	int numMaterial;
	int PlaneStressFlag;
	int gravity_Flag;
} FEMDATATOSEND;

int qdInitialReader(FEMDATATOSEND *dataToSend, FILE *femDatFile){
	
	int check = 0;
	char buffer[256];
	fgets(buffer,BUFSIZ,femDatFile);
	check = fscanf(femDatFile,"%d %d %d %d %d \n",&(dataToSend->numEl),&(dataToSend->numNodPnt),&(dataToSend->numMaterial),&(dataToSend->PlaneStressFlag),&(dataToSend->gravity_Flag));
if(check!=5){
	
	printf("Error reading initial values, read %d integers, not 5\n",check);
	exit(1);
}
return 0;
}
/*----------------------------------------------------*/



int qdClientMemory(FEMDATATOSEND *dataToSend){

/* Doubles */
	dataToSend->doubles.allTheDoubles = (double *)calloc(dataToSend->sizeOfMemDoubles,sizeof(double));
	if(!(dataToSend->doubles.allTheDoubles)){
		printf("Failed to allocate memory for the doubles");
		exit(1);
	}
/* Integers */
	dataToSend->integers.allTheInts = (int *)calloc(dataToSend->sizeOfMemIntegers,sizeof(int));
	
	if(!(dataToSend->integers.allTheInts)){
		printf("Failed to allocate memory for the ints");
		exit(1);
	}

return 0;
}
/*----------------------------------------------------*/



int qdClientDistributePointers(FEMDATATOSEND *dataToSend){

	int memCounter = 0;
/* Doubles */
	dataToSend->doubles.coord=dataToSend->doubles.allTheDoubles + memCounter;
	memCounter += dataToSend->numNodPnt * numSpatialDim;

	dataToSend->doubles.force = dataToSend->doubles.allTheDoubles + memCounter;
	memCounter += dataToSend->degOfFreedom;

	dataToSend->doubles.materialProperties = dataToSend->doubles.allTheDoubles + memCounter;
	memCounter += 3*dataToSend->numMaterial;

	dataToSend->doubles.displacedNodesX = dataToSend->doubles.allTheDoubles + memCounter;
	memCounter += dataToSend->numNodPnt +1;

	dataToSend->doubles.displacedNodesY = dataToSend->doubles.allTheDoubles + memCounter;

	memCounter = 0;

/* Integers */
	//printf("number of elements %d",dataToSend->numEl);
	dataToSend->integers.connect = dataToSend->integers.allTheInts + memCounter;
	memCounter += dataToSend->numEl*nodesPerElement;

	dataToSend->integers.el_matl = dataToSend->integers.allTheInts + memCounter;
	memCounter += dataToSend->numEl;
	
	dataToSend->integers.nodesWithForce = dataToSend->integers.allTheInts + memCounter;
	memCounter += dataToSend->numNodPnt+1;

	dataToSend->integers.forceGroup = dataToSend->integers.allTheInts + memCounter;
	memCounter += dataToSend->numNodPnt;

	dataToSend->integers.fixedNodesX = dataToSend->integers.allTheInts + memCounter;
	memCounter += dataToSend->numNodPnt+1;

	dataToSend->integers.fixedNodesY = dataToSend->integers.allTheInts + memCounter;


return 0;
}
/*-------------------------------------------------------*/



int qdClientReader(FEMDATATOSEND *dataToSend,FILE *femDatFile){
	
	char buffer[BUFSIZ];
	int i,j, matNum, elNum, nodeNum, counter;
	
	printf("Reading femqd file\n");
	printf("Number of Elements: %d, nodes: %d, materials: %d, degree of freedom: %d\n",
			dataToSend->numEl,dataToSend->numNodPnt, 
			dataToSend->numMaterial, dataToSend->degOfFreedom);

	printf("Plane theory flag: %d\n\n",dataToSend->PlaneStressFlag);
	printf("Gravity flag: %d\n\n",dataToSend->gravity_Flag);
/*Pull out the header text*/
       fgets( buffer, BUFSIZ, femDatFile);

/* Pull out material properties*/

	for(i = 0; i<dataToSend->numMaterial; i++){
	 fscanf(femDatFile,"%d\n", &matNum );	
	 printf("Material (%d) Emod, nu, density\n",matNum);
	 fscanf(femDatFile," %lf %lf %lf\n",(dataToSend->doubles.materialProperties+3*matNum),
			 (dataToSend->doubles.materialProperties+3*matNum+1),
			 (dataToSend->doubles.materialProperties+3*matNum+2));
	 printf(" %9.4f %9.4f %9.4f\n",*(dataToSend->doubles.materialProperties+3*matNum),
			 *(dataToSend->doubles.materialProperties+3*matNum+1),
			 *(dataToSend->doubles.materialProperties+3*matNum+2));
	}
	fgets(buffer,BUFSIZ,femDatFile);
	printf("\n");


/*Reading the connectivity*/
/*
 * 1) connect contains the nodalpoints that make up an element. the size of connect is 
 *    nodesPerElement*numberOfElements 
 * 2) node numbering occurs in a counter clokwise direction  
 */
for( i = 0; i < dataToSend->numEl; i++ ) {
	   fscanf( femDatFile, "%d", &elNum );
	   printf( "connectivity for element (%4d) ", elNum);
	   for( j = 0; j < nodesPerElement; j++ ) {
		   fscanf( femDatFile, "%d", (dataToSend->integers.connect+nodesPerElement*elNum+j));
		   printf( "%4d ", *(dataToSend->integers.connect+nodesPerElement*elNum+j));
	   }
	   fscanf( femDatFile, "%d\n", (dataToSend->integers.el_matl+elNum));
	   printf( " with matl %3d\n", *(dataToSend->integers.el_matl+elNum));
	} 
        fgets( buffer, BUFSIZ, femDatFile );
        printf( "\n" );	

/* Pull out node coordinate info and echo to screen                     */
/*    1) coord contains the x, y coordinates of a node                  */  
/*       (size of coord is nsd*numnp)                                   */ 
	for( i = 0; i < dataToSend->numNodPnt; i++ ) {
           fscanf( femDatFile, "%d", &nodeNum);
           printf( "Node (%d) ", nodeNum);
	   printf( "coordinates ");
	   for( j = 0; j < numSpatialDim; j++ ) {
		   fscanf( femDatFile, "%lf", (dataToSend->doubles.coord+numSpatialDim*nodeNum+j));
		   printf( "%9.4f ", *(dataToSend->doubles.coord+numSpatialDim*nodeNum+j));
	   }
           fscanf( femDatFile, "\n");
           printf( "\n" );	   
	}
        fgets( buffer, BUFSIZ, femDatFile );
        printf( "\n" );	

/* Pull out x-boundary condition info and echo to screen                */
/*    1) bc.fix.x contains the node numbers where the                   */ 
/*       x-displacement is prescribed                                   */       
/*    2) x-displacement value is put into correct location in the U     */ 
/*    	 array. (size of U is dof=ndof * numnp)                         */ 
/*    3) bc.num_fix.x contains the total number of nodes where          */ 
/*       x-displacement has been prescribed                             */ 
	counter = 0;
	fscanf( femDatFile, "%d",  (dataToSend->integers.fixedNodesX + counter));
	printf( "node (%4d) has an x prescribed displacement of: ", 
					*dataToSend->integers.fixedNodesX);
	while( *(dataToSend->integers.fixedNodesX+counter) > -1 ) {
	//while(counter<11){	
		fscanf( femDatFile, "%lf\n%d", (dataToSend->doubles.displacedNodesX+counter),
			       				(dataToSend->integers.fixedNodesX+counter+1));
		printf( "%14.6e\n", *(dataToSend->doubles.displacedNodesX+counter));
		printf( "node (%4d) has an x prescribed displacement of: ",
			*(dataToSend->integers.fixedNodesX+counter+1));
		counter++;
	}
        dataToSend->numFixx = counter;
	fscanf( femDatFile, "\n" );
        fgets( buffer, BUFSIZ, femDatFile );
	printf( "\n\n" );

/* Pull out y-boundary condition info and echo to screen                */
/*    1) bc.fix.y contains the node numbers where the                   */ 
/*       y-displacement is prescribed                                   */       
/*    2) y-displacement value is put into correct location in the U     */ 
/*    	 array. (Size of U is dof=ndof * numnp)                         */ 
/*    3) bc.num_fix.y contains the total number of nodes where          */ 
/*       y-displacement has been prescribed                             */ 
        counter = 0;
	fscanf( femDatFile, "%d", dataToSend->integers.fixedNodesY+counter );
	printf( "node (%4d) has an y prescribed displacement of: ", 
				*(dataToSend->integers.fixedNodesY+counter));

	while( *(dataToSend->integers.fixedNodesY+counter) > -1) {
		fscanf( femDatFile, "%lf\n%d", (dataToSend->doubles.displacedNodesY+counter),
			       			dataToSend->integers.fixedNodesY+counter+1);
		printf( "%14.6e\n", *(dataToSend->doubles.displacedNodesY+counter));
		printf( "node (%4d) has an y prescribed displacement of: ",
			*(dataToSend->integers.fixedNodesY+counter+1));
		counter++;
	}
        dataToSend->numFixy = counter;
	fscanf( femDatFile, "\n" );
        fgets( buffer, BUFSIZ, femDatFile );
	printf( "\n\n" );
	
/* Pull out force boundary condition info and echo to screen            */
/*    1) bc.force contains the node numbers where a nodal force         */ 
/*       has been prescribed                                            */       
/*    3) bc.num_force contains the total number of nodes where          */ 
/*       a force has been prescribed                                    */ 
	counter = 0;
	printf( "force vector for node: " );
	fscanf( femDatFile, "%d", dataToSend->integers.nodesWithForce);
	printf( "(%4d)", *(dataToSend->integers.nodesWithForce));
	while( *(dataToSend->integers.nodesWithForce + counter) > -1 ) {
		for( j = 0; j < numDegOfFreedom; j++ ) {
			fscanf( femDatFile, "%lf ", (dataToSend->doubles.force+counter*2+j));
			printf( "%16.4f "  , *(dataToSend->doubles.force+counter*2+j));
		}
		fscanf( femDatFile, "(%4d)", dataToSend->integers.forceGroup+counter); 
		fscanf( femDatFile, "\n");
		printf( "\n" );
		printf( "force vector for node: ");
		counter++;
		fscanf( femDatFile, "%d", dataToSend->integers.nodesWithForce + counter);
	}
	dataToSend->numForce = counter;
	fscanf( femDatFile, "\n" );
	fgets( buffer, BUFSIZ, femDatFile );
	printf( "\n\n" );

	return 0;
}	
/*------------------------------------------------*/
int qdClientInitialSender(FEMDATATOSEND *dataToSend,int sockfd){
	
	int dataBuffer[8] = {0};
	dataBuffer[0] = (dataToSend->numEl);
	dataBuffer[1] = (dataToSend->numNodPnt);
	dataBuffer[2] = (dataToSend->numMaterial);
	dataBuffer[3] = (dataToSend->numFixx);
	dataBuffer[4] = (dataToSend->numFixy);
	dataBuffer[5] = (dataToSend->numForce);
	dataBuffer[6] = (dataToSend->PlaneStressFlag);
	dataBuffer[7] = (dataToSend->gravity_Flag);

	if(SendNIntsNoBlock( sockfd, dataBuffer, 8*sizeof(int32_t))){
		printf("Problems with sendNints in qdInitialSender");
		return 1;
	}

return 0;
}
/*--------------------------------------------------------------------*/


int qdClientSender(FEMDATATOSEND *dataToSend, int sockfd){
	
	int i, check, sizeofCoord, sizeofConnect;

/* Send Coordinates */	
         sizeofCoord = dataToSend->numNodPnt*numSpatialDim; 
         check = SendNDoubles(sockfd,dataToSend->doubles.coord,sizeofCoord*sizeof(double)); 
         if(check){
         	printf("Problem with sending Coord in qdClientSender");
		return 1;
	}

/* Sending Force */
	check = 0; 
	//send nodes with force
	printf("Sending %d nodes with force\n",dataToSend->numForce);
	check = SendNInts( sockfd, dataToSend->integers.nodesWithForce, dataToSend->numForce*sizeof(int));

	//Send the forces
	check += SendNDoubles( sockfd, dataToSend->doubles.force, dataToSend->numForce*numDegOfFreedom*sizeof(double));
/*
	for(i=0;i<dataToSend->numForce;i++){
		check += SendInt32(sockfd,*(dataToSend->integers.nodesWithForce+i));
		check += SendNDoubles(sockfd,dataToSend->doubles.force+i*numDegOfFreedom,
					numDegOfFreedom*sizeof(double));
		printf("sent Force[%d] = [%lf, %lf ]\n",*(dataToSend->integers.nodesWithForce+i),
		*(dataToSend->doubles.force+i*numDegOfFreedom),
		*(dataToSend->doubles.force+i*numDegOfFreedom+1) );
		
	}*/
	if(check){
		printf("Problem with sending forces in qdClientSender");
		return 1;
	}

	/* Sending Connect array */
	sizeofConnect = dataToSend->numEl*nodesPerElement;
	printf("Sending connect array, with %d ints",sizeofConnect);
	check = SendNInts(sockfd,dataToSend->integers.connect,sizeofConnect*sizeof(int));
	if(check){
		printf("Problems with sending Connect array in qdClientSender");
		return 1;
	}



/* Sending material properties */
	check = 0;
	for(i=0;i<dataToSend->numMaterial;i++){
		check += SendNDoubles(sockfd,dataToSend->doubles.materialProperties+i*3,
					3*sizeof(double));
	}
	if(check){
		printf("Problems with sending material properties");
		return 1;
	}
/* Sending Prescribed displacements in X */
	check = 0;
	for(i=0;i<dataToSend->numFixx;i++){
		check += SendInt32(sockfd,*(dataToSend->integers.fixedNodesX+i));
		check += SendNDoubles(sockfd,dataToSend->doubles.displacedNodesX + i,
					sizeof(double));
	}
	if(check){
		printf("Problems with sending prescribed positions in X");
		return 1;
	}
/* Sending Prescribed displacements in Y */
	check = 0;	
	for(i=0;i<dataToSend->numFixy;i++){
		check += SendInt32(sockfd,*(dataToSend->integers.fixedNodesY + i));
		check += SendNDoubles(sockfd,dataToSend->doubles.displacedNodesY + i,
					sizeof(double));
	}
	if(check){
		printf("Problems with sending prescribed positions in Y");
		return 1;
	}


return 0;
}

#endif