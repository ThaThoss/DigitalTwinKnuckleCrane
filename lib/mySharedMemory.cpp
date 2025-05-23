#include <semaphore.h>  
#include <sys/shm.h>
#include <sys/ipc.h>
#include <iostream>
#include <cmath>

#include "./commonDataSizes.h"
#include "./femClient.h"
#include "./mySharedMemory.h"
#include "./forceKnuckleToFEMLoop.h"


int calcBytesNeededForRBD(int nRbdBodies,int nFemBodies, int axisOfRotation[]){

	//ints
	int bytesSpatFree = nRbdBodies*3*sizeof(int);
	int bytesForAxisOfRotation = nRbdBodies*sizeof(int);
	//doubles
	int bytesForCmPos = nRbdBodies*3*2*sizeof(double);
	int bytesForMass = nRbdBodies*sizeof(double);
	int bytesForInertia = nRbdBodies*9*sizeof(double);
	int bytesForInitialTransl = nRbdBodies*6*sizeof(double);
	
	int bytesForMassTimesAcc = 21*sizeof(double);
	int bytesForCraneGlobalPos = 3*sizeof(double);
	int bytesForAppliedLoads = 4*nFemBodies*sizeof(double);
	int bytesForInitialAng = calcNumBytesForInitialAngRBD( axisOfRotation, nRbdBodies);
	int numAngl = calcNumAngAccForRBD(axisOfRotation, nRbdBodies);
	int bytesForAngAcc = numAngl*sizeof(double);

	int numBytes =  bytesSpatFree + bytesForAxisOfRotation + (bytesForCmPos + bytesForMass + 
		bytesForInertia + bytesForInitialTransl + bytesForInitialAng + bytesForAngAcc + 
		bytesForMassTimesAcc + bytesForAppliedLoads + bytesForCraneGlobalPos);

	return numBytes;

}

int calcNumBytesForInitialAngRBD(int rotationalAxis[],int nBodies){
	int bytesForInitialAng = 0;
	for(int i=0; i<nBodies;i++){
		if(rotationalAxis[i]==4){
			bytesForInitialAng += 12;
		}else if(rotationalAxis[i]>4 || rotationalAxis[i]<0){
			std::cout << "invalid rotational axis, axis was: "<< rotationalAxis[i] << ", for body number " << i << std::endl;
			return -1;
			
		}else{
			bytesForInitialAng += 2;
		}
	}
	return bytesForInitialAng*sizeof(double);
}

int calcNumAngAccForRBD(int rotationalAxis[], int nBodies){
	int numAngl = 0;
	for(int i=0; i<nBodies;i++){
		if(rotationalAxis[i]==4){
			numAngl += 3;
		}else if(rotationalAxis[i]>4 || rotationalAxis[i]<0){
			std::cout << "invalid rotational axis, axis was: "<< rotationalAxis[i] << ", for body number " << i << std::endl;
			return -1;
			
		}else{
			numAngl += 1;
		}
	}
return numAngl;

}

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
		int *femHeader = (int*)(shmStruct->sharedFEMData + sizeof(int)*8*femBodyNumber);
		dataToSend->numEl = femHeader[0];
		dataToSend->numNodPnt = femHeader[1];
		dataToSend->numMaterial = femHeader[2];
		dataToSend->PlaneStressFlag = femHeader[3];
		dataToSend->gravity_Flag = femHeader[4];
		dataToSend->numFixx = femHeader[5];
		dataToSend->numFixy = femHeader[6];
		dataToSend->numForce = femHeader[7];

	sem_post(&(shmStruct->semLock));

	//std::cout << "Header " << femBodyNumber << " in getFEMHeader (FEM Loop)" <<std::endl;
    //std::cout << dataToSend->numEl << " , " << dataToSend->numNodPnt << " , " << dataToSend->numMaterial << " , " << dataToSend->PlaneStressFlag << " , " << dataToSend->gravity_Flag << std::endl; 


	return 0;
}
int getFEMHeaders(int headers[][8], sharedMemoryStructForIntegration *shmStruct, int nFEMBodies){

sem_wait(&(shmStruct->semLock));
for(int i=0;i<nFEMBodies;i++){
	//numel numnp nmat plane_stress_flag, gravity_flag
	int *femHeader = (int*)(shmStruct->sharedFEMData + sizeof(int)*8*i );
	headers[i][0] = femHeader[0];//numEl
	headers[i][1] = femHeader[1];//numNodPnt
	headers[i][2] = femHeader[2];//numMaterial
	headers[i][3] = femHeader[3];//PlaneStressFlag
	headers[i][4] = femHeader[4];//gravity_Flag
	headers[i][5] = femHeader[5];//numFixedX
	headers[i][6] = femHeader[6];//numFixedY
	headers[i][7] = femHeader[7];//numFOrce

	//std::cout << "Header " << i << " in getFEMHeaders" <<std::endl;
    //std::cout << headers[i][0] << " , " << headers[i][1] << " , " << headers[i][2] << " , " << headers[i][3] << " , " << headers[i][4] << std::endl; 
}

sem_post(&(shmStruct->semLock));


	return 0;
}




