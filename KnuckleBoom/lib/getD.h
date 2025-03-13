#ifndef GETD
#define GETD
int getD(struct EndEquationMatrix D, struct Frame Frames[4], struct EndEquationVector AngularVelocities ){
    int row = D.firstDim;
    int i=0;
    int j=0;
    /* Insert first omega to D */
    D.Matrix[i*row + j + 1] = -AngularVelocities.Vector[2]; D.Matrix[i*row + j + 2] = AngularVelocities.Vector[1];
    i++;
    D.Matrix[i*row + j] =  AngularVelocities.Vector[2]; D.Matrix[i*row + j + 2] = -AngularVelocities.Vector[0];
    i++;
    D.Matrix[i*row + j] = -AngularVelocities.Vector[1]; D.Matrix[i*row + j + 1] =  AngularVelocities.Vector[0];
    i++;

    /* Increment i and j to next omega */
    i += 3;
    j += 6;

    /* Calculate omega vector 2 */
    double D0 = Frames[1].locR.R[0][0]*AngularVelocities.Vector[0] + Frames[1].locR.R[1][0]*AngularVelocities.Vector[1] + Frames[1].locR.R[2][0]*AngularVelocities.Vector[2] + 0;
   
    double D1 = Frames[1].locR.R[0][1]*AngularVelocities.Vector[0] + Frames[1].locR.R[1][1]*AngularVelocities.Vector[1] + Frames[1].locR.R[2][1]*AngularVelocities.Vector[2] + 0;
   
    double D2 = Frames[1].locR.R[0][2]*AngularVelocities.Vector[0] + Frames[1].locR.R[1][2]*AngularVelocities.Vector[1] + Frames[1].locR.R[2][2]*AngularVelocities.Vector[2] + AngularVelocities.Vector[3];

    /* Inserte omega skew 2 */
    D.Matrix[i*row + j + 1] = -D2; D.Matrix[i*row + j + 2] = D1;
    i++;
    D.Matrix[i*row + j] = D2; D.Matrix[i*row + j + 2] = -D0;
    i++;
    D.Matrix[i*row + j] = -D1; D.Matrix[i*row + j + 1] = D0;
    i++;

    /* Increment i and j to next omega */
    i += 3;
    j += 6;

    /* Calculate omega vector 3 */
    double D01 = Frames[2].locR.R[0][0]*D0 + Frames[2].locR.R[1][0]*D1 + Frames[2].locR.R[2][0]*D2 + 0;
   
    double D11 = Frames[2].locR.R[0][1]*D0 + Frames[2].locR.R[1][1]*D1 + Frames[2].locR.R[2][1]*D2 + AngularVelocities.Vector[4];
   
    double D21 = Frames[2].locR.R[0][2]*D0 + Frames[2].locR.R[1][2]*D1 + Frames[2].locR.R[2][2]*D2 + 0;

    /* Insert omega 3 skew */
    D.Matrix[i*row + j + 1] = -D21; D.Matrix[i*row + j + 2] = D11;
    i++;
    D.Matrix[i*row + j] = D21; D.Matrix[i*row + j + 2] = -D01;
    i++;
    D.Matrix[i*row + j] = -D11; D.Matrix[i*row + j + 1] = D01;
    i++;

    /* Increment i and j to next omega */
    i += 3;
    j += 6;

    double D02 = Frames[3].locR.R[0][0]*D01 + Frames[3].locR.R[1][0]*D11 + Frames[3].locR.R[2][0]*D21 + 0;
   
    double D12 = Frames[3].locR.R[0][1]*D01 + Frames[3].locR.R[1][1]*D11 + Frames[3].locR.R[2][1]*D21 + AngularVelocities.Vector[5];
   
    double D22 = Frames[3].locR.R[0][2]*D01 + Frames[3].locR.R[1][2]*D11 + Frames[3].locR.R[2][2]*D21 + 0;

    D.Matrix[i*row + j + 1] = -D22; D.Matrix[i*row + j + 2] = D12;
    i++;
    D.Matrix[i*row + j] = D22; D.Matrix[i*row + j + 2] = -D02;
    i++;
    D.Matrix[i*row + j] = -D12; D.Matrix[i*row + j + 1] = D02;
    i++;

return 1;
}
#endif