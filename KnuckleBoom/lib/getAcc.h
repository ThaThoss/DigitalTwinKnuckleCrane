#ifndef GETACC
#define GETACC
void getAcc(struct EndEquationMatrices * endEquationMatrices, struct Frame Frames[4], struct ArrayData myArrayData, struct PhysicalValues myPhysicalValues ){




getB( &endEquationMatrices->B, myPhysicalValues, Frames);
getdB(&endEquationMatrices->dB, myPhysicalValues, Frames);
getD(endEquationMatrices->D, Frames, endEquationMatrices->AngularVelocities );  
transposeB(&endEquationMatrices->BT, endEquationMatrices->B);

MatMatMultEndEqMat(&endEquationMatrices->MB,endEquationMatrices->Mass,endEquationMatrices->B);//MB
MatMatMultEndEqMat(&endEquationMatrices->Mstar,endEquationMatrices->BT,endEquationMatrices->MB);

MatMatMultEndEqMat(&endEquationMatrices->MdB,endEquationMatrices->Mass,endEquationMatrices->dB);
MatMatMultEndEqMat(&endEquationMatrices->DMB,endEquationMatrices->D,endEquationMatrices->MB);
MatMatAddEndEq(&endEquationMatrices->MdBDMB,endEquationMatrices->MdB, endEquationMatrices->DMB);//MddB+DMB
MatMatMultEndEqMat(&endEquationMatrices->Nstar, endEquationMatrices->BT, endEquationMatrices->MdBDMB);

get_Forces( endEquationMatrices, Frames, myArrayData, myPhysicalValues );



MatVecEndEq(&endEquationMatrices->Fstar, endEquationMatrices->BT, endEquationMatrices->F);
MatVecEndEq(&endEquationMatrices->NStarqn, endEquationMatrices->Nstar, endEquationMatrices->AngularVelocities);
VecVecSubEndEq(&endEquationMatrices->FmNstarqn, endEquationMatrices->Fstar, endEquationMatrices->NStarqn);



MyInverse(endEquationMatrices->Mstar);

MatVecEndEq(&endEquationMatrices->AngularAccelerations, endEquationMatrices->Mstar, endEquationMatrices->FmNstarqn);//ddq
/*
printf("\nddth1 actual outside is : %lf\n",endEquationMatrices->AngularAccelerations.Vector[0]);
printf("\nddth2 actual outside is : %lf \n",endEquationMatrices->AngularAccelerations.Vector[1]);
printf("\nddth3 actual outside is : %lf \n",endEquationMatrices->AngularAccelerations.Vector[2]);
printf("\nddth4 actual outside is : %lf\n",endEquationMatrices->AngularAccelerations.Vector[3]);
printf("\nddth5 actual outside is : %lf \n",endEquationMatrices->AngularAccelerations.Vector[4]);
printf("\nddth6 actual outside is : %lf  \n",endEquationMatrices->AngularAccelerations.Vector[5]);
*/
}
#endif