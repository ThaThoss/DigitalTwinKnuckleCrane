#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qdconst.h"
#include "qdstruct.h"

extern int dof, nmat, nmode, numel, numnp, plane_stress_flag;

int qdreader( BOUND bc, int *connect, double *coord, int *el_matl, double *force,
	MATL *matl, FILE *o1, double *U) { 


	int i, j, dum;
	char buf[ BUFSIZ ];

	printf( "number of elements:%d nodes:%d materials:%d dof:%d\n", 
		numel, numnp, nmat, dof);
	printf( "Plane Theory :%d \n", plane_stress_flag);
        fgets( buf, BUFSIZ, o1 );
	printf("\n");

/* Pull out material info and echo to screen */
        for( i = 0; i < nmat; i++ ) {
	   fscanf( o1, "%d\n", &dum );
	   printf( "material (%3d) Emod, nu, density", dum );
	   fscanf( o1, " %lf %lf %lf\n", &matl[dum].E, &matl[dum].nu, &matl[dum].rho);
	   printf( " %9.4f %9.4f %9.4f\n", matl[dum].E, matl[dum].nu, matl[dum].rho);
	} 
	fgets( buf, BUFSIZ, o1 );
	printf( "\n" );

/* Pull out connectivity info and echo to screen                        */
/*    1) connect contains the node numbers which make up an element     */
/*       (size of connect is npel * numel)                              */ 
/*    2) node numbering occurs in a counter-clockwise direction         */ 
	for( i = 0; i < numel; i++ ) {
	   fscanf( o1, "%d", &dum );
	   printf( "connectivity for element (%4d) ", dum);
	   for( j = 0; j < npel; j++ ) {
		   fscanf( o1, "%d", (connect+npel*dum+j));
		   printf( "%4d ", *(connect+npel*dum+j));
	   }
	   fscanf( o1, "%d\n", (el_matl+dum));
	   printf( " with matl %3d\n", *(el_matl+dum));
	} 
        fgets( buf, BUFSIZ, o1 );
        printf( "\n" );	

/* Pull out node coordinate info and echo to screen                     */
/*    1) coord contains the x, y coordinates of a node                  */  
/*       (size of coord is nsd*numnp)                                   */ 
	for( i = 0; i < numnp; i++ ) {
           fscanf( o1, "%d", &dum);
           printf( "Node (%d) ", dum);
	   printf( "coordinates ");
	   for( j = 0; j < nsd; j++ ) {
		   fscanf( o1, "%lf", (coord+nsd*dum+j));
		   printf( "%9.4f ", *(coord+nsd*dum+j));
	   }
           fscanf( o1, "\n");
           printf( "\n" );	   
	}
        fgets( buf, BUFSIZ, o1 );
        printf( "\n" );	

/* Pull out x-boundary condition info and echo to screen                */
/*    1) bc.fix.x contains the node numbers where the                   */ 
/*       x-displacement is prescribed                                   */       
/*    2) x-displacement value is put into correct location in the U     */ 
/*    	 array. (size of U is dof=ndof * numnp)                         */ 
/*    3) bc.num_fix.x contains the total number of nodes where          */ 
/*       x-displacement has been prescribed                             */ 
	dum = 0;
	fscanf( o1, "%d", &bc.fix[dum].x );
	printf( "node (%4d) has an x prescribed displacement of: ", bc.fix[dum].x);
	while( bc.fix[dum].x > -1 ) {
		fscanf( o1, "%lf\n%d", (U+ndof*bc.fix[dum].x), &bc.fix[dum+1].x);
		printf( "%14.6e\n", *(U+ndof*bc.fix[dum].x));
		printf( "node (%4d) has an x prescribed displacement of: ",
			bc.fix[dum+1].x);
		dum++;
	}
        bc.num_fix[0].x = dum;
	fscanf( o1, "\n" );
        fgets( buf, BUFSIZ, o1 );
	printf( "\n\n" );

/* Pull out y-boundary condition info and echo to screen                */
/*    1) bc.fix.y contains the node numbers where the                   */ 
/*       y-displacement is prescribed                                   */       
/*    2) y-displacement value is put into correct location in the U     */ 
/*    	 array. (Size of U is dof=ndof * numnp)                         */ 
/*    3) bc.num_fix.y contains the total number of nodes where          */ 
/*       y-displacement has been prescribed                             */ 
        dum = 0;
	fscanf( o1, "%d", &bc.fix[dum].y );
	printf( "node (%4d) has an y prescribed displacement of: ", bc.fix[dum].y);
	while( bc.fix[dum].y > -1 ) {
		fscanf( o1, "%lf\n%d", (U+ndof*bc.fix[dum].y+1), &bc.fix[dum+1].y);
		printf( "%14.6e\n", *(U+ndof*bc.fix[dum].y+1));
		printf( "node (%4d) has an y prescribed displacement of: ",
			bc.fix[dum+1].y);
		dum++;
	}
        bc.num_fix[0].y = dum;
	fscanf( o1, "\n" );
        fgets( buf, BUFSIZ, o1 );
	printf( "\n\n" );
	
/* Pull out force boundary condition info and echo to screen            */
/*    1) bc.force contains the node numbers where a nodal force         */ 
/*       has been prescribed                                            */       
/*    3) bc.num_force contains the total number of nodes where          */ 
/*       a force has been prescribed                                    */ 
	dum = 0;
	printf( "force vector for node: " );
	fscanf( o1, "%d", &bc.force[dum]);
	printf( "(%4d)", bc.force[dum]);
	while( bc.force[dum] > -1 ) {
		for( j = 0; j < ndof; j++ ) {
			fscanf( o1, "%lf ", (force+ndof*bc.force[dum]+j));
			printf( "%16.4f "  , *(force+ndof*bc.force[dum]+j));
                }
                fscanf( o1, "\n");
		printf( "\n" );
		printf( "force vector for node: ");
		dum++;
		fscanf( o1, "%d", &bc.force[dum]);
		printf( "(%4d)" ,  bc.force[dum]);

	}
	bc.num_force[0] = dum;
	fscanf( o1, "\n" );
	fgets( buf, BUFSIZ, o1 );
	printf( "\n\n" );

	return 1;
}	
