

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> // memcpy

#include "dynfunctions.h"
#include "MyDynStructs.h"

int SetMassMatrix(struct EndEquationMatrix M){
    int N = M.firstDim;
    
    M.Matrix[0*N + 0] =  2267.5;
    M.Matrix[1*N + 0] = -989.57;
    M.Matrix[2*N + 0] = -1.5592;
    M.Matrix[0*N + 1] = -989.57;
    M.Matrix[1*N + 1] = 8698.78;
    M.Matrix[2*N + 1] = -1.9803;
    M.Matrix[0*N + 2] = -1.5592;
    M.Matrix[1*N + 2] = -1.9803;
    M.Matrix[2*N + 2] =  9428.4;

    M.Matrix[3*N + 3] = 160;
    M.Matrix[4*N + 4] = 160;
    M.Matrix[5*N + 5] = 160;

    M.Matrix[6*N + 6] =  0.9843;
    M.Matrix[7*N + 6] = 0;
    M.Matrix[8*N + 6] = 0;
    M.Matrix[6*N + 7] = 0;
    M.Matrix[7*N + 7] = 0.6945;
    M.Matrix[8*N + 7] = 0.0689;
    M.Matrix[6*N + 8] = 0;
    M.Matrix[7*N + 8] = 0.0689;
    M.Matrix[8*N + 8] = 1.007;

    M.Matrix[9*N + 9] = 150;
    M.Matrix[10*N + 10] = 150;
    M.Matrix[11*N + 11] = 150;

    M.Matrix[12*N + 12] = 0.475;
    M.Matrix[13*N + 12] = 0;
    M.Matrix[14*N + 12] = 0.0161;
    M.Matrix[12*N + 13] = 0;
    M.Matrix[13*N + 13] = 5.914;
    M.Matrix[14*N + 13] = 0;
    M.Matrix[12*N + 14] = 0.0161;
    M.Matrix[13*N + 14] = 0;
    M.Matrix[14*N + 14] = 5.757;

    M.Matrix[15*N + 15] = 150;
    M.Matrix[16*N + 16] = 150;
    M.Matrix[17*N + 17] = 150;

    M.Matrix[18*N + 18] = 0.425;
    M.Matrix[19*N + 18] = 0;
    M.Matrix[20*N + 18] = 0.57;
    M.Matrix[18*N + 19] = 0;
    M.Matrix[19*N + 19] = 1.688;
    M.Matrix[20*N + 19] = 0;
    M.Matrix[18*N + 20] = 0.57;
    M.Matrix[19*N + 20] = 0;
    M.Matrix[20*N + 20] = 1.787;

  return 0;
}


