#define IXWEBSOCKET_USE_OPENSSL_LOGS
#include <ixwebsocket/IXWebSocketServer.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <cmath>

#include "./serverExternal.h"
#include "./../../lib/sendReceive.h"
#include "./../../lib/mySharedMemory.h"
#include "./../../lib/json.hpp"


sharedMemoryStructForIntegration *shmStru ;
using json = nlohmann::json;
using namespace std;

void serverKnuckleControllLoop( ix::WebSocketServer *serverKnuckle,ix::WebSocketServer *serverFEM, sharedMemoryStructForIntegration *shmStru){
     std::cout << "extra loop Starting" << std::endl;
    bool gogo = true;
    while(gogo){
        sleep(1);
        sem_wait(&(shmStru->semLock));
        if( shmStru->gogo==0){
            serverKnuckle->stop();
            serverFEM->stop();
            gogo=false;
            std::cout << "extra loop stopping" << std::endl;
        }
        sem_post(&(shmStru->semLock));
    }
     std::cout << "extra loop Stopped" << std::endl;
}


void getBodyMeshdata(sharedMemoryPointers *shrMemPointers, MESHDATA *meshData, sharedMemoryStructForIntegration *shmStru ){

    sem_wait(&(shmStru->semLock));

        memcpy(meshData->Elements, shrMemPointers->elements, meshData->header[0]*sizeof(int)*4);
        memcpy(meshData->Nodes, shrMemPointers->nodes, meshData->header[1]*sizeof(double)*2);
        memcpy(meshData->vonMieses, shrMemPointers->vonMieses, meshData->header[1]*sizeof(double)*1);

        memcpy(meshData->femJointPos, shmStru->femJointPos[meshData->bodyNumber], sizeof(double)*2);
        memcpy(meshData->femZeroPoint, shmStru->femZeroPoint[meshData->bodyNumber], sizeof(double)*2);
        memcpy(meshData->forceNodes,shrMemPointers->forceNodes,sizeof(int)*meshData->numForce);
        memcpy(meshData->forceGroup,shrMemPointers->forceGroup,sizeof(int)*meshData->numForce);
        meshData->deformationScale = shmStru->deformationScale[meshData->bodyNumber];
        meshData->bodyScale = shmStru->femBodyScale[meshData->bodyNumber];
        std::cout << "meshData->deformationScale = " << meshData->deformationScale << "in body: " << meshData->bodyNumber << std::endl;

    sem_post(&(shmStru->semLock));

}

void moveBodieForViz(MESHDATA *meshData){

    for(int i=0;i<meshData->header[1];i++){
        *(meshData->Nodes + i*2) = *(meshData->Nodes + i*2) - meshData->femZeroPoint[0];
        *(meshData->Nodes + i*2 +1 ) = *(meshData->Nodes + i*2 + 1) - meshData->femZeroPoint[1];
    }
}

int getPreviousJointPos(const MESHDATA* meshdata ,double moveToXY[2]){

    double X = 0;
    double Y = 0;
    int nNodesInGroup = 0;
    int nodeNumb = 0;

    for(int i=0; i<meshdata->numForce; i++){

        if(*(meshdata->forceGroup + i)==0){
            nodeNumb = *(meshdata->forceNodes + i);
            X += *(meshdata->deformation + nodeNumb*2 );
            Y += *(meshdata->deformation + nodeNumb*2 +1 );
            nNodesInGroup++;
        }
    }

    if(nNodesInGroup !=0){
        X = X/nNodesInGroup;
        Y = Y/nNodesInGroup;
    }else if(nNodesInGroup == 0){
        std::cout << "No nodes found in group 0 of body: " << meshdata->bodyNumber << std::endl;
        return 1;
    }
    moveToXY[0] = X;
    moveToXY[1] = Y;
return 0;
}

