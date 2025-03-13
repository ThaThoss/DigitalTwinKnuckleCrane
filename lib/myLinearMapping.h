#ifndef MYLINMAPP
#define MYLINMAP

#include <stdint.h>

class myLinearMapping
{
private:
    double a;
    double y0;
    int isa0 = 0;
public:
    myLinearMapping(int16_t minRef, int16_t maxRef, double minOut, double maxOut );

    double map(int16_t valToMap);
};



#endif