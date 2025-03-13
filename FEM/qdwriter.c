#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "qdconst.h"
#include "qdstruct.h"

extern int dof, nmat, numel, numnp, plane_stress_flag, gravity_flag;

int qdwriter( BOUND bc, int *connect, double *coord, int *el_matl,
		int *id, MATL *matl, char *name, STRAIN *strain, SDIM *strain_node, 
		STRESS *stress, SDIM *stress_node, double *U, double *vonMieses) {

	int 	i,j, check, name_length=0;
	char 	*ccheck;
	double	fpointx, fpointy;
	char	out[30]={'\0'};
	FILE	*o3;

	name_length = strlen(name);
	if( name_length > 25) name_length = 25;

	memset(out, 0, 30*sizeof(char));

	ccheck = strncpy(out, name, name_length);
	if(!ccheck) printf( "Problems with strncpy\n");

	ccheck = strncpy(out+name_length, ".oqd", 4);
	if(!ccheck) printf( "Problems with strncpy\n");

	o3 = fopen( out, "w");

	fprintf(o3, "   numel numnp nmat plane_stress_flag gravity_flag");	
	fprintf(o3, " (This is for the quad mesh file: %s)\n", name);	
	fprintf(o3, "    %4d %4d %4d %4d %4d \n", 
		numel, numnp, nmat, plane_stress_flag,gravity_flag);	
	fprintf(o3, "matl no., E modulus, Poisson's Ratio, density \n");

	for( i = 0; i < nmat; i++) {
		fprintf(o3, "  %4d    %12.6e %12.6e %12.6e\n", i, 
			matl[i].E, matl[i].nu, matl[i].rho);	
	}

	fprintf(o3, "el no., connectivity, matl no \n");
	for( i = 0; i < numel; i++) {
	   fprintf(o3, "%6d ", i);	   
	   for( j = 0; j < npel; j++) {
		fprintf(o3, "%6d ", *(connect+npel*i+j));
	   }
	   fprintf(o3, "   %3d\n", *(el_matl+i));
	} 
	
	fprintf(o3, "node no., coordinates \n");
        for(i = 0; i < numnp; i++) {
	   fpointx = *(coord+nsd*i) + *(U+ndof*i);
	   fpointy = *(coord+nsd*i+1) + *(U+ndof*i+1);
	   fprintf( o3, "%4d %14.6f %14.6f\n", i, fpointx, fpointy);	
	}	

	fprintf(o3, "prescribed displacement x: node disp value \n");
	for(i = 0; i < numnp; i++) {
		fprintf( o3, "%4d %14.6e\n", i, *(U+ndof*i));
	}
	fprintf( o3, " -10\n");	
	
	fprintf(o3, "prescribed displacement y: node disp value \n");
	for(i = 0; i < numnp; i++) {
		fprintf( o3, "%4d %14.6e\n", i, *(U+ndof*i+1));
	}
	fprintf( o3, " -10\n");	
/*
	fprintf( o3, "node with point load and load vector in x, y\n");
	for( i = 0; i < bc.num_force[0]; i++) {
		node              = bc.force[i];
		*(id+ndof*node)   = -1;
		*(id+ndof*node+1) = -1;		
	}
        
	for( i = 0; i < numnp; ++i )
        {
           if( *(id+ndof*i) < 0 || *(id+ndof*i+1) < 0 )
           {
           	fprintf( o3,"%4d",i);
           	for( j = 0; j < ndof; ++j )
           	{
               		fprintf( o3," %16.4e ",*(force+ndof*i+j));
           	}
           	fprintf( o3, "\n");
           }
        }
        fprintf( o3, " -10\n");
*/
        fprintf( o3, "node no. with stress ");
        fprintf( o3, "and stress vector in xx,yy,xy \n");
	for( i = 0; i < numnp; ++i )
	{
		fprintf( o3,"%4d  ",i);
		fprintf( o3,"%14.6e ",stress_node[i].xx);
		fprintf( o3,"%14.6e ",stress_node[i].yy);
		fprintf( o3,"%14.6e ",stress_node[i].xy);
		fprintf( o3, "\n");
	}
        fprintf( o3, " -10 \n");
        fprintf( o3, "node no. with stress ");
        fprintf( o3, "and principal stress I,II \n");
	for( i = 0; i < numnp; ++i )
	{
		fprintf( o3,"%4d  ",i);
		fprintf( o3,"%14.6e ",stress_node[i].I);
		fprintf( o3,"%14.6e ",stress_node[i].II);
		fprintf( o3, "\n");
	}
        fprintf( o3, " -10 \n");
		fprintf( o3, "node no. with vonMieses ");
		fprintf( o3, "and vonMieses magnitude \n");
	
	for(i=0;i<numnp;i++)
	{
		fprintf( o3,"%4d  ",i);
		fprintf( o3,"%14.6e \n ",vonMieses[i]);
	}
		fprintf( o3, " -10 \n");
        fprintf( o3, "node no. with strain ");
        fprintf( o3, "and strain vector in xx,yy,xy \n");
	for( i = 0; i < numnp; ++i )
	{
		fprintf( o3,"%4d  ",i);
		fprintf( o3,"%14.6e ",strain_node[i].xx);
		fprintf( o3,"%14.6e ",strain_node[i].yy);
		fprintf( o3,"%14.6e ",strain_node[i].xy);
		fprintf( o3, "\n");
	}
        fprintf( o3, " -10 \n");
        fprintf( o3, "node no. with strain ");
        fprintf( o3, "and principal strain I,II \n");
	for( i = 0; i < numnp; ++i )
	{
		fprintf( o3,"%4d  ",i);
		fprintf( o3,"%14.6e ",strain_node[i].I);
		fprintf( o3,"%14.6e ",strain_node[i].II);
		fprintf( o3, "\n");
	}
        fprintf( o3, " -10 \n");
		fclose(o3);

return 0;
}
