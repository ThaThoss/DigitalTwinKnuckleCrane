function getForce(F, time, M, numElemProp, numGenCoord) {


    var Bn = zeroMatrix(numElemProp, numGenCoord);
    var Bndot = zeroMatrix(numElemProp, numGenCoord);
    var D = zeroMatrix(numElemProp, numElemProp);



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

    forceAngle(Q, time)

    var Rotations = {

        R: MyEye(3, 3),
        TR: zeroMatrix(3, 3),
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

    Rotations.R = expMRodriguez(Q.q1r, Q.q2r, Q.q3r, time, 1);


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

    Rotations.dTR21 = MatTran(Rotations.dR21);
    Rotations.dTR32 = MatTran(Rotations.dR32);
    Rotations.dTR43 = MatTran(Rotations.dR43);

    //-----------------------------------------------------------


    var sw1 = zeroMatrix(3, 3)

    sw1 = SkewMat([Q.q1r, Q.q2r, Q.q3r])

    Rotations.Rdot = MatMatMult(Rotations.R, sw1)





    //-------------------------------------------------------
    // R-dotted calculations

    MyMatMatMult(Rotations.dR2_1, Rotations.R, Rotations.dR21, 3, 3, 3);
    MyMatMatMult(Rotations.dR2_2, Rotations.Rdot, Rotations.R21, 3, 3, 3);
    MyMatMatAdd(Rotations.dR2, Rotations.dR2_1, Rotations.dR2_2, 3, 3);

    MyMatMatMult(Rotations.dR3_1, Rotations.R2, Rotations.dR32, 3, 3, 3);
    MyMatMatMult(Rotations.dR3_2, Rotations.dR2, Rotations.R32, 3, 3, 3);
    MyMatMatAdd(Rotations.dR3, Rotations.dR3_1, Rotations.dR3_2, 3, 3);

    MyMatMatMult(Rotations.dR4_1, Rotations.R3, Rotations.dR43, 3, 3, 3);
    MyMatMatMult(Rotations.dR4_2, Rotations.dR3, Rotations.R43, 3, 3, 3);
    MyMatMatAdd(Rotations.dR4, Rotations.dR4_1, Rotations.dR4_2, 3, 3);


    var Xdot = zeroVector(numElemProp);
    var Xddot = zeroVector(numElemProp);
    var MXdot = zeroVector(numElemProp);
    var BdiffQdot = zeroVector(numElemProp);
    var MXddot = zeroVector(numElemProp);
    var BnQddot = zeroVector(numElemProp);
    var DMXdot = zeroVector(numElemProp);
    var MXddotDMXdot = zeroVector(numElemProp);
    var Fknown = zeroVector(numElemProp);
    var g = 9.81;



    //F[4]=-myPhysics.m2*g;

    //F[10]=-myPhysics.m3*g;

    //F[16]=myPhysics.m4*g;




    var Qvel = [Q.q1r, Q.q2r, Q.q3r, Q.q4r, Q.q5r, Q.q6r];
    var Qacc = [Q.q1rr, Q.q2rr, Q.q3rr, Q.q4rr, Q.q5rr, Q.q6rr];




    get_Bn(Bn, Q, Rotations);

    get_Bndot(Bndot, Q, Rotations);

    get_D(D, Q, Rotations);



    MyMatVecMult(Xdot, Bn, Qvel, numElemProp, numGenCoord)



    MyMatVecMult(BdiffQdot, Bndot, Qvel, numElemProp, numGenCoord)

    MyMatVecMult(BnQddot, Bn, Qacc, numElemProp, numGenCoord)

    MyVecVecAdd(Xddot, BnQddot, BdiffQdot, numElemProp)

    MyMatVecMult(MXddot, M, Xddot, numElemProp, numElemProp)

    MyMatVecMult(MXdot, M, Xdot, numElemProp, numElemProp)
    MyMatVecMult(DMXdot, D, MXdot, numElemProp, numElemProp)

    MyVecVecAdd(F, MXddot, DMXdot, numElemProp)
        //MyVecVecSub(F,MXddotDMXdot,Fknown,numElemProp) 


    //MyVectorPrinter(F, numElemProp)

    /*
    	
    	


    	var iangle2 = Math.floor(time/period2)

    		if(iangle2 == 0) {
    			T2 = M2
    		} else {

    			var xangle2 = Math.floor((time - period2)/(3*period2))
    			var yangle2 = xangle2%2
    	
    			if(yangle2 == 0) {
    				T2 = -M2
    			}
    			if(yangle2 == 1) {
    				T2 = M2
    			}
    		}
    		
    		F[6] = -T2 
    		F[12] =  T2 


    	var iangle3 = Math.floor(time/period3)

    		if(iangle3 == 0) {
    			T3 = M3
    		} else {

    			var xangle3 = Math.floor((time - period3)/(3*period3))
    			var yangle3 = xangle3%2
    	
    			if(yangle3 == 0) {
    				T3 = -M3
    			}
    			if(yangle3 == 1) {
    				T3 = M3
    			}
    		}

    		F[1] = -T3 
    		F[7] =  T3 

    */
}

function GetAngles() {

    var time = 0;

    var Q = {
        q1: 0,
        q2: 0,
        q3: 0,
        q4: 0,
        q5: 0,
        q6: 0,

        q1r: 0,
        q2r: 0,
        q3r: 0,
        q4r: 0,
        q5r: 0,
        q6r: 0,

        q1rr: 0,
        q2rr: 0,
        q3rr: 0,
        q4rr: 0,
        q5rr: 0,
        q6rr: 0

    }


    for (var i = 0; time < myTime.tend; i++) {

        forceAngle(Q, time)
        myGeneralizedData.w1[i] = 0;
        myGeneralizedData.w2[i] = 0;
        myGeneralizedData.w3[i] = 0;

        myGeneralizedData.q4[i] = Q.q4;
        myGeneralizedData.q5[i] = Q.q5;
        myGeneralizedData.q6[i] = Q.q6;

        myGeneralizedData.time[i] = time;

        time = time + myTime.dt;
    }
    myTime.nstop = i;
}

function forceAngle(Q, time) {

    // tower
    // bevegelse 

    if (myMotorControls.FACTOR4 == 1) {

        Q.q4 = -0.0491 * Math.pow(time, 2) + 0.4839 * time;
        Q.q5 = -0.0025 * Math.pow(time, 2) + 0.0299 * time;
        Q.q6 = -0.0064 * Math.pow(time, 2) + 0.3362 * time;

        Q.q4r = -0.0982 * time + 0.4839;
        Q.q5r = -0.0005 * time + 0.0299;
        Q.q6r = -0.0128 * time + 0.3362;

        Q.q4rr = -0.0982;
        Q.q5rr = -0.0005;
        Q.q6rr = -0.0128;

    } else if (myMotorControls.FACTOR1 == 1) {

        Q.q4 = -0.0491 * Math.pow(time, 2) + 0.4839 * time;

        Q.q4r = -0.0982 * time + 0.4839;

        Q.q4rr = -0.0982;

    } else if (myMotorControls.FACTOR2 == 1) {

        Q.q5 = -0.0449 * Math.pow(time, 2) + 0.5386 * time;

        Q.q5r = -0.0898 * time + 0.5386;

        Q.q5rr = -0.0898;

    } else if (myMotorControls.FACTOR3 == 1) {

        Q.q6 = -0.0064 * Math.pow(time, 2) + 0.3362 * time;

        Q.q6r = -0.0128 * time + 0.3362;

        Q.q6rr = -0.0128;

    }
}


function get_Gravity() {

    var g = -9.81
    var F = zeroVector(myArrayData.numElemProp)

    F[4] = myPhysics.m2 * g;

    F[10] = myPhysics.m3 * g;

    F[16] = myPhysics.m4 * g;
    return F;
}

function get_Position_Error(Rotations) {

    var globPosInert = [myPhysics.x, myPhysics.y, myPhysics.z];
    var j2fromc1 = [myPhysics.b2, myPhysics.h21, myPhysics.L21];
    var j3fromj2 = [0, 6.017, 0];
    var j4fromj3 = [0, 0, -22.0];
    var Tipfromj4 = [0, -5.05, 9.11];

    var InertPos = zeroVector(3);
    var tempV = zeroVector(3);

    tempV = MatVecMult(Rotations.R4, Tipfromj4);
    InertPos = VecVecAdd(InertPos, tempV);

    tempV = MatVecMult(Rotations.R3, j4fromj3);
    InertPos = VecVecAdd(InertPos, tempV);

    tempV = MatVecMult(Rotations.R2, j3fromj2);
    InertPos = VecVecAdd(InertPos, tempV);

    tempV = MatVecMult(Rotations.R, j2fromc1);
    InertPos = VecVecAdd(InertPos, tempV);

    tempV = VecVecSub(globPosInert, InertPos);

    var PosError = Math.sqrt(tempV[0] * tempV[0] + tempV[1] * tempV[1] + tempV[2] * tempV[2]);
    return PosError;
}


function get_angelspos(Rotations, Q, Mstar, Nstar, F, Fstar, time) {


    var numGenCoord = 6

    var dt = myTime.dt * 10;
    var th3w = -Math.sin(time / 8) * Math.PI / 6; //Math.sin(time / 3) * Math.PI / 24;
    var th1w = 0; //Math.sin(time / 3) * Math.PI / 30;
    var th2w = 0;
    var th3p = 2 * (th3w - Q.q3 - Q.q3r * dt) / (dt * dt)
    var th2p = 2 * (th2w - Q.q2 - Q.q2r * dt) / (dt * dt);
    var th1p = 2 * (th1w - Q.q1 - Q.q1r * dt) / (dt * dt);
    var Sj2 = [myPhysics.b2, myPhysics.h21 + 6.017, myPhysics.L21]

    var P = zeroVector(3)
    var Pt = zeroVector(3)

    var Posxp = [myPhysics.x, myPhysics.y, myPhysics.z]


    P = MatVecMult(Rotations.TR, Posxp)


    Pt = VecVecSub(P, Sj2)
        //MyVectorPrinter(Posxp,3)

    var x = Pt[0]
    var y = Pt[1]
    var z = Pt[2]

    var xn = Math.sqrt(Math.pow(x, 2) + Math.pow(z, 2))


    var th4w = Math.asin(-x / xn)


    var c = Math.sqrt(Math.pow(xn, 2) + Math.pow(y, 2))

    var th5w1 = Math.asin(y / c)

    if (c > 32) {
        c = 32;

    } else if (c < 11.6) {
        c = 11.6;
    }
    var th5w2 = Math.acos((Math.pow(22, 2) + Math.pow(c, 2) - Math.pow(10.4, 2)) / (2 * 22 * c))


    var th5w = th5w1 + th5w2


    //var th6w1 = Math.acos(5.05/(Math.sqrt(Math.pow(5.05,2)+Math.pow(9.11,2))))

    var th6w1 = 0.5048845334502091
        /*
            var A = (Math.pow(22, 2) + Math.pow(10.4, 2) - Math.pow(c, 2)) / (2 * 10.4 * 22)
            var B = y / c;
            if (A < -1 || A > 1 || B > 1 || B < -1) {
                alert("A Bugg {0}");
                alert(A);
                alert(c)
            }
            */
    var th6w2 = Math.acos((Math.pow(22, 2) + Math.pow(10.4, 2) - Math.pow(c, 2)) / (2 * 10.4 * 22))
        //alert(th6w2)
    var th6w = th6w2 - th6w1


    //var A = Math.acos((Math.pow(10.4,2)+Math.pow(c,2)-Math.pow(22,2))/(2*10.4*c))

    //var summen = A+th5w2+th6w2
    /*
        th4w = Math.PI / 6;
        th5w = 0;
        th6w = 0;
        var Angles = [Q.q4, th4w, Q.q5, th5w, Q.q6, th6w]
            //MyVectorPrinter(Angles, 6)
    */
    var th4p = 2 * (th4w - Q.q4 - (Q.q4r * dt)) / (dt * dt)
    var th5p = 2 * (th5w - Q.q5 - (Q.q5r * dt)) / (dt * dt)
    var th6p = 2 * (th6w - Q.q6 - (Q.q6r * dt)) / (dt * dt)

    var Vel = [Q.q1r, Q.q2r, Q.q3r, Q.q4r, Q.q5r, Q.q6r]
    var Acc = [th1p, th2p, th3p, th4p, th5p, th6p]


    //alert(th5w)
    //alert(Q.q5)

    //MyVectorPrinter(Acc, 6)
    //MyVectorPrinter(Vel, 6)

    var matvec1_NstarVel = zeroVector(numGenCoord);

    MyMatVecMult(matvec1_NstarVel, Nstar, Vel, numGenCoord, numGenCoord);

    var matvec1_MstarAcc = zeroVector(numGenCoord);

    MyMatVecMult(matvec1_MstarAcc, Mstar, Acc, numGenCoord, numGenCoord);

    var Fstarn = zeroVector(numGenCoord)

    var vecvec1_Fstarn = zeroVector(numGenCoord)

    MyVecVecAdd(vecvec1_Fstarn, matvec1_NstarVel, matvec1_MstarAcc, numGenCoord)

    MyVecVecSub(Fstarn, vecvec1_Fstarn, Fstar, numGenCoord)
        //  Fstar = Fstarn;

    var TempFV = MatVecMult(Rotations.R21, [Fstarn[4], Fstarn[3], 0]);
    F[0] = Fstarn[0] - TempFV[0];
    F[1] = Fstarn[1] - Fstarn[3];
    F[2] = Fstarn[2] - TempFV[2];

    F[7] = Fstarn[3];
    F[12] = Fstarn[4] - Fstarn[5];
    F[18] = Fstarn[5];

    //MyVectorPrinter(F, myArrayData.numElemProp)

}