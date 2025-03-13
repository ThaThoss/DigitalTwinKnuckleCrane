/*Structs to be used with Knuckle Boom crane Project*/

#ifndef MYDYNSTRUCTS
#define MYDYNSTRUCTS
struct Rotation{
 
  double R[3][3];

};


struct Frame{
 
  struct Rotation AbsR;
  struct Rotation locR;
  struct Rotation TlocR;
  struct Rotation dAbsR;
  struct Rotation dlocR;
  struct Rotation TdlocR;
  struct Rotation tempR1;
  struct Rotation tempR2; 

};


struct ArrayData{
  
  int spatialDim;
  int numGenCoord;
  int numLocalSpace;
  int numCartesianDim;
  int numBodies;
  double dt;
  double Time;
  int tend;
  int Axis[4];
 
};

struct PhysicalValues{
  
  double c1toj2Skew[3][3];
  double j2toc2Skew[3][3];
  
  double c2toj3Skew[3][3];
  double j3toc3Skew[3][3];

  double c3toj4Skew[3][3];
  double j4toc4Skew[3][3];

  double numLinks;

  double craneGlobCoord[3];
  double craneAngles[3];
  double Masses[4];

  int moveCrane;


};

struct EndEquationMatrix{

  double *Matrix;
  int firstDim;
  int secondDim;

};

struct EndEquationVector{

  double *Vector;
  int firstDim;

};


struct EndEquationMatrices{

  struct EndEquationMatrix Mass;
  struct EndEquationMatrix D;
  struct EndEquationMatrix MB;
  struct EndEquationMatrix DMB;
  struct EndEquationMatrix MdBDMB;
  struct EndEquationMatrix MdB;
  struct EndEquationMatrix BT;
  struct EndEquationMatrix B;
  struct EndEquationMatrix dB;
  struct EndEquationMatrix Mstar;
  struct EndEquationMatrix MstarI;
  struct EndEquationMatrix Nstar;

  struct EndEquationVector F;
  struct EndEquationVector Fstar;
  struct EndEquationVector NStarqn;
  struct EndEquationVector FmNstarqn;
  struct EndEquationVector Angles;
  struct EndEquationVector AngularVelocities;
  struct EndEquationVector AngularAccelerations;

  struct EndEquationVector NstarVel;
  struct EndEquationVector MstarAcc;
  struct EndEquationVector MstarAccAddNstarVel;
  struct EndEquationVector MstarAccAddNstarVelSubFstar;

  struct EndEquationVector MdBDMBdq;
  struct EndEquationVector MBddq;

  //struct EndEquationVector Sol;
  //struct EndEquationVector SolV;
  struct EndEquationVector waveMoments;
  struct EndEquationVector CraneMoments;

  struct EndEquationVector MomTow;
  struct EndEquationVector MomBoom;
  struct EndEquationVector MomStick;

};

struct SolutionVectors{

struct EndEquationVector Sol;
struct EndEquationVector SolV;
struct EndEquationVector k1;
struct EndEquationVector k2;
struct EndEquationVector Ang;
struct EndEquationVector AngV;
struct EndEquationVector TempAng;
struct EndEquationVector TempAngV;
struct EndEquationVector TempSol;
struct EndEquationVector TempSolV;
struct EndEquationVector maAndMom;

};
#endif
