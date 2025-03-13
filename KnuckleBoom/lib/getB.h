#ifndef GETB
#define GETB
void getB(struct EndEquationMatrix * B, struct PhysicalValues myPhysicalValues,  struct Frame Frames[4]){
double TempMatrixForLinear[3][3]  = {0};
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

 //int row = B->firstDim;
 int col = B->secondDim;
 int j = 0;
 int i = 0;
 /* B11 */
/* Set the initial angular vel 1 from frame 1 */
    B->Matrix[0] = 1;
    B->Matrix[1*col +1] = 1;
    B->Matrix[2*col +2] = 1;

/* B21 */
/* Set velocity of second senter of mass */

MatMatMult3by3(PlaceHolder3, Frames[0].AbsR.R, myPhysicalValues.c1toj2Skew);

Transpose3by3(PlaceHolder1, Frames[1].locR.R );
MatMatMult3by3(PlaceHolder2, myPhysicalValues.j2toc2Skew, PlaceHolder1);
MatMatMult3by3(PlaceHolder1, Frames[1].AbsR.R, PlaceHolder2);



MatMatAdd3by3( TempMatrixForLinear, PlaceHolder3, PlaceHolder1 );

/* B22 will always be 0
TempVecForLinear[0]=Frames[1].AbsR.R[0][0]*myPhysicalValues.j2toc2Skew[0][2];
TempVecForLinear[1]=Frames[1].AbsR.R[1][0]*myPhysicalValues.j2toc2Skew[0][2];
TempVecForLinear[2]=Frames[1].AbsR.R[2][0]*myPhysicalValues.j2toc2Skew[0][2];
*/
j=0;
for(i=3;i<6;i++){
 /* B21 */
    B->Matrix[i*col + 0] = TempMatrixForLinear[j][0];
    B->Matrix[i*col + 1] = TempMatrixForLinear[j][1];
    B->Matrix[i*col + 2] = TempMatrixForLinear[j][2];
 /* B22 
    B->Matrix[i*col + 3] = TempVecForLinear[j]; */   
j++;
}

/* B31 */
j=0;
for(i=6;i<9;i++){

    B->Matrix[i*col + 0] = Frames[1].TlocR.R[j][0];
    B->Matrix[i*col + 1] = Frames[1].TlocR.R[j][1];
    B->Matrix[i*col + 2] = Frames[1].TlocR.R[j][2];
j++;    
}

/* B32 */
B->Matrix[8*col +3] = 1;

/* B41 */
MatMatMult3by3(PlaceHolder2, myPhysicalValues.c2toj3Skew, Frames[1].TlocR.R);
MatMatMult3by3(PlaceHolder1, Frames[1].AbsR.R, PlaceHolder2);


/* Save transpose times e3 for B42 and B52*/
TempVec1[0] = Frames[2].TlocR.R[0][2];
TempVec1[1] = Frames[2].TlocR.R[1][2];
TempVec1[2] = Frames[2].TlocR.R[2][2];

MatMatMult3by3(TempMatrixForAngular2, Frames[2].TlocR.R, Frames[1].TlocR.R);

/* Save B51 and B52*/
j=0;
for(i=12;i<15;i++){

    B->Matrix[i*col + 0] = TempMatrixForAngular2[j][0];
    B->Matrix[i*col + 1] = TempMatrixForAngular2[j][1];
    B->Matrix[i*col + 2] = TempMatrixForAngular2[j][2];
    B->Matrix[i*col + 3] = TempVec1[j];
j++;  
}
B->Matrix[13*col + 4] = 1;
/*Continue B41 */

MatMatMult3by3(PlaceHolder2, myPhysicalValues.j3toc3Skew, TempMatrixForAngular2);
MatMatMult3by3(PlaceHolder3,Frames[2].AbsR.R, PlaceHolder2);

MatMatAdd3by3( PlaceHolder2  , PlaceHolder3, PlaceHolder1);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear, PlaceHolder2  );

/* B42 */
/* Note, The first input R2*c2toj3*e3=0 and has been ignored */
TempVec2[0] = 0;
TempVec2[1] = myPhysicalValues.j3toc3Skew[1][2]*Frames[2].TlocR.R[2][2];
TempVec2[2] = 0;
MatVecMult3by3(TempVecForLinear,Frames[2].AbsR.R,TempVec2);


/* B43 */
TempVec1[0] = 0;
TempVec1[1] = 0;
TempVec1[2] = myPhysicalValues.j3toc3Skew[2][1];
MatVecMult3by3(TempVec4, Frames[2].AbsR.R, TempVec1);

