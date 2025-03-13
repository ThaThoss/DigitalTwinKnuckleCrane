#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "qdconst.h"

typedef struct {
        double xx,yy,xy,I,II;
} SDIM;

typedef struct {
        double x, y;
} XYF;

typedef struct {
        int x, y;
} XYI;

typedef struct {
	XYI *num_fix;
	int *num_force;
	XYI *fix;
	int *force;
} BOUND;

typedef struct {
        double E;
        double nu;
        double rho;
} MATL;

typedef struct {
        SDIM pt[num_int];
} STRESS;

typedef struct {
        SDIM pt[num_int];
} STRAIN;

typedef struct {
	int numel;
	int numnp;
	int nmat;
	int plane_stress_flag;
	int num_fix_x;
	int num_fix_y;
	int num_force;
	int sizeOfCoord;
	int sizeOfConnect;
} SIZES;

typedef struct {
	char* buffer;
	double *forces;
	double *gravity_X;
	double *gravity_Y;
	double gravity_constant;
	int numBytesofForce;
	int gravity_flag;
	int *gogo;
} FORCESTRUCT;
