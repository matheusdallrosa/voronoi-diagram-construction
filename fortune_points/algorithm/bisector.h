#pragma once

#include "fortune_points/geom/line.h"
#include "fortune_points/diagram/site.h"
#include "fortune_points/geom/bhaskara.h"

#define MINUS 0
#define PLUS 1
#define FAKE_BISECTOR 2

struct Bisector{
  Bisector();
  /*
    Position of the event in which this bisector is being created.
  */
  double ly,xbase;

  /* pm := MINUS | PLUS | FAKE_BISECTOR */
  int pm;

  /*ID.*/
  int id;

  /*Line that represents this bisector.*/
  Line l;

  /*
    The expression:
    s0.y < s1.y || (s0.y = s1.y && s0.x < s1.x)
    will always be true.
  */
  Site s0,s1;
  Bisector(int _id,int _pm,Site s0,Site s1,Point);

  bool vertical()const;

  Bhaskara_Solution horiz_intercep(double ly)const;

  double xcoord(double ly)const;

  bool mypoint(double x)const;

  void print()const;
};

struct Bisector_cmp{
  bool operator()(Bisector a,Bisector b);
};
