#include "getMAandMom.h"
#include "MyDynStructs.h"
#include "dynfunctions.h"


void getMAandMom(struct SolutionVectors * mySolutionVectors, struct EndEquationMatrices * endEquationMatrices ){

    MatVecEndEq(&endEquationMatrices->MdBDMBdq, endEquationMatrices->MdBDMB, endEquationMatrices->AngularVelocities);
    MatVecEndEq(&endEquationMatrices->MBddq, endEquationMatrices->MB, endEquationMatrices->AngularAccelerations);

    VecVecAddEndEq(&mySolutionVectors->maAndMom, endEquationMatrices->MBddq, endEquationMatrices->MdBDMBdq);

}
