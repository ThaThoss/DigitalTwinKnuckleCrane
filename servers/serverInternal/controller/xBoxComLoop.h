#ifndef XBOX
#define XBOX

#include <semaphore.h>
#include <stdio.h>
#include "./../../../lib/mySharedMemory.h"
#include "./../../../lib/sendReceive.h"
#include "./../../../lib/commonDataSizes.h"

void xBoxComLoop(int clientSocket, sharedMemoryStructForIntegration *shmStru );


#endif