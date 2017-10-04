#include <stdio.h>
#include <math.h>

#include <algorithm>

#include "star_mapping.h"
#include "boundary.h"

#include "fortune_weighted_points/geom/polynomial.h"
#include "fortune_weighted_points/geom/distance.h"

#include "fortune_weighted_points/common/floating_point.h"

#include "debug.h"

Boundary::Boundary(){}

Boundary::Boundary(Point p){
  this->base = p;
  this->side = MINUS;
  this->is_vertical_line = false;
}

/*
Este construtor deve ser utilizado quando _base é um vértice.
*/
Boundary::Boundary(int _id,WSite _p, WSite _q,Point _base){
  this->id = _id;
  /*
    Para simplificar as operações feitas com as fronteiras,
    o site p sempre vai ser maior do que q, ou seja:
    p.y + p.get_weight() > q.y + q.get_weight();
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
      se esta condição for verdadeira este bissetor inicia em um vértice que está na mesma posição que um site mapeado.
    */
    if(!cmpf(_base.x,this->p.x) && cmpf(_base.y,this->p.y) <= 0)
      this->side = (cmpf(this->p.x,this->q.x) == -1) ? PLUS : MINUS;
    /*
      Caso a base não esteja contida na parte MINUS da fronteira,
      então está é a parte PLUS da fronteira.
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

//quem usar este método deve verificar antes se
//a fronteira é uma reta vertical.
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
  Verifica se o ponto dado s está contido nesta parte da fronteira.
*/
bool Boundary::my_point(Point s)const{
  double s_y = s.y+this->p.get_weight();
  if(this->is_vertical_line) {
    if(cmpf(s.x,vertical_line)) return false;
    double delta_y = base.y - s_y;
    if(cmpf(delta_y,0) <= 0) return true;
    return cmpf(delta_y*delta_y,Distance::sq_two_points(s,p)) <= 0;
  }

  //polinômio que representa a intersecção de s.x com star_bisector.
  SecondDegree poly = star_bisector.x_intersec_equation(s.x);

  /*Caso em que poly não precisa ser de segundo grau.*/
  if(star_bisector.direction == AsymptotesDirection::DIFF_DIRECTION ||
      star_bisector.direction == AsymptotesDirection::ONE_VERTICAL){
    if(this->side == MINUS) return cmpf(s.x,base.x) <= 0;
    /*
    Caso a base desta fronteira esteja em um vértice,
    mesmo que o lado seja PLUS, vamos considerar que a base
    pertence a fronteira.
    */
    int diff = cmpf(s.x,base.x);
    return cmpf(base.x,p.x) ? (diff >= 0) : (diff == 1);
  }
  int s_side = cmpf(s.x,p.x);
  bool turn_to_left = (cmpf(p.x,q.x) == 1);

  if(turn_to_left && s_side <= 0) return this->side == MINUS;
  if(!turn_to_left && s_side > 0) return this->side == PLUS;
  /*
    Verifica se o ponto s e o menor ou maior ponto de interseccão.
  */
  double d = Distance::sq_two_points(s,this->p);
  Bhaskara_Equation eq = Bhaskara::equation(poly.a,poly.b,poly.c);
  double k = eq.b + eq.two_times_a * s_y;
  double k2 = k*k + eq.delta - eq.two_times_a*eq.two_times_a*d;
  /*
    Verificar se (sqrt(delta)-b)/(2*a) > -(sqrt(delta)+b)/(2*a) sem utilizar raíz quadrada.
  */
  bool is_highest_root = (cmpf(eq.two_times_a,0) == 1);
  /*
    Decidindo se o ponto s é a menor ou a maior intersecção
    entre a reta vertical s.x e a hipérbole mapeada.
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
  printf("Id bissetor:%d lado: %d\n",id,side);
  printf("Base: (%lf,%lf)\n",base.x,base.y);
  p.print();
  q.print();
  if(is_vertical()) printf("x = %lf\n\n",vertical_line);
  else {
    printf("Hyperbole: "); bisector.print();
    printf("Hyperbole Mapeada: "); star_bisector.print();
    printf("Assintotas: ");
    if(star_bisector.direction == AsymptotesDirection::SAME_DIRECTION) printf("Mesma direcao\n\n");
    if(star_bisector.direction == AsymptotesDirection::DIFF_DIRECTION) printf("Direcoes opostas\n\n");
    if(star_bisector.direction == AsymptotesDirection::ONE_VERTICAL) printf("Uma vertical e a outra tanto faz.\n\n");
  }
}

AsymptotesDirection Boundary::get_direction(){
  return star_bisector.direction;
}

/*
  Retorna o site(this->p ou this->q) dono da região que contém o ponto s.
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
    Verificar se (sqrt(delta)-b)/(2*a) > -(sqrt(delta)+b)/(2*a) sem utilizar raíz quadrada.
  */
  int a_sign = cmpf(eq.two_times_a,0.);
  bool is_highest_root = (a_sign == 1);

  double k = eq.two_times_a * base.x + eq.b;
  if(rs.side == MINUS){
    if(is_highest_root){//comparar s.x com -(sqrt(delta)+b)/(2*a)
      if(a_sign == -1) return cmpf(k,0) == 1 ? true : cmpf(k*k,eq.delta) <= 0;
      return cmpf(k,0) == 1 ? false : cmpf(k*k,eq.delta) >= 0;
    }
    //comparar s.x com (sqrt(delta)-b)/(2*a)
    if(a_sign == -1) return cmpf(k,0) == -1 ? false : cmpf(k*k,eq.delta) >= 0;
    return cmpf(k,0) == -1 ? true : cmpf(k*k,eq.delta) <= 0;
  }
  if(is_highest_root){//comparar s.x com (sqrt(delta)-b)/(2*a)
    if(a_sign == -1) return cmpf(k,0) == -1 ? false : cmpf(k*k,eq.delta) >= 0;
    return cmpf(k,0) == -1 ? true : cmpf(k*k,eq.delta) <= 0;
  }
  //comparar s.x com -(sqrt(delta)+b)/(2*a)
  if(a_sign == -1) return cmpf(k,0) == 1 ? true : cmpf(k*k,eq.delta) <= 0;
  return cmpf(k,0) == 1 ? false : cmpf(k*k,eq.delta) >= 0;
}

/*
  Será utilizado a maior base entre as duas bases das fronteiras pq e rs.
  A coordenada y da maior base aponta a posição da linha varredora,
  e a coordenada x da maior base aponta a posição da base na linha varredora.
*/
bool StarBoundaryComp::operator()(Boundary pq,Boundary rs){
  if(!cmpf(rs.base.y,pq.base.y)){
    int x_diff = cmpf(pq.base.x,rs.base.x);
    if(x_diff) return (x_diff == -1);
    //printf("caso especial onde mais de uma fronteira foi criada em rs.base.\n");
    if(rs.side != pq.side) return pq.side < rs.side;
    if(rs.star_bisector.direction == AsymptotesDirection::SAME_DIRECTION) return (cmpf(rs.p.x,rs.q.x) == -1);
    if(pq.star_bisector.direction == AsymptotesDirection::SAME_DIRECTION) return (cmpf(pq.p.x,pq.q.x) == -1);
    return false;
  }
  return rs.base < pq.base ? comp(pq.base,rs) : !comp(rs.base,pq);
}
