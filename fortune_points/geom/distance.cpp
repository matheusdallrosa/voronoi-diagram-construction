#include "distance.h"

#include <math.h>

double Distance::two_points(Point a,Point b){
  double dx = a.x-b.x;
  double dy = a.y-b.y;
  return sqrt(dx*dx+dy*dy);
}