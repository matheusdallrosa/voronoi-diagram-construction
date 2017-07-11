#include <math.h>

#include <algorithm>
#include <vector>
#include <set>

#include "algorithm/inter_detector.h"
#include "algorithm/inter.h"

#include "diagram/wsite.h"

#include "geom/apollonius_circle.h"
#include "geom/point.h"

#include "debug.h"

InterDetector::InterDetector(){}

bool circle_comp(WSite a, WSite b){
  return a.get_weight() > b.get_weight();
}

std::vector<Inter> InterDetector::detect(Boundary Cqr,Boundary Crs,double W){  
  if(Cqr.get_id() == Crs.get_id()) return std::vector<Inter>();
  /*
    Existem 4 sites que utilizam as fronteiras Cqr e Crs para delimitar as fronteiras das suas regiões,
    porém desses 4 sites 2 são iguais. Utilizo o set para simplificar o código.
  */
  std::set<WSite,StarWSiteComp> triple;
  triple.insert(Cqr.lowest());
  triple.insert(Cqr.highest());
  triple.insert(Crs.lowest());
  triple.insert(Crs.highest());
  /*Caso não existam três sites a intersecção já é inválida.*/
  if(triple.size() != 3) return std::vector<Inter>(); 
  /*O método para detecção do círculo de Apollonius utiliza os círculos ordenados por raio.*/
  std::vector<WSite> ordered_trile(triple.begin(),triple.end());
  sort(ordered_trile.begin(),ordered_trile.end(),circle_comp);
  std::vector<Point> centers = ApolloniusCircle::circle_center(ordered_trile[0],W-ordered_trile[0].get_weight(),
                                                                ordered_trile[1],W-ordered_trile[1].get_weight(),
                                                                ordered_trile[2],W-ordered_trile[2].get_weight());
  std::sort(centers.begin(),centers.end());
  std::vector<Inter> inters;
  //verificar se pertence a esta metade do bissetor.
  for(auto& p : centers) {
    if(DEBUG){
      printf("\nTestando inter entre: %d-%s | %d-%s\n",Cqr.get_id(),Cqr.get_side() ? "PLUS" : "MINUS",
                                              Crs.get_id(),Crs.get_side() ? "PLUS" : "MINUS");
      printf("em: (%lf,%lf)\n",p.x,p.y);    
      printf("%d %d\n",Cqr.my_point(p), Crs.my_point(p));
    }            
    /*Pode ser que este ponto não pertença a parte Cqr do bissetor Bqr, ou 
      não pertença a parte Crs do bissetor Brs.
    */
    if(Cqr.my_point(p) & Crs.my_point(p)){      
      inters.push_back(Inter(Cqr,Crs,p.x,p.y));  
    }    
  }  
  return inters;
}