#ifndef COMNUM
#define COMNUM
#include <stdint.h>

#define SZ_DOUBLE 8
#define SZ_INT16 2

const int sizeofDouble = sizeof(double);
const int sizeofInt16 = sizeof(int16_t);
const int knuckleNBytesFromKnu = SZ_DOUBLE*48;//R1, angles, angv, angacc, ma and mom, crane tip
const int knuckleNBytesToKnu = SZ_DOUBLE*3 + SZ_INT16*2;


int calcFemNumBytesFromClient(int header[5]);
#endif