j=0;
for(i=9;i<12;i++){
    /*B41*/
    B->Matrix[i*col + 0] = TempMatrixForLinear[j][0];
    B->Matrix[i*col + 1] = TempMatrixForLinear[j][1];
    B->Matrix[i*col + 2] = TempMatrixForLinear[j][2];
    /*B42*/
    B->Matrix[i*col + 3] = TempVecForLinear[j];
    /*B43*/
    B->Matrix[i*col + 4] = TempVec4[j];
j++;
}

/* B61 */
MatMatMult3by3(TempMatrixForAngular, Frames[3].TlocR.R, TempMatrixForAngular2 );
MatMatMult3by3(PlaceHolder1, myPhysicalValues.j4toc4Skew, TempMatrixForAngular);
MatMatMult3by3(PlaceHolder2, Frames[3].AbsR.R, PlaceHolder1);

MatMatMult3by3(PlaceHolder1, myPhysicalValues.c3toj4Skew,TempMatrixForAngular2 );
MatMatMult3by3(PlaceHolder3, Frames[2].AbsR.R, PlaceHolder1);


MatMatAdd3by3(PlaceHolder1, PlaceHolder3, PlaceHolder2);
MatMatAdd3by3(TempMatrixForLinear,TempMatrixForLinear, PlaceHolder1);


/* B62 */
/*Grab TR32*e3 */
TempVec1[0] = 0; /* 0*Frames[2].locR.R[2][0]; */
TempVec1[1] = myPhysicalValues.c3toj4Skew[1][2]*Frames[2].locR.R[2][2];
TempVec1[2] = myPhysicalValues.c3toj4Skew[2][1]*Frames[2].locR.R[2][1];
MatVecMult3by3(TempVec2,Frames[2].AbsR.R,TempVec1);

TempVec1[0]=Frames[2].locR.R[2][0];/* Transpose */
TempVec1[1]=Frames[2].locR.R[2][1];
TempVec1[2]=Frames[2].locR.R[2][2];
Transpose3by3(PlaceHolder1,Frames[3].locR.R);
MatVecMult3by3(TempVec3,PlaceHolder1,TempVec1);

/* B71 */
j=0;
for(i=18;i<21;i++){

    B->Matrix[i*col + 0] = TempMatrixForAngular[j][0];
    B->Matrix[i*col + 1] = TempMatrixForAngular[j][1];
    B->Matrix[i*col + 2] = TempMatrixForAngular[j][2];
    /* B72 */
    B->Matrix[i*col + 3] = TempVec3[j];
    /* B73 */
    B->Matrix[i*col + 4] = Frames[3].locR.R[1][j];
j++;
}
B->Matrix[19*col + 5] = 1;


MatVecMult3by3(TempVec1,myPhysicalValues.j4toc4Skew,TempVec3);
MatVecMult3by3(TempVec3,Frames[3].AbsR.R,TempVec1);

VecVecAdd3(TempVec1,TempVec2,TempVec3);
VecVecAdd3(TempVecForLinear,TempVecForLinear,TempVec1);

/* B63 */
TempVec1[0] = 0;
TempVec1[1] = 0;
TempVec1[2] = myPhysicalValues.c3toj4Skew[2][1];
MatVecMult3by3(TempVec2,Frames[2].AbsR.R,TempVec1);

TempVec1[0] = Frames[3].locR.R[1][0];
TempVec1[1] = Frames[3].locR.R[1][1];
TempVec1[2] = Frames[3].locR.R[1][2];
MatVecMult3by3(TempVec3,myPhysicalValues.j4toc4Skew,TempVec1);
MatVecMult3by3(TempVec1,Frames[3].AbsR.R,TempVec3);
VecVecAdd3(TempVec3,TempVec2,TempVec1);
VecVecAdd3(TempVec1,TempVec4,TempVec3);

TempVec2[0] = myPhysicalValues.j4toc4Skew[0][1];
TempVec2[1] = 0;
TempVec2[2] = myPhysicalValues.j4toc4Skew[2][1];
MatVecMult3by3(TempVec3,Frames[3].AbsR.R,TempVec2);



j=0;
for(i=15;i<18;i++){
    /*B61 */
    B->Matrix[i*col + 0] = TempMatrixForLinear[j][0];
    B->Matrix[i*col + 1] = TempMatrixForLinear[j][1];
    B->Matrix[i*col + 2] = TempMatrixForLinear[j][2];
    /* B62 */
    B->Matrix[i*col + 3] = TempVecForLinear[j];
    /* B63 */
    B->Matrix[i*col + 4] = TempVec1[j];
    /* 64 */
    B->Matrix[i*col + 5] = TempVec3[j];
j++;    
}

j=0;


}
#endif