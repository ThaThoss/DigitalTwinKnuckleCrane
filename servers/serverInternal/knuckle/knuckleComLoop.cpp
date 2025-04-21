
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "knuckleComLoop.h"
#include "./../../../lib/mySharedMemory.h"
#include "./../../../lib/sendReceive.h"
#include "./../../../lib/commonDataSizes.h"

void knuckleComLoop(int shmid, int knucklePort, char knuckleIp[16]){

    int16_t gogo = 1;
    //Defined in commonDataSizes
    int numCharsToRecieve = knuckleNBytesFromKnu;
    int nCharsToSend = knuckleNBytesToKnu;
    int memCounter = 0;
    
    unsigned char *dataToRecieve = (unsigned char*)malloc(numCharsToRecieve);
    unsigned char *dataToSend = (unsigned char*)malloc(nCharsToSend);

    printf("Attaching shared memory from knuckle thread \n");
    sharedMemoryStructForIntegration *shmStru; 
    shmStru = (sharedMemoryStructForIntegration*)shmat(shmid,NULL,0);
    if (shmStru == (void *) -1) {
      perror("Knuckle Shared memory attach in Knuckle Com Loop");
    }
    sem_wait(&(shmStru->semLock));
        int nBodies = shmStru->numRBDbodies;
        int axisOfRotation[nBodies] = {0};
        int numBytesTosend = shmStru->numBytesForRBD;
        memcpy(axisOfRotation,(shmStru->sharedFEMData + shmStru->numBytesForHeader),nBodies*sizeof(int));
    sem_post(&(shmStru->semLock));
    SHAREDMEMORYPOINTERSRBD shMemRBDPoint;
    int semProtect = 1;
    distributeRbdMemPointers( shmStru,&shMemRBDPoint, axisOfRotation, semProtect );

    numBytesTosend = calcBytesNeededForRBD( nBodies, axisOfRotation);

    sem_wait(&(shmStru->semLock));
        char *initialDataToSend = (char *)calloc(numBytesTosend,sizeof(char));
        memcpy(initialDataToSend,shMemRBDPoint.dataForRBD,numBytesTosend);
        shmStru->isKnuckleRunning = 1;
    sem_post(&(shmStru->semLock));

    char buffer[255] = {0};

   	int portno = knucklePort;
	int sockfd = connectToServer(portno,knuckleIp);
	if(sockfd==-1){
		printf("Error in connectToServer");
	}

	read(sockfd,buffer,255);
	printf("Ansver was %s, in knuckle com loop\n",buffer);
	int check = strncmp(buffer,"200",3);
	if(check){
		printf("\nServer error with code %s\n",buffer);
	}
    sleep(1);
    SendInt32(sockfd,nBodies);
    SendNInts(sockfd,axisOfRotation,nBodies*sizeof(int));

    sleep(2);
    SendNChar(sockfd,initialDataToSend,numBytesTosend);
    sleep(2);
    write(sockfd,"gogo",sizeof("gogo"));
    printf("Knuckle com loop up and running");
    while(gogo){

        if(ReceiveNUnsignedChars(sockfd, dataToRecieve, numCharsToRecieve))
        {
            printf("Problems with ReceiveNUnsignedChars in knuckleComLoop \n");
        }
        //dataToRecieve = [R1,th4,th5,th6,dth1,dth2,dth3,dth4,dth5,dth6,ddth1,ddth2,ddth3,ddth4,ddth5,ddth6,ma_and_mom, globCraneTipWantedPos];48 doubles;
        memCounter=0;
        sem_wait(&(shmStru->semLock));
            memcpy(shmStru->R1, dataToRecieve, 9*SZ_DOUBLE);
            memcpy(shMemRBDPoint.initialAngularValues,dataToRecieve,shmStru->bytesForInitialAng);//get angles, angv.
            memCounter += 9;
            memcpy(shmStru->angles, (dataToRecieve + memCounter*SZ_DOUBLE), 3*SZ_DOUBLE);
            memCounter += 3;
            memcpy(shmStru->angularVel, (dataToRecieve + memCounter*SZ_DOUBLE), 6*SZ_DOUBLE);
            memCounter += 6;
            memcpy(shmStru->angularAcc, (dataToRecieve + memCounter*SZ_DOUBLE), 6*SZ_DOUBLE);
            memcpy(shMemRBDPoint.angAcc, (dataToRecieve + memCounter*SZ_DOUBLE), 6*SZ_DOUBLE);
            memCounter += 6;
            memcpy(shmStru->massTimesAccAndMom, (dataToRecieve + memCounter*SZ_DOUBLE), 21*SZ_DOUBLE);
            memcpy(shMemRBDPoint.massTimesAcc, (dataToRecieve + memCounter*SZ_DOUBLE), 21*SZ_DOUBLE);           
            memCounter += 21;
            memcpy(shmStru->globalPosPoint, (dataToRecieve + memCounter*SZ_DOUBLE), 3*SZ_DOUBLE);
            memcpy(shMemRBDPoint.craneGlobalPos, (dataToRecieve + memCounter*SZ_DOUBLE), 3*SZ_DOUBLE);
            memcpy(dataToSend,shmStru->dataForKnuckle,nCharsToSend);
            gogo = shmStru->gogo;
            shmStru->newData=1;
            //myPrintAngles(shMemRBDPoint.initialAngularValues, shMemRBDPoint.angAcc, shMemRBDPoint.craneGlobalPos);
        sem_post(&(shmStru->semLock));
        
        
        if(SendNUnsignedChar(sockfd, dataToSend, nCharsToSend))
        {
            printf("problems with SendNUnsignedChar in knuckleComLoop\n");
            
        }

    }
    sem_wait(&(shmStru->semLock));
    shmStru->isKnuckleRunning = 0;
    sem_post(&(shmStru->semLock));

    free(dataToRecieve);
    free(dataToSend);
    shmdt(shmStru);
    close(sockfd);
    std::cout << "knuckleComLoop recieved gogo 0, shutting down..." << std::endl;
}


int calcBytesNeededForRBD(int nBodies, int axisOfRotation[]){

	int bytesForInitialAng = 0;
    for(int i=0; i<nBodies;i++){
        if(axisOfRotation[i]==4){
            bytesForInitialAng += 12;
        }else if(axisOfRotation[i]>4 || axisOfRotation[i]<0){
            std::cout << "invalid rotational axis, axis was: "<< axisOfRotation[i] << ", for body number " << i << std::endl;
            return 1;
            
        }else{
            bytesForInitialAng += 2;
        }
    }
    std::cout << "numDoubles for Ang = "<< bytesForInitialAng << std::endl;

	//ints
	int bytesSpatFree = nBodies*3*sizeof(int);
	int bytesForAxisOfRotation = nBodies*sizeof(int);
	//doubles
	int bytesForCmPos = nBodies*3*2*sizeof(double);
	int bytesForMass = nBodies*sizeof(double);
	int bytesForInertia = nBodies*9*sizeof(double);
	int bytesForInitialTransl = nBodies*6*sizeof(double);
	bytesForInitialAng = bytesForInitialAng*sizeof(double);

	int numBytes =  bytesSpatFree + bytesForAxisOfRotation + (bytesForCmPos + bytesForMass + bytesForInertia + bytesForInitialTransl + bytesForInitialAng);

	return numBytes;

}