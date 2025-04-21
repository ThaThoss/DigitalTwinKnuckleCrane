#include <semaphore.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "forceKnuckleToFEMLoop.h"
#include "./mySharedMemory.h"

using namespace std;

int getValuesForCalc(VALUESFORFORCECALC *valuesForForceCalc, sharedMemoryStructForIntegration *shmStru, int bodyNumb){

    sem_wait(&(shmStru->semLock));

        valuesForForceCalc->coord1[0] = shmStru->femZeroPoint[bodyNumb][0];//reaction force 1 (fixed point)
        valuesForForceCalc->coord1[1] = shmStru->femZeroPoint[bodyNumb][1];

        valuesForForceCalc->coord2[0] = shmStru->femSylinderPrePos[bodyNumb][0];//reactoion force 2 (fixed point)
        valuesForForceCalc->coord2[1] = shmStru->femSylinderPrePos[bodyNumb][1];

        valuesForForceCalc->coordcm[0] = shmStru->femCmPoint[bodyNumb][0];//centre of mass
        valuesForForceCalc->coordcm[1] = shmStru->femCmPoint[bodyNumb][1];

        valuesForForceCalc->coordLoad1[0] = shmStru->femJointPos[bodyNumb][0];
        valuesForForceCalc->coordLoad1[1] = shmStru->femJointPos[bodyNumb][1];

        valuesForForceCalc->coordLoad2[0] = shmStru->femSylindertPos[bodyNumb][0];
        valuesForForceCalc->coordLoad2[1] = shmStru->femSylindertPos[bodyNumb][2];

        valuesForForceCalc->massTimesGravity = shmStru->mass[0]*9.81;


    sem_post(&(shmStru->semLock));

    valuesForForceCalc->vec1from2[0] = valuesForForceCalc->coord1[0] - valuesForForceCalc->coord2[0];
    valuesForForceCalc->vec1from2[1] = valuesForForceCalc->coord1[1] - valuesForForceCalc->coord2[1];
    
    valuesForForceCalc->vec2from1[0] = valuesForForceCalc->coord2[0] - valuesForForceCalc->coord1[0];
    valuesForForceCalc->vec2from1[1] = valuesForForceCalc->coord2[1] - valuesForForceCalc->coord1[1];


    valuesForForceCalc->vecCmFrom1[0] = valuesForForceCalc->coordcm[0] - valuesForForceCalc->coord1[0];
    valuesForForceCalc->vecCmFrom1[1] = valuesForForceCalc->coordcm[1] - valuesForForceCalc->coord1[1];

    valuesForForceCalc->vecCmFrom2[0] = valuesForForceCalc->coordcm[0] - valuesForForceCalc->coord2[0];
    valuesForForceCalc->vecCmFrom2[1] = valuesForForceCalc->coordcm[1] - valuesForForceCalc->coord2[1];


    valuesForForceCalc->vecLoad1from1[0] = valuesForForceCalc->coordLoad1[0] - valuesForForceCalc->coord1[0];
    valuesForForceCalc->vecLoad1from1[1] = valuesForForceCalc->coordLoad1[1] - valuesForForceCalc->coord1[1];
    
    valuesForForceCalc->vecLoad1from2[0] = valuesForForceCalc->coordLoad1[0] - valuesForForceCalc->coord2[0];
    valuesForForceCalc->vecLoad1from2[1] = valuesForForceCalc->coordLoad1[1] - valuesForForceCalc->coord2[1];

    valuesForForceCalc->vecLoad2from1[0] = valuesForForceCalc->coordLoad2[0] - valuesForForceCalc->coord1[0];
    valuesForForceCalc->vecLoad2from1[1] = valuesForForceCalc->coordLoad2[1] - valuesForForceCalc->coord1[1];
    
    valuesForForceCalc->vecLoad2from2[0] = valuesForForceCalc->coordLoad2[0] - valuesForForceCalc->coord2[0];
    valuesForForceCalc->vecLoad2from2[1] = valuesForForceCalc->coordLoad2[1] - valuesForForceCalc->coord2[1];

    return 0;
}

