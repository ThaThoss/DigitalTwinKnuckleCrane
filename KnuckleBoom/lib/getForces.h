#ifndef GETFORCES
#define GETFORCES

#include <math.h>
#include "MyDynStructs.h"
#include "dynfunctions.h"


void get_Forces( struct EndEquationMatrices * endEquationMatrices, struct Frame Frames[4], struct ArrayData myArrayData, struct PhysicalValues myPhysicalValues )
{



  set_Gravity_to_F(&endEquationMatrices->F,myArrayData, myPhysicalValues);
  MatVecEndEq(&endEquationMatrices->Fstar, endEquationMatrices->BT, endEquationMatrices->F);

  /* Wave is set here : */
  /* Train wave 1 = sin(myArrayData.Time/3.0) * M_PI/24.0;*/
  /* Train wave 2 = sin(myArrayData.Time/6.0) * M_PI/12.0;;*/
  /* Test wave To That is Used = sin(myArrayData.Time/8.0) * M_PI/7.0;;*/
  /* Train wave 4 = sin(myArrayData.Time/8.0) * M_PI/6.0;;*/
  /* Last test : sin(myArrayData.Time/8.0) * M_PI/30.0;*/
  
  
  //Ship:---------------------------------
  double th1w =  sin(myArrayData.Time/10.0) * M_PI/6.0;/*sin(myArrayData.Time/3.0) * M_PI/24.0;*//* Roll */
  double th2w = 0;/*Yaw*/
  double th3w = 0;/*Pitch*/
  double dt = myArrayData.dt*1.0;


  endEquationMatrices->AngularAccelerations.Vector[0] = 2 * (th1w - endEquationMatrices->Angles.Vector[0] - endEquationMatrices->AngularVelocities.Vector[0]*dt)/(dt*dt);
  endEquationMatrices->AngularAccelerations.Vector[1] = 2 * (th2w - endEquationMatrices->Angles.Vector[1] - endEquationMatrices->AngularVelocities.Vector[1]*dt)/(dt*dt);
  endEquationMatrices->AngularAccelerations.Vector[2] = 2 * (th3w - endEquationMatrices->Angles.Vector[2] - endEquationMatrices->AngularVelocities.Vector[2]*dt)/(dt*dt);
//-----------------------------------------------

  double th4w = 0;
  double th5w = 0;
  double th6w = 0;


  if(myPhysicalValues.moveCrane)
  {
    /*Needed angles for crane */
    double PosInert[3] = {myPhysicalValues.craneGlobCoord[0],myPhysicalValues.craneGlobCoord[1],myPhysicalValues.craneGlobCoord[2]};
    //---------------------------------------------------------------------------

    double PosFrame1[3];
    MatVecMult3by3(PosFrame1,Frames[0].TlocR.R,PosInert);
    double X = PosFrame1[0] - myPhysicalValues.c1toj2Skew[1][2];
    double Y = PosFrame1[1] + myPhysicalValues.c1toj2Skew[0][2];
    double Z = PosFrame1[2] - (myPhysicalValues.c1toj2Skew[0][1] + 6.017);

    double xn = sqrt(X*X + Y*Y);
    if(abs(xn>0.001)){
      if(X>0){
        th4w = asin(Y/xn);
      }else if(X<0 && Y>0 ){
        th4w = M_PI - asin(Y/xn);
      }else if(X<0 && Y<0 ){
        th4w = -M_PI - asin(Y/xn);
      }
    }

    double c = sqrt(xn*xn + Z*Z);

    if(c>32){
      c=32.0;
      printf("Out of reach\n");
    }
    else if(c<11.6){
      c=11.6;
      printf("Too close");
    }
    
    double th5w1 = asin(Z/c);
    double th5w2 = acos((22.0*22.0 + c*c -10.4*10.4) / (2.0 * 22.0 * c));
    th5w = -(th5w1 + th5w2);

    double th6w1 = 0.5048845334502091+0.002037;
    double th6w2 = acos((22.0*22.0 +10.4*10.4 - c*c) / (2.0 * 10.4 * 22.0));  
    th6w = -(th6w2 - th6w1);

  }else{

    th4w = myPhysicalValues.craneAngles[0];
    th5w = myPhysicalValues.craneAngles[1];
    th6w = myPhysicalValues.craneAngles[2];

  }

  endEquationMatrices->AngularAccelerations.Vector[3] = 2 * (th4w - endEquationMatrices->Angles.Vector[3] - endEquationMatrices->AngularVelocities.Vector[3]*dt)/(dt*dt);
  endEquationMatrices->AngularAccelerations.Vector[4] = 2 * (th5w - endEquationMatrices->Angles.Vector[4] - endEquationMatrices->AngularVelocities.Vector[4]*dt)/(dt*dt);
  endEquationMatrices->AngularAccelerations.Vector[5] = 2 * (th6w - endEquationMatrices->Angles.Vector[5] - endEquationMatrices->AngularVelocities.Vector[5]*dt)/(dt*dt);

  MatVecEndEq(&endEquationMatrices->NstarVel,endEquationMatrices->Nstar,endEquationMatrices->AngularVelocities);
  MatVecEndEq(&endEquationMatrices->MstarAcc,endEquationMatrices->Mstar,endEquationMatrices->AngularAccelerations);
  VecVecAddEndEq(&endEquationMatrices->MstarAccAddNstarVel ,endEquationMatrices->NstarVel,endEquationMatrices->MstarAcc);
  VecVecSubEndEq(&endEquationMatrices->MstarAccAddNstarVelSubFstar,endEquationMatrices->MstarAccAddNstarVel,endEquationMatrices->Fstar);
 
  double tempV[3] = {0,endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[4],endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[3]};


  double tempV2[3];
  MatVecMult3by3(tempV2,Frames[1].locR.R,tempV);

  endEquationMatrices->waveMoments.Vector[0] =  endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[0];
  endEquationMatrices->waveMoments.Vector[1] =  endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[1];
  endEquationMatrices->waveMoments.Vector[2] =  endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[2];

  endEquationMatrices->F.Vector[0] = endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[0] - tempV2[0];
  endEquationMatrices->F.Vector[1] = endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[1] - tempV2[1];
  endEquationMatrices->F.Vector[2] = endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[2] - tempV2[2];

  endEquationMatrices->F.Vector[8] = endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[3];
  endEquationMatrices->F.Vector[13] = endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[4] - endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[5];
  endEquationMatrices->F.Vector[19] = endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector[5];

}
#endif