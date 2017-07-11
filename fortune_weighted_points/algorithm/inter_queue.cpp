#include <stdio.h>

#include "algorithm/inter_queue.h"
#include "debug.h"

InterQueue::InterQueue(){
}

Inter InterQueue::first(){
  return *inter_queue.begin();
}

Inter InterQueue::next(){
  Inter a = *inter_queue.begin();
  rmv_inter(a);
  return a;
}

void InterQueue::add_inter(Inter a){
  if(DEBUG){
    printf("Nova inter:");
    a.print();
  }      
  inter_position pos = inter_queue.insert(a);  
  ii comp = a.get_idx_comp(); 
  idx.insert(Index<inter_position>(comp.first,comp.second,pos));    
}

void InterQueue::add_inter(std::vector<Inter> inters){
  for(int i = 0; i < (int)inters.size(); i++) add_inter(inters[i]);
}

void InterQueue::rmv_inter(ii comp){
  /*Podem existir duas intersecções com o pair comp.*/
  for(idx_position a_idx = idx.find(Index<inter_position>(comp.first,comp.second)); 
      a_idx != idx.end(); 
      a_idx = idx.find(Index<inter_position>(comp.first,comp.second))){    
    if(DEBUG){
      printf("Remover inter:\n");
      a_idx->get_idx()->print();
    }    
    inter_queue.erase(a_idx->get_idx());
    idx.erase(a_idx);  
  }  
}

void InterQueue::rmv_inter(Inter a){  
  rmv_inter(a.get_idx_comp());
}

ii InterQueue::get_idx_comp(Boundary a,Boundary b){
  int ida_a = a.get_id() * (a.get_side() == MINUS ? -1 : 1);
  int ida_b = b.get_id() * (b.get_side() == MINUS ? -1 : 1);
  return ii(std::max(ida_a,ida_b),std::min(ida_a,ida_b));
}

void InterQueue::rmv_inter(Boundary a,Boundary b){  
  rmv_inter(get_idx_comp(a,b));
}

InterCheck InterQueue::check(Boundary a,Boundary b){  
  ii comp = get_idx_comp(a,b);
  idx_position a_idx = idx.find(Index<inter_position>(comp.first,comp.second)); 
  if(a_idx == idx.end()) return InterCheck({false});
  return InterCheck({true,*(a_idx->get_idx())});
}

bool InterQueue::is_empty(){
  return inter_queue.empty();
}

void InterQueue::print(){
  printf("\nFila de inter: %d\n",(int)inter_queue.size());
  for(auto i : inter_queue) i.print();  
  printf("\n");
}