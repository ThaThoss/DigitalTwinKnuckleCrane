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
#include <semaphore.h>

#include "./../../lib/sendReceive.h"
#include "./../../lib/mySharedMemory.h"
#include "./../../lib/commonDataSizes.h"
#include "./../../lib/femClient.h"
#include "./../../lib/myPortsAndIP.h"
#include "./../../lib/forceKnuckleToFEMLoop.h"
#include "./controller/xBoxComLoop.h"
#include "./knuckle/knuckleComLoop.h"
#include "./FEM/femComLoop.h"

using namespace std;

int main(int argc, char *argv[]){
    
    int clCount=0, shmid, FEMClients = 0, femPort, KnucklePort, clientSocket;
     
    char names[3][32] = {{"./mesh/TowerMesh.dat"},{"./mesh/mainBoomMesh.dat"},{"./mesh/outerBoomMesh.dat"}};

    MYPORTSANDIP myPortsAndIP;
    char femIp[16] = "127.0.0.1";
   

    thread controllThread; 
    thread commThreadRBD;	


    if (argc >= 4) {
     	femPort = atoi(argv[1]);
        KnucklePort = atoi(argv[2]);
        shmid  = atoi(argv[3]);
        clientSocket = atoi(argv[4]);
    }else{
        char msg[] = {"Socket for fem and knuckle and shmid needed as input nr 1, 2 and 3"};
        cout << "argc in internal was: " << argc << endl;
	    myError(msg);
    }

    // shmat to attach to shared memory
    sharedMemoryStructForIntegration *shmStru; 
    shmStru = (sharedMemoryStructForIntegration*)shmat(shmid,NULL,0);
    if (shmStru == (void *) -1) {
      perror("Shared memory attach");
      return 1;
    }
    //Get headers from shared memory
    //numel numnp nmat plane_stress_flag, gravity_flag

    sem_wait(&(shmStru->semLock));
        int nFEMBodies = shmStru->numFEMBodies;
        int nRBDBodies = shmStru->numRBDbodies;
        int numBytesBefFEM = shmStru->numBytesBefFEM;
    sem_post(&(shmStru->semLock));

    int headers[nFEMBodies][8] = {0};
    getFEMHeaders( headers, shmStru,  nFEMBodies);

    cout << "nFEMBodies Internal = "<< nFEMBodies << endl;
    for(int i=0;i<nFEMBodies;i++){
        for(int j=0;j<8;j++){
            cout << headers[i][j] << " , ";
        }cout << endl;
    }

    cout << "Internal Server Shared memory attatched, gogo was: " << shmStru->gogo << endl;

    controllThread = thread(xBoxComLoop,clientSocket,shmStru);
    cout << "XBoxController Client connected" << endl;
   
    int bytesForPointer[4] = {0};
    bytesForPointer[0] = numBytesBefFEM ;
    thread *commThreadFEM[nFEMBodies];

    sem_t semSyncWait;  // Synchronization semaphore
    sem_t semSyncGo;  // Synchronization semaphore
    sharedMemoryPointers shmPointers[nFEMBodies];

    for(int i=0;i<nFEMBodies;i++){    	
        distrubuteFemSharedMemPointers( shmStru, &shmPointers[i], i, bytesForPointer[i] );
        commThreadFEM[i] = new thread(femComLoop, i, bytesForPointer[i], shmid, femIp, names[FEMClients], femPort, &semSyncWait, &semSyncGo);
        bytesForPointer[i+1] = bytesForPointer[i] + calcBytesNeededForFEM( headers[i][0],  headers[i][1],  headers[i][2],  headers[i][3],  headers[i][4]);
        FEMClients++;
        cout << "FEM thread num " << i << " started" << endl;
        clCount++;
    }

    commThreadRBD = thread(knuckleComLoop,shmid, KnucklePort , myPortsAndIP.ipKnuckle);

    cout << "RBD thread started" << endl;
/*               
    while(FEMClients + RBDClients +Controller < 2){

        clientSocket[clCount] = accept(serverSocket, (struct sockaddr *) &cli_addr, &clilen);
        cout << "First Client accepted" << endl;

        if (clientSocket[clCount] < 0){
            char msg[] = "ERROR on accept first client in internal Server";
            myError(msg);
        }

        write(clientSocket[clCount],"200",sizeof("200"));
        if(ReceiveInt32(clientSocket[clCount], &communicationCheck)){
            char msg[] = "Error in RecieveInt32 in digitaltwincommunication loop";
            myError(msg);
        }
        if(communicationCheck == 1 && RBDClients <1){
            
            commThreadRBD = thread(knuckleComLoop,clientSocket[clCount],shmStru);
            RBDClients++;
            cout << "RBD Client connected" << endl;
            clCount++;

        }else if(communicationCheck == 3 && Controller<1){
            	
            controllThread = thread(xBoxComLoop,clientSocket[clCount],shmStru);
            Controller++;
            cout << "XBoxController Client connected" << endl;
            clCount++;
        }

    }
*/

    cout << "Internal loops up and running, internal server up and running" << endl;
    int gogo = isGogo(shmStru);
    int check = 0;
    double *angles = (double*)calloc(12,sizeofDouble);
    double gravityDir[12] = {0};
   // cout << "In server Internal, be load on outer" << endl;fflush(stdout);
    double loadOnOuter = returnLoadOnOuter(shmStru);
    VALUESFORFORCECALC valuesForForceCalc[nFEMBodies];

    for(int i=0;i<nFEMBodies;i++){
  //cout << "In server Internal, bef getValuesForCalc nr "<<i << endl;fflush(stdout);
        getValuesForCalc(&valuesForForceCalc[i], shmStru,i);

    }
    for (int i = 0; i < nFEMBodies; i++) {
        sem_wait(&semSyncWait);
    }


    // do force stuff
    int nforceGroups[nFEMBodies] = {0};
    int preGroupNum = 0;
    int maxGrupNum = 0;
    for(int j=0; j<nFEMBodies;j++){
        sem_wait(&shmStru->semLock);

        for (int i=0; i<shmStru->numForce[j]; i++){
            if(preGroupNum <= shmPointers[j].forceGroup[i]){
                preGroupNum = shmPointers[j].forceGroup[i]+1;
            }     
        }
        sem_post(&shmStru->semLock);
        nforceGroups[j] = preGroupNum; 
        if(maxGrupNum<preGroupNum){
            maxGrupNum = preGroupNum;
        }


        preGroupNum = 0;
    }

    //cout << "In server Internal, numNodesPerForceGroup" << endl;fflush(stdout);
    int numNodesPerForceGroup[nFEMBodies*maxGrupNum] = {0};


    for(int j=0; j<nFEMBodies;j++){
        for(int k=0; k<nforceGroups[j];k++){
            sem_wait(&shmStru->semLock);
            for(int i=0; i<shmStru->numForce[j];i++){
                if(k==shmPointers[j].forceGroup[i]){
                    *(numNodesPerForceGroup + j*maxGrupNum + k ) += 1;
                }
            }
            sem_post(&shmStru->semLock);
        }
    }


    //cout << "In server Internal, sem_post" << endl;fflush(stdout);
    for (int i = 0; i < nFEMBodies; i++) {
        sem_post(&semSyncGo);
    }

    //cout << "In server Internal, while gogo" << gogo<<endl;fflush(stdout);
    while(gogo){

        cout << "In com loop Internal, waiting for loops to finish" << endl;fflush(stdout);
        for (int i = 0; i < nFEMBodies; i++) {
            sem_wait(&semSyncWait);
        }
        //cout << "In com loop Internal, done waiting, calc forces" << endl;

        getAnglesFromSharedMem(shmStru, angles);
        calcGravityDirAndSave( shmStru, angles, gravityDir);

        //cout << "In com loop Internal, get mass and FOrce " << endl;
        check = getMassAccandMomZfromShm(shmStru, valuesForForceCalc, nFEMBodies);//acc and Mmm      
        check = getReactionForceAndSave(shmStru, valuesForForceCalc, nFEMBodies, nRBDBodies, gravityDir, loadOnOuter );
      

  
        if(check){
            std::cout << "Error in getReactionForce, shutting down from forceKnuckleToFEMLoop" << std::endl;
            dontGogo( shmStru);
        }
        //cout << "In com loop Internal, save to mesh " << endl;
        distributeForceToMesh(shmStru, shmPointers, valuesForForceCalc, numNodesPerForceGroup, nforceGroups, maxGrupNum);

        //cout << "In com loop Internal, start loops " << endl;
        for (int i = 0; i < nFEMBodies; i++) {
            sem_post(&semSyncGo);
        }

        gogo = isGogo(shmStru);
    }
    cout << "gogo in Internal was " << gogo << "shutting down" << endl;

    free(angles);

    for(int i=0;i<nFEMBodies;i++){
        commThreadFEM[i]->join();
    }


    commThreadRBD.join();
    controllThread.join();


    shmStru->gogo = 0;
    shmdt(shmStru); 
    sem_destroy(&semSyncWait);
    sem_destroy(&semSyncGo);
    cout << "threads joined, internal server shutting down" << endl;
    
}

