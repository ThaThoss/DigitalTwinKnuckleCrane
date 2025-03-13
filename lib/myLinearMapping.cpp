#include <stdio.h>
#include <stdint.h>

#include "myLinearMapping.h"

myLinearMapping::myLinearMapping(int16_t minRef, int16_t maxRef, double minOut, double maxOut )
{
    if((maxRef- minRef) != 0){
        a = (maxOut-minOut)/((double)maxRef - (double)minRef);
    }else{
        printf("a = 0");
        isa0 = 1;
    }
    y0 = minOut;
}

double myLinearMapping::map(int16_t valToMap)
{
    return a*(double)valToMap + y0;
}