
#include "femClient.h"
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>

std::vector<FEMFILE> findFemFiles(const std::string& folderPath, const std::string& patternStr) {
	namespace fs = std::filesystem;

    std::vector<FEMFILE> femFiles;
    std::regex pattern(patternStr);

    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                std::smatch match;
                if (std::regex_match(filename, match, pattern)) {
                    int number = std::stoi(match[1]);
                    std::string part = match[2];
                    femFiles.push_back({number, part, folderPath+filename});
                }
            }
        }

        // Sort by mesh number
        std::sort(femFiles.begin(), femFiles.end(),
            [](const FEMFILE& a, const FEMFILE& b) {
                return a.meshNumber < b.meshNumber;
            });

    } catch (const std::exception& e) {
        std::cerr << "Error scanning FEM files: " << e.what() << "\n";
    }

    return femFiles;
}

int femReadMesh(FEMDATATOSEND *dataToSend, const char fileName[]){
	using namespace std;
	
	FILE *femDatFile = {0};
	femDatFile = fopen(fileName,"r");
	if(femDatFile == NULL){
		printf("Can't find file %30s, in femClient/femReadMesh\n",fileName);
		exit(1);
	}

	qdInitialReader(dataToSend,femDatFile);
	cout << "numEl "<<dataToSend->numEl<<" numNodPnt "<<dataToSend->numNodPnt<<" numMaterial "<<dataToSend->numMaterial<<" PlaneStressFlag "<<dataToSend->PlaneStressFlag<<" gravity_Flag "<<dataToSend->gravity_Flag<<endl;
	
	calcFemDataSize(dataToSend);

	qdClientMemory(dataToSend);

	qdClientDistributePointers(dataToSend);

	qdClientReader(dataToSend, femDatFile);

	fclose(femDatFile);

	return 0;
}

int calcFemDataSize(FEMDATATOSEND *dataToSend){

	printf("Calculating sizes\n");
	dataToSend->degOfFreedom = dataToSend->numNodPnt*numDegOfFreedom;
	/* Size of memory needed for the doubles */
	int dubSizeForce = dataToSend->degOfFreedom;
	int dubSizeCoord = dataToSend->numNodPnt*numSpatialDim;
	int dubSizeMatProp = 3*dataToSend->numMaterial;
	int dubSizeDisplacedNodes = 2*dataToSend->numNodPnt + 2;
	int dubSizeDiplacementResult = 2*dataToSend->numNodPnt;
	int dubSizeVonMieses = dataToSend->numNodPnt;
	dataToSend->sizeOfMemDoubles = dubSizeForce + 
			dubSizeCoord + dubSizeDisplacedNodes +dubSizeMatProp +dubSizeDiplacementResult+dubSizeVonMieses;

	/* Size of memory needed for the integers */
	int intSizeConnect = dataToSend->numEl*nodesPerElement;
	int intSizeElementalMaterial = dataToSend->numEl;
	int intSizeFixedNodes = 2*dataToSend->numNodPnt+2;
	int intSizePreForce = dataToSend->numNodPnt*2+1; // *2 for group number----------------------------------------	
	dataToSend->sizeOfMemIntegers = intSizeConnect+intSizeElementalMaterial + 
				       intSizeFixedNodes+intSizePreForce;

	dataToSend->numBytesForFinalFemReciever = (dubSizeForce + dubSizeDiplacementResult + dubSizeVonMieses)*sizeof(double) + intSizePreForce*sizeof(int);

	calcNumBytesToSend(dataToSend);

	return dataToSend->sizeOfMemIntegers + dataToSend->sizeOfMemDoubles;

}

int calcNumBytesToSend(FEMDATATOSEND *dataToSend){

    int numEl = dataToSend->numEl;
    int numNodPnt = dataToSend->numNodPnt;
    int numMaterial = dataToSend->numMaterial;
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

	dataToSend->numBytesToSend = sizeOfMemIntegers*sizeof(int) + sizeOfMemDoubles*sizeof(double);

	return sizeOfMemIntegers*sizeof(int) + sizeOfMemDoubles*sizeof(double);



}


