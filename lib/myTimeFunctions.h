#ifndef MYTIMEFUNC
#define MYTIMEFUNC
#include <chrono>
#include <thread>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <cstdio>

#ifndef BILLION
#define BILLION  1000000000L
#endif


class PreciseSleep{


    public:
        PreciseSleep();
        int sleepTillTimeStep(double dt);
        int startClock();
        double getTotalDuration();
        double getLastTimeStep();



    private:

       

        double estimate = 5e-3;
        double mean = 5e-3;
        double m2 = 0;
        double observed = 0;
        double delta = 0;
        double stddev = 0;
        int64_t count = 1;

        struct timespec preTime, realTime, startTime, dtStart;

        double dt;






};
#endif