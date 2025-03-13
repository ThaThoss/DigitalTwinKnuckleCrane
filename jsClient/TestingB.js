function BTest() {
    var J1_00 = 2267.5;
    var J1_10 = -989.57;
    var J1_20 = -1.5592;
    var J1_01 = -989.57;
    var J1_11 = 8698.78;
    var J1_21 = -1.9803;
    var J1_02 = -1.5592;
    var J1_12 = -1.9803;
    var J1_22 = 9428.4;

    var J2_00 = 0.9843;
    var J2_10 = 0;
    var J2_20 = 0;
    var J2_01 = 0;
    var J2_11 = 0.6945;
    var J2_21 = 0.0689;
    var J2_02 = 0;
    var J2_12 = 0.0689;
    var J2_22 = 1.007;

    var J3_00 = 0.475;
    var J3_10 = 0;
    var J3_20 = 0.0161;
    var J3_01 = 0;
    var J3_11 = 5.914;
    var J3_21 = 0;
    var J3_02 = 0.0161;
    var J3_12 = 0;
    var J3_22 = 5.757;

    var J4_00 = 0.425;
    var J4_10 = 0;
    var J4_20 = 0.57;
    var J4_01 = 0;
    var J4_11 = 1.688;
    var J4_21 = 0;
    var J4_02 = 0.57;
    var J4_12 = 0;
    var J4_22 = 1.787;

    var m2 = 160
    var m3 = 150
    var m4 = 150

    var numGenCoord = 6;
    var numElemProp = 21;
    var BnT = zeroMatrix(numGenCoord, numElemProp);
    var Bn = zeroMatrix(numElemProp, numGenCoord);
    var Bndot = zeroMatrix(numElemProp, numGenCoord);
    var D = zeroMatrix(21, 21);
    var dt = 0.001;
    var i = 0;
    var M = zeroMatrix(numElemProp, numElemProp);
    var DM = zeroMatrix(numElemProp, numElemProp);
    var DMB = zeroMatrix(numElemProp, numGenCoord);
    var MBndotmDMB = zeroMatrix(numElemProp, numGenCoord);
    var F = zeroVector(numElemProp);
    var Fstar = zeroVector(numGenCoord);
    var MBn = zeroMatrix(numElemProp, numGenCoord);
    var MBndot = zeroMatrix(numElemProp, numGenCoord);
    var MstarIFmNstarqn = zeroVector(numGenCoord);
    var NStarqn = zeroVector(numGenCoord);
    var FmNstarqn = zeroVector(numGenCoord);
    var Mstar = zeroMatrix(numGenCoord, numGenCoord);
    var Nstar = zeroMatrix(numGenCoord, numGenCoord);
    var MstarI = zeroMatrix(numGenCoord, numGenCoord);
    var TempR = zeroMatrix(3, 3);
    var TempR2 = MyEye(3, 3);
    var Acc = zeroVector(6);
    var TempR = zeroMatrix(3, 3);
    var TempR2 = MyEye(3, 3);
    F[5] = -9.81;
    F[11] = -9.81;
    F[17] = -9.81;
    var Q = {
        q1r: 1,
        q2r: 2,
        q3r: 3,
        q4r: 4,
        q5r: 5,
        q6r: 6, // added
        q4: 3.14 / 2,
        q5: 3.14 / 3,
        q6: 3.14 / 4 // added
    }


    var Rotations = {

        R: MyEye(3, 3),
        Rp: MyEye(3, 3),
        Rdot: zeroMatrix(3, 3),
        R21: MyEye(3, 3),
        R2: MyEye(3, 3),
        R32: MyEye(3, 3),
        R3: MyEye(3, 3),
        R43: MyEye(3, 3),
        R4: MyEye(3, 3),

        TR21: MyEye(3, 3),
        TR32: MyEye(3, 3),
        TR43: MyEye(3, 3),
        TR: MyEye(3, 3),

        dTR21: MyEye(3, 3),
        dTR32: MyEye(3, 3),
        dTR43: MyEye(3, 3),

        dR21: MyEye(3, 3),
        dR2: MyEye(3, 3),
        dR32: MyEye(3, 3),
        dR3: MyEye(3, 3),
        dR43: MyEye(3, 3),
        dR4: MyEye(3, 3),

        dR2_1: MyEye(3, 3),
        dR2_2: MyEye(3, 3),
        dR3_1: MyEye(3, 3),
        dR3_2: MyEye(3, 3),
        dR4_1: MyEye(3, 3),
        dR4_2: MyEye(3, 3)

    }
    M[0][0] = J1_00;
    M[1][0] = J1_10;
    M[2][0] = J1_20;
    M[0][1] = J1_01;
    M[1][1] = J1_11;
    M[2][1] = J1_21;
    M[0][2] = J1_02;
    M[1][2] = J1_12;
    M[2][2] = J1_22;

    M[3][3] = m2;
    M[4][4] = m2;
    M[5][5] = m2;

    M[6][6] = J2_00;
    M[7][6] = J2_10;
    M[8][6] = J2_20;
    M[6][7] = J2_01;
    M[7][7] = J2_11;
    M[8][7] = J2_21;
    M[6][8] = J2_02;
    M[7][8] = J2_12;
    M[8][8] = J2_22;

    M[9][9] = m3;
    M[10][10] = m3;
    M[11][11] = m3;

    M[12][12] = J3_00;
    M[13][12] = J3_10;
    M[14][12] = J3_20;
    M[12][13] = J3_01;
    M[13][13] = J3_11;
    M[14][13] = J3_21;
    M[12][14] = J3_02;
    M[13][14] = J3_12;
    M[14][14] = J3_22;

    M[15][15] = m4;
    M[16][16] = m4;
    M[17][17] = m4;

    M[18][18] = J4_00;
    M[19][18] = J4_10;
    M[20][18] = J4_20;
    M[18][19] = J4_01;
    M[19][19] = J4_11;
    M[20][19] = J4_21;
    M[18][20] = J4_02;
    M[19][20] = J4_12;
    M[20][20] = J4_22;

    TempR = MatMatMult(TempR2, expMRodriguez(Q.q1r, Q.q2r, Q.q3r, dt, 1));
    CopyMatrix(TempR, Rotations.R);
    //Rotations.R = expMRodriguez(Q.q1r, Q.q2r, Q.q3r, time, 1);

    //MyVectorPrinter([w1,w2,w3],3);
    //MyMatrixPrinter(Rotations.R,3,3);
    makeRotation(Rotations.R21, Q.q4, 3);
    makeRotation(Rotations.R32, Q.q5, 2);
    makeRotation(Rotations.R43, Q.q6, 2);

    MyMatMatMult(Rotations.R2, Rotations.R, Rotations.R21, 3, 3, 3);
    MyMatMatMult(Rotations.R3, Rotations.R2, Rotations.R32, 3, 3, 3);
    MyMatMatMult(Rotations.R4, Rotations.R3, Rotations.R43, 3, 3, 3);

    makeDiffRotation(Rotations.dR21, Q.q4, Q.q4r, 3);
    makeDiffRotation(Rotations.dR32, Q.q5, Q.q5r, 2);
    makeDiffRotation(Rotations.dR43, Q.q6, Q.q6r, 2);

    Rotations.TR21 = MatTran(Rotations.R21);
    Rotations.TR32 = MatTran(Rotations.R32);
    Rotations.TR43 = MatTran(Rotations.R43);

    Rotations.TR = MatTran(Rotations.R);

    Rotations.dTR21 = MatTran(Rotations.dR21);
    Rotations.dTR32 = MatTran(Rotations.dR32);
    Rotations.dTR43 = MatTran(Rotations.dR43);

    //-----------------------------------------------------------
    // R-dotted calculations
    TempR = zeroMatrix(3, 3);
    TempR[0][1] = -Q.q3r;
    TempR[0][2] = Q.q2r;
    TempR[1][0] = Q.q3r;
    TempR[1][2] = -Q.q1r;
    TempR[2][0] = -Q.q2r;
    TempR[2][1] = Q.q1r;
    MyMatMatMult(Rotations.Rdot, Rotations.R, TempR, 3, 3, 3);
    //MyMatMatSub(Rotations.Rdot, Rotations.R, Rotations.Rp, 3, 3)
    //MyMatByCons(Rotations.Rdot, Rotations.Rdot, 1 / dt, 3, 3)

    MyMatMatMult(Rotations.dR2_1, Rotations.R, Rotations.dR21, 3, 3, 3);
    MyMatMatMult(Rotations.dR2_2, Rotations.Rdot, Rotations.R21, 3, 3, 3);
    MyMatMatAdd(Rotations.dR2, Rotations.dR2_1, Rotations.dR2_2, 3, 3);

    MyMatMatMult(Rotations.dR3_1, Rotations.R2, Rotations.dR32, 3, 3, 3);
    MyMatMatMult(Rotations.dR3_2, Rotations.dR2, Rotations.R32, 3, 3, 3);
    MyMatMatAdd(Rotations.dR3, Rotations.dR3_1, Rotations.dR3_2, 3, 3);

    MyMatMatMult(Rotations.dR4_1, Rotations.R3, Rotations.dR43, 3, 3, 3);
    MyMatMatMult(Rotations.dR4_2, Rotations.dR3, Rotations.R43, 3, 3, 3);
    MyMatMatAdd(Rotations.dR4, Rotations.dR4_1, Rotations.dR4_2, 3, 3);
    ///-----------------------------------
    //Rotations.dTR43[0][0] = 0;
    //var vec=[0,1,0];

    //NyttNavn=vec;


    //document.write("<h1></h1>");
    //document.getElementById("Text2").innerHTML=Rotations.TR21;


    alert("Bef get_D in testingD");

    var count = 2;
    get_Bndot(Bndot, Q, Rotations)
    get_Bn(Bn, Q, Rotations)
    get_D(D, Q, Rotations)
    MatTranspose(BnT, Bn, numElemProp, numGenCoord);

    MyMatMatMult(MBn, M, Bn, numElemProp, numGenCoord, numElemProp);

    MyMatMatMult(Mstar, BnT, MBn, numGenCoord, numGenCoord, numElemProp);

    MyMatMatMult(MBndot, M, Bndot, numElemProp, numGenCoord, numElemProp);



    MyMatMatMult(DM, D, M, numElemProp, numElemProp, numElemProp);
    MyMatMatMult(DMB, DM, Bn, numElemProp, numGenCoord, numElemProp);

    for (var k1 = 0; k1 < numElemProp; k1++) {
        for (var k2 = 0; k2 < numGenCoord; k2++) {
            MBndotmDMB[k1][k2] = MBndot[k1][k2] + DMB[k1][k2];
        }
    }

    MyMatMatMult(Nstar, BnT, MBndotmDMB, numGenCoord, numGenCoord, numElemProp);

    var qn = [Q.q1r, Q.q2r, Q.q3r, Q.q4r, Q.q5r, Q.q6r];
    MyMatVecMult(NStarqn, Nstar, qn, numGenCoord, numGenCoord);
    MyVecVecSub(FmNstarqn, Fstar, NStarqn, numGenCoord);
    MstarI = inverse(Mstar);
    MyMatVecMult(Acc, MstarI, FmNstarqn, numGenCoord, numGenCoord);

    for (var k = 0; k < 3; k++) {
        for (var l = 0; l < 3; l++) {
            document.write(Rotations.dR4[k][l] + "&emsp;");
        }
        document.write("<br/>");
    }

    document.write("<br/>")
    document.write("<br/>")
    var j = 0;
    for (i = 0; i < numGenCoord; i++) {
        for (j = 0; j < numGenCoord; j++) {
            document.write(Nstar[i][j] + "&emsp;");

        }
        document.write("<br/>");
        if (count == i) {
            count += 3;
            document.write("<br/>");
        }
    }
    for (i = 0; i < numGenCoord; i++) {

        document.write(Acc[i] + "&emsp;");

    }



    alert("wow");

}