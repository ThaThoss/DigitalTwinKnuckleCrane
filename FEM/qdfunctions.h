




#ifndef QDFUNC
#define QDFUNC


int qdshl( double, double *, double * );
/*
int shg_print( double *, int, int );

int qdshl_node2( double * );

int shl_print( double * );
*/
int qdMemory( double **, int, int **, int, MATL **, int, XYI **, int,
		SDIM **, int, STRAIN **, STRESS **, int );

int qdformid ( BOUND, int * );

int formlm( int *, int *, int *, int, int, int );

int qdreader( BOUND, int *, double *, int *, double *, MATL *, FILE *, double *);

int qdshl( double , double *, double * );

int quadB( double *, double *) ;

int matXT( double *, double *, double *, int , int , int );

int globalKassemble( double *, double *, int *, int , int );

int qdKassemble( double *, int *, double *, int *, double *,	
	int  *, int *,  MATL *, double *, double *, double *, double *,
		FORCESTRUCT *, BOUND );

int qdKassembleStress( int *, double *, int *,	
	MATL *, double *, double *, STRAIN *,
    SDIM *, STRESS *, SDIM *, double *);

double qdArea(double *);
		
int qdwriter(BOUND, int *, double *, int *, int *, MATL *, char *, STRAIN *, SDIM *, STRESS *, SDIM *, double *, double *);
int matvecmult(double * , double * , double * , int, int );
int solve(double *, double *, double *, double *, int);
int solveGauss( double *, double *, int *, int , int );
int qdCalcForce(double * , double *,double *, BOUND ,FORCESTRUCT *, int *);



int     sof, analysis_flag, dof, neqn, nmat, nmode, numel, numnp, plane_stress_flag, gravity_flag;
int     numel_k, numel_P;
double  shg[sosh], shg_node[sosh], shl_node[sosh], shl[sosh];
double  shl_node2[sosh_node2], w[num_int]; 

#endif