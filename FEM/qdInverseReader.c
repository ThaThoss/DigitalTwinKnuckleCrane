
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int qdInverseReader(double* inv, int neqn, char fileName[]) {

    char out[30] = {'\0'};
    memset(out, 0, sizeof(out));

    int name_length = strlen(fileName);
    if (name_length > 25) name_length = 25;

    if (strncpy(out, fileName, name_length) == NULL) {
        printf("Problems with strncpy, file name was: %s\n", fileName);
    }

    FILE *filePointer = fopen(out, "r");
    if (!filePointer) {
        printf("Failed to open file: %s\n", out);
        return -1;
    }

    for (int i = 0; i < neqn; i++) {
        for (int j = 0; j < neqn; j++) {
            if (fscanf(filePointer, "%lf", inv + i * neqn + j) != 1) {
                printf("Read error at position [%d][%d]\n", i, j);
                fclose(filePointer);
                return -2;
            }
        }
    }

    fclose(filePointer);
    return 0;
}
