#include <stdio.h>
#include <math.h>

#include <algorithm>
#include <vector>

#include "common/floating_point.h"

#include "geom/hyperbole.h"
#include "geom/distance.h"

#include "diagram/vertice.h"
#include "diagram/wsite.h"
#include "diagram/edge.h"

#include "algorithm/inter_detector.h"
#include "algorithm/inter_queue.h"
#include "algorithm/boundary.h"
#include "algorithm/status.h"

#include "debug.h"

/* Cancela as interseccões entre a fronteira dada suas vizinhas.*/
void cancel_intersections(Boundary b,closest_boundaries& neighbors,Status& status,InterQueue& inter_queue){    
  inter_queue.rmv_inter(b,neighbors.first);     
  inter_queue.rmv_inter(b,neighbors.second);    
}

/* 
  Caso uma fronteira a seja vertical, e um dos sites de uma fronteira b está contido em a,
  então b deve ser inserida novamente no status.
*/
void reinsert(Boundary a,Boundary b,double W,Status& status,InterQueue& inter_queue){
  if(a.is_vertical() && (a.my_point(b.highest()) || a.my_point(b.lowest()))){
    status.add_boundary(b);
    closest_boundaries neighbors = status.neighbors(b);
    inter_queue.add_inter(InterDetector::detect(neighbors.first,b,W));
    inter_queue.add_inter(InterDetector::detect(neighbors.second,b,W));
  }
}

void make_insertions(Boundary a,double W,closest_boundaries& neighbors,Status& status,InterQueue& inter_queue){    
  inter_queue.add_inter(InterDetector::detect(neighbors.first,a,W));    
  inter_queue.add_inter(InterDetector::detect(neighbors.second,a,W));  
}

void new_edge(std::vector<Edge> &edges,Boundary &b){
  if(b.is_vertical()) edges.push_back(Edge(b.get_id(),b.get_vertical_line(),b.highest(),b.lowest()));
  else edges.push_back(Edge(b.get_id(),b.get_hyperbole(),b.highest(),b.lowest()));
}

bool site_inter_comp(WSite s,Inter i){  
  double dy = s.y+s.get_weight()-i.mapp_y;
  if(cmpf(dy,0) == -1) return true;
  int y_sign = cmpf(dy*dy,i.mapp_dist_squared);
  if(y_sign) return (y_sign == -1);
  /*
    Quando um site e um vértice estão no ponto do plano,
    considero que o vértice vem antes.
  */
  return cmpf(s.x,i.x) <= 0;
}

