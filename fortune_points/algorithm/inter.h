#pragma once

#include "bisector.h"

struct Inter : Point{
  Bisector b0,b1;
  double star_val,non_mapp_y;
  Inter();
  Inter(Bisector,Bisector,double x,double y,double star);
  void print();
};