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
  read_sites();
  fortune(sites,edges,vertices);

  for(vector<Edge>::iterator it = edges.begin(); it != edges.end(); it++){
    printf("ID: %d\n",it->id);
    printf("Sites: %d %d\n",it->s0,it->s1);
    printf("Bisector: %lfx + %lfy + %lf = 0\n\n",it->l.a,it->l.b,it->l.c);
  }
  int cnt = 0;
  for(vector<Vertice>::iterator it = vertices.begin(); it != vertices.end(); it++){
    printf("V%d = (%lf, %lf)\n",cnt,it->x,it->y);
    cnt++;
    printf("Incident edges: %d %d %d\n",it->b0,it->b1,it->b2);
  }
  return 0;
}
