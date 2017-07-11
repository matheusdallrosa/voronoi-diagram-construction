#include "inter_queue.h"
#include "debug.h"
#include <stdio.h>

Index::Index(){}

Index::Index(int _s0,int _s0_side,int _s1,int _s1_side){      
  if(_s0 < _s1){
    this->s0 = _s0;
    this->s0_side = _s0_side;
    this->s1 = _s1;
    this->s1_side = _s1_side;
  }
  else{
    this->s0 = _s1;
    this->s0_side = _s1_side;
    this->s1 = _s0;
    this->s1_side = _s0_side;
  }
}

Index::Index(Inter_idx it) : Index(it->b0.id,it->b0.pm,
                                    it->b1.id,it->b1.pm){
  inter = it;  
}

bool Index::operator<(Index o)const{
  if(s0 != o.s0) return s0 < o.s0;
  if(s0_side != o.s0_side) return s0_side < o.s0_side;
  if(s1 != o.s1) return s1 < o.s1;
  return s1_side < o.s1_side;
}

Inter_Queue::Inter_Queue(){}

void Inter_Queue::enqueue(Inter inter){
  Inter_idx it = interq.insert(inter);
  index.insert(Index(it));
  if(DEBUG) {
    printf("Nova inter: %d-%d %d-%d\n",inter.b0.id,inter.b0.pm,inter.b1.id,inter.b1.pm);
    printf("Em: (%lf,%lf)\n\n",inter.x,inter.y);
  }
}

bool Inter_Queue::is_empty(){
  return (interq.size() == 0);
}

Inter_idx Inter_Queue::first(){  
  return interq.begin();
}

Inter Inter_Queue::next(){
  Inter_idx next_inter = first();
  Inter new_inter = *next_inter;
  index.erase(Index(next_inter));
  interq.erase(next_inter);  
  return new_inter;
}

void Inter_Queue::cancel(int s0,int s0_side,int s1,int s1_side){  
  std::set<Index>::iterator it = index.find(Index(s0,s0_side,s1,s1_side));
  if(it == index.end()) return;    
  if(DEBUG) printf("Inter cancelada: %d %d\n\n",it->inter->b0.id,it->inter->b1.id);  
  interq.erase(it->inter);
  index.erase(it);
}

Inter_Check Inter_Queue::check(int s0,int s0_side,int s1,int s1_side){
  std::set<Index>::iterator it = index.find(Index(s0,s0_side,s1,s1_side));
  if(it == index.end()) return Inter_Check(false,interq.end());  
  return Inter_Check(true,it->inter);
}

void Inter_Queue::cancel(Inter_idx it){
  if(DEBUG) printf("Inter cancelada: %d %d\n\n",it->b0.id,it->b1.id);
  index.erase(Index(it));
  interq.erase(it);
}