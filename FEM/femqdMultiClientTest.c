#include "Client.h"


int main(int argc, char *argv[]){


/* Create socket and connect */	
	
	FILE *femDatFile;
	DATATOSEND dataToSend;	
	char ip[16], name[32];
	char buffer[BUFSIZ];
	
	int sockfd[2], portno, n, check;
	int numberOfElements, numberOfNodalPoints, numberOfMaterialProperties, planeStressFlag, degOfFreedom;
	int *connect, el_matl;
	int dubSizeForce, dubSizeCoord, dubSizeU, dubSizeMatProp, intSizeConnect, intSizeElementalMaterial, intSizeFixedNodes, intSizePreForce, dubSizeDisplacedNodes;
	double *coord, *force, *U;

	
	argvChecker(argc, argv, ip, name);
	printf("ip: %s \n",ip);	
	
	portno = atoi(PORT);
	for(int i=0;i<2;i++){
		sockfd[i] = connectToServer(portno,ip);
		if(sockfd[i]==-1){
			printf("Error in connectToServer");
			return -1;
		}

		n = read(sockfd[i],buffer,255);
		printf("Ansver was %s\n",buffer);
		check = strncmp(buffer,"200",3);
		if(check){
			printf("\nServer error with code %s\n",buffer);
			return -1;
		}

		femDatFile = fopen(name,"r");
		if(femDatFile == NULL){
			printf("Can't find file %30s\n",name);
			exit(1);
		}
	printf("reading...\n");	
		check = qdInitialReader(&dataToSend,femDatFile);

		dataToSend.degOfFreedom = dataToSend.numNodPnt*numDegOfFreedom;
	printf("Calculating sizes\n");
	/* Size of memory needed for the doubles */
		dubSizeForce = dataToSend.degOfFreedom;
		dubSizeCoord = dataToSend.numNodPnt*numSpatialDim;
		dubSizeMatProp = 3*dataToSend.numMaterial;
		dubSizeDisplacedNodes = 2*dataToSend.numNodPnt + 2;
		dataToSend.sizeOfMemDoubles = dubSizeForce + 
				dubSizeCoord + dubSizeDisplacedNodes;

	/* Size of memory needed for the integers */
		intSizeConnect = dataToSend.numEl*nodesPerElement;
		intSizeElementalMaterial = dataToSend.numEl;
		intSizeFixedNodes = 2*dataToSend.numNodPnt+2;
		intSizePreForce = dataToSend.numNodPnt+1;	
		dataToSend.sizeOfMemIntegers = intSizeConnect+intSizeElementalMaterial + 
						intSizeFixedNodes+intSizePreForce;

		
		check = qdClientMemory(&dataToSend);
		check = qdClientDistributePointers(&dataToSend);
		check = qdClientReader(&dataToSend, femDatFile);
		printf("Sending\n");
		check = qdClientInitialSender(&dataToSend,sockfd[i]);
		
		check = qdClientSender(&dataToSend,sockfd[i]);
		
	}
	for(int i =0;i<2;i++){
		close(sockfd[i]);
	}

	printf("Done :)\n");
	

	return 0;
}




