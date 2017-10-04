#ifndef status_header
#define status_header
#include <set>

#include "boundary.h"
#include "index.h"

#include "fortune_weighted_points/geom/point.h"

typedef std::pair<Boundary,Boundary> closest_boundaries;

class Status{
  std::set<Boundary,StarBoundaryComp> status;
  typedef std::set<Boundary,StarBoundaryComp>::iterator status_position;

  std::set<Index<status_position> > idx;
  typedef std::set<Index<status_position> >::iterator idx_position;
public:
  Status();

  closest_boundaries closest_bound(Point);

  void add_boundary(Boundary);

  void rmv_boundary(Boundary);

  closest_boundaries neighbors(Boundary);

  void print();
};
#endif
