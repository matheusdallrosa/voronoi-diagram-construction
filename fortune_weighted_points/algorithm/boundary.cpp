#include <stdio.h>
#include <math.h>

#include <algorithm>

#include "star_mapping.h"
#include "boundary.h"

#include "fortune_weighted_points/geom/polynomial.h"
#include "fortune_weighted_points/geom/distance.h"

#include "common/floating_point.h"

#include "debug.h"

Boundary::Boundary(){}

Boundary::Boundary(Point p){
  this->base = p;
  this->side = MINUS;
  this->is_vertical_line = false;
}

/*
This constructor must be used when _base is a vertice.
*/
Boundary::Boundary(int _id,WSite _p, WSite _q,Point _base){
  this->id = _id;
  /*
    This:
    p.y + p.get_weight() > q.y + q.get_weight();
    must be always true.
    This condition makes a lot of operations easier.
  */
  if(StarWSiteComp()(_p,_q)) this->p = _q, this->q = _p;
  else this->p = _p, this->q = _q;
  if(cmpf(p.get_weight(),q.get_weight()) || cmpf(p.y,q.y)){
    this->base = this->p;
    this->is_vertical_line = false;
    this->bisector = Hyperbole(p,q,p.get_weight()-q.get_weight());
    this->star_bisector = StarMapping::map_bisector(p,q,bisector);
    this->side = MINUS;
    /*
      If this condition is true, then this bisector is incident on a
      vertice whose position is the same as a mapped site.
    */
    if(!cmpf(_base.x,this->p.x) && cmpf(_base.y,this->p.y) <= 0)
      this->side = (cmpf(this->p.x,this->q.x) == -1) ? PLUS : MINUS;
    /*
      If the base is not contained on the MINUS part of the bisector,
      then this is the PLUS part of the bisector.
    */
    else if(!my_point(_base)) this->side = PLUS;
  }
  else{
    this->side = MINUS;
    this->is_vertical_line = true;
    this->vertical_line = (p.x+q.x)/2.;
  }
  this->base = _base;
  this->base.y += p.get_weight() + Distance::two_points(p,_base);
}

Boundary::Boundary(int _id,WSite _p, WSite _q,Point _base,bool _side){
  this->id = _id;
  this->base = _base;

  this->side = _side;
  if(StarWSiteComp()(_p,_q)) this->p = _q, this->q = _p;
  else this->p = _p, this->q = _q;
  if(cmpf(p.get_weight(),q.get_weight()) || cmpf(p.y,q.y)){
    this->is_vertical_line = false;
    this->bisector = Hyperbole(p,q,p.get_weight()-q.get_weight());
    this->star_bisector = StarMapping::map_bisector(p,q,bisector);
  }
  else{
    this->is_vertical_line = true;
    this->vertical_line = (p.x+q.x)/2.;
  }
}

// if you're using this method you must verify if
// this bisector is vertical.
double Boundary::get_vertical_line()const{
  return vertical_line;
}

bool Boundary::is_vertical()const{
  return is_vertical_line;
}

Hyperbole Boundary::get_hyperbole(){
  return bisector;
}

int Boundary::get_id()const{
  return id;
}

bool Boundary::get_side()const{
  return side;
}

WSite Boundary::highest()const{
  return p;
}

WSite Boundary::lowest()const{
  return q;
}

/*
  Given a point s, my_point verifies if this bisector contains s.
*/
bool Boundary::my_point(Point s)const{
  double s_y = s.y+this->p.get_weight();
  if(this->is_vertical_line) {
    if(cmpf(s.x,vertical_line)) return false;
    double delta_y = base.y - s_y;
    if(cmpf(delta_y,0) <= 0) return true;
    return cmpf(delta_y*delta_y,Distance::sq_two_points(s,p)) <= 0;
  }

  //polynomial that represents the intersection from the vertical line
  //at s.x with the star_bisector.
  SecondDegree poly = star_bisector.x_intersec_equation(s.x);

  /*Case where polynomial not necessarily have second degree.*/
  if(star_bisector.direction == AsymptotesDirection::DIFF_DIRECTION ||
      star_bisector.direction == AsymptotesDirection::ONE_VERTICAL){
    if(this->side == MINUS) return cmpf(s.x,base.x) <= 0;
    /*
    If the base of this bisector is a vertice, even if the side is PLUS,
    we consider that the base belongs to this bisector.
    */
    int diff = cmpf(s.x,base.x);
    return cmpf(base.x,p.x) ? (diff >= 0) : (diff == 1);
  }
  int s_side = cmpf(s.x,p.x);
  bool turn_to_left = (cmpf(p.x,q.x) == 1);

  if(turn_to_left && s_side <= 0) return this->side == MINUS;
  if(!turn_to_left && s_side > 0) return this->side == PLUS;
  /*
    Check if the point s is above or below the intersection point.
  */
  double d = Distance::sq_two_points(s,this->p);
  Bhaskara_Equation eq = Bhaskara::equation(poly.a,poly.b,poly.c);
  double k = eq.b + eq.two_times_a * s_y;
  double k2 = k*k + eq.delta - eq.two_times_a*eq.two_times_a*d;
  /*
    Check if (sqrt(delta)-b)/(2*a) > -(sqrt(delta)+b)/(2*a) without
    using the square root function.
  */
  bool is_highest_root = (cmpf(eq.two_times_a,0) == 1);
  /*
    Checking if s is the lowest or highest intersection
    between the vertical line at s.x and the mapped hyperbole.
  */
  bool s_is_root = (cmpf(k2*k2,4.*k*k*eq.delta) == 0) && (cmpf(k2*k,0) >= 0);
  if(s_is_root){
    int diff_k_delta = cmpf(k*k,eq.delta);
    if(diff_k_delta == 1 && is_highest_root) s_is_root = false;
    else if(diff_k_delta == -1 && !is_highest_root) s_is_root = false;
  }
  if(this->side == PLUS) {
    if(turn_to_left) return is_highest_root ? (s_is_root == false) : (s_is_root == true);
    return is_highest_root ? (s_is_root == true) : (s_is_root == false);
  }
  if(turn_to_left) return is_highest_root ? s_is_root == true : s_is_root == false;
  return is_highest_root ? s_is_root == false : s_is_root == true;
}

