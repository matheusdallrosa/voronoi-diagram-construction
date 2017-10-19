#ifndef inter_header
#define inter_header

#include <vector>

#include "boundary.h"

#include "fortune_weighted_points/diagram/wsite.h"

#include "common/define.h"

#include "fortune_weighted_points/geom/point.h"

class Inter : public Point{
  ii idx;
  Boundary Cqr, Crs;
public:
  /*Círculos tangentes ao círculo com centro nesta intersecção.*/
  std::vector<WSite> tangent;
  double mapp_dist_squared, mapp_weight, mapp_y;
  Inter();
  Inter(Boundary,Boundary,double,double);
  ii get_idx_comp();
  Boundary get_Cqr()const;
  Boundary get_Crs()const;
  void print()const;
  friend class InterComp;
};

class InterComp{
public:
  bool operator()(Inter,Inter);
};
#endif
