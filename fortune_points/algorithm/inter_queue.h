#pragma once

#include "inter.h"
#include <set>

typedef std::multiset<Inter>::iterator Inter_idx;

/*
We make a search for 4 integers which can uniquely represent an intersection.
The 4 integers are the: id and side from the intersecting bisectors.
*/
struct Index{
  int s0,s0_side;
  int s1,s1_side;

  Inter_idx inter;

  Index();

  Index(int,int,int,int);

  Index(Inter_idx);

  bool operator<(Index o)const;
};

typedef std::pair<bool,Inter_idx> Inter_Check;

struct Inter_Queue{
  std::set<Index> index;
  std::multiset<Inter> interq;

  Inter_Queue();

  void enqueue(Inter);

  bool is_empty();

  Inter_idx first();

  Inter next();

  Inter_Check check(int,int,int,int);

  void cancel(Inter_idx);

  void cancel(int,int,int,int);
};
