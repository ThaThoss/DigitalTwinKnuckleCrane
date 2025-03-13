#ifndef FEMLOOP
#define FEMLOOP
#include <semaphore.h>
#include <stdio.h>
#include <cstring>
#include <iostream>

#include "./../../../lib/mySharedMemory.h"
#include "./../../../lib/sendReceive.h"
#include "./../../../lib/commonDataSizes.h"
#include "./../../../lib/femClient.h"


void femComLoop(int femBodyNumber,int bytesForPointer, int shimd, char femIp[16], char name[32], int FemPort,  sem_t *semSyncWait, sem_t *semSyncGo );


#endif