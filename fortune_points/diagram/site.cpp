#include "site.h"
#include <stdio.h>

Site::Site(){}

Site::Site(int _id,double x,double y) : Point(x,y){
  id = _id;
} 

void Site::print(){
  printf("id: %d\n",id);
  printf("Coords: (%.3lf, %.3lf)\n",x,y);
}