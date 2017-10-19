#include "gtest/gtest.h"
#include "fortune_points/geom/point.h"
#include "common/floating_point.h"

namespace{
  TEST(PointTest, PointConstructor){
    Point *p = new Point(100000000,300000000);
    ASSERT_NEAR(p->x, 100000000, EPS);
    ASSERT_NEAR(p->y, 300000000, EPS);
    delete p;
  }

  TEST(PointTest, PointLessThanPredicate1){
    Point *p = new Point(100000000,300000000),
          *q = new Point(100000000,300000000.00001);
    ASSERT_TRUE(*p < *q);
    delete p;
    delete q;
  }

  TEST(PointTest, PointLessThanPredicate2){
    Point *p = new Point(100000000,300000000.000001),
          *q = new Point(100000000.000001,300000000.000001);
    ASSERT_TRUE(*p < *q);
    delete p;
    delete q;
  }

  TEST(PointTest, PointEqualsPredicate){
    Point *p = new Point(300000000.000001,300000000.000001),
          *q = new Point(300000000.000001,300000000.000001);
    ASSERT_TRUE(*p == *q);
    delete p;
    delete q;
  }
}
