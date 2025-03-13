function get_Vel(Bn, BnT, D, Bndot, M, F, MBn, MBndot, Mstar, Nstar, Fstar, DM, DMB, MBndotmDMB, NStarqn, FmNstarqn, MstarI, Rotations, Q, TIME, dt, numGenCoord, numElemProp) {

    var Acc = zeroVector(numGenCoord);
    var Vel = zeroVector(numGenCoord);





    get_Bn(Bn, Q, Rotations);

    MatTranspose(BnT, Bn, numElemProp, numGenCoord);

    get_Bndot(Bndot, Q, Rotations);

    get_D(D, Q, Rotations);

    //var Qtemp=[Q.q1,Q.q2,Q.q3,Q.q4,Q.q5,Q.q6];
    //var Qrtemp=[Q.q1r,Q.q2r,Q.q3r,Q.q4r,Q.q5r,Q.q6r];

    //forceAngle(Q,Rotations,D,TIME)

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


    if (myMotorControls.FACTOR5 == 1) {
        Fstar = zeroVector(6);
        F = get_Gravity();

        MyMatVecMult(Fstar, BnT, F, numGenCoord, numElemProp);

        get_angelspos(Rotations, Q, Mstar, Nstar, F, Fstar, TIME);


    } else {

        getForce(F, TIME, M, numElemProp, numGenCoord);

        MyMatVecMult(Fstar, BnT, F, numGenCoord, numElemProp);

    }
    MyMatVecMult(Fstar, BnT, F, numGenCoord, numElemProp);

    /*
    Q.q1=Qtemp[0]
    Q.q2=Qtemp[1]
    Q.q3=Qtemp[2]
    Q.q4=Qtemp[3]
    Q.q5=Qtemp[4]
    Q.q6=Qtemp[5]

    Q.q1r=Qrtemp[0]
    Q.q2r=Qrtemp[1]
    Q.q3r=Qrtemp[2]
    Q.q4r=Qrtemp[3]
    Q.q5r=Qrtemp[4]
    Q.q6r=Qrtemp[5]

    */

    //MyVectorPrinter(F, numElemProp)

    //----------------------------------------------------------------------


    var qn = [Q.q1r, Q.q2r, Q.q3r, Q.q4r, Q.q5r, Q.q6r];
    MyMatVecMult(NStarqn, Nstar, qn, numGenCoord, numGenCoord);


    MyVecVecSub(FmNstarqn, Fstar, NStarqn, numGenCoord);


    MstarI = inverse(Mstar);



    //MyVectorPrinter(Fstar,numGenCoord)
    MyMatVecMult(Acc, MstarI, FmNstarqn, numGenCoord, numGenCoord);

    //MyMatrixPrinter(Bndot,numElemProp,numGenCoord)
    /*MyVectorPrinter(F,numElemProp)
    MyVectorPrinter(Fstar,numGenCoord)
    */
    Q.q1rr = Acc[0]
    Q.q2rr = Acc[1]
    Q.q3rr = Acc[2]
    Q.q4rr = Acc[3]
    Q.q5rr = Acc[4]
    Q.q6rr = Acc[5]

    Vel = MyVecByCons(Acc, dt, numGenCoord);
    //MyVectorPrinter(Fstar, numGenCoord)

    //var Accxp = get_Accxp(Q, Rotations, D)

    //MyVectorPrinter(Accxp, 3)



    return Vel;
}

