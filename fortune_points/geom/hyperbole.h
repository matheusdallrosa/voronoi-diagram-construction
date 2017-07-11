#pragma once

#include "point.h"

struct Hyperbole{
    /*Coeficientes da forma geral de uma conica.*/
    double A,B,C,D,E,F;
    Hyperbole();
    
    /*
      Construir uma hyperbole pela definicao:
      diferenca da distancia entre dois pontos e igual a uma constante a.
    */
    Hyperbole(Point, Point, double);        
};