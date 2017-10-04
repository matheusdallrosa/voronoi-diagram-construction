#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "debug.h"
#include "status.h"
#include "bisector.h"
#include "inter_queue.h"
#include "fortune_points/geom/floating_point.h"
#include "fortune_points/geom/point.h"
#include "fortune_points/geom/line.h"
#include "fortune_points/geom/distance.h"
#include "fortune.h"

void fortune(std::vector<Site> &sites,
              std::vector<Edge> &edges,
              std::vector<Vertice> &vertices){
  int bis_id = 0;

  std::sort(sites.begin(),sites.end());
  std::vector<Site>::iterator site = sites.begin();

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
    /*id that will be used for the bisector that will be created on the next event*/
    bis_id++;
    if(site != sites.end() && (interq.is_empty() || *site < *(interq.first()))){
      /*L.y = p.y*/
      Site p = *site;

      if(DEBUG) {
        printf("Evento do site:\n");
        p.print();
      }
      /*
        Region_Bound.first = Owner of the region that contains p.
        Region_bound.second = Rightmost boundary from the region of Region_Bound.first.
      */
      Region_Bound reg_bisector = status.find_region(p);
      Site q = reg_bisector.first;
      if(DEBUG) {
        printf("Bissetor encontrado:\n");
        reg_bisector.second.print();
        printf("Região encontrada: %d\n\n",q.id);
      }
      /*Search for the left neighboor for test intersection.*/
      Bisector neigh = status.left_neighbor(reg_bisector.second);

      /*MINUS part of the new boundary.*/
      Bisector Cpq_m = status.insert(Bisector(bis_id,MINUS,p,q,p)),Cpq_p;
      edges.push_back(Edge(Cpq_m.id,Cpq_m.l,Cpq_m.s0.id,Cpq_m.s1.id));

      /*Checking if the new boundary is vertical, and if it is check if
        it's also contained on Cpq_m.*/
      bool create_plus_bound = !Cpq_m.vertical() &&
                                !(reg_bisector.second.vertical() && !cmpf(p.x,-reg_bisector.second.l.c));
      if(create_plus_bound){
        /*Creating the part PLUS of the new Boundary*/
        Cpq_p = status.insert(Bisector(bis_id,PLUS,p,q,p));
      }

      /*
        Testing intersection from reg_bisector with its neighborhood.
      */
      Inter_Check ic = interq.check(reg_bisector.second.id,reg_bisector.second.pm,neigh.id,neigh.pm);
      if(ic.first){
        /*
          If there is an intersection, we have to check if the site p
          is to the left from the two intercepting boundaries(neigh e reg_bisector.s),
          or the site p is in between the boundaries.
        */
        double x0 = reg_bisector.second.xcoord(p.y);
        double x1 = neigh.xcoord(p.y);
        /*Checking if p is to the right or left from both boundaries.*/
        if((cmpf(p.x,x0) == 1 && cmpf(p.x,x1) == 1) ||
            (cmpf(p.x,x0) == -1 && cmpf(p.x,x1) == -1)){
          /*
            We only remove the intersection if p is inside the
            Voronoi circle.
          */
          Inter inter = *(ic.second);
          if(cmpf(Distance::two_points(p,Point(inter.x,inter.non_mapp_y)), inter.star_val) == -1)
            interq.cancel(neigh.id,neigh.pm,reg_bisector.second.id,reg_bisector.second.pm);

        }
        /*If p is in between both boundaries, then p is inside the Voronoi circle.*/
        else interq.cancel(neigh.id,neigh.pm,reg_bisector.second.id,reg_bisector.second.pm);
      }

      /*
        Checking if the MINUS part of the boundary does intercept with
        its neighborhood.
      */
      bool check;
      Inter new_inter = status.left_intersec(Cpq_m,check);
      if(check) interq.enqueue(new_inter);
      new_inter = status.right_intersec(Cpq_m,check);
      if(check) interq.enqueue(new_inter);

      /*
        If p and q don't belong to the same horizontal line.
      */
      if(create_plus_bound){
        /*
          We check if the PLUS part of the boundary does intercept
          with its neighborhood.
        */
        new_inter = status.left_intersec(Cpq_p,check);
        if(check) interq.enqueue(new_inter);
        new_inter = status.right_intersec(Cpq_p,check);
        if(check) interq.enqueue(new_inter);
      }
      site++;
    }
    /*vertice event*/
    else{
      /*
        inter is the intersection of two boundaries that is being intercepted
        by L, so L.y = inter.y
      */
      Inter inter = interq.next();
      if(DEBUG){
        printf("Evento do Vértice:\n");
        inter.print();
      }
      /*Creating the new boundary Cqs.*/
      Site q,s;
      Bisector Cqr = inter.b0, Crs = inter.b1;
      q = (Cqr.s0.id == Crs.s0.id || Cqr.s0.id == Crs.s1.id) ? Cqr.s1 : Cqr.s0;
      s = (Crs.s0.id == Cqr.s0.id || Crs.s0.id == Cqr.s1.id) ? Crs.s1 : Crs.s0;

      /*
        If one of the intersecting bisector is vertical, we have to check
        in which side the intersection is.
      */
      Bisector Cqs = Bisector(bis_id,FAKE_BISECTOR,q,s,inter);
      if(Cqr.vertical()){
        int diff = cmpf(inter.x,-Cqr.l.c);
        /*Maybe there is floating point error on the above comparison.*/
        if(diff) Cqs.pm = (diff == -1) ? PLUS : MINUS;
        else Cqs.pm = cmpf(Crs.s1.x,-Cqr.l.c) <= 0 ? PLUS : MINUS;
      }
      else if(Crs.vertical()){
        int diff = cmpf(inter.x,-Crs.l.c);
        /*Maybe there is floating point error on the above comparison.*/
        if(diff) Cqs.pm = (diff == -1) ? PLUS : MINUS;
        else Cqs.pm = cmpf(Cqr.s1.x,-Crs.l.c) <= 0 ? PLUS : MINUS;
      }
      /*
        To now the side of the new boundary we have to compare the inter's x
        coordinate with the highest site's coordinate.
      */
      else Cqs.pm = cmpf(inter.x,std::max(Cqr.s1,Crs.s1).x) <= 0 ? MINUS : PLUS;

      edges.push_back(Edge(Cqs.id,Cqs.l,Cqs.s0.id,Cqs.s1.id));

      vertices.push_back(Vertice(inter.x,inter.non_mapp_y,Cqr.id,Crs.id,Cqs.id));

      /*Canceling the intersections between inter and its neighborhood.*/
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

      /*If a boundary A is vertical, and one of the sites from
        other boundary B belongs to A the boundary B must be reinserted.*/
      if(Crs.vertical() && (Crs.mypoint(Cqr.s0.x) || Crs.mypoint(Cqr.s1.x))){
        /*Obtendo o iterator para Cqr, parar poder testar novas interseccoes.*/
        Bisector Cqr_idx = status.insert(Cqr);

        /*Looking for new intersections.*/
        bool check;
        Inter new_inter = status.left_intersec(Cqr_idx,check);
        if(check) interq.enqueue(new_inter);
        new_inter = status.right_intersec(Cqr_idx,check);
        if(check) interq.enqueue(new_inter);
      }
      else if(Cqr.vertical() && (Cqr.mypoint(Crs.s0.x) || Cqr.mypoint(Crs.s1.x))){
        Bisector Crs_idx = status.insert(Crs);
        /*Testing for new intersections.*/
        bool check;
        Inter new_inter = status.left_intersec(Crs_idx,check);
        if(check) interq.enqueue(new_inter);
        new_inter = status.right_intersec(Crs_idx,check);
        if(check) interq.enqueue(new_inter);
      }
      /*Current position of the new boundary.*/
      Bisector Cqs_idx = status.insert(Cqs);

      /*Testing for new intersections.*/
      bool check;
      Inter new_inter = status.left_intersec(Cqs_idx,check);
      if(check) interq.enqueue(new_inter);
      new_inter = status.right_intersec(Cqs_idx,check);
      if(check) interq.enqueue(new_inter);
    }
    if(DEBUG) status.print();
  }
}
