#include <chrono>
#include <thread>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <cstdio>

#include "myTimeFunctions.h"
#define BILLION  1000000000L

PreciseSleep::PreciseSleep() {
    using namespace std;
    using namespace std::chrono;

    struct timespec preTime, realTime, startTime, dtStart;

    preTime = (struct timespec){0};
    realTime = (struct timespec){0};
    startTime = (struct timespec){0};
    dtStart = (struct timespec){0};



}

int PreciseSleep::startClock(){
    if( clock_gettime( CLOCK_REALTIME, &startTime) == -1 ){
      printf( "clock gettime error" );
      return 1;
    }
    preTime.tv_sec = startTime.tv_sec;
    preTime.tv_nsec = startTime.tv_nsec;

    dtStart.tv_sec = startTime.tv_sec;
    dtStart.tv_nsec = startTime.tv_nsec;

    return 0;
}

int PreciseSleep::sleepTillTimeStep(double seconds){
    using namespace std;
    using namespace std::chrono;

    estimate = 5e-3;
    mean = 5e-3;
    m2 = 0;
    count = 1;

    if( clock_gettime( CLOCK_REALTIME, &realTime) == -1 ){
      printf( "clock gettime" );
      return 1;
    }

    observed = ( realTime.tv_sec - preTime.tv_sec )
        + (double)( realTime.tv_nsec - preTime.tv_nsec ) / (double)BILLION;
    //observed = (end - preTime).count() / 1e9;

    seconds -= observed;

    while (seconds > estimate) {

        clock_gettime( CLOCK_REALTIME, &preTime);
        this_thread::sleep_for(milliseconds(1));
        clock_gettime( CLOCK_REALTIME, &realTime);

        observed = ( realTime.tv_sec - preTime.tv_sec )
        + (double)( realTime.tv_nsec - preTime.tv_nsec ) / (double)BILLION;
        seconds -= observed;

        ++count;
        delta = observed - mean;
        mean += delta / count;
        m2   += delta * (observed - mean);
        stddev = sqrt(m2 / (count - 1));
        estimate = mean + stddev;
    }

    // spin lock
    auto start = high_resolution_clock::now();
    while ((high_resolution_clock::now() - start).count() / 1e9 < seconds);

    if( clock_gettime( CLOCK_REALTIME, &preTime) == -1 ){
      printf( "clock gettime ERROR" );
      return 2;
    }
    clock_gettime( CLOCK_REALTIME, &realTime);

    dt = ( realTime.tv_sec - dtStart.tv_sec ) + 
    (double)( realTime.tv_nsec - dtStart.tv_nsec ) / (double)BILLION;

    if( clock_gettime( CLOCK_REALTIME, &dtStart) == -1 ){
      printf( "clock gettime" );
      return 1;
    }

    return 0;
}

double PreciseSleep::getTotalDuration(){

  if( clock_gettime( CLOCK_REALTIME, &realTime) == -1 ){
    printf( "clock gettime" );
    return 1.0;
  }
  double timeSinceStart = ( realTime.tv_sec - startTime.tv_sec )
      + (double)( realTime.tv_nsec - startTime.tv_nsec ) / (double)BILLION;

  return timeSinceStart;

}

double PreciseSleep::getLastTimeStep(){
  return dt;
}