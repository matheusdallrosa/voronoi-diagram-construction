#include <string>
#include <vector>

#include "fortune_weighted_points/diagram/edge.h"
#include "fortune_weighted_points/diagram/vertice.h"
#include "fortune_weighted_points/diagram/wsite.h"

void read_algorithm_output(std::string dir,
                            std::vector<Edge> &edges,
                            std::vector<Vertice> &vertices);

void read_algorithm_input(std::string dir, std::vector<WSite> &sites);