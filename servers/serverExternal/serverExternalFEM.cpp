#define IXWEBSOCKET_USE_OPENSSL_LOGS
#include <ixwebsocket/IXWebSocketServer.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <typeinfo>

#include "./serverExternal.h"
#include "./../../lib/sendReceive.h"
#include "./../../lib/mySharedMemory.h"
#include "./../../lib/json.hpp"


sharedMemoryStructForIntegration *shmStru ;
using json = nlohmann::json;

void serverFEMControllLoop( ix::WebSocketServer *serverFEM, sharedMemoryStructForIntegration *shmStru){
     std::cout << "extra loop Starting" << std::endl;
    bool gogo = true;
    while(gogo){
        sleep(1);
        sem_wait(&(shmStru->semLock));
        if( shmStru->gogo==0){
            serverFEM->stop();
            gogo=false;
            std::cout << "extra loop FEM stopping" << std::endl;
        }
        sem_post(&(shmStru->semLock));
    }
     std::cout << "extra loop FEM Stopped" << std::endl;
}


void getBodyMeshdata(sharedMemoryPointers *shrMemPointers, MESHDATA *meshData, sharedMemoryStructForIntegration *shmStru ){
std::cout << "in getBodyMeshdata " << std::endl;
    sem_wait(&(shmStru->semLock));

        memcpy(meshData->Elements, shrMemPointers->elements, meshData->header[0]*sizeof(int)*4);
        memcpy(meshData->Nodes, shrMemPointers->nodes, meshData->header[1]*sizeof(double)*2);
        memcpy(meshData->vonMieses, shrMemPointers->vonMieses, meshData->header[1]*sizeof(double)*1);

    sem_post(&(shmStru->semLock));

}

void getBodyNodeAndVonMieses(sharedMemoryPointers *shrMemPointers, MESHDATA *meshData, sharedMemoryStructForIntegration *shmStru ){
std::cout << "in getBodyNodeAndVonMieses " << std::endl;
    sem_wait(&(shmStru->semLock));

        memcpy(meshData->Nodes, shrMemPointers->nodes, meshData->header[1]*sizeof(double)*2);
        memcpy(meshData->vonMieses, shrMemPointers->vonMieses, meshData->header[1]*sizeof(double)*1);

    sem_post(&(shmStru->semLock));

}