int qdInitialReader(FEMDATATOSEND *dataToSend, FILE *femDatFile){
	
	int check = 0;
	char buffer[256];
	fgets(buffer,BUFSIZ,femDatFile);
	check = fscanf(femDatFile,"%d %d %d %d %d \n",&(dataToSend->numEl),&(dataToSend->numNodPnt),&(dataToSend->numMaterial),&(dataToSend->PlaneStressFlag),&(dataToSend->gravity_Flag));
if(check!=5){
	
	printf("Error reading initial values, read %d integers, not 5\n",check);
	exit(1);
}
return 0;
}
/*----------------------------------------------------*/



int qdClientMemory(FEMDATATOSEND *dataToSend){

	int numBytes= dataToSend->sizeOfMemDoubles*sizeof(double) + dataToSend->sizeOfMemIntegers*sizeof(int);
	dataToSend->allTheData = (char *)calloc(numBytes,sizeof(char));
	if(!(dataToSend->allTheData)){
		printf("Failed to allocate memory for the doubles");
		exit(1);
	}

/* Doubles */
	dataToSend->doubles.allTheDoubles = (double *)dataToSend->allTheData;

/* Integers */
	dataToSend->integers.allTheInts = (int *)(dataToSend->allTheData + dataToSend->sizeOfMemDoubles*sizeof(double));
	
return 0;
}
/*----------------------------------------------------*/



int qdClientDistributePointers(FEMDATATOSEND *dataToSend){

	int memCounter = 0;
/* Doubles */
	dataToSend->doubles.coord=(double *)(dataToSend->allTheData + memCounter);
	memCounter += (dataToSend->numNodPnt * numSpatialDim)*sizeof(double);

	dataToSend->doubles.materialProperties = (double *)(dataToSend->allTheData+ memCounter);
	memCounter += (3*dataToSend->numMaterial)*sizeof(double);

	dataToSend->doubles.displacedNodesX = (double *)(dataToSend->allTheData + memCounter);
	memCounter += (dataToSend->numNodPnt +1)*sizeof(double);

	dataToSend->doubles.displacedNodesY = (double *)(dataToSend->allTheData + memCounter);
	memCounter += (dataToSend->numNodPnt +1)*sizeof(double);

/* Integers */
	//printf("number of elements %d",dataToSend->numEl);
	dataToSend->integers.connect = (int *)(dataToSend->allTheData + memCounter);
	memCounter += (dataToSend->numEl*nodesPerElement)*sizeof(int);

	dataToSend->integers.el_matl = (int *)(dataToSend->allTheData + memCounter);
	memCounter += (dataToSend->numEl)*sizeof(int);

	dataToSend->integers.fixedNodesX = (int *)(dataToSend->allTheData + memCounter);
	memCounter += (dataToSend->numNodPnt+1)*sizeof(int);

	dataToSend->integers.fixedNodesY = (int *)(dataToSend->allTheData + memCounter);
	memCounter += (dataToSend->numNodPnt+1)*sizeof(int);

	// Results to recieve:
	dataToSend->finalFemDataToRecieve = dataToSend->allTheData + memCounter;
	dataToSend->numBytesForFinalFemReciever = 0;

	dataToSend->integers.forceGroup = (int *)(dataToSend->allTheData + memCounter);
	dataToSend->numBytesForFinalFemReciever += (dataToSend->numNodPnt)*sizeof(int);
	memCounter += (dataToSend->numNodPnt)*sizeof(int);

	dataToSend->doubles.force = (double *)(dataToSend->allTheData + memCounter);
	dataToSend->numBytesForFinalFemReciever += (dataToSend->degOfFreedom)*sizeof(double);
	memCounter += (dataToSend->degOfFreedom)*sizeof(double);

	dataToSend->integers.nodesWithForce = (int *)(dataToSend->allTheData+ memCounter);
	dataToSend->numBytesForFinalFemReciever += (dataToSend->numNodPnt+1)*sizeof(int);
	memCounter += (dataToSend->numNodPnt+1)*sizeof(int);

	dataToSend->doubles.deformation = (double *)(dataToSend->allTheData+ memCounter);
	dataToSend->numBytesForFinalFemReciever +=  (dataToSend->numNodPnt*2)*sizeof(double);
	memCounter += (dataToSend->numNodPnt*2)*sizeof(double);

	dataToSend->doubles.vonMieses = (double *)(dataToSend->allTheData + memCounter);
	dataToSend->numBytesForFinalFemReciever += (dataToSend->numNodPnt)*sizeof(double);
	memCounter += (dataToSend->numNodPnt)*sizeof(double);



return 0;
}
/*-------------------------------------------------------*/