int distrubuteFemSharedMemPointers(sharedMemoryStructForIntegration *shrdMemStruct,sharedMemoryPointers *sharedPointers, int femBodyNumber, int bytesForPointer ){


sem_wait(&(shrdMemStruct->semLock));


	int sharedMemoryCounter = femBodyNumber*8*sizeof(int);
	//Headers stored together first, 8 ints * number of FEM bodies.
	//numel numnp nmat plane_stress_flag, gravity_flag
	sharedPointers->header = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	int numberOfElements = *(sharedPointers->header);
	int numNodes = *(sharedPointers->header+1);
	int numMatProperties = *(sharedPointers->header+2);


	//Jump to the start of this bodies data, data is ordered with doubles first, then ints. Then solution data
	
	//Nodal coordinates
	sharedMemoryCounter = bytesForPointer;
	sharedPointers->nodes = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numNodes*numSpatialDim*sizeofDouble;//nodal points is the second int in header

	// Material properties;
	sharedPointers->mat_E_Poiss_Mass = (double*)(shrdMemStruct->sharedFEMData + sharedMemoryCounter );
	sharedMemoryCounter += 3*sizeofDouble*numMatProperties;

	// prescribed displacements X
	sharedPointers->xLockedNodesCoord = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeofDouble;

	// prescribed displacements Y
	sharedPointers->yLockedNodesCoord = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeofDouble;

	
	//Second part is the integers
	//Elements 
	sharedPointers->elements = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numberOfElements*nodesPerElement*sizeof(int);//first int in header is number of elements

	//Third is the elements material number
	sharedPointers->elementalMaterialNumber = (int*)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numberOfElements*sizeof(int);//first int in header is number of elements

	//prescribed displacement x: 
  	sharedPointers->xLocedNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);

	//prescribed displacement y: 
  	sharedPointers->yLocedNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);



	//Results to send:
	sharedPointers->finalFEMResultToSend = (shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	
	//Force nodes group number:
	sharedPointers->forceGroup = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes)*sizeof(int);
	sharedPointers->numBytesForFEMToSend += (numNodes)*sizeof(int);

	//Force vectors:
	sharedPointers->forceVectors = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numSpatialDim*(numNodes)*sizeofDouble;
	sharedPointers->numBytesForFEMToSend += numSpatialDim*(numNodes)*sizeofDouble;

	//Nodes with force: 
	sharedPointers->forceNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);
	sharedPointers->numBytesForFEMToSend += (numNodes+1)*sizeof(int);

	//Resulting Nodal coordinates, deformed mesh
	sharedPointers->deformedNodes = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numNodes*numSpatialDim*sizeofDouble;//nodal points is the second int in header
	sharedPointers->numBytesForFEMToSend += numNodes*numSpatialDim*sizeofDouble;

	//Resulting von Mieses stress
	sharedPointers->vonMieses = (double*)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numNodes*sizeof(double);//first int in header is number of elements
	sharedPointers->numBytesForFEMToSend += numNodes*sizeofDouble;
