#ifndef GLOBPOS
#define GLOBPOS

#include "MyDynStructs.h"
#include "dynfunctions.h"

#include "./../../lib/sendReceive.h"

class globalPosForCrane
{
private:

    struct Rotation R1;
    struct Rotation R2;
    struct Rotation R;
    double _theta1, _theta2, _dt;
    double radius[3] = {0};
    double _globalPos[3] = {0};
    double *globPointChange;
    double towerTop[3] {0};
    double c1toj3[3] = {-11.9, -7.5, 13.5+6.017};

    int16_t *_gogo;
    int16_t *_moveCrane;

    int _sockfd;
    unsigned char *dataToRecieve, *dataToSend;
    int _nCharsToReceive;
    int  _nCharsToSend;
    int _globPointPosDataPos;

    int setInitialRadandAngles(struct PhysicalValues myPhysicalValues, struct Frame Frames[4] );


public:

    globalPosForCrane(int _sockfd, double dt,struct PhysicalValues myPhysicalValues, struct Frame Frames[4]);
    ~globalPosForCrane();

    int getGlobalPosForCrane(double *globalPos);
  
    

    int gogo();
    int moveCrane();

    // Pack data for sending to server
    //dataToSend = [R1,th4,th5,th6, globCraneTipWantedPos];15 doubles;
    int sendData( struct Frame Frames[4],struct SolutionVectors SolutionVectors, struct EndEquationVector AngularAccelerations);
    int recieveData();

};

#endif