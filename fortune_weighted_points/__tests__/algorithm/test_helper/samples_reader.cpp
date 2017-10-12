#include <stdio.h>
#include <stdlib.h>

#include "fortune_weighted_points/geom/line.h"
#include "fortune_weighted_points/geom/hyperbole.h"
#include "fortune_weighted_points/__tests__/algorithm/test_helper/samples_reader.h"

#include <string>
#include <vector>

/*
  The read_algorithm_output matches
  the test_output_generator.cpp output format.
*/
void read_algorithm_output(std::string dir,
                            std::vector<Edge> &edges,
                            std::vector<Vertice> &vertices){
  FILE *output = fopen(dir.c_str(),"r");
  double a, b, c;
  int numberOfEdges = 0, edgeId, s0, s1;
  fscanf(output,"Number of edges: %d\n",&numberOfEdges);
  while(numberOfEdges--){
    fscanf(output,"ID: %d\n",&edgeId);
    fscanf(output,"Sites: %d %d\n",&s0,&s1);
    Hyperbole h;
    fscanf(output,"Bisector: %lfx^2 + %lfxy + %lfy^2 + %lfx + %lfy + %lf = 0\n\n",
            &h.A,&h.B,&h.C,&h.D,&h.E,&h.F);
    //the wsites created here are just dummy objects to handle
    //the id of each site.
    edges.push_back(Edge(edgeId,h,WSite(s0,0,0,0),WSite(s1,0,0,0)));
  }
  double x,y;
  int numberOfVertices = 0, verticeId, e0, e1, e2;
  fscanf(output,"Number of vertices: %d\n",&numberOfVertices);
  while(numberOfVertices--){
    fscanf(output,"V%d = (%lf, %lf)\n",&verticeId,&x,&y);
    fscanf(output,"Incident edges: %d %d %d\n",&e0,&e1,&e2);
    vertices.push_back(Vertice(vertices.size(),{e0,e1,e2},x,y));
  }
  fclose(output);
}

void read_algorithm_input(std::string dir, std::vector<WSite> &sites){
  FILE *input = fopen(dir.c_str(),"r");
  if(!input){
    printf("asudhasudh\n");
  }
  int N;
  double x, y, wi;
  fscanf(input,"%d",&N);
  while(N--){
    fscanf(input,"%lf %lf %lf",&x,&y,&wi);
    sites.push_back(WSite(sites.size(),x,y,wi));
  }
  fclose(input);
}