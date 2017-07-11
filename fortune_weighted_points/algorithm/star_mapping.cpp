#include <algorithm>

#include "algorithm/star_mapping.h"

#include "common/floating_point.h"

#include "geom/distance.h"
#include "geom/line.h"

void StarMapping::map_pp(pp & p,WSite w){
  p.first.y += Distance::two_points(p.first,w) + w.get_weight();
  p.second.y += Distance::two_points(p.second,w) + w.get_weight();
}

Hyperbole StarMapping::map_bisector(WSite a, WSite b, Hyperbole h){  
  
  WSite p,q;
  double weight_diff = a.get_weight()-b.get_weight();    
  if(cmpf(weight_diff,0) >= 0) p = a, q = b;      
  else p = b, q = a, weight_diff = -weight_diff;      
    
  /*Caso o bissetore entre a e b seja uma reta.*/
  if(cmpf(weight_diff,0.1) <= 0){
    p.y +=  p.get_weight(), q.y += q.get_weight();
    Line pq = Line(Point(-p.y,p.x),Point(-q.y,q.x));  
    pq.translate(Point((p.x+q.x)/2,(p.y+q.y)/2));
    double m = pq.getAngular();
    double b = pq.getLinear();  
    return Hyperbole(-1.,-2.*m,1,2.*(m*p.y + p.x),-2.*b,2.*b*p.y-(p.x*p.x + p.y*p.y),AsymptotesDirection::DIFF_DIRECTION);  
  }  
  
  Line pq = Line(Point(-p.y,p.x),Point(-q.y,q.x));  
  int weight_y_diff = cmpf(std::abs(p.y-q.y),weight_diff);  
  pp p01,p23;
  
  AsymptotesDirection asymp_direction;
  if(!weight_y_diff){
    if(!cmpf(p.y+p.get_weight(),q.y+q.get_weight()) && cmpf(p.x,q.x) == -1) std::swap(p,q);    
    asymp_direction = AsymptotesDirection::ONE_VERTICAL;
  }
  else if(weight_y_diff == 1) asymp_direction = AsymptotesDirection::DIFF_DIRECTION;  
  /*
    Caso weight_y_diff == 0 obrigatoriamente
    p.y + Wp != q.y + Wq deve ser verdade.
  */
  if(weight_y_diff >= 0){        
    //printf("one vertical and one nonvertical asymptote.\n");
    //printf("one asymptote to the left, the other to the right.\n");
    pq.translate(p);
    p01 = h.line_intersec(pq.getAngular(),pq.getLinear()); 
    /*Colhendo os 4 pontos.*/   
    if(fabs(p.x-q.x) < 1.){      
      if(cmpf(p.y,q.y) == 1) pq.translate(Point(p.x,p.y+10));
      else pq.translate(Point(p.x,p.y-10));
    }
    else{
      if(cmpf(p.x,q.x) == 1) pq.translate(Point(p.x+10,p.y));
      else pq.translate(Point(p.x-10,p.y));
    }
    p23 = h.line_intersec(pq.getAngular(),pq.getLinear());
  }
  else {    
    asymp_direction = AsymptotesDirection::SAME_DIRECTION;
    /*Colhendo os 4 pontos.*/   
    if(fabs(p.x-q.x) < 1.){
      if(cmpf(p.y,q.y) == 1){            
        p01 = h.y_intersec(p.y+10.);
        p23 = h.y_intersec(p.y+20.);    
      }
      else{
        p01 = h.x_intersec(p.y-10.);
        p23 = h.y_intersec(p.y-20.);
      }
    }
    else{
      //printf("both asymptote to the left or both to the right.\n");    
      if(cmpf(p.x,q.x) == 1){            
        p01 = h.x_intersec(p.x+10.);
        p23 = h.x_intersec(p.x+20.);    
      }
      else{
        p01 = h.x_intersec(p.x-10.);
        p23 = h.x_intersec(p.x-20.);
      }
    }
  }  
  StarMapping::map_pp(p01,p);      
  StarMapping::map_pp(p23,p);    
  return Hyperbole(p01.first,p01.second,p23.first,p23.second,Point(p.x,p.y+p.get_weight()),asymp_direction);  
}