sem_post(&(shrdMemStruct->semLock));
	return 0;
}
int distributeRbdMemPointers(sharedMemoryStructForIntegration *shrdMemStruct,SHAREDMEMORYPOINTERSRBD *sharedMemoryRBDPointers, int *rotationalAxis, int semProtect ){

	int dataCounter = 0;

	if(semProtect){
		sem_wait(&(shrdMemStruct->semLock));
	}

	int nBodies = shrdMemStruct->numRBDbodies;
	int nFemBodies = shrdMemStruct->numFEMBodies;
	sharedMemoryRBDPointers->dataForRBD = (shrdMemStruct->sharedFEMData + shrdMemStruct->numBytesForHeader);
	sharedMemoryRBDPointers->axisOfRotation = (int *)sharedMemoryRBDPointers->dataForRBD; 
	dataCounter += nBodies*sizeof(int);

	sharedMemoryRBDPointers->spatialFreedom = (int *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*3*sizeof(int);

	sharedMemoryRBDPointers->cogCoords = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*2*3*sizeof(double);


	sharedMemoryRBDPointers->mass = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*sizeof(double);

	sharedMemoryRBDPointers->inertia = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*9*sizeof(double);

	int bytesForInitialAng = calcNumBytesForInitialAngRBD( rotationalAxis, nBodies);

	int numAng = calcNumAngAccForRBD(rotationalAxis, nBodies);

	shrdMemStruct->numAngles = numAng;

	sharedMemoryRBDPointers->initialAngularValues = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += bytesForInitialAng;

	sharedMemoryRBDPointers->initialTranslationalValues = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*6*sizeof(double);

	// Results:
	sharedMemoryRBDPointers->angAcc = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += numAng*sizeof(double);

	sharedMemoryRBDPointers->massTimesAcc = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += 21*sizeof(double);//nBodies*6 -3, Size of dX vectors in RBD. -3 as ship is not moving

	sharedMemoryRBDPointers->craneGlobalPos = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += 3*sizeof(double);

	sharedMemoryRBDPointers->appliedLoads = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += 4*nFemBodies*sizeof(double);


	if(semProtect){
		sem_post(&(shrdMemStruct->semLock));
	}
	return 0;
}

int distributeRbdMemPointers(sharedMemoryStructForIntegration *shrdMemStruct,SHAREDMEMORYPOINTERSRBD *sharedMemoryRBDPointers ){

	int dataCounter = 0;
	
		sem_wait(&(shrdMemStruct->semLock));


	int nBodies = shrdMemStruct->numRBDbodies;
	int nFemBodies = shrdMemStruct->numFEMBodies;
	sharedMemoryRBDPointers->dataForRBD = (shrdMemStruct->sharedFEMData + shrdMemStruct->numBytesForHeader);
	sharedMemoryRBDPointers->axisOfRotation = (int *)sharedMemoryRBDPointers->dataForRBD; 
	dataCounter += nBodies*sizeof(int);

	sharedMemoryRBDPointers->spatialFreedom = (int *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*3*sizeof(int);

	sharedMemoryRBDPointers->cogCoords = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*2*3*sizeof(double);


	sharedMemoryRBDPointers->mass = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*sizeof(double);

	sharedMemoryRBDPointers->inertia = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*9*sizeof(double);

	int bytesForInitialAng = calcNumBytesForInitialAngRBD( sharedMemoryRBDPointers->axisOfRotation, nBodies);

	int numAng = calcNumAngAccForRBD(sharedMemoryRBDPointers->axisOfRotation, nBodies);
	shrdMemStruct->numAngles = numAng;

	sharedMemoryRBDPointers->initialAngularValues = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += bytesForInitialAng*sizeof(double);

	sharedMemoryRBDPointers->initialTranslationalValues = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += nBodies*6*sizeof(double);

	// Results:
	sharedMemoryRBDPointers->angAcc = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += numAng*sizeof(double);

	sharedMemoryRBDPointers->massTimesAcc = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += 21*sizeof(double);//nBodies*6 -3, Size of dX vectors in RBD. -3 as ship is not moving

	sharedMemoryRBDPointers->craneGlobalPos = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += 3*sizeof(double);

	sharedMemoryRBDPointers->appliedLoads = (double *)(sharedMemoryRBDPointers->dataForRBD + dataCounter);
	dataCounter += 4*nFemBodies*sizeof(double);


	
	sem_post(&(shrdMemStruct->semLock));

	return 0;
}

int saveFemData(sharedMemoryStructForIntegration *shrdMemStruct,FEMDATATOSEND *femData,sharedMemoryPointers *sharedPointers, int femBodyNumber, int bytesForPointer ){

sem_wait(&(shrdMemStruct->semLock));

	int numberOfElements = *(sharedPointers->header);
	int numNodes = *(sharedPointers->header+1);
	int numMatProperties = *(sharedPointers->header+2);

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
	memcpy(sharedPointers->forceVectors,femData->doubles.force,femData->numForce*numSpatialDim*sizeofDouble);

	memset(sharedPointers->deformedNodes, 0, numNodes*numSpatialDim*sizeofDouble);
	memset(sharedPointers->vonMieses, 0, numNodes*sizeofDouble );

sem_post(&(shrdMemStruct->semLock));

	return 0;
}

int getFemData(sharedMemoryStructForIntegration *shrdMemStruct,FEMDATATOSEND *femData,sharedMemoryPointers *sharedPointers, int femBodyNumber ){

	sem_wait(&(shrdMemStruct->semLock));
	
		int numberOfElements = *(sharedPointers->header);
		int numNodes = *(sharedPointers->header+1);
		int numMatProperties = *(sharedPointers->header+2);
	
		femData->numForce = shrdMemStruct->numForce[femBodyNumber];
	
		//Copy in all aterial properties, 3 dubles per material
		memcpy(femData->doubles.materialProperties, sharedPointers->mat_E_Poiss_Mass, 3*sizeofDouble*numMatProperties);
	
		//Copy elements over,4 ints per element
		memcpy(femData->integers.connect, sharedPointers->elements, numberOfElements*nodesPerElement*sizeof(int));
		/*
		printf("\n\nElements in get FemData from femBody %d\n\n", femBodyNumber);
		for(int i=0;i<numberOfElements;i++){
			printf("[%d, %d] = [%d, %d, %d, %d];\n",femBodyNumber,i,*(femData->integers.connect + i*4 +0),*(femData->integers.connect + i*4 +1),*(femData->integers.connect + i*4 +2),*(femData->integers.connect + i*4 +3));
		}
		printf("\n-------------------------------------\n\n");
	*/
		//Copy elementalMaterialNumber, 1 int per element
		memcpy(femData->integers.el_matl, sharedPointers->elementalMaterialNumber, numberOfElements*sizeof(int));
	
		//Copy nodes, 2 doubles per node
		memcpy(femData->doubles.coord, sharedPointers->nodes, 2*sizeofDouble*numSpatialDim*numNodes);
	
		//Copy Fixed X nodes, one int per locked node.
		memcpy(femData->integers.fixedNodesX, sharedPointers->xLocedNodes, femData->numFixx*sizeof(int));
		//2 doubles per node
		memcpy(femData->doubles.displacedNodesX, sharedPointers->xLockedNodesCoord, femData->numFixx*2*sizeofDouble);
	
		//Copy Fixed Y nodes, one int per locked node.
		memcpy(femData->integers.fixedNodesY, sharedPointers->yLocedNodes, femData->numFixy*sizeof(int));
		//2 doubles per node
		memcpy(femData->doubles.displacedNodesY, sharedPointers->yLockedNodesCoord, femData->numFixy*2*sizeofDouble);
	
		//Copy force nodes, one int per force vector
		memcpy(femData->integers.nodesWithForce, sharedPointers->forceNodes, femData->numForce*sizeof(int));
		
		//Copy forceGroup for nodes, one int per force vector
		memcpy(femData->integers.forceGroup, sharedPointers->forceGroup, femData->numForce*sizeof(int));
	
		//2 doubles per node
		memcpy(femData->doubles.force, sharedPointers->forceVectors, femData->numForce*numSpatialDim*sizeofDouble);
	
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

int saveForceResultToSharedMemory(sharedMemoryStructForIntegration *shrdMemStruct, FEMDATATOSEND *femData, sharedMemoryPointers *sharedPointers, double* force){

	sem_wait(&(shrdMemStruct->semLock));
		memcpy(sharedPointers->forceVectors,force,femData->degOfFreedom*sizeofDouble);
	sem_post(&(shrdMemStruct->semLock));

	return 0;
}



int getForceFromShardeMem(double *Force, double* gravityDirection, sharedMemoryStructForIntegration *shrdMemStruct,sharedMemoryPointers *sharedPointers, int numForceNodes, int bodyNum){

	sem_wait(&(shrdMemStruct->semLock));
	//2 doubles per node
	memcpy( Force, sharedPointers->forceVectors,numForceNodes*numSpatialDim*sizeofDouble);
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
	//std::cout << "axisDir from internal calcGravity... = ["<<axisDir[0] << " , "<<axisDir[1] << " , " << axisDir[2] << " , " << axisDir[3] << std::endl;

	for(int i=0;i<numRBDBodies;i++){

		switch (axisDir[i]){
			case 4:
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
				std::cout << "------- axis has invalid value in calcGravityDirAndSave from mySharedMemory. Axis was" <<  axisDir[i] << "-------------------------"<<std::endl;
				return 1;
		}
		//std::cout << "--calcGravityDirAndSave-- gravitydir body "<< i <<" =[" << *(gravityDir + i*3)<<" , " << *(gravityDir + i*3+1) << " , " << *(gravityDir + i*3+2) << std::endl;
	}
	sem_wait(&(shrdMemStruct->semLock));
		memcpy(shrdMemStruct->gravityDir,gravityDir,sizeofDouble*numRBDBodies*3);
	sem_post(&(shrdMemStruct->semLock));
	return 0;
}

int getAnglesFromSharedMem(sharedMemoryStructForIntegration *shrdMemStruct, SHAREDMEMORYPOINTERSRBD *sharedMemoryRBDPointers, double *angles, double *angAcc, double *craneGlobPos){
	sem_wait(&(shrdMemStruct->semLock));
		memcpy(angles,sharedMemoryRBDPointers->initialAngularValues,shrdMemStruct->bytesForInitialAng);
		memcpy(angAcc,sharedMemoryRBDPointers->angAcc,shrdMemStruct->numAngles*sizeof(double));
		memcpy(craneGlobPos,sharedMemoryRBDPointers->craneGlobalPos,3*sizeof(double));
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


int printShrdFemData(sharedMemoryStructForIntegration *shrdMemStruct, int bytesForPointer, int femBodyNumb){

	printf("bef Shared Lock");
	sem_wait(&(shrdMemStruct->semLock));
	int sharedMemoryCounter = femBodyNumb*8*sizeof(int);
	//Headers stored together first, 8 ints * number of FEM bodies.
	//numel numnp nmat plane_stress_flag, gravity_flag
	int *header = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	int numberOfElements = *(header);
	int numNodes = *(header+1);
	int numMatProperties = *(header+2);
	printf("in print Shared, nEl %d, nNode %d, nMat %d", numberOfElements,numNodes,numMatProperties);

	sharedMemoryCounter = bytesForPointer;
	double *nodes = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	printf("Nodes of Body %d, in server\n",femBodyNumb);
	for(int i=0;i<numNodes;i++){
		printf("[%d, %d] = [%lf, %lf]\n", femBodyNumb,i,*(nodes + i*2 + 0),*(nodes + i*2 + 1));

	}printf("\n\n\n");
	sharedMemoryCounter += numNodes*numSpatialDim*sizeofDouble;//nodal points is the second int in header
	
		// Material properties;
		//sharedPointers->mat_E_Poiss_Mass = (double*)(shrdMemStruct->sharedFEMData + sharedMemoryCounter );
		sharedMemoryCounter += 3*sizeofDouble*numMatProperties;
	
		// prescribed displacements X
		//sharedPointers->xLockedNodesCoord = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
		sharedMemoryCounter += (numNodes+1)*sizeofDouble;
	
		// prescribed displacements Y
		//sharedPointers->yLockedNodesCoord = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
		sharedMemoryCounter += (numNodes+1)*sizeofDouble;

			//Second part is the integers
	//Elements 
	printf("Elements of Body %d, in Server\n",femBodyNumb);
	int *elements = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	for(int i=0;i<numberOfElements;i++){
		printf("[%d, %d] = [%d, %d, %d, %d];\n",femBodyNumb,i,*(elements+ i*4 +0),*(elements+ i*4 +1),*(elements + i*4 +2),*(elements+ i*4 +3));
	}printf("\n\n\n");
	sharedMemoryCounter += numberOfElements*nodesPerElement*sizeof(int);//first int in header is number of elements
/*
	//Third is the elements material number
	sharedPointers->elementalMaterialNumber = (int*)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += numberOfElements*sizeof(int);//first int in header is number of elements

	//prescribed displacement x: 
  	sharedPointers->xLocedNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);

	//prescribed displacement y: 
  	sharedPointers->yLocedNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);

	//Force nodes group number:
	sharedPointers->forceGroup = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes)*sizeof(int);

	//Force vectors:
	//sharedPointers->forceVectors = (double *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes)*sizeofDouble*numSpatialDim;

	//Nodes with force: 
	sharedPointers->forceNodes = (int *)(shrdMemStruct->sharedFEMData + sharedMemoryCounter);
	sharedMemoryCounter += (numNodes+1)*sizeof(int);

*/



	sem_post(&(shrdMemStruct->semLock));
	return 0;
}
int setForceGroups(sharedMemoryStructForIntegration *shmStru,sharedMemoryPointers *allSharedPointers){

	int numNodes = 0;
	int numForce = 0;
	int setTo = -1;

	for(int i=0; i<shmStru->numFEMBodies;i++){
		numNodes = allSharedPointers[i].header[1];
		numForce = allSharedPointers[i].header[7];
		int forceGroups[numForce] = {0};
		memcpy(forceGroups,allSharedPointers[i].forceGroup,numForce*sizeof(int));
		memset(allSharedPointers[i].forceGroup,setTo,numNodes*sizeof(int));
		for(int j=0;j<numForce;j++){
			*(allSharedPointers[i].forceGroup + allSharedPointers[i].forceNodes[j] ) = forceGroups[j];
		}
	}

	return 0;
}

int waitForKnukleToFinish(sharedMemoryStructForIntegration *shmStru){
	sem_wait(&shmStru->semLock);
    int knuckleRunning = shmStru->isKnuckleRunning;
    sem_post(&shmStru->semLock);

    while(knuckleRunning){
        sleep(1);
        sem_wait(&shmStru->semLock);
        knuckleRunning = shmStru->isKnuckleRunning;
        sem_post(&shmStru->semLock);

    }
	return 0;
}

int myPrintAngles(double *angles, double *angAcc, double *_globalPos){

  printf("----R1----\n");
  printf("%lf, %lf, %lf\n",angles[0],angles[1],angles[2]);
  printf("%lf, %lf, %lf\n",angles[3],angles[4],angles[5]);
  printf("%lf, %lf, %lf\n",angles[6],angles[7],angles[8]);
  printf("Ang: [%lf, %lf, %lf, %lf, %lf, %lf]\n",angles[9],angles[10],angles[11]);
  printf("AngV: [%lf, %lf, %lf, %lf, %lf, %lf]\n",angles[12],angles[13],angles[14],angles[15],angles[16],angles[17]);
  printf("AngAcc: [%lf, %lf, %lf, %lf, %lf, %lf]\n",angAcc[0],angAcc[1],angAcc[2],angAcc[3],angAcc[4],angAcc[5]);
  printf("CraneGlobPos : [%lf, %lf, %lf]\n",_globalPos[0],_globalPos[1],_globalPos[2]);

  return 0;

}