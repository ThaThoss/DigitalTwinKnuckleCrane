#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include <stdint.h>
#include <filesystem>
#include <regex>
#include <string>



typedef struct {
    int meshNumber;
    std::string partName;
    std::string filename;
}RBDFILE;

typedef struct{
    int nBodies = 0;
    int numBytes = 0;
    int numDoubleForInitialAng = 0;

    int *axisOfRotations;
    int *spatialFreedom;

    double *centreOfMassCoord;
    double *mass;
    double *inertia;
    double *initialAngularValues;
    double *initialTranslationalValues;
    double *angularAcc;
    double *massTimesAcc;
    double *craneGlobalPos;
    double *appliedLoads;

    char *data;

}RBDDATATOSEND;

std::vector<std::string> findRBDFiles(const std::string& folderPath, const std::string& patternStr);

int readRBDdata(RBDDATATOSEND *dataToSend, const char fileName[]);

int rbdGetMemoryAndSetPointers(RBDDATATOSEND *dataToSend, int axisOfRotation[]);

int readRBDfile(RBDDATATOSEND *dataToSend, FILE *filePointer);

int saveRBDdataToFile(RBDDATATOSEND *dataToSend, const char fileName[],int nFEMBodies);

int freeRbdMemory(RBDDATATOSEND *dataToSend);