int qdClientReader(FEMDATATOSEND *dataToSend,FILE *femDatFile){
	
	char buffer[BUFSIZ];
	int i,j, matNum, elNum, nodeNum, counter;
	
	//printf("Reading femqd file\n");
	//printf("Number of Elements: %d, nodes: %d, materials: %d, degree of freedom: %d\n",
	//		dataToSend->numEl,dataToSend->numNodPnt, 
	//		dataToSend->numMaterial, dataToSend->degOfFreedom);

	//printf("Plane theory flag: %d\n\n",dataToSend->PlaneStressFlag);
	//printf("Gravity flag: %d\n\n",dataToSend->gravity_Flag);
/*Pull out the header text*/
       fgets( buffer, BUFSIZ, femDatFile);

/* Pull out material properties*/

	for(i = 0; i<dataToSend->numMaterial; i++){
	 fscanf(femDatFile,"%d\n", &matNum );	
	 //printf("Material (%d) Emod, nu, density\n",matNum);
	 fscanf(femDatFile," %lf %lf %lf\n",(dataToSend->doubles.materialProperties+3*matNum),
			 (dataToSend->doubles.materialProperties+3*matNum+1),
			 (dataToSend->doubles.materialProperties+3*matNum+2));
	 //printf(" %9.4f %9.4f %9.4f\n",*(dataToSend->doubles.materialProperties+3*matNum),
	//		 *(dataToSend->doubles.materialProperties+3*matNum+1),
	//		 *(dataToSend->doubles.materialProperties+3*matNum+2));
	}
	fgets(buffer,BUFSIZ,femDatFile);
	//printf("\n");


/*Reading the connectivity*/
/*
 * 1) connect contains the nodalpoints that make up an element. the size of connect is 
 *    nodesPerElement*numberOfElements 
 * 2) node numbering occurs in a counter clokwise direction  
 */
for( i = 0; i < dataToSend->numEl; i++ ) {
	   fscanf( femDatFile, "%d", &elNum );
	   //printf( "connectivity for element (%4d) ", elNum);
	   for( j = 0; j < nodesPerElement; j++ ) {
		   fscanf( femDatFile, "%d", (dataToSend->integers.connect+nodesPerElement*elNum+j));
		   //printf( "%4d ", *(dataToSend->integers.connect+nodesPerElement*elNum+j));
	   }
	   fscanf( femDatFile, "%d\n", (dataToSend->integers.el_matl+elNum));
	   //printf( " with matl %3d\n", *(dataToSend->integers.el_matl+elNum));
	} 
        fgets( buffer, BUFSIZ, femDatFile );
        //printf( "\n" );	

/* Pull out node coordinate info and echo to screen                     */
/*    1) coord contains the x, y coordinates of a node                  */  
/*       (size of coord is nsd*numnp)                                   */ 
	for( i = 0; i < dataToSend->numNodPnt; i++ ) {
           fscanf( femDatFile, "%d", &nodeNum);
         //  printf( "Node (%d) ", nodeNum);
	  // printf( "coordinates ");
	   for( j = 0; j < numSpatialDim; j++ ) {
		   fscanf( femDatFile, "%lf", (dataToSend->doubles.coord+numSpatialDim*nodeNum+j));
		  // printf( "%9.4f ", *(dataToSend->doubles.coord+numSpatialDim*nodeNum+j));
	   }
           fscanf( femDatFile, "\n");
          // printf( "\n" );	   
	}
        fgets( buffer, BUFSIZ, femDatFile );
       // printf( "\n" );	

/* Pull out x-boundary condition info and echo to screen                */
/*    1) bc.fix.x contains the node numbers where the                   */ 
/*       x-displacement is prescribed                                   */       
/*    2) x-displacement value is put into correct location in the U     */ 
/*    	 array. (size of U is dof=ndof * numnp)                         */ 
/*    3) bc.num_fix.x contains the total number of nodes where          */ 
/*       x-displacement has been prescribed                             */ 
	counter = 0;
	fscanf( femDatFile, "%d",  (dataToSend->integers.fixedNodesX + counter));
	//printf( "node (%4d) has an x prescribed displacement of: ", 
	//				*dataToSend->integers.fixedNodesX);
	while( *(dataToSend->integers.fixedNodesX+counter) > -1 ) {
		
		fscanf( femDatFile, "%lf\n%d", (dataToSend->doubles.displacedNodesX+counter),
			       				(dataToSend->integers.fixedNodesX+counter+1));
		//printf( "%14.6e\n", *(dataToSend->doubles.displacedNodesX+counter));
		//printf( "node (%4d) has an x prescribed displacement of: ",
			//*(dataToSend->integers.fixedNodesX+counter+1));
		counter++;
	}
        dataToSend->numFixx = counter;
	fscanf( femDatFile, "\n" );
        fgets( buffer, BUFSIZ, femDatFile );
	//printf( "\n\n" );

/* Pull out y-boundary condition info and echo to screen                */
/*    1) bc.fix.y contains the node numbers where the                   */ 
/*       y-displacement is prescribed                                   */       
/*    2) y-displacement value is put into correct location in the U     */ 
/*    	 array. (Size of U is dof=ndof * numnp)                         */ 
/*    3) bc.num_fix.y contains the total number of nodes where          */ 
/*       y-displacement has been prescribed                             */ 
        counter = 0;
	fscanf( femDatFile, "%d", dataToSend->integers.fixedNodesY+counter );
	//printf( "node (%4d) has an y prescribed displacement of: ", 
			//	*(dataToSend->integers.fixedNodesY+counter));

	while( *(dataToSend->integers.fixedNodesY+counter) > -1) {
		fscanf( femDatFile, "%lf\n%d", (dataToSend->doubles.displacedNodesY+counter),
			       			dataToSend->integers.fixedNodesY+counter+1);
		//printf( "%14.6e\n", *(dataToSend->doubles.displacedNodesY+counter));
		//printf( "node (%4d) has an y prescribed displacement of: ",
			//*(dataToSend->integers.fixedNodesY+counter+1));
		counter++;
	}
        dataToSend->numFixy = counter;
	fscanf( femDatFile, "\n" );
        fgets( buffer, BUFSIZ, femDatFile );
	//printf( "\n\n" );
	
/* Pull out force boundary condition info and echo to screen            */
/*    1) bc.force contains the node numbers where a nodal force         */ 
/*       has been prescribed                                            */       
/*    3) bc.num_force contains the total number of nodes where          */ 
/*       a force has been prescribed                                    */ 
	counter = 0;
	//printf( "force vector for node: " );
	fscanf( femDatFile, "%d", dataToSend->integers.nodesWithForce);
	//printf( "(%4d)", *(dataToSend->integers.nodesWithForce));
	while( *(dataToSend->integers.nodesWithForce + counter) > -1 ) {
		for( j = 0; j < numDegOfFreedom; j++ ) {
			fscanf( femDatFile, "%lf", (dataToSend->doubles.force+counter*2+j));
			//printf( "%16.4f "  , *(dataToSend->doubles.force+counter*2+j));
		}
			fscanf( femDatFile, "%d", dataToSend->integers.forceGroup+counter); 
			//printf("  %d  \n", *(dataToSend->integers.forceGroup+counter));   
			fscanf( femDatFile, "\n");
	
		//printf( "force vector for node: ");
		counter++;
		fscanf( femDatFile, "%d", dataToSend->integers.nodesWithForce + counter);
		//printf( "(%4d)" ,  *(dataToSend->integers.nodesWithForce + counter));
	}
	//printf( "\n" );

	dataToSend->numForce = counter;
	fscanf( femDatFile, "\n" );
	fgets( buffer, BUFSIZ, femDatFile );
	//printf( "\n\n" );

	return 0;

}	
/*------------------------------------------------*/
int qdClientInitialSender(FEMDATATOSEND *dataToSend,int sockfd){
	
	int dataBuffer[8] = {0};
	dataBuffer[0] = (dataToSend->numEl);
	dataBuffer[1] = (dataToSend->numNodPnt);
	dataBuffer[2] = (dataToSend->numMaterial);
	dataBuffer[3] = (dataToSend->numFixx);
	dataBuffer[4] = (dataToSend->numFixy);
	dataBuffer[5] = (dataToSend->numForce);
	dataBuffer[6] = (dataToSend->PlaneStressFlag);
	dataBuffer[7] = (dataToSend->gravity_Flag);

	if(SendNIntsNoBlock( sockfd, dataBuffer, 8*sizeof(int32_t))){
		printf("Problems with sendNints in qdInitialSender");
		return 1;
	}

return 0;
}
/*--------------------------------------------------------------------*/


