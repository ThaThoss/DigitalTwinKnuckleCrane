#include <semaphore.h>  
#include <sys/shm.h>
#include <sys/ipc.h>
#include <iostream>
#include <cmath>

#include "./commonDataSizes.h"
#include "./femClient.h"
#include "./mySharedMemory.h"
#include "./forceKnuckleToFEMLoop.h"




int calcBytesNeededForFEM(int numbElements, int numNodalPoints, int numMaterial, int plane_stress_flag, int gravity_flag){

   int bytesNeeded = 0;

// Assuming plane strain/stress, 2 deg of freedom, quad mesh
   int nodalDegreeOfFreedom = 2;
   
   int doubles = 0;
   int integers = 0;
   int degOfFreedom = numNodalPoints*nodalDegreeOfFreedom;

	int dubSizeForce = degOfFreedom;
	int dubSizeCoord = numNodalPoints*numSpatialDim;
	int dubSizeMatProp = 3 * numMaterial;
	int dubSizeDisplacedNodes = 2*numNodalPoints + 2;
    int dubSizeResults = degOfFreedom + numNodalPoints;//Result space
	doubles = dubSizeForce + dubSizeCoord + dubSizeDisplacedNodes +dubSizeMatProp + dubSizeResults;

/* Size of memory needed for the integers */
	int intSizeConnect = numbElements*nodesPerElement;
	int intSizeElementalMaterial = numbElements;
	int intSizeFixedNodes = 2*numNodalPoints+2;
	int intSizePreForce = numNodalPoints*2+1;	
	integers = intSizeConnect+intSizeElementalMaterial + 
				       intSizeFixedNodes+intSizePreForce;



    bytesNeeded = doubles*sizeof(double) + integers*sizeof(int);

    return bytesNeeded;
}

int getFEMHeader(FEMDATATOSEND *dataToSend, sharedMemoryStructForIntegration *shmStruct, int femBodyNumber){

	sem_wait(&(shmStruct->semLock));
		//numel numnp nmat plane_stress_flag, gravity_flag
		int *femHeader = (int*)(shmStruct->sharedFEMData + sizeof(int)*5*femBodyNumber );
		dataToSend->numEl = femHeader[0];
		dataToSend->numNodPnt = femHeader[1];
		dataToSend->numMaterial = femHeader[2];
		dataToSend->PlaneStressFlag = femHeader[3];
		dataToSend->gravity_Flag = femHeader[4];
	sem_post(&(shmStruct->semLock));

	//std::cout << "Header " << femBodyNumber << " in getFEMHeader (FEM Loop)" <<std::endl;
    //std::cout << dataToSend->numEl << " , " << dataToSend->numNodPnt << " , " << dataToSend->numMaterial << " , " << dataToSend->PlaneStressFlag << " , " << dataToSend->gravity_Flag << std::endl; 


	return 0;
}
int getFEMHeaders(int headers[][5], sharedMemoryStructForIntegration *shmStruct, int nFEMBodies){

sem_wait(&(shmStruct->semLock));
for(int i=0;i<nFEMBodies;i++){
	//numel numnp nmat plane_stress_flag, gravity_flag
	int *femHeader = (int*)(shmStruct->sharedFEMData + sizeof(int)*5*i );
	headers[i][0] = femHeader[0];//numEl
	headers[i][1] = femHeader[1];//numNodPnt
	headers[i][2] = femHeader[2];//numMaterial
	headers[i][3] = femHeader[3];//PlaneStressFlag
	headers[i][4] = femHeader[4];//gravity_Flag
	//std::cout << "Header " << i << " in getFEMHeaders" <<std::endl;
    //std::cout << headers[i][0] << " , " << headers[i][1] << " , " << headers[i][2] << " , " << headers[i][3] << " , " << headers[i][4] << std::endl; 
}

sem_post(&(shmStruct->semLock));


	return 0;
}




