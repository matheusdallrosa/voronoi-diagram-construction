#pragma once

#include "point.h"

class Line{
public:
  double a,b,c;

  Line();

  Line(double,double,double);

  Line(Point,Point);

  bool parallel(Line)const;

  Point operator==(Line)const;

  bool is_vertical()const;

  void translate(Point);

  double relate(Point);
};
