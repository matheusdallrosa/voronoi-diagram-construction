#include <stdio.h>
#include <math.h>

#include <algorithm>

#include "inter.h"

#include "fortune_weighted_points/geom/distance.h"

#include "fortune_weighted_points/diagram/wsite.h"

#include "common/floating_point.h"

Inter::Inter(){}

Inter::Inter(Boundary _Cqr,Boundary _Crs,double x,double y) : Point(x,y){
  this->Cqr = _Cqr, this->Crs = _Crs;
  WSite p = this->Cqr.highest();
  this->mapp_weight = p.get_weight();
  this->mapp_y = y + this->mapp_weight;
  this->mapp_dist_squared = Distance::sq_two_points(p,*this);
  /*
    Identificadores de uma intersecção entre os bissetores _Cqr e _Crs.
  */
  int ida_Cqr = Cqr.get_id() * (Cqr.get_side() == MINUS ? -1 : 1);
  int ida_Crs = Crs.get_id() * (Crs.get_side() == MINUS ? -1 : 1);
  this->idx = ii(std::max(ida_Cqr,ida_Crs),std::min(ida_Cqr,ida_Crs));
  WSite Cqr_highest = Cqr.highest(), Cqr_lowest = Cqr.lowest();
  WSite Crs_highest = Crs.highest(), Crs_lowest = Crs.lowest();
  /*
    O vector tangent vai conter os três sites tangentes ao círculo com o centro
    nesta interseccão, sendo que o primeiro item do vector, precisa ser o wsites
    que não vai ser utilizado para a criacão do bissetor incidente neste
    possível vértice.
  */
  if(Cqr_highest.get_id() == Crs_highest.get_id() ||
      Cqr_highest.get_id() == Crs_lowest.get_id()) tangent.push_back(Cqr_highest);
  else if(Cqr_lowest.get_id() == Crs_highest.get_id() ||
          Cqr_lowest.get_id() == Crs_lowest.get_id()) tangent.push_back(Cqr_lowest);
  int repeated_id = tangent[0].get_id();
  if(repeated_id != Cqr_highest.get_id()) tangent.push_back(Cqr_highest);
  if(repeated_id != Cqr_lowest.get_id()) tangent.push_back(Cqr_lowest);
  if(repeated_id != Crs_highest.get_id()) tangent.push_back(Crs_highest);
  if(repeated_id != Crs_lowest.get_id()) tangent.push_back(Crs_lowest);
}

ii Inter::get_idx_comp(){
  return idx;
}

Boundary Inter::get_Cqr()const{
  return Cqr;
}

Boundary Inter::get_Crs()const{
  return Crs;
}

/*Comparando duas intersecções sem tirar raíz quadrada.*/
bool InterComp::operator()(Inter a,Inter b){
  double k = a.mapp_y - b.mapp_y;

  int k_sign = cmpf(k,0);
  int d_sign = cmpf(b.mapp_dist_squared,a.mapp_dist_squared);
  if(!k_sign && !d_sign) return cmpf(a.x,b.x) == -1;
  if(k_sign <= 0 && d_sign >= 0) return true;
  if(k_sign >= 0 && d_sign <= 0) return false;

  double k_squared = k*k;
  double k_prime = k_squared + a.mapp_dist_squared - b.mapp_dist_squared;
  double k_prime_squared = k_prime * k_prime;
  if(k_sign >= 0 && d_sign >= 0){
    if(cmpf(k_prime,0) >= 0) return false;
    return cmpf(k_prime_squared,4. * k_squared * a.mapp_dist_squared) >= 0;
  }
  /*neste caso: k_sign <= 0 && d_sign <= 0*/
  if(cmpf(k_prime,0) <= 0) return true;
  return cmpf(k_prime_squared,4. * k_squared * a.mapp_dist_squared) <= 0;
}

void Inter::print()const{
  printf("Nova inter entre: id:%d lado:%d | id:%d lado:%d\n",Cqr.get_id(),Cqr.get_side(),Crs.get_id(),Crs.get_side());
  printf("Nao mapeada:(%lf,%lf)\n",x,y);
  printf("mapeada:(%lf,%lf,%lf)\n\n",x,y,mapp_y + sqrt(mapp_dist_squared));
}
