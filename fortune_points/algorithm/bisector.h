#pragma once

#include "geom/line.h"
#include "diagram/site.h"
#include "geom/bhaskara.h"

#define MINUS 0
#define PLUS 1 
#define FAKE_BISECTOR 2

struct Bisector{
  Bisector();
  /*Posicao da linha varredora quando esse bisetor foi criado.*/
  double ly,xbase;
  
  /*int para apontar se este bisetor é C^{+},C^{-} ou foi criado para busca.
    pm = 1,2 ou 3.
    se pm = 1 então mais, se pm = 2 entao menos, senao foi criado para uma busca.*/
  int pm;
  
  /*inteiro identificador do Bisetor.*/
  int id;   
  
  /*Linha do bisetor.*/
  Line l;   
  
  /*O site s0 sempre vai ser o site de menor localidade.*/
  Site s0,s1;     
  Bisector(int _id,int _pm,Site s0,Site s1,Point);  
  
  bool vertical()const;
  
  Bhaskara_Solution horiz_intercep(double ly)const;
  
  double xcoord(double ly)const;
  
  bool mypoint(double x)const;

  void print()const;
};

struct Bisector_cmp{
  bool operator()(Bisector a,Bisector b);
};