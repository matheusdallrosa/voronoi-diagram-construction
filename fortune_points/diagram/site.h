#pragma once

#include "geom/point.h"

struct Site : Point{
  int id;
  Site();
  Site(int _id,double x,double y); 
  virtual void print();
};