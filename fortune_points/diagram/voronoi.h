#include "geom/line.h"
#include "geom/point.h"
/*
  Estruturas para armazenar os vertices e as arestas do diagrama.
  Se sobrar tempo vou implementar uma DCEL, na realidade uma DCEL para
  o diagrama de Voronoi é bem simples.
*/

struct Vertice : Point{
  int id;
  int b0,b1,b2;
  Vertice();
  Vertice(double,double,int,int,int);
};

struct Edge{
  int id,s0,s1;
  Line l;
  Edge();
  Edge(int,Line,int,int);
};