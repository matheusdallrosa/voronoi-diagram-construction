#include <stdio.h>

#include "common/floating_point.h"

#include "wsite.h"

WSite::WSite(){}

WSite::WSite(int _id,double x,double y,double _weight) : Point(x,y){
  id = _id;
  weight = _weight;
}

double WSite::get_weight()const{
  return weight;
}

int WSite::get_id()const{
  return id;
}

void WSite::print() const{
  printf("id: %d\n",id);
  printf("Coords: (%.3lf, %.3lf) W:%.3lf\n\n",x,y+weight,weight);
}

/*
  retorna true se p < q.
*/
bool StarWSiteComp::operator()(WSite p,WSite q){
  int diff = cmpf(p.y+p.get_weight(),q.y+q.get_weight());
  return diff ? (diff == -1) : (cmpf(p.x,q.x) == -1);
}
