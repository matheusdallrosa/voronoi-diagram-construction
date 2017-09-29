#include "fortune_points/geom/line.h"
#include "fortune_points/geom/point.h"
/*
  Structs to store the vertices and edges of the diagram.
  A DCEL should be more suitable.
*/
struct Vertice : Point{
  int id;
  int b0,b1,b2;
  Vertice();
  Vertice(double,double,int,int,int);
};

struct Edge{
  int id,s0,s1;
  Line l;
  Edge();
  Edge(int,Line,int,int);
};