using namespace std;
int main(int argc, char *argv[]){

    int shmid, port, femPort;

    // Run a server on localhost at a given port.
    // Bound host name, max connections and listen backlog can also be passed in as parameters.
    if (argc >= 3) {
     	port = atoi(argv[1]);
        shmid  = atoi(argv[2]);
        femPort = atoi("8082");
    }else{
        char msg[] = {"Socket and shmid needed as input nr 1 and 2"};
	    myError(msg);
    }

    cout << "FEM Port = "<< femPort << " , RBD Port = " << port << endl;

    std::string host("127.0.0.1"); // If you need this server to be accessible on a different machine, use "0.0.0.0"

    ix::WebSocketServer serverFEM(femPort, host);
  
        /*Shared memory section:*/

        // shmat to attach to shared memory
    shmStru = (sharedMemoryStructForIntegration*)shmat(shmid,NULL,0);
    if (shmStru == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }
    std::cout << "External Server Shared memory attatched" << std::endl;

   


        int numFEMBodies = shmStru->numFEMBodies;
        int headers[numFEMBodies][5] = {0};
        getFEMHeaders(headers, shmStru, numFEMBodies);



    MESHDATA meshdata[3];
    for(int i=0;i<numFEMBodies;i++){
        int memCounter = 0;
        memcpy(meshdata[i].header,headers[i],sizeof(int)*5);
        meshdata[i].bodyNumber = i;
        meshdata[i].numBytes = meshdata->header[0]*sizeof(int)*4 + meshdata[i].header[1]*sizeof(double)*3;
        meshdata[i].data = (char*)calloc(meshdata->numBytes,sizeof(char));

        meshdata[i].Elements = (int *)meshdata[i].data;
        memCounter += meshdata[i].header[0]*sizeof(int)*4;

        meshdata[i].Nodes = (double *)(meshdata[i].data + memCounter);
        memCounter += meshdata[i].header[1]*sizeof(double)*2;


        meshdata[i].vonMieses = (double *)(meshdata[i].data + memCounter);
        memCounter += meshdata[i].header[1]*sizeof(double)*1;

    }




    int bytesForPointer[4] = {0};
    bytesForPointer[0] = sizeof(int)*5*numFEMBodies;
    sharedMemoryPointers shmPointers[3];
    for(int i=0;i<numFEMBodies;i++){
        bytesForPointer[i+1] += calcBytesNeededForFEM(headers[i][0], headers[i][1], headers[i][2], headers[i][3], headers[i][4]);
        distrubuteFemSharedMemPointers( shmStru, &shmPointers[i],  i, bytesForPointer[i+1] );

        getBodyMeshdata(&shmPointers[i], &meshdata[i], shmStru );
    }

    serverFEM.setOnClientMessageCallback([&serverFEM, shmPointers, meshdata](std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket & webSocket, const ix::WebSocketMessagePtr & msg) {
        // The ConnectionState object contains information about the connection,
        // at this point only the client ip address and the port.
        std::cout << "Remote ip: " << connectionState->getRemoteIp() << std::endl;

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
            std::cout << "Received: " << msg->str << std::endl;
            


             // Parse the received JSON string
             /*
             json jasonRecieved;
             jasonRecieved = json::parse(msg->str);
   
 
            std::cout <<"received after jason parse: " << jasonRecieved.dump() << std::endl; 
*/
            std::cout << "bef get numFEMBodies" << std::endl;
            sem_wait(&(shmStru->semLock));

                int numFemBodies = shmStru->numFEMBodies;
                int16_t gogo = shmStru->gogo;

            sem_post(&(shmStru->semLock));



            // Create the JSON object
            json output;
            output["numBodies"] = numFemBodies;
            

            for (int i = 0; i < numFemBodies; ++i) {

                sem_wait(&(shmStru->semLock));
                    memcpy(meshdata[i].Nodes, shmPointers[i].nodes, meshdata[i].header[1]*sizeof(double)*2);
                    memcpy(meshdata[i].vonMieses, shmPointers[i].vonMieses, meshdata[i].header[1]*sizeof(double)*1);
                sem_post(&(shmStru->semLock));


                // Create a body object
                json body;

                json headerJson = json::array();

                    headerJson.push_back({meshdata[i].header[0],meshdata[i].header[1]});


                body["Header"] = headerJson;

                // Add Nodes
                json nodesJson = json::array();
                for (int j = 0; j < meshdata[i].header[1]; ++j) { // Loop over nodes
                    nodesJson.push_back({*(meshdata[i].Nodes +j*2 ), *(meshdata[i].Nodes +j*2 +1)});
                }
                body["Nodes"] = nodesJson;

                // Add Elements
                json elementsJson = json::array();
                for (int j = 0; meshdata[i].header[0] < 2; ++j) { // Loop over elements
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
  
    auto resFem = serverFEM.listen();
    if (!resFem.first) {
        cout << "something wrong with listen" << endl; 
        std::cerr << "Error during listen: " << resFem.second << std::endl;
        return 1;
    }
  
    // Per message deflate connection is enabled by default. It can be disabled
    // which might be helpful when running on low power devices such as a Rasbery Pi
    serverFEM.disablePerMessageDeflate();

    // Run the serverKnuckle in the background. serverKnuckle can be stoped by calling serverKnuckle.stop()
    serverFEM.start();
    cout << "External serverFEM ready and listening" << endl;
     
    thread controllThread = thread(serverFEMControllLoop,&serverFEM,shmStru);

    // Block until serverKnuckle.stop() is called.
    
    serverFEM.wait();
    controllThread.join();
    cout << "External server closed" << endl;

}//main()
