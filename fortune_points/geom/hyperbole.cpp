#include "hyperbole.h"
#include "debug.h"

#include <stdio.h>

Hyperbole::Hyperbole(){
}

Hyperbole::Hyperbole(Point p, Point q, double a){
  double a2 = a*a;
  double b = 2.*p.x - 2.*q.x;
  double c = 2.*p.y - 2.*q.y;
  double d = q.x*q.x + q.y*q.y -(p.x*p.x + p.y*p.y + a2);

  this->A = 4.*a2 - b*b;
  this->B = -2.*b*c;
  this->C = 4.*a2 - c*c;
  this->D = -8.*a2*p.x - 2.*b*d;
  this->E = -8.*a2*p.y - 2.*c*d;
  this->F = 4.*a2*p.x*p.x + 4.*a2*p.y*p.y - d*d;
  if(DEBUG) printf("%lfx^2 %lfxy %lfy^2 %lfx %lfy %lf\n",A,B,C,D,E,F);
}