void fortune(std::vector<WSite> & wsites,double W,
              std::vector<Edge> &edges,std::vector<Vertice> &vertices){
  if((int)wsites.size() == 1) return; //o diagrama é o plano inteiro.
  sort(wsites.begin(),wsites.end(),StarWSiteComp());

  std::vector<WSite>::iterator site = wsites.begin();
    
  Status status;  
  InterQueue inter_queue;  
  
  int bis_id = 0;
  WSite s0 = *(site++), s1 = *(site++);  
  Point m_s1 = Point(s1.x,s1.y+s1.get_weight());
  Boundary B01_minus = Boundary(bis_id,s0,s1,m_s1,MINUS);
  new_edge(edges,B01_minus);
  status.add_boundary(B01_minus);                                               
  if(!B01_minus.is_vertical()) status.add_boundary(Boundary(bis_id,s0,s1,m_s1,PLUS));
  if(DEBUG) {
    status.print(); 
    printf("\n");  
  }
  while(site != wsites.end() || !inter_queue.is_empty()){        
    bis_id++;
    /*evento de site*/
    if(site != wsites.end() && (inter_queue.is_empty() || site_inter_comp(*site,inter_queue.first()))){
      WSite p = *site;
      /*mapeando o site p para sua fronteira(mapeada).*/
      Point mp = Point(p.x,p.y+p.get_weight());
      if(DEBUG) {
        printf("Evento do site:\n"); 
        p.print();      
      }
      /*
        encontrar um pair de fronteiras tal que, a primeira fronteira do pair
        é a primeira fronteira à esquerda de p, e a segunda fronteira do pair
        é a primeira fronteira à direita de p.
      */
      closest_boundaries closest_pair = status.closest_bound(mp);
      
      Boundary left_boundary = closest_pair.first;
      Boundary right_boundary = closest_pair.second;       
      if(DEBUG) {
        printf("Fronteira da esquerda:\n");
        left_boundary.print();
        printf("\nFronteira da direita:\n");
        right_boundary.print();
      }
      bool on_right_bisector = false;
      /*encontrar a região q que contém mp.*/
      WSite q = right_boundary.find_region(mp,on_right_bisector);
            
      /*criacão da parte Cpq- do bissetor Bpq*/
      Boundary Cpq_minus = Boundary(bis_id,p,q,mp,MINUS), Cpq_plus;
      new_edge(edges,Cpq_minus);
      status.add_boundary(Cpq_minus);       
      closest_boundaries neighbors = status.neighbors(Cpq_minus);
      make_insertions(Cpq_minus,W,neighbors,status,inter_queue);            
      
      /*
        A parte Cpq+ só vai ser criada se Cpq não for uma reta vertical, 
        e se p não está contido em right_boundary caso right_boundary seja vertical.
      */   
      if(!Cpq_minus.is_vertical() && !on_right_bisector){
        Cpq_plus = Boundary(bis_id,p,q,mp,PLUS);
        status.add_boundary(Cpq_plus);        
        make_insertions(Cpq_plus,W,neighbors,status,inter_queue);
      }      
      /*verificando se existe interseccão entre as duas fronteiras.*/
      InterCheck check = inter_queue.check(right_boundary,left_boundary); 
      if(check.exists){
        StarBoundaryComp side_check;
        /*caso as duas fronteiras estejam nos extremos do status, talvez não seja necessário cancelar a interseccão.*/                
        if(side_check.comp(mp,right_boundary) == side_check.comp(mp,left_boundary)){
          /*Checar se parte do círculo com centro em p está contido no círculo com centro em check.inter*/                    
          double k = check.inter.mapp_weight-W;          
          double dist_p_inter = Distance::sq_two_points(p,check.inter);          
          double k_prime = dist_p_inter-check.inter.mapp_dist_squared+k*k;          
          if(cmpf(dist_p_inter,check.inter.mapp_dist_squared) == 1 ||
              cmpf(k_prime,0) == 1 || cmpf(k_prime*k_prime,4.*k*k*check.inter.mapp_dist_squared) == -1){          
            inter_queue.rmv_inter(check.inter);          
          }
        }
        else inter_queue.rmv_inter(check.inter);
      }      
      site++;
    }
    /*evento de vértice.*/
    else{ 
      Inter inter = inter_queue.next();                        
      if(DEBUG){
        printf("Evento do Vértice:\n");      
        inter.print();
      } 
      Boundary Cqr = inter.get_Cqr(), Crs = inter.get_Crs();    
      inter_queue.rmv_inter(Cqr,Crs);          
      Boundary Cqs = Boundary(bis_id,inter.tangent[1],inter.tangent[2],inter);   
      new_edge(edges,Cqs);  
      vertices.push_back(Vertice(vertices.size(),std::vector<int>({Cqr.get_id(),Crs.get_id(),Cqs.get_id()}),inter.x,inter.y));
      closest_boundaries Cqr_neigh = status.neighbors(Cqr);
      closest_boundaries Crs_neigh = status.neighbors(Crs);
      cancel_intersections(Cqr,Cqr_neigh,status,inter_queue);             
      cancel_intersections(Cqr,Crs_neigh,status,inter_queue);             
      status.rmv_boundary(Cqr);      
      cancel_intersections(Crs,Cqr_neigh,status,inter_queue);      
      cancel_intersections(Crs,Crs_neigh,status,inter_queue);      
      status.rmv_boundary(Crs);            
      reinsert(Cqr,Crs,W,status,inter_queue);
      reinsert(Crs,Cqr,W,status,inter_queue);
      status.add_boundary(Cqs);
      closest_boundaries neighbors = status.neighbors(Cqs);
      make_insertions(Cqs,W,neighbors,status,inter_queue);
    }
    if(DEBUG) inter_queue.print();
    if(DEBUG) status.print();
  }
}

int main(void){    
  int N;
  scanf("%d",&N); // N > 1
  double W = 0.,x,y,w;
  std::vector<WSite> wsites;
  for(int i = 0; i < N; i++){
    scanf("%lf %lf %lf",&x,&y,&w);
    W = std::max(W,w);
    wsites.push_back(WSite(i,x,y,w));
  }
  std::vector<Edge> edges;
  std::vector<Vertice> vertices;
  fortune(wsites,W,edges,vertices);
  printf("Arestas\n");
  for(auto& e : edges){
    printf("ID: %d\n",e.id);
    printf("Sites: %d %d\n",e.p.get_id(),e.q.get_id());
    if(e.is_vertical) printf("x = %lf\n",e.vertical_line);
    else e.hyperbole.print();
  }
  printf("\nVértices\n");
  for(auto& v : vertices){
    printf("V%d = (%lf, %lf)\n",v.id,v.x,v.y);
    printf("Arestas: %d %d %d\n\n",v.edges[0],v.edges[1],v.edges[2]);
  }
  return 0;
}