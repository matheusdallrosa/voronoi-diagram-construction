#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "fortune_points/algorithm/fortune.h"
#include "fortune_points/diagram/voronoi.h"
#include "fortune_points/diagram/site.h"

vector<Site> sites;
void read_sites(){
  double x,y;
  while(scanf("%lf %lf",&x,&y) != EOF)
    sites.push_back(Site(sites.size(),x,y));
}

vector<Edge> edges;
vector<Vertice> vertices;

int main(void){
  printf("Lendo sites..\n");
  read_sites();
  printf("Computando diagrama..\n");
  fortune(sites,edges,vertices);
  /*
  if(sz(sites) <= 10000){
      for(vector<Vertice>::iterator it = vertices.begin(); it != vertices.end(); it++){
          double mapp = dist(*it,sites[edges[it->b0].s0]);
          for(int i = 0; i < sz(sites); i++){
            double dt = dist(sites[i],*it);
            if(cmpf(dt,mapp) == -1){
              printf("Wrong diagram.\n");
              printf("%.20lf\n",EPS);
              printf("%.20lf %.20lf\n",dt,mapp);
              return 0;
            }
          }
      }
      printf("Correct diagram.\n");
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
