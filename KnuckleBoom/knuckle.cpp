#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include "./lib/craneControllAndCom.h"
#include "./lib/MyDynStructs.h"
#include "./lib/dynfunctions.h"
#include "./lib/getD.h"
#include "./lib/getB.h" 
#include "./lib/getdB.h" 
#include "./lib/getForces.h" 
#include "./lib/getAcc.h"
#include "./lib/getMAandMom.h"

#include "./../lib/myTimeFunctions.h"
#include "./../lib/sendReceive.h"
#include "./../lib/myPortsAndIP.h"

#define BILLION  1000000000L
#define PORT "8080"
#define IP "127.0.0.1"


using namespace std;

int main(int argc, char* argv[])
{

  int check;
  double time = 0, timestep = 0.005, dt =  0;
  char buffer[255];

  PreciseSleep mySleep;

  //Integration constants for RK4
  double a[4][4] = {0};
        a[1][1] = 0.5;
        a[2][2] = 0.5;
        a[3][3] = 1.0;
  double b[4] = {0};
  double const c[4] = {0, 0.5, 0.5, 1.0};

  b[0] = 1.0/6.0;
  b[1] = 1.0/3.0;
  b[2] = 1.0/3.0;
  b[3] = 1.0/6.0;

  //Connect to client:
	printf("Attempting to respond...");
  int sockfd = atoi(argv[1]);
  int nBodies = 0;
  int rc = write(sockfd,"200",sizeof("200"));
  ReceiveInt32(sockfd,&nBodies);
  printf("Recieved %d number of bodies", nBodies);
  int axisOfRotations[nBodies] = {0};
  sleep(1);
  ReceiveNInts(sockfd,axisOfRotations,nBodies*sizeof(int));
  int nBytesToRecive = calcBytesNeededForRBD( nBodies, axisOfRotations);
  char *initialData = (char *)calloc(nBytesToRecive,sizeof(char));
  ReceiveNChars(sockfd,initialData,nBytesToRecive);

  printf("Recievced axis of rotation[%d, %d, %d , %d]",axisOfRotations[0],axisOfRotations[1],axisOfRotations[2],axisOfRotations[3]);



  
/*Set array Data*/
  struct ArrayData myArrayData; 
    myArrayData.spatialDim = 3;
    myArrayData.Time = 0;
    myArrayData.numGenCoord = 6;
    myArrayData.numLocalSpace = 6;
    myArrayData.numCartesianDim = 21;
    myArrayData.numBodies = nBodies;
    myArrayData.dt = timestep;
    myArrayData.Axis[0] = 0;
    myArrayData.Axis[1] = 3;
    myArrayData.Axis[2] = 2;
    myArrayData.Axis[3] = 2;
  struct PhysicalValues myPhysicalValues;
  setSkewPositions(&myPhysicalValues);
  myPhysicalValues.moveCrane = 1;


  /* Set all the matrices for the end equation */
  struct EndEquationMatrices endEquationMatrices;
  check = endEquationAllocater(&endEquationMatrices, myArrayData);
  if(check){
    cout << "problem with endEquationAllocater, line 108 in main" << endl;
  }
  check = SetMassMatrix( endEquationMatrices.Mass );
  if(check){
    cout << "problem with SetMassMatrix, line 112 in main" << endl;
  }
  endEquationMatrices.AngularVelocities.Vector[0] = 0;
  endEquationMatrices.AngularVelocities.Vector[1] = 0;
  endEquationMatrices.AngularVelocities.Vector[2] = 0;
  endEquationMatrices.AngularVelocities.Vector[3] = 0;
  endEquationMatrices.AngularVelocities.Vector[4] = 0;
  endEquationMatrices.AngularVelocities.Vector[5] = 0;

  endEquationMatrices.Angles.Vector[0] = 0;
  endEquationMatrices.Angles.Vector[1] = 0;
  endEquationMatrices.Angles.Vector[2] = 0;
  endEquationMatrices.Angles.Vector[3] = 0;
  endEquationMatrices.Angles.Vector[4] = 0;
  endEquationMatrices.Angles.Vector[5] = 0;

  /*Set Frames*/
  struct Frame Frames[4];
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      if(i==j){
        Frames[0].tempR1.R[i][j]=1;
        Frames[0].AbsR.R[i][j]=1;
      }else{
        Frames[0].tempR1.R[i][j]=0;
        Frames[0].AbsR.R[i][j]=0;
        }
    }
  }

  check = setRotations(Frames, &endEquationMatrices, myArrayData);
  if(check){
    cout << "problem with setRotations, in main" << endl;
  }
  /* Get Global coordinates of crane tip */
  get_GlobPosCraneTip(&myPhysicalValues, Frames);
  myPhysicalValues.moveCrane = 0;

  struct SolutionVectors mySolutionVectors;
  SolutionVectorsAllocater(&mySolutionVectors, myArrayData);
  mySolutionVectors.Sol.Vector[3] = endEquationMatrices.Angles.Vector[3];
  mySolutionVectors.Sol.Vector[4] = endEquationMatrices.Angles.Vector[4];
  mySolutionVectors.Sol.Vector[5] = endEquationMatrices.Angles.Vector[5];

  /* Sets Gravity to F, as well as zero it out*/
  set_Gravity_to_F(&endEquationMatrices.F,myArrayData,myPhysicalValues);





