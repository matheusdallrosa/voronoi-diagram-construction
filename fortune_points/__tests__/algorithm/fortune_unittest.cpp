#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"
#include "test_helper/samples_reader.h"
#include "fortune_points/diagram/site.h"
#include "fortune_points/diagram/voronoi.h"
#include "fortune_points/algorithm/fortune.h"
#include "common/floating_point.h"

namespace{
  const int NUMBER_OF_TESTS = 26;
  const std::string TEST_INPUT_DIR =
    "fortune_points/__tests__/algorithm/sites_input_samples/input";
  const std::string TEST_OUTPUT_DIR =
    "fortune_points/__tests__/algorithm/diagram_output_samples/output";
  TEST(FortuneAlgorithmTest, DiagramTest){
    for(int i = 0; i < NUMBER_OF_TESTS; i++){
      std::string testCase = std::to_string(i);
      std::vector<Site> sites;
      read_algorithm_input(TEST_INPUT_DIR+testCase+".in",sites);
      std::vector<Edge> correctEdges, computedEdges;
      std::vector<Vertice> correctVertices, computedVertices;
      read_algorithm_output(TEST_OUTPUT_DIR+testCase+".out",
                            correctEdges, correctVertices);
      fortune(sites,computedEdges,computedVertices);
      std::string failMSG = "Fail on input " + testCase + ".";
      ASSERT_EQ(computedEdges.size(), correctEdges.size()) << failMSG;
      for(int j = 0; j < (int)correctEdges.size(); j++){
        //printf("ID: %d\n",computedEdges[j].id);
        //printf("Sites: %d %d\n",computedEdges[j].s0,computedEdges[j].s1);
        //printf("Bisector: %lf %lf %lf\n",computedEdges[j].l.a,computedEdges[j].l.b,computedEdges[j].l.c);
        ASSERT_NEAR(correctEdges[j].l.a,computedEdges[j].l.a,EPS) << failMSG;
        ASSERT_NEAR(correctEdges[j].l.b,computedEdges[j].l.b,EPS) << failMSG;
        ASSERT_NEAR(correctEdges[j].l.c,computedEdges[j].l.c,EPS) << failMSG;
      }
      ASSERT_EQ(computedVertices.size(), correctVertices.size()) << failMSG;
      for(int j = 0; j < (int)correctVertices.size(); j++){
        //printf("%lf,%lf\n",computedVertices[j].x,computedVertices[j].y);
        ASSERT_NEAR(correctVertices[j].x,computedVertices[j].x,EPS) << failMSG;
        ASSERT_NEAR(correctVertices[j].y,computedVertices[j].y,EPS) << failMSG;
      }
      printf("%s\n",("Succeed on input " + testCase + ".").c_str());
    }
  }
}