#pragma once

#include "point.h"

struct Hyperbole{
    /*Coefficients from a conic section.*/
    double A,B,C,D,E,F;
    
    Hyperbole();

    /*
      Builds a hyperbole by definition.
    */
    Hyperbole(Point, Point, double);
};
