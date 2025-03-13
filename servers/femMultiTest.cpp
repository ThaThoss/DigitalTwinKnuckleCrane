#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "./femComLoop.h"
#include "./../lib/sendReceive.h"
#include "./../lib/mySharedMemory.h"
#include "./../lib/commonDataSizes.h"
#include "./../lib/femClient.h"


using namespace std;

int main(int argc, char *argv[]){

    char names[3][32] = {{"dog3"},{"dog3"},{"dog3"}};
    int FEMClients = 0;
    char femIp[16] = "127.0.0.1";
    char femport[] = FEMPORT;
    int nFEMBodies = 3;
   


    //numel numnp nmat plane_stress_flag, gravity_flag
    //196 	250   1   1   1 // dog file;
    int headers[nFEMBodies][5] = {{196, 	250,   1,   1,   1},{196, 	250,   1,   1,   1},{196, 	250,   1,   1,   1}};

    int numBytesForSharedMemory = 0;
    numBytesForSharedMemory += sizeof(sharedMemoryStructForIntegration);//Size of known sizes in struct
    numBytesForSharedMemory += sizeof(int)*5*nFEMBodies;// header for FEM files, 5 ints each;
    for(int i=0;i<nFEMBodies;i++){
        numBytesForSharedMemory += calcBytesNeededForFEM( headers[i][0],  headers[i][1],  headers[i][2],  headers[i][3],  headers[i][4]);
    }


    /*Shared memory section:*/
	sharedMemoryStructForIntegration *shmStru;
	key_t key = ftok("./../lib/sharedMemoryFile",KEY_VAL);

    // shmget returns an identifier in shmid
    int shmid = shmget(key,numBytesForSharedMemory,0644|IPC_CREAT);
    cout<<"keyDT = " << key << " and size of = "<< numBytesForSharedMemory << endl;
    if (shmid == -1) {
      perror("Shared memory");
      return 1;
    }

    // shmat to attach to shared memory
    shmStru = (sharedMemoryStructForIntegration*)shmat(shmid,NULL,0);
    if (shmStru == (void *) -1) {
      perror("Shared memory attach");
      return 1;
    }
    sem_init(&(shmStru->semLock),1,1);

    shmStru->numFEMBodies = nFEMBodies;
    shmStru->numRBDbodies = 4;

    memcpy(shmStru->sharedFEMData,headers,5*sizeof(int)*3);
  
    cout << "Internal Server Shared memory attatched, gogo was: " << shmStru->gogo << endl;

    thread commThreadFEM[3];
    int bytesForPointer[4] = {0};
    bytesForPointer[0] = sizeof(int)*5*nFEMBodies;

for(int i=0;i<3;i++){
    commThreadFEM[i] = thread(femComLoop, i, bytesForPointer[i], shmid, femIp, names[FEMClients], femport);
    bytesForPointer[i+1] = bytesForPointer[i] + calcBytesNeededForFEM( headers[i][0],  headers[i][1],  headers[i][2],  headers[i][3],  headers[i][4]);
    FEMClients++;
    cout << "FEM Client connected nr " << FEMClients <<  endl;          
}
cout << "done connecting----------------" << endl;

commThreadFEM[0].join();
commThreadFEM[1].join();
commThreadFEM[2].join();

cout << "All threads done, start debugging----------------" << endl;
sharedMemoryPointers femBodies[3] = {0};

for(int i=0;i<3;i++){
    distrubuteFemSharedMemPointers( shmStru, &femBodies[i], i, bytesForPointer[i] );
}
//Print all nodes
cout << "-----  Nodes   -----" << endl;
for(int i=0;i<headers[0][1];i++){
    for(int j=0;j<3;j++){
        cout <<femBodies[j].nodes[i*2] << ", " << femBodies[j].nodes[i*2+1]<< "; ";
    }
    cout << endl;
}

cout << "-----  Elements   -----" << endl;
for(int i=0;i<headers[0][0];i++){
    for(int j=0;j<3;j++){
        cout <<femBodies[j].elements[i*4] << ", " << femBodies[j].elements[i*4+1]<< ", "<< femBodies[j].elements[i*4+2]<< ", "<< femBodies[j].elements[i*4+3]<< "; ";
    }
    cout << endl;
}

cout << "-----  Deformation   -----" << endl;
for(int i=0;i<headers[0][1];i++){
    for(int j=0;j<3;j++){
        cout <<femBodies[j].deformedNodes[i*2] << ", " << femBodies[j].deformedNodes[i*2+1]<< "; ";
    }
    cout << endl;
}

cout << "-----  vonMieses   -----" << endl;
for(int i=0;i<headers[0][1];i++){
    for(int j=0;j<3;j++){
        cout <<femBodies[j].vonMieses[i] << "; ";
    }
    cout << endl;
}


shmdt(shmStru);

return 0;
}