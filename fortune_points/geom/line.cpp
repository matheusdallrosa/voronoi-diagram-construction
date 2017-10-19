#include "line.h"
#include "common/floating_point.h"

Line::Line(){}

Line::Line(double _a, double _b, double _c){
  a = _a,b = _b,c = _c;
}

Line::Line(Point p0,Point p1){
  if(!cmpf(p0.x,p1.x)){
    this->a = 1.;
    this->b = 0.;
    this->c = -p0.x;
  }
  else{
    this->a = -(p0.y - p1.y) / (p0.x - p1.x);
    this->b = 1.;
    this->c = -(this->a * p0.x) - p0.y;
  }
}

bool Line::parallel(Line l)const{
  return !cmpf(a,l.a) && !cmpf(b,l.b);
}

double Line::relate(Point p){
  return a*p.x + b*p.y + c;
}

bool Line::is_vertical()const{
  return (cmpf(b,0.) == 0);
}

void Line::translate(Point p){
  this->c = -(p.x*this->a + p.y*this->b);
}

Point Line::operator==(Line o)const{
  double x = (o.b*c - b*o.c)/(o.a*b - a*o.b);
  double y = cmpf(b,0.) ? -(a*x + c) : -(o.a*x + o.c);
  return Point(x,y);
}
