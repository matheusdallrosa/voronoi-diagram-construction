#ifndef boundary_header
#define boundary_header

#include "diagram/wsite.h"

#include "geom/hyperbole.h"
#include "geom/point.h"

const bool PLUS = true;
const bool MINUS = false;

class Boundary{
  int id;

  /*
    base é o ponto no plano onde esta fronteira foi criada.
    base >= p
    base >= q
  */
  Point base;    
  WSite p,q;
    
  /*
    Caso a fronteira seja uma reta vertical.
  */
  double vertical_line;
  bool is_vertical_line;  
  /*
    Caso a fronteira seja uma hiperbole.
  */
  Hyperbole bisector,star_bisector;  
  bool side;
public:  
  Boundary();
  /*
    Construtor utilizado para o caso de busca no status.
  */
  Boundary(Point);
  /*
    Construtor para o caso de a base ser um vértice.
  */
  Boundary(int,WSite, WSite,Point);
  /*
    Construtor para o caso de a base ser um site.
  */
  Boundary(int,WSite,WSite,Point,bool);
  /*
    Retorna o site mais próximo do ponto.
  */
  WSite closest_site(Point) const;
  void print() const;    
  int get_id()const;
  bool get_side()const;
  bool my_point(Point)const;
  WSite highest()const;
  WSite lowest()const;  
  WSite find_region(Point,bool&);  
  Hyperbole get_hyperbole();
  AsymptotesDirection get_direction();
  double get_vertical_line()const;
  bool is_vertical()const;
  friend class StarBoundaryComp;
};

class StarBoundaryComp{
public:
  /*
    Compara um ponto com uma fronteira mapeada.
  */
  bool comp(Point,Boundary);
  bool operator()(Boundary,Boundary);
};

#endif