void setSkewPositions(struct PhysicalValues * myPhysicalValues){


  myPhysicalValues->c1toj2Skew[0][0] = 0;
  myPhysicalValues->c1toj2Skew[1][0] = -13.5;
  myPhysicalValues->c1toj2Skew[2][0] = 7.5;
  myPhysicalValues->c1toj2Skew[0][1] = 13.5;
  myPhysicalValues->c1toj2Skew[1][1] = 0;
  myPhysicalValues->c1toj2Skew[2][1] = -11.9;
  myPhysicalValues->c1toj2Skew[0][2] = -7.5;
  myPhysicalValues->c1toj2Skew[1][2] = 11.9;
  myPhysicalValues->c1toj2Skew[2][2] = 0;

  myPhysicalValues->j2toc2Skew[0][0] = 0;
  myPhysicalValues->j2toc2Skew[1][0] = -2.6;
  myPhysicalValues->j2toc2Skew[2][0] = 0;
  myPhysicalValues->j2toc2Skew[0][1] = 2.6;
  myPhysicalValues->j2toc2Skew[1][1] = 0;
  myPhysicalValues->j2toc2Skew[2][1] = 0;
  myPhysicalValues->j2toc2Skew[0][2] = 0;
  myPhysicalValues->j2toc2Skew[1][2] = 0;
  myPhysicalValues->j2toc2Skew[2][2] = 0;

  myPhysicalValues->c2toj3Skew[0][0] = 0;
  myPhysicalValues->c2toj3Skew[1][0] = -3.5;
  myPhysicalValues->c2toj3Skew[2][0] = 0;
  myPhysicalValues->c2toj3Skew[0][1] = 3.5;
  myPhysicalValues->c2toj3Skew[1][1] = 0;
  myPhysicalValues->c2toj3Skew[2][1] = 0;
  myPhysicalValues->c2toj3Skew[0][2] = 0;
  myPhysicalValues->c2toj3Skew[1][2] = 0;
  myPhysicalValues->c2toj3Skew[2][2] = 0;

  myPhysicalValues->j3toc3Skew[0][0] = 0;
  myPhysicalValues->j3toc3Skew[1][0] = 0;
  myPhysicalValues->j3toc3Skew[2][0] = 0;
  myPhysicalValues->j3toc3Skew[0][1] = 0;
  myPhysicalValues->j3toc3Skew[1][1] = 0;
  myPhysicalValues->j3toc3Skew[2][1] = -10.7;
  myPhysicalValues->j3toc3Skew[0][2] = 0;
  myPhysicalValues->j3toc3Skew[1][2] = 10.7;
  myPhysicalValues->j3toc3Skew[2][2] = 0;  

  myPhysicalValues->c3toj4Skew[0][0] = 0;
  myPhysicalValues->c3toj4Skew[1][0] = 0;
  myPhysicalValues->c3toj4Skew[2][0] = 0;
  myPhysicalValues->c3toj4Skew[0][1] = 0;
  myPhysicalValues->c3toj4Skew[1][1] = 0;
  myPhysicalValues->c3toj4Skew[2][1] = -11.3;
  myPhysicalValues->c3toj4Skew[0][2] = 0;
  myPhysicalValues->c3toj4Skew[1][2] = 11.3;
  myPhysicalValues->c3toj4Skew[2][2] = 0; 

  myPhysicalValues->j4toc4Skew[0][0] = 0;
  myPhysicalValues->j4toc4Skew[1][0] = 2.4;
  myPhysicalValues->j4toc4Skew[2][0] = 0;
  myPhysicalValues->j4toc4Skew[0][1] = -2.4;
  myPhysicalValues->j4toc4Skew[1][1] = 0;
  myPhysicalValues->j4toc4Skew[2][1] = 4.1;
  myPhysicalValues->j4toc4Skew[0][2] = 0;
  myPhysicalValues->j4toc4Skew[1][2] = -4.1;
  myPhysicalValues->j4toc4Skew[2][2] = 0; 


  myPhysicalValues->numLinks = 4;

  myPhysicalValues->craneGlobCoord[0] = 0;
  myPhysicalValues->craneGlobCoord[1] = 0;
  myPhysicalValues->craneGlobCoord[2] = 0;

  myPhysicalValues->Masses[0] = 9000;
  myPhysicalValues->Masses[1] = 160;
  myPhysicalValues->Masses[2] = 150;
  myPhysicalValues->Masses[3] = 150;


}

void set_Gravity_to_F(struct EndEquationVector * F, struct ArrayData myArrayData, struct PhysicalValues myPhysicalValues  ){
  int MassCount = 1;
  int GravityCount = 5;
  for (int i =0; i<myArrayData.numCartesianDim;i++){
    if(i==GravityCount){
    F->Vector[i] = -9.81*myPhysicalValues.Masses[MassCount];
    GravityCount += 6;
    MassCount++;
  }else{F->Vector[i] = 0;}
  }
}

void SolutionVectorsAllocater(struct SolutionVectors * mySolutionVectors, struct ArrayData myArrayData){

mySolutionVectors->Sol.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->Sol.firstDim = 6;

mySolutionVectors->SolV.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->SolV.firstDim = 6;

mySolutionVectors->k1.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->k1.firstDim = 6;

mySolutionVectors->k2.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->k2.firstDim = 6;

mySolutionVectors->Ang.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->Ang.firstDim = 6;

mySolutionVectors->AngV.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->AngV.firstDim = 6;

mySolutionVectors->TempAng.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->TempAng.firstDim = 6;

mySolutionVectors->TempAngV.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->TempAngV.firstDim = 6;

mySolutionVectors->TempSol.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->TempSol.firstDim = 6;

mySolutionVectors->TempSolV.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
 mySolutionVectors->TempSolV.firstDim = 6;

mySolutionVectors->maAndMom.Vector = (double *)calloc(myArrayData.numCartesianDim, sizeof(double));
 mySolutionVectors->maAndMom.firstDim = myArrayData.numCartesianDim;//21

}

/*Memory Allocation */
int endEquationAllocater(struct EndEquationMatrices *endEquationMatrices, struct ArrayData myArrayData)
{


/*----Matrices-----*/

/* Mass Matrix*/
  endEquationMatrices->Mass.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numCartesianDim,sizeof(double));
  endEquationMatrices->Mass.firstDim = 21;
  endEquationMatrices->Mass.secondDim = 21;

/* D matrix */
  endEquationMatrices->D.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numCartesianDim,sizeof(double));
  endEquationMatrices->D.firstDim = 21;
  endEquationMatrices->D.secondDim = 21;

/* B Matrix */
  endEquationMatrices->B.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->B.firstDim = 21;
  endEquationMatrices->B.secondDim = 6;

/* B transpose Matrix */
  endEquationMatrices->BT.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->BT.firstDim = 6;
  endEquationMatrices->BT.secondDim = 21;

