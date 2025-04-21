#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <limits>
#include <regex>
#include <filesystem>

#include "rbdClient.h"



std::vector<std::string> findRBDFiles(const std::string& folderPath, const std::string& patternStr) {
    namespace fs = std::filesystem;

    std::vector<std::string> files;
    std::regex pattern(patternStr);

    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (!entry.is_regular_file()) continue;

            std::string name = entry.path().filename().string();

            if (std::regex_match(name, pattern)) {
                files.push_back(entry.path().string());  // Full relative or absolute path
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error scanning folder: " << e.what() << std::endl;
    }

    return files;
}

int readRBDdata(RBDDATATOSEND *dataToSend, const char fileName[]){

    FILE *filePointer;

    filePointer = fopen(fileName,"r");
    
    /*Read initial data*/
    char buffer[100] = {"\n"};
    fgets(buffer,sizeof(buffer),filePointer);
    std::cout << buffer;
    fscanf(filePointer, "%d\n",&dataToSend->nBodies);
    std::cout << dataToSend->nBodies << std::endl;

    int axisOfRotation[dataToSend->nBodies] = {0};

    fgets(buffer,sizeof(buffer),filePointer);
    std::cout << buffer;
    for(int i=0;i<dataToSend->nBodies;i++){
        fscanf(filePointer,"%d",&axisOfRotation[i]);
        std::cout << axisOfRotation[i] << ", ";
    }
    fscanf(filePointer,"\n");
    std::cout << std::endl;

    rbdGetMemoryAndSetPointers(dataToSend, axisOfRotation);

    readRBDfile(dataToSend, filePointer);
    fclose(filePointer);
 
    return 0;

}

