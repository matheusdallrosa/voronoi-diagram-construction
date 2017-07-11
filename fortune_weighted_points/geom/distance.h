#ifndef distance_header
#define distance_header
#include "point.h"

class Distance{
private:
  Distance();
public:
  static double two_points(Point,Point);
  
  static double sq_two_points(Point,Point);
};
#endif