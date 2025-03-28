#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "qdconst.h"
#include "qdstruct.h"

double qdArea(double *);
int qdshg( double *, int , double *, double *, double *);
int quadB( double *, double *);
int matXT( double *, double *, double *, int , int , int );
int matX( double *, double *, double *, int , int , int );
int globalKassemble( double *, double *, int *, int , int );
int qdCalcForce(double * force, double *tempForce,double *nodalMass, BOUND bc,FORCESTRUCT *forceStruct, int *id);



extern int      analysis_flag, dof, neqn, numel, numnp, plane_stress_flag, sof;
extern int      numel_K, numel_P;
extern double   shg[sosh], shg_node[sosh], shl[sosh], shl_node[sosh];
extern double   shl_node2[sosh_node2], w[num_int];

int qdKassemble( double *A, int *connect, double *coord, int *el_matl, double *force,	
	int  *id, int *lm,  MATL *matl, double *node_counter, double *U, double *nodalMass, double *tempForce,
		FORCESTRUCT *forceStruct, BOUND bc) {
	
	int    i1, i2, k, j, s, check, node;
    int    dof_el[neqel], counter; 
	int    matl_num;

	double Emod, Pois, Density, G;
	double gravity_constant = forceStruct->gravity_constant;

	double lamda, mu;
    double B[soB]={0}, DB[soB]={0}; 
	double D11, D12, D21, D22;
    double K_temp[neqlsq]={0}, K_el[neqlsq]={0}, U_el[neqel]={0}, force_el[neqel]={0.0}; 
	double coord_el_trans[npel*nsd]={0};
	double det[num_int]={0}, wXdet;
	double stress_el[sdim]={0}, strain_el[sdim]={0}, xxaddyy, xxsubyy, xysq;
	double gravityConst = 9.81, elementArea = 0.0;

	int bodyNum = 0;
	if(numel==2460){
		bodyNum = 1;
	}else if(numel==2220){
		
		bodyNum = 2;
	}else if(numel==1328){
		bodyNum=3;
	}



	
	for( k = 0; k < numel; k++ ) {
/*****************************************************************************************
*                                                                                        *	   
*                                Constitutive Matrix Setup                               * 
*                                      Lines 54 - 68                                     * 
*                                                                                        *
*                             |lambda+2*mu     lambda        0 |                         *
*                             |                                |                         * 
*                        D =  |   lambda     lambda+2*mu     0 |                         * 
*                             |                                |                         * 
*                             |      0             0        mu |                         * 
*                                                                                        * 
*                        Plane Strain:                                                   * 
*                        	lambda = (Emod*pois)/((1+pois)(1-2*pois))               	 *
*                                   mu = Emod/(2*(1+pois))                               *
*                                                                                        *
*                        Plane Stress:                                                   * 
*                        	lambda = (Emod*pois)/(1-pois^2)                         	 * 
*                                   mu = Emod/(2*(1+pois))                               *
*                        	                                                      		 *
*                                   mu = G = Shear Modulus                               *
*                                                                                        * 
*****************************************************************************************/

		matl_num = *(el_matl + k);
		Emod     = matl[matl_num].E;
        Pois     = matl[matl_num].nu;
		Density  = matl[matl_num].rho;
		mu       = Emod/(1.0+Pois)/2.0;

		
		lamda = Emod*Pois/((1.0+Pois)*(1.0-2.0*Pois));
		
		if(plane_stress_flag) 
             		lamda = Emod*Pois/(1.0-Pois*Pois);          

		D11 = lamda + 2.0 * mu;
		D12 = lamda;
		D21 = lamda;
		D22 = lamda + 2.0 * mu;	
        G   = mu;
	        
		for( j = 0; j < npel; j++ ) {
                	
			node = *(connect + npel*k + j);

			*(coord_el_trans+j)        = *(coord + nsd*node); 
			*(coord_el_trans+npel*1+j) = *(coord + nsd*node + 1);
			
			*(dof_el + ndof*j)     = ndof * node;
			*(dof_el + ndof*j + 1) = ndof * node + 1;	
	
/* Count the number of times a particular node is part of an element */
			if(analysis_flag == 1)
					*(node_counter + node) += 1.0; 
		}

	
/* Assembly of the shg matrix for each integration point */
		check = qdshg(det, k, shl, shg, coord_el_trans);
		if(!check) printf( "Problems with qdshg \n");

		elementArea = qdArea(coord_el_trans);

		if(elementArea<=0.00001){
			printf("Area of element nr %d, negative or close to zero in bodie %d. Area = %lf\n",k,bodyNum,elementArea);
			printf("coord_el_trans = [%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf]\n",*coord_el_trans,*(coord_el_trans+1),*(coord_el_trans+2),*(coord_el_trans+3),*(coord_el_trans+4),*(coord_el_trans+5),*(coord_el_trans+6),*(coord_el_trans+7));
		}

		memset(U_el    , 0, neqel*sof);
		memset(K_el    , 0, neqlsq*sof);
		memset(force_el, 0, neqel*sof);

		for( j = 0; j < num_int; j++) {
/****************************************************************************************
*                                                                                       *
*                            Element Stiffness Matrix Setup                             *
*                                    lines 101 - 128                                    *
*                                                                                       *
****************************************************************************************/
			memset(B     , 0, soB*sof);
			memset(DB    , 0, soB*sof);
			memset(K_temp, 0, neqlsq*sof);
		
/* Assembly of the B matrix */
			check = quadB(B, (shg+npel*(nsd+1)*j));	
			if(!check) printf( "Problems with quadB \n");	
	
/* Set up DB matrix */	
			for( i1 = 0; i1 < neqel; i1++) {
				
				*(DB+i1) = *(B+i1)*D11 +
					*(B+neqel*1+i1)*D12;
				*(DB+neqel*1+i1) = *(B+i1)*D21 +
					*(B+neqel*1+i1)*D22;
				*(DB+neqel*2+i1) = *(B+neqel*2+i1)*G;	
			}	
			
			wXdet = *(w+j)*(*(det+j));

/* Calculate K at current gauss pt */
			check = matXT(K_temp, B, DB, neqel, neqel, sdim);
			if(!check) printf( "Problems with matXT \n");
			
			for( i2 = 0; i2 < neqlsq; i2++) {
				*(K_el+i2) += *(K_temp+i2) * wXdet;
			}	
		}	
	
		for( j = 0; j < neqel; j++) {
			*(U_el + j) = *(U + *(dof_el+j));
		}	

		check = matX( force_el, K_el, U_el, neqel, 1, neqel);
		if(!check) printf( "Problems with matX \n");	
	
	

/* Compute the equivalent nodal forces based on prescribed displacements */
		for( j = 0; j < neqel; j++){
			*(force + *(dof_el+j)) -= *(force_el + j);
			
		}
		for(j = 0; j<npel;j++){
			node = *(connect + npel*k + j);
			*(nodalMass + node ) += elementArea*Density*gravity_constant;
		}
		
/* Assembly of the global stiffness matrix */
		check = globalKassemble(A, K_el, (lm + k*neqel), neqel, neqn);
		if(!check) printf( "Problems with globalKassemble \n");	
			
	}

	for(j=0;j<numnp;j++){
		*(nodalMass+j) = ( *(nodalMass+j) / *(node_counter+j));
		if(*(node_counter+j)<1){
		printf("nodalMass = %lf, with count = %lf",*(nodalMass+j),*(node_counter+j));
		}
	}

/* Contract the global force matrix using the id array */
	memcpy(tempForce,force,dof*sizeof(double));
	//printf("Før qdCalcForce\n");
	qdCalcForce(force, tempForce, nodalMass,bc, forceStruct, id);
	

	
	return 1;
}
