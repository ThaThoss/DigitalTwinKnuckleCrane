#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include "Server.h"
#include "qdfunctions.h"
int qdCalcForce(double * force, double *tempForce,double *nodalMass, BOUND bc,FORCESTRUCT *forceStruct, int *id);

int qdSaveInverse(double* inverse, int neqn, char fileName[]);
int qdInverseReader(double* inv, int neqn, char fileName[]);

int compareMatrices(double* A, double* B, int size, double tol) {
	double diff = 0;
	for (int i = 0; i < size * size; i++) {
		diff = fabs(A[i] - B[i]);
		if (fabs(A[i] - B[i]) > tol) {
			printf("Mismatch at index %d: %f vs %f, diff was: %lf\n", i, A[i], B[i], diff);
		}
	}
	return 1;
}
volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    printf("\nSIGINT received! Shutting down...\n");
    stop = 1;
}

int main(int argc, char** argv)
{
	char name[30], buf[ BUFSIZ ];

	BOUND   bc;
	MATL    *matl;
	XYI     *mem_XYI; 
	STRESS  *stress;
	STRAIN  *strain;
	SDIM    *stress_node, *strain_node, *mem_SDIM;	
	SIZES 	sizes;
	
	int     i, j, k, rc;
	int     *id, *lm,  *mem_int; 
	int     *connect, *el_matl;
	int     check =0, MemoryCounter;
	int     sofmXYI, sofmSDIM, ptr_inc;
	int     sofmA, sofmi, sofmf, sofmSTRESS;
	int     mem_case, counter;
	int 	sockfd;
	int numFixX = 0;
	int numFixY = 0;
	int numForce = 0;

	double  *mem_double; 
	double  *coord, *force, *tempForce, *U, *A;	
	double  *node_counter, *nodalMass;	
	double  g;

	struct sigaction sa;
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; 
	sigaction(SIGINT, &sa, NULL);
	
	sockfd = atoi(argv[1]);
	rc = write(sockfd,"200",sizeof("200"));


	sof = sizeof(double);

	FORCESTRUCT forceStruct;
	forceStruct.gravity_flag = 0;
	forceStruct.gravity_constant = 9.81;
	check = qdServerInitialReceiver( &sizes, &forceStruct, sockfd);
/* Set external variables, because stupid */
	nmat = sizes.nmat;
	numel = sizes.numel;
	numnp = sizes.numnp;
	plane_stress_flag = sizes.plane_stress_flag;
	gravity_flag = forceStruct.gravity_flag;
	numFixX = sizes.num_fix_x;
	numFixY = sizes.num_fix_y;
	numForce = sizes.num_force;

	int bodyNum = 0;
	printf("nmat = %d, numel = %d, numnp = %d, planeFlag = %d, gravityFlg = %d, nFXX = %d, nFXY = %d, nForce = %d\n",nmat,numel,numnp,plane_stress_flag,gravity_flag,numFixX,numFixY,numForce);
	//add gravity value to initial value reciever
	// {{2460, 2566,   1,   1,   1},{2220, 2397,   1,   1,   1},{1328, 1468,   1,   1,   1}};

	char stiffnessFileMname[24] = {0};
	if(numel==2460){
		strcpy(stiffnessFileMname, "towerInverse.dat");
		bodyNum = 1;
	}else if(numel==2220){
		strcpy(stiffnessFileMname, "boomInverse.dat");
		bodyNum = 2;
	}else if(numel==1328){
		strcpy(stiffnessFileMname, "outerInverse.dat");
		bodyNum=3;
	}

/* Create local shape functions at gauss points */
    g = 2.0/sq3;
	check = qdshl( g, shl, w );
	if(!check) printf( " Problems with qdshl \n");

/* Create local shape functions at nodal points */
	g = 2.0;
	check = qdshl( g, shl_node, w );
	if(!check) printf( " Problems with qdshl \n");

	dof = sizes.numnp * ndof;

	numel_k = sizes.numel;
	numel_P = 0;
	sizes.sizeOfCoord = sizes.numnp*nsd;
	sizes.sizeOfConnect = sizes.numel*npel;

/* Begin allocation of memory */
	MemoryCounter = 0;

/* For the doubles */
	sofmf = nsd*sizes.numnp + sizes.numnp + 3*dof + sizes.numnp; 
    MemoryCounter += sofmf * sizeof(double);

/* For the integers */
	sofmi = sizes.numel*npel + dof +  sizes.numel + sizes.num_force + 1 + 1 + sizes.numel*npel*ndof; 
    MemoryCounter += sofmi * sizeof(int);

/* For the XYI integers */
	if(sizes.num_fix_x>sizes.num_fix_y){
		sofmXYI = sizes.num_fix_x + 1;
	}else{
		sofmXYI = sizes.num_fix_y + 1;
	}
	MemoryCounter += sofmXYI*sizeof(XYI);


/* For the SDIM doubles */
	sofmSDIM = 2 * sizes.numnp;
    MemoryCounter += sofmSDIM * sizeof(SDIM);
 
/* For the STRESS */
	sofmSTRESS = sizes.numel;
    MemoryCounter += sofmSTRESS*sizeof(STRESS) + sofmSTRESS*sizeof(STRAIN);

	check = qdMemory( &mem_double, sofmf, &mem_int, sofmi, &matl, sizes.nmat,
	&mem_XYI, sofmXYI, &mem_SDIM, sofmSDIM, &strain, &stress, sofmSTRESS );
	if(!check) printf( " Problems with qdMemory \n");




/* For the doubles */
	                                                ptr_inc = 0;
	coord        = (mem_double+ptr_inc);            ptr_inc += sizes.numnp*nsd;
	force        = (mem_double+ptr_inc);            ptr_inc += dof;
	tempForce	 = (mem_double+ptr_inc);            ptr_inc += dof;
    U        	 = (mem_double+ptr_inc);            ptr_inc += dof;
	node_counter = (mem_double+ptr_inc);            ptr_inc += sizes.numnp;
	nodalMass	 = (mem_double+ptr_inc);            ptr_inc += sizes.numnp;

/* For the integers */
                                                    ptr_inc = 0;	
	connect      = (mem_int+ptr_inc);               ptr_inc += sizes.numel*npel;
    id           = (mem_int+ptr_inc);               ptr_inc += dof; 
    lm           = (mem_int+ptr_inc);               ptr_inc += sizes.numel*npel*ndof; 
	el_matl      = (mem_int+ptr_inc);               ptr_inc += sizes.numel;
    bc.force     = (mem_int+ptr_inc);               ptr_inc += sizes.num_force;
	bc.num_force = (mem_int+ptr_inc);               ptr_inc += 1;

/* For the XYI integers */
                                                    ptr_inc = 0;
	bc.fix       = (mem_XYI+ptr_inc);               ptr_inc += sofmXYI - 1;
    bc.num_fix   = (mem_XYI+ptr_inc);               ptr_inc += 1;

/* For the SDIM doubles */
                                                    ptr_inc = 0;


	stress_node  = (mem_SDIM+ptr_inc);              ptr_inc += sizes.numnp;
	strain_node  = (mem_SDIM+ptr_inc);              ptr_inc += sizes.numnp;	

/* Set sizes to bc */
	bc.num_force[0] = sizes.num_force;
	bc.num_fix[0].x = sizes.num_fix_x;
	bc.num_fix[0].y = sizes.num_fix_y;



	/*[force x y],gravityx(double),gravityy(double),gogo*/
	forceStruct.numBytesofForce = (bc.num_force[0]*2+ndof)*sizeof(double) + sizeof(int);
	forceStruct.buffer = (char*)calloc(forceStruct.numBytesofForce, sizeof(char));
	forceStruct.forces = (double*)forceStruct.buffer;

	forceStruct.gravity_X = (forceStruct.forces + bc.num_force[0]*ndof + 0);
	forceStruct.gravity_Y = (forceStruct.forces + bc.num_force[0]*ndof + 1);
	forceStruct.gogo = (int*)(forceStruct.buffer + (bc.num_force[0]*2+ndof)*sizeof(double));
	*(forceStruct.gogo) = 42;

	sleep(120);
/* Receive the data from Client */ 
	check =	qdServerReceiver(bc, connect, coord, el_matl, forceStruct, matl, U, &sizes, sockfd);

/*
for(int i=0;i<sizes.numel;i++){
	printf("el[%d] = [%d, %d, %d, %d]\n",i,*(connect+i*4 +0), *(connect+i*4 +1),*(connect+i*4 +2),*(connect+i*4 +3));
}
printf("\n\n");
for(int i=0;i<sizes.numnp;i++){
	printf("nodeNr[%d] = [%lf, %lf]\n",i,*(coord+i*2 +0), *(coord+i*2 +1));
}
printf("\n\n");
*/
printf( " Bef  qdformid\n" );
	check = qdformid( bc, id );
	if(!check) printf( " Problems with qdformid \n");



	check = formlm( connect, id, lm, ndof, npel, numel );


	if(!check) printf( " Problems with formlm \n" );
	//printf("neqn is : %d, and dof is: %d\n" ,neqn,dof);
	sofmA = neqn * neqn;
	MemoryCounter += sofmA*sizeof(double);	
	A = (double *)calloc(sofmA, sizeof(double)); 
	if(!A) {
	    printf(" Failed to allocate memory for A double \n");
     	exit(1);
	}		




	analysis_flag = 1;
	printf("Starting qdKassemble\n");
	check = qdKassemble( A, connect, coord, el_matl, force, id, lm, 
			matl, node_counter, U, nodalMass,tempForce, &forceStruct, bc); 
	if(!check) printf( " Problems with qdKassembler \n");

	printf("qdKassemble ended\n");


	double *solutionVec, *vonMieses, *dataToSend;
	int numBytesToSend = (dof+numnp)*sizeof(double);
	dataToSend = (double *)calloc(numnp+dof,sizeof(double));
	solutionVec = (double *)calloc(neqn,sizeof(double));
	vonMieses = (dataToSend+dof);
	double *inv = (double *)calloc(sofmA,sizeof(double));

	printf("reading Inverse from file...");
	qdInverseReader(inv, neqn, stiffnessFileMname);
	printf("Initial Solver started\n");
	//To speed up the demonstration, the solution for the specific simulation has been saved. Uncomment this for future use
	//solve( solutionVec, inv, A, force, neqn);
	matvecmult(solutionVec,inv,force,neqn,neqn);
	
	//qdSaveInverse(inv,neqn,  stiffnessFileMname);
	//qdInverseReader(A, neqn, stiffnessFileMname);
/*
	int tol = 0.01;
	printf("--------------------Comparing Matrices-----------------\n");
	compareMatrices( A,  inv, neqn, tol);
	free(A);
	printf("--------------------Done Comparing Matrices-----------------\n");
*/


// Start of FEM Loop //
// -------------------------------------------------------------//

	counter = 0;
	for( i = 0; i < dof; i++) {
	   if( *(id + i) > -1 ) {
	       *(U +  i) = *(solutionVec + counter);
	       counter++;	
	   }
	}
/* Print solution	
	for( i = 0; i<dof; i++){
		printf("U[%d]=%lf\n",i,*(U+i));
	}	
*/

/* Calculate stress and strain*/ 
printf("Starting qdKassmembleStress\n");
	check = qdKassembleStress( connect, coord, el_matl, 	
	  matl, node_counter, U, strain, strain_node, stress, stress_node, vonMieses); 

	if(!check) printf( "Problems with qdKassembler \n");

	//name[0] = 'd';
	//check = qdwriter( bc, connect, coord, el_matl,  id, matl, 
	//		name, strain, strain_node, stress, stress_node, U, vonMieses);
	//if(!check) printf( "Problems with qdwriter \n");


	int gogo=1;
	int sizeOfDouble = sizeof(double);
	//first run with gravity straight down.

	memcpy(dataToSend,U,dof*sizeof(double));//U has predefined dispacements, carefull if non 0;



	printf("Starting FEM loop... \n");
	while(gogo && !stop){
		//Send solution, x,y translation, and von mieses stress;
		
		if( SendNChar(sockfd,(char*)dataToSend ,numBytesToSend )){
			printf("problem with sending solution");
		}

		//recieve force
		printf("Requesting Force Result \n");
		gogo = recieveForce( bc, forceStruct, sockfd);
		printf("Recieved force, gravity x,y = [%lf , %lf] \n", *(forceStruct.gravity_X),*(forceStruct.gravity_Y));

		//calculate force
		qdCalcForce(force, tempForce, nodalMass, bc, &forceStruct, id);
		/*
		if(bodyNum==3){
			printf("----------------------Force:--------------------\n\n\n");
			for(int i=0; i<numnp;i++){

				printf("%d: %lf, %lf\n", i, *(force+i*2), *(force +i*2 +1));

			}
			printf("\n\n\n");
		}
		*/
		//solve again
		matvecmult(solutionVec,inv,force,neqn,neqn);
		counter = 0;
		for( i = 0; i < dof; i++) {
			if( *(id + i) > -1 ) {
				*(dataToSend +  i) = *(solutionVec + counter);
				counter++;	
			}
		}

		//calculate von mieses, vonMieses pointer already pointing to dataTosend
		memset(stress, 0, sofmSTRESS*sizeof(STRESS));
		memset(strain, 0, sofmSTRESS*sizeof(STRAIN));
		memset(strain_node, 0, sizes.numnp*sizeof(SDIM));
		memset(stress_node, 0, sizes.numnp*sizeof(SDIM));
		check = qdKassembleStress( connect, coord, el_matl, 	
				matl, node_counter, dataToSend, strain, strain_node, stress, 
				stress_node, vonMieses); 
/*
	if(bodyNum==3){
		for( int i = 0; i<numnp; i++){
			printf("VonMieses[%d]=%lf\n",i,*(vonMieses+i));
		}
	}
*/

	}

	close(sockfd);
	free(strain);
	free(matl);
	free(mem_int); 
	free(mem_SDIM);
	free(mem_XYI);	
	free(inv);
	free(solutionVec);
	free(dataToSend);
	free(forceStruct.buffer);
	free(stress);
	free(mem_double);
	
printf("FEM server shutting down, gogo was: %d\n", gogo);
	

}
