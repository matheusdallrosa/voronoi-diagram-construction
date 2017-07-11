#ifndef vertice_header
#define vertice_header

#include <vector>

#include "geom/point.h"

class Vertice : public Point{  
public:
  int id;
  std::vector<int> edges;//edges.size() >= 3 sempre.
  Vertice(int,std::vector<int>,double,double);
};

#endif 