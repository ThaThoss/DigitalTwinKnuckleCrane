
#ifndef MYSHAREDM
#define MYSHAREDM

#include <semaphore.h>  
#include <sys/shm.h>
#include <sys/ipc.h>

#include "./commonDataSizes.h"
#include "./femClient.h"



#define KEY_VAL 42

typedef struct {
   
   sem_t semLock;
   //double changeGlobPos[3] = {0};
   double globalPosPoint[3] = {0};
   //[r,th1,th2,gogo,moveCrane]
   //[double,double,double,int16,int16]
   unsigned char dataForKnuckle[knuckleNBytesToKnu] = {0};
   int numBytesForAngles = 96;// R1 = 9 doubles, angles = 3;
   double angles[3] = {0};
   double angularVel[3] = {0};
   double angularAcc[3] = {0};
   double massTimesAccAndMom[21] = {0};
   int axisOfRotation[4] = {0};
   double gravityDir[12] = {0};
   double loadOnOuter = 0.0;
   double momentZforFEM[3] = {0};
   double appliedForceForFEM[3][4] = {0};
   double R1[9] = {0};
   double mass[3] = {0};
   int16_t gogo = 1;
   int newData = 0;
   int numFEMBodies = 0;
   int numRBDbodies = 0;
   int femDataReady = 0;
   int numBytesBefFEM = 0;
   int numForce[3] = {0};
   int numForceGroups = 0;

   double femJointPos[3][2] = {0};
   double femSylindertPos[3][2] = {0};
   double femZeroPoint[3][2] = {0};
   double femCmPoint[3][2] = {0};
   double femSylinderPrePos[3][2] = {0};
 //  double femLoadPoint1[3][2] = {0};
 //  double femLoadPoint2[3][2] = {0};
   int femNumPointLoads[3] = {0};


   int deformationScale[3] = {0};
   int femBodyScale[3] = {0};

   //FEM data, shared memory can only have one dynamicly allocated memory area.
   char sharedFEMData[];

}sharedMemoryStructForIntegration;

typedef struct
{
   //Fem pointers, must be set for each instance that want to use them.
  
   // numel numnp nmat plane_stress_flag, gravity_flag
   int *header;

   //matl no., E modulus, Poisson Ratio, mass
   double *mat_E_Poiss_Mass;

   //connectivity   
   int *elements;

   //matl No
   int* elementalMaterialNumber;

   //node no. coordinates
   double *nodes;

   //prescribed displacement x: node  disp value
   int *xLocedNodes;
   double *xLockedNodesCoord;

   //prescribed displacement y: node  disp value
   int *yLocedNodes;
   double *yLockedNodesCoord;

   //node with point load and load vector in x,y
   int *forceNodes;
   double *forceVectors;
   int *forceGroup;

   // deformedNodes
   double *deformedNodes;
   // vonMieses stress
   double *vonMieses;

} sharedMemoryPointers;



int calcBytesNeededForFEM(int numel, int numnp, int nmat, int plane_stress_flag, int gravity_flag);
int getFEMHeader(FEMDATATOSEND *dataToSend,sharedMemoryStructForIntegration *shmStruct, int femBodyNumber);
int getFEMHeaders(int headers[][5], sharedMemoryStructForIntegration *shmStruct, int nFEMBodies);
int distrubuteFemSharedMemPointers(sharedMemoryStructForIntegration *shrdMemStruct,sharedMemoryPointers *sharedPointers, int femBodyNumber, int bytesForPointer );

int saveFemData(sharedMemoryStructForIntegration *shrdMemStruct, FEMDATATOSEND *femData, sharedMemoryPointers *sharedPointers, int femBodyNumber, int bytesForPointer );
int saveDeformationToSharedMemory(sharedMemoryStructForIntegration *shrdMemStruct, FEMDATATOSEND *femData, sharedMemoryPointers *sharedPointers, double *nodalDeformation);
int saveVonMiesesToSharedMemory(sharedMemoryStructForIntegration *shrdMemStruct, FEMDATATOSEND *femData, sharedMemoryPointers *sharedPointers, double *vonMieses);
int getForceFromShardeMem(double *Force, double* gravityDirection, sharedMemoryStructForIntegration *shrdMemStruct,sharedMemoryPointers *sharedPointers, int numForceNodes, int bodyNum);
int getAnglesFromSharedMem(sharedMemoryStructForIntegration *shrdMemStruct, double *angles);

int calcGravityDirAndSave(sharedMemoryStructForIntegration *shrdMemStruct, double * angles, double* gravityDir);
int isGogo(sharedMemoryStructForIntegration *shrdMemStruct); //returns 1 for yes, 0 for no.
int dontGogo(sharedMemoryStructForIntegration *shrdMemStruct);
double returnLoadOnOuter(sharedMemoryStructForIntegration *shrdMemStruct);

int saveBodyLoadToSharedMem(sharedMemoryStructForIntegration *shrdMemStruct, double load1x, double load1y, double load2x, double load2y, int bodynum );

#endif