int saveRBDdataToFile(RBDDATATOSEND *dataToSend, const char fileName[],int nFEMBodies){

    FILE *filePointer;

    std::string newFileName(fileName);  // Convert to std::string
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

    filePointer = fopen(nameOnly.c_str(),"w");

    int nBod = dataToSend->nBodies;
    fprintf(filePointer,"nBod\n");
    fprintf(filePointer,"%d\n",nBod);

    fprintf(filePointer,"Axis of Rotation\n");
    for(int i=0; i<nBod;i++){
        fprintf(filePointer," %d",dataToSend->axisOfRotations[i]);
    }
    fprintf(filePointer,"\nSpatial freedom\n");
    for(int i=0;i<nBod;i++){
        fprintf(filePointer,"%d %d %d %d\n",*(dataToSend->spatialFreedom + 0),*(dataToSend->spatialFreedom + 1),*(dataToSend->spatialFreedom + 2),*(dataToSend->spatialFreedom + 3));
    }
    fprintf(filePointer,"centre Of Mass Coordinates\n");
    for(int i=0; i<nBod;i++){
        fprintf(filePointer,"%lf %lf %lf\n",*(dataToSend->centreOfMassCoord + 0 + i*6),*(dataToSend->centreOfMassCoord + 1 + i*6),*(dataToSend->centreOfMassCoord + 2 + i*6));
        fprintf(filePointer,"%lf %lf %lf\n",*(dataToSend->centreOfMassCoord + 3 + i*6),*(dataToSend->centreOfMassCoord + 4 + i*6),*(dataToSend->centreOfMassCoord + 5 + i*6));
    }
    fprintf(filePointer,"Mass\n");
    for(int i=0;i<nBod;i++){
        fprintf(filePointer,"%lf ",dataToSend->mass[i]);
    }
    fprintf(filePointer,"\nInertia\n");
    for(int i=0;i<nBod;i++){
        fprintf(filePointer,"%lf %lf %lf %lf %lf %lf %lf %lf %lf\n",*(dataToSend->inertia + 0 + i*9),*(dataToSend->inertia + 1 + i*9),*(dataToSend->inertia + 2 + i*9),
        *(dataToSend->inertia + 3 + i*9),*(dataToSend->inertia + 4 + i*9),*(dataToSend->inertia + 5 + i*9),
        *(dataToSend->inertia + 6 + i*9),*(dataToSend->inertia + 7 + i*9),*(dataToSend->inertia + 8 + i*9));
    }
    fprintf(filePointer,"initial angular values R1, ang, angV\n");
    int counter = 0;
    for(int i=0;i<nBod;i++){
        if(dataToSend->axisOfRotations[i]>3){
            fprintf(filePointer,("%lf %lf %lf %lf %lf %lf %lf %lf %lf "), *(dataToSend->initialAngularValues + counter + 0),*(dataToSend->initialAngularValues + counter + 1),
            *(dataToSend->initialAngularValues + counter + 2),*(dataToSend->initialAngularValues + counter + 3),*(dataToSend->initialAngularValues + counter + 4),
            *(dataToSend->initialAngularValues + counter + 5),*(dataToSend->initialAngularValues + counter + 6),*(dataToSend->initialAngularValues + counter + 7),
            *(dataToSend->initialAngularValues + counter + 8));
            counter += 9;
        }else if(dataToSend->axisOfRotations[i]>0 && dataToSend->axisOfRotations[i]<4){
            fprintf(filePointer,"%lf ",*(dataToSend->initialAngularValues + counter));
            counter++;
        }
    }
    for(int i=0;i<nBod;i++){
        if(dataToSend->axisOfRotations[i]>3){
            fprintf(filePointer,("%lf %lf %lf "), *(dataToSend->initialAngularValues + counter + 0),*(dataToSend->initialAngularValues + counter + 1),
            *(dataToSend->initialAngularValues + counter + 2));
            counter += 3;
        }else if(dataToSend->axisOfRotations[i]>0 && dataToSend->axisOfRotations[i]<4){
            fprintf(filePointer,"%lf ",*(dataToSend->initialAngularValues + counter));
            counter++;
        }
    }

    fprintf(filePointer,"\ninitial translation initial velocity\n");
    for(int i=0;i<nBod;i++){
        fprintf(filePointer,("%lf %lf %lf %lf %lf %lf\n"), *(dataToSend->initialTranslationalValues + i*6 + 0),*(dataToSend->initialTranslationalValues + i*6 + 1),
            *(dataToSend->initialTranslationalValues + i*6 + 2),*(dataToSend->initialTranslationalValues + i*6 + 3),*(dataToSend->initialTranslationalValues + i*6 + 4),*(dataToSend->initialTranslationalValues + i*6 + 5));
    }
    fprintf(filePointer,"initial angular Acceleration\n");
    counter = 0;
    for(int i=0;i<nBod;i++){

        if(dataToSend->axisOfRotations[i]>3){
            fprintf(filePointer,("%lf %lf %lf "), *(dataToSend->angularAcc + counter + 0),*(dataToSend->angularAcc + counter + 1),
            *(dataToSend->angularAcc + counter + 2));
            counter += 3;
        }else if(dataToSend->axisOfRotations[i]>0 && dataToSend->axisOfRotations[i]<4){
            fprintf(filePointer,"%lf ",*(dataToSend->angularAcc + counter));
            counter++;
        }

    }
    fprintf(filePointer,"\nmass Times Acc\n");
    fprintf(filePointer,"0.0 0.0 0.0 %lf %lf %lf\n", *(dataToSend->massTimesAcc + 0),*(dataToSend->massTimesAcc + 1),*(dataToSend->massTimesAcc + 2));
    for(int i=1; i<nBod; i++){
        fprintf(filePointer,"%lf %lf %lf %lf %lf %lf\n",*(dataToSend->massTimesAcc + i*6 + 0 + 3),*(dataToSend->massTimesAcc + i*6 + 1 + 3),*(dataToSend->massTimesAcc + i*6 + 2 + 3),
        *(dataToSend->massTimesAcc + i*6 + 3 + 3),*(dataToSend->massTimesAcc + i*6 + 4 + 3),*(dataToSend->massTimesAcc + i*6 + 5 + 3));
    }
    fprintf(filePointer,"Crane Global Position\n");
    fprintf(filePointer,"%lf %lf %lf\n",(dataToSend->craneGlobalPos[0]),(dataToSend->craneGlobalPos[1]),(dataToSend->craneGlobalPos[2]));

    fprintf(filePointer,"Applied Loads\n");
    for(int i=0; i<nFEMBodies;i++){
        fprintf(filePointer,"%lf %lf %lf %lf\n",*(dataToSend->appliedLoads + i*4 + 0),*(dataToSend->appliedLoads + i*4 + 1),*(dataToSend->appliedLoads + i*4 + 2),*(dataToSend->appliedLoads + i*4 + 3));
    }
	
    fclose(filePointer);
 
    return 0;

}

