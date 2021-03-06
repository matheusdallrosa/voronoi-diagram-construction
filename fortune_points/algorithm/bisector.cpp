#include "bisector.h"
#include "debug.h"
#include "common/floating_point.h"

#include <algorithm>
#include <stdio.h>

Bisector::Bisector(){}

Bisector::Bisector(int _id,int _pm,Site s0,Site s1,Point loc){
  this->id = _id;
  this->pm = _pm;
  this->ly = loc.y;
  this->xbase = loc.x;
  if(s0 < s1){
    this->s0 = s0;
    this->s1 = s1;
  }
  else{
    this->s0 = s1;
    this->s1 = s0;
  }

  //computing the bisector of s0 and s1.
  this->l = Line(Point(s0.y,-s0.x),Point(s1.y,-s1.x));
  this->l.translate(Point((s0.x+s1.x)/2., (s0.y+s1.y)/2.));

  if(DEBUG){
    printf("\nNew bisector: \n");
    print();
  }
}

bool Bisector::mypoint(double x)const{
  if(this->vertical()) return !cmpf(this->l.c,-x);
  if(this->pm == MINUS) return cmpf(x,this->xbase) <= 0;
  /*
  If a vertice belongs to this bisector, and this bisector
  is PLUS we are gonna consider that the base belongs to this
  bisector.
  */
  int diff = cmpf(x,this->xbase);
  return cmpf(this->xbase,this->s1.x) ? (diff >= 0) : (diff == 1);
}

Bhaskara_Solution Bisector::horiz_intercep(double ly)const{
  if(vertical()) return Bhaskara_Solution(- this->l.c, - this->l.c);
  double m = - this->l.a / this->l.b;
  double bl = - this->l.c / this->l.b;
  double qy = this->s1.y, qx = this->s1.x;
  double c = 2*bl*(ly-qy) + qy*qy + qx*qx - ly*ly;
  double b = 2*(m*(ly-qy) - qx);
  return Bhaskara::solve(1,b,c);
}

double Bisector::xcoord(double ly)const{
  Bhaskara_Solution yinter = this->horiz_intercep(ly);
  return (this->pm == PLUS) ? yinter.rt1 : yinter.rt0;
}

void Bisector::print()const{
  printf("Id: %d\n",id);
  printf("Side: %s\n",pm == FAKE_BISECTOR ? "FAKE" : (pm ? "PLUS" : "MINUS") );
  printf("Sites: %d - %d | Base:%.2lf\n",s0.id,s1.id,xbase);
  printf("Line equation: %.2lf %.2lf %.2lf\n\n",this->l.a,this->l.b,this->l.c);
}

bool Bisector::vertical()const{
  return l.is_vertical();
}

bool Bisector_cmp::operator()(Bisector a,Bisector b){
  if(a.id == b.id) return a.pm < b.pm;
  double ly = std::max(a.ly,b.ly);
  int diff = cmpf(a.xcoord(ly),b.xcoord(ly));
  if(diff) return (diff == -1);
  if(a.vertical()) return true;
  if(b.vertical()) return false;
  if(a.pm != b.pm) return a.pm < b.pm;
  diff = cmpf(a.xbase, b.xbase);
  return (diff == -1);
}
