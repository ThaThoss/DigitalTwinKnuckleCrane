#ifndef DYNFUNC
#define DYNFUNC

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy
#include <stdint.h>

#include "MyDynStructs.h"

/* Set the values of the M matrix */

int SetMassMatrix(struct EndEquationMatrix M);


void setSkewPositions(struct PhysicalValues * myPhysicalValues);


void set_Gravity_to_F(struct EndEquationVector * F, struct ArrayData myArrayData, struct PhysicalValues myPhysicalValues  );


void SolutionVectorsAllocater(struct SolutionVectors * mySolutionVectors, struct ArrayData myArrayData);

/*Memory Allocation */
int endEquationAllocater(struct EndEquationMatrices *endEquationMatrices, struct ArrayData myArrayData);

/* Set a matrix to zero */
int zeroMatrix(double *Matrix, int N,int M);

/* Matrix multiplier, for 3 by 3 matrices  */
void MatMatMult3by3(double ResultMat[3][3], double const Mat1[3][3], double const Mat2[3][3]);

/* Matrix Vector Multiplier, for 3 by 3 matrices  */
void MatVecMult3by3(double ResultVec[3] ,double const Mat1[3][3], double const Vec[3]);

/* Matrix Vector Multiplier, for 3 by 3 matrices  */
void MatTransVecMult3by3(double ResultVec[3] ,double const Mat1[3][3], double const Vec[3]);

/* Matrix Multiplied By Constant */
void MatConstMult3by3(double ResultMat[3][3] ,double const Mat1[3][3], double const Constant);

void MatMatAdd3by3(double ResultMat[3][3], double Mat1[3][3], double Mat2[3][3]);

void copyMatrix3by3(double MatOut[3][3], double MatInn[3][3]);

void VecVecAdd3(double ResVec[3], double Vec1[3], double Vec2[3]);

void VecVecSub3(double ResVec[3], double Vec1[3], double Vec2[3]);

/* Get Transpose */
void Transpose3by3(double RetMatr[3][3], double const Matrix[3][3] );

/* Get principal rotation matrices */
int getRotation(double Matrix[3][3], int axis, double angle);

/* Get the time differentiated principal rotation matrices */
int getdiffRotation(double Matrix[3][3], int axis, double angle, double angularVelocity);

/* Print Matrix, used for debugging */
int PrintMyEndMatrix(struct EndEquationMatrix Matrix);


void Print3by3(double Matrix[3][3]);

int MatMatMultEndEqMat(struct EndEquationMatrix * MatOut, struct EndEquationMatrix const Mat1,struct EndEquationMatrix const Mat2 );

void MatVecEndEq(struct EndEquationVector * VecOut,struct EndEquationMatrix const MatInn, struct EndEquationVector const VecInn);

void MatMatAddEndEq(struct EndEquationMatrix * MatOut,struct EndEquationMatrix const MatInn1, struct EndEquationMatrix const MatInn2);

void VecVecAddEndEq(struct EndEquationVector * VecOut,struct EndEquationVector const VecInn1, struct EndEquationVector const VecInn2);

void VecVecSubEndEq(struct EndEquationVector * VecOut,struct EndEquationVector const VecInn1, struct EndEquationVector const VecInn2);

void VecByConstEndEq(struct EndEquationVector * VecOut,struct EndEquationVector const VecInn1, double Const);

void copyVector(struct EndEquationVector * VecOut, struct EndEquationVector const VecInn);

void transposeB(struct EndEquationMatrix * BT,struct EndEquationMatrix const B);

/*Invert a Matrix*/
int MyInverse(struct EndEquationMatrix _A);

/*Get the rotational matrix of an unknown rotational axis */
int expMRodriguez(double Rot[3][3], double w1, double w2, double w3, double dt);


void set_R1(struct Frame * Frames, struct EndEquationMatrices *endEquationMatrices,  struct ArrayData myArrayData);

int setRotations(struct Frame Frames[4], struct EndEquationMatrices *endEquationMatrices,  struct ArrayData myArrayData);

void get_GlobPosCraneTip(struct PhysicalValues * myPhysicalValues , struct Frame Frames[4]);

double get_Position_Error(struct PhysicalValues * myPhysicalValues, struct Frame Frames[4]);

void KillMemory(struct EndEquationMatrices * endEquationMatrices, struct SolutionVectors * mySolutionVectors );
#endif 