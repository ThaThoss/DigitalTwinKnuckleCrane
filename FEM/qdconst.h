#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define    pt5      0.5
#define    pt25     0.25
#define    pt1667   0.166666666667
#define    sq3      1.732050808

#define nsd         2                   /* spatial dimensions per node    */
#define ndof        2                   /* degrees of freedom per node    */
#define npel        4                   /* nodes per element              */
#define neqel       npel*ndof           /* degrees of freedom per element */
#define num_int     4                   /* number of integration points   */
#define neqlsq      neqel*neqel         /* neql squared                   */
#define sdim        3                   /* stress dimensions per element  */
#define soB         sdim*neqel          /* size of B matrix               */
#define MsoB        nsd*neqel           /* size of B_mass matrix          */
#define sosh       (nsd+1)*npel*num_int /* size of shl and shg matrix     */
#define sosh_node2  nsd*2*num_int       /* size of shl_node2              */
#define KB          1024.0              /* number of bytes in a kilobyte  */
#define MB          1.0486e+06          /* number of bytes in a megabyte  */

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y)) 