function get_Accxp(Q, Rotations, D) {


    var s21 = [myPhysics.b2, myPhysics.h21, myPhysics.L21]
    var s32 = [0, 6.017, 0]
    var s43 = [0, 0, 22.000]
    var sp = [0, 5.27, 9]

    var dth2e3 = [0, Q.q4r, 0]
    var dth3e1 = [Q.q5r, 0, 0]
    var dth4e1 = [Q.q6r, 0, 0]

    var ddth2e3 = [0, Q.q4rr, 0]
    var ddth3e1 = [Q.q5rr, 0, 0]
    var ddth4e1 = [Q.q6rr, 0, 0]



    var w1 = [Q.q1r, Q.q2r, Q.q3r];

    var Sw1 = zeroMatrix(3, 3);
    Sw1 = SkewMat(w1);


    var dw1 = [Q.q1rr, Q.q2rr, Q.q3rr];

    var Sdw1 = zeroMatrix(3, 3);
    Sdw1 = SkewMat(dw1);



    var Sw2 = zeroMatrix(3, 3);
    var Sw3 = zeroMatrix(3, 3);
    var Sw4 = zeroMatrix(3, 3);

    Sw2[1][0] = D[7][6];
    Sw2[2][0] = D[8][6];
    Sw2[0][1] = D[6][7];
    Sw2[2][1] = D[8][7];
    Sw2[0][2] = D[6][8];
    Sw2[1][2] = D[7][8];

    Sw3[1][0] = D[13][12];
    Sw3[2][0] = D[14][12];
    Sw3[0][1] = D[12][13];
    Sw3[2][1] = D[14][13];
    Sw3[0][2] = D[12][14];
    Sw3[1][2] = D[13][14];

    Sw4[1][0] = D[19][18];
    Sw4[2][0] = D[20][18];
    Sw4[0][1] = D[18][19];
    Sw4[2][1] = D[20][19];
    Sw4[0][2] = D[18][20];
    Sw4[1][2] = D[19][20];




    //---------- dw2

    var matvec1_dw2 = zeroVector(3);
    var matvec2_dw2 = zeroVector(3);

    var vecvec1_dw2 = zeroVector(3)
    var dw2 = zeroVector(3);

    matvec1_dw2 = MatVecMult(Rotations.dTR21, w1);
    matvec2_dw2 = MatVecMult(Rotations.TR21, dw1);

    vecvec1_dw2 = VecVecAdd(matvec1_dw2, ddth2e3)
    dw2 = VecVecAdd(vecvec1_dw2, matvec2_dw2);

    //---------- dw3

    var matvec1_dw3 = zeroVector(3);
    var matvec2_dw3 = zeroVector(3);
    var matvec3_dw3 = zeroVector(3);
    var matvec4_dw3 = zeroVector(3);
    var matvec5_dw3 = zeroVector(3);
    var matvec6_dw3 = zeroVector(3);
    var matvec7_dw3 = zeroVector(3);
    var matvec8_dw3 = zeroVector(3);

    var vecvec1_dw3 = zeroVector(3);
    var vecvec2_dw3 = zeroVector(3);
    var vecvec3_dw3 = zeroVector(3);
    var vecvec4_dw3 = zeroVector(3);
    var dw3 = zeroVector(3);

    matvec1_dw3 = MatVecMult(Rotations.dTR32, dth2e3);

    matvec2_dw3 = MatVecMult(Rotations.TR32, ddth2e3);

    matvec3_dw3 = MatVecMult(Rotations.TR21, w1);
    matvec4_dw3 = MatVecMult(Rotations.dTR32, matvec3_dw3);

    matvec5_dw3 = MatVecMult(Rotations.dTR21, w1);
    matvec6_dw3 = MatVecMult(Rotations.TR32, matvec5_dw3);

    matvec7_dw3 = MatVecMult(Rotations.TR21, dw1);
    matvec8_dw3 = MatVecMult(Rotations.TR32, matvec7_dw3);

    vecvec1_dw3 = VecVecAdd(matvec1_dw3, ddth3e1);
    vecvec2_dw3 = VecVecAdd(matvec2_dw3, vecvec1_dw3);
    vecvec3_dw3 = VecVecAdd(matvec4_dw3, vecvec2_dw3);
    vecvec4_dw3 = VecVecAdd(matvec6_dw3, vecvec3_dw3);
    dw3 = VecVecAdd(matvec8_dw3, vecvec4_dw3);

    //----------- dw4


    var matvec1_dw4 = zeroVector(3);
    var matvec2_dw4 = zeroVector(3);
    var matvec3_dw4 = zeroVector(3);
    var matvec4_dw4 = zeroVector(3);
    var matvec5_dw4 = zeroVector(3);
    var matvec6_dw4 = zeroVector(3);
    var matvec7_dw4 = zeroVector(3);
    var matvec8_dw4 = zeroVector(3);
    var matvec9_dw4 = zeroVector(3);
    var matvec10_dw4 = zeroVector(3);
    var matvec11_dw4 = zeroVector(3);
    var matvec12_dw4 = zeroVector(3);
    var matvec13_dw4 = zeroVector(3);
    var matvec14_dw4 = zeroVector(3);
    var matvec15_dw4 = zeroVector(3);
    var matvec16_dw4 = zeroVector(3);
    var matvec17_dw4 = zeroVector(3);
    var matvec18_dw4 = zeroVector(3);
    var matvec19_dw4 = zeroVector(3);
    var matvec20_dw4 = zeroVector(3);

    var vecvec1_dw4 = zeroVector(3);
    var vecvec2_dw4 = zeroVector(3);
    var vecvec3_dw4 = zeroVector(3);
    var vecvec4_dw4 = zeroVector(3);
    var vecvec5_dw4 = zeroVector(3);
    var vecvec6_dw4 = zeroVector(3);
    var vecvec7_dw4 = zeroVector(3);
    var vecvec8_dw4 = zeroVector(3);
    var dw4 = zeroVector(3);

    matvec1_dw4 = MatVecMult(Rotations.dTR43, dth3e1);

    matvec2_dw4 = MatVecMult(Rotations.TR43, ddth3e1);

    matvec3_dw4 = MatVecMult(Rotations.TR32, dth2e3);
    matvec4_dw4 = MatVecMult(Rotations.dTR43, matvec3_dw4);

    matvec5_dw4 = MatVecMult(Rotations.dTR32, dth2e3);
    matvec6_dw4 = MatVecMult(Rotations.TR43, matvec5_dw4);

    matvec7_dw4 = MatVecMult(Rotations.TR32, ddth2e3);
    matvec8_dw4 = MatVecMult(Rotations.TR43, matvec7_dw4);

    matvec9_dw4 = MatVecMult(Rotations.TR21, w1);
    matvec10_dw4 = MatVecMult(Rotations.TR32, matvec9_dw4);
    matvec11_dw4 = MatVecMult(Rotations.dTR43, matvec10_dw4);

    matvec12_dw4 = MatVecMult(Rotations.TR21, w1);
    matvec13_dw4 = MatVecMult(Rotations.dTR32, matvec12_dw4);
    matvec14_dw4 = MatVecMult(Rotations.TR43, matvec13_dw4);

    matvec15_dw4 = MatVecMult(Rotations.dTR21, w1);
    matvec16_dw4 = MatVecMult(Rotations.TR32, matvec15_dw4);
    matvec17_dw4 = MatVecMult(Rotations.TR43, matvec16_dw4);

    matvec18_dw4 = MatVecMult(Rotations.TR21, dw1);
    matvec19_dw4 = MatVecMult(Rotations.TR32, matvec18_dw4);
    matvec20_dw4 = MatVecMult(Rotations.TR43, matvec19_dw4);

    vecvec1_dw4 = VecVecAdd(matvec1_dw4, ddth4e1);
    vecvec2_dw4 = VecVecAdd(matvec2_dw4, vecvec1_dw4);
    vecvec3_dw4 = VecVecAdd(matvec4_dw4, vecvec2_dw4);
    vecvec4_dw4 = VecVecAdd(matvec6_dw4, vecvec3_dw4);
    vecvec5_dw4 = VecVecAdd(matvec8_dw4, vecvec4_dw4);
    vecvec6_dw4 = VecVecAdd(matvec11_dw4, vecvec5_dw4);
    vecvec7_dw4 = VecVecAdd(matvec14_dw4, vecvec6_dw4);
    vecvec8_dw4 = VecVecAdd(matvec17_dw4, vecvec7_dw4);
    dw4 = VecVecAdd(matvec20_dw4, vecvec8_dw4);


    var Sdw2 = zeroMatrix(3, 3);
    var Sdw3 = zeroMatrix(3, 3);
    var Sdw4 = zeroMatrix(3, 3);

    Sdw2 = SkewMat(dw2);
    Sdw3 = SkewMat(dw3);
    Sdw4 = SkewMat(dw4);

    //------------------ Accxp = ddxp


    var e3toe1 = zeroMatrix(3, 3);
    var e4toe1 = zeroMatrix(3, 3);

    e3toe1 = MatMatMult(Rotations.R21, Rotations.R32);
    e4toe1 = MatMatMult(e3toe1, Rotations.R43);

    var matvec1_ddxp = zeroVector(3);
    var matvec2_ddxp = zeroVector(3);
    var matvec3_ddxp = zeroVector(3);
    var matvec4_ddxp = zeroVector(3);
    var matvec5_ddxp = zeroVector(3);
    var matvec6_ddxp = zeroVector(3);
    var matvec7_ddxp = zeroVector(3);
    var matvec8_ddxp = zeroVector(3);
    var matvec9_ddxp = zeroVector(3);
    var matvec10_ddxp = zeroVector(3);
    var matvec11_ddxp = zeroVector(3);
    var matvec12_ddxp = zeroVector(3);
    var matvec13_ddxp = zeroVector(3);
    var matvec14_ddxp = zeroVector(3);
    var matvec15_ddxp = zeroVector(3);
    var matvec16_ddxp = zeroVector(3);
    var matvec17_ddxp = zeroVector(3);
    var matvec18_ddxp = zeroVector(3);

    var vecvec1_ddxp = zeroVector(3);
    var vecvec2_ddxp = zeroVector(3);
    var vecvec3_ddxp = zeroVector(3);
    var vecvec4_ddxp = zeroVector(3);
    var vecvec5_ddxp = zeroVector(3);
    var vecvec6_ddxp = zeroVector(3);
    var Accxp = zeroVector(3);

    matvec1_ddxp = MatVecMult(Sdw4, s43);
    matvec2_ddxp = MatVecMult(e4toe1, matvec1_ddxp);

    matvec3_ddxp = MatVecMult(Sw4, s43);
    matvec4_ddxp = MatVecMult(Sw4, matvec3_ddxp);
    matvec5_ddxp = MatVecMult(e4toe1, matvec4_ddxp);

    matvec6_ddxp = MatVecMult(Sdw3, s32);
    matvec7_ddxp = MatVecMult(e3toe1, matvec6_ddxp);

    matvec8_ddxp = MatVecMult(Sw3, s32);
    matvec9_ddxp = MatVecMult(Sw3, matvec8_ddxp);
    matvec10_ddxp = MatVecMult(e3toe1, matvec9_ddxp);

    matvec11_ddxp = MatVecMult(Sdw2, s21);
    matvec12_ddxp = MatVecMult(Rotations.R21, matvec11_ddxp);

    matvec13_ddxp = MatVecMult(Sw2, s21);
    matvec14_ddxp = MatVecMult(Sw2, matvec13_ddxp);
    matvec15_ddxp = MatVecMult(Rotations.R21, matvec14_ddxp);

    matvec16_ddxp = MatVecMult(Sdw1, s21);

    matvec17_ddxp = MatVecMult(Sw1, s21);

    matvec18_ddxp = MatVecMult(Sw1, matvec17_ddxp);


    vecvec1_ddxp = VecVecAdd(matvec2_ddxp, matvec5_ddxp);
    vecvec2_ddxp = VecVecAdd(vecvec1_ddxp, matvec7_ddxp);
    vecvec3_ddxp = VecVecAdd(vecvec2_ddxp, matvec10_ddxp);
    vecvec4_ddxp = VecVecAdd(vecvec3_ddxp, matvec12_ddxp);
    vecvec5_ddxp = VecVecAdd(vecvec4_ddxp, matvec15_ddxp);
    vecvec6_ddxp = VecVecAdd(vecvec5_ddxp, matvec16_ddxp);
    Accxp = VecVecAdd(vecvec6_ddxp, matvec18_ddxp);

    return Accxp

}