void Boundary::print()const{
  printf("Bisector id:%d Side: %d\n",id,side);
  printf("Base: (%lf,%lf)\n",base.x,base.y);
  p.print();
  q.print();
  if(is_vertical()) printf("x = %lf\n\n",vertical_line);
  else {
    printf("Hyperbole: "); bisector.print();
    printf("Mapped Hyperbole: "); star_bisector.print();
    printf("Asymptotes: ");
    if(star_bisector.direction == AsymptotesDirection::SAME_DIRECTION) printf("Same direction\n\n");
    if(star_bisector.direction == AsymptotesDirection::DIFF_DIRECTION) printf("Opposite directions\n\n");
    if(star_bisector.direction == AsymptotesDirection::ONE_VERTICAL) printf("One vertical and the other is whatever.\n\n");
  }
}

AsymptotesDirection Boundary::get_direction(){
  return star_bisector.direction;
}

/*
  Returns the site(this->p or this->q) which is
  the owner of the region that contains the point s.
*/
WSite Boundary::find_region(Point s,bool& on_boundary){
  bool to_left = StarBoundaryComp().comp(s,*this);
  if(is_vertical_line){
    on_boundary = cmpf(s.x,vertical_line) == 0;
  }
  else {
    on_boundary = cmpf(star_bisector.A*s.x*s.x + star_bisector.B*s.x*s.y + star_bisector.C*s.y*s.y
                        + star_bisector.D*s.x + star_bisector.E*s.y + star_bisector.F,0.) == 0;
  }
  if(side == MINUS) return to_left ? q : p;
  return !to_left ? q : p;
}

bool StarBoundaryComp::comp(Point base,Boundary rs){
  if(rs.is_vertical_line) return (cmpf(base.x,rs.vertical_line) <= 0);
  Bhaskara_Equation eq = rs.star_bisector.y_bhaskara_intersec_equation(base.y);

  /*
    Checking if (sqrt(delta)-b)/(2*a) > -(sqrt(delta)+b)/(2*a)
    without using the square root function.
  */
  int a_sign = cmpf(eq.two_times_a,0.);
  bool is_highest_root = (a_sign == 1);

  double k = eq.two_times_a * base.x + eq.b;
  if(rs.side == MINUS){
    if(is_highest_root){
      //compares s.x with -(sqrt(delta)+b)/(2*a)
      if(a_sign == -1) return cmpf(k,0) == 1 ? true : cmpf(k*k,eq.delta) <= 0;
      return cmpf(k,0) == 1 ? false : cmpf(k*k,eq.delta) >= 0;
    }
    //compares s.x with (sqrt(delta)-b)/(2*a)
    if(a_sign == -1) return cmpf(k,0) == -1 ? false : cmpf(k*k,eq.delta) >= 0;
    return cmpf(k,0) == -1 ? true : cmpf(k*k,eq.delta) <= 0;
  }
  if(is_highest_root){
    //compares s.x with (sqrt(delta)-b)/(2*a)
    if(a_sign == -1) return cmpf(k,0) == -1 ? false : cmpf(k*k,eq.delta) >= 0;
    return cmpf(k,0) == -1 ? true : cmpf(k*k,eq.delta) <= 0;
  }
  //compares s.x with -(sqrt(delta)+b)/(2*a)
  if(a_sign == -1) return cmpf(k,0) == 1 ? true : cmpf(k*k,eq.delta) <= 0;
  return cmpf(k,0) == 1 ? false : cmpf(k*k,eq.delta) >= 0;
}

/*
  On the comparison we need to know the position of the sweeping line.
  The y coordinate of the highest base between the bases of pq and rs
  represents the position of the horizontal sweeping line on the plane.
*/
bool StarBoundaryComp::operator()(Boundary pq,Boundary rs){
  if(!cmpf(rs.base.y,pq.base.y)){
    int x_diff = cmpf(pq.base.x,rs.base.x);
    if(x_diff) return (x_diff == -1);
    //special case where more than one bisector was created on this->base
    if(rs.side != pq.side) return pq.side < rs.side;
    if(rs.star_bisector.direction == AsymptotesDirection::SAME_DIRECTION) return (cmpf(rs.p.x,rs.q.x) == -1);
    if(pq.star_bisector.direction == AsymptotesDirection::SAME_DIRECTION) return (cmpf(pq.p.x,pq.q.x) == -1);
    return false;
  }
  return rs.base < pq.base ? comp(pq.base,rs) : !comp(rs.base,pq);
}