int qdClientSender(FEMDATATOSEND *dataToSend, int sockfd){
	
	int i, check, sizeofCoord, sizeofConnect;

/* Send Coordinates */	
         sizeofCoord = dataToSend->numNodPnt*numSpatialDim; 
         check = SendNDoubles(sockfd,dataToSend->doubles.coord,sizeofCoord*sizeof(double)); 
         if(check){
         	printf("Problem with sending Coord in qdClientSender");
		return 1;
	}

/* Sending Force */
	check = 0; 
	//send nodes with force
	printf("Sending %d nodes with force\n",dataToSend->numForce);
	check = SendNInts( sockfd, dataToSend->integers.nodesWithForce, dataToSend->numForce*sizeof(int));

	//Send the forces
	check += SendNDoubles( sockfd, dataToSend->doubles.force, dataToSend->numForce*numDegOfFreedom*sizeof(double));

	if(check){
		printf("Problem with sending forces in qdClientSender");
		return 1;
	}

	/* Sending Connect array */
	sizeofConnect = dataToSend->numEl*nodesPerElement;
	printf("Sending connect array, with %d ints",sizeofConnect);
	check = SendNInts(sockfd,dataToSend->integers.connect,sizeofConnect*sizeof(int));
	if(check){
		printf("Problems with sending Connect array in qdClientSender");
		return 1;
	}



/* Sending material properties */
	check = 0;
	for(i=0;i<dataToSend->numMaterial;i++){
		check += SendNDoubles(sockfd,dataToSend->doubles.materialProperties+i*3,
					3*sizeof(double));
	}
	if(check){
		printf("Problems with sending material properties");
		return 1;
	}
/* Sending Prescribed displacements in X */
	check = 0;
	for(i=0;i<dataToSend->numFixx;i++){
		check += SendInt32(sockfd,*(dataToSend->integers.fixedNodesX+i));
		check += SendNDoubles(sockfd,dataToSend->doubles.displacedNodesX + i,
					sizeof(double));
	}
	if(check){
		printf("Problems with sending prescribed positions in X");
		return 1;
	}
/* Sending Prescribed displacements in Y */
	check = 0;	
	for(i=0;i<dataToSend->numFixy;i++){
		check += SendInt32(sockfd,*(dataToSend->integers.fixedNodesY + i));
		check += SendNDoubles(sockfd,dataToSend->doubles.displacedNodesY + i,
					sizeof(double));
	}
	if(check){
		printf("Problems with sending prescribed positions in Y");
		return 1;
	}


return 0;
}

