#ifndef wsite
#define wsite
#include "geom/point.h"

class WSite : public Point{
  int id;
  double weight;
public:  
  WSite();
  WSite(int _id,double x,double y,double _weight);
  double get_weight()const;
  int get_id()const;
  void print()const;
  friend class StarWSiteComp;
};

/*
  Realiza a comparacão entre dois sites mapeados.
*/
class StarWSiteComp{
public: 
  bool operator()(WSite,WSite);
};
#endif