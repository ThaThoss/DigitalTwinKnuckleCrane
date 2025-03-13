
#include <ixwebsocket/IXWebSocketServer.h>
#include "./../../lib/mySharedMemory.h"

typedef struct {

    int bodyNumber = 0;
    int numBytes = 0;
    double femJointPos[2] = {0};
    double femZeroPoint[2] = {0};
    int header[5] = {0};

    int deformationScale = 1;
    int bodyScale = 1;

    double *Nodes;
    double *deformation;
    double *vonMieses;

    int *Elements;

    char *data;

}MESHDATA;


void serverKnuckleControllLoop( ix::WebSocketServer *serverKnuckle, ix::WebSocketServer *serverFEM, sharedMemoryStructForIntegration *shmStru);

void getBodyMeshdata(sharedMemoryPointers *shrMemPointers, MESHDATA *meshData, sharedMemoryStructForIntegration *shmStru );

void moveBodieForViz(MESHDATA *meshData,double zeroPoint[2]);


