#ifndef MYMATH_H
#define MYMATH_H


int mapInt(int valToMap,int in_min, int in_max, int out_min, int out_max){


	return (valToMap-in_min) * (out_max-out_min) / (in_max - in_min) + out_min;
}



#endif
