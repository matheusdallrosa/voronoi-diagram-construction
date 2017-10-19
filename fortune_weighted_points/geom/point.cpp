#include "point.h"

#include "common/floating_point.h"

Point::Point(){}

Point::Point(double _x,double _y){
  x = _x, y = _y;
}

bool Point::operator<(Point o)const{
  int diff = cmpf(y,o.y);
  return diff ? (diff == -1) : (cmpf(x,o.x) == -1);
}

bool Point::operator==(Point o)const{
  return !cmpf(x,o.x) && !cmpf(y,o.y);
}
