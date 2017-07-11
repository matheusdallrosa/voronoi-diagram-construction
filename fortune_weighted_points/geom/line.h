#ifndef line
#define line
#include "point.h"

class Line{
  double a,b,c;  
  
public:
  Line();
  
  Line(double,double,double);  
  
  Line(Point,Point);   
  
  bool parallel(Line)const;   
  
  Point operator==(Line)const; 
  
  bool is_vertical()const;
  
  void translate(Point);
  
  double getAngular();
  
  double getLinear();
};
#endif