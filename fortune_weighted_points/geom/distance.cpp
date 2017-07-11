#include <math.h>

#include "distance.h"

Distance::Distance(){}

double Distance::sq_two_points(Point a,Point b){
  double dx = a.x-b.x;
  double dy = a.y-b.y;
  return dx*dx + dy*dy; 
}

double Distance::two_points(Point a,Point b){
  double dx = a.x-b.x;
  double dy = a.y-b.y;
  return sqrt(dx*dx+dy*dy);
}