#ifndef bhaskara
#define bhaskara
#include "common/define.h"

struct Bhaskara_Equation{
  double delta,b,two_times_a;
  Bhaskara_Equation(double,double,double);
};

class Bhaskara{
private:
  Bhaskara();
public:
  /*
  primeiro parametro: coefficient of x^2
  segundo parametro: coefficient of x
  terceiro parametro: independent term
  */
  static dd solve(double,double,double);
  
  /*
    Retorna uma equacao:
    0 = (-b +- sqrt(delta) ) / 2*a
  */
  static Bhaskara_Equation equation(double,double,double);
};
#endif