int rbdGetMemoryAndSetPointers(RBDDATATOSEND *dataToSend, int axisOfRotation[]){
    
    int bytesForInitialAng = 0;
    int numAngl = 0;
    for(int i=0; i<dataToSend->nBodies;i++){
        if(axisOfRotation[i]==4){
            bytesForInitialAng += 12;
            numAngl += 3;
        }else if(axisOfRotation[i]>4 || axisOfRotation[i]<0){
            std::cout << "invalid rotational axis, axis was: "<< axisOfRotation[i]<<", for body number "<< i << std::endl;
            return 1;
            
        }else{
            bytesForInitialAng += 2;
            numAngl += 1;
        }
    }
    std::cout << "numDoubles for Ang = "<< bytesForInitialAng << std::endl;
    //ints
    int bytesSpatFree = dataToSend->nBodies*3*sizeof(int);
    int bytesForAxisOfRotation = dataToSend->nBodies*sizeof(int);
    //doubles
    int bytesForCmPos = dataToSend->nBodies*3*2*sizeof(double);
    int bytesForMass = dataToSend->nBodies*sizeof(double);
    int bytesForInertia = dataToSend->nBodies*9*sizeof(double);
    int bytesForInitialTransl = dataToSend->nBodies*6*sizeof(double);
    int bytesForAngAcc = numAngl*sizeof(double);
    int bytesFormasTimesAcc = 21*sizeof(double);
    int bytesForcraneGlobalPos = 3*sizeof(double);
    int bytesForappliedLoads = 4*3*sizeof(double);
    dataToSend->numDoubleForInitialAng = bytesForInitialAng;
    bytesForInitialAng = bytesForInitialAng*sizeof(double);
   

    dataToSend->numBytes =  bytesSpatFree + bytesForAxisOfRotation + (bytesForCmPos + bytesForMass + bytesForInertia + bytesForInitialTransl + bytesForInitialAng + 
        bytesForAngAcc +bytesForappliedLoads + bytesForcraneGlobalPos + bytesFormasTimesAcc);

    dataToSend->data = (char*)calloc(dataToSend->numBytes,sizeof(char));

    int dataCounter = 0;
    dataToSend->axisOfRotations = (int *)(dataToSend->data + dataCounter);
    dataCounter = bytesForAxisOfRotation;
    memcpy(dataToSend->axisOfRotations, axisOfRotation, bytesForAxisOfRotation);

    dataToSend->spatialFreedom = (int *)(dataToSend->data + dataCounter);
    dataCounter += bytesSpatFree;

    dataToSend->centreOfMassCoord = (double *)(dataToSend->data + dataCounter);
    dataCounter += bytesForCmPos;

    dataToSend->mass = (double *)(dataToSend->data + dataCounter);
    dataCounter += bytesForMass;

    dataToSend->inertia = (double *)(dataToSend->data + dataCounter);
    dataCounter += bytesForInertia;

    dataToSend->initialAngularValues = (double *)(dataToSend->data + dataCounter);
    dataCounter += bytesForInitialAng;  

    dataToSend->initialTranslationalValues = (double *)(dataToSend->data + dataCounter);
    dataCounter += bytesForInitialTransl; 

    // Results:
	dataToSend->angularAcc = (double *)(dataToSend->data + dataCounter);
	dataCounter += bytesForAngAcc;

	dataToSend->massTimesAcc = (double *)(dataToSend->data + dataCounter);
	dataCounter += bytesFormasTimesAcc;//nBodies*6 -3, Size of dX vectors in RBD. -3 as ship is not moving

	dataToSend->craneGlobalPos = (double *)(dataToSend->data + dataCounter);
	dataCounter += bytesForcraneGlobalPos;

	dataToSend->appliedLoads = (double *)(dataToSend->data + dataCounter);
	dataCounter += bytesForappliedLoads;

    return dataToSend->numBytes;
}

