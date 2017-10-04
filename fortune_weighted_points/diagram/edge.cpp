#include "edge.h"

Edge::Edge(int _id,double x,WSite _p,WSite _q){
  this->id = _id;
  this->is_vertical = true;
  this->vertical_line = x;
}

Edge::Edge(int _id,Hyperbole h,WSite p,WSite q){
  this->id = _id;
  this->is_vertical = false;
  this->hyperbole = h;
}