/* Time derivative of B */
  endEquationMatrices->dB.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->dB.firstDim = 21;
  endEquationMatrices->dB.secondDim = 6; 

/* M multiplied By B */
  endEquationMatrices->MB.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->MB.firstDim = 21;
  endEquationMatrices->MB.secondDim = 6; 

/*D Multiplied with M multiplied By B */
  endEquationMatrices->DMB.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->DMB.firstDim = 21;
  endEquationMatrices->DMB.secondDim = 6; 

/*M Multiplied with dB */
  endEquationMatrices->MdB.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->MdB.firstDim = 21;
  endEquationMatrices->MdB.secondDim = 6; 

/*M Multiplied with dB added with DMB */
  endEquationMatrices->MdBDMB.Matrix = (double *)calloc(myArrayData.numCartesianDim*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->MdBDMB.firstDim = 21;
  endEquationMatrices->MdBDMB.secondDim = 6; 

/* M star matrix */
  endEquationMatrices->Mstar.Matrix = (double *)calloc(myArrayData.numGenCoord*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->Mstar.firstDim = 6;
  endEquationMatrices->Mstar.secondDim = 6; 

/* M star Inverse matrix */
  endEquationMatrices->MstarI.Matrix = (double *)calloc(myArrayData.numGenCoord*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->MstarI.firstDim = 6;
  endEquationMatrices->MstarI.secondDim = 6; 

/* N star matrix */
  endEquationMatrices->Nstar.Matrix = (double *)calloc(myArrayData.numGenCoord*myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->Nstar.firstDim = 6;
  endEquationMatrices->Nstar.secondDim = 6; 

/*-----Vectors-----*/

/* Force Vector */
  endEquationMatrices->F.Vector = (double *)calloc(myArrayData.numCartesianDim,sizeof(double));
  endEquationMatrices->F.firstDim = 21;

/* F star vector */
  endEquationMatrices->Fstar.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->Fstar.firstDim = 6;


/* Nstar multiplied with qn */    
  endEquationMatrices->NStarqn.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->NStarqn.firstDim = 6;

/* F star minus Nstar multiplied with qn */    
  endEquationMatrices->FmNstarqn.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->FmNstarqn.firstDim = 6; 

/* Angles */    
  endEquationMatrices->Angles.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->Angles.firstDim = 6; 

/* Angular Velocities */    
  endEquationMatrices->AngularVelocities.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->AngularVelocities.firstDim = 6; 

/* Angular Accelerations */    
  endEquationMatrices->AngularAccelerations.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->AngularAccelerations.firstDim = 6;  

  endEquationMatrices->NstarVel.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->NstarVel.firstDim = 6; 

  endEquationMatrices->MstarAcc.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->MstarAcc.firstDim = 6; 

  endEquationMatrices->MstarAccAddNstarVel.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->MstarAccAddNstarVel.firstDim = 6; 

  endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector = (double *)calloc(myArrayData.numGenCoord,sizeof(double));
  endEquationMatrices->MstarAccAddNstarVelSubFstar.firstDim = 6; 

  endEquationMatrices->waveMoments.Vector = (double *)calloc(3,sizeof(double));
  endEquationMatrices->waveMoments.firstDim = 3;

  endEquationMatrices->CraneMoments.Vector = (double *)calloc(3,sizeof(double));
  endEquationMatrices->CraneMoments.firstDim = 3; 

  endEquationMatrices->MomTow.Vector = (double *)calloc(200000,sizeof(double));
  endEquationMatrices->MomTow.firstDim = 50000; 

  endEquationMatrices->MomBoom.Vector = (double *)calloc(200000,sizeof(double));
  endEquationMatrices->MomBoom.firstDim = 50000; 

  endEquationMatrices->MomStick.Vector = (double *)calloc(200000,sizeof(double));
  endEquationMatrices->MomStick.firstDim = 50000; 

  /*Vectors used for ma and mom calculations for FEM*/
  endEquationMatrices->MdBDMBdq.Vector = (double *)calloc(myArrayData.numCartesianDim, sizeof(double));
  endEquationMatrices->MdBDMBdq.firstDim = myArrayData.numCartesianDim;//21

  endEquationMatrices->MBddq.Vector = (double *)calloc(myArrayData.numCartesianDim, sizeof(double));
  endEquationMatrices->MBddq.firstDim = myArrayData.numCartesianDim;//21

return 0;
}

/* Set a matrix to zero */
int zeroMatrix(double *Matrix, int N,int M){

  for (int i= 0; i<N; i++ ){
    for(int j = 0; j<M; j++ ){
        Matrix[i*3+j] = 0;
    }
  }

return 1;
}

void MatMatMult3by3(double ResultMat[3][3], double const Mat1[3][3], double const Mat2[3][3])
{

ResultMat[0][0] = Mat1[0][0]*Mat2[0][0] + Mat1[0][1]*Mat2[1][0] + Mat1[0][2]*Mat2[2][0];
ResultMat[1][0] = Mat1[1][0]*Mat2[0][0] + Mat1[1][1]*Mat2[1][0] + Mat1[1][2]*Mat2[2][0];
ResultMat[2][0] = Mat1[2][0]*Mat2[0][0] + Mat1[2][1]*Mat2[1][0] + Mat1[2][2]*Mat2[2][0];

ResultMat[0][1] = Mat1[0][0]*Mat2[0][1] + Mat1[0][1]*Mat2[1][1] + Mat1[0][2]*Mat2[2][1];
ResultMat[1][1] = Mat1[1][0]*Mat2[0][1] + Mat1[1][1]*Mat2[1][1] + Mat1[1][2]*Mat2[2][1];
ResultMat[2][1] = Mat1[2][0]*Mat2[0][1] + Mat1[2][1]*Mat2[1][1] + Mat1[2][2]*Mat2[2][1];

ResultMat[0][2] = Mat1[0][0]*Mat2[0][2] + Mat1[0][1]*Mat2[1][2] + Mat1[0][2]*Mat2[2][2];
ResultMat[1][2] = Mat1[1][0]*Mat2[0][2] + Mat1[1][1]*Mat2[1][2] + Mat1[1][2]*Mat2[2][2];
ResultMat[2][2] = Mat1[2][0]*Mat2[0][2] + Mat1[2][1]*Mat2[1][2] + Mat1[2][2]*Mat2[2][2];

}

/* Matrix Vector Multiplier, for 3 by 3 matrices  */
void MatVecMult3by3(double ResultVec[3] ,double const Mat1[3][3], double const Vec[3])
{
ResultVec[0] = Mat1[0][0]*Vec[0] + Mat1[0][1]*Vec[1] + Mat1[0][2]*Vec[2];
ResultVec[1] = Mat1[1][0]*Vec[0] + Mat1[1][1]*Vec[1] + Mat1[1][2]*Vec[2];
ResultVec[2] = Mat1[2][0]*Vec[0] + Mat1[2][1]*Vec[1] + Mat1[2][2]*Vec[2];
}

/* Matrix Vector Multiplier, for 3 by 3 matrices  */
void MatTransVecMult3by3(double ResultVec[3] ,double const Mat1[3][3], double const Vec[3])
{
ResultVec[0] = Mat1[0][0]*Vec[0] + Mat1[1][0]*Vec[1] + Mat1[2][0]*Vec[2];
ResultVec[1] = Mat1[0][1]*Vec[0] + Mat1[1][1]*Vec[1] + Mat1[2][1]*Vec[2];
ResultVec[2] = Mat1[0][2]*Vec[0] + Mat1[1][2]*Vec[1] + Mat1[2][2]*Vec[2];
}

/* Matrix Multiplied By Constant */
void MatConstMult3by3(double ResultMat[3][3] ,double const Mat1[3][3], double const Constant)
{
ResultMat[0][0] = Mat1[0][0]*Constant;
ResultMat[1][0] = Mat1[1][0]*Constant;
ResultMat[2][0] = Mat1[2][0]*Constant;

ResultMat[0][1] = Mat1[0][1]*Constant;
ResultMat[1][1] = Mat1[1][1]*Constant;
ResultMat[2][1] = Mat1[2][1]*Constant;

ResultMat[0][2] = Mat1[0][2]*Constant;
ResultMat[1][2] = Mat1[1][2]*Constant;
ResultMat[2][2] = Mat1[2][2]*Constant;
}

void MatMatAdd3by3(double ResultMat[3][3], double Mat1[3][3], double Mat2[3][3])
{
ResultMat[0][0] = Mat1[0][0] + Mat2[0][0];
ResultMat[1][0] = Mat1[1][0] + Mat2[1][0];
ResultMat[2][0] = Mat1[2][0] + Mat2[2][0];

ResultMat[0][1] = Mat1[0][1] + Mat2[0][1];
ResultMat[1][1] = Mat1[1][1] + Mat2[1][1];
ResultMat[2][1] = Mat1[2][1] + Mat2[2][1];

ResultMat[0][2] = Mat1[0][2] + Mat2[0][2];
ResultMat[1][2] = Mat1[1][2] + Mat2[1][2];
ResultMat[2][2] = Mat1[2][2] + Mat2[2][2];
}

void copyMatrix3by3(double MatOut[3][3], double MatInn[3][3]){

MatOut[0][0] = MatInn[0][0];
MatOut[0][1] = MatInn[0][1];
MatOut[0][2] = MatInn[0][2];

MatOut[1][0] = MatInn[1][0];
MatOut[1][1] = MatInn[1][1];
MatOut[1][2] = MatInn[1][2];

MatOut[2][0] = MatInn[2][0];
MatOut[2][1] = MatInn[2][1];
MatOut[2][2] = MatInn[2][2];

}


void VecVecAdd3(double ResVec[3], double Vec1[3], double Vec2[3]){
  
  ResVec[0] = Vec1[0]+Vec2[0];
  ResVec[1] = Vec1[1]+Vec2[1];
  ResVec[2] = Vec1[2]+Vec2[2];

}

void VecVecSub3(double ResVec[3], double Vec1[3], double Vec2[3]){
  
  ResVec[0] = Vec1[0]-Vec2[0];
  ResVec[1] = Vec1[1]-Vec2[1];
  ResVec[2] = Vec1[2]-Vec2[2];

}

/* Get Transpose */
void Transpose3by3(double RetMatr[3][3], double const Matrix[3][3] )
{
    for(int i=0; i<3;i++){
        for(int j=0;j<3;j++){
            RetMatr[i][j] = Matrix[j][i];
        }
    }
}

/* Get principal rotation matrices */
int getRotation(double Matrix[3][3], int axis, double angle){

  double cs = cos(angle);
  double si = sin(angle);

  switch(axis)
  {
    case 1:
	   Matrix[0][0] = 1; Matrix[0][1] =  0; Matrix[0][2] =   0; 
	   Matrix[1][0] = 0; Matrix[1][1] = cs; Matrix[1][2] = -si;
	   Matrix[2][0] = 0; Matrix[2][1] = si; Matrix[2][2] =  cs;
	   return 1;
    case 2:
	   Matrix[0][0] = cs;  Matrix[0][1] = 0; Matrix[0][2] = si;
	   Matrix[1][0] =  0;  Matrix[1][1] = 1; Matrix[1][2] =  0;
	   Matrix[2][0] = -si; Matrix[2][1] = 0; Matrix[2][2] = cs;
	   return 1;
    case 3:
	   Matrix[0][0] = cs; Matrix[0][1] = -si; Matrix[0][2] = 0;
	   Matrix[1][0] = si; Matrix[1][1] =  cs; Matrix[1][2] = 0;
	   Matrix[2][0] =  0; Matrix[2][1] =   0; Matrix[2][2] = 1;
	   return 1;
    default:
	   printf("Problem with getRotation, axis was: %d \n",axis);
	   return 0;
  }
return 1;
}

/* Get the time differentiated principal rotation matrices */
int getdiffRotation(double Matrix[3][3], int axis, double angle, double angularVelocity)
{
  double cs = cos(angle)*angularVelocity;
  double si = sin(angle)*angularVelocity;

  switch(axis)
  {
    case 1:
            Matrix[0][0] = 0; Matrix[0][1] =   0; Matrix[0][2] =   0;
            Matrix[1][0] = 0; Matrix[1][1] = -si; Matrix[1][2] = -cs;
            Matrix[2][0] = 0; Matrix[2][1] =  cs; Matrix[2][2] = -si;
	    return 1;
    case 2:
            Matrix[0][0] = -si; Matrix[0][1] = 0; Matrix[0][2] =  cs;
            Matrix[1][0] =   0; Matrix[1][1] = 0; Matrix[1][2] =   0;
            Matrix[2][0] = -cs; Matrix[2][1] = 0; Matrix[2][2] = -si;
	    return 1;
    case 3:
            Matrix[0][0] = -si; Matrix[0][1] = -cs; Matrix[0][2] = 0;
            Matrix[1][0] =  cs; Matrix[1][1] = -si; Matrix[1][2] = 0;
            Matrix[2][0] =   0; Matrix[2][1] =   0; Matrix[2][2] = 0;
	    return 1;
    default:
            printf("Problem with getdiffRotation, axis was: %d \n",axis);
            return 0;
  }
 return 1;
}

/* Print Matrix, used for debugging */
int PrintMyEndMatrix(struct EndEquationMatrix Matrix)
{
  int row = Matrix.firstDim;
  int col = Matrix.secondDim;

  int c = 1;
for (int i = 0; i<row; i++) {

  if(i == 3*c ){
        printf("\n");
        c++;
        /* jumper+=3; */
        }

	for (int j =0/* jumper */; j<col/*jumper+3*/; j++) {
		printf("%lf",Matrix.Matrix[i*col + j]); printf(", ");
    
	}
	printf(";\n");

}
printf("\n\n");
return 1;
}

void Print3by3(double Matrix[3][3]){

for(int i=0;i<3;i++){
  for(int j=0;j<3;j++){
    printf("%lf, ",Matrix[i][j]);
  }printf(";\n");
}
printf("\n\n");

}

int MatMatMultEndEqMat(struct EndEquationMatrix * MatOut, struct EndEquationMatrix const Mat1,struct EndEquationMatrix const Mat2 ){

int row1 = Mat1.firstDim;
int col1 = Mat1.secondDim;
int row2 = Mat2.firstDim;
int col2 = Mat2.secondDim;

for(int i=0;i<row1;i++){
  for(int j=0;j<col2;j++){
    MatOut->Matrix[i*col2 + j] = 0;
    for(int k=0;k<row2;k++){
      MatOut->Matrix[i*col2 + j] += Mat1.Matrix[i*col1 + k]*Mat2.Matrix[k*col2 + j];
    }
  }
}
return 0;
}

void MatVecEndEq(struct EndEquationVector * VecOut,struct EndEquationMatrix const MatInn, struct EndEquationVector const VecInn){
  int row = MatInn.firstDim;
  int col = MatInn.secondDim;
  for(int i=0;i<row;i++){
    VecOut->Vector[i] = 0;
    for(int j=0;j<col;j++){
      VecOut->Vector[i] += MatInn.Matrix[i*col + j]*VecInn.Vector[j];
    }
  }
}

void MatMatAddEndEq(struct EndEquationMatrix * MatOut,struct EndEquationMatrix const MatInn1, struct EndEquationMatrix const MatInn2){
  int row = MatInn1.firstDim;
  int col = MatInn1.secondDim;
  for(int i=0;i<row;i++){
    for(int j=0;j<col;j++){
      MatOut->Matrix[i*col +j] = MatInn1.Matrix[i*col + j] + MatInn2.Matrix[i*col + j];
    }
  }
}

void VecVecAddEndEq(struct EndEquationVector * VecOut,struct EndEquationVector const VecInn1, struct EndEquationVector const VecInn2){
  int row = VecInn1.firstDim;
  for(int i=0;i<row;i++){
  
      VecOut->Vector[i] = VecInn1.Vector[i] + VecInn2.Vector[i];
    
  }
}


void VecVecSubEndEq(struct EndEquationVector * VecOut,struct EndEquationVector const VecInn1, struct EndEquationVector const VecInn2){
  int row = VecInn1.firstDim;
  for(int i=0;i<row;i++){
  
      VecOut->Vector[i] = VecInn1.Vector[i] - VecInn2.Vector[i];
    
  }
}

void VecByConstEndEq(struct EndEquationVector * VecOut,struct EndEquationVector const VecInn1, double Const){
  int row = VecInn1.firstDim;
  for(int i=0;i<row;i++){
  
      VecOut->Vector[i] = VecInn1.Vector[i]*Const;
    
  }
}

void copyVector(struct EndEquationVector * VecOut, struct EndEquationVector const VecInn){

  for(int i=0;i<VecInn.firstDim;i++){
    VecOut->Vector[i] = VecInn.Vector[i];
  }
}

void transposeB(struct EndEquationMatrix * BT,struct EndEquationMatrix const B){
  int row = B.firstDim;
  int col = B.secondDim;

  for(int i=0;i<row;i++){
    for(int j=0;j<col;j++){
      BT->Matrix[j*row + i] = B.Matrix[i*col + j];
    }
  }
}


/*Invert a Matrix*/
int MyInverse(struct EndEquationMatrix _A) {

    double temp;
    int N = _A.firstDim;
    double *E = (double *)calloc(N*N, sizeof(double));

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) {
            if (i == j)
                E[i*N + j] = 1;
        }
      }

    for (int k = 0; k < N; k++) {
        temp = _A.Matrix[k*N + k];

        for (int j = 0; j < N; j++) {
            _A.Matrix[k*N +j] /= temp;
            E[k*N + j] /= temp;
        }

        for (int i = k + 1; i < N; i++) {
            temp = _A.Matrix[i*N+k];

            for (int j = 0; j < N; j++) {
                _A.Matrix[i*N + j] -= _A.Matrix[k*N + j] * temp;
                E[i*N + j] -= E[k*N + j] * temp;
            }
        }
    }

    for (int k = N - 1; k > 0; k--) {
        for (int i = k - 1; i >= 0; i--) {
            temp = _A.Matrix[i*N + k];

            for (int j = 0; j < N; j++) {
                _A.Matrix[i*N + j] -= _A.Matrix[k*N + j] * temp;
                E[i*N + j] -= E[k*N + j] * temp;
            }
        }
    }

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            _A.Matrix[i*N + j] = E[i*N + j];
        }
      }
    return 1;
}



