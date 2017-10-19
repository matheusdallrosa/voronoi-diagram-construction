#include "gtest/gtest.h"
#include "fortune_points/geom/line.h"
#include "fortune_points/geom/point.h"
#include "common/floating_point.h"

namespace{
  TEST(LineTest, LineCoefficientConstructor){
    double a, b, c;
    a = b = c = 1000000000;
    Line *l = new Line(a, b, c);
    ASSERT_NEAR(l->a, a, EPS);
    ASSERT_NEAR(l->b, b, EPS);
    ASSERT_NEAR(l->c, c, EPS);
    delete l;
  }

  TEST(LineTest, LinePointConstructor1){
    Point p1 = Point(1000000000, 1000000000),
          p2 = Point(1000000000, 0);
    Line *l = new Line(p1,p2);
    ASSERT_EQ(l->a, 1.);
    ASSERT_EQ(l->b, 0.);
    ASSERT_NEAR(l->c, -p1.x, EPS);
    ASSERT_NEAR(l->relate(p1), 0, EPS);
    ASSERT_NEAR(l->relate(p2), 0, EPS);
    delete l;
  }

  TEST(LineTest, LinePointConstructor2){
    Point p1 = Point(1000000000, 1000000000),
          p2 = Point(-1000000000, -1000000000);
    Line *l = new Line(p1,p2);
    ASSERT_NEAR(l->a, -1, EPS);
    ASSERT_EQ(l->b, 1.);
    ASSERT_NEAR(l->c, 0, EPS);
    ASSERT_NEAR(l->relate(p1), 0, EPS);
    ASSERT_NEAR(l->relate(p2), 0, EPS);
    delete l;
  }

  TEST(LineTest, LinePointConstructor3){
    Point p1 = Point(1000000000, 1000000000),
          p2 = Point(-1000000000, -1000000000-0.000001);
    Line *l = new Line(p1,p2);
    ASSERT_NEAR(l->a, -1, EPS);
    ASSERT_EQ(l->b, 1.);
    ASSERT_NEAR(l->c, -0.000000448, EPS);
    ASSERT_NEAR(l->relate(p1), 0, EPS);
    ASSERT_NEAR(l->relate(p2), 0, EPS);
    delete l;
  }

  TEST(LineTest, LinePointConstructor4){
    Point p1 = Point(1000000000, 1000000000),
          p2 = Point(100000, 1000000000);
    Line *l = new Line(p1,p2);
    ASSERT_NEAR(l->a, 0, EPS);
    ASSERT_EQ(l->b, 1.);
    ASSERT_NEAR(l->c, -1000000000, EPS);
    ASSERT_NEAR(l->relate(p1), 0, EPS);
    ASSERT_NEAR(l->relate(p2), 0, EPS);
    delete l;
  }

  TEST(LineTest, LineParallelMethod){
    Point p1 = Point(-2,2),
          p2 = Point(4.26, 2.11);
    Line *l1 = new Line(p1,p2);
    Line *l2 = new Line(-0.017571885, 1., -1.614073163);
    ASSERT_TRUE(l1->parallel(*l2));
    ASSERT_TRUE(l2->parallel(*l1));
    delete l1;
    delete l2;
  }

  TEST(LineTest, LineRelateMethod1){
    Point p1 = Point(1.68,3.62),
          p2 = Point(4.84,4.12);
    Line *l = new Line(p1,p2);
    ASSERT_GT(l->relate(Point(1,5)),0);
    ASSERT_GT(l->relate(Point(2,5)),0);
    ASSERT_GT(l->relate(Point(9,5)),0);
    ASSERT_LT(l->relate(Point(9,3)),0);
    ASSERT_LT(l->relate(Point(5,3)),0);
    ASSERT_LT(l->relate(Point(0,3)),0);
    delete l;
  }

  TEST(LineTest, LineIsVerticalMethod){
    Point p1 = Point(1.6800011,0),
          p2 = Point(1.6800012,0);
    Line *l = new Line(p1,p2);
    ASSERT_TRUE(l->is_vertical());
    delete l;
  }

  TEST(LineTest, LineTranslateMethod){
    Point p1 = Point(1.86,1.3),
          p2 = Point(3.62,5.02);
    Line *l1 = new Line(p1,p2);
    Line *l2 = new Line(p1,p2);
    Point p3 = Point(123124,12398);
    l2->translate(p3);
    ASSERT_TRUE(l1->parallel(*l2));
    ASSERT_NEAR(l2->relate(p3), 0, EPS);
    delete l1;
    delete l2;
  }

  TEST(LineTest, LineIntersectionOperator){
    Point p1 = Point(1.86,1.3);
    Point p2 = Point(3.62,5.02);
    Line *l1 = new Line(p1,p2);

    Point p3 = Point(-1.9,2.3);
    Point p4 = Point(-3.06,-5.24);
    Line *l2 = new Line(p3,p4);

    Point p = *l1 == *l2;
    ASSERT_NEAR(l1->relate(p), 0, EPS);
    ASSERT_NEAR(l2->relate(p), 0, EPS);
    delete l1;
    delete l2;
  }
}
