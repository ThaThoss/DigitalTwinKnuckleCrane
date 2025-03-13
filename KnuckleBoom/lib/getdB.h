#ifndef GETdB
#define GETdB
void getdB(struct EndEquationMatrix * dB, struct PhysicalValues myPhysicalValues,  struct Frame Frames[4]){
double TempMatrixForLinear[3][3] = {0};
double TempMatrixForAngular[3][3] = {0};
double TempMatrixForAngular2[3][3] = {0}; 
double PlaceHolder1[3][3] = {0};
double PlaceHolder2[3][3] = {0};
double PlaceHolder3[3][3] = {0}; 
double TempVecForLinear[3] = {0};
double TempVec1[3] = {0};
double TempVec2[3] = {0};
double TempVec3[3] = {0};
double TempVec4[3] = {0};

 //int row = dB->firstDim;
 int col = dB->secondDim;
 int j = 0;
 int i = 0;




/* dB21 */

MatMatMult3by3(PlaceHolder1,Frames[0].dAbsR.R,myPhysicalValues.c1toj2Skew);

MatMatMult3by3(PlaceHolder2,myPhysicalValues.j2toc2Skew,Frames[1].TdlocR.R);
MatMatMult3by3(PlaceHolder3,Frames[1].AbsR.R,PlaceHolder2);
MatMatAdd3by3(PlaceHolder1,PlaceHolder1,PlaceHolder3);

MatMatMult3by3(PlaceHolder2,myPhysicalValues.j2toc2Skew,Frames[1].TlocR.R);
MatMatMult3by3(PlaceHolder3,Frames[1].dAbsR.R,PlaceHolder2);

MatMatAdd3by3(TempMatrixForLinear,PlaceHolder1,PlaceHolder3);



j=0;
for(i=3;i<6;i++){
 /* B21 */
    dB->Matrix[i*col + 0] = TempMatrixForLinear[j][0];
    dB->Matrix[i*col + 1] = TempMatrixForLinear[j][1];
    dB->Matrix[i*col + 2] = TempMatrixForLinear[j][2];
 /* B22 Will always be zero
    dB->Matrix[i*col + 3] = TempVecForLinear[j]; */   
j++;
}
/* dB31 */
j=0;
for(i=6;i<9;i++){

    dB->Matrix[i*col + 0] = Frames[1].TdlocR.R[j][0];
    dB->Matrix[i*col + 1] = Frames[1].TdlocR.R[j][1];
    dB->Matrix[i*col + 2] = Frames[1].TdlocR.R[j][2];
 
j++;
}
/* dB41 */
MatMatMult3by3(PlaceHolder1,myPhysicalValues.c2toj3Skew,Frames[1].TdlocR.R);
MatMatMult3by3(PlaceHolder2,Frames[1].AbsR.R,PlaceHolder1);
MatMatMult3by3(PlaceHolder1,myPhysicalValues.c2toj3Skew,Frames[1].TlocR.R);
MatMatMult3by3(PlaceHolder3,Frames[1].dAbsR.R,PlaceHolder1);
MatMatAdd3by3(PlaceHolder1,PlaceHolder2,PlaceHolder3);

MatMatMult3by3(TempMatrixForAngular,Frames[2].TlocR.R,Frames[1].TdlocR.R);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.j3toc3Skew,TempMatrixForAngular);
MatMatMult3by3(PlaceHolder2,Frames[2].AbsR.R,PlaceHolder3);
MatMatAdd3by3(PlaceHolder1,PlaceHolder1,PlaceHolder2);

MatMatMult3by3(TempMatrixForAngular2,Frames[2].TdlocR.R,Frames[1].TlocR.R);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.j3toc3Skew,TempMatrixForAngular2);
MatMatMult3by3(PlaceHolder2,Frames[2].AbsR.R,PlaceHolder3);
MatMatAdd3by3(PlaceHolder1,PlaceHolder1,PlaceHolder2);

MatMatMult3by3(PlaceHolder2,Frames[2].TlocR.R,Frames[1].TlocR.R);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.j3toc3Skew,PlaceHolder2);
MatMatMult3by3(PlaceHolder2,Frames[2].dAbsR.R,PlaceHolder3);
MatMatAdd3by3(PlaceHolder1,PlaceHolder1,PlaceHolder2);

MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear,PlaceHolder1);

/* dB42 */
TempVec2[0] = 0;
TempVec2[1] = myPhysicalValues.j3toc3Skew[1][2]*Frames[2].TdlocR.R[2][2];
TempVec2[2] = 0;
MatVecMult3by3(TempVec1,Frames[2].AbsR.R,TempVec2);

