#include "bhaskara.h"
#include "common/floating_point.h"

#include <math.h>

#include <algorithm>

Bhaskara_Solution::Bhaskara_Solution(double _rt0,double _rt1){
  rt0 = _rt0;
  rt1 = _rt1;
}

Bhaskara_Solution Bhaskara::solve(double a,double b,double c){
  double delta = b*b - 4.*a*c;
  double delta_rt = (cmpf(delta,0.) == 1) ? sqrt(delta) : 0.;
  double rt0 = (-b-delta_rt)/(2.*a);
  double rt1 = (-b+delta_rt)/(2.*a);
  return Bhaskara_Solution(std::min(rt0,rt1),std::max(rt0,rt1));
}