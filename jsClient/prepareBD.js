// the crane tower rotates about the z-axis
// the crane main boom rotates about the x-axis
// the crane outer boom rotates about the x-axis


function get_D(D, Q, Rotations) {


    var w11 = Q.q1r;
    var w12 = Q.q2r;
    var w13 = Q.q3r;

    var w1 = [w11, w12, w13];
    //var w21 = [0, Q.q4r, 0];
    var w21 = [0, 0, Q.q4r];
    //var w32 = [Q.q5r, 0, 0];
    //var w43 = [Q.q6r, 0, 0];
    var w32 = [0, Q.q5r, 0];
    var w43 = [0, Q.q6r, 0];

    var matvec1_w2 = zeroVector(3);
    var vecvec1_w2 = zeroVector(3);

    var matvec1_w3 = zeroVector(3);
    var matvec2_w3 = zeroVector(3);
    var matvec3_w3 = zeroVector(3);
    var vecvec1_w3 = zeroVector(3);
    var vecvec2_w3 = zeroVector(3);

    var matvec1_w4 = zeroVector(3);
    var matvec2_w4 = zeroVector(3);
    var matvec3_w4 = zeroVector(3);
    var matvec4_w4 = zeroVector(3);
    var matvec5_w4 = zeroVector(3);
    var matvec6_w4 = zeroVector(3);
    var vecvec1_w4 = zeroVector(3);
    var vecvec2_w4 = zeroVector(3);
    var vecvec3_w4 = zeroVector(3);


    //---------------------------------------------------
    // abs. angular velocity vector for the tower w2

    matvec1_w2 = MatVecMult(Rotations.TR21, w1);
    vecvec1_w2 = VecVecAdd(matvec1_w2, w21); // w2 //Husk endre elise

    //----------------------------------------------------
    // abs. angular velocity vector for the main boom w3

    matvec1_w3 = MatVecMult(Rotations.TR32, w21);
    matvec2_w3 = MatVecMult(Rotations.TR21, w1);
    matvec3_w3 = MatVecMult(Rotations.TR32, matvec2_w3);
    vecvec1_w3 = VecVecAdd(matvec1_w3, w32);
    vecvec2_w3 = VecVecAdd(matvec3_w3, vecvec1_w3); // w3

    //----------------------------------------------------
    // abs. angular velocity vector for the outer boom w4

    matvec1_w4 = MatVecMult(Rotations.TR43, w32);
    matvec2_w4 = MatVecMult(Rotations.TR32, w21);
    matvec3_w4 = MatVecMult(Rotations.TR43, matvec2_w4);
    matvec4_w4 = MatVecMult(Rotations.TR21, w1);
    matvec5_w4 = MatVecMult(Rotations.TR32, matvec4_w4);
    matvec6_w4 = MatVecMult(Rotations.TR43, matvec5_w4);
    vecvec1_w4 = VecVecAdd(matvec1_w4, w43);
    vecvec2_w4 = VecVecAdd(vecvec1_w4, matvec3_w4);
    vecvec3_w4 = VecVecAdd(vecvec2_w4, matvec6_w4); // w4


    //----------------------------------------------------------
    //Skewing the abs. velocity vectors
    var Sw1 = zeroMatrix(3, 3);
    var Sw2 = zeroMatrix(3, 3);
    var Sw3 = zeroMatrix(3, 3);
    var Sw4 = zeroMatrix(3, 3);

    Sw1 = SkewMat(w1);
    Sw2 = SkewMat(vecvec1_w2);
    Sw3 = SkewMat(vecvec2_w3);
    Sw4 = SkewMat(vecvec3_w4);



    //----------------------------------------------------------------
    // D-matrix

    D[1][0] = Sw1[1][0]; // Where are the values for the w's calculated?
    D[2][0] = Sw1[2][0];
    D[0][1] = Sw1[0][1];
    D[2][1] = Sw1[2][1];
    D[0][2] = Sw1[0][2];
    D[1][2] = Sw1[1][2];

    D[7][6] = Sw2[1][0];
    D[8][6] = Sw2[2][0];
    D[6][7] = Sw2[0][1];
    D[8][7] = Sw2[2][1];
    D[6][8] = Sw2[0][2];
    D[7][8] = Sw2[1][2];

    D[13][12] = Sw3[1][0];
    D[14][12] = Sw3[2][0];
    D[12][13] = Sw3[0][1];
    D[14][13] = Sw3[2][1];
    D[12][14] = Sw3[0][2];
    D[13][14] = Sw3[1][2];

    D[19][18] = Sw4[1][0];
    D[20][18] = Sw4[2][0];
    D[18][19] = Sw4[0][1];
    D[20][19] = Sw4[2][1];
    D[18][20] = Sw4[0][2];
    D[19][20] = Sw4[1][2];

}