int qdClientWriter( FEMDATATOSEND *dataToSave, const char name[]) {

int 	i,j,node;
double	fpointx, fpointy;
FILE	*o3;
int numel = dataToSave->numEl;
int numnp = dataToSave->numNodPnt;
int nmat = dataToSave->numMaterial;
int plane_stress_flag = dataToSave->PlaneStressFlag;
int gravity_flag = dataToSave->gravity_Flag;
int npel = 4;//nodes per element
int nsd = 2;//num spatial dimentions
int ndof = 2;//nodal degree of freedom

std::string newFileName(name);  // Convert to std::string
std::string extension = ".dat";
std::string suffix = "_result";

    // Extract just the filename (after the last slash)
    size_t lastSlash = newFileName.find_last_of("/\\");
    std::string nameOnly = (lastSlash != std::string::npos)
        ? newFileName.substr(lastSlash + 1)
        : newFileName;

    size_t pos = nameOnly.rfind(extension);
    if (pos != std::string::npos) {
        nameOnly.insert(pos, suffix);
    } else {
        std::cerr << "File does not have a .dat extension!" << std::endl;
    }
    nameOnly = "../results/" + nameOnly;

o3 = fopen( nameOnly.c_str(), "w");

fprintf(o3, "   numel numnp nmat plane_stress_flag gravity_flag");	
fprintf(o3, " (This is for the quad mesh file: %s)\n", name);	
fprintf(o3, "    %4d %4d %4d %4d %4d \n", 
	numel, numnp, nmat, plane_stress_flag,gravity_flag);	
fprintf(o3, "matl no., E modulus, Poisson's Ratio, density \n");

for( i = 0; i < nmat; i++) {
	fprintf(o3, "  %4d    %12.6e %12.6e %12.6e\n", i, *(dataToSave->doubles.materialProperties+3*i),
	*(dataToSave->doubles.materialProperties+3*i+1),
	*(dataToSave->doubles.materialProperties+3*i+2));	
}

fprintf(o3, "el no., connectivity, matl no \n");
for( i = 0; i < numel; i++) {
   fprintf(o3, "%6d ", i);	   
   for( j = 0; j < npel; j++) {
	fprintf(o3, "%6d ", *(dataToSave->integers.connect+npel*i+j));
   }
   fprintf(o3, "   %3d\n", *(dataToSave->integers.el_matl+i));
} 

fprintf(o3, "node no., coordinates \n");
	for(i = 0; i < numnp; i++) {
   fpointx = *(dataToSave->doubles.coord+nsd*i) + *(dataToSave->doubles.deformation+ndof*i);
   fpointy = *(dataToSave->doubles.coord+nsd*i+1) + *(dataToSave->doubles.deformation+ndof*i+1);
   fprintf( o3, "%4d %14.6f %14.6f\n", i, fpointx, fpointy);	
}	

fprintf(o3, "prescribed displacement x: node disp value \n");
for(i = 0; i < numnp; i++) {
	fprintf( o3, "%4d %14.6e\n", i, *(dataToSave->doubles.deformation+ndof*i));
}
fprintf( o3, " -10\n");	

fprintf(o3, "prescribed displacement y: node disp value \n");
for(i = 0; i < numnp; i++) {
	fprintf( o3, "%4d %14.6e\n", i, *(dataToSave->doubles.deformation+ndof*i+1));
}
fprintf( o3, " -10\n");	


fprintf( o3, "node with point load and load vector in x, y, with the force group\n");
int *id = (int*)calloc(numnp,sizeof(int));
memset(id,1,numnp*sizeof(int));
for( i = 0; i < dataToSave->numForce; i++) {
	node              = dataToSave->integers.nodesWithForce[i];
	*(id+ndof*node)   = -1;
	*(id+ndof*node+1) = -1;		
}
	
for( i = 0; i < numnp; ++i ) {
	   if( *(id+ndof*i) < 0 || *(id+ndof*i+1) < 0 )
	   {
		   fprintf( o3,"%4d",i);
		   for( j = 0; j < ndof; ++j )
		   {
				   fprintf( o3," %16.4e ",*(dataToSave->doubles.force+ndof*i+j));
		   }
		   fprintf( o3, "%d\n",dataToSave->integers.forceGroup[i]);
	   }
	}
fprintf( o3, " -10\n");
free(id);

/*
fprintf( o3, "node no. with stress ");
fprintf( o3, "and stress vector in xx,yy,xy \n");
for( i = 0; i < numnp; ++i )
{
	fprintf( o3,"%4d  ",i);
	fprintf( o3,"%14.6e ",stress_node[i].xx);
	fprintf( o3,"%14.6e ",stress_node[i].yy);
	fprintf( o3,"%14.6e ",stress_node[i].xy);
	fprintf( o3, "\n");
}
	fprintf( o3, " -10 \n");
	fprintf( o3, "node no. with stress ");
	fprintf( o3, "and principal stress I,II \n");
for( i = 0; i < numnp; ++i )
{
	fprintf( o3,"%4d  ",i);
	fprintf( o3,"%14.6e ",stress_node[i].I);
	fprintf( o3,"%14.6e ",stress_node[i].II);
	fprintf( o3, "\n");
}
	fprintf( o3, " -10 \n");*/

fprintf( o3, "node no. with vonMieses ");
fprintf( o3, "and vonMieses magnitude \n");
for(i=0;i<numnp;i++)
{
	fprintf( o3,"%4d  ",i);
	fprintf( o3,"%14.6e \n ",dataToSave->doubles.vonMieses[i]);
}
fprintf( o3, " -10 \n");

/*
fprintf( o3, "node no. with strain ");
fprintf( o3, "and strain vector in xx,yy,xy \n");
for( i = 0; i < numnp; ++i )
{
	fprintf( o3,"%4d  ",i);
	fprintf( o3,"%14.6e ",strain_node[i].xx);
	fprintf( o3,"%14.6e ",strain_node[i].yy);
	fprintf( o3,"%14.6e ",strain_node[i].xy);
	fprintf( o3, "\n");
}
	fprintf( o3, " -10 \n");
	fprintf( o3, "node no. with strain ");
	fprintf( o3, "and principal strain I,II \n");
for( i = 0; i < numnp; ++i )
{
	fprintf( o3,"%4d  ",i);
	fprintf( o3,"%14.6e ",strain_node[i].I);
	fprintf( o3,"%14.6e ",strain_node[i].II);
	fprintf( o3, "\n");
}
	fprintf( o3, " -10 \n");*/
	fclose(o3);

return 0;
}


int freeDataToSend(FEMDATATOSEND *dataToSend){

	free(dataToSend->allTheData);
	return 0;

}

int printElements(FEMDATATOSEND *dataToSend, int bodyNum){



	for(int i=0; i<dataToSend->numEl; i++){
		printf("[%d, %d] = [%d, %d, %d, %d];\n",bodyNum,i,*(dataToSend->integers.connect + i*4 +0),*(dataToSend->integers.connect + i*4 +1),*(dataToSend->integers.connect + i*4 +2),*(dataToSend->integers.connect + i*4 +3));
	}

	return 0;
}
