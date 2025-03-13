#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


void crout(double *MatIn, double *L, double *U, int n)
{
	int i, j, k;
	double sum = 0;

	for (i = 0; i < n; i++) 
    {
		*(U + i*n+i) = 1;
        *(L + i*n) = *(MatIn + i*n);
	}

    for(i = 1; i < n; i++)
    {
        *(U + i) = *(MatIn + i) / *MatIn;
    }

	for (j = 0; j < n; j++) 
    {
		for (i = j; i < n; i++) 
        {
			sum = 0;
			for (k = 0; k < j; k++) 
            {
				sum += *(L + i*n+k) * *(U +k*n +j);	
			}
			*(L + i*n +j) = *(MatIn + i*n + j) - sum;
		}

		for (i = j; i < n; i++)
        {
			sum = 0;
			for(k = 0; k < j; k++)
            {
				sum += *(L + j*n + k) * *(U + k*n + i);
			}
			if (*(L + j*n + j) == 0)
            {
				
				fprintf(stderr, "det(L) close to 0!\n Can't divide by 0...\nError in invertMatrix/crout");
				exit(EXIT_FAILURE);
			}
			*(U + j*n +i) = (*(MatIn + j*n + i) - sum) / *(L + j*n +j);
		}
	}
}

// Function to solve equations
void solveCrout(int n, double *b, double *L, double *U)
{
	int i, j;
	float sum;

	for (i = 0; i < n; i++)
    {
        sum = b[i];
        
        for (j = 0; j < i; j++)
        { 
            sum -= *(L + i*n +j)*b[j];
        }
        b[i] = sum / *(L + i*n +i);     
	}

	for (i = n - 1; i >= 0; i--) 
    {
		sum = b[i];

		for(j = i + 1; j < n; j++)
        { 
            sum -= *(U + i*n +j)*b[j];
        }
		b[i] = sum;
	}
}

// Function to transpose a matrix
int transpose(double *b, int m, int n)
{
    double *t = (double*)calloc(n*m,sizeof(double));
    int i, j;

    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            *(t + j*n +i) = *(b + i*n +j);
        }
    }
	memcpy(b,t,m*n*sizeof(double));
	free(t);

    return 0;
}




// Function to find inverse of the matrix
int inverseLUDC(double *L, double *U, double *A, double *inv, int n)
{
    crout(A, L, U, n);

    int i, j;

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            *(inv + i*n +j) = 0;
        }

        *(inv + i*n +i) = 1.0;
    }

    for(i = 0; i < n; i++)
    {
        solveCrout(n, (inv + i*n), L, U);
    }
	
	transpose(inv, n, n);

    return 0;
}


int inverseMatrix(double *inv, double *A, int neqn)
{

	double * L = (double*)calloc(neqn*neqn,sizeof(double));
	double * U = (double*)calloc(neqn*neqn,sizeof(double));
	
	inverseLUDC(L, U, A, inv, neqn);

	free(L);
	free(U);

	return 0;

}

int matvecmult(double* vecOut, double* matIn, double* vecIn, int n, int m){

	for(int i=0;i<n;i++){
		*(vecOut + i) = 0;
		for(int j=0;j<m;j++){
			*(vecOut + i) += *(matIn +i*n + j )*vecIn[j];
		}
	}


	return 0;
}


int solve(double* solutionVec, double *inv, double *A, double *f, int neqn) {

    int check = inverseMatrix(inv,A,neqn);
    if(check) printf( " Problems with inverse in solve \n");
    matvecmult(solutionVec,inv,f,neqn,neqn);
    

	return 1;
}


int solveGauss( double *A, double *f, int *id, int neqn, int dof) {


	int     i, j, k;
	double  factor, sum;


/* Gauss Elimination is used to solve the equation Kd=f */
/* Forward Substitution */
	for( i = 0; i < neqn-1; i++) {
	   for( j = i+1; j < neqn; j++) {
		{	
		    factor = *(A+j*neqn+i) / *(A+i*neqn+i); 
	      	    for( k = i; k < neqn; k++) {
			*(A+j*neqn+k) -= *(A+i*neqn+k) * factor; 
	            }	
		 } 
	      	*(f+j) -= *(f+i) * factor;
	      } 
	   }
	

/* Backward  Substitution */
	*(f+neqn-1) /= *(A+(neqn-1)*neqn+(neqn-1));
	for( i = neqn-2; i > -1; i--) {
            sum = 0.0; 
	    for( j = i+1; j < neqn; j++) {
		 sum += *(A+i*neqn+j) * *(f+j); 	
	    }
	    *(f+i) = (*(f+i) - sum) / *(A+i*neqn+i);
	}	    


	return 1;
}