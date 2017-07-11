#include "floating_point.h"

#include <math.h>

int cmpf(double a,double b){
  if(fabs(a-b) < EPS) return 0;
  return (a < b) ? -1 : 1;
}