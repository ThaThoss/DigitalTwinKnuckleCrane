

#include <stddef.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

#define MIN(x,y) (((x)<(y))?(x):(y)) 
#define MAX(x,y) (((x)>(y))?(x):(y))

int diag( int *idiag, int *lm, int ndof, int neqn, int npel, int numel) 
{ 
    
    int i,j,k,m,node,minEqNum,eqNum;

    for( i = 0; i < neqn; ++i ) 
    {
        *(idiag+i)=i;	
    }

    for( k = 0; k < numel; ++k ) 
    {
        minEqNum=neqn;

        for( j = 0; j < npel; ++j ) 
        {

            for( i = 0; i < ndof; ++i ) 
            {

                eqNum = *(lm + ndof*npel*k + ndof*j + i);

                if(eqNum > -1 ) {
                    minEqNum = MIN(minEqNum, eqNum);
                }
            }
        }


        for( j = 0; j < npel; ++j )  
        {
			for( i = 0; i < ndof; ++i )  
            {

				eqNum=*(lm + ndof*npel*k + ndof*j + i);
				
                if(eqNum > -1 )
                {
					*(idiag+eqNum) = MIN( *(idiag+eqNum), minEqNum);
				}
			}
             
        }
    } 

    for( i = 1; i < neqn; ++i )   
    {
		        *(idiag+i) = i + 1 + *(idiag+i-1) - *(idiag+i) ;	
    }
	return 0;
}


   


