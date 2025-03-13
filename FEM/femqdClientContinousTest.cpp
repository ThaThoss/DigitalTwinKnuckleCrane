#include "Client.h"
#include "./SendReceiveFemQD.h"


int main(int argc, char *argv[]){


/* Create socket and connect */	
	
	FILE *femDatFile;
	DATATOSEND dataToSend;	
	char ip[16], name[32];
	char buffer[BUFSIZ];
	
	int sockfd, portno, check;
	//int numberOfElements, numberOfNodalPoints, numberOfMaterialProperties, planeStressFlag, degOfFreedom;
	//int *connect, el_matl;
	int dubSizeForce, dubSizeCoord, dubSizeMatProp, intSizeConnect, intSizeElementalMaterial, intSizeFixedNodes, intSizePreForce, dubSizeDisplacedNodes;
	//double *coord, *force, *U;

	
	argvChecker(argc, argv, ip, name);
	printf("ip: %s \n",ip);	
	
	portno = atoi(PORT);
	sockfd = connectToServer(portno,ip);
	if(sockfd==-1){
		printf("Error in connectToServer");
		return -1;
	}

	read(sockfd,buffer,255);
	printf("Ansver was %s\n",buffer);
	check = strncmp(buffer,"200",3);
	if(check){
		printf("\nServer error with code %s\n",buffer);
		return -1;
	}
	printf("filename = %s\n",name);
	femDatFile = fopen(name,"r");
	if(femDatFile == NULL){
		printf("Can't find file %30s\n",name);
		exit(1);
	}
printf("reading file %s...\n",name);	
	check = qdInitialReader(&dataToSend,femDatFile);
	dataToSend.degOfFreedom = dataToSend.numNodPnt*numDegOfFreedom;
printf("Calculating sizes\n");
/* Size of memory needed for the doubles */
	dubSizeForce = dataToSend.degOfFreedom;
	dubSizeCoord = dataToSend.numNodPnt*numSpatialDim;
	dubSizeMatProp = 3*dataToSend.numMaterial;
	dubSizeDisplacedNodes = 2*dataToSend.numNodPnt + 2;
	dataToSend.sizeOfMemDoubles = dubSizeForce + 
			dubSizeCoord + dubSizeDisplacedNodes +dubSizeMatProp;

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
	check = qdClientInitialSender(&dataToSend,sockfd);
	
	check = qdClientSender(&dataToSend,sockfd);

	printf("Done sending mesh:)\n");

	int numBytesToRecieve = (dataToSend.degOfFreedom +dataToSend.numNodPnt)*sizeof(double);
	double *dataToReceive = (double*)calloc(dataToSend.degOfFreedom +dataToSend.numNodPnt,
												sizeof(double));
												
	int bytesToSend = (dataToSend.numForce*2+2)*sizeof(double) + sizeof(int);
	char* dataBufferForSend = (char*)calloc(bytesToSend,sizeof(char));
	double *force = (double*)dataBufferForSend;
	
	for(int k =0;k<dataToSend.numForce;k++){
	printf("force sent = [%lf, %lf]\n",*(dataToSend.doubles.force+2*k), *(dataToSend.doubles.force +2*k+1));
}

	memcpy(force,dataToSend.doubles.force,dataToSend.numForce*2*sizeof(double));
	double *gravityDirection = force+dataToSend.numForce*2;



	gravityDirection[0] = 0;
	gravityDirection[1] = -1;
	int *gogo = (int*)(dataBufferForSend + (dataToSend.numForce*2+2)*sizeof(double));
	gogo[0] = 1;
	int count = 0;
	printf("Starting FEM loop, ready to recieve...\n");
	while(gogo[0]){

		ReceiveNChars( sockfd, (char*)dataToReceive, numBytesToRecieve);
		if(count>5){
			gogo[0]=0;
		}
		count++;
		// buffer = [Force x and y, gravity x and y, gogo int]
		SendNChar(sockfd,  dataBufferForSend, bytesToSend );
		for(int k =0;k<dataToSend.numForce;k++){
		printf("force sent = [%lf, %lf]\n",*(force+2*k), *(force +2*k+1));
		}

		for(int i = 0; i<dataToSend.degOfFreedom; i++){
			printf("U[%d]=%lf\n",i,*(dataToReceive+i));
		}
		for( int i = 0; i<dataToSend.numNodPnt; i++){
			printf("VonMieses[%d]=%lf\n",i,*(dataToReceive+i +dataToSend.degOfFreedom ));
		}

	}


	printf("Fem client shutting down\n");
	close(sockfd);

	return 0;
}




