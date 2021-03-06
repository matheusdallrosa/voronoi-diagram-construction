#include <stdio.h>
#include <stdlib.h>

#include "fortune_points/geom/line.h"
#include "fortune_points/__tests__/algorithm/test_helper/samples_reader.h"

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
    fscanf(output,"Bisector: %lfx + %lfy + %lf = 0\n\n",&a,&b,&c);
    edges.push_back(Edge(edgeId,Line(a,b,c),s0,s1));
  }
  double x,y;
  int numberOfVertices = 0, verticeId, e0, e1, e2;
  fscanf(output,"Number of vertices: %d\n",&numberOfVertices);
  while(numberOfVertices--){
    fscanf(output,"V%d = (%lf, %lf)\n",&verticeId,&x,&y);
    fscanf(output,"Incident edges: %d %d %d\n",&e0,&e1,&e2);
    vertices.push_back(Vertice(x,y,e0,e1,e2));
  }
  fclose(output);
}

void read_algorithm_input(std::string dir,std::vector<Site> &sites){
  FILE *input = fopen(dir.c_str(),"r");
  if(!input){
    printf("asudhasudh\n");
  }
  double x, y;
  while(fscanf(input,"%lf %lf",&x,&y) != EOF){
    sites.push_back(Site(sites.size(),x,y));
  }
  fclose(input);
}