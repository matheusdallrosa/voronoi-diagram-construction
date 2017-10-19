#ifndef hyperbole_header
#define hyperbole_header
#include <vector>

#include "common/define.h"

#include "polynomial.h"
#include "bhaskara.h"
#include "point.h"

enum class AsymptotesDirection{
  ONE_VERTICAL,
  SAME_DIRECTION,
  DIFF_DIRECTION
};

class Hyperbole{
  /*ter a cópia dos coeficientes em um vector pode ser útil.*/
  std::vector<double> coef;
public:
  AsymptotesDirection direction;
  /*
    Coeficientes da forma geral de uma cônica.
    Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0;
  */
  double A,B,C,D,E,F,angular;

  Hyperbole();

  Hyperbole(double,double,double,double,double,double,AsymptotesDirection);

  /*
    Construir uma hyperbole pela definicão:
    diferenca da distância entre dois pontos e igual a uma constante a.
  */
  Hyperbole(Point, Point, double);

  /*
    O construtor se utiliza da fórmula:
    [q, 1, 4][q, 2, 3][p, 1, 3][p, 2, 4] − [q, 1, 3][q, 2, 4][p, 1, 4][p, 2, 3] = 0
    Onde q,1,2,3 e 4 são pontos constantes, e p um ponto variável.
    [q,1,4] e o determinante da matriz defifinida pelos pontos q,1,4.

    Esse construtor vai ser necessário para representar uma hipérbole após o mapeamento.
  */
  Hyperbole(Point, Point, Point, Point, Point,AsymptotesDirection);

  void normalize();

  /*
    Interseccão da hipérbole com uma reta horizontal.
  */
  pp y_intersec(double);

  /*
    Equacão de bhaskara para a interseccão da hipérbole com uma reta horizontal.
  */
  Bhaskara_Equation y_bhaskara_intersec_equation(double)const;

  /*
    Interseccão da hipérbole com uma reta vertical.
  */
  pp x_intersec(double)const;

  /*
    Equacão de segundo grau para a interseccão da hipérbole com uma reta vertical.
  */
  SecondDegree x_intersec_equation(double)const;

  /*
    Equacão de bhaskara para a interseccão da hipérbole com uma reta vertical.
  */
  Bhaskara_Equation x_bhaskara_intersec_equation(double)const;

  /*
    Interseccão da hipérbole com uma reta y = mx + b.
  */
  pp line_intersec(double,double);

  void print()const;
};
#endif