/*Get the rotational matrix of an unknown rotational axis */
int expMRodriguez(double Rot[3][3], double w1, double w2, double w3, double dt) {

  
    double f1, f2;    
    double normw0 = sqrt(w1 * w1 + w2 * w2 + w3 * w3);
    double O[3][3] = {0};

    double I[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    double m2[3][3] = {0};
    double m3[3][3] = {0};
    double m4[3][3] = {0};
    double m5[3][3] = {0};

    O[0][0] = 0;
    O[0][1] = -w3;
    O[0][2] = w2;
    O[1][0] = w3;
    O[1][1] = 0;
    O[1][2] = -w1;
    O[2][0] = -w2;
    O[2][1] = w1;
    O[2][2] = 0;

    if (normw0 >= 0.00000001) {
         f1 = (sin(normw0 * dt)) / (normw0);
         f2 = (1 - cos(normw0 * dt)) / (normw0 * normw0);
    };
    if (normw0 <= 0.00000001) {
         f1 = 0;
         f2 = 0;
    };

     MatConstMult3by3(m2 ,O, f1);
    
     MatMatMult3by3(m3 ,O, O);
    
     MatConstMult3by3(m4 ,m3, f2);

     MatMatAdd3by3(m5, m2, m4);

     MatMatAdd3by3(Rot, I, m5);


    return 1;
}


void set_R1(struct Frame Frames[4], struct EndEquationMatrices *endEquationMatrices,  struct ArrayData myArrayData){
  
  expMRodriguez( Frames[0].locR.R, endEquationMatrices->AngularVelocities.Vector[0], 
    endEquationMatrices->AngularVelocities.Vector[1],
    endEquationMatrices->AngularVelocities.Vector[2], myArrayData.dt);
  MatMatMult3by3(Frames[0].AbsR.R ,Frames[0].tempR1.R, Frames[0].locR.R);
}


int setRotations(struct Frame Frames[4], struct EndEquationMatrices *endEquationMatrices,  struct ArrayData myArrayData){

  /* Get R1 */  
  set_R1(Frames, endEquationMatrices, myArrayData);
  
  Transpose3by3(Frames[0].TlocR.R,Frames[0].AbsR.R);
  /* Get the time derivative of R1 */
  Frames[0].dlocR.R[0][0]=0;Frames[0].dlocR.R[0][1]=-endEquationMatrices->AngularVelocities.Vector[2];Frames[0].dlocR.R[0][2]=endEquationMatrices->AngularVelocities.Vector[1];
  Frames[0].dlocR.R[1][0]=endEquationMatrices->AngularVelocities.Vector[2];Frames[0].dlocR.R[1][1]=0;Frames[0].dlocR.R[1][2]=-endEquationMatrices->AngularVelocities.Vector[0];
  Frames[0].dlocR.R[2][0]=-endEquationMatrices->AngularVelocities.Vector[1];Frames[0].dlocR.R[2][1]=endEquationMatrices->AngularVelocities.Vector[0];Frames[0].dlocR.R[2][2]=0;
  MatMatMult3by3(Frames[0].dAbsR.R ,Frames[0].AbsR.R, Frames[0].dlocR.R);
  Transpose3by3(Frames[0].TdlocR.R,Frames[0].dAbsR.R);

  for (int i=1; i<4; i++){
   /* Get local rotations */  
   getRotation(Frames[i].locR.R, myArrayData.Axis[i],endEquationMatrices->Angles.Vector[i+2]);
   /* Get the Transpose of the local R*/
   Transpose3by3(Frames[i].TlocR.R,Frames[i].locR.R);
   /* Get local differetniated rotations */  
   getdiffRotation(Frames[i].dlocR.R, myArrayData.Axis[i],endEquationMatrices->Angles.Vector[i+2],endEquationMatrices->AngularVelocities.Vector[i+2]);
   /* Get the Transpose of the local dR */   
   Transpose3by3(Frames[i].TdlocR.R,Frames[i].dlocR.R);
   /* Get Absolute rotations */ 
   MatMatMult3by3(Frames[i].AbsR.R ,Frames[i-1].AbsR.R, Frames[i].locR.R);

   /* Get Absolute time differentiatd matrices */
   MatMatMult3by3(Frames[i].tempR1.R ,Frames[i-1].AbsR.R, Frames[i].dlocR.R);
   MatMatMult3by3(Frames[i].tempR2.R ,Frames[i-1].dAbsR.R, Frames[i].locR.R);
   MatMatAdd3by3(Frames[i].dAbsR.R, Frames[i].tempR1.R, Frames[i].tempR2.R);
  }
  return 0;
}

void get_GlobPosCraneTip(struct PhysicalValues * myPhysicalValues , struct Frame Frames[4]){

    double j2fromc1[3] = {myPhysicalValues->c1toj2Skew[1][2],-myPhysicalValues->c1toj2Skew[0][2],myPhysicalValues->c1toj2Skew[0][1]};
    double j3fromj2[3] = {0,0,6.017};
    double j4fromj3[3] = {22.0,0,0};
    double Tipfromj4[3] = {-9.11,0,-5.05};

    double tempM1[3][3];
    double tempM2[3][3];
    double tempV[3];

    MatMatMult3by3(tempM1, Frames[1].locR.R, Frames[2].locR.R);
    MatMatMult3by3(tempM2, tempM1, Frames[3].locR.R);

    MatVecMult3by3(tempV, tempM2, Tipfromj4);
    VecVecAdd3(myPhysicalValues->craneGlobCoord,myPhysicalValues->craneGlobCoord,tempV);
    MatVecMult3by3(tempV, tempM1, j4fromj3);
    VecVecAdd3(myPhysicalValues->craneGlobCoord,myPhysicalValues->craneGlobCoord,tempV);
    MatVecMult3by3(tempV,Frames[1].locR.R,j3fromj2);
    VecVecAdd3(myPhysicalValues->craneGlobCoord,myPhysicalValues->craneGlobCoord,tempV);
    VecVecAdd3(myPhysicalValues->craneGlobCoord,myPhysicalValues->craneGlobCoord,j2fromc1);

}

double get_Position_Error(struct PhysicalValues * myPhysicalValues, struct Frame Frames[4]){

  double j2fromc1[3] = {myPhysicalValues->c1toj2Skew[1][2],-myPhysicalValues->c1toj2Skew[0][2],myPhysicalValues->c1toj2Skew[0][1]};
  double j3fromj2[3] = {0,0,6.017};
  double j4fromj3[3] = {22.0,0,0};
  double Tipfromj4[3] = {-9.11,0,-5.05};

  double InertPos[3];
  double tempV[3];


  MatVecMult3by3(InertPos, Frames[3].AbsR.R, Tipfromj4);

  MatVecMult3by3(tempV, Frames[2].AbsR.R, j4fromj3);
  VecVecAdd3(InertPos,InertPos,tempV);

  MatVecMult3by3(tempV,Frames[1].AbsR.R,j3fromj2);
  VecVecAdd3(InertPos,InertPos,tempV);

  MatVecMult3by3(tempV,Frames[0].AbsR.R,j2fromc1);
  VecVecAdd3(InertPos,InertPos,tempV);

  VecVecSub3(tempV,myPhysicalValues->craneGlobCoord,InertPos);


  double PosError = sqrt(tempV[0]*tempV[0]+tempV[1]*tempV[1]+tempV[2]*tempV[2]);
  return PosError;
}

void KillMemory(struct EndEquationMatrices * endEquationMatrices, struct SolutionVectors * mySolutionVectors ){

  free(endEquationMatrices->Mass.Matrix);
  free(endEquationMatrices->D.Matrix);
  free(endEquationMatrices->MB.Matrix);
  free(endEquationMatrices->DMB.Matrix);
  free(endEquationMatrices->MdBDMB.Matrix);
  free(endEquationMatrices->MdB.Matrix);
  free(endEquationMatrices->BT.Matrix);
  free(endEquationMatrices->B.Matrix);
  free(endEquationMatrices->dB.Matrix);
  free(endEquationMatrices->Mstar.Matrix);
  free(endEquationMatrices->MstarI.Matrix);
  free(endEquationMatrices->Nstar.Matrix);

  free(endEquationMatrices->F.Vector);
  free(endEquationMatrices->Fstar.Vector);
  free(endEquationMatrices->NStarqn.Vector);
  free(endEquationMatrices->FmNstarqn.Vector);
  free(endEquationMatrices->Angles.Vector);
  free(endEquationMatrices->AngularVelocities.Vector);
  free(endEquationMatrices->AngularAccelerations.Vector);

  free(endEquationMatrices->NstarVel.Vector);
  free(endEquationMatrices->MstarAcc.Vector);
  free(endEquationMatrices->MstarAccAddNstarVel.Vector);
  free(endEquationMatrices->MstarAccAddNstarVelSubFstar.Vector);
  //free(endEquationMatrices->Sol.Vector);
  //free(endEquationMatrices->SolV.Vector);
  free(endEquationMatrices->waveMoments.Vector);

  free(mySolutionVectors->Sol.Vector);
  free(mySolutionVectors->SolV.Vector);
  free(mySolutionVectors->k1.Vector);
  free(mySolutionVectors->k2.Vector);
  free(mySolutionVectors->Ang.Vector);
  free(mySolutionVectors->AngV.Vector);
  free(mySolutionVectors->TempAng.Vector);
  free(mySolutionVectors->TempAngV.Vector);
  free(mySolutionVectors->TempSol.Vector);
  free(mySolutionVectors->TempSolV.Vector);
  free(endEquationMatrices->CraneMoments.Vector);

  free(endEquationMatrices->MdBDMBdq.Vector);
  free(endEquationMatrices->MBddq.Vector);
  free(mySolutionVectors->maAndMom.Vector);





}
