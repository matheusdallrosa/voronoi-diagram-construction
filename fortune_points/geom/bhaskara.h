#pragma once

class Bhaskara_Solution{
public:
  /*rt0 <= rt1 always.*/
  double rt0,rt1;  
  Bhaskara_Solution(double,double);
};

class Bhaskara{
public:
  /*
  first double input param: coefficient of x^2
  second double input param: coefficient of x
  third double input param: independent term
  */
  static Bhaskara_Solution solve(double,double,double);
};