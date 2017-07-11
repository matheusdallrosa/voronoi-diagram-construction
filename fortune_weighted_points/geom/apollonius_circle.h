#ifndef apollonius_circle
#define apollonius_circle
#include <vector>

#include "geom/point.h"

/*
  Esta classe e responsável por calcular o centro do círculo de Apollonius,
  o centro desse círculo e um possível vértice do diagrama.
*/
class ApolloniusCircle{
/*Não existe razão para instanciar esta classe.*/
private:
  ApolloniusCircle();
  static std::vector<Point> apollonius_circle_circum_center(Point & G0, Point & G1, Point & G2);
  static bool correct_target(double,double,double,double,double,double,Point);
  static Point compute_center(double, double, double, Point &);
public:
/*
  Dado três círculos C0,C1,C2, calcular o círculo tangente a estes três círculos.
  O círculo tangente a C0,C1 e C2 e único.
  O método utilizado para computar o círculo será discrito na monografia.
  Cada ponto representa o centro de um círculo e o parâmetro double seguinte
  representa o raio do círculo.
  Os pontos devem ser passados por ordem crescente de raio.
*/
  static std::vector<Point> circle_center(Point &, double, Point &, double, Point &, double);     
};
#endif