function get_Posxp(Q, Rotations) {

    var s21 = [myPhysics.b2, myPhysics.h21, myPhysics.L21]
    var s32 = [0, 6.017, 0]
    var s43 = [0, 0, -22.000]
    var sp = [0, -5.05, 9.11]

    var e3toe1 = zeroMatrix(3, 3);
    var e4toe1 = zeroMatrix(3, 3);

    e3toe1 = MatMatMult(Rotations.R21, Rotations.R32);
    e4toe1 = MatMatMult(e3toe1, Rotations.R43);

    var matvec1_xp = zeroVector(3);
    var matvec2_xp = zeroVector(3);
    var matvec3_xp = zeroVector(3);

    var vecvec1_xp = zeroVector(3);
    var vecvec2_xp = zeroVector(3);
    var Posxp = zeroVector(3);

    matvec1_xp = MatVecMult(e4toe1, sp);

    matvec2_xp = MatVecMult(e3toe1, s43);

    matvec3_xp = MatVecMult(Rotations.R21, s32);

    vecvec1_xp = VecVecAdd(matvec1_xp, matvec2_xp);
    vecvec2_xp = VecVecAdd(vecvec1_xp, matvec3_xp);
    Posxp = VecVecAdd(vecvec2_xp, s21);
    //MyVectorPrinter(matvec1_xp, 3)


    return Posxp


}