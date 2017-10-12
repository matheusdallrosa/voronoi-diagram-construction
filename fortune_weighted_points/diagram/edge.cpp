#include "edge.h"

Edge::Edge(int _id,double x,WSite _p,WSite _q){
  this->id = _id;
  this->is_vertical = true;
  this->vertical_line = x;
  this->p = _p, this->q = _q;
}

Edge::Edge(int _id,Hyperbole h,WSite _p,WSite _q){
  this->id = _id;
  this->is_vertical = false;
  this->hyperbole = h;
  this->p = _p, this->q = _q;
}
