#ifndef FEMCLIENT
#define FEMCLIENT


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <semaphore.h>
#include <filesystem>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>

#include "./sendReceive.h"
//#include "./../lib/mySharedMemory.h"
//#include "./SendReceiveFemQD.h"

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
	double *vonMieses;
	double *deformation;
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
	int numBytesForFinalFemReciever;
	int numBytesToSend;
	char *allTheData;
	char *finalFemDataToRecieve;
} FEMDATATOSEND;



typedef struct {
    int meshNumber;
    std::string partName;
    std::string filename;
}FEMFILE;

std::vector<FEMFILE> findFemFiles(const std::string& folderPath, const std::string& patternStr);

int femReadMesh(FEMDATATOSEND *dataToSend, const char fileName[]);

int calcFemDataSize(FEMDATATOSEND *dataToSend);

int calcNumBytesToSend(FEMDATATOSEND *dataToSend);

int qdInitialReader(FEMDATATOSEND *dataToSend, FILE *femDatFile);

int qdClientMemory(FEMDATATOSEND *dataToSend);

int qdClientDistributePointers(FEMDATATOSEND *dataToSend);

int qdClientReader(FEMDATATOSEND *dataToSend,FILE *femDatFile);

int qdClientInitialSender(FEMDATATOSEND *dataToSend,int sockfd);

int qdClientSender(FEMDATATOSEND *dataToSend, int sockfd);

int qdClientWriter( FEMDATATOSEND *dataToSave, const char name[]);

int freeDataToSend(FEMDATATOSEND *dataToSend);

int printElements(FEMDATATOSEND *dataToSend, int bodyNum);

#endif