#include "commonDataSizes.h"
#include <cstdio>

int calcFemNumBytesFromClient(int header[5]){

	printf("numE %d, nNode %d, numMat %d", header[0],header[1],header[2]);

    
    int numSpatialDim =2;
    int nodesPerElement = 4;
    int numEl = header[0];
    int numNodPnt = header[1];
    int numMaterial = header[2];
	int degOfFreedom = numNodPnt*2;//numNodPnt*numDegOfFreedom;
	/* Size of memory needed for the doubles */
	int dubSizeForce = degOfFreedom;
	int dubSizeCoord = numNodPnt*numSpatialDim;
	int dubSizeMatProp = 3*numMaterial;
	int dubSizeDisplacedNodes = 2*numNodPnt + 2;
	int sizeOfMemDoubles = dubSizeForce + 
			dubSizeCoord + dubSizeDisplacedNodes +dubSizeMatProp;

	/* Size of memory needed for the integers */
	int intSizeConnect = numEl*nodesPerElement;
	int intSizeElementalMaterial = numEl;
	int intSizeFixedNodes = 2*numNodPnt+2;
	int intSizePreForce = numNodPnt*2+1; // *2 for group number----------------------------------------	
	int sizeOfMemIntegers = intSizeConnect+intSizeElementalMaterial + 
				       intSizeFixedNodes+intSizePreForce;

	return sizeOfMemIntegers*sizeof(int) + sizeOfMemDoubles*sizeof(double);




}