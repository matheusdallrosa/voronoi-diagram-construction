#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"
#include "test_helper/samples_reader.h"
#include "fortune_weighted_points/diagram/edge.h"
#include "fortune_weighted_points/diagram/wsite.h"
#include "fortune_weighted_points/diagram/vertice.h"
#include "fortune_weighted_points/algorithm/fortune.h"
#include "fortune_weighted_points/common/floating_point.h"

namespace{
  const int NUMBER_OF_TESTS = 30;
  const std::string TEST_INPUT_DIR =
    "fortune_weighted_points/__tests__/algorithm/sites_input_samples/input";
  const std::string TEST_OUTPUT_DIR =
    "fortune_weighted_points/__tests__/algorithm/diagram_output_samples/output";
  TEST(FortuneAlgorithmTest, DiagramTest){
    for(int i = 0; i < NUMBER_OF_TESTS; i++){
      std::string testCase = std::to_string(i);
      std::vector<WSite> sites;
      read_algorithm_input(TEST_INPUT_DIR+testCase+".in",sites);
      std::vector<Edge> correctEdges, computedEdges;
      std::vector<Vertice> correctVertices, computedVertices;
      read_algorithm_output(TEST_OUTPUT_DIR+testCase+".out",
                            correctEdges, correctVertices);
      double W = 0;
      for(auto& ws : sites) W = std::max(W,ws.get_weight());
      fortune(sites,W,computedEdges,computedVertices);
      std::string testFailMSG = "Fail on input " + testCase + ".";
      ASSERT_EQ(computedEdges.size(), correctEdges.size()) << testFailMSG;
      for(int j = 0; j < (int)correctEdges.size(); j++){
        std::string edgeFailMSG = testFailMSG+" On Edge: "+std::to_string(j);
        ASSERT_EQ(correctEdges[j].id,computedEdges[j].id) << edgeFailMSG;
        ASSERT_EQ(correctEdges[j].is_vertical,computedEdges[j].is_vertical) << edgeFailMSG;
        if(correctEdges[j].is_vertical){
          ASSERT_NEAR(correctEdges[j].vertical_line,computedEdges[j].vertical_line,EPS) << edgeFailMSG;
        }
        else{
          Hyperbole h0 = correctEdges[j].hyperbole, h1 = computedEdges[j].hyperbole;
          ASSERT_NEAR(h0.A,h1.A,EPS) << edgeFailMSG;
          ASSERT_NEAR(h0.B,h1.B,EPS) << edgeFailMSG;
          ASSERT_NEAR(h0.C,h1.C,EPS) << edgeFailMSG;
          ASSERT_NEAR(h0.D,h1.D,EPS) << edgeFailMSG;
          ASSERT_NEAR(h0.E,h1.E,EPS) << edgeFailMSG;
          ASSERT_NEAR(h0.F,h1.F,EPS) << edgeFailMSG;
        }
      }
      ASSERT_EQ(computedVertices.size(), correctVertices.size()) << testFailMSG;
      for(int j = 0; j < (int)correctVertices.size(); j++){
        std::string verticeFailMSG = testFailMSG+" On Vertice: "+std::to_string(j);
        ASSERT_EQ(correctVertices[j].id,computedVertices[j].id) << verticeFailMSG;
        ASSERT_NEAR(correctVertices[j].edges[0],computedVertices[j].edges[0],EPS) << verticeFailMSG;
        ASSERT_NEAR(correctVertices[j].edges[1],computedVertices[j].edges[1],EPS) << verticeFailMSG;
        ASSERT_NEAR(correctVertices[j].edges[2],computedVertices[j].edges[2],EPS) << verticeFailMSG;
        ASSERT_NEAR(correctVertices[j].x,computedVertices[j].x,EPS) << verticeFailMSG;
        ASSERT_NEAR(correctVertices[j].y,computedVertices[j].y,EPS) << verticeFailMSG;
      }
      printf("%s\n",("Succeed on input " + testCase + ".").c_str());
    }
  }
}