#ifndef point
#define point
class Point{
public:
  double x,y;
    
  Point();
  /*
  first double input param: x coordinate.
  second double input param: y coordinate.
  */
  Point(double,double);  
  bool operator<(Point)const;
  bool operator==(Point)const;
};
#endif