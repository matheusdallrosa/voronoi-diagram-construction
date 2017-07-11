#ifndef edge_header
#define edge_header

#include "geom/hyperbole.h"
#include "diagram/wsite.h"

class Edge{
public:
  int id;
  bool is_vertical;
  double vertical_line;
  Hyperbole hyperbole;
  WSite p,q;
  Edge(int,double,WSite,WSite);
  Edge(int,Hyperbole,WSite,WSite);
};

#endif