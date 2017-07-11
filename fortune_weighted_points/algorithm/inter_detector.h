#ifndef inter_detector_header
#define inter_detector_header
#include <vector>

#include "algorithm/boundary.h"
#include "algorithm/inter.h"

#include "diagram/wsite.h"

#include "geom/apollonius_circle.h"
#include "geom/point.h"

class InterDetector{
private:
  InterDetector();
public:
  static std::vector<Inter> detect(Boundary,Boundary,double);
};
#endif