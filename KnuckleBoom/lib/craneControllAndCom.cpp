
#include <stdlib.h>
#include <stdint.h>
#include <string.h> // memcpy
#include <iostream>

#include "craneControllAndCom.h"
#include "MyDynStructs.h"
#include "dynfunctions.h"
#include "./../../lib/commonDataSizes.h"

#include "./../../lib/sendReceive.h"

globalPosForCrane::globalPosForCrane(int sockfd, double dt,struct PhysicalValues myPhysicalValues,struct Frame Frames[4] )
{
  
  _theta1 = 0.0;
  _theta2 = 0.0;
  _dt = dt;
  _nCharsToSend = knuckleNBytesFromKnu; //Defined in commonDataSizes
  _globPointPosDataPos = 12*SZ_DOUBLE;
  _nCharsToReceive = knuckleNBytesToKnu; //Defined in commonDataSizes
 
  _sockfd = sockfd;

  dataToRecieve = (unsigned char*)malloc(_nCharsToReceive);


  dataToSend = (unsigned char*)malloc(_nCharsToSend);
   
  globPointChange = (double*)dataToRecieve;
  *(globPointChange + 0) = 0;
  *(globPointChange + 1) = 0;
  *(globPointChange + 2) = 0;

  _gogo = (int16_t*)(dataToRecieve+SZ_DOUBLE*3);
  *_gogo = 1;

  _moveCrane = (int16_t*)(dataToRecieve+SZ_DOUBLE*3 + sizeof(int16_t));
  *_moveCrane = 0;

  setInitialRadandAngles(myPhysicalValues, Frames );


}

globalPosForCrane::~globalPosForCrane(){

  free(dataToRecieve);
  free(dataToSend);

}

int globalPosForCrane::setInitialRadandAngles(struct PhysicalValues myPhysicalValues, struct Frame Frames[4] ){

  memcpy(_globalPos, myPhysicalValues.craneGlobCoord,SZ_DOUBLE*3);
  double PosInFrame1[3];
  towerTop[0] = myPhysicalValues.c1toj2Skew[1][2];
  towerTop[1] = -myPhysicalValues.c1toj2Skew[0][2];
  towerTop[2] = myPhysicalValues.c1toj2Skew[0][1] + 6.017;

  MatVecMult3by3(PosInFrame1,Frames[0].TlocR.R,_globalPos);
  double X = PosInFrame1[0] - myPhysicalValues.c1toj2Skew[1][2];
  double Y = PosInFrame1[1] + myPhysicalValues.c1toj2Skew[0][2];
  double Z = PosInFrame1[2] - (myPhysicalValues.c1toj2Skew[0][1] + 6.017);

  double xn = sqrt(X*X + Y*Y);
  if(abs(xn)>0.0001){
    if(X>0){
      _theta1 = asin(Y/xn);
    }else {
      _theta1 = M_PI - asin(Y/xn);
    }
  }

  radius[0] = sqrt(xn*xn + Z*Z);

  _theta2 = -asin(Z/radius[0]);

  return 0;
}




int globalPosForCrane::sendData( struct Frame Frames[4],struct SolutionVectors SolutionVectors, struct EndEquationVector AngularAccelerations)
{
  
  int doublesSent = 0;

  memcpy(dataToSend + SZ_DOUBLE*doublesSent,Frames[0].AbsR.R[0],SZ_DOUBLE*3);
  doublesSent += 3;
  memcpy(dataToSend + SZ_DOUBLE*doublesSent,Frames[0].AbsR.R[1],SZ_DOUBLE*3);
  doublesSent += 3;
  memcpy(dataToSend + SZ_DOUBLE*doublesSent,Frames[0].AbsR.R[2],SZ_DOUBLE*3);
  doublesSent += 3;
  memcpy(dataToSend + SZ_DOUBLE*doublesSent,(SolutionVectors.Sol.Vector+3),SZ_DOUBLE*3);//angles;
  doublesSent += 3;
  memcpy(dataToSend + SZ_DOUBLE*doublesSent,(SolutionVectors.SolV.Vector),SZ_DOUBLE*6);//angular velocities;//new
  doublesSent += 6;
  memcpy(dataToSend + SZ_DOUBLE*doublesSent,(AngularAccelerations.Vector),SZ_DOUBLE*6);//angular accelerations;//new
  doublesSent += 6;
  memcpy(dataToSend + SZ_DOUBLE*doublesSent,(SolutionVectors.maAndMom.Vector),SZ_DOUBLE*SolutionVectors.maAndMom.firstDim);//mass times acc and moments on each body. accelerations;//new
  doublesSent += SolutionVectors.maAndMom.firstDim;
  memcpy(dataToSend + SZ_DOUBLE*doublesSent,_globalPos,SZ_DOUBLE*3);
  doublesSent += 3;
/*
printf("----R1----\n");
  printf("%lf, %lf, %lf\n",Frames[0].AbsR.R[0][0],Frames[0].AbsR.R[0][1],Frames[0].AbsR.R[0][2]);
  printf("%lf, %lf, %lf\n",Frames[0].AbsR.R[1][0],Frames[0].AbsR.R[1][1],Frames[0].AbsR.R[1][2]);
  printf("%lf, %lf, %lf\n",Frames[0].AbsR.R[2][0],Frames[0].AbsR.R[2][1],Frames[0].AbsR.R[2][2]);
  printf("AngV: [%lf, %lf, %lf, %lf, %lf, %lf]\n",SolutionVectors.SolV.Vector[0],SolutionVectors.SolV.Vector[1],SolutionVectors.SolV.Vector[2],SolutionVectors.SolV.Vector[3],SolutionVectors.SolV.Vector[4],SolutionVectors.SolV.Vector[5]);
  printf("AngAcc: [%lf, %lf, %lf, %lf, %lf, %lf]\n",AngularAccelerations.Vector[0],AngularAccelerations.Vector[1],AngularAccelerations.Vector[2],AngularAccelerations.Vector[3],AngularAccelerations.Vector[4],AngularAccelerations.Vector[5]);
  printf("CraneGlobPos : [%lf, %lf, %lf]\n",_globalPos[0],_globalPos[1],_globalPos[2]);
  */
  //dataToSend = [R1,th4,th5,th6,dth1,dth2,dth3,dth4,dth5,dth6,ddth1,ddth2,ddth3,ddth4,ddth5,ddth6,ma_and_mom, globCraneTipWantedPos];48 doubles;
  if(SendNUnsignedChar(_sockfd, dataToSend, _nCharsToSend ))
  {
    printf("problems with SendNUnsignedChar in craneControll");
    return 1;
  }

  return 0;
}