int readRBDfile(RBDDATATOSEND *dataToSend, FILE *filePointer){

    using namespace std;
    char buffer[100] = "\n";
    int numBodies = dataToSend->nBodies;

    //Spatial freedoms
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;

    for(int i=0; i<numBodies;i++){
        fscanf(filePointer,"%d %d %d",(dataToSend->spatialFreedom + 3*i + 0), (dataToSend->spatialFreedom + 3*i + 1) , (dataToSend->spatialFreedom + 3*i + 2));
        //cout << *(dataToSend->spatialFreedom + 3*i + 0)<<", " <<*(dataToSend->spatialFreedom + 3*i + 1) <<", "<< *(dataToSend->spatialFreedom + 3*i + 2) << endl;
    }
    //fscanf(filePointer,"\n");

    // Centre of mass vectors
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;

    for(int i=0;i<numBodies*2;i++){
        double *row = dataToSend->centreOfMassCoord + i * 3;
        fscanf(filePointer,"%lf %lf %lf",row,row + 1,row + 2);
        //cout << *(dataToSend->centreOfMassCoord  + 3*i)<< ", " << *(dataToSend->centreOfMassCoord  + 3*i + 1) << ", " << *(dataToSend->centreOfMassCoord  + 3*i + 2) << endl;
    }
    //fscanf(filePointer,"\n");

    //Mass of the bodies
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;
    for(int i=0;i<numBodies;i++){
        fscanf(filePointer,"%lf",(dataToSend->mass+i));
        //cout << *(dataToSend->mass+i) << " ";
    }
    //fscanf(filePointer,"\n");
    //cout << endl;

    //inertia of the bodies
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;
    for(int i=0;i<numBodies;i++){
        for(int j=0; j<3;j++){
            fscanf(filePointer,"%lf %lf %lf", (dataToSend->inertia + i*9 + j*3 + 0),
                                               (dataToSend->inertia + i*9 + j*3 + 1),
                                               (dataToSend->inertia + i*9 + j*3 + 2));
            //cout << *(dataToSend->inertia + i*9 + j*3 + 0) << " " << *(dataToSend->inertia + i*9 + j*3 + 1) << " " << *(dataToSend->inertia + i*9 + j*3 + 2) << " " ;
        }
        
        //cout << endl;
    }
    //fscanf(filePointer,"\n");
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;

    for(int i=0; i< dataToSend->numDoubleForInitialAng;i++){
        fscanf(filePointer,"%lf",dataToSend->initialAngularValues+i );
        //cout << *(dataToSend->initialAngularValues+i) << " ";
    }
    //fscanf(filePointer,"\n");
   // cout << endl;

    fgets(buffer,sizeof(buffer),filePointer);
    //cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
   // cout << buffer;

    
    for(int i=0; i<numBodies;i++){
   
            double *row = dataToSend->initialTranslationalValues + i*6;
            fscanf(filePointer,"%lf %lf %lf %lf %lf %lf",row,row+1,row+2,row+3,row+4,row+5 );
           // cout << *row <<", "<< *(row+1)<<", "<<*(row+2)<<", "<< *(row+3)<<", "<<*(row+4)<<", "<<*(row+5) << " "<< endl;
    }
   
    
    return 0;

}

int freeRbdMemory(RBDDATATOSEND *dataToSend){
    free(dataToSend->data);
    return 0;
}