function get_Bn(Bn, Q, Rotations) {


    var sj21 = [myPhysics.b2, myPhysics.h21, myPhysics.L21];
    //var sj21 = [11.9, 7.5, 13.5];
    var sc21 = [0, myPhysics.h22, myPhysics.L22];
    //var sc21 = [0, 0, 2.6];

    var sj32 = [0, myPhysics.h31, myPhysics.L31];
    //var sj32 = [0, 0, 3.5];
    var sc32 = [0, myPhysics.h32, myPhysics.L32];
    //var sc32 = [10.7, 0, 0];

    var sj43 = [0, myPhysics.h41, myPhysics.L41];
    //var sj43 = [11.3, 0, 0];
    var sc43 = [0, myPhysics.h42, myPhysics.L42];
    //var sc43 = [-4.1, 0, -2.4];

    var Ssj21 = zeroMatrix(3, 3);
    var Ssj32 = zeroMatrix(3, 3);
    var Ssj43 = zeroMatrix(3, 3);
    var Ssc21 = zeroMatrix(3, 3);
    var Ssc32 = zeroMatrix(3, 3);
    var Ssc43 = zeroMatrix(3, 3);

    var e1 = [1, 0, 0];
    var e3 = [0, 1, 0];

    Ssj21 = SkewMat(sj21);



    Ssj32 = SkewMat(sj32);
    Ssj43 = SkewMat(sj43);
    Ssc21 = SkewMat(sc21);
    Ssc32 = SkewMat(sc32);
    Ssc43 = SkewMat(sc43);



    var TSsj21 = zeroMatrix(3, 3);
    var TSsj32 = zeroMatrix(3, 3);
    var TSsj43 = zeroMatrix(3, 3);
    var TSsc21 = zeroMatrix(3, 3);
    var TSsc32 = zeroMatrix(3, 3);
    var TSsc43 = zeroMatrix(3, 3);

    TSsj21 = MatTran(Ssj21);
    TSsj32 = MatTran(Ssj32);
    TSsj43 = MatTran(Ssj43);
    TSsc21 = MatTran(Ssc21);
    TSsc32 = MatTran(Ssc32);
    TSsc43 = MatTran(Ssc43);

    // B21

    var matmat1_B21 = zeroMatrix(3, 3);
    var matmat2_B21 = zeroMatrix(3, 3);
    var matmat3_B21 = zeroMatrix(3, 3);
    var B21 = zeroMatrix(3, 3);

    matmat1_B21 = MatMatMult(Rotations.R, TSsj21);
    matmat2_B21 = MatMatMult(TSsc21, Rotations.TR21);
    matmat3_B21 = MatMatMult(Rotations.R2, matmat2_B21);

    B21 = MatMatAdd(matmat3_B21, matmat1_B21); // B21

    // B41

    var matmat1_B41 = zeroMatrix(3, 3);
    var matmat2_B41 = zeroMatrix(3, 3);
    var matmat3_B41 = zeroMatrix(3, 3);
    var matmat4_B41 = zeroMatrix(3, 3);
    var matmat5_B41 = zeroMatrix(3, 3);
    var matmat6_B41 = zeroMatrix(3, 3);
    var matmat7_B41 = zeroMatrix(3, 3);
    var matmat8_B41 = zeroMatrix(3, 3);

    var plusmat1_B41 = zeroMatrix(3, 3);
    var plusmat2_B41 = zeroMatrix(3, 3);
    var B41 = zeroMatrix(3, 3);

    matmat1_B41 = MatMatMult(Rotations.R, TSsj21);
    matmat2_B41 = MatMatMult(TSsc21, Rotations.TR21);
    matmat3_B41 = MatMatMult(Rotations.R2, matmat2_B41);
    plusmat1_B41 = MatMatAdd(matmat1_B41, matmat3_B41);


    matmat4_B41 = MatMatMult(TSsj32, Rotations.TR21);
    matmat5_B41 = MatMatMult(Rotations.R2, matmat4_B41);

    matmat6_B41 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat7_B41 = MatMatMult(TSsc32, matmat6_B41);
    matmat8_B41 = MatMatMult(Rotations.R3, matmat7_B41);


    plusmat2_B41 = MatMatAdd(plusmat1_B41, matmat5_B41);
    B41 = MatMatAdd(plusmat2_B41, matmat8_B41); //B41

    // B51



    var B51 = zeroMatrix(3, 3);

    B51 = MatMatMult(Rotations.TR32, Rotations.TR21);


    // B61

    var matmat1_B61 = zeroMatrix(3, 3);
    var matmat2_B61 = zeroMatrix(3, 3);
    var matmat3_B61 = zeroMatrix(3, 3);
    var matmat4_B61 = zeroMatrix(3, 3);
    var matmat5_B61 = zeroMatrix(3, 3);
    var matmat6_B61 = zeroMatrix(3, 3);
    var matmat7_B61 = zeroMatrix(3, 3);
    var matmat8_B61 = zeroMatrix(3, 3);
    var matmat9_B61 = zeroMatrix(3, 3);
    var matmat10_B61 = zeroMatrix(3, 3);
    var matmat11_B61 = zeroMatrix(3, 3);
    var matmat12_B61 = zeroMatrix(3, 3);
    var matmat13_B61 = zeroMatrix(3, 3);
    var matmat14_B61 = zeroMatrix(3, 3);
    var matmat15_B61 = zeroMatrix(3, 3);

    var plusmat1_B61 = zeroMatrix(3, 3);
    var plusmat2_B61 = zeroMatrix(3, 3);
    var plusmat3_B61 = zeroMatrix(3, 3);
    var plusmat4_B61 = zeroMatrix(3, 3);
    var B61 = zeroMatrix(3, 3);

    matmat1_B61 = MatMatMult(Rotations.R, TSsj21);

    matmat2_B61 = MatMatMult(TSsc21, Rotations.TR21);
    matmat3_B61 = MatMatMult(Rotations.R2, matmat2_B61);

    matmat4_B61 = MatMatMult(TSsj32, Rotations.TR21);
    matmat5_B61 = MatMatMult(Rotations.R2, matmat4_B61);

    matmat6_B61 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat7_B61 = MatMatMult(TSsc32, matmat6_B61);
    matmat8_B61 = MatMatMult(Rotations.R3, matmat7_B61);


    matmat9_B61 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat10_B61 = MatMatMult(TSsj43, matmat9_B61);
    matmat11_B61 = MatMatMult(Rotations.R3, matmat10_B61);

    matmat12_B61 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat13_B61 = MatMatMult(Rotations.TR43, matmat12_B61);
    matmat14_B61 = MatMatMult(TSsc43, matmat13_B61);
    matmat15_B61 = MatMatMult(Rotations.R4, matmat14_B61);

    plusmat1_B61 = MatMatAdd(matmat1_B61, matmat3_B61);
    plusmat2_B61 = MatMatAdd(plusmat1_B61, matmat5_B61);
    plusmat3_B61 = MatMatAdd(plusmat2_B61, matmat8_B61);
    plusmat4_B61 = MatMatAdd(plusmat3_B61, matmat11_B61);
    B61 = MatMatAdd(plusmat4_B61, matmat15_B61);


    // B71

    var matmat1_B71 = zeroMatrix(3, 3);
    var B71 = zeroMatrix(3, 3);

    matmat1_B71 = MatMatMult(Rotations.TR32, Rotations.TR21);
    B71 = MatMatMult(Rotations.TR43, matmat1_B71); //B71

    // B22

    var matvec1_B22 = zeroMatrix(3, 1);
    var B22 = zeroMatrix(3, 1);

    matvec1_B22 = MatVecMult(TSsc21, e3);
    B22 = MatVecMult(Rotations.R2, matvec1_B22); // B22

    // B42



    var matvec1_B42 = zeroVector(3);
    var matvec2_B42 = zeroVector(3);
    var matvec3_B42 = zeroVector(3);
    var matvec4_B42 = zeroVector(3);
    var matvec5_B42 = zeroVector(3);
    var matvec6_B42 = zeroVector(3);
    var matvec7_B42 = zeroVector(3);

    var vecvec1_B42 = zeroVector(3);
    var B42 = zeroVector(3);
    //This is zero, always
    matvec1_B42 = MatVecMult(TSsc21, e3);
    matvec2_B42 = MatVecMult(Rotations.R2, matvec1_B42);


    matvec3_B42 = MatVecMult(TSsj32, e3);
    matvec4_B42 = MatVecMult(Rotations.R2, matvec3_B42);
    //No Distance other than up,

    matvec5_B42 = MatVecMult(Rotations.TR32, e3);
    matvec6_B42 = MatVecMult(TSsc32, matvec5_B42);
    matvec7_B42 = MatVecMult(Rotations.R3, matvec6_B42);

    vecvec1_B42 = VecVecAdd(matvec2_B42, matvec4_B42);
    B42 = VecVecAdd(vecvec1_B42, matvec7_B42);

    // B52


    var B52 = zeroVector(3);

    B52 = MatVecMult(Rotations.TR32, e3);

    // B62

    var matvec1_B62 = zeroVector(3);
    var matvec2_B62 = zeroVector(3);
    var matvec3_B62 = zeroVector(3);
    var matvec4_B62 = zeroVector(3);
    var matvec5_B62 = zeroVector(3);
    var matvec6_B62 = zeroVector(3);
    var matvec7_B62 = zeroVector(3);
    var matvec8_B62 = zeroVector(3);
    var matvec9_B62 = zeroVector(3);
    var matvec10_B62 = zeroVector(3);
    var matvec11_B62 = zeroVector(3);
    var matvec12_B62 = zeroVector(3);
    var matvec13_B62 = zeroVector(3);
    var matvec14_B62 = zeroVector(3);

    var vecvec1_B62 = zeroVector(3);
    var vecvec2_B62 = zeroVector(3);
    var vecvec3_B62 = zeroVector(3);
    var B62 = zeroVector(3);

    matvec1_B62 = MatVecMult(TSsc21, e3);
    matvec2_B62 = MatVecMult(Rotations.R2, matvec1_B62);

    matvec3_B62 = MatVecMult(TSsj32, e3);
    matvec4_B62 = MatVecMult(Rotations.R2, matvec3_B62);

    matvec5_B62 = MatVecMult(Rotations.TR32, e3);
    matvec6_B62 = MatVecMult(TSsc32, matvec5_B62);
    matvec7_B62 = MatVecMult(Rotations.R3, matvec6_B62);



    matvec8_B62 = MatVecMult(Rotations.TR32, e3);
    matvec9_B62 = MatVecMult(TSsj43, matvec8_B62);
    matvec10_B62 = MatVecMult(Rotations.R3, matvec9_B62);

    matvec11_B62 = MatVecMult(Rotations.TR32, e3);
    matvec12_B62 = MatVecMult(Rotations.TR43, matvec11_B62);
    matvec13_B62 = MatVecMult(TSsc43, matvec12_B62);
    matvec14_B62 = MatVecMult(Rotations.R4, matvec13_B62);

    vecvec1_B62 = VecVecAdd(matvec2_B62, matvec4_B62);
    vecvec2_B62 = VecVecAdd(vecvec1_B62, matvec7_B62);
    vecvec3_B62 = VecVecAdd(vecvec2_B62, matvec10_B62);
    B62 = VecVecAdd(vecvec3_B62, matvec14_B62);

    // B72

    var matvec1_B72 = zeroVector(3);
    var B72 = zeroVector(3);

    matvec1_B72 = MatVecMult(Rotations.TR32, e3);
    B72 = MatVecMult(Rotations.TR43, matvec1_B72);

    // B43

    var matvec1_B43 = zeroVector(3);
    var B43 = zeroVector(3);

    matvec1_B43 = MatVecMult(TSsc32, e1);
    B43 = MatVecMult(Rotations.R3, matvec1_B43);

    // B63

    var matvec1_B63 = zeroVector(3);
    var matvec2_B63 = zeroVector(3);
    var matvec3_B63 = zeroVector(3);
    var matvec4_B63 = zeroVector(3);
    var matvec5_B63 = zeroVector(3);
    var matvec6_B63 = zeroVector(3);
    var matvec7_B63 = zeroVector(3);

    var vecvec1_B63 = zeroVector(3);
    var B63 = zeroVector(3);

    matvec1_B63 = MatVecMult(TSsc32, e1);
    matvec2_B63 = MatVecMult(Rotations.R3, matvec1_B63);

    matvec3_B63 = MatVecMult(TSsj43, e1);
    matvec4_B63 = MatVecMult(Rotations.R3, matvec3_B63);

    matvec5_B63 = MatVecMult(Rotations.TR43, e1);
    matvec6_B63 = MatVecMult(TSsc43, matvec5_B63);
    matvec7_B63 = MatVecMult(Rotations.R4, matvec6_B63);

    vecvec1_B63 = VecVecAdd(matvec2_B63, matvec4_B63);
    B63 = VecVecAdd(vecvec1_B63, matvec7_B63);

    // B73

    var B73 = zeroVector(3);

    B73 = MatVecMult(Rotations.TR43, e1);

    // B64

    var matvec1_B64 = zeroVector(3);
    var B64 = zeroVector(3);

    matvec1_B64 = MatVecMult(TSsc43, e1);
    B64 = MatVecMult(Rotations.R4, matvec1_B64);

    // B-matrix

    Bn[0][0] = 1;

    Bn[1][1] = 1;
    Bn[2][2] = 1;





    Bn[3][0] = B21[0][0];
    Bn[4][0] = B21[1][0];
    Bn[5][0] = B21[2][0];
    Bn[3][1] = B21[0][1];
    Bn[4][1] = B21[1][1];
    Bn[5][1] = B21[2][1];
    Bn[3][2] = B21[0][2];
    Bn[4][2] = B21[1][2];
    Bn[5][2] = B21[2][2];



    Bn[6][0] = Rotations.TR21[0][0];
    Bn[7][0] = Rotations.TR21[1][0];
    Bn[8][0] = Rotations.TR21[2][0];
    Bn[6][1] = Rotations.TR21[0][1];
    Bn[7][1] = Rotations.TR21[1][1];
    Bn[8][1] = Rotations.TR21[2][1];
    Bn[6][2] = Rotations.TR21[0][2];
    Bn[7][2] = Rotations.TR21[1][2];
    Bn[8][2] = Rotations.TR21[2][2];



    Bn[9][0] = B41[0][0];
    Bn[10][0] = B41[1][0];
    Bn[11][0] = B41[2][0];
    Bn[9][1] = B41[0][1];
    Bn[10][1] = B41[1][1];
    Bn[11][1] = B41[2][1];
    Bn[9][2] = B41[0][2];
    Bn[10][2] = B41[1][2];
    Bn[11][2] = B41[2][2];

    Bn[12][0] = B51[0][0];
    Bn[13][0] = B51[1][0];
    Bn[14][0] = B51[2][0];
    Bn[12][1] = B51[0][1];
    Bn[13][1] = B51[1][1];
    Bn[14][1] = B51[2][1];
    Bn[12][2] = B51[0][2];
    Bn[13][2] = B51[1][2];
    Bn[14][2] = B51[2][2];

    Bn[15][0] = B61[0][0];
    Bn[16][0] = B61[1][0];
    Bn[17][0] = B61[2][0];
    Bn[15][1] = B61[0][1];
    Bn[16][1] = B61[1][1];
    Bn[17][1] = B61[2][1];
    Bn[15][2] = B61[0][2];
    Bn[16][2] = B61[1][2];
    Bn[17][2] = B61[2][2];

    Bn[18][0] = B71[0][0];
    Bn[19][0] = B71[1][0];
    Bn[20][0] = B71[2][0];
    Bn[18][1] = B71[0][1];
    Bn[19][1] = B71[1][1];
    Bn[20][1] = B71[2][1];
    Bn[18][2] = B71[0][2];
    Bn[19][2] = B71[1][2];
    Bn[20][2] = B71[2][2];

    Bn[3][3] = B22[0];
    Bn[4][3] = B22[1];
    Bn[5][3] = B22[2];

    Bn[6][3] = e3[0];
    Bn[7][3] = e3[1];
    Bn[8][3] = e3[2];

    Bn[9][3] = B42[0];
    Bn[10][3] = B42[1];
    Bn[11][3] = B42[2];

    Bn[12][3] = B52[0];
    Bn[13][3] = B52[1];
    Bn[14][3] = B52[2];

    Bn[15][3] = B62[0];
    Bn[16][3] = B62[1];
    Bn[17][3] = B62[2];

    Bn[18][3] = B72[0];
    Bn[19][3] = B72[1];
    Bn[20][3] = B72[2];

    Bn[9][4] = B43[0];
    Bn[10][4] = B43[1];
    Bn[11][4] = B43[2];

    Bn[12][4] = e1[0];
    Bn[13][4] = e1[1];
    Bn[14][4] = e1[2];

    Bn[15][4] = B63[0];
    Bn[16][4] = B63[1];
    Bn[17][4] = B63[2];

    Bn[18][4] = B73[0];
    Bn[19][4] = B73[1];
    Bn[20][4] = B73[2];

    Bn[15][5] = B64[0];
    Bn[16][5] = B64[1];
    Bn[17][5] = B64[2];

    Bn[18][5] = e1[0];
    Bn[19][5] = e1[1];
    Bn[20][5] = e1[2];

}