TempVec2[0] = 0;
TempVec2[1] = myPhysicalValues.j3toc3Skew[1][2]*Frames[2].TlocR.R[2][2];
TempVec2[2] = 0;
MatVecMult3by3(TempVec3,Frames[2].dAbsR.R,TempVec2);
VecVecAdd3(TempVecForLinear, TempVec3, TempVec1);

/* dB43 */
TempVec1[0] = 0;
TempVec1[1] = 0;
TempVec1[2] = myPhysicalValues.j3toc3Skew[2][1];
MatVecMult3by3(TempVec4, Frames[2].dAbsR.R, TempVec1);

j=0;
for(i=9;i<12;i++){
 /* dB41 */
    dB->Matrix[i*col + 0] = TempMatrixForLinear[j][0];
    dB->Matrix[i*col + 1] = TempMatrixForLinear[j][1];
    dB->Matrix[i*col + 2] = TempMatrixForLinear[j][2];
 /* dB42 */
    dB->Matrix[i*col + 3] = TempVecForLinear[j];
 /* dB43 */
    dB->Matrix[i*col + 4] = TempVec4[j];

j++;
}

/* dB51 */
MatMatAdd3by3(TempMatrixForAngular,TempMatrixForAngular, TempMatrixForAngular2);
j=0;
for(i=12;i<15;i++){
 /* dB51 */
    dB->Matrix[i*col + 0] = TempMatrixForAngular[j][0];
    dB->Matrix[i*col + 1] = TempMatrixForAngular[j][1];
    dB->Matrix[i*col + 2] = TempMatrixForAngular[j][2];
 /* dB52 */
    dB->Matrix[i*col + 3] = Frames[2].TdlocR.R[j][2];

j++;
}

/* dB61 */
MatMatMult3by3(PlaceHolder1,Frames[2].TlocR.R,Frames[1].TdlocR.R);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.c3toj4Skew,PlaceHolder1);
MatMatMult3by3(PlaceHolder2,Frames[2].AbsR.R,PlaceHolder3);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear,PlaceHolder2);

MatMatMult3by3(PlaceHolder2,Frames[3].TlocR.R,PlaceHolder1);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.j4toc4Skew,PlaceHolder2);
MatMatMult3by3(PlaceHolder2,Frames[3].AbsR.R,PlaceHolder3);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear,PlaceHolder2);


MatMatMult3by3(PlaceHolder1,Frames[2].TdlocR.R,Frames[1].TlocR.R);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.c3toj4Skew,PlaceHolder1);
MatMatMult3by3(PlaceHolder2,Frames[2].AbsR.R,PlaceHolder3);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear,PlaceHolder2);

MatMatMult3by3(PlaceHolder2,Frames[3].TlocR.R,PlaceHolder1);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.j4toc4Skew,PlaceHolder2);
MatMatMult3by3(PlaceHolder2,Frames[3].AbsR.R,PlaceHolder3);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear,PlaceHolder2);


MatMatMult3by3(PlaceHolder1,Frames[2].TlocR.R,Frames[1].TlocR.R);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.c3toj4Skew,PlaceHolder1);
MatMatMult3by3(PlaceHolder2,Frames[2].dAbsR.R,PlaceHolder3);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear,PlaceHolder2);

MatMatMult3by3(TempMatrixForAngular2,Frames[3].TdlocR.R,PlaceHolder1);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.j4toc4Skew,TempMatrixForAngular2);
MatMatMult3by3(PlaceHolder2,Frames[3].AbsR.R,PlaceHolder3);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear,PlaceHolder2);

MatMatMult3by3(PlaceHolder2,Frames[3].TlocR.R,PlaceHolder1);
MatMatMult3by3(PlaceHolder3,myPhysicalValues.j4toc4Skew,PlaceHolder2);
MatMatMult3by3(PlaceHolder2,Frames[3].dAbsR.R,PlaceHolder3);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear,PlaceHolder2);
/* ---------------------------------------------------------------------- */
/* dB62 */
TempVec2[0] = 0;
TempVec2[1] = myPhysicalValues.c3toj4Skew[1][2]*Frames[2].TdlocR.R[2][2];
TempVec2[2] = 0;
MatVecMult3by3(TempVec1,Frames[2].AbsR.R,TempVec2);
VecVecAdd3(TempVecForLinear, TempVecForLinear, TempVec1);

TempVec2[0] = 0;
TempVec2[1] = myPhysicalValues.c3toj4Skew[1][2]*Frames[2].TlocR.R[2][2];
TempVec2[2] = 0;
MatVecMult3by3(TempVec1,Frames[2].dAbsR.R,TempVec2);
VecVecAdd3(TempVecForLinear, TempVecForLinear, TempVec1);


