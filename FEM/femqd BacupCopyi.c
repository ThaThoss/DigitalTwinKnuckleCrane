#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Server.h"


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
        int     check, MemoryCounter;
        int     sofmXYI, sofmSDIM, ptr_inc;
	int     sofmA, sofmi, sofmf, sofmSTRESS;
        int     mem_case, counter;
	int 	sockfd;

        double  *mem_double; 
	double  *coord, *force, *U, *A;	
        double  *node_counter;	
	double  g;
	sockfd = atoi(argv[1]);
	rc = write(sockfd,"200",sizeof("200"));


	sof = sizeof(double);

	check = qdServerInitialReceiver( &sizes, sockfd);
/* Set external variables, because stupid */
	nmat = sizes.nmat;
	numel = sizes.numel;
	numnp = sizes.numnp;
	plane_stress_flag = sizes.plane_stress_flag;


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
	sofmf = nsd*sizes.numnp + 2*dof + sizes.numnp; 
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
        U            = (mem_double+ptr_inc);            ptr_inc += dof;
	node_counter = (mem_double+ptr_inc);            ptr_inc += sizes.numnp;

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

/* Receive the data from Client */ 
	check =	qdServerReceiver(bc, connect, coord, el_matl, force, matl, U, &sizes, sockfd);

	
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
	check = qdKassemble( A, connect, coord, el_matl, force, id, lm, 
			matl, node_counter, U, strain, strain_node, stress, 
			stress_node); 
	if(!check) printf( " Problems with qdKassembler \n");


	//check = solveGauss(A, force, id, neqn, dof);
	if(!check) printf( " Problems with solve \n");

double *solutionVec = (double *)calloc(neqn,sizeof(double));
double *inv = (double *)calloc(sofmA,sizeof(double));

solve( solutionVec, inv, A, force, neqn);
 


//matvecmult(solutionVec,inv,force,neqn,neqn);
	counter = 0;
	for( i = 0; i < dof; i++) {
	   if( *(id + i) > -1 ) {
	       *(U +  i) = *(solutionVec + counter);
	       counter++;	
	   }
	}
	
 /* Print solution*/	
	for( i = 0; i<dof; i++){
		printf("U[%d]=%lf\n",i,*(U+i));
	}


/* Calculate reaction forces*/ 
	analysis_flag = 2;
	memset(force, 0, dof*sof);
	check = qdKassembleStress( connect, coord, el_matl, 	
	  matl, node_counter, U, strain, strain_node, stress, stress_node); 

	if(!check) printf( "Problems with qdKassembler \n");
	check = qdwriter( bc, connect, coord, el_matl,  id, matl, 
			name, strain, strain_node, stress, stress_node, U);
	if(!check) printf( "Problems with qdwriter \n");

	free(strain);
	free(stress);
	free(mem_SDIM);
	free(matl);
	free(A);
	free(mem_double);
	free(mem_int); 
	free(mem_XYI);	
	close(sockfd);
	printf("Done?? ");  

}
