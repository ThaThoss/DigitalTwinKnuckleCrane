
#include <semaphore.h>
#include <stdio.h>
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

    char buffer[255] = {0};

    printf("ip: %s \n",knuckleIp);	
	
	int portno = knucklePort;
	int sockfd = connectToServer(portno,knuckleIp);
	if(sockfd==-1){
		printf("Error in connectToServer");
	}

	read(sockfd,buffer,255);
	printf("Ansver was %s\n",buffer);
	int check = strncmp(buffer,"200",3);
	if(check){
		printf("\nServer error with code %s\n",buffer);
	}

    write(sockfd,"gogo",sizeof("gogo"));
    while(gogo){

        if(ReceiveNUnsignedChars(sockfd, dataToRecieve, numCharsToRecieve))
        {
            printf("Problems with ReceiveNUnsignedChars in knuckleComLoop");
        }
          //dataToRecieve = [R1,th4,th5,th6,dth1,dth2,dth3,dth4,dth5,dth6,ddth1,ddth2,ddth3,ddth4,ddth5,ddth6,ma_and_mom, globCraneTipWantedPos];48 doubles;
          memCounter=0;
          sem_wait(&(shmStru->semLock));
            memcpy(shmStru->R1, dataToRecieve, 9*SZ_DOUBLE);
            memCounter += 9;
            memcpy(shmStru->angles, (dataToRecieve + memCounter*SZ_DOUBLE), 3*SZ_DOUBLE);
            memCounter += 3;
            memcpy(shmStru->angularVel, (dataToRecieve + memCounter*SZ_DOUBLE), 6*SZ_DOUBLE);
            memCounter += 6;
            memcpy(shmStru->angularAcc, (dataToRecieve + memCounter*SZ_DOUBLE), 6*SZ_DOUBLE);
            memCounter += 6;
            memcpy(shmStru->massTimesAccAndMom, (dataToRecieve + memCounter*SZ_DOUBLE), 21*SZ_DOUBLE);           
            memCounter += 21;
            memcpy(shmStru->globalPosPoint, (dataToRecieve + memCounter*SZ_DOUBLE), 3*SZ_DOUBLE);
            memcpy(dataToSend,shmStru->dataForKnuckle,nCharsToSend);
            gogo = shmStru->gogo;
            shmStru->newData=1;
        sem_post(&(shmStru->semLock));
        
        if(SendNUnsignedChar(sockfd, dataToSend, nCharsToSend))
        {
            printf("problems with SendNUnsignedChar in knuckleComLoop");
            
        }

    }
    free(dataToRecieve);
    free(dataToSend);
    shmdt(shmStru);
    close(sockfd);
    std::cout << "knuckleComLoop recieved gogo 0, shutting down..." << std::endl;
}