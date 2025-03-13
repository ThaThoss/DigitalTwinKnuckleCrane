#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "qdconst.h"
#include "qdstruct.h"

extern int      numel, numnp, plane_stress_flag, sof;
extern double   shg[sosh], shg_node[sosh], shl[sosh], shl_node[sosh];
extern double   w[num_int];

int qdKassembleStress( int *connect, double *coord, int *el_matl,	
	MATL *matl, double *node_counter, double *U, STRAIN *strain,
    SDIM *strain_node, STRESS *stress, SDIM *stress_node, double *vonMieses) {
	
	int    i1, i2, k, j, check, node;
    int    dof_el[neqel]; 
	int    matl_num;

	double Emod, Pois, G;
	double lamda, mu;
    double B[soB], DB[soB]; 
	double D11, D12, D21, D22;
    double K_temp[neqlsq], K_el[neqlsq], U_el[neqel], force_el[neqel]; 
	double coord_el_trans[npel*nsd];
	double det[num_int], wXdet;
	double stress_el[sdim], strain_el[sdim], xxaddyy, xxsubyy, xysq;

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
*                        	lambda = (Emod*pois)/((1+pois)(1-2*pois))                    *
*                                   mu = Emod/(2*(1+pois))                               *
*                                                                                        *
*                        Plane Stress:                                                   * 
*                        	lambda = (Emod*pois)/(1-pois^2)                              * 
*                                   mu = Emod/(2*(1+pois))                               *
*                        	                                                             *
*                                   mu = G = Shear Modulus                               *
*                                                                                        * 
*****************************************************************************************/
		matl_num = *(el_matl + k);
		Emod     = matl[matl_num].E;
        Pois     = matl[matl_num].nu;
		mu       = Emod/(1.0+Pois)/2.0;

		
		lamda = Emod*Pois/((1.0+Pois)*(1.0-2.0*Pois));
		
		if(plane_stress_flag) 
             		lamda = Emod*Pois/(1.0-Pois*Pois);          

		D11 = lamda + 2.0 * mu;
		D12 = lamda;
		D21 = lamda;
		D22 = lamda + 2.0 * mu;	
        G = mu;
	        
		for( j = 0; j < npel; j++ ) {
                	
			node = *(connect + npel*k + j);

			*(coord_el_trans+j)        = *(coord + nsd*node); 
			*(coord_el_trans+npel*1+j) = *(coord + nsd*node + 1);
			
			*(dof_el + ndof*j)     = ndof * node;
			*(dof_el + ndof*j + 1) = ndof * node + 1;

		}
	
		
		/* Assembly of the shg matrix for each integration point 
		check = qdshg(det, k, shl, shg, coord_el_trans);
		if(!check) printf( "Problems with qdshg in qdkasmblStress \n");

		memset(U_el    , 0, neqel*sof);
		memset(K_el    , 0, neqlsq*sof);
		memset(force_el, 0, neqel*sof);

	*/	//for( j = 0; j < num_int; j++) {
/****************************************************************************************
*                                                                                       *
*                            Element Stiffness Matrix Setup                             *
*                                    lines 101 - 128                                    *
*                                                                                       *
****************************************************************************************/
			/*memset(B     , 0, soB*sof);
			memset(DB    , 0, soB*sof);
			memset(K_temp, 0, neqlsq*sof);
		*/
/* Assembly of the B matrix 
			check = quadB(B, (shg+npel*(nsd+1)*j));	
			if(!check) printf( "Problems with quadB \n");	
	*/
/* Set up DB matrix 
			for( i1 = 0; i1 < neqel; i1++) {
				
				*(DB+i1) = *(B+i1)*D11 +
					*(B+neqel*1+i1)*D12;
				*(DB+neqel*1+i1) = *(B+i1)*D21 +
					*(B+neqel*1+i1)*D22;
				*(DB+neqel*2+i1) = *(B+neqel*2+i1)*G;	
			}		
			
			wXdet = *(w+j)*(*(det+j));
*/
/* Calculate K at current gauss pt }
			check = matXT(K_temp, B, DB, neqel, neqel, sdim);
			if(!check) printf( "Problems with matXT \n");
			
		for( i2 = 0; i2 < neqlsq; i2++) {
				*(K_el+i2) += *(K_temp+i2) * wXdet;
			}	
		}
		*/	
		for( j = 0; j < neqel; j++) {
			*(U_el + j) = *(U + *(dof_el+j));
		}	
/*
		check = matX( force_el, K_el, U_el, neqel, 1, neqel);
		if(!check) printf( "Problems with matX \n");	
*/	
/* Calculate the element reaction forces 
		for( j = 0; j < neqel; j++) {
			
			*(force + *(dof_el+j)) += *(force_el + j);
		}
*/
/* Calculate shg at nodal point */	
		check = qdshg(det, k, shl_node, shg_node, coord_el_trans);
		if(!check) printf( "Problems with qdshg \n");	
	
		for( j = 0; j < npel; j++) {
				
			memset(B, 0, soB*sof);
			memset(stress_el, 0, sdim*sof);
			memset(strain_el, 0, sdim*sof);

			node = *(connect+npel*k+j);

/* Calculate B matrix at nodal point */
			check = quadB(B, (shg_node+npel*(nsd+1)*j));
			if(!check) printf( "Problems with quadB \n");	   
	
/* Calculation of the local strain matrix */
			check=matX(strain_el, B, U_el, sdim, 1, neqel);
			if(!check) printf( "Problems with matX\n");

/* Update of the global strain matrix */
			strain[k].pt[j].xx = *(strain_el);
			strain[k].pt[j].yy = *(strain_el+1);
			strain[k].pt[j].xy = *(strain_el+2);
		
/* Calculate principle strains */
			xxaddyy = 0.5*(strain[k].pt[j].xx + strain[k].pt[j].yy);
			xxsubyy = 0.5*(strain[k].pt[j].xx - strain[k].pt[j].yy);
			xysq    = strain[k].pt[j].xy * strain[k].pt[j].xy;

			strain[k].pt[j].I  = xxaddyy + sqrt( xxsubyy*xxsubyy
				+ xysq);
			strain[k].pt[j].II = xxaddyy - sqrt( xxsubyy*xxsubyy
				+ xysq);

/* Add all the strains for a particular node from all the elements which share that node */
			strain_node[node].xx += strain[k].pt[j].xx;
			strain_node[node].yy += strain[k].pt[j].yy;
			strain_node[node].xy += strain[k].pt[j].xy;
			strain_node[node].I  += strain[k].pt[j].I; 
			strain_node[node].II += strain[k].pt[j].II;

/* Calculation of the local stress matrix */ 
			*(stress_el)   = strain[k].pt[j].xx*D11 + 
			strain[k].pt[j].yy*D12;
			*(stress_el+1) = strain[k].pt[j].xx*D21 +
			strain[k].pt[j].yy*D22;
			*(stress_el+2) = strain[k].pt[j].xy*G;

/* Update of the global stress matrix */
				stress[k].pt[j].xx += *(stress_el);
			stress[k].pt[j].yy += *(stress_el + 1);
			stress[k].pt[j].xy += *(stress_el + 2);
//printf("stress_el for el nr %d, node nr %d = [%lf, %lf, %lf] \n",k,node,*(stress_el + 0),*(stress_el + 1),*(stress_el + 2));

/* Calculate the principal stresses */
			xxaddyy = 0.5*(stress[k].pt[j].xx + stress[k].pt[j].yy);
			xxsubyy = 0.5*(stress[k].pt[j].xx - stress[k].pt[j].yy);
			xysq    = stress[k].pt[j].xy * stress[k].pt[j].xy;
		
			stress[k].pt[j].I  = xxaddyy + sqrt( xxsubyy*xxsubyy
				+ xysq);
			stress[k].pt[j].II = xxaddyy - sqrt( xxsubyy*xxsubyy
				+ xysq);
		
/* Add all the stresses for a particular node from all the elements which share that node */
			stress_node[node].xx += stress[k].pt[j].xx;
			stress_node[node].yy += stress[k].pt[j].yy;
			stress_node[node].xy += stress[k].pt[j].xy;
			stress_node[node].I  += stress[k].pt[j].I; 
			stress_node[node].II += stress[k].pt[j].II;
	//printf("stress_node for node nr %d = [%lf, %lf, %lf] \n",node,stress_node[node].xx,stress_node[node].yy,stress_node[node].xy);

		}//for(j=0;j<npel;j++);		
		
	
    }

/* Average all the stresses and strains at the nodes */

    for( j = 0; j < numnp; j++) {
		        
        strain_node[j].xx /= *(node_counter + j);
        strain_node[j].yy /= *(node_counter + j);
        strain_node[j].xy /= *(node_counter + j);
        strain_node[j].I  /= *(node_counter + j);
        strain_node[j].II /= *(node_counter + j);
    
        stress_node[j].xx /= *(node_counter + j);
        stress_node[j].yy /= *(node_counter + j);
        stress_node[j].xy /= *(node_counter + j);
        stress_node[j].I  /= *(node_counter + j);
        stress_node[j].II /= *(node_counter + j);
    }

	/*Calculate vonMiese at the nodal points*/
	double vonMiesesSq = 0.0;
	for(j=0;j<numnp;j++){
		vonMiesesSq = stress_node[j].xx*stress_node[j].xx-stress_node[j].xx*stress_node[j].yy+stress_node[j].yy*stress_node[j].yy + 3*stress_node[j].xy*stress_node[j].xy;
		vonMieses[j] = sqrt(vonMiesesSq);
	}

	
	return 1;

}