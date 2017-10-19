#include "point.h"
#include "common/floating_point.h"

Point::Point(){}

Point::Point(double _x,double _y){
  x = _x, y = _y;
}

bool Point::operator<(Point o)const{
  return cmpf(y,o.y) ? y < o.y : x < o.x;
}

bool Point::operator==(Point o)const{
  return !cmpf(x,o.x) && !cmpf(y,o.y);
}