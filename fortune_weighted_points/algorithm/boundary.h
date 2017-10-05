#ifndef boundary_header
#define boundary_header

#include "fortune_weighted_points/diagram/wsite.h"

#include "fortune_weighted_points/geom/hyperbole.h"
#include "fortune_weighted_points/geom/point.h"

const bool PLUS = true;
const bool MINUS = false;

class Boundary{
  int id;

  /*
    base is the point on the plane where this bisector was created.
    It's always true that:
    base >= p
    base >= q
  */
  Point base;
  WSite p,q;

  /*
    When this bisector is a vertical line.
  */
  double vertical_line;
  bool is_vertical_line;
  /*
    When this bisector is a hyperbole.
  */
  Hyperbole bisector,star_bisector;
  bool side;
public:
  Boundary();
  /*
    This constructor must be used to perform a search
    on the status for the first bisector to the right
    of the given point.
  */
  Boundary(Point);
  /*
    This constructor must be used when the base is a vertice.
  */
  Boundary(int,WSite, WSite,Point);
  /*
    This constructor must be used when the base is a site.
  */
  Boundary(int,WSite,WSite,Point,bool);
  /*
    Returns the closest site from the given point.
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
    Compares a point with a mapped bisector.
  */
  bool comp(Point,Boundary);
  bool operator()(Boundary,Boundary);
};

#endif