int globalPosForCrane::recieveData()
{

  if(ReceiveNUnsignedChars(_sockfd,  dataToRecieve, _nCharsToReceive ))
  {
    printf("Problem with with recieve data in craneControll");
    return 1;
  }
  //dataToRecieve = {globPointChange[3], gogo, moveCrane}
  _theta1 = *(globPointChange + 1)*_dt + _theta1;
  _theta2 = *(globPointChange + 2)*_dt + _theta2;

  if(_theta1>5*M_PI/6){
    _theta1 = 5*M_PI/6;
  }else if(_theta1<-2*M_PI/6){
    _theta1 = -2*M_PI/6;
  }
  if(_theta2>(M_PI/3)){
    _theta2=(M_PI/3);
  }else if(_theta2<(-M_PI/2+0.001)){
    _theta2=(-M_PI/2+0.001);
  }

  getRotation( R1.R, 3, _theta1);
  getRotation( R2.R, 2, _theta2);
  MatMatMult3by3(R.R, R1.R, R2.R);
  radius[0] = *(globPointChange + 0)*_dt + radius[0];
  MatVecMult3by3(_globalPos,R.R,radius);
  VecVecAdd3(_globalPos,_globalPos,towerTop);
  /*
  std::cout << "globalPosInternal = [" << _globalPos[0] << ", " << _globalPos[1]
  <<", " << _globalPos[2] << " ]" << std::endl;

  std::cout << "thGlob = " << _theta1 << ", " << _theta2 << ", radius = " << radius[0] << std::endl;
  
  std::cout << "towerTop = [" << towerTop[0] << ", " << towerTop[1]
  <<", " << towerTop[2] << " ]" << std::endl;
*/
  
  return 0;
}

int globalPosForCrane::getGlobalPosForCrane(double* globalPos)
{
  memcpy(globalPos ,_globalPos,SZ_DOUBLE*3);
  return 0;
}

int globalPosForCrane::gogo()
{
  if(*_gogo==0){
    return 1;
  }
  return 0;
}

int globalPosForCrane::moveCrane()
{
  return (int)*_moveCrane;
}

int calcBytesNeededForRBD(int nBodies, int axisOfRotation[]){

	int bytesForInitialAng = 0;
    for(int i=0; i<nBodies;i++){
        if(axisOfRotation[i]==4){
            bytesForInitialAng += 12;
        }else if(axisOfRotation[i]>4 || axisOfRotation[i]<0){
            std::cout << "invalid rotational axis, axis was: "<< axisOfRotation[i] << ", for body number " << i << std::endl;
            return 1;
            
        }else{
            bytesForInitialAng += 2;
        }
    }
    std::cout << "numDoubles for Ang = "<< bytesForInitialAng << std::endl;

	//ints
	int bytesSpatFree = nBodies*3*sizeof(int);
	int bytesForAxisOfRotation = nBodies*sizeof(int);
	//doubles
	int bytesForCmPos = nBodies*3*2*sizeof(double);
	int bytesForMass = nBodies*sizeof(double);
	int bytesForInertia = nBodies*9*sizeof(double);
	int bytesForInitialTransl = nBodies*6*sizeof(double);
	bytesForInitialAng = bytesForInitialAng*sizeof(double);

	int numBytes =  bytesSpatFree + bytesForAxisOfRotation + (bytesForCmPos + bytesForMass + bytesForInertia + bytesForInitialTransl + bytesForInitialAng);

	return numBytes;

}