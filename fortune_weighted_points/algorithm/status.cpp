#include <stdio.h>

#include "algorithm/status.h"

#include "debug.h"

Status::Status(){}

closest_boundaries Status::closest_bound(Point s){
  Boundary fake_boundary = Boundary(s);  
  status_position right = status.upper_bound(fake_boundary);
  if(right == status.end()){
    right--;  
    return closest_boundaries(*(right),*(right));
  }
  Boundary right_boundary = *right;
  if(status.size() == 1) return closest_boundaries(right_boundary,right_boundary);
  if(right == status.begin()) return closest_boundaries(right_boundary,right_boundary);
  return closest_boundaries(*(--right),right_boundary);
}

void Status::add_boundary(Boundary b){
  if(DEBUG){
    printf("ADCIONANDO fronteira:\n");
    b.print();
  }
  status_position s = status.insert(b).first;    
  idx.insert(Index <status_position> (b.get_id(),b.get_side(),s));
}

void Status::rmv_boundary(Boundary b){
  if(DEBUG){
    printf("REMOVENDO fronteira:\n");
    b.print();
  }
  idx_position b_idx = idx.find(Index <status_position> (b.get_id(),b.get_side()));
  if(b_idx != idx.end()){
    if(b_idx->get_idx() != status.end()) status.erase(b_idx->get_idx());
    idx.erase(b_idx);
  }
}

closest_boundaries Status::neighbors(Boundary b){  
  idx_position b_idx = idx.find(Index <status_position> (b.get_id(),b.get_side()));    
  if(status.size() == 1 || b_idx == idx.end()) return closest_boundaries(b,b);  
  status_position b_it = b_idx->get_idx();
  status_position next_b_it = b_it;
  if(b_it == status.begin()) return closest_boundaries(*(b_it),*(++next_b_it));
  status_position prev_b_it = b_it;
  if(++next_b_it == status.end()) return closest_boundaries(*(--prev_b_it),*(b_it));
  return closest_boundaries(*(--prev_b_it),*next_b_it);
}

void Status::print(){
  printf("\nstatus atual:\n");
  for(status_position it = status.begin(); it != status.end(); it++){
    it->print(); 
    printf("\n");
  }
}