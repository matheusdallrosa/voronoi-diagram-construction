#include <math.h>

#include "fortune_weighted_points/common/floating_point.h"

#include "bhaskara.h"

Bhaskara::Bhaskara(){}

/*
  Este metódo se utiliza das equações mostradas no paper:
  What every computer scientist should know about floating-point arithmetic.
*/
dd Bhaskara::solve(double a,double b,double c){
  double b2 = b*b;
  double delta = b2 - 4.*a*c;
  double delta_rt = (cmpf(delta,0.) == 1) ? sqrt(delta) : 0.;
  double den = 2.*a;
  if(4.*b2 > a*c){
    return (cmpf(b,0) == 1) ? dd(-(b+delta_rt)/(den),(-2.*c)/(b+delta_rt))
                            : dd((2.*c)/(-b+delta_rt),(-b+delta_rt)/den);
  }
  return dd(-(b+delta_rt)/den,(-b+delta_rt)/den);
}

Bhaskara_Equation Bhaskara::equation(double a, double b, double c){
  return Bhaskara_Equation(b*b-4.*a*c,b,2.*a);
}

Bhaskara_Equation::Bhaskara_Equation(double a, double _b, double c){
  delta = a, b = _b, two_times_a = c;
}
