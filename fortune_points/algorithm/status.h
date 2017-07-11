#pragma once

#include "bisector.h"
#include <set>

typedef std::multiset<Bisector,Bisector_cmp> Bisector_set;
typedef Bisector_set::iterator Status_item;
typedef std::pair<Site,Bisector> Region_Bound;

#include "inter.h"

struct Inter;

/*
A struct Check serve para fazermos uma busca direta por um bissetor no status da linha.
Isso ajuda a evitar a possibilidade de não econtrar um bissetor por erro de ponto flutuante.
*/
struct Check{
  int id,pm;
  Status_item bisector_it;
  Check();
  Check(int,int);
  Check(Status_item);
  bool operator<(Check)const;
};

struct Status{
  std::set<Check> valid;
  Bisector_set bisectors;  
  Region_Bound find_region(Site);
  Bisector insert(Bisector);
  void erase(Bisector);
  Inter left_intersec(Bisector,bool &);
  Inter right_intersec(Bisector,bool &);
  Bisector left_neighbor(Bisector);
  Bisector right_neighbor(Bisector);
  Bisector neighbor(Bisector);
  Status_item find(Bisector);
  void print();
};