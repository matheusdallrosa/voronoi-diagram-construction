#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "debug.h"
#include "status.h"
#include "bisector.h"
#include "inter_queue.h"
#include "geom/floating_point.h"
#include "geom/point.h"
#include "geom/line.h"
#include "geom/distance.h"
#include "diagram/site.h"
#include "diagram/voronoi.h"

vector<Site> sites;
void read_sites(){
  double x,y;
  while(scanf("%lf %lf",&x,&y) != EOF)
    sites.push_back(Site(sites.size(),x,y));
}

vector<Edge> edges;
vector<Vertice> vertices;
void fortune(){
  int bis_id = 0;
  
  sort(sites.begin(),sites.end()); 
  vector<Site>::iterator site = sites.begin();
  
  Inter_Queue interq; 
  
  Status status;
  
  Site s0 = *(site++),s1 = *(site++);  
  Bisector B01_m = Bisector(bis_id,MINUS,s0,s1,s1);  
  status.insert(B01_m);      
  edges.push_back(Edge(B01_m.id,B01_m.l,B01_m.s0.id,B01_m.s1.id));
  if(!B01_m.vertical()){
    Bisector B01_p = Bisector(bis_id,PLUS,s0,s1,s1);
    status.insert(B01_p);        
  }
  if(DEBUG) {
    status.print(); 
    printf("\n");  
  }
  while(site != sites.end() || !interq.is_empty()){   
    /*id das fronteiras que serao criadas no proximo evento.*/
    bis_id++;  
    if(site != sites.end() && (interq.is_empty() || *site < *(interq.first()))){
      /*p e o site que esta sendo interceptado por L, portanto L.y = p.y*/
      Site p = *site;
      
      if(DEBUG) {
        printf("Evento do site:\n"); 
        p.print();      
      }
      /*
        Region_Bound.first = Site q dono da regiao que contem p.
        Region_bound.second = Fronteira mais a direita na região q.
      */
      Region_Bound reg_bisector = status.find_region(p);
      Site q = reg_bisector.first;      
      if(DEBUG) {
        printf("Bissetor encontrado:\n");
        reg_bisector.second.print();
        printf("Região encontrada: %d\n\n",q.id);
      }
      /*Obtendo fronteira vizinha para testar interseccao.*/
      Bisector neigh = status.left_neighbor(reg_bisector.second);  
      
      /*Criacao da parte MINUS da fronteira.*/        
      Bisector Cpq_m = status.insert(Bisector(bis_id,MINUS,p,q,p)),Cpq_p;     
      edges.push_back(Edge(Cpq_m.id,Cpq_m.l,Cpq_m.s0.id,Cpq_m.s1.id));
      
      /*Verificando se a fronteira encontrada e vertical e se q esta contido nessa fronteira.*/      
      bool create_plus_bound = !Cpq_m.vertical() && 
                                !(reg_bisector.second.vertical() && !cmpf(p.x,-reg_bisector.second.l.c));                                     
      if(create_plus_bound){
        /*Criando a parte PLUS da fronteira.*/                
        Cpq_p = status.insert(Bisector(bis_id,PLUS,p,q,p));
      }           
          
      /*
        Testando a interseccao com a fronteira vizinha.
        ic.f = true se existe interseccao, senao = false.
      */      
      Inter_Check ic = interq.check(reg_bisector.second.id,reg_bisector.second.pm,neigh.id,neigh.pm);
      if(ic.first){        
        /*
          Caso existir interseccao devemos testar se:
          O site p esta contido a esquerda das duas fronteiras que 
          se interceptam(neigh e reg_bisector.s). 
          Ou o site p está entre as duas fronteiras.
        */
        double x0 = reg_bisector.second.xcoord(p.y);
        double x1 = neigh.xcoord(p.y);
        /*Verificando se p esta a direita ou a esquerda das duas fronteiras.*/
        if((cmpf(p.x,x0) == 1 && cmpf(p.x,x1) == 1) ||
            (cmpf(p.x,x0) == -1 && cmpf(p.x,x1) == -1)){          
          /*
            So cancelaremos a interseccao se p estiver no interior do 
            circulo de Voronoi.
          */
          Inter inter = *(ic.second);          
          if(cmpf(Distance::two_points(p,Point(inter.x,inter.non_mapp_y)), inter.star_val) == -1)
            interq.cancel(neigh.id,neigh.pm,reg_bisector.second.id,reg_bisector.second.pm);          
          
        }
        /*Se p esta entre as duas fronteiras, entao p esta no interior do circulo.*/
        else interq.cancel(neigh.id,neigh.pm,reg_bisector.second.id,reg_bisector.second.pm);
      }
          
      /*
        Checando se a parte MINUS da fronteira nao se intercepta com sua 
        vizinha a esquerda(temo que em alguns casos devo testar o vizinho 
        da direita tbm.) 
      */                 
      bool check;
      Inter new_inter = status.left_intersec(Cpq_m,check);
      if(check) interq.enqueue(new_inter);
      new_inter = status.right_intersec(Cpq_m,check);
      if(check) interq.enqueue(new_inter);
      
      /*
        Se p e q nao estiverem sobre a mesma linha horizontal.
      */
      if(create_plus_bound){
        /*
          Checamos se a parte PLUS da fronteira se intercepta 
          com seu vizinho a direita.
        */                    
        new_inter = status.left_intersec(Cpq_p,check);
        if(check) interq.enqueue(new_inter);
        new_inter = status.right_intersec(Cpq_p,check);
        if(check) interq.enqueue(new_inter);
      }            
      site++;
    }
    /*caso contrario, devemos processar um evento de vertice.*/
    else{      
      /*
        inter e a interseccao que esta sendo interceptado por L, portanto
        L.y = inter.y
      */     
      Inter inter = interq.next();    
      if(DEBUG){
        printf("Evento do Vértice:\n");      
        inter.print();
      }        
      /*Criando a nova fronteira Cqs.*/
      Site q,s;
      Bisector Cqr = inter.b0, Crs = inter.b1;
      q = (Cqr.s0.id == Crs.s0.id || Cqr.s0.id == Crs.s1.id) ? Cqr.s1 : Cqr.s0;
      s = (Crs.s0.id == Cqr.s0.id || Crs.s0.id == Cqr.s1.id) ? Crs.s1 : Crs.s0;                     
      
      /*
        Caso um dos bissetores seja vertical temos que testar de 
        qual lado a interseccao esta.
      */
      Bisector Cqs = Bisector(bis_id,FAKE_BISECTOR,q,s,inter);   
      if(Cqr.vertical()){
        int diff = cmpf(inter.x,-Cqr.l.c);        
        //caso exista algum erro de ponto flutuante na comparação que resultou em diff.
        if(diff) Cqs.pm = (diff == -1) ? PLUS : MINUS;
        /*
          Caso a interseccao esteja sobre uma fronteira vertical olhamos
          para o coeficiente angular da reta.
        */
        else Cqs.pm = cmpf(Crs.s1.x,-Cqr.l.c) <= 0 ? PLUS : MINUS;
      }
      else if(Crs.vertical()){
        int diff = cmpf(inter.x,-Crs.l.c);
        //caso exista algum erro de ponto flutuante na comparação que resultou em diff.
        if(diff) Cqs.pm = (diff == -1) ? PLUS : MINUS;
        else Cqs.pm = cmpf(Cqr.s1.x,-Crs.l.c) <= 0 ? PLUS : MINUS;
      }
      /*
        Para descobrir o lado da nova fronteira, precisamos comprar o x da
        interseccao com o x do site mais acima.      
      */
      else Cqs.pm = cmpf(inter.x,max(Cqr.s1,Crs.s1).x) <= 0 ? MINUS : PLUS;
      
      /*Armazenando a nova aresta.*/
      edges.push_back(Edge(Cqs.id,Cqs.l,Cqs.s0.id,Cqs.s1.id));
      
      /*Armazenando o novo vertice.*/
      vertices.push_back(Vertice(inter.x,inter.non_mapp_y,Cqr.id,Crs.id,Cqs.id)); 
      
      /*
        Cancelando as interseccoes com os vizinhos
        das fronteiras que se interceptam em inter.
      */            
      Bisector neigh = status.left_neighbor(inter.b0);      
      interq.cancel(neigh.id,neigh.pm,inter.b0.id,inter.b0.pm);      
      neigh = status.right_neighbor(inter.b0);      
      interq.cancel(neigh.id,neigh.pm,inter.b0.id,inter.b0.pm);                                                                  
      status.erase(inter.b0);            
      
      neigh = status.left_neighbor(inter.b1);
      interq.cancel(neigh.id,neigh.pm,inter.b1.id,inter.b1.pm);      
      neigh = status.right_neighbor(inter.b1);
      interq.cancel(neigh.id,neigh.pm,inter.b1.id,inter.b1.pm);      
      status.erase(inter.b1);
      
      /*Caso uma fronteira A seja vertical, e um dos sites que compoem a 
        fronteira B estiver sobre A, a fronteira B deve ser inserida novamente.*/
      if(Crs.vertical() && (Crs.mypoint(Cqr.s0.x) || Crs.mypoint(Cqr.s1.x))){                
        /*Obtendo o iterator para Cqr, parar poder testar novas interseccoes.*/    
        Bisector Cqr_idx = status.insert(Cqr);  
        
        /*Testando novas interseccoes.*/
        bool check;      
        Inter new_inter = status.left_intersec(Cqr_idx,check);      
        if(check) interq.enqueue(new_inter);
        new_inter = status.right_intersec(Cqr_idx,check);      
        if(check) interq.enqueue(new_inter);                                         
      }             
      else if(Cqr.vertical() && (Cqr.mypoint(Crs.s0.x) || Cqr.mypoint(Crs.s1.x))){        
        /*Obtendo o iterator para Crs, parar poder testar novas interseccoes.*/    
        Bisector Crs_idx = status.insert(Crs);  
        /*Testando novas interseccoes.*/
        bool check;      
        Inter new_inter = status.left_intersec(Crs_idx,check);      
        if(check) interq.enqueue(new_inter);
        new_inter = status.right_intersec(Crs_idx,check);      
        if(check) interq.enqueue(new_inter);                                         
      }
      /*Obtendo o iterator para Cqs, parar poder testar novas interseccoes.*/    
      Bisector Cqs_idx = status.insert(Cqs);               
      
      /*Testando novas interseccoes.*/
      bool check;      
      Inter new_inter = status.left_intersec(Cqs_idx,check);      
      if(check) interq.enqueue(new_inter);
      new_inter = status.right_intersec(Cqs_idx,check);      
      if(check) interq.enqueue(new_inter);                            
    }     
    if(DEBUG) status.print();
  }  
}