TempVec2[0] = Frames[2].TdlocR.R[0][2];
TempVec2[1] = Frames[2].TdlocR.R[1][2];
TempVec2[2] = Frames[2].TdlocR.R[2][2];
MatVecMult3by3(TempVec1, Frames[3].TlocR.R, TempVec2);
MatVecMult3by3(TempVec2, myPhysicalValues.j4toc4Skew, TempVec1);
MatVecMult3by3(TempVec1, Frames[3].AbsR.R, TempVec2);
VecVecAdd3(TempVecForLinear, TempVecForLinear, TempVec1);

TempVec2[0] = Frames[2].TlocR.R[0][2];
TempVec2[1] = Frames[2].TlocR.R[1][2];
TempVec2[2] = Frames[2].TlocR.R[2][2];
MatVecMult3by3(TempVec1, Frames[3].TdlocR.R, TempVec2);
MatVecMult3by3(TempVec2, myPhysicalValues.j4toc4Skew, TempVec1);
MatVecMult3by3(TempVec1, Frames[3].AbsR.R, TempVec2);
VecVecAdd3(TempVecForLinear, TempVecForLinear, TempVec1);

TempVec2[0] = Frames[2].TlocR.R[0][2];
TempVec2[1] = Frames[2].TlocR.R[1][2];
TempVec2[2] = Frames[2].TlocR.R[2][2];
MatVecMult3by3(TempVec1, Frames[3].TlocR.R, TempVec2);
MatVecMult3by3(TempVec2, myPhysicalValues.j4toc4Skew, TempVec1);
MatVecMult3by3(TempVec1, Frames[3].dAbsR.R, TempVec2);
VecVecAdd3(TempVecForLinear, TempVecForLinear, TempVec1);
/*---------------------------------------------*/
/* dB63 */
TempVec1[0] = 0;
TempVec1[1] = 0;
TempVec1[2] = myPhysicalValues.c3toj4Skew[2][1];
MatVecMult3by3(TempVec2, Frames[2].dAbsR.R, TempVec1);
VecVecAdd3(TempVec4, TempVec4, TempVec2);

/* dTR43*e2 = [0,0,0], so I skip it */
/* TR43*e2 = [0,1,0], so I calculate directly */
TempVec1[0] = myPhysicalValues.j4toc4Skew[0][1];
TempVec1[1] = 0;
TempVec1[2] = myPhysicalValues.j4toc4Skew[2][1];
MatVecMult3by3(TempVec2,Frames[3].dAbsR.R,TempVec1);
VecVecAdd3(TempVec4, TempVec4, TempVec2);
/* -------------------------- */
/* dB64 */

j=0;
for(i=15;i<18;i++){
 /* dB61 */
    dB->Matrix[i*col + 0] = TempMatrixForLinear[j][0];
    dB->Matrix[i*col + 1] = TempMatrixForLinear[j][1];
    dB->Matrix[i*col + 2] = TempMatrixForLinear[j][2];
 /* dB62 */
    dB->Matrix[i*col + 3] = TempVecForLinear[j];
 /* dB63 */
    dB->Matrix[i*col + 4] = TempVec4[j];
 /* dB64 */
    dB->Matrix[i*col + 5] = TempVec2[j];
j++;
}

/* dB71 */
MatMatMult3by3(PlaceHolder1,Frames[3].TlocR.R,TempMatrixForAngular);
MatMatAdd3by3(TempMatrixForAngular2,TempMatrixForAngular2,PlaceHolder1);
/*-------------------------------------------------*/
/* dB72 */

TempVec1[0] = Frames[2].TdlocR.R[0][2];
TempVec1[1] = Frames[2].TdlocR.R[1][2];
TempVec1[2] = Frames[2].TdlocR.R[2][2];
MatVecMult3by3(TempVec2,Frames[3].TlocR.R,TempVec1);

TempVec1[0] = Frames[2].TlocR.R[0][2];
TempVec1[1] = Frames[2].TlocR.R[1][2];
TempVec1[2] = Frames[2].TlocR.R[2][2];

MatVecMult3by3(TempVec3,Frames[3].TdlocR.R,TempVec1);
VecVecAdd3(TempVec1,TempVec2,TempVec3);

/* dB73 */
TempVec2[0]=0;
TempVec2[1]=1;
TempVec2[2]=0;
MatVecMult3by3(TempVec3,Frames[3].TdlocR.R,TempVec2);

j=0;
for(i=18;i<21;i++){
 /* dB71 */
    dB->Matrix[i*col + 0] = TempMatrixForAngular2[j][0];
    dB->Matrix[i*col + 1] = TempMatrixForAngular2[j][1];
    dB->Matrix[i*col + 2] = TempMatrixForAngular2[j][2];
 /* dB72 */
    dB->Matrix[i*col + 3] = TempVec1[j];
 /* dB73 = [0,0,0] */
    dB->Matrix[i*col + 4] = TempVec3[j];
j++;
}
}
#endif