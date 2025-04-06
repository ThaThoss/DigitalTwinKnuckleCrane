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

int rbdGetMemoryAndSetPointers(RBDDATATOSEND *dataToSend, int axisOfRotation[]){
    
    int bytesForInitialAng = 0;
    for(int i=0; i<dataToSend->nBodies;i++){
        if(axisOfRotation[i]==4){
            bytesForInitialAng += 12;
        }else if(axisOfRotation[i]>4 || axisOfRotation[i]<0){
            std::cout << "invalid rotational axis, axis was: "<< axisOfRotation[i]<<", for body number "<< i << std::endl;
            return 1;
            
        }else{
            bytesForInitialAng += 2;
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
    dataToSend->numDoubleForInitialAng = bytesForInitialAng;
    bytesForInitialAng = bytesForInitialAng*sizeof(double);
    

    dataToSend->numBytes =  bytesSpatFree + bytesForAxisOfRotation + (bytesForCmPos + bytesForMass + bytesForInertia + bytesForInitialTransl + bytesForInitialAng);

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

    return dataToSend->numBytes;
}

int readRBDfile(RBDDATATOSEND *dataToSend, FILE *filePointer){

    using namespace std;
    char buffer[100] = "\n";
    int numBodies = dataToSend->nBodies;

    //Spatial freedoms
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;

    for(int i=0; i<numBodies;i++){
        fscanf(filePointer,"%d %d %d",(dataToSend->spatialFreedom + 3*i + 0), (dataToSend->spatialFreedom + 3*i + 1) , (dataToSend->spatialFreedom + 3*i + 2));
        cout << *(dataToSend->spatialFreedom + 3*i + 0)<<", " <<*(dataToSend->spatialFreedom + 3*i + 1) <<", "<< *(dataToSend->spatialFreedom + 3*i + 2) << endl;
    }
    //fscanf(filePointer,"\n");

    // Centre of mass vectors
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;

    for(int i=0;i<numBodies*2;i++){
        double *row = dataToSend->centreOfMassCoord + i * 3;
        fscanf(filePointer,"%lf %lf %lf",row,row + 1,row + 2);
        cout << *(dataToSend->centreOfMassCoord  + 3*i)<< ", " << *(dataToSend->centreOfMassCoord  + 3*i + 1) << ", " << *(dataToSend->centreOfMassCoord  + 3*i + 2) << endl;
    }
    //fscanf(filePointer,"\n");

    //Mass of the bodies
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;
    for(int i=0;i<numBodies;i++){
        fscanf(filePointer,"%lf",(dataToSend->mass+i));
        cout << *(dataToSend->mass+i) << " ";
    }
    //fscanf(filePointer,"\n");
    cout << endl;

    //inertia of the bodies
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;
    for(int i=0;i<numBodies;i++){
        for(int j=0; j<3;j++){
            fscanf(filePointer,"%lf %lf %lf", (dataToSend->inertia + i*9 + j*3 + 0),
                                               (dataToSend->inertia + i*9 + j*3 + 1),
                                               (dataToSend->inertia + i*9 + j*3 + 2));
            cout << *(dataToSend->inertia + i*9 + j*3 + 0) << " " << *(dataToSend->inertia + i*9 + j*3 + 1) << " " << *(dataToSend->inertia + i*9 + j*3 + 2) << " " ;
        }
        
        cout << endl;
    }
    //fscanf(filePointer,"\n");
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;

    for(int i=0; i< dataToSend->numDoubleForInitialAng;i++){
        fscanf(filePointer,"%lf",dataToSend->initialAngularValues+i );
        cout << *(dataToSend->initialAngularValues+i) << " ";
    }
    //fscanf(filePointer,"\n");
    cout << endl;

    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;
    fgets(buffer,sizeof(buffer),filePointer);
    cout << buffer;

    
    for(int i=0; i<numBodies;i++){
   
            double *row = dataToSend->initialTranslationalValues + i*6;
            fscanf(filePointer,"%lf %lf %lf %lf %lf %lf",row,row+1,row+2,row+3,row+4,row+5 );
            cout << *row <<", "<< *(row+1)<<", "<<*(row+2)<<", "<< *(row+3)<<", "<<*(row+4)<<", "<<*(row+5) << " "<< endl;
    }
   
    
    return 0;

}

int freeRbdMemory(RBDDATATOSEND *dataToSend){
    free(dataToSend->data);
    return 0;
}