int main(void){   
  printf("Lendo sites..\n");
  read_sites();
  printf("Computando diagrama..\n");
  fortune(); 
  /*
  if(sz(sites) <= 10000){
      for(vector<Vertice>::iterator it = vertices.begin(); it != vertices.end(); it++){
          double mapp = dist(*it,sites[edges[it->b0].s0]);
          for(int i = 0; i < sz(sites); i++){
            double dt = dist(sites[i],*it);
            if(cmpf(dt,mapp) == -1){
              printf("Diagrama computado incorretamente.\n");
              printf("%.20lf\n",EPS);
              printf("%.20lf %.20lf\n",dt,mapp);
              return 0;
            }          
          }
      }      
      printf("Diagrama computado corretamente.\n");
  } */ 
  printf("Sites\n");
  for(int i = 0; i < (int)sites.size(); i++)
    printf("S%d = (%lf,%lf)\n",i,sites[i].x,sites[i].y);  
  printf("\nBisector:\n");
  for(vector<Edge>::iterator it = edges.begin(); it != edges.end(); it++){
    printf("ID: %d\n",it->id); 
    printf("Sites: %d %d\n",it->s0,it->s1);   
    printf("Bissetor: %.2lfx + %.2lfy + %.2lf = 0\n\n",it->l.a,it->l.b,it->l.c);
  }
  printf("Vertices:\n");
  int cnt = 0;
  for(vector<Vertice>::iterator it = vertices.begin(); it != vertices.end(); it++){
    printf("V%d = (%lf, %lf)\n",cnt,it->x,it->y);
    cnt++;
    printf("Arestas incidentes: %d %d %d\n\n",it->b0,it->b1,it->b2);
  }
  return 0;
}