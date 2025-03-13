#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qdconst.h"
#include "qdstruct.h"

int qdMemory( double **mem_double, int sofmf, int **mem_int, int sofmi, 
	MATL **matl, int nmat, XYI **mem_XYI, int sofmXYI, SDIM **mem_SDIM, 
	int sofmSDIM, STRAIN **strain, STRESS **stress, int sofmSTRESS ) {

/* For the doubles */
	*mem_double = (double *)calloc(sofmf, sizeof(double));

	if(!mem_double)
	{
	    printf( "failed to allocate memory for doubles\n ");
            exit(1);
	}

/* For the materials */
	*matl = (MATL *)calloc(nmat, sizeof(MATL));	
	if(!matl)
	{
	    printf( "failed to allocate memory for matl doubles\n ");
            exit(1);
 
	}	

/* For the STRESS doubles */
	*stress = (STRESS *)calloc(sofmSTRESS, sizeof(STRESS));
	if(!stress)
	{
	    printf( "failed to allocate memory for stress doubles\n ");
            exit(1);
	} 


/* For the STRAIN doubles */
	*strain = (STRAIN *)calloc(sofmSTRESS, sizeof(STRAIN));
	if(!strain)
	{
	    printf( "failed to allocate memory for strain doubles\n ");
            exit(1);
	} 

/* For the integers */
	*mem_int = (int *)calloc(sofmi, sizeof(int));
        if(!mem_int)
	{
		printf( "failed to allocate memory for the integers\n ");
		exit(1);
	}

/* For the XYI integers */
	*mem_XYI = (XYI *)calloc(sofmXYI, sizeof(XYI));
	if(!mem_XYI)
	{
		printf( "failed to allocate memory for XYI integers\n" );
		exit(1);
	}

/* For the SDIM doubles */
	*mem_SDIM = (SDIM *)calloc(sofmSDIM, sizeof(SDIM));
	if(!mem_SDIM)
	{
		printf( "failed to allocate memory for SDIM doubles\n" );
		exit(1);
	}

	return 1;	
}
