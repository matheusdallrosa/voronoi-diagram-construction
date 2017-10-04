#ifndef inter_queue_header
#define inter_queue_header
#include <vector>
#include <set>

#include "inter.h"
#include "index.h"

struct InterCheck{
  bool exists;
  Inter inter;
};

class InterQueue{
  std::multiset<Inter,InterComp> inter_queue;
  typedef std::multiset<Inter,InterComp>::iterator inter_position;

  std::multiset<Index<inter_position> > idx;
  typedef std::multiset<Index<inter_position> >::iterator idx_position;
public:
  InterQueue();

  Inter first();

  Inter next();

  void add_inter(Inter);

  void add_inter(std::vector<Inter>);

  void rmv_inter(Inter);

  void rmv_inter(ii);

  void rmv_inter(Boundary,Boundary);

  ii get_idx_comp(Boundary,Boundary);

  InterCheck check(Boundary,Boundary);

  bool is_empty();

  void print();
};
#endif
