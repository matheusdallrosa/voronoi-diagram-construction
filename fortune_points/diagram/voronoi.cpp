#include "voronoi.h"

Vertice::Vertice(double x,double y,int _b0,int _b1,int _b2) : Point(x,y){
  b0 = _b0, b1 = _b1, b2 = _b2;
}

Edge::Edge(int _id,Line _l,int _s0,int _s1){
  id = _id;
  l = _l;
  s0 = _s0;
  s1 = _s1;
}