function get_Bndot(Bndot, Q, Rotations) {

    var sj21 = [myPhysics.b2, myPhysics.h21, myPhysics.L21];
    //var sj21 = [11.9, 7.5, 13.5];
    var sc21 = [0, myPhysics.h22, myPhysics.L22];
    //var sc21 = [0, 0, 2.6];

    var sj32 = [0, myPhysics.h31, myPhysics.L31];
    //var sj32 = [0, 0, 3.5];
    var sc32 = [0, myPhysics.h32, myPhysics.L32];
    //var sc32 = [10.7, 0, 0];

    var sj43 = [0, myPhysics.h41, myPhysics.L41];
    // var sj43 = [11.3, 0, 0];
    var sc43 = [0, myPhysics.h42, myPhysics.L42];
    // var sc43 = [-4.1, 0, -2.4];

    // does this informarion need to be restated? or is it enough to have it further up

    var Ssj21 = zeroMatrix(3, 3);
    var Ssj32 = zeroMatrix(3, 3);
    var Ssj43 = zeroMatrix(3, 3);
    var Ssc21 = zeroMatrix(3, 3);
    var Ssc32 = zeroMatrix(3, 3);
    var Ssc43 = zeroMatrix(3, 3);

    var e1 = [1, 0, 0];
    var e3 = [0, 1, 0];

    Ssj21 = SkewMat(sj21);
    Ssj32 = SkewMat(sj32);
    Ssj43 = SkewMat(sj43);
    Ssc21 = SkewMat(sc21);
    Ssc32 = SkewMat(sc32);
    Ssc43 = SkewMat(sc43);

    var TSsj21 = zeroMatrix(3, 3);
    var TSsj32 = zeroMatrix(3, 3);
    var TSsj43 = zeroMatrix(3, 3);
    var TSsc21 = zeroMatrix(3, 3);
    var TSsc32 = zeroMatrix(3, 3);
    var TSsc43 = zeroMatrix(3, 3);

    TSsj21 = MatTran(Ssj21);
    TSsj32 = MatTran(Ssj32);
    TSsj43 = MatTran(Ssj43);
    TSsc21 = MatTran(Ssc21);
    TSsc32 = MatTran(Ssc32);
    TSsc43 = MatTran(Ssc43);


    // dB21

    var matmat1_dB21 = zeroMatrix(3, 3);
    var matmat2_dB21 = zeroMatrix(3, 3);
    var matmat3_dB21 = zeroMatrix(3, 3);
    var matmat4_dB21 = zeroMatrix(3, 3);
    var matmat5_dB21 = zeroMatrix(3, 3);

    var plusmat1_dB21 = zeroMatrix(3, 3);
    var dB21 = zeroMatrix(3, 3);



    matmat1_dB21 = MatMatMult(Rotations.Rdot, TSsj21);


    matmat2_dB21 = MatMatMult(TSsc21, Rotations.dTR21);
    matmat3_dB21 = MatMatMult(Rotations.R2, matmat2_dB21);


    matmat4_dB21 = MatMatMult(TSsc21, Rotations.TR21);
    matmat5_dB21 = MatMatMult(Rotations.dR2, matmat4_dB21);

    plusmat1_dB21 = MatMatAdd(matmat1_dB21, matmat3_dB21);
    dB21 = MatMatAdd(plusmat1_dB21, matmat5_dB21); // dB21

    // dB41

    var matmat1_dB41 = zeroMatrix(3, 3);
    var matmat2_dB41 = zeroMatrix(3, 3);
    var matmat3_dB41 = zeroMatrix(3, 3);
    var matmat4_dB41 = zeroMatrix(3, 3);
    var matmat5_dB41 = zeroMatrix(3, 3);
    var matmat6_dB41 = zeroMatrix(3, 3);
    var matmat7_dB41 = zeroMatrix(3, 3);
    var matmat8_dB41 = zeroMatrix(3, 3);
    var matmat9_dB41 = zeroMatrix(3, 3);
    var matmat10_dB41 = zeroMatrix(3, 3);
    var matmat11_dB41 = zeroMatrix(3, 3);
    var matmat12_dB41 = zeroMatrix(3, 3);
    var matmat13_dB41 = zeroMatrix(3, 3);
    var matmat14_dB41 = zeroMatrix(3, 3);
    var matmat15_dB41 = zeroMatrix(3, 3);
    var matmat16_dB41 = zeroMatrix(3, 3);
    var matmat17_dB41 = zeroMatrix(3, 3);
    var matmat18_dB41 = zeroMatrix(3, 3);

    var plusmat1_dB41 = zeroMatrix(3, 3);
    var plusmat2_dB41 = zeroMatrix(3, 3);
    var plusmat3_dB41 = zeroMatrix(3, 3);
    var plusmat4_dB41 = zeroMatrix(3, 3);
    var plusmat5_dB41 = zeroMatrix(3, 3);
    var plusmat6_dB41 = zeroMatrix(3, 3);
    var dB41 = zeroMatrix(3, 3);

    matmat1_dB41 = MatMatMult(Rotations.Rdot, TSsj21);

    matmat2_dB41 = MatMatMult(TSsc21, Rotations.dTR21);
    matmat3_dB41 = MatMatMult(Rotations.R2, matmat2_dB41);

    matmat4_dB41 = MatMatMult(TSsc21, Rotations.TR21);
    matmat5_dB41 = MatMatMult(Rotations.dR2, matmat4_dB41);



    matmat6_dB41 = MatMatMult(TSsj32, Rotations.dTR21);
    matmat7_dB41 = MatMatMult(Rotations.R2, matmat6_dB41);

    matmat8_dB41 = MatMatMult(TSsj32, Rotations.TR21);
    matmat9_dB41 = MatMatMult(Rotations.dR2, matmat8_dB41);


    matmat10_dB41 = MatMatMult(Rotations.TR32, Rotations.dTR21);
    matmat11_dB41 = MatMatMult(TSsc32, matmat10_dB41);
    matmat12_dB41 = MatMatMult(Rotations.R3, matmat11_dB41);

    matmat13_dB41 = MatMatMult(Rotations.dTR32, Rotations.TR21);
    matmat14_dB41 = MatMatMult(TSsc32, matmat13_dB41);
    matmat15_dB41 = MatMatMult(Rotations.R3, matmat14_dB41);

    matmat16_dB41 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat17_dB41 = MatMatMult(TSsc32, matmat16_dB41);
    matmat18_dB41 = MatMatMult(Rotations.dR3, matmat17_dB41);

    plusmat1_dB41 = MatMatAdd(matmat1_dB41, matmat3_dB41);
    plusmat2_dB41 = MatMatAdd(plusmat1_dB41, matmat5_dB41);
    plusmat3_dB41 = MatMatAdd(plusmat2_dB41, matmat7_dB41);
    plusmat4_dB41 = MatMatAdd(plusmat3_dB41, matmat9_dB41);
    plusmat5_dB41 = MatMatAdd(plusmat4_dB41, matmat12_dB41);
    plusmat6_dB41 = MatMatAdd(plusmat5_dB41, matmat15_dB41);

    dB41 = MatMatAdd(plusmat6_dB41, matmat18_dB41); //dB41

    // dB51

    var matmat1_dB51 = zeroMatrix(3, 3);
    var matmat2_dB51 = zeroMatrix(3, 3);
    var dB51 = zeroMatrix(3, 3);

    matmat1_dB51 = MatMatMult(Rotations.TR32, Rotations.dTR21);
    matmat2_dB51 = MatMatMult(Rotations.dTR32, Rotations.TR21);

    dB51 = MatMatAdd(matmat1_dB51, matmat2_dB51);

    // dB61

    var matmat1_dB61 = zeroMatrix(3, 3);
    var matmat2_dB61 = zeroMatrix(3, 3);
    var matmat3_dB61 = zeroMatrix(3, 3);
    var matmat4_dB61 = zeroMatrix(3, 3);
    var matmat5_dB61 = zeroMatrix(3, 3);
    var matmat6_dB61 = zeroMatrix(3, 3);
    var matmat7_dB61 = zeroMatrix(3, 3);
    var matmat8_dB61 = zeroMatrix(3, 3);
    var matmat9_dB61 = zeroMatrix(3, 3);
    var matmat10_dB61 = zeroMatrix(3, 3);
    var matmat11_dB61 = zeroMatrix(3, 3);
    var matmat12_dB61 = zeroMatrix(3, 3);
    var matmat13_dB61 = zeroMatrix(3, 3);
    var matmat14_dB61 = zeroMatrix(3, 3);
    var matmat15_dB61 = zeroMatrix(3, 3);
    var matmat16_dB61 = zeroMatrix(3, 3);
    var matmat17_dB61 = zeroMatrix(3, 3);
    var matmat18_dB61 = zeroMatrix(3, 3);
    var matmat19_dB61 = zeroMatrix(3, 3);
    var matmat20_dB61 = zeroMatrix(3, 3);
    var matmat21_dB61 = zeroMatrix(3, 3);
    var matmat22_dB61 = zeroMatrix(3, 3);
    var matmat23_dB61 = zeroMatrix(3, 3);
    var matmat24_dB61 = zeroMatrix(3, 3);
    var matmat25_dB61 = zeroMatrix(3, 3);
    var matmat26_dB61 = zeroMatrix(3, 3);
    var matmat27_dB61 = zeroMatrix(3, 3);
    var matmat28_dB61 = zeroMatrix(3, 3);
    var matmat29_dB61 = zeroMatrix(3, 3);
    var matmat30_dB61 = zeroMatrix(3, 3);
    var matmat31_dB61 = zeroMatrix(3, 3);
    var matmat32_dB61 = zeroMatrix(3, 3);
    var matmat33_dB61 = zeroMatrix(3, 3);
    var matmat34_dB61 = zeroMatrix(3, 3);
    var matmat35_dB61 = zeroMatrix(3, 3);
    var matmat36_dB61 = zeroMatrix(3, 3);
    var matmat37_dB61 = zeroMatrix(3, 3);
    var matmat38_dB61 = zeroMatrix(3, 3);
    var matmat39_dB61 = zeroMatrix(3, 3);
    var matmat40_dB61 = zeroMatrix(3, 3);
    var matmat41_dB61 = zeroMatrix(3, 3);
    var matmat42_dB61 = zeroMatrix(3, 3);
    var matmat43_dB61 = zeroMatrix(3, 3);

    var plusmat1_dB61 = zeroMatrix(3, 3);
    var plusmat2_dB61 = zeroMatrix(3, 3);
    var plusmat3_dB61 = zeroMatrix(3, 3);
    var plusmat4_dB61 = zeroMatrix(3, 3);
    var plusmat5_dB61 = zeroMatrix(3, 3);
    var plusmat6_dB61 = zeroMatrix(3, 3);
    var plusmat7_dB61 = zeroMatrix(3, 3);
    var plusmat8_dB61 = zeroMatrix(3, 3);
    var plusmat9_dB61 = zeroMatrix(3, 3);
    var plusmat10_dB61 = zeroMatrix(3, 3);
    var plusmat11_dB61 = zeroMatrix(3, 3);
    var plusmat12_dB61 = zeroMatrix(3, 3);
    var plusmat13_dB61 = zeroMatrix(3, 3);
    var dB61 = zeroMatrix(3, 3);
    //From dB21
    matmat1_dB61 = MatMatMult(Rotations.Rdot, TSsj21);

    matmat2_dB61 = MatMatMult(TSsc21, Rotations.dTR21);
    matmat3_dB61 = MatMatMult(Rotations.R2, matmat2_dB61);

    matmat4_dB61 = MatMatMult(TSsc21, Rotations.TR21);
    matmat5_dB61 = MatMatMult(Rotations.dR2, matmat4_dB61);
    // From dB41
    matmat6_dB61 = MatMatMult(TSsj32, Rotations.dTR21);
    matmat7_dB61 = MatMatMult(Rotations.R2, matmat6_dB61);

    matmat8_dB61 = MatMatMult(TSsj32, Rotations.TR21);
    matmat9_dB61 = MatMatMult(Rotations.dR2, matmat8_dB61);

    matmat10_dB61 = MatMatMult(Rotations.TR32, Rotations.dTR21);
    matmat11_dB61 = MatMatMult(TSsc32, matmat10_dB61);
    matmat12_dB61 = MatMatMult(Rotations.R3, matmat11_dB61);

    matmat13_dB61 = MatMatMult(Rotations.dTR32, Rotations.TR21);
    matmat14_dB61 = MatMatMult(TSsc32, matmat13_dB61);
    matmat15_dB61 = MatMatMult(Rotations.R3, matmat14_dB61);

    matmat16_dB61 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat17_dB61 = MatMatMult(TSsc32, matmat16_dB61);
    matmat18_dB61 = MatMatMult(Rotations.dR3, matmat17_dB61);
    // -----------------------------------------------------------------------
    matmat19_dB61 = MatMatMult(Rotations.TR32, Rotations.dTR21);
    matmat20_dB61 = MatMatMult(TSsj43, matmat19_dB61);
    matmat21_dB61 = MatMatMult(Rotations.R3, matmat20_dB61);

    matmat22_dB61 = MatMatMult(Rotations.dTR32, Rotations.TR21);
    matmat23_dB61 = MatMatMult(TSsj43, matmat22_dB61);
    matmat24_dB61 = MatMatMult(Rotations.R3, matmat23_dB61);

    matmat25_dB61 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat26_dB61 = MatMatMult(TSsj43, matmat25_dB61);
    matmat27_dB61 = MatMatMult(Rotations.dR3, matmat26_dB61);


    matmat28_dB61 = MatMatMult(Rotations.TR32, Rotations.dTR21);
    matmat29_dB61 = MatMatMult(Rotations.TR43, matmat28_dB61);
    matmat30_dB61 = MatMatMult(TSsc43, matmat29_dB61);
    matmat31_dB61 = MatMatMult(Rotations.R4, matmat30_dB61);

    matmat32_dB61 = MatMatMult(Rotations.dTR32, Rotations.TR21);
    matmat33_dB61 = MatMatMult(Rotations.TR43, matmat32_dB61);
    matmat34_dB61 = MatMatMult(TSsc43, matmat33_dB61);
    matmat35_dB61 = MatMatMult(Rotations.R4, matmat34_dB61);

    matmat36_dB61 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat37_dB61 = MatMatMult(Rotations.dTR43, matmat36_dB61);
    matmat38_dB61 = MatMatMult(TSsc43, matmat37_dB61);
    matmat39_dB61 = MatMatMult(Rotations.R4, matmat38_dB61);

    matmat40_dB61 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat41_dB61 = MatMatMult(Rotations.TR43, matmat40_dB61);
    matmat42_dB61 = MatMatMult(TSsc43, matmat41_dB61);
    matmat43_dB61 = MatMatMult(Rotations.dR4, matmat42_dB61);

    plusmat1_dB61 = MatMatAdd(matmat1_dB61, matmat3_dB61);
    plusmat2_dB61 = MatMatAdd(plusmat1_dB61, matmat5_dB61);
    plusmat3_dB61 = MatMatAdd(plusmat2_dB61, matmat7_dB61);
    plusmat4_dB61 = MatMatAdd(plusmat3_dB61, matmat9_dB61);
    plusmat5_dB61 = MatMatAdd(plusmat4_dB61, matmat12_dB61);
    plusmat6_dB61 = MatMatAdd(plusmat5_dB61, matmat15_dB61);
    plusmat7_dB61 = MatMatAdd(plusmat6_dB61, matmat18_dB61);
    plusmat8_dB61 = MatMatAdd(plusmat7_dB61, matmat21_dB61);
    plusmat9_dB61 = MatMatAdd(plusmat8_dB61, matmat24_dB61);
    plusmat10_dB61 = MatMatAdd(plusmat9_dB61, matmat27_dB61);
    plusmat11_dB61 = MatMatAdd(plusmat10_dB61, matmat31_dB61);
    plusmat12_dB61 = MatMatAdd(plusmat11_dB61, matmat35_dB61);
    plusmat13_dB61 = MatMatAdd(plusmat12_dB61, matmat39_dB61);

    dB61 = MatMatAdd(plusmat13_dB61, matmat43_dB61);


    // dB71

    var matmat1_dB71 = zeroMatrix(3, 3);
    var matmat2_dB71 = zeroMatrix(3, 3);
    var matmat3_dB71 = zeroMatrix(3, 3);
    var matmat4_dB71 = zeroMatrix(3, 3);
    var matmat5_dB71 = zeroMatrix(3, 3);
    var matmat6_dB71 = zeroMatrix(3, 3);

    var plusmat1_dB71 = zeroMatrix(3, 3);
    var dB71 = zeroMatrix(3, 3);

    matmat1_dB71 = MatMatMult(Rotations.TR32, Rotations.dTR21);
    matmat2_dB71 = MatMatMult(Rotations.TR43, matmat1_dB71);

    matmat3_dB71 = MatMatMult(Rotations.dTR32, Rotations.TR21);
    matmat4_dB71 = MatMatMult(Rotations.TR43, matmat3_dB71);

    matmat5_dB71 = MatMatMult(Rotations.TR32, Rotations.TR21);
    matmat6_dB71 = MatMatMult(Rotations.dTR43, matmat5_dB71);

    plusmat1_dB71 = MatMatAdd(matmat2_dB71, matmat4_dB71);
    dB71 = MatMatAdd(plusmat1_dB71, matmat6_dB71);


    // dB22

    var matvec1_dB22 = zeroVector(3);
    var dB22 = zeroVector(3);
    // This is zero
    matvec1_dB22 = MatVecMult(TSsc21, e3);
    dB22 = MatVecMult(Rotations.dR2, matvec1_dB22); // dB22

    // dB42

    var matvec1_dB42 = zeroVector(3);
    var matvec2_dB42 = zeroVector(3);
    var matvec3_dB42 = zeroVector(3);
    var matvec4_dB42 = zeroVector(3);
    var matvec5_dB42 = zeroVector(3);
    var matvec6_dB42 = zeroVector(3);
    var matvec7_dB42 = zeroVector(3);
    var matvec8_dB42 = zeroVector(3);
    var matvec9_dB42 = zeroVector(3);
    var matvec10_dB42 = zeroVector(3);

    var vecvec1_dB42 = zeroVector(3);
    var vecvec2_dB42 = zeroVector(3);
    var dB42 = zeroVector(3);
    //This is zero
    matvec1_dB42 = MatVecMult(TSsc21, e3);
    matvec2_dB42 = MatVecMult(Rotations.dR2, matvec1_dB42);
    //This is zero
    matvec3_dB42 = MatVecMult(TSsj32, e3);
    matvec4_dB42 = MatVecMult(Rotations.dR2, matvec3_dB42);

    matvec5_dB42 = MatVecMult(Rotations.dTR32, e3);
    matvec6_dB42 = MatVecMult(TSsc32, matvec5_dB42);
    matvec7_dB42 = MatVecMult(Rotations.R3, matvec6_dB42);

    matvec8_dB42 = MatVecMult(Rotations.TR32, e3);
    matvec9_dB42 = MatVecMult(TSsc32, matvec8_dB42);
    matvec10_dB42 = MatVecMult(Rotations.dR3, matvec9_dB42);

    vecvec1_dB42 = VecVecAdd(matvec2_dB42, matvec4_dB42);
    vecvec2_dB42 = VecVecAdd(vecvec1_dB42, matvec7_dB42);
    dB42 = VecVecAdd(vecvec2_dB42, matvec10_dB42);

    // dB52


    var dB52 = zeroVector(3);

    dB52 = MatVecMult(Rotations.dTR32, e3);

    // dB62

    var matvec1_dB62 = zeroVector(3);
    var matvec2_dB62 = zeroVector(3);
    var matvec3_dB62 = zeroVector(3);
    var matvec4_dB62 = zeroVector(3);
    var matvec5_dB62 = zeroVector(3);
    var matvec6_dB62 = zeroVector(3);
    var matvec7_dB62 = zeroVector(3);
    var matvec8_dB62 = zeroVector(3);
    var matvec9_dB62 = zeroVector(3);
    var matvec10_dB62 = zeroVector(3);
    var matvec11_dB62 = zeroVector(3);
    var matvec12_dB62 = zeroVector(3);
    var matvec13_dB62 = zeroVector(3);
    var matvec14_dB62 = zeroVector(3);
    var matvec15_dB62 = zeroVector(3);
    var matvec16_dB62 = zeroVector(3);
    var matvec17_dB62 = zeroVector(3);
    var matvec18_dB62 = zeroVector(3);
    var matvec19_dB62 = zeroVector(3);
    var matvec20_dB62 = zeroVector(3);
    var matvec21_dB62 = zeroVector(3);
    var matvec22_dB62 = zeroVector(3);
    var matvec23_dB62 = zeroVector(3);
    var matvec24_dB62 = zeroVector(3);
    var matvec25_dB62 = zeroVector(3);
    var matvec26_dB62 = zeroVector(3);
    var matvec27_dB62 = zeroVector(3);
    var matvec28_dB62 = zeroVector(3);

    var vecvec1_dB62 = zeroVector(3);
    var vecvec2_dB62 = zeroVector(3);
    var vecvec3_dB62 = zeroVector(3);
    var vecvec4_dB62 = zeroVector(3);
    var vecvec5_dB62 = zeroVector(3);
    var vecvec6_dB62 = zeroVector(3);
    var vecvec7_dB62 = zeroVector(3);
    var dB62 = zeroVector(3);
    //This is zero
    matvec1_dB62 = MatVecMult(TSsc21, e3);
    matvec2_dB62 = MatVecMult(Rotations.dR2, matvec1_dB62);

    matvec3_dB62 = MatVecMult(TSsj32, e3);
    matvec4_dB62 = MatVecMult(Rotations.dR2, matvec3_dB62);
    //---------------------------------------------------------------
    //From dB42  
    matvec5_dB62 = MatVecMult(Rotations.dTR32, e3);
    matvec6_dB62 = MatVecMult(TSsc32, matvec5_dB62);
    matvec7_dB62 = MatVecMult(Rotations.R3, matvec6_dB62);

    matvec8_dB62 = MatVecMult(Rotations.TR32, e3);
    matvec9_dB62 = MatVecMult(TSsc32, matvec8_dB62);
    matvec10_dB62 = MatVecMult(Rotations.dR3, matvec9_dB62);
    //-----------------------------------------------------------------
    matvec11_dB62 = MatVecMult(Rotations.dTR32, e3);
    matvec12_dB62 = MatVecMult(TSsj43, matvec11_dB62);
    matvec13_dB62 = MatVecMult(Rotations.R3, matvec12_dB62);

    matvec14_dB62 = MatVecMult(Rotations.TR32, e3);
    matvec15_dB62 = MatVecMult(TSsj43, matvec14_dB62);
    matvec16_dB62 = MatVecMult(Rotations.dR3, matvec15_dB62);


    matvec17_dB62 = MatVecMult(Rotations.dTR32, e3);
    matvec18_dB62 = MatVecMult(Rotations.TR43, matvec17_dB62);
    matvec19_dB62 = MatVecMult(TSsc43, matvec18_dB62);
    matvec20_dB62 = MatVecMult(Rotations.R4, matvec19_dB62);

    matvec21_dB62 = MatVecMult(Rotations.TR32, e3);
    matvec22_dB62 = MatVecMult(Rotations.dTR43, matvec21_dB62);
    matvec23_dB62 = MatVecMult(TSsc43, matvec22_dB62);
    matvec24_dB62 = MatVecMult(Rotations.R4, matvec23_dB62);

    matvec25_dB62 = MatVecMult(Rotations.TR32, e3);
    matvec26_dB62 = MatVecMult(Rotations.TR43, matvec25_dB62);
    matvec27_dB62 = MatVecMult(TSsc43, matvec26_dB62);
    matvec28_dB62 = MatVecMult(Rotations.dR4, matvec27_dB62);

    vecvec1_dB62 = VecVecAdd(matvec2_dB62, matvec4_dB62);
    vecvec2_dB62 = VecVecAdd(vecvec1_dB62, matvec7_dB62);
    vecvec3_dB62 = VecVecAdd(vecvec2_dB62, matvec10_dB62);
    vecvec4_dB62 = VecVecAdd(vecvec3_dB62, matvec13_dB62);
    vecvec5_dB62 = VecVecAdd(vecvec4_dB62, matvec16_dB62);
    vecvec6_dB62 = VecVecAdd(vecvec5_dB62, matvec20_dB62);
    vecvec7_dB62 = VecVecAdd(vecvec6_dB62, matvec24_dB62);
    dB62 = VecVecAdd(vecvec7_dB62, matvec28_dB62);

    // dB72

    var matvec1_dB72 = zeroVector(3);
    var matvec2_dB72 = zeroVector(3);
    var matvec3_dB72 = zeroVector(3);
    var matvec4_dB72 = zeroVector(3);

    var dB72 = zeroVector(3);

    matvec1_dB72 = MatVecMult(Rotations.dTR32, e3);
    matvec2_dB72 = MatVecMult(Rotations.TR43, matvec1_dB72);

    matvec3_dB72 = MatVecMult(Rotations.TR32, e3);
    matvec4_dB72 = MatVecMult(Rotations.dTR43, matvec3_dB72);

    dB72 = VecVecAdd(matvec2_dB72, matvec4_dB72);


    // dB43

    var matvec1_dB43 = zeroVector(3);
    var dB43 = zeroVector(3);

    matvec1_dB43 = MatVecMult(TSsc32, e1);
    dB43 = MatVecMult(Rotations.dR3, matvec1_dB43);

    // dB63

    var matvec1_dB63 = zeroVector(3);
    var matvec2_dB63 = zeroVector(3);
    var matvec3_dB63 = zeroVector(3);
    var matvec4_dB63 = zeroVector(3);
    var matvec5_dB63 = zeroVector(3);
    var matvec6_dB63 = zeroVector(3);
    var matvec7_dB63 = zeroVector(3);
    var matvec8_dB63 = zeroVector(3);
    var matvec9_dB63 = zeroVector(3);
    var matvec10_dB63 = zeroVector(3);

    var vecvec1_dB63 = zeroVector(3);
    var vecvec2_dB63 = zeroVector(3);
    var dB63 = zeroVector(3);
    // From 43
    matvec1_dB63 = MatVecMult(TSsc32, e1);
    matvec2_dB63 = MatVecMult(Rotations.dR3, matvec1_dB63);
    //----------
    matvec3_dB63 = MatVecMult(TSsj43, e1);
    matvec4_dB63 = MatVecMult(Rotations.dR3, matvec3_dB63);

    // dTR43*e1 = [0,0,0];
    matvec5_dB63 = MatVecMult(Rotations.dTR43, e1);
    matvec6_dB63 = MatVecMult(TSsc43, matvec5_dB63);
    matvec7_dB63 = MatVecMult(Rotations.R4, matvec6_dB63);

    matvec8_dB63 = MatVecMult(Rotations.TR43, e1);
    matvec9_dB63 = MatVecMult(TSsc43, matvec8_dB63);
    matvec10_dB63 = MatVecMult(Rotations.dR4, matvec9_dB63);

    vecvec1_dB63 = VecVecAdd(matvec2_dB63, matvec4_dB63);
    vecvec2_dB63 = VecVecAdd(vecvec1_dB63, matvec7_dB63);
    dB63 = VecVecAdd(vecvec2_dB63, matvec10_dB63);

    // dB73

    var dB73 = zeroVector(3);

    dB73 = MatVecMult(Rotations.dTR43, e1);

    // dB64

    var matvec1_dB64 = zeroVector(3);
    var dB64 = zeroVector(3);

    matvec1_dB64 = MatVecMult(TSsc43, e1);
    dB64 = MatVecMult(Rotations.dR4, matvec1_dB64);

    // B-matrix dotted

    Bndot[3][0] = dB21[0][0];
    Bndot[4][0] = dB21[1][0];
    Bndot[5][0] = dB21[2][0];
    Bndot[3][1] = dB21[0][1];
    Bndot[4][1] = dB21[1][1];
    Bndot[5][1] = dB21[2][1];
    Bndot[3][2] = dB21[0][2];
    Bndot[4][2] = dB21[1][2];
    Bndot[5][2] = dB21[2][2];

    Bndot[6][0] = Rotations.dTR21[0][0];
    Bndot[7][0] = Rotations.dTR21[1][0];
    Bndot[8][0] = Rotations.dTR21[2][0];
    Bndot[6][1] = Rotations.dTR21[0][1];
    Bndot[7][1] = Rotations.dTR21[1][1];
    Bndot[8][1] = Rotations.dTR21[2][1];
    Bndot[6][2] = Rotations.dTR21[0][2];
    Bndot[7][2] = Rotations.dTR21[1][2];
    Bndot[8][2] = Rotations.dTR21[2][2];

    Bndot[9][0] = dB41[0][0];
    Bndot[10][0] = dB41[1][0];
    Bndot[11][0] = dB41[2][0];
    Bndot[9][1] = dB41[0][1];
    Bndot[10][1] = dB41[1][1];
    Bndot[11][1] = dB41[2][1];
    Bndot[9][2] = dB41[0][2];
    Bndot[10][2] = dB41[1][2];
    Bndot[11][2] = dB41[2][2];

    Bndot[12][0] = dB51[0][0];
    Bndot[13][0] = dB51[1][0];
    Bndot[14][0] = dB51[2][0];
    Bndot[12][1] = dB51[0][1];
    Bndot[13][1] = dB51[1][1];
    Bndot[14][1] = dB51[2][1];
    Bndot[12][2] = dB51[0][2];
    Bndot[13][2] = dB51[1][2];
    Bndot[14][2] = dB51[2][2];

    Bndot[15][0] = dB61[0][0];
    Bndot[16][0] = dB61[1][0];
    Bndot[17][0] = dB61[2][0];
    Bndot[15][1] = dB61[0][1];
    Bndot[16][1] = dB61[1][1];
    Bndot[17][1] = dB61[2][1];
    Bndot[15][2] = dB61[0][2];
    Bndot[16][2] = dB61[1][2];
    Bndot[17][2] = dB61[2][2];

    Bndot[18][0] = dB71[0][0];
    Bndot[19][0] = dB71[1][0];
    Bndot[20][0] = dB71[2][0];
    Bndot[18][1] = dB71[0][1];
    Bndot[19][1] = dB71[1][1];
    Bndot[20][1] = dB71[2][1];
    Bndot[18][2] = dB71[0][2];
    Bndot[19][2] = dB71[1][2];
    Bndot[20][2] = dB71[2][2];

    Bndot[3][3] = dB22[0];
    Bndot[4][3] = dB22[1];
    Bndot[5][3] = dB22[2];

    Bndot[9][3] = dB42[0];
    Bndot[10][3] = dB42[1];
    Bndot[11][3] = dB42[2];

    Bndot[12][3] = dB52[0];
    Bndot[13][3] = dB52[1];
    Bndot[14][3] = dB52[2];

    Bndot[15][3] = dB62[0];
    Bndot[16][3] = dB62[1];
    Bndot[17][3] = dB62[2];

    Bndot[18][3] = dB72[0];
    Bndot[19][3] = dB72[1];
    Bndot[20][3] = dB72[2];

    Bndot[9][4] = dB43[0];
    Bndot[10][4] = dB43[1];
    Bndot[11][4] = dB43[2];

    Bndot[15][4] = dB63[0];
    Bndot[16][4] = dB63[1];
    Bndot[17][4] = dB63[2];

    Bndot[18][4] = dB73[0];
    Bndot[19][4] = dB73[1];
    Bndot[20][4] = dB73[2];

    Bndot[15][5] = dB64[0];
    Bndot[16][5] = dB64[1];
    Bndot[17][5] = dB64[2];
}