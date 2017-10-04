#include "vertice.h"

Vertice::Vertice(int _id,std::vector<int> _edges,double x,double y) : Point(x,y){
  this->id = _id;
  this->edges = _edges;
}
