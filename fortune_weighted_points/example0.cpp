#include <stdio.h>
#include <math.h>

#include <algorithm>
#include <vector>

#include "fortune_weighted_points/algorithm/fortune.h"
#include "fortune_weighted_points/diagram/vertice.h"
#include "fortune_weighted_points/diagram/wsite.h"
#include "fortune_weighted_points/diagram/edge.h"

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
