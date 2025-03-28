#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int qdSaveInverse(double* inverse,int neqn,  char fileName[]){

	

    char out[30] = {'\0'};
    memset(out, 0, 30*sizeof(char));

    int name_length = strlen(fileName);
	if( name_length > 25) name_length = 25;

	if (strncpy(out, fileName, name_length) == NULL) {
        printf("Problems with strncpy, file name was: %s\n", fileName);
    }
    

	FILE *filePointer = fopen( out, "w");
    if (!filePointer) {
        printf("Failed to open file: %s\n", out);
        return -1;
    }
    


    for(int i=0;i<neqn;i++){
        for(int j=0;j<neqn;j++){            
            fprintf(filePointer, "%14.6e ", *(inverse +i*neqn + j) );	        
        }
        fprintf(filePointer,"\n" );	
    }
    fclose(filePointer);

return 0;

}