int getReactionForceAndSave(sharedMemoryStructForIntegration *shrdMemStruct, VALUESFORFORCECALC *AllvaluesForForceCalc, int NumFEMBodies,int numRBDBodies, double *gravityDir, double loadOnOuter){

    int check = 0;
    double rightHandSide[4] = {0};
    double mgx, mgy;
    
    //{f1x,f2y,f2x,f1y}
    double leftHandSide[4] = {0};

    //get Load On Outer
    double load1x = gravityDir[(numRBDBodies-1)*3]*loadOnOuter;
    double load1y = gravityDir[(numRBDBodies-1)*3 + 2]*loadOnOuter;
    /*
    std::cout << "--getReactionForceAndSave-- load on body " << numRBDBodies-1 << " = [" << load1x << " , "<< load1y <<"]"<< std::endl;
    std::cout << "--getReactionForceAndSave-- gravityDir " << numRBDBodies-1 << " = [" << gravityDir[(numRBDBodies-1)*3] << " , "<< gravityDir[(numRBDBodies-1)*3 +1] <<" , "<< gravityDir[(numRBDBodies-1)*3+2]<<"]"<< std::endl;
    std::cout <<" load on outer = " << loadOnOuter << std::endl;
    */
    double load2x = 0;
    double load2y = 0;


    for(int i=NumFEMBodies-1;i>0;i--){

        saveBodyLoadToSharedMem(shrdMemStruct, load1x, load1y, load2x, load2y, i );

        //std::cout << "--getReactionForceAndSave-- gravityDir " << i+1 << " = [" << gravityDir[(i+1)*3] << " , "<< gravityDir[(i+1)*3 +1] <<" , "<< gravityDir[(i+1)*3+2]<<"]"<< std::endl;
       // std::cout << "--getReactionForceAndSave-- load on body " << i+1 << " = [" << load1x << " , "<< load1y << " , " << load2x <<" , " << load2y<<" ]"<< std::endl;
        mgx = AllvaluesForForceCalc[i].massTimesGravity*gravityDir[(i+1)*3];//gravityDir has numRBDBodies number of inputs (i+1)
        mgy = AllvaluesForForceCalc[i].massTimesGravity*gravityDir[(i+1)*3 + 2];

        //sum of forces in x and y
        rightHandSide[0] = AllvaluesForForceCalc[i].massTimesAcc[0] - load1x - load2x - mgx;
        rightHandSide[1] = AllvaluesForForceCalc[i].massTimesAcc[1] - load1y - load2y - mgy;
        
        //Moment equation done in Matlab
        rightHandSide[2] = load1x*AllvaluesForForceCalc[i].vecLoad1from1[1] - AllvaluesForForceCalc[i].momentZforFEM - 
            load1y*AllvaluesForForceCalc[i].vecLoad1from1[0] + load2x*AllvaluesForForceCalc[i].vecLoad2from1[1] - 
            load2y*AllvaluesForForceCalc[i].vecLoad2from1[0] - AllvaluesForForceCalc[i].massTimesAcc[1]*AllvaluesForForceCalc[i].vecCmFrom1[0] + 
            AllvaluesForForceCalc[i].massTimesAcc[0]*AllvaluesForForceCalc[i].vecCmFrom1[1] - mgx*AllvaluesForForceCalc[i].vecCmFrom1[1] + 
            mgy*AllvaluesForForceCalc[i].vecCmFrom1[0];
        
        rightHandSide[3] = load1x*AllvaluesForForceCalc[i].vecLoad1from2[1] - AllvaluesForForceCalc[i].momentZforFEM - 
            load1y*AllvaluesForForceCalc[i].vecLoad1from2[0] + load2x*AllvaluesForForceCalc[i].vecLoad2from2[1] - 
            load2y*AllvaluesForForceCalc[i].vecLoad2from2[0] - AllvaluesForForceCalc[i].massTimesAcc[1]*AllvaluesForForceCalc[i].vecCmFrom2[0] + 
            AllvaluesForForceCalc[i].massTimesAcc[0]*AllvaluesForForceCalc[i].vecCmFrom2[1] - mgx*AllvaluesForForceCalc[i].vecCmFrom2[1] + mgy*AllvaluesForForceCalc[i].vecCmFrom2[0];
 

        leftHandSide[0] = AllvaluesForForceCalc[i].vec2from1[0];
        leftHandSide[1] = -AllvaluesForForceCalc[i].vec2from1[1];

        leftHandSide[2] = AllvaluesForForceCalc[i].vec1from2[0];
        leftHandSide[3] = -AllvaluesForForceCalc[i].vec1from2[1];
        check = invMatVec(leftHandSide, rightHandSide);
        if(check){
            std::cout << "Error in getReactionForce, invMatCHeck failed. Exiting" << std::endl;
            return 1;
        }
    //Load on next body:
        load1x = -leftHandSide[0]/5;
        load2y = -leftHandSide[1]/5;
        load2x = -leftHandSide[2]/5;
        load1y = -leftHandSide[3]/5;

     /*       //{f1x,f2y,f2x,f1y}
    double leftHandSide[4][4] = {{1,0,1,0},//sum Fx
                                 {0,1,0,1},//sum Fy
                                 {0,r1,r2,0},//sum Mom1
                                 {r3,0,0,r4}};//sum Mom2*/
    }
    //std::cout << "--getReactionForceAndSave-- gravityDir " << 1 << " = [" << gravityDir[(1)*3] << " , "<< gravityDir[(1)*3 +1] <<" , "<< gravityDir[(1)*3+2]<<"]"<< std::endl;
    //std::cout << "--getReactionForceAndSave-- load on body " << 1 << " = [" << load1x << " , "<< load1y << " , " << load2x <<" , " << load2y<<" ]"<< std::endl;
        
    saveBodyLoadToSharedMem(shrdMemStruct, load1x, load1y, load2x, load2y, 0 );//save the tower's force
    return 0;
}