int distrubuteFemSharedMemPointers(sharedMemoryStructForIntegration *shrdMemStruct,sharedMemoryPointers *sharedPointers, int femBodyNumber, int bytesForPointer ){


sem_wait(&(shrdMemStruct->semLock));


	int sharedMemoryCounter = femBodyNumber*5*sizeof(int);
	//Headers stored together first, 5 ints * number of FEM bodies.
	//numel numnp nmat plane_stress_flag, gravity_flag
	sharedPointers->header = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	int numberOfElements = *(sharedPointers->header);
	int numNodes = *(sharedPointers->header+1);
	int numMatProperties = *(sharedPointers->header+2);
	int plane_stress_flag = *(sharedPointers->header+3);
	int gravity_flag = *(sharedPointers->header+4);

	//Jump to the start of this bodies data, first is material properties;
	sharedMemoryCounter = bytesForPointer;
	sharedPointers->mat_E_Poiss_Mass = (double*)(shrdMemStruct->sharedFEMData + sharedMemoryCounter );
	sharedMemoryCounter += 3*sizeofDouble*numMatProperties;

	//Second part is the elements 
	sharedPointers->elements = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numberOfElements*nodesPerElement*sizeof(int);//first int in header is number of elements

	//Third is the elements material number
	sharedPointers->elementalMaterialNumber = (int*)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numberOfElements*sizeof(int);//first int in header is number of elements

	//Nodal coordinates
	sharedPointers->nodes = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numNodes*numSpatialDim*sizeofDouble;//nodal points is the second int in header

	//prescribed displacement x: 
  	sharedPointers->xLocedNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);

   	sharedPointers->xLockedNodesCoord = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeofDouble;

	//prescribed displacement y: 
  	sharedPointers->yLocedNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);

   	sharedPointers->yLockedNodesCoord = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeofDouble;

	//Nodes with force: 
  	sharedPointers->forceNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);

	//Force nodes group number:
	sharedPointers->forceGroup = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes)*sizeof(int);

	//Force vectors:
   	sharedPointers->forceVectors = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeofDouble;

	//Resulting Nodal coordinates, deformed mesh
	sharedPointers->deformedNodes = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numNodes*numSpatialDim*sizeofDouble;//nodal points is the second int in header

	//Resulting von Mieses stress
	sharedPointers->vonMieses = (double*)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numNodes*sizeof(double);//first int in header is number of elements
sem_post(&(shrdMemStruct->semLock));
	return 0;
}


int saveFemData(sharedMemoryStructForIntegration *shrdMemStruct,FEMDATATOSEND *femData,sharedMemoryPointers *sharedPointers, int femBodyNumber, int bytesForPointer ){

sem_wait(&(shrdMemStruct->semLock));

	int numberOfElements = *(sharedPointers->header);
	int numNodes = *(sharedPointers->header+1);
	int numMatProperties = *(sharedPointers->header+2);
	int plane_stress_flag = *(sharedPointers->header+3);
	int gravity_flag = *(sharedPointers->header+4);

	shrdMemStruct->numForce[femBodyNumber] = femData->numForce;

	//Copy in all aterial properties, 3 dubles per material
	memcpy(sharedPointers->mat_E_Poiss_Mass,femData->doubles.materialProperties,3*sizeofDouble*numMatProperties);

	//Copy elements over,4 ints per element
	memcpy(sharedPointers->elements,femData->integers.connect,numberOfElements*nodesPerElement*sizeof(int));

	//Copy elementalMaterialNumber, 1 int per element
	memcpy(sharedPointers->elementalMaterialNumber,femData->integers.el_matl,numberOfElements*sizeof(int));

	//Copy nodes, 2 doubles per node
	memcpy(sharedPointers->nodes,femData->doubles.coord,2*sizeofDouble*numSpatialDim*numNodes);

	//Copy Fixed X nodes, one int per locked node.
	memcpy(sharedPointers->xLocedNodes,femData->integers.fixedNodesX,femData->numFixx*sizeof(int));
	//2 doubles per node
	memcpy(sharedPointers->xLockedNodesCoord,femData->doubles.displacedNodesX,femData->numFixx*2*sizeofDouble);

	//Copy Fixed Y nodes, one int per locked node.
	memcpy(sharedPointers->yLocedNodes,femData->integers.fixedNodesY,femData->numFixy*sizeof(int));
	//2 doubles per node
	memcpy(sharedPointers->yLockedNodesCoord,femData->doubles.displacedNodesY,femData->numFixy*2*sizeofDouble);

	//Copy force nodes, one int per force vector
	memcpy(sharedPointers->forceNodes,femData->integers.nodesWithForce,femData->numForce*sizeof(int));
	
	//Copy forceGroup for nodes, one int per force vector
	memcpy(sharedPointers->forceGroup,femData->integers.forceGroup,femData->numForce*sizeof(int));

	//2 doubles per node
	memcpy(sharedPointers->forceVectors,femData->doubles.force,femData->numForce*2*sizeofDouble);

	memset(sharedPointers->deformedNodes, 0, numNodes*numSpatialDim*sizeofDouble);
	memset(sharedPointers->vonMieses, 0, numNodes*sizeofDouble );

sem_post(&(shrdMemStruct->semLock));

	return 0;
}

