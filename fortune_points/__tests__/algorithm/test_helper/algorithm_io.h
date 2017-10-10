#include <string>
#include <vector>

#include "fortune_points/diagram/voronoi.h"
#include "fortune_points/diagram/site.h"

void read_algorithm_output(std::string dir,
                            std::vector<Edge> &edges,
                            std::vector<Vertice> &vertices);

void read_algorithm_input(std::string dir, std::vector<Site> &sites);