
#include <semaphore.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include "./../../../lib/mySharedMemory.h"
#include "./../../../lib/sendReceive.h"
#include "xBoxComLoop.h"
#include "./../../../lib/commonDataSizes.h"

void xBoxComLoop(int clientSocket, sharedMemoryStructForIntegration *shmStru ){

    int16_t gogo = true;
    int gogoToSend = 0;
    int xBoxSocket = clientSocket;
    unsigned char *dataToRecieve = (unsigned char*)malloc(knuckleNBytesToKnu);

    write(clientSocket,"gogo",sizeof("gogo"));
    while(gogo){

        if(ReceiveNUnsignedChars(xBoxSocket, dataToRecieve, knuckleNBytesToKnu))
        {
            printf("Problems with ReceiveNUnsignedChars in xBoxComLoop");
        }
        memcpy(&gogo, (dataToRecieve + SZ_DOUBLE*3), sizeof(int16_t));

        sem_wait(&(shmStru->semLock));
            memcpy(shmStru->dataForKnuckle,dataToRecieve,knuckleNBytesToKnu);
            if(shmStru->gogo){
                gogoToSend = 1;
            }else{
                gogoToSend = 0;
            }
            shmStru->newData = 1;
        sem_post(&(shmStru->semLock));
        SendInt32(xBoxSocket,gogoToSend);

    }
    sem_wait(&(shmStru->semLock));
        shmStru->gogo = 0;
    sem_post(&(shmStru->semLock));

    std::cout << "xBoxComLoop recieved gogo 0, shutting down." << std::endl;



}