int saveDeformationToSharedMemory(sharedMemoryStructForIntegration *shrdMemStruct, FEMDATATOSEND *femData, sharedMemoryPointers *sharedPointers, double* nodalDeformation){

	sem_wait(&(shrdMemStruct->semLock));
		memcpy(sharedPointers->deformedNodes,nodalDeformation,femData->numNodPnt*numSpatialDim*sizeofDouble);
	sem_post(&(shrdMemStruct->semLock));

	return 0;
}

int saveVonMiesesToSharedMemory(sharedMemoryStructForIntegration *shrdMemStruct, FEMDATATOSEND *femData, sharedMemoryPointers *sharedPointers, double* vonMieses){

	sem_wait(&(shrdMemStruct->semLock));
		memcpy(sharedPointers->vonMieses,vonMieses,femData->numNodPnt*sizeofDouble);
	sem_post(&(shrdMemStruct->semLock));

	return 0;
}





int getForceFromShardeMem(double *Force, double* gravityDirection, sharedMemoryStructForIntegration *shrdMemStruct,sharedMemoryPointers *sharedPointers, int numForceNodes, int bodyNum){

	sem_wait(&(shrdMemStruct->semLock));
	//2 doubles per node
	memcpy( Force, sharedPointers->forceVectors,numForceNodes*2*sizeofDouble);
		gravityDirection[0] = *(shrdMemStruct->gravityDir + 3*bodyNum);//ignore direction into the page(2 dir)
		gravityDirection[1] = *(shrdMemStruct->gravityDir + 3*bodyNum + 2);
	sem_post(&(shrdMemStruct->semLock));
	//printf("Gravity dir from body %d, is [ %lf, %lf ]\n", bodyNum,gravityDirection[0],gravityDirection[1] );
return 0;
}