int invMatVec(double leftSide[4], double rightSide[4]){

/*  The system:
    leftHandSide[4][4] = {{1,0,1,0},//sum Fx
                          {0,1,0,1},//sum Fy
                          {0,r1,r2,0},//sum Mom1
                          {r3,0,0,r4}};//sum Mom2
    rigthSide[4] = [v1;v2;v3;v4];

    Solution = inv(leftSide)*rightSide
    Was solved in Matlab using symbolic manipulator. 

    matlabCode:
    syms r1 r2 r3 r4 v1 v2 v3 v4

    mat = [1,0,1,0;0,1,0,1;0,r1,r2,0;r3,0,0,r4];
    F = [v1;v2;v3;v4];
    Sol = simplify(inv(mat)*F);

    Result from Sol below:
*/

    double r1 = leftSide[0];
    double r2 = leftSide[1];
    double r3 = leftSide[2];
    double r4 = leftSide[3];

    double v1 = rightSide[0];
    double v2 = rightSide[1];
    double v3 = rightSide[2];
    double v4 = rightSide[3];

    double divBy = r1*r3 - r2*r4;
    if(divBy==0){
        std::cout << "Error in invMatVec, from forceKnuckleComLoop. divBy was 0" << std::endl;
        return 1;
    }

    leftSide[0] = (r1*v4 + r4*v3 - r1*r4*v2 - r2*r4*v1)/divBy;
    leftSide[1] = (r2*v4 + r3*v3 - r2*r3*v1 - r2*r4*v2)/divBy;
    leftSide[2] = -(r1*v4 + r4*v3 - r1*r3*v1 - r1*r4*v2)/divBy;
    leftSide[3] = -(r2*v4 + r3*v3 - r1*r3*v2 - r2*r3*v1)/divBy;


    return 0;
}


