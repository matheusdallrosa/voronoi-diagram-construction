#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "fortune_weighted_points/algorithm/fortune.h"
#include "fortune_weighted_points/diagram/wsite.h"
#include "fortune_weighted_points/diagram/edge.h"
#include "fortune_weighted_points/diagram/vertice.h"

double W;
vector<WSite> sites;
void read_sites(){
  W = 0;
  double x,y,wi;
  while(scanf("%lf %lf %lf",&x,&y,&wi) != EOF){
    W = std::max(W,wi);
    sites.push_back(WSite(sites.size(),x,y,wi));
  }
}

vector<Edge> edges;
vector<Vertice> vertices;

int main(void){
  read_sites();
  fortune(sites,W,edges,vertices);
  printf("Number of edges: %d\n",(int)edges.size());
  for(vector<Edge>::iterator it = edges.begin(); it != edges.end(); it++){
    printf("ID: %d\n",it->id);
    printf("Sites: %d %d\n",it->p.get_id(),it->q.get_id());
    Hyperbole h = it->hyperbole;
    printf("Bisector: %lfx^2 + %lfxy + %lfy^2 + %lfx + %lfy + %lf = 0\n\n",
            h.A,h.B,h.C,h.D,h.E,h.F);
  }
  int cnt = 0;
  printf("Number of vertices: %d\n",(int)vertices.size());
  for(vector<Vertice>::iterator it = vertices.begin(); it != vertices.end(); it++){
    printf("V%d = (%lf, %lf)\n",cnt,it->x,it->y);
    cnt++;
    printf("Incident edges: %d %d %d\n",it->edges[0],it->edges[1],it->edges[2]);
  }
  return 0;
}
