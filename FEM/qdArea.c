

#include "qdconst.h"


double qdArea(double *coord_el_trans){

    double x1 = *(coord_el_trans+1)-*(coord_el_trans+0);
    double x2 = *(coord_el_trans+3)-*(coord_el_trans+0);

    double y1 = *(coord_el_trans+npel+1)-*(coord_el_trans+npel+0);
    double y2 = *(coord_el_trans+npel+3)-*(coord_el_trans+npel+0);

    //cross product of two vectors in the xy plane gives area directly.//
    double croffPr = (x1)*(y2)-(y1)*(x2);

/*
     printf("x1 = %lf, x2 = %lf, y1 = %lf, y2 = %lf\n",x1,x2,y1,y2);
     printf("Area inside = %lf\n",croffPr);

*/


    return croffPr;

    

}