int distributeForceToMesh(sharedMemoryStructForIntegration *shrdMemStruct, sharedMemoryPointers *shmPointers,VALUESFORFORCECALC *AllvaluesForForceCalc, int *numNodesPerForceGroup, int *nforceGroups, int maxNumGoups){

    
    sem_wait(&(shrdMemStruct->semLock));
    int nFemBodies = shrdMemStruct->numFEMBodies;
    int numForce[nFemBodies] = {0};
    memcpy(numForce, shrdMemStruct->numForce,sizeof(int)*3);


    sem_post(&(shrdMemStruct->semLock));

    int numDegFreedom = 2;
    double loadx = 0;
    double loady = 0;


    for(int i=0; i<nFemBodies;i++){

        int forceGroup[numForce[i]] = {0};

        sem_wait(&(shrdMemStruct->semLock));
            memcpy(forceGroup,shmPointers[i].forceGroup,sizeof(int)*numForce[i]);
        sem_post(&(shrdMemStruct->semLock));

        for(int j=0; j<nforceGroups[i];j++){

        sem_wait(&(shrdMemStruct->semLock));
        //Calculated in getReactionForceAndSave
            loadx = *(shrdMemStruct->appliedForceForFEM[i] + j*numDegFreedom + 0);
            loady = *(shrdMemStruct->appliedForceForFEM[i] + j*numDegFreedom + 1);
        sem_post(&(shrdMemStruct->semLock));


        loadx = loadx/(*(numNodesPerForceGroup +i*maxNumGoups + j));
        loady = loady/(*(numNodesPerForceGroup +i*maxNumGoups + j));

            for(int k=0; k<numForce[i];k++){
                if(forceGroup[k]==j){
                    sem_wait(&(shrdMemStruct->semLock));
                    *(shmPointers[i].forceVectors + k*numDegFreedom +0 ) = loadx;
                    *(shmPointers[i].forceVectors + k*numDegFreedom +1 ) = loady;
                    sem_post(&(shrdMemStruct->semLock));
                }
            }
        }
    }
    return 0;
}

int getMassAccandMomZfromShm(sharedMemoryStructForIntegration *shrdMemStruct , VALUESFORFORCECALC AllvaluesForForceCalc[], int numFEMBodies){


	int massAccCounter = 3;//ingore the ship
	int momentCounter = 7;//second direction for bodies. 

	sem_wait(&(shrdMemStruct->semLock));
		for(int i = 0; i<numFEMBodies;i++){
			memcpy(AllvaluesForForceCalc[i].massTimesAcc ,(shrdMemStruct->massTimesAccAndMom + massAccCounter), 3*sizeofDouble);
			massAccCounter += 6;
			AllvaluesForForceCalc[i].momentZforFEM = *(shrdMemStruct->massTimesAccAndMom + momentCounter);
			momentCounter += 6;
		}
	sem_post(&(shrdMemStruct->semLock));



	return 0;
}

int saveRBDValuesBefEnd(sharedMemoryStructForIntegration *shmStru, SHAREDMEMORYPOINTERSRBD *sharedMemoryRBDPointers, VALUESFORFORCECALC AllvaluesForForceCalc[], double *angles, double *accAng, double *craneGlobPos){
    sem_wait(&(shmStru->semLock));
        memcpy(sharedMemoryRBDPointers->initialAngularValues,angles,shmStru->bytesForInitialAng);//get angles, angv.
        memcpy(sharedMemoryRBDPointers->angAcc, accAng, shmStru->numAngles*SZ_DOUBLE);
        memcpy(sharedMemoryRBDPointers->craneGlobalPos, craneGlobPos, 3*SZ_DOUBLE);

        for(int i = 0; i<shmStru->numFEMBodies; i++){

            *(sharedMemoryRBDPointers->appliedLoads + i*4 +0 ) = shmStru->appliedForceForFEM[i][0];
            *(sharedMemoryRBDPointers->appliedLoads + i*4 +1 ) = shmStru->appliedForceForFEM[i][1];
            *(sharedMemoryRBDPointers->appliedLoads + i*4 +2 ) = shmStru->appliedForceForFEM[i][2];
            *(sharedMemoryRBDPointers->appliedLoads + i*4 +3 ) = shmStru->appliedForceForFEM[i][3];

        }
    sem_post(&(shmStru->semLock));

	return 0;
}