using namespace std;
int main(int argc, char *argv[]){

    int shmid, port, femPort;

    // Run a server on localhost at a given port.
    // Bound host name, max connections and listen backlog can also be passed in as parameters.
    if (argc >= 3) {
     	port = atoi(argv[1]);
        shmid  = atoi(argv[2]);
        femPort = atoi(argv[3]);
    }else{
        char msg[] = {"Socket and shmid needed as input nr 1 and 2"};
	    myError(msg);
    }

    cout << "FEM Port = "<< femPort << " , RBD Port = " << port << endl;

    std::string host("127.0.0.1"); // If you need this server to be accessible on a different machine, use "0.0.0.0"
    ix::WebSocketServer serverKnuckle(port, host);
    ix::WebSocketServer serverFEM(femPort, host);

        /*Shared memory section:*/

        // shmat to attach to shared memory
    shmStru = (sharedMemoryStructForIntegration*)shmat(shmid,NULL,0);
    if (shmStru == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }
    std::cout << "External Server Shared memory attatched" << std::endl;

    int bytesForPointer[4] = {0};
sem_wait(&(shmStru->semLock));
    int numFEMBodies = shmStru->numFEMBodies;
    int numForce[numFEMBodies] = {0};
    bytesForPointer[0] = shmStru->numBytesBefFEM;//5*sizeof(int)*3 set in main server
sem_post(&(shmStru->semLock));

    cout << "Waiting for femDataReady" << endl;
    //Wait for the main server to populate the shared memory
    int femDataReady = 0;
    while(femDataReady<numFEMBodies){
        sem_wait(&(shmStru->semLock));
        femDataReady = shmStru->femDataReady;
        sem_post(&(shmStru->semLock));
        sleep(1);
    }
    cout << "Done Waiting for femDataReady" << endl;
    
    sem_wait(&(shmStru->semLock));
    memcpy(numForce,shmStru->numForce,sizeof(double)*numFEMBodies);
    sem_post(&(shmStru->semLock));

    int headers[numFEMBodies][5] = {0};
    getFEMHeaders(headers, shmStru, numFEMBodies);

    /*
    for(int i=0; i<numFEMBodies;i++){
        cout << "Header " << i << endl;
        cout << headers[i][0] << " , " << headers[i][1] << " , " << headers[i][2] << " , " << headers[i][3] << " , " << headers[i][4] << endl; 
    }
*/


    MESHDATA meshdata[3];
    for(int i=0;i<numFEMBodies;i++){
        int memCounter = 0;
        memcpy(meshdata[i].header,headers[i],sizeof(int)*5);
        meshdata[i].bodyNumber = i;
        meshdata[i].numForce = numForce[i];

        // 4*ints for elements, 5* double for nodes, deformation, vonMieses, and forcenodes(ints) and force group (ints)
        meshdata[i].numBytes = meshdata->header[0]*sizeof(int)*4 + meshdata[i].header[1]*sizeof(double)*5 + numForce[i]*sizeof(int)*2;
        //cout << "meshdata["<<i<<"].numBytes = " << meshdata[i].numBytes << endl;

        meshdata[i].data = (char*)calloc(meshdata->numBytes,sizeof(char));
        if(!(meshdata[i].data)){
            cout << "Failed to allocate external server meshdata for body: " << i << endl;
            exit(1);
        }
        //cout << "Calloc succeded to allocate external server meshdata for body: " << i << endl;


        // Set pointers:
        meshdata[i].Elements = (int *)meshdata[i].data;
        memCounter += meshdata[i].header[0]*sizeof(int)*4;

        meshdata[i].Nodes = (double *)(meshdata[i].data + memCounter);
        memCounter += meshdata[i].header[1]*sizeof(double)*2;

        meshdata[i].deformation = (double *)(meshdata[i].data + memCounter);
        memCounter += meshdata[i].header[1]*sizeof(double)*2;

        meshdata[i].vonMieses = (double *)(meshdata[i].data + memCounter);
        memCounter += meshdata[i].header[1]*sizeof(double)*1;

        meshdata[i].forceNodes = (int *)(meshdata[i].data + memCounter);
        memCounter += numForce[i]*sizeof(int);

        meshdata[i].forceGroup = (int *)(meshdata[i].data + memCounter);
        memCounter += numForce[i]*sizeof(int);

    }
 

    sharedMemoryPointers shmPointers[3];
    for(int i=0;i<numFEMBodies;i++){
        distrubuteFemSharedMemPointers( shmStru, &shmPointers[i],  i, bytesForPointer[i] );
        bytesForPointer[i+1] = bytesForPointer[i] + calcBytesNeededForFEM(headers[i][0], headers[i][1], headers[i][2], headers[i][3], headers[i][4]);
        cout << "---------------Header------------------" << endl;
        cout << headers[i][0]<< " , " <<  headers[i][1] <<" , "<< headers[i][2] << " , " << headers[i][3] << " , "<< headers[i][4] << endl;

        getBodyMeshdata(&shmPointers[i], &meshdata[i], shmStru );

        // move mesh so it's point of rotation is at [0,0] for vizualisation
        moveBodieForViz(&meshdata[i]);

    }

    /*
    for(int i =0; i<numFEMBodies;i++){
        cout << "--------------- Force Nodes fomr external in Bodie: " << i << ",  with numFOrce = "<< meshdata[i].numForce <<"-----------------" << endl;
        for(int j=0; j< meshdata[i].numForce; j++ ){
            cout << *(meshdata[i].forceNodes + j) << "   " <<  *(meshdata[i].forceGroup +j) << endl;
        }
    }
*/

    

    //cout <<"before serverFEM in External" << endl;
    serverFEM.setOnClientMessageCallback([&serverFEM, shmPointers, meshdata](std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket & webSocket, const ix::WebSocketMessagePtr & msg) {
        // The ConnectionState object contains information about the connection,
        // at this point only the client ip address and the port.
        //std::cout << "Remote ip: " << connectionState->getRemoteIp() << std::endl;

        if (msg->type == ix::WebSocketMessageType::Open)
        {
            std::cout << "New FEM connection" << std::endl;

            // A connection state object is available, and has a default id
            // You can subclass ConnectionState and pass an alternate factory
            // to override it. It is useful if you want to store custom
            // attributes per connection (authenticated bool flag, attributes, etc...)
            std::cout << "id: " << connectionState->getId() << std::endl;

            // The uri the client did connect to.
            std::cout << "Uri: " << msg->openInfo.uri << std::endl;

            std::cout << "FEM Headers:" << std::endl;
            for (auto it : msg->openInfo.headers)
            {
                std::cout << "\t" << it.first << ": " << it.second << std::endl;
            }
        }
        else if (msg->type == ix::WebSocketMessageType::Message)
        {
           // std::cout << "FEM Received: " << msg->str << std::endl;
            

          
            sem_wait(&(shmStru->semLock));

                int numFemBodies = shmStru->numFEMBodies;
                int16_t gogo = shmStru->gogo;

            sem_post(&(shmStru->semLock));



            // Create the JSON object
            json output;
            output["numBodies"] = numFemBodies;
            
            double angle = 0;
            double moveX = 0, moveY = 0;
            for (int i = 0; i < numFemBodies; ++i) {

                sem_wait(&(shmStru->semLock));
                    memcpy(meshdata[i].deformation, shmPointers[i].deformedNodes, meshdata[i].header[1]*sizeof(double)*2);
                    memcpy(meshdata[i].vonMieses, shmPointers[i].vonMieses, meshdata[i].header[1]*sizeof(double)*1);
                    if(i>0){angle += shmStru->angles[i];}
                sem_post(&(shmStru->semLock));
                double cosTh = cos(-angle);
                double sinTh = sin(-angle);

                double moveToXY[2] = {0};
                

                if(i==0){// Do not rotate or translate body 0
                    for(int j=0;j<meshdata[i].header[1];j++){
                        //Add nodes and deformation, nodes are already moved to zero.
                        *(meshdata[i].deformation +j*2) = *(meshdata[i].deformation +j*2)*meshdata[i].deformationScale + *(meshdata[i].Nodes +j*2);
                        *(meshdata[i].deformation +j*2 +1) = *(meshdata[i].deformation +j*2 +1)*meshdata[i].deformationScale + *(meshdata[i].Nodes +j*2 +1);

                        //Scale bodies
                        *(meshdata[i].deformation +j*2) = *(meshdata[i].deformation +j*2)*meshdata[i].bodyScale;
                        *(meshdata[i].deformation +j*2 + 1) = *(meshdata[i].deformation +j*2 + 1)*meshdata[i].bodyScale;

                    }
                }else{
                    if(getPreviousJointPos(&meshdata[i-1], moveToXY)){
                        std::cout << "problems with getPreviousJointPos in body :" << i << endl;
                    }
                    moveX = moveToXY[0];
                    moveY = moveToXY[1];

                    for(int j=0;j<meshdata[i].header[1];j++){
                        //Add nodes and deformation, nodes are already moved to zero.
                        *(meshdata[i].deformation +j*2) = *(meshdata[i].deformation +j*2)*meshdata[i].deformationScale + *(meshdata[i].Nodes +j*2);
                        *(meshdata[i].deformation +j*2 +1) = *(meshdata[i].deformation +j*2 +1)*meshdata[i].deformationScale + *(meshdata[i].Nodes +j*2 +1);
                        
                        // Rotate mesh, if not the first body
                        double X = *(meshdata[i].deformation +j*2);
                        double Y = *(meshdata[i].deformation +j*2 +1);
                        *(meshdata[i].deformation +j*2) = X*cosTh - Y*sinTh;
                        *(meshdata[i].deformation +j*2 +1 ) = X*sinTh + Y*cosTh;

                        //Scale bodies
                        *(meshdata[i].deformation +j*2) = *(meshdata[i].deformation +j*2)*meshdata[i].bodyScale;
                        *(meshdata[i].deformation +j*2 + 1) = *(meshdata[i].deformation +j*2 + 1)*meshdata[i].bodyScale;

                        //Move body to joint
                        *(meshdata[i].deformation +j*2) = *(meshdata[i].deformation +j*2) + moveX;
                        *(meshdata[i].deformation +j*2 + 1) = *(meshdata[i].deformation +j*2 + 1) + moveY;

                    }
                }




                
            /*
                cout << "-----Nodes-----" << endl;
                for(int k=0; k<meshdata[i].header[1]; k++ ){
                    std::cout << "[ " << *(meshdata[i].Nodes + k*2) << " , " << *(meshdata[i].Nodes + k*2 +1) << " ]" << std::endl; 
                }
            */

                // Create a body object
                json body;

                json headerJson = json::array();

                    headerJson.push_back({meshdata[i].header[0],meshdata[i].header[1]});


                body["Header"] = headerJson;

                // Add Nodes
                json nodesJson = json::array();
                for (int j = 0; j < meshdata[i].header[1]; ++j) { // Loop over nodes
                    nodesJson.push_back({*(meshdata[i].deformation +j*2 ), *(meshdata[i].deformation +j*2 +1)});
                }
                body["Nodes"] = nodesJson;

                // Add Elements
                json elementsJson = json::array();
                for (int j = 0; j < meshdata[i].header[0]; ++j) { // Loop over elements
                    elementsJson.push_back({*(meshdata[i].Elements + j*4 + 0), *(meshdata[i].Elements + j*4 + 1), 
                                            *(meshdata[i].Elements + j*4 + 2), *(meshdata[i].Elements + j*4 + 3)});
                }
                body["Elements"] = elementsJson;

                // Add vonMieses
                json vonMiesesJson = json::array();
                for (int j = 0; j < meshdata[i].header[1]; ++j) { // Loop over vonMieses values
                    vonMiesesJson.push_back(*(meshdata[i].vonMieses + j));
                }
                body["vonMieses"] = vonMiesesJson;

                // Add the body to the output JSON
                output["Body" + std::to_string(i + 1)] = body;
            }

            //std::cout << "Sending back: " << myJasonObj.dump() << std::endl;
            webSocket.send(output.dump(), msg->binary);

            if(gogo == 0){
                webSocket.stop();
                std::cout << " recieved gogo = 0, shutting down" << std::endl;
            }
            
        }
    });
    
    //cout <<"before serverKnuckle in External" << endl;
    serverKnuckle.setOnClientMessageCallback([&serverKnuckle](std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket & webSocket, const ix::WebSocketMessagePtr & msg) {
        // The ConnectionState object contains information about the connection,
        // at this point only the client ip address and the port.
        std::cout << "Remote ip: " << connectionState->getRemoteIp() << std::endl;

        if (msg->type == ix::WebSocketMessageType::Open)
        {
            std::cout << "New connection" << std::endl;

            // A connection state object is available, and has a default id
            // You can subclass ConnectionState and pass an alternate factory
            // to override it. It is useful if you want to store custom
            // attributes per connection (authenticated bool flag, attributes, etc...)
            std::cout << "id: " << connectionState->getId() << std::endl;

            // The uri the client did connect to.
            std::cout << "Uri: " << msg->openInfo.uri << std::endl;

            std::cout << "RBD Headers:" << std::endl;
            for (auto it : msg->openInfo.headers)
            {
                std::cout << "\t" << it.first << ": " << it.second << std::endl;
            }
        }
        else if (msg->type == ix::WebSocketMessageType::Message)
        {
            //std::cout << "Received: " << msg->str << std::endl;
            


             // Parse the received JSON string
             json jasonRecieved;
             jasonRecieved = json::parse(msg->str);
   
 
            double R1[9] = {0};
            double angles[3] = {0};
            double globalPosPoint[3] = {0};


            //std::cout <<"received after jason parse: " << jasonRecieved.dump() << std::endl; 

            
            sem_wait(&(shmStru->semLock));

                memcpy(R1,shmStru->R1,SZ_DOUBLE*9);
                memcpy(angles,shmStru->angles,SZ_DOUBLE*3);
                memcpy(globalPosPoint,shmStru->globalPosPoint,SZ_DOUBLE*3);
                int16_t gogo = shmStru->gogo;

            sem_post(&(shmStru->semLock));
            for(int i=0;i<3;i++){
                if(abs(angles[i])<0.0001){
                    angles[i] = 0;
                }
                if(abs(globalPosPoint[i])<0.0001){
                    globalPosPoint[i] = 0;
                }
            }    

            std::vector<std::string> string_data;

            for (const auto& val : R1) {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(6) << val;  // set precision to 6
                string_data.push_back(stream.str());
            }

            std::vector<std::string> string_data2;

            for (const auto& val : angles) {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(6) << val;  // set precision to 6
                string_data2.push_back(stream.str());
            }

            std::vector<std::string> string_data3;

            for (const auto& val : globalPosPoint) {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(6) << val;  // set precision to 6
                string_data3.push_back(stream.str());
            }


            json myJasonObj;
            myJasonObj["R1"] = string_data;
            myJasonObj["angles"] = string_data2;
            myJasonObj["globalPosPoint"] = string_data3;
            myJasonObj["gogo"] = gogo;

            
            std::cout << "angles = [ " << angles[0] << ", " << angles[1] << ", " << angles[2] << "] "<< std::endl;
           /* std::cout << "globalPosPoint = [ " << globalPosPoint[0] << ", " << globalPosPoint[1] << ", " << globalPosPoint[2] << "] "<< std::endl;
            std::cout << "R1 = [ " << myJasonObj["R1"]<< std::endl;
            */

          
            webSocket.send(myJasonObj.dump(), msg->binary);

            if(gogo == 0){
               webSocket.stop();
               std::cout << " recieved gogo = 0, shutting down" << std::endl;
           }
            
        }
    });
    
    auto resKnuckle = serverKnuckle.listen();
    if (!resKnuckle.first) {
        cout << "something wrong with listen in serverExternal Knuckle" << endl; 
        std::cerr << "Error during listen: " << resKnuckle.second << std::endl;
        return 1;
    }

    auto resFem = serverFEM.listen();
    if (!resFem.first) {
        cout << "something wrong with listen in serverExternal FEM" << endl; 
        std::cerr << "Error during listen: " << resFem.second << std::endl;
        return 1;
    }
  
    // Per message deflate connection is enabled by default. It can be disabled
    // which might be helpful when running on low power devices such as a Rasbery Pi
    serverKnuckle.disablePerMessageDeflate();

    serverFEM.disablePerMessageDeflate();

    // Run the serverKnuckle in the background. serverKnuckle can be stoped by calling serverKnuckle.stop()
    serverKnuckle.start();
    cout << "External serverKnuckle ready and listening" << endl;
    serverFEM.start();
    cout << "External serverFEM ready and listening" << endl;
     
    thread controllThread = thread(serverKnuckleControllLoop, &serverKnuckle,&serverFEM,shmStru);

    // Block until serverKnuckle.stop() is called.
    serverKnuckle.wait();
    serverFEM.wait();
    controllThread.join();
    for(int i=0;i<numFEMBodies;i++){
        free(meshdata[i].data);
    }
    shmdt(shmStru); 
    cout << "External server closed" << endl;

}//main()
