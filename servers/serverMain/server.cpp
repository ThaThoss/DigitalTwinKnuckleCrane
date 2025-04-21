#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>


#include "./../../lib/sendReceive.h"
#include "./../../lib/mySharedMemory.h"
#include "./../../lib/forceKnuckleToFEMLoop.h"
#include "./../../lib/commonDataSizes.h"
#include "./../../lib/myPortsAndIP.h"


using namespace std;

int main(int argc, char *argv[]){

    int pidInt, pidOut;
    double zerosForShm[9] = {0};
    int16_t intsForShm[2] = {0};
    int nFEMBodies = 3;
    int nBodies[2] = {0};

    cout <<"Client connected, responding 200" << endl;

    int clientSoc = atoi(argv[1]);
    write(clientSoc,"200",sizeof("200"));
    
    //read num bodies, [RBD, FEM]
    sleep(1);
    ReceiveNInts(clientSoc,nBodies,sizeof(nBodies));
    cout <<"Recieved number of bodies, RBD: " <<nBodies[0]<<", fem: " << nBodies[1] << endl;

    // get rotaional axis for RBD
    sleep(2);
    int rotationalAxis[nBodies[0]] = {0};
    ReceiveNInts(clientSoc,rotationalAxis,nBodies[0]*sizeof(int));

    //recieve FEM data
    int headersNew[nBodies[1]*8] = {0};
    int numBytesHeader = 8*nBodies[1]*sizeof(int);

    sleep(2);
    cout << "Recieving headers"<< endl;fflush(stdout);
    if(ReceiveNInts(clientSoc,headersNew,numBytesHeader)){
        cout << "someting wrong with Recieve Headers" << endl;
        return 1; 
    };

    for(int i=0; i<nBodies[1];i++){
        cout <<"HeadersNew["<<i<<"], numEl = "<<headersNew[i*8 +0]<<", numNodPnt = "<<headersNew[i*8 +1]<<", numMat = "<<headersNew[i*8 +2]<<", Flag = "<<headersNew[i*8 +3]<<", gravity = "<<headersNew[i*8 +4]<< endl;
        cout << "numFIxX "<<headersNew[i*8 +5]<<" numFixY "<<headersNew[i*8+6] << " numFOrce " << headersNew[i*8+7]<<endl;
        fflush(stdout);
    }


    //numel numnp nmat plane_stress_flag, gravity_flag
    //196 	250   1   1   1 // dog file;
    //int headers[nFEMBodies][8] = {{2460, 2566,   1,   1,   1, 41, 41, 7},{2220, 2397,   1,   1,   1, 42, 42, 40 },{1328, 1468,   1,   1,   1, 36, 36, 25}};
    int deformationScale[3] = {200,3,3};
    int bodyScale[3] = {1,2,2};
    double femZeroPoint[3][2] = {{0,0},{0.2,0.5},{2.07,1.1}};//Where the previous body is attached, used in serverexternal for positioning the fem bodies
    double femSylinderPrePos[3][2] = {{0,0},{1.7,0.006},{0.25,0.97}};//Where the fixed area of the previous body's cylinder comes in
    double femSylindertPos[3][2] = {{4.0,2.75},{2.887,0.0041},{0,0}};//Where the force of the next body's cylinder comes in
    double femCmPoint[3][2] = {{2.5,3.0},{2.3,0.325},{1.225,0.553}}; //Coordinate of the centre of mass.
    double femJointPos[3][2] = {{2.4,5.6},{4.4,0.5},{0.25,0.0456}};//Where the next body is attached,  used in serverexternal for positioning the fem bodies
    double angles[3] = {0,0,0};
    //int axisOfRotation[4] = {4,3,2,2};

    int numBytesForSharedMemory = 0;
    numBytesForSharedMemory += sizeof(sharedMemoryStructForIntegration);//Size of known sizes in struct
    numBytesForSharedMemory += sizeof(int)*8*nFEMBodies;// header for FEM files, 8 ints each;
    int nBytesRBD = calcBytesNeededForRBD( nBodies[0],nBodies[1], rotationalAxis);// bytes for RBD
    numBytesForSharedMemory += nBytesRBD;

    for(int i=0;i<nFEMBodies;i++){
        numBytesForSharedMemory += calcBytesNeededForFEM( headersNew[i*8 + 0],  headersNew[i*8 + 1],  
                                    headersNew[i*8 + 2],  headersNew[i*8 + 3],  headersNew[i*8 + 4]);
    }

    cout << "numBytesForSharedMemory = " << numBytesForSharedMemory << endl;


    /*Shared memory section:*/
	sharedMemoryStructForIntegration *shmStru;
	key_t key = ftok("./../lib/sharedMemoryFile",KEY_VAL);
    

    // shmget returns an identifier in shmid
    int shmid = shmget(key,numBytesForSharedMemory,0644|IPC_CREAT);
    //cout<<"keyDT = " << key << ", numBytesForSharedMemory : " << numBytesForSharedMemory << endl;
    if (shmid == -1) {
      perror("Shared memory shmget");
      return 1;
    }

    // shmat to attach to shared memory
    shmStru = (sharedMemoryStructForIntegration*)shmat(shmid,NULL,0);
    if (shmStru == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }
    sem_init(&(shmStru->semLock),1,1);
    sem_wait(&(shmStru->semLock));
    //Recieve axis again, easier to send everything.
    ReceiveNChars(clientSoc,(shmStru->sharedFEMData + numBytesHeader),nBytesRBD );
    cout << "Recieved RBD data"<< endl;
    shmStru->numBytesForRBD = nBytesRBD;
    shmStru->numBytesForHeader = numBytesHeader;

    SHAREDMEMORYPOINTERSRBD sharedMemoryRBDPointers;
    int semProtect = 0;

    shmStru->bytesForInitialAng = calcNumBytesForInitialAngRBD(rotationalAxis, nBodies[0]);
  

    distributeRbdMemPointers( shmStru,&sharedMemoryRBDPointers, rotationalAxis, semProtect );
    
        memcpy(shmStru->R1,zerosForShm,SZ_DOUBLE*9);
        shmStru->R1[0] = 1.0;
        shmStru->R1[4] = 1.0;
        shmStru->R1[8] = 1.0;
        shmStru->mass[0] = 160;
        shmStru->mass[1] = 150;
        shmStru->mass[2] = 150;
        shmStru->gogo = 1;
        shmStru->loadOnOuter = 1000.0;
        shmStru->numBytesForAngles = 96;
        shmStru->femDataReady = 0;
        memcpy(shmStru->angles,zerosForShm,SZ_DOUBLE*3);
        memcpy(shmStru->globalPosPoint,zerosForShm,SZ_DOUBLE*3);
        memcpy(shmStru->dataForKnuckle,zerosForShm,SZ_DOUBLE*3);
        intsForShm[0] = shmStru->gogo;
        memcpy((shmStru->dataForKnuckle+SZ_DOUBLE*3),intsForShm,SZ_INT16*2);
        shmStru->numFEMBodies = nFEMBodies;
        shmStru->numRBDbodies = 4;
        int numBytesToJump = 0;
        int femNumPointLoads[3] = {2,2,1};

        for(int i=0; i<nFEMBodies;i++){
            memcpy(shmStru->sharedFEMData + numBytesToJump,(headersNew + i*8),8*sizeof(int));  
            memcpy(shmStru->femZeroPoint[i],femZeroPoint[i], sizeofDouble*2); 
            memcpy(shmStru->femCmPoint[i],  femCmPoint[i],   sizeofDouble*2);  
            memcpy(shmStru->femJointPos[i], femJointPos[i], sizeofDouble*2);   
            memcpy(shmStru->femSylindertPos[i], femSylindertPos[i], sizeofDouble*2); 
            memcpy(shmStru->femSylinderPrePos[i], femSylinderPrePos[i], sizeofDouble*2); 
            numBytesToJump += sizeof(int)*8;
            shmStru->deformationScale[i] = deformationScale[i];
            shmStru->angles[i] = angles[i];
            shmStru->femBodyScale[i] = bodyScale[i];
            shmStru->femNumPointLoads[i] = femNumPointLoads[i];
            shmStru->numForce[i] = headersNew[i*8 + 7];
           
    
        }
        memcpy(shmStru->axisOfRotation,rotationalAxis,nBodies[0]*sizeof(int));
        /*
        for(int i=0;i<nBodies[0];i++){
            shmStru->axisOfRotation[i] = rotationalAxis[i];
        }*/
        shmStru->numBytesBefFEM = numBytesToJump + nBytesRBD;

        int numBytesForFEMBodie[nBodies[1]+1] = {0};
        int bytesForPointer[nBodies[1]+1] = {0};
        bytesForPointer[0] = numBytesToJump + nBytesRBD;

        for(int i=0;i<nBodies[1];i++){
            cout << "recieving body: "<< i+1 << ", out of: "<< nBodies[1]<<endl;
            numBytesForFEMBodie[i+1] = calcFemNumBytesFromClient( (headersNew + 8*i));
            cout << "numBytesToRecieve: " << numBytesForFEMBodie[i+1] << endl;
            bytesForPointer[i+1] = bytesForPointer[i] + calcBytesNeededForFEM( *(headersNew +i*8 + 0),  *(headersNew +i*8 + 1),  *(headersNew +i*8 + 2),  *(headersNew +i*8 + 3), *(headersNew +i*8 + 4));
            ReceiveNChars(clientSoc,(shmStru->sharedFEMData + bytesForPointer[i]), numBytesForFEMBodie[i+1]);
            cout << endl;

        }      
        cout << "done recieving mesh" <<endl;



    sem_post(&(shmStru->semLock));
    std::cout << "gogo in server: "<< shmStru->gogo << std::endl;

    MYPORTSANDIP myPorts;

    
    pidInt = fork();
    if (pidInt<0){
        char msg[] = "error on fork in serverInternal";
        myError(msg);
    }

    if(pidInt == 0){
        char femPortIDInternal[10], shmID[10], knucklePortIDInternal[10], socketClient[20];
        sprintf(femPortIDInternal,"%s",myPorts.Port_Internal_FEM);
        sprintf(knucklePortIDInternal,"%s",myPorts.Port_Internal_KNUCKLE);
        sprintf(shmID,"%d",shmid);
        sprintf(socketClient,"%s",argv[1]);

        char programName[] = "./serverInternal/serverInternal.out";
        char *args[]={programName,femPortIDInternal,knucklePortIDInternal, shmID, socketClient, NULL};

        if (execv(args[0],args) == -1) {
            // If execv returns, it means it failed
            std::cerr << "execv serverInternal failed: " << strerror(errno) << std::endl;
            return 1;  // Exit child process with an error code
        }
       
        exit(0);

    }else{
 
        pidOut = fork();
        if(pidOut==0){
            char portID[10], shmID[10], femPortID[10];
            sprintf(portID,"%s",myPorts.Port_External_Knuckle);
            sprintf(shmID,"%d",shmid);
            sprintf(femPortID,"%s",myPorts.Port_External_FEM);

            char programNameAnimate[] = "./serverExternal/serverExternal.out";
            char *argsAni[]={programNameAnimate, portID,shmID,femPortID, NULL};

            if (execv(argsAni[0],argsAni) == -1) {
                // If execv returns, it means it failed
                std::cerr << "execv serverExternal failed: " << strerror(errno) << std::endl;
                return 1;  // Exit child process with an error code
            }

        }
    }

    cout << "Servers forked" << endl;
    int status;
    // Wait for child1
    if (waitpid(pidInt, &status, 0) > 0) {
        if (WIFEXITED(status)) {
            std::cout << "Internal Server exited with status " << WEXITSTATUS(status) << "\n";
        } else {
            std::cout << "Internal Server did not exit successfully\n";
        }
    }

    // Wait for child2
    if (waitpid(pidOut, &status, 0) > 0) {
        if (WIFEXITED(status)) {
            std::cout << "External Server exited with status " << WEXITSTATUS(status) << "\n";
        } else {
            std::cout << "External Server did not exit successfully\n";
        }
    }
    
    sem_destroy(&(shmStru->semLock)); 
    shmdt(shmStru); 
    shmctl(shmid,IPC_RMID,NULL);
    cout << "Semaphore destroyed, shared memory cleared, servers are shuting down." << endl;


}

