#include <stdio.h>

#include <algorithm>

#include "common/floating_point.h"
#include "common/define.h"

#include "geom/apollonius_circle.h"
#include "geom/distance.h"
#include "geom/bhaskara.h"
#include "geom/line.h"

#include "debug.h"

ApolloniusCircle::ApolloniusCircle(){}

bool ApolloniusCircle::correct_target(double tang_line_a, double tang_line_b, double tang_line_c,
                                      double numX, double numY, double d, Point G0){        
  if(cmpf(tang_line_c,0)){      
    double cd = tang_line_c * d;            
    return cmpf(tang_line_a*numX + tang_line_b*numY + cd,0) == cmpf(cd,0);        
  }  
  return false;
}

Point ApolloniusCircle::compute_center(double a,double b,double c,Point & G){
  return Point(-a/(2.*c) + G.x,b/(2.*c) + G.y);
}

std::vector<Point> ApolloniusCircle::circle_center(Point & G0, double r0, 
                                                    Point & G1, double r1, 
                                                    Point & G2, double r2){                 
  /*caso os três círculos tenham o mesmo raio basta calcular o circuncentro.*/
  if(cmpf(r0,r1) == 0 && cmpf(r1,r2) == 0)
    return ApolloniusCircle::apollonius_circle_circum_center(G0,G1,G2);
    
  r1 -= r0, r2 -= r0;    
  double d1 = std::abs(Distance::sq_two_points(G0,G1) - r1*r1);    
  double d2 = std::abs(Distance::sq_two_points(G0,G2) - r2*r2);                                                    
  /* 
    W1 = (numX1/d1,numY1/d1), 
    W2 = (numX2/d2,numY2/d2)
  */  
  double numX1 = G1.x-G0.x, numY1 = G0.y-G1.y;
  double numX2 = G2.x-G0.x, numY2 = G0.y-G2.y;
  //printf("W1 = (%lf/%lf,%lf/%lf) %lf\n",numX1,d1,numY1,d1,r1);
  //printf("W2 = (%lf/%lf,%lf/%lf) %lf\n",numX2,d2,numY2,d2,r2);
  
  std::vector<Point> centers;
  /*
    se a origem está contida em algum dos círculos, então 
    não vai exisitir um centro correto.
  */
  if(cmpf(numX1*numX1+numY1*numY1, r1*r1) <= 0 || 
      cmpf(numX2*numX2+numY2*numY2, r2*r2) <= 0){
    //printf("Um dos circulos contem a origem.\n");
    return centers;      
  }
  
  /*
    Caso os dois círculos tiverem os mesmos raios,
    o problema se reduz a computar uma linha que passa pelos
    dois centros e transladar esta linha para a circunferência.
  */
  if(cmpf(r1*d2,r2*d1) == 0){ // caso 1.
    //printf("Tentando computar 1\n");
    if(cmpf(numX1*d2,numX2*d1) == 0){ // caso 1_1.
      //printf("  Computado 1\n");
                    
      if(cmpf(numX1+r1,0)) centers.push_back(Point(d1/(2.*(numX1+r1))+G0.x,G0.y));        
      if(cmpf(numX2-r2,0)) centers.push_back(Point(d2/(2.*(numX2-r2))+G0.x,G0.y));              
      return centers;
    }
    
    //printf("\nTentando computar 2\n");    
    if(cmpf(numY1*d2,numY2*d1) == 0){ // caso 1_2.
      //printf("  Computado 2\n");
      
      if(cmpf(numY1+r1,0)) centers.push_back(Point(G0.x,-d1/(2.*(numY1+r1))+G0.y));        
      if(cmpf(numY2-r2,0)) centers.push_back(Point(G0.x,-d2/(2.*(numY2-r2))+G0.y));            
      return centers;
    }    
    
    // caso 1_3.
    double m = (d2*numX1 - d1*numX2)/(d1*numY2-d2*numY1);
    double b = (numY2 - numX2*m)/d2;
    
    double numK = d2*b - numY2;
    double c0 = (m*m+1)*d2*d2;
    double c1 = 2.*d2*(numK*m-numX2);
    double c2 = numK*numK + numX2*numX2 - r2*r2;
    dd xs = Bhaskara::solve(c0,c1,c2);
    
    double y = xs.first*m + b;    
    double tang_line_a = (d1*numY2 - d2*numY1);
    double tang_line_b = (d2*numX1 - d1*numX2);
    double tang_line_c = -(tang_line_a*xs.first + tang_line_b*y);   
    //printf("\nTentando computar 3\n");         
    if(ApolloniusCircle::correct_target(tang_line_a,tang_line_b,tang_line_c,numX1,numY1,d1,G0)){ 
      //printf("  Computado 3\n");     
      centers.push_back(ApolloniusCircle::compute_center(tang_line_a,tang_line_b,tang_line_c,G0));      
    }
    
    if(!cmpf(xs.first,xs.second)) return centers;
    
    y = xs.second*m + b;
    tang_line_c = -(tang_line_a*xs.second + tang_line_b*y);        
    //printf("\nTentando computar 4\n");         
    if(ApolloniusCircle::correct_target(tang_line_a,tang_line_b,tang_line_c,numX1,numY1,d1,G0)){      
      //printf("  Computado 4\n");     
      centers.push_back(ApolloniusCircle::compute_center(tang_line_a,tang_line_b,tang_line_c,G0));
    }    
    return centers;    
  }  
  
  /*
    Caso o r1 == 0 o problema se reduz a computar uma reta tangente
    ao círculo W2 que passa por G1.
  */
  else if(cmpf(r1,0) == 0){ // caso 2.
    double a = d1*numX2-d2*numX1;
    double b = d1*numY2-d2*numY1;
    double c = (d2*(numX1*numX2 + numY1*numY2) - d1*(numX2*numX2 + numY2*numY2 - r2*r2))/d2;    
    if(cmpf(b,0.) == 0){ //reta vertical. caso 2_1
      double x = -(c/a);       
      double c1 = -d2*2.*numY2;
      double c2 = numX2*numX2 + numY2*numY2 + d2*d2*x*x - (r2*r2 + 2*d2*numX2*x);
      
      dd ys = Bhaskara::solve(d2*d2,c1,c2);
      double tang_line_b = (numX1 - d1*x);
      double tang_line_a = (d1*ys.first-numY1);      
      double tang_line_c = numY1*x - numX1*ys.first;
      //printf("\nTentando computar 5\n");         
      if(ApolloniusCircle::correct_target(tang_line_a,tang_line_b,tang_line_c,numX2,numY2,d2,G0)){        
        //printf("  Computado 5\n");
        centers.push_back(ApolloniusCircle::compute_center(tang_line_a,tang_line_b,tang_line_c,G0));              
      }
      
      if(!cmpf(ys.first,ys.second)) return centers;
      
      tang_line_c = numY1*x - numX1*ys.second;      
      tang_line_a = (d1*ys.second - numY1);   
      //printf("\nTentando computar 6\n");               
      if(ApolloniusCircle::correct_target(tang_line_a,tang_line_b,tang_line_c,numX2,numY2,d2,G0)){        
        //printf("  Computado 6\n");
        centers.push_back(ApolloniusCircle::compute_center(tang_line_a,tang_line_b,tang_line_c,G0));        
      }
    }    
    else{ //caso 2_2
      double c0 = b*b + a*a;
      double c1 = (2*(d2*a*c + b*(a*numY2-b*numX2)))/d2;
      double c2 = c*c + (b*2*c*numY2)/d2 + (b*b*(numX2*numX2 + numY2*numY2 - r2*r2))/(d2*d2);
      
      dd xs = Bhaskara::solve(c0,c1,c2);  
      
      double y = -(a*xs.first+c)/b; 
      double tang_line_c = numY1*xs.first-numX1*y;              
      double tang_line_b = (numX1 - d1*xs.first);
      double tang_line_a = (d1*y - numY1);     
      //printf("\nTentando computar 7\n");      
      if(ApolloniusCircle::correct_target(tang_line_a,tang_line_b,tang_line_c,numX2,numY2,d2,G0)){            
        //printf("  Computado 7\n");
        centers.push_back(ApolloniusCircle::compute_center(tang_line_a,tang_line_b,tang_line_c,G0));                
      }
      
      if(!cmpf(xs.first,xs.second)) return centers;
      
      y = -(a*xs.second+c)/b;                  
      tang_line_c = numY1*xs.second - numX1*y;   
      tang_line_b = (numX1 - d1*xs.second);   
      tang_line_a = (d1*y - numY1);         
      //printf("\nTentando computar 8\n");                               
      if(ApolloniusCircle::correct_target(tang_line_a,tang_line_b,tang_line_c,numX2,numY2,d2,G0)){                
        //printf("  Computado 8\n");
        centers.push_back(ApolloniusCircle::compute_center(tang_line_a,tang_line_b,tang_line_c,G0));                                    
      }
    }
  }  
  else{ //caso 3.
    double den = d2*r1 - d1*r2;
    /*Calculando as coordenadas do ponto i.*/
    double ix = (numX2*r1 - numX1*r2) / den;
    double iy = (numY2*r1 - numY1*r2) / den;
    
    /*
      Calculando as coordenadas do ponto C(ponto médio entre i e W1)
    */
    double Cx = (d1*ix + numX1)/(2.*d1);
    double Cy = (d1*iy + numY1)/(2.*d1);
    
    
    /*Quadrado da distância entre i e o ponto W1.*/
    double dx = d1*ix - numX1;
    double dy = d1*iy - numY1;
    double rC_sq = (dx*dx + dy*dy)/(4.*d1*d1);    
    
    /*
      Reta que contem os pontos de interseccão do círculo C
      com o círculo W1.
    */
    double a = numX1 - d1*ix;
    double b = numY1 - d1*iy;
    double c = ix*numX1 + iy*numY1 + (r1*r1 - (numX1*numX1 + numY1*numY1))/d1;
    
    /*
      Coeficientes do polinômio de segundo grau, cuja as raízes
      são as coordenadas x dos pontos de interseccão entre o círculo C
      e o círculo W1.
    */
    double c0 = a*a + b*b;
    double c1 = 2.*(a*c + b*a*Cy - Cx*b*b);
    double c2 = c*c + b*2*c*Cy + b*b*(Cx*Cx + Cy*Cy - rC_sq);
    
    dd rts = Bhaskara::solve(c0,c1,c2);      
             
    double tang_line_c = c*ix + (a*ix + b*iy)*rts.first;        
    double tang_line_a = -(c + b*iy + a*rts.first);
    double tang_line_b = (b*ix - b*rts.first);
    //printf("\nTentando computar 9\n");
    if(ApolloniusCircle::correct_target(tang_line_a,tang_line_b,tang_line_c,numX1,numY1,d1,G0)){             
      //printf("  Computado 9\n");
      centers.push_back(ApolloniusCircle::compute_center(tang_line_a,tang_line_b,tang_line_c,G0));      
    } 
    
    /*
      se esta condicão for verdadeira, 
      o segundo centro será igual o primeiro.
    */
    if(!cmpf(rts.first,rts.second)) return centers;    
             
    tang_line_c = c*ix + (a*ix + b*iy)*rts.second;
    tang_line_a = -(c + b*iy + a*rts.second);
    tang_line_b = (b*ix - b*rts.second);
    //printf("\nTentando computar 10\n");
    if(ApolloniusCircle::correct_target(tang_line_a,tang_line_b,tang_line_c,numX2,numY2,d2,G0)){            
      //printf("  Computado 10\n");     
      centers.push_back(ApolloniusCircle::compute_center(tang_line_a,tang_line_b,tang_line_c,G0));      
    } 
  }
  return centers;
}

std::vector<Point> ApolloniusCircle::apollonius_circle_circum_center(Point & G0, Point & G1, Point & G2){  
  std::vector<Point> center;
  Point rG0 = Point(-G0.y,G0.x);
  Line G0G1(rG0,Point(-G1.y,G1.x)), G0G2(rG0,Point(-G2.y,G2.x));
  if(G0G1.parallel(G0G2)) return center;
  G0G1.translate(Point((G0.x+G1.x)/2,(G0.y+G1.y)/2));
  G0G2.translate(Point((G0.x+G2.x)/2,(G0.y+G2.y)/2));
  center.push_back(G0G1 == G0G2);
  return center;
}