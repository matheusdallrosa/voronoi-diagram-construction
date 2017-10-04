#ifndef inter_detector_header
#define inter_detector_header
#include <vector>

#include "boundary.h"
#include "inter.h"

#include "fortune_weighted_points/diagram/wsite.h"

#include "fortune_weighted_points/geom/apollonius_circle.h"
#include "fortune_weighted_points/geom/point.h"

class InterDetector{
private:
  InterDetector();
public:
  static std::vector<Inter> detect(Boundary,Boundary,double);
};
#endif