int calcGravityDirAndSave(sharedMemoryStructForIntegration *shrdMemStruct, double * angles, double* gravityDir){




	sem_wait(&(shrdMemStruct->semLock));
		int numRBDBodies = shrdMemStruct->numRBDbodies;
		int axisDir[numRBDBodies] = {0};
		memcpy(axisDir,shrdMemStruct->axisOfRotation,sizeof(int)*numRBDBodies);
	sem_post(&(shrdMemStruct->semLock));
	
	double tempGravityDir[3] = {0,0,-1};
	int anglesCounter = 0;
	double *R = angles;
	double cs,si;
	//std::cout << "axisDIr = ["<<axisDir[0] << " , "<<axisDir[1] << " , " << axisDir[2] << " , " << axisDir[3] << std::endl;

	for(int i=0;i<numRBDBodies;i++){

		switch (axisDir[i]){
			case 0:
				R = (angles + anglesCounter);
				// e(1)R^T*g
				*(gravityDir + i*3 + 0) = R[0]*tempGravityDir[0] + R[3]*tempGravityDir[1] + R[6]*tempGravityDir[2];
				*(gravityDir + i*3 + 1) = R[1]*tempGravityDir[0] + R[4]*tempGravityDir[1] + R[7]*tempGravityDir[2];
				*(gravityDir + i*3 + 2) = R[2]*tempGravityDir[0] + R[5]*tempGravityDir[1] + R[8]*tempGravityDir[2];
				anglesCounter += 9;
				memcpy(tempGravityDir,(gravityDir + i*3),sizeofDouble*3);
				//std::cout << "-calcGravityDirAndSave- angle case 0 of body: " << i << std::endl;
				break;

			case 1:
				cs = cos(*(angles + anglesCounter));
				si = sin(*(angles + anglesCounter));
				*(gravityDir + i*3 + 0) = tempGravityDir[0];
				*(gravityDir + i*3 + 1) = cs*tempGravityDir[1] + si*tempGravityDir[2];
				*(gravityDir + i*3 + 2) = -si*tempGravityDir[1] + cs*tempGravityDir[2];
				anglesCounter++;
				memcpy(tempGravityDir,(gravityDir + i*3),sizeofDouble*3);
				//std::cout << "-calcGravityDirAndSave- angle case 1 of body: " << i << " = " << *(angles + anglesCounter) << std::endl;
				break;

			case 2:
				cs = cos(*(angles + anglesCounter));
				si = sin(*(angles + anglesCounter));
				*(gravityDir + i*3 + 0) = cs*tempGravityDir[0] - si*tempGravityDir[2];
				*(gravityDir + i*3 + 1) = tempGravityDir[1];
				*(gravityDir + i*3 + 2) = si*tempGravityDir[0] + cs*tempGravityDir[2];
				anglesCounter++;
				memcpy(tempGravityDir,(gravityDir + i*3),sizeofDouble*3);
				//std::cout << "-calcGravityDirAndSave- angle case 2 of body: " << i << " = " << *(angles + anglesCounter) << std::endl;
				break;

			case 3:
				cs = cos(*(angles + anglesCounter));
				si = sin(*(angles + anglesCounter));
				*(gravityDir + i*3 + 0) =  cs*tempGravityDir[0] + si*tempGravityDir[1];
				*(gravityDir + i*3 + 1) = -si*tempGravityDir[0] + cs*tempGravityDir[1];
				*(gravityDir + i*3 + 2) = tempGravityDir[2];
				anglesCounter++;
				memcpy(tempGravityDir,(gravityDir + i*3),sizeofDouble*3);
				//std::cout << "-calcGravityDirAndSave- angle case 3 of body: " << i << " = " << *(angles + anglesCounter) << std::endl;
				break;
			default:
				std::cout << "axis has invalid value in calcGravityDirAndSave from mySharedMemory. Axis was" <<  axisDir[i] << std::endl;
				return 1;
		}
		//std::cout << "--calcGravityDirAndSave-- gravitydir body "<< i <<" =[" << *(gravityDir + i*3)<<" , " << *(gravityDir + i*3+1) << " , " << *(gravityDir + i*3+2) << std::endl;
	}
	sem_wait(&(shrdMemStruct->semLock));
		memcpy(shrdMemStruct->gravityDir,gravityDir,sizeofDouble*numRBDBodies*3);
	sem_post(&(shrdMemStruct->semLock));
	return 0;
}

int getAnglesFromSharedMem(sharedMemoryStructForIntegration *shrdMemStruct, double *angles){
	sem_wait(&(shrdMemStruct->semLock));
		memcpy(angles,shrdMemStruct->R1,9*sizeofDouble);
		memcpy((angles+9),shrdMemStruct->angles,3*sizeofDouble);
	sem_post(&(shrdMemStruct->semLock));


	return 0;
}


int isGogo(sharedMemoryStructForIntegration *shrdMemStruct){
	sem_wait(&(shrdMemStruct->semLock));
		if(shrdMemStruct->gogo){

			sem_post(&(shrdMemStruct->semLock));
			return 1;
		}
	sem_post(&(shrdMemStruct->semLock));

	return 0;
}

int dontGogo(sharedMemoryStructForIntegration *shrdMemStruct){

	sem_wait(&(shrdMemStruct->semLock));
		shrdMemStruct->gogo = 0;
	sem_post(&(shrdMemStruct->semLock));

	return 0;
}

double returnLoadOnOuter(sharedMemoryStructForIntegration *shrdMemStruct){
	double out = 0;
	sem_wait(&(shrdMemStruct->semLock));
		out = shrdMemStruct->loadOnOuter;
	sem_post(&(shrdMemStruct->semLock));
	return out;
}

int saveBodyLoadToSharedMem(sharedMemoryStructForIntegration *shrdMemStruct, double load1x, double load1y, double load2x, double load2y, int bodynum ){

	//std::cout << "--saveBodyLoadToSharedMem-- force on Body: " << bodynum << " = [" <<load1x << " , " << load1y << " , " << load2x << " , " << load2y << "]"<< std::endl;
	
	sem_wait(&(shrdMemStruct->semLock));
		shrdMemStruct->appliedForceForFEM[bodynum][0] = load1x;
		shrdMemStruct->appliedForceForFEM[bodynum][1] = load1y;
		shrdMemStruct->appliedForceForFEM[bodynum][2] = load2x;
		shrdMemStruct->appliedForceForFEM[bodynum][3] = load2y;
	sem_post(&(shrdMemStruct->semLock));

	return 0;
}