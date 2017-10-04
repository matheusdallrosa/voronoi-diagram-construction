#include <stdio.h>

#include <algorithm>

#include "fortune_weighted_points/common/floating_point.h"
#include "fortune_weighted_points/common/define.h"


#include "hyperbole.h"

#include "debug.h"

/*
  As equações utilizadas podem ser adaptadas para valores maiores,
  só que com um trade-off entre precisão e overflow.
*/

Hyperbole::Hyperbole(){
}

Hyperbole::Hyperbole(double A,double B,double C,double D,double E,double F,AsymptotesDirection _direction){
  this->direction = _direction;
  this->angular = B;
  this->A = A, this->B = B, this->C = C;
  this->D = D, this->E = E, this->F = F;
  normalize();
}

/* euclid_dist(p,z)-euclid_dist(q,z) = a */
Hyperbole::Hyperbole(Point p, Point q, double a){
  double a2 = a*a;
  double b = 2.*p.x - 2.*q.x;
  double c = 2.*p.y - 2.*q.y;
  double d = q.x*q.x + q.y*q.y -(p.x*p.x + p.y*p.y + a2);

  this->A = 4.*a2 - b*b;
  this->B = this->angular = -2.*b*c;
  this->C = 4.*a2 - c*c;
  this->D = -8.*a2*p.x - 2.*b*d;
  this->E = -8.*a2*p.y - 2.*c*d;
  this->F = 4.*a2*p.x*p.x + 4.*a2*p.y*p.y - d*d;
  normalize();
}

/*
  Método para computar uma hipérbole a partir de 5 pontos.
  [q, 1, 4][q, 2, 3][p, 1, 3][p, 2, 4] − [q, 1, 3][q, 2, 4][p, 1, 4][p, 2, 3] = 0
*/
Hyperbole::Hyperbole(Point q, Point p1, Point p2, Point p3, Point p4,AsymptotesDirection _direction){
  this->direction = _direction;
  /* Determinantes [q, 1, 4][q, 2, 3]: */
  double det1 = q.y*(p4.x-p1.x) + q.x*(p1.y-p4.y) + p1.x*p4.y - p4.x*p1.y;
  double det2 = q.y*(p3.x-p2.x) + q.x*(p2.y-p3.y) + p2.x*p3.y - p3.x*p2.y;
  double det12 = det1*det2;

  /* Reta [p, 1, 3]: */
  double a1 = p3.y-p1.y, b1 = p1.x-p3.x, c1 = p3.x*p1.y - p1.x*p3.y;

  /* Reta [p, 2, 4]: */
  double a2 = p4.y-p2.y, b2 = p2.x-p4.x, c2 = p4.x*p2.y - p2.x*p4.y;

  /* Determinantes [q, 1, 3][q, 2, 4]: */
  double det3 = q.y*(p3.x-p1.x) + q.x*(p1.y-p3.y) + p1.x*p3.y - p3.x*p1.y;
  double det4 = q.y*(p4.x-p2.x) + q.x*(p2.y-p4.y) + p2.x*p4.y - p4.x*p2.y;
  double det34 = det3*det4;

  /* Reta [p, 1, 4]: */
  double a3 = p4.y-p1.y, b3 = p1.x-p4.x, c3 = p4.x*p1.y - p1.x*p4.y;

  /* Reta [p, 2, 3]: */
  double a4 = p3.y-p2.y, b4 = p2.x-p3.x, c4 = p3.x*p2.y - p2.x*p3.y;
  if(cmpf(det34,det12) >= 0){
    this->A = (det12*a1*a2)/det34 - a3*a4;
    this->B = this->angular = (det12*(a1*b2 + b1*a2))/det34 - (a3*b4 + b3*a4);
    this->C = (det12*b1*b2)/det34 - b3*b4;
    this->D = (det12*(a1*c2 + c1*a2))/det34 - (a3*c4 + c3*a4);
    this->E = (det12*(b1*c2 + c1*b2))/det34 - (b3*c4 + c3*b4);
    this->F = (det12*c1*c2)/det34 - c3*c4;
  }
  else{
    this->A = a1*a2 - (det34*a3*a4)/det12;
    this->B = this->angular = (a1*b2 + b1*a2) - (det34*(a3*b4 + b3*a4))/det12;
    this->C = b1*b2 - (det34*b3*b4)/det12;
    this->D = (a1*c2 + c1*a2) - (det34*(a3*c4 + c3*a4))/det12;
    this->E = (b1*c2 + c1*b2) - (det34*(b3*c4 + c3*b4))/det12;
    this->F = c1*c2 - (det34*c3*c4)/det12;
  }
  normalize();
}

/*
  o código pra normalizar não ficou elegante,
  mas foi a melhor maneira até agora.
*/
void Hyperbole::normalize(){
  coef = {A,B,C,D,E,F};
  double mx = 0;
  for(auto a : coef) mx = std::max(mx,std::abs(a));
  for(auto a : coef){
    double abs_a = std::abs(a);
    int normalized = cmpf(abs_a,1);
    if(!normalized) return;
    if(normalized == 1) mx = std::min(mx,abs_a);
  }
  A = coef[0] / mx, B = coef[1] / mx;
  C = coef[2] / mx, D = coef[3] / mx;
  E = coef[4] / mx, F = coef[5] / mx;
}

void Hyperbole::print()const{
  printf("%lfx^2 + %lfxy + %lfy^2 + %lfx + %lfy + %lf = 0\n",A,B,C,D,E,F);
}

pp Hyperbole::y_intersec(double y){
  dd xs = Bhaskara::solve(A,B*y+D,C*y*y + E*y + F);
  return pp(Point(xs.first,y),Point(xs.second,y));
}

Bhaskara_Equation Hyperbole::y_bhaskara_intersec_equation(double y)const{
  return Bhaskara::equation(A,B*y+D,C*y*y + E*y + F);
}

pp Hyperbole::x_intersec(double x)const{
  dd ys = Bhaskara::solve(C,B*x+E,A*x*x + D*x + F);
  return pp(Point(x,ys.first),Point(x,ys.second));
}

SecondDegree Hyperbole::x_intersec_equation(double x)const{
  return SecondDegree({C,B*x+E,A*x*x + D*x + F});
}

Bhaskara_Equation Hyperbole::x_bhaskara_intersec_equation(double x)const{
  return Bhaskara::equation(C,B*x+E,A*x*x + D*x + F);
}

pp Hyperbole::line_intersec(double m, double b){
  double c2 = A + m*(B + C*m);
  double c1 = b*B + D + m*(2*b*C + E);
  double c0 = b*(b*C + E) + F;
  dd xs = Bhaskara::solve(c2,c1,c0);
  return pp(Point(xs.first,m*xs.first+b),Point(xs.second,m*xs.second+b));
}
