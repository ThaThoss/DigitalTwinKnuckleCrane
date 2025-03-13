function physics() {

    var dt = myTime.dt;
    var tend = myTime.tend;

    //Integration constants for RK4
    var a = zeroMatrix(4, 4);
    a[1][1] = 0.5;
    a[2][2] = 0.5;
    a[3][3] = 1;
    var b = [1 / 6, 1 / 3, 1 / 3, 1 / 6];
    var c = [0, 0.5, 0.5, 1];
    //-----------------------------

    //-------------------------------------

    var m1 = myPhysics.m1;
    var m2 = myPhysics.m2;
    var m3 = myPhysics.m3;
    var m4 = myPhysics.m4;

    // added from here

    var b2 = myPhysics.b2;
    var L21 = myPhysics.L21;
    var h21 = myPhysics.h21;
    var L22 = myPhysics.L22;
    var h22 = myPhysics.h22;
    var L31 = myPhysics.L31;
    var h31 = myPhysics.h31;
    var L32 = myPhysics.L32;
    var h32 = myPhysics.h32;
    var L41 = myPhysics.L41;
    var h41 = myPhysics.h41;
    var L42 = myPhysics.L42;
    var h42 = myPhysics.h42;

    // to here
    /*
    	var Lps = myPhysics.Lps;
    	var Lsb = myPhysics.Lsb;
    	var Lh = myPhysics.Lh;
    */
    var damp = myPhysics.damp;
    var numLinks = myPhysics.numLinks;

    //---------------------------------------------------


    var amp1 = -1;
    var amp2 = -1;


    var spatialDim = myArrayData.spatialDim;
    var numGenCoord = myArrayData.numGenCoord;
    var numLocalSpace = myArrayData.numLocalSpace;

    //-------------------------------------------------------

    var numElemProp = myArrayData.numElemProp; // changed from 18

    // Find J-matrix in CREO, need help

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

    //--------------------------------------------------------

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

    //----------------------------------------------------------


    var M = zeroMatrix(numElemProp, numElemProp);
    var D = zeroMatrix(numElemProp, numElemProp);
    var DM = zeroMatrix(numElemProp, numElemProp);
    var DMB = zeroMatrix(numElemProp, numGenCoord);
    var MBndotmDMB = zeroMatrix(numElemProp, numGenCoord);
    var F = zeroVector(numElemProp);
    var Fstar = zeroVector(numGenCoord);
    var MBn = zeroMatrix(numElemProp, numGenCoord);
    var MBndot = zeroMatrix(numElemProp, numGenCoord);
    var BnT = zeroMatrix(numGenCoord, numElemProp);
    var MstarIFmNstarqn = zeroVector(numGenCoord);
    var NStarqn = zeroVector(numGenCoord);
    var FmNstarqn = zeroVector(numGenCoord);
    var Bn = zeroMatrix(numElemProp, numGenCoord);
    var Bndot = zeroMatrix(numElemProp, numGenCoord);
    var Mstar = zeroMatrix(numGenCoord, numGenCoord);
    var Nstar = zeroMatrix(numGenCoord, numGenCoord);
    var MstarI = zeroMatrix(numGenCoord, numGenCoord);
    var TempR = zeroMatrix(3, 3);
    var TempR2 = MyEye(3, 3);
    //------------------------------------------------------------------

    var Q = {
        q1r: 0,
        q2r: 0,
        q3r: 0,
        q4r: 0,
        q5r: 0,
        q6r: 0,

        q1: 0,
        q2: 0,
        q3: 0,
        q4: 0,
        q5: 0,
        q6: 0,

        q1rr: 0,
        q2rr: 0,
        q3rr: 0,
        q4rr: 0,
        q5rr: 0,
        q6rr: 0

    }

    //------------------------------------------------------------------
    // Mass-matrix (fixed)

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

    //----------------------------------------------------------

    //----------------------------------------------------------

    var nstop = myTime.nstop;


    var time = 0;



    //----------------------------------
    // here was Q before
    //-------------------------------

    var k1 = zeroVector(numGenCoord);
    var k2 = zeroVector(numGenCoord);

    var Sol = zeroVector(numGenCoord);
    var SolV = zeroVector(numGenCoord);

    var Ang = zeroVector(numGenCoord);
    var AngV = zeroVector(numGenCoord);

    var TempAng = zeroVector(numGenCoord);
    var TempAngV = zeroVector(numGenCoord);

    var TempSol = zeroVector(numGenCoord);
    var TempSolV = zeroVector(numGenCoord);

    if (myMotorControls.FACTOR4 == 1) {
        Q.q4r = -0.0982 * time + 0.4839;
        Q.q5r = -0.0005 * time + 0.0299;
        Q.q6r = -0.0128 * time + 0.3362;

        SolV[3] = Q.q4r
        SolV[4] = Q.q5r
        SolV[5] = Q.q6r

        tend = 11;

    } else if (myMotorControls.FACTOR1 == 1) {

        Q.q4r = -0.0982 * time + 0.4839;
        SolV[3] = Q.q4r
        tend = 11;

    } else if (myMotorControls.FACTOR2 == 1) {

        Q.q5r = -0.0898 * time + 0.5386;

        SolV[4] = Q.q5r
        tend = 11;

    } else if (myMotorControls.FACTOR3 == 1) {

        Q.q6r = -0.0128 * time + 0.3362;

        SolV[5] = Q.q6r
        tend = 11;

    } else if (myMotorControls.FACTOR5 == 1) {


        Q.q4 = myMotorControls.TowerPos;
        Q.q5 = myMotorControls.MainboomPos;
        Q.q6 = myMotorControls.OuterboomPos;


        makeRotation(Rotations.R21, Q.q4, 2);
        makeRotation(Rotations.R32, Q.q5, 1);
        makeRotation(Rotations.R43, Q.q6, 1);


        var Posxp = get_Posxp(Q, Rotations)

        myPhysics.x = Posxp[0]
        myPhysics.y = Posxp[1]
        myPhysics.z = Posxp[2]



        Sol[3] = Q.q4
        Sol[4] = Q.q5
        Sol[5] = Q.q6



        //Q.q3r =Math.cos(time)*Math.PI/(18*3);

        //Q.q2r =Math.cos(time)*Math.PI/(20*3);	

        //SolV[2] = Q.q3r
        //SolV[3] = -Q.q2r
    }


    for (var i = 0; time < tend; i++) {
        /*
document.write("SolV Bef"+"&emsp;");
MyVectorPrinter(SolV,numGenCoord);
*/
        //Integration, RK4:
        Ang[0] = Q.q1;
        Ang[1] = Q.q2;
        Ang[2] = Q.q3;
        Ang[3] = Q.q4;
        Ang[4] = Q.q5;
        Ang[5] = Q.q6;

        AngV[0] = Q.q1r;
        AngV[1] = Q.q2r;
        AngV[2] = Q.q3r;
        AngV[3] = Q.q4r;
        AngV[4] = Q.q5r;
        AngV[5] = Q.q6r;


        for (var k = 0; k < 4; k++) {

            //Prepare Q for next RK4 step
            TempAngV = MyVecByCons(k1, a[k][k], numGenCoord);

            Q.q1r = AngV[0] + TempAngV[0];
            Q.q2r = AngV[1] + TempAngV[1];
            Q.q3r = AngV[2] + TempAngV[2];
            Q.q4r = AngV[3] + TempAngV[3];
            Q.q5r = AngV[4] + TempAngV[4];
            Q.q6r = AngV[5] + TempAngV[5];

            TempAng = MyVecByCons(k2, a[k][k], numGenCoord);

            Q.q1 = Ang[0] + TempAng[0];
            Q.q2 = Ang[1] + TempAng[1];
            Q.q3 = Ang[2] + TempAng[2];
            Q.q4 = Ang[3] + TempAng[3];
            Q.q5 = Ang[4] + TempAng[4];
            Q.q6 = Ang[5] + TempAng[5];
            //---------------------------------------------------------------------



            //------------------------------------------------------------
            // All the Rotations are located here

            TempR = MatMatMult(TempR2, expMRodriguez(Q.q1r, Q.q2r, Q.q3r, dt, 1));
            CopyMatrix(TempR, Rotations.R);
            //Rotations.R = expMRodriguez(Q.q1r, Q.q2r, Q.q3r, time, 1);

            //MyVectorPrinter([w1,w2,w3],3);
            //MyMatrixPrinter(Rotations.R,3,3);
            makeRotation(Rotations.R21, Q.q4, 2);
            makeRotation(Rotations.R32, Q.q5, 1);
            makeRotation(Rotations.R43, Q.q6, 1);

            MyMatMatMult(Rotations.R2, Rotations.R, Rotations.R21, 3, 3, 3);
            MyMatMatMult(Rotations.R3, Rotations.R2, Rotations.R32, 3, 3, 3);
            MyMatMatMult(Rotations.R4, Rotations.R3, Rotations.R43, 3, 3, 3);

            makeDiffRotation(Rotations.dR21, Q.q4, Q.q4r, 2);
            makeDiffRotation(Rotations.dR32, Q.q5, Q.q5r, 1);
            makeDiffRotation(Rotations.dR43, Q.q6, Q.q6r, 1);

            Rotations.TR21 = MatTran(Rotations.R21);
            Rotations.TR32 = MatTran(Rotations.R32);
            Rotations.TR43 = MatTran(Rotations.R43);

            Rotations.TR = MatTran(Rotations.R);

            Rotations.dTR21 = MatTran(Rotations.dR21);
            Rotations.dTR32 = MatTran(Rotations.dR32);
            Rotations.dTR43 = MatTran(Rotations.dR43);

            //-----------------------------------------------------------
            // R-dotted calculations
            MyMatMatSub(Rotations.Rdot, Rotations.R, Rotations.Rp, 3, 3)
            MyMatByCons(Rotations.Rdot, Rotations.Rdot, 1 / dt, 3, 3)

            MyMatMatMult(Rotations.dR2_1, Rotations.R, Rotations.dR21, 3, 3, 3);
            MyMatMatMult(Rotations.dR2_2, Rotations.Rdot, Rotations.R21, 3, 3, 3);
            MyMatMatAdd(Rotations.dR2, Rotations.dR2_1, Rotations.dR2_2, 3, 3);

            MyMatMatMult(Rotations.dR3_1, Rotations.R2, Rotations.dR32, 3, 3, 3);
            MyMatMatMult(Rotations.dR3_2, Rotations.dR2, Rotations.R32, 3, 3, 3);
            MyMatMatAdd(Rotations.dR3, Rotations.dR3_1, Rotations.dR3_2, 3, 3);

            MyMatMatMult(Rotations.dR4_1, Rotations.R3, Rotations.dR43, 3, 3, 3);
            MyMatMatMult(Rotations.dR4_2, Rotations.dR3, Rotations.R43, 3, 3, 3);
            MyMatMatAdd(Rotations.dR4, Rotations.dR4_1, Rotations.dR4_2, 3, 3);
            // ------------------------------------------------------------
            //Get Solution point "k"
            // Need TIME for get forces
            var TIME = time + c[k] * dt;

            k1 = get_Vel(Bn, BnT, D, Bndot, M, F, MBn, MBndot, Mstar, Nstar, Fstar, DM, DMB, MBndotmDMB, NStarqn, FmNstarqn, MstarI, Rotations, Q, TIME, dt, numGenCoord, numElemProp);

            k2[0] = Q.q1r * dt;
            k2[1] = Q.q2r * dt;
            k2[2] = Q.q3r * dt;
            k2[3] = Q.q4r * dt;
            k2[4] = Q.q5r * dt;
            k2[5] = Q.q6r * dt;

            //MyVectorPrinter(k1,numGenCoord )


            //Save solution
            TempSol = MyVecByCons(k2, b[k], numGenCoord);
            MyVecVecAdd(Sol, Sol, TempSol, numGenCoord);

            TempSolV = MyVecByCons(k1, b[k], numGenCoord);
            MyVecVecAdd(SolV, SolV, TempSolV, numGenCoord);

        }
        /*
        document.write("SolV Aft"+"&emsp;");
        MyVectorPrinter(SolV,numGenCoord);
        */
        //Update	

        Q.q1r = SolV[0];
        Q.q2r = SolV[1];
        Q.q3r = SolV[2];
        Q.q4r = SolV[3];
        Q.q5r = SolV[4];
        Q.q6r = SolV[5];


        Q.q1 = Sol[0];
        Q.q2 = Sol[1];
        Q.q3 = Sol[2];
        Q.q4 = Sol[3];
        Q.q5 = Sol[4];
        Q.q6 = Sol[5];

        //MyVectorPrinter(SolV,numGenCoord);
        TempR = MatMatMult(TempR2, expMRodriguez(Q.q1r, Q.q2r, Q.q3r, dt, 1));
        CopyMatrix(TempR, Rotations.R);
        CopyMatrix(Rotations.R, TempR2);

        makeRotation(Rotations.R21, Q.q4, 2);
        makeRotation(Rotations.R32, Q.q5, 1);
        makeRotation(Rotations.R43, Q.q6, 1);

        MyMatMatMult(Rotations.R2, Rotations.R, Rotations.R21, 3, 3, 3);
        MyMatMatMult(Rotations.R3, Rotations.R2, Rotations.R32, 3, 3, 3);
        MyMatMatMult(Rotations.R4, Rotations.R3, Rotations.R43, 3, 3, 3);


        myGeneralizedData.time[i] = time;

        myGeneralizedData.w1[i] = SolV[0];
        myGeneralizedData.w2[i] = SolV[1];
        myGeneralizedData.w3[i] = SolV[2];

        myGeneralizedData.q4[i] = Sol[3];
        myGeneralizedData.q5[i] = Sol[4];
        myGeneralizedData.q6[i] = Sol[5];

        var PosError = get_Position_Error(Rotations)
            //document.write(PosError + "&emsp;" + "<br/>");
        CopyMatrix(Rotations.R, Rotations.Rp);

        time = time + dt;

    }
    myTime.nstop = i
}