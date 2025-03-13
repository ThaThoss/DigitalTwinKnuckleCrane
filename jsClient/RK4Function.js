
var k1=zeroVector(numGenCoord);
var k2=zeroVector(numGenCoord);

var Sol=zeroVector(numGenCoord);
var SolV=zeroVector(numGenCoord);

var Ang=zeroVector(numGenCoord);
var AngV=zeroVector(numGenCoord);

var TempAng=zeroVector(numGenCoord);
var TempAngV=zeroVector(numGenCoord);

var TempSol=zeroVector(numGenCoord);
var TempSolV=zeroVector(numGenCoord);

MyCopyVector(Sol, Q.Angle,numGenCoord);
MyCopyVector(SolV, Q.angleVel,numGenCoord);


for (var i=0;  i < nstop; i++) {

//Integration, RK4:
Ang[0]=Q.q1;
Ang[1]=Q.q2;
Ang[2]=Q.q3;
Ang[3]=Q.q4;
Ang[4]=Q.q5;
Ang[5]=Q.q6;
Ang[6]=Q.q6;

AngV[0]=Q.q1r;
AngV[1]=Q.q2r;
AngV[2]=Q.q3r;
AngV[3]=Q.q4r;
AngV[4]=Q.q5r;
AngV[5]=Q.q6r;
AngV[6]=Q.q7r;



for(k=0;k<4;k++){

//Prepare Q for next RK4 step
TempAngV=MyVecByCons(k1,a[k][k],numGenCoord);

Q.q1r=AngV[0]+TempAngV[0];
Q.q2r=AngV[1]+TempAngV[1];
Q.q3r=AngV[2]+TempAngV[2];
Q.q4r=AngV[3]+TempAngV[3];

TempAng=MyVecByCons(k2,a[k][k],numGenCoord);

Q.q1=Ang[0]+TempAng[0];
Q.q2=Ang[1]+TempAng[1];
Q.q3=Ang[2]+TempAng[2];
Q.q4=Ang[3]+TempAng[3];


//Get Solution point "k"
var TIME=time+c[k]*dt;

k1=get_Vel(Bn,BnT,D,Bdiff,M,F,MBn,MBndot,Mstar,Nstar,Fstar,DM,DMB,MBndotmDMB,NStarqn,FmNstarqn,MstarI,Rotations,Mass,Q,TIME,dt,numGenCoord,numElemProp);
	
k2[0]=Q.q1r*dt;
k2[1]=Q.q2r*dt;
k2[2]=Q.q3r*dt;
k2[3]=Q.q4r*dt;


//Save solution
TempSol=MyVecByCons( k2,b[k],numGenCoord);
MyVecVecAdd(Sol,Sol,TempSol,numGenCoord);

TempSolV=MyVecByCons( k1,b[k],numGenCoord);
MyVecVecAdd(SolV,SolV,TempSolV,numGenCoord);
}


//Update	

Q.q1r=SolV[0];
Q.q2r=SolV[1];
Q.q3r=SolV[2];
Q.q4r=SolV[3];

Q.q1=Sol[0];
Q.q2=Sol[1];
Q.q3=Sol[2];
Q.q4=Sol[3];

 		

		myGeneralizedData.time[i]  = time;

		myGeneralizedData.w1=SolV[0];
		myGeneralizedData.w2=SolV[1];
		myGeneralizedData.w3=SolV[2];
		
		myGeneralizedData.q4=Sol[3];
		myGeneralizedData.q5=Sol[4];
		myGeneralizedData.q6=Sol[5];
		myGeneralizedData.q7=Sol[6];
		
time = time + dt;
}//End integration	
