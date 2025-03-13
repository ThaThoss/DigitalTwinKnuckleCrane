#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Server.h"

int main(int argc, char** argv)
{
	printf("In femqdTester\n");	
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
	printf("sent 200\n");

	sof = sizeof(double);
	printf("receiving ints\n");
	check = qdServerInitialReceiver( &sizes, sockfd);
	printf("Number of elements are %d\n",sizes.numel);
	printf("number of nodal points is %d\n",sizes.numnp);
	printf("Number of materials is %d\n",sizes.nmat);
	printf("Plane stress flag are %d \n",sizes.plane_stress_flag);
	printf("Number of fixed nodes are %d in x and %d in y \n",sizes.num_fix_x,sizes.num_fix_y);
	printf("Number of forces are %d \n",sizes.num_force);


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
/* Receive data */
	qdServerReceiver(bc, connect, coord, el_matl, force, matl, U, &sizes, sockfd);

	check = printConnect(connect, sizes);
	check = printCoord(coord, sizes);

	free(strain);
	free(stress);
	free(mem_SDIM);
	free(matl);
	free(A);
	free(mem_double);
	free(mem_int); 
	free(mem_XYI);	
	printf("Done :D");
}
