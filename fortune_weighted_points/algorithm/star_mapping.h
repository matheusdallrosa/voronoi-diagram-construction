#ifndef star_mappinh_header
#define star_mapping_header
#include "fortune_weighted_points/geom/hyperbole.h"

#include "common/define.h"

#include "fortune_weighted_points/diagram/wsite.h"

class StarMapping {
public:
  static void map_pp(pp &,WSite);
  static Hyperbole map_bisector(WSite p,WSite q,Hyperbole h);
};
#endif
