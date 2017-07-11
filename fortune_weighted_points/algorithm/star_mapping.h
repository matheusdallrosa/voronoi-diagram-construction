#ifndef star_mappinh_header
#define star_mapping_header
#include "geom/hyperbole.h"

#include "common/define.h"

#include "diagram/wsite.h"

class StarMapping {
public:
  static void map_pp(pp &,WSite);
  static Hyperbole map_bisector(WSite p,WSite q,Hyperbole h);
};
#endif