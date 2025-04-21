#ifndef FORCETOFEM
#define FORCETOFEM


#include "mySharedMemory.h"


typedef struct{

    double coord1[2] = {0};
    double coord2[2] = {0};

    double coordcm[2] = {0};

    double coordLoad1[2] ={0};
    double coordLoad2[2] ={0};



    double vec1from2[2] = {0};
    double vec2from1[2] = {0};

    double vecCmFrom1[2] = {0};
    double vecCmFrom2[2] = {0};

    double vecLoad1from1[2] = {0};
    double vecLoad1from2[2] = {0};

    double vecLoad2from1[2] = {0};
    double vecLoad2from2[2] = {0};

    double load1[2] = {0};
    double load2[2] = {0};

    double massTimesAcc[3] = {0};
    double momentZforFEM = 0;
    double gravityDirection[2] = {0};

    double massTimesGravity = 0;

    double forceArray[4][4] = {{1,0,1,0},//sum Fx
                               {0,1,0,1},//sum Fy
                               {0,0,1,1},//sum Mom1
                               {1,0,0,1}};//sum Mom2

}VALUESFORFORCECALC;


int getValuesForCalc(VALUESFORFORCECALC *valuesForForceCalc, sharedMemoryStructForIntegration *shmStru, int bodyNumb);

int getReactionForceAndSave(sharedMemoryStructForIntegration *shrdMemStruct, VALUESFORFORCECALC *AllvaluesForForceCalc, int NumFEMBodies,int numRBDBodies, double *gravityDir, double loadOnOuter);

int distributeForceToMesh(sharedMemoryStructForIntegration *shrdMemStruct, sharedMemoryPointers *shmPointers,VALUESFORFORCECALC *AllvaluesForForceCalc, int *numNodesPerForceGroup, int *nforceGroups, int maxNumGoups);

int getMassAccandMomZfromShm(sharedMemoryStructForIntegration *shrdMemStruct ,VALUESFORFORCECALC AllvaluesForForceCalc[], int numFEMBodies);

int invMatVec(double leftSide[4], double rightSide[4]);

int saveRBDValuesBefEnd(sharedMemoryStructForIntegration *shmStru, SHAREDMEMORYPOINTERSRBD *sharedMemoryRBDPointers, VALUESFORFORCECALC AllvaluesForForceCalc[], double *angles, double *accAng, double *craneGlobPos);

#endif