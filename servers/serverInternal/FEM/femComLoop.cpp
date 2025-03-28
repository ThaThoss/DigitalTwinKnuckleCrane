
#include <semaphore.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "femComLoop.h"
#include "./../../../lib/mySharedMemory.h"
#include "./../../../lib/sendReceive.h"
#include "./../../../lib/commonDataSizes.h"
#include "./../../../lib/femClient.h"




//#include "../FEM/SendReceiveFemQD.h"

using namespace std;
void femComLoop(int femBodyNumber, int bytesForPointer,int shmid, char femIp[16], char name[32], int femPORT,  sem_t *semSyncWait, sem_t *semSyncGo){


	char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory for fem loop numb %d: %s\n",femBodyNumber ,cwd);
    } else {
        perror("getcwd() error");
    }



	/* Create socket and connect */	
	
	FILE *femDatFile = {0};
	FEMDATATOSEND dataToSend;
	sharedMemoryPointers sharedPointers = {0};	
	char buffer[BUFSIZ];
	
	int sockfd=0, portno, check;
	//int numberOfElements, numberOfNodalPoints, numberOfMaterialProperties, planeStressFlag, degOfFreedom;
	//int *connect, el_matl;
	int dubSizeForce, dubSizeCoord, dubSizeMatProp, intSizeConnect, intSizeElementalMaterial, intSizeFixedNodes, intSizePreForce, dubSizeDisplacedNodes;
	//double *coord, *force, *U;

	// shmat to attach to shared memory
	printf("Attaching shared memory from thread %d\n",femBodyNumber);
    sharedMemoryStructForIntegration *shmStru; 
    shmStru = (sharedMemoryStructForIntegration*)shmat(shmid,NULL,0);
    if (shmStru == (void *) -1) {
      perror("Shared memory attach");
    }

	printf("filename = %s\n",name);
	femDatFile = fopen(name,"r");
	if(femDatFile == NULL){
		printf("Can't find file %30s\n",name);
		exit(1);
	}

	printf("Attached shared memory from thread %d\n",femBodyNumber);
	check = qdInitialReader(&dataToSend,femDatFile);
	
	 //numel numnp nmat plane_stress_flag, gravity_flag
	check = getFEMHeader( &dataToSend, shmStru, femBodyNumber);

    cout << "numEl "<<dataToSend.numEl<<" numNodPnt "<<dataToSend.numNodPnt<<" numMaterial "<<dataToSend.numMaterial<<" PlaneStressFlag "<<dataToSend.PlaneStressFlag<<" gravity_Flag "<<dataToSend.gravity_Flag<<endl;
	


	
	dataToSend.degOfFreedom = dataToSend.numNodPnt*numDegOfFreedom;



	printf("Calculating sizes\n");
	/* Size of memory needed for the doubles */
	dubSizeForce = dataToSend.degOfFreedom;
	dubSizeCoord = dataToSend.numNodPnt*numSpatialDim;
	dubSizeMatProp = 3*dataToSend.numMaterial;
	dubSizeDisplacedNodes = 2*dataToSend.numNodPnt + 2;
	dataToSend.sizeOfMemDoubles = dubSizeForce + 
			dubSizeCoord + dubSizeDisplacedNodes +dubSizeMatProp;

	/* Size of memory needed for the integers */
	intSizeConnect = dataToSend.numEl*nodesPerElement;
	intSizeElementalMaterial = dataToSend.numEl;
	intSizeFixedNodes = 2*dataToSend.numNodPnt+2;
	intSizePreForce = dataToSend.numNodPnt*2+1; // *2 for group number----------------------------------------	
	dataToSend.sizeOfMemIntegers = intSizeConnect+intSizeElementalMaterial + 
				       intSizeFixedNodes+intSizePreForce;

		

	check = qdClientMemory(&dataToSend);

	check = qdClientDistributePointers(&dataToSend);

	check = qdClientReader(&dataToSend, femDatFile);



	check = distrubuteFemSharedMemPointers( shmStru, &sharedPointers, femBodyNumber, bytesForPointer );
	
	check = saveFemData( shmStru, &dataToSend, &sharedPointers, femBodyNumber, bytesForPointer );


/*
sem_wait(&(shmStru->semLock));
	cout << "Number of force nodes: " << dataToSend.numForce << endl;
	cout << "Force Node Number,   Group number  " << endl;
	for(int i=0; i< dataToSend.numForce; i++ ){
		cout << *(sharedPointers.forceNodes+i) << "   " <<  *(sharedPointers.forceGroup +i) << endl;
	}
sem_post(&(shmStru->semLock));
*/


	int numBytesToRecieve = (dataToSend.degOfFreedom +dataToSend.numNodPnt)*sizeof(double);
	double *dataToReceive = (double* )calloc(numBytesToRecieve,sizeofDouble);
		if(dataToReceive==NULL){
		printf("Calloc failed for dataToReceive for Send in femComLoop numb %d", femBodyNumber);
		exit(EXIT_FAILURE);
	}

	double *nodalDeformation = dataToReceive;
	double *vonMieses = (dataToReceive + dataToSend.degOfFreedom);

	
	check =	saveDeformationToSharedMemory( shmStru,  &dataToSend,  &sharedPointers, nodalDeformation);
	check = saveVonMiesesToSharedMemory( shmStru,  &dataToSend,  &sharedPointers, vonMieses);

	sem_wait(&(shmStru->semLock));
	int femdatanumb = shmStru->femDataReady +1;
	shmStru->femDataReady = femdatanumb;
	sem_post(&(shmStru->semLock));
	cout << " incremented femDataReady is now " << femdatanumb << endl;




	printf("ip: %s \n",femIp);	
	
	portno = femPORT;
	sockfd = connectToServer(portno,femIp);
	if(sockfd==-1){
		printf("Error in connectToServer");
	}

	read(sockfd,buffer,255);
	printf("Ansver was %s\n",buffer);
	check = strncmp(buffer,"200",3);
	if(check){
		printf("\nServer error with code %s\n",buffer);
	}

	


	printf("Done sending mesh:)\n");
											
	int bytesToSend = (dataToSend.numForce*2+2)*sizeof(double) + sizeof(int);
	char* dataBufferForSend = (char*)calloc(bytesToSend,sizeof(char));
	if(dataBufferForSend==NULL){
		printf("Calloc failed for dataBuffer for Send in femComLoop numb %d", femBodyNumber);
		exit(EXIT_FAILURE);
	}
	double *force = (double*)dataBufferForSend;
	
	/*
	for(int k =0;k<dataToSend.numForce;k++){
		printf("force sent = [%lf, %lf]\n",*(dataToSend.doubles.force+2*k), *(dataToSend.doubles.force +2*k+1));
	}
   */


	double *gravityDirection = force+dataToSend.numForce*2;
    getForceFromShardeMem( force, gravityDirection, shmStru, &sharedPointers, dataToSend.numForce, femBodyNumber);

	int *gogo = (int*)(dataBufferForSend + (dataToSend.numForce*2+2)*sizeof(double));
	gogo[0] = 1;
	int count = 0;

	sem_post(semSyncWait);
	printf("Fem loop %d: Finished preparing, waiting...\n", femBodyNumber);

	// Wait for reset
	sem_wait(semSyncGo);
	printf("FEM loop %d connected and ready, sending initial values, starting calculations...\n", femBodyNumber);
	
	check = qdClientInitialSender(&dataToSend,sockfd);
	
	check = qdClientSender(&dataToSend,sockfd);
	while(gogo[0]){
		
		//printf("-----------Waiting to recieve-------------\n");
		ReceiveNChars( sockfd, (char*)dataToReceive, numBytesToRecieve);

		count++;
        // Signal completion to controll thread
        sem_post(semSyncWait);
        //printf("Fem loop %d: Finished work, waiting...\n", femBodyNumber);

        // Wait for reset
        sem_wait(semSyncGo);
        //printf("Fem loop %d: Starting next cycle...\n", femBodyNumber);
		getForceFromShardeMem( force, gravityDirection, shmStru, &sharedPointers, dataToSend.numForce, femBodyNumber);


		*gogo = isGogo(shmStru);
		// buffer = [Force x and y, gravity x and y, gogo int]
		SendNChar(sockfd,  dataBufferForSend, bytesToSend );

		check =	saveDeformationToSharedMemory( shmStru,  &dataToSend,  &sharedPointers, nodalDeformation);
		check = saveVonMiesesToSharedMemory( shmStru,  &dataToSend,  &sharedPointers, vonMieses);

		
		
	}

	free(dataToReceive);
	free(dataBufferForSend);
	shmdt(shmStru);

	printf("Fem client %d shutting down\n",femBodyNumber);

	close(sockfd);

	
}