#include "inter.h" 

#include <stdio.h>

Inter::Inter(){}

Inter::Inter(Bisector _b0,Bisector _b1,double x,double y,double star) : Point(x,y+star){    
  non_mapp_y = y;
  star_val = star;
  b0 = _b0, b1 = _b1;
} 

void Inter::print(){
  printf("Inter entre: %d,%s %d,%s\n",b0.id,b0.pm == PLUS ? "PLUS" : "MINUS",
                                      b1.id,b1.pm == PLUS ? "PLUS" : "MINUS");
  printf("No ponto: (%lf,%lf)\n\n",x,y);
}