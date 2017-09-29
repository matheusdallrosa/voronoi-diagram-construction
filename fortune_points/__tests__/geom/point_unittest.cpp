#include "gtest/gtest.h"
#include "fortune_points/geom/point.h"

namespace{
TEST(PointTest, PointConstructor){
  Point *p = new Point(2,3);
  ASSERT_NEAR(p->x, 2, 1.0e-11);
}
}
