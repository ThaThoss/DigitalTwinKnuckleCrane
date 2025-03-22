
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int qdInverseReader(double* inv, int neqn, char fileName[]){

    char out[30] = {'\0'};
    memset(out, 0, 30*sizeof(char));

    int name_length = strlen(fileName);
	if( name_length > 25) name_length = 25;

	int ccheck = strncpy(out, fileName, name_length);
	if(!ccheck) printf( "Problems with strncpy, file name was: %s\n",fileName);

	FILE *filePointer = fopen( out, "w");

    for(int i=0;i<neqn;i++){
        for(int j=0; j<neqn;j++){
            fscanf(filePointer,"%14.6e ",(inv + i*neqn + j));
        }fscanf(filePointer,"\n");
    }




    return 0;
}