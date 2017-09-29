#include "fortune_points/geom/floating_point.h"
#include "fortune_points/geom/bhaskara.h"
#include "fortune_points/geom/distance.h"
#include "debug.h"
#include "status.h"

#include <stdio.h>

Check::Check(){}

Check::Check(int _id,int _pm){
  id = _id, pm = _pm;
}

Check::Check(Status_item si) : Check(si->id,si->pm){
  this->bisector_it = si;
}

bool Check::operator<(Check o)const{
  return (id != o.id) ? id < o.id : pm < o.pm;
}

Region_Bound Status::find_region(Site s){
  Status_item boundary = bisectors.upper_bound(Bisector(-1,FAKE_BISECTOR,s,s,s));
  if(boundary == bisectors.end()) boundary--;
  Bhaskara_Solution yinter = boundary->horiz_intercep(s.y);

  Site region;
  if(cmpf(yinter.rt0,yinter.rt1)) region = (cmpf(yinter.rt0,s.x) <= 0 &&
                                          cmpf(s.x,yinter.rt1) <= 0) ? boundary->s1
                                                                    : boundary->s0;
  else if(boundary->vertical()) region = cmpf(s.x,yinter.rt0) <= 0 ? boundary->s0
                                                                  : boundary->s1;
  else region = cmpf(s.x,yinter.rt0) ? boundary->s0 : boundary->s1;
  return Region_Bound(region,*boundary);
}

Status_item Status::find(Bisector b){
  std::set<Check>::iterator check_it = valid.find(Check(b.id,b.pm));
  if(check_it == valid.end()) return bisectors.end();
  return check_it->bisector_it;
}

Bisector Status::insert(Bisector b){
  Check check = Check(bisectors.insert(b));
  valid.insert(check);
  return *(check.bisector_it);
}

Inter Status::left_intersec(Bisector b,bool &check){
  Status_item bis = this->find(b);
  Status_item bis2 = bis;
  check = false;
  if(bis == bisectors.begin() || bis->id == (--bis2)->id || bis->l.parallel(bis2->l))
    return Inter();
  Point inter = (bis->l == bis2->l);
  if(bis->mypoint(inter.x) && bis2->mypoint(inter.x)) {
    check = true;
    return Inter(*bis,*bis2,inter.x,inter.y,Distance::two_points(inter,bis->s1));
  }
  return Inter();
}

Inter Status::right_intersec(Bisector b,bool &check){
  Status_item bis = this->find(b);
  Status_item bis2 = bis;
  check = false;
  if(++bis2 == bisectors.end() || bis->id == bis2->id || bis->l.parallel(bis2->l))
    return Inter();
  Point inter = (bis->l == bis2->l);
  if(bis->mypoint(inter.x) && bis2->mypoint(inter.x)){
    check = true;
    return Inter(*bis,*bis2,inter.x,inter.y,Distance::two_points(inter,bis->s1));
  }
  return Inter();
}

void Status::erase(Bisector bis){
  std::set<Check>::iterator check_it = valid.find(Check(bis.id,bis.pm));
  if(check_it == valid.end()) return;
  bisectors.erase(check_it->bisector_it);
  valid.erase(check_it);
}

Bisector Status::left_neighbor(Bisector b){
  if(bisectors.size() == 1) return b;
  Status_item bis = this->find(b);
  return (bis == bisectors.begin()) ? *(++bis) : *(--bis);
}

Bisector Status::right_neighbor(Bisector b){
  if(bisectors.size() == 1) return b;
  Status_item bis = this->find(b);
  return (++bis == bisectors.end()) ? *(--bis) : *bis;
}

Bisector Status::neighbor(Bisector bis){
  return bis.pm == MINUS ? left_neighbor(bis) : right_neighbor(bis);
}

void Status::print(){
  Bisector_cmp bcmp;
  printf("Status da linha:\n");
  for(Status_item it = bisectors.begin(); it != bisectors.end(); it++){
    it->print();
    if(DEBUG){
      Status_item it2 = it; it2++;
      if(it2 != bisectors.end()) printf("%d\n",bcmp(*it,*it2));
    }
  }
}