//Initialize globalPosForCrane object
  globalPosForCrane craneTip(sockfd, timestep, myPhysicalValues, Frames);
  
 
  //Wait for client to be ready
  read(sockfd,buffer,255);
	check = strncmp(buffer,"gogo",4);
	if(check){
		printf("\nServer error with code %s\n",buffer);
		return -1;
	}
  printf("Knuckle ready, ansver was %s\n",buffer);
  mySleep.startClock();





  // Start main loop--------------------------------------------------------------- //
  bool gogo = true;
  while(gogo){
    copyVector(&mySolutionVectors.Ang, mySolutionVectors.Sol);
    copyVector(&mySolutionVectors.AngV, mySolutionVectors.SolV);

    for(int k=0; k<4;k++){
 
     /* Prepare AngVel and Angles for next RK4 step */
      VecByConstEndEq(&mySolutionVectors.TempAngV, mySolutionVectors.k1, a[k][k]);
      VecVecAddEndEq(&endEquationMatrices.AngularVelocities,mySolutionVectors.AngV,mySolutionVectors.TempAngV);

      VecByConstEndEq(&mySolutionVectors.TempAng, mySolutionVectors.k2, a[k][k]);
      VecVecAddEndEq(&endEquationMatrices.Angles, mySolutionVectors.Ang, mySolutionVectors.TempAng);

     /* Set rotations for next step */
      setRotations(Frames, &endEquationMatrices, myArrayData);

      /* Set the specifik RK4 time */
      
      myArrayData.Time = time + c[k] * myArrayData.dt;
  
      /* Get the acceleration */
      getAcc(&endEquationMatrices, Frames, myArrayData, myPhysicalValues );
      /* -----------------------------------------------------*/

      /* Get k1 and k2 */
      VecByConstEndEq(&mySolutionVectors.k1,endEquationMatrices.AngularAccelerations,myArrayData.dt);
      VecByConstEndEq(&mySolutionVectors.k2,endEquationMatrices.AngularVelocities,myArrayData.dt);

      /* Save Solution */
      VecByConstEndEq(&mySolutionVectors.TempSol,mySolutionVectors.k2,b[k]);
      VecVecAddEndEq(&mySolutionVectors.Sol,mySolutionVectors.Sol,mySolutionVectors.TempSol);

      VecByConstEndEq(&mySolutionVectors.TempSolV,mySolutionVectors.k1,b[k]);
      VecVecAddEndEq(&mySolutionVectors.SolV,mySolutionVectors.SolV,mySolutionVectors.TempSolV);
    }//end RK4 for(k)

    copyVector(&endEquationMatrices.Angles, mySolutionVectors.Sol);
    copyVector(&endEquationMatrices.AngularVelocities, mySolutionVectors.SolV);
    getAcc(&endEquationMatrices, Frames, myArrayData, myPhysicalValues );
    getMAandMom(&mySolutionVectors, &endEquationMatrices);

    
    /* set Rotations for next run 
    setRotations(Frames, &endEquationMatrices, myArrayData);
    for(int k=0; k<3; k++){
      for(int l=0;l<3;l++){
        fprintf(g, "%lf", Frames[0].AbsR.R[k][l]);
        if(k+l<4){
          fprintf(g, ", ");
        }else{
          fprintf(g, "\n");
        }
      }
    }*/
      memcpy(Frames[0].tempR1.R, Frames[0].AbsR.R, sizeof(double)*9);




    //Comunication Section --------------------------------------------
    // Send angles to server
    craneTip.sendData(Frames,mySolutionVectors,endEquationMatrices.AngularAccelerations );

    //Recieve wanted crane position
    craneTip.recieveData();
    //returns 1 if gogo == 0;
    if(craneTip.gogo()){
      gogo = false;
      std::cout << "Knuckle got gogo = 0, shutting down" << std::endl;
      break;
    }
     
    if(craneTip.moveCrane()){

      myPhysicalValues.moveCrane = 1;
      craneTip.getGlobalPosForCrane(myPhysicalValues.craneGlobCoord);
      
      /*
      std::cout <<"moveCrane() = " << craneTip.moveCrane() << ", gogo = " << craneTip.gogo() << std::endl; 
      
      std::cout << "th4 = " << mySolutionVectors.Sol.Vector[3] << ", th5 = "<<
      mySolutionVectors.Sol.Vector[4] << ", th6 = " << mySolutionVectors.Sol.Vector[5] << std::endl;
      
      std::cout << "GlobCoord = [ " << myPhysicalValues.craneGlobCoord[0] << ", "<<
      myPhysicalValues.craneGlobCoord[1] << ", " << myPhysicalValues.craneGlobCoord[2]
       <<" ]"<< std::endl; 
       */
   
    }else if(myPhysicalValues.moveCrane){  

      myPhysicalValues.moveCrane = 0;
      myPhysicalValues.craneAngles[0] = mySolutionVectors.Sol.Vector[3];
      myPhysicalValues.craneAngles[1] = mySolutionVectors.Sol.Vector[4];
      myPhysicalValues.craneAngles[2] = mySolutionVectors.Sol.Vector[5];

    }
   //----------------------------------------------------------------


    mySleep.sleepTillTimeStep(timestep);

    dt = mySleep.getLastTimeStep();
    time = mySleep.getTotalDuration();

    myArrayData.dt = dt;
    if(dt>timestep+0.0001){
      cout << "dt large, = " << dt << endl;
    }
    //-----------------------------------------------------------------

  }//end main for(i)

  KillMemory( &endEquationMatrices, &mySolutionVectors);
  free(initialData);
  printf("\nI'm done \n");
/*
  fclose(g);
  fclose(f);
*/  
  time = mySleep.getTotalDuration();
   
  printf("total time in knuckle was: %lf\n, knucle closed\n", time);

  return 1;
}
