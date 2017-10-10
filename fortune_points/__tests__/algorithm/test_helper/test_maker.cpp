#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "fortune_points/geom/point.h"

int N,X,Y;
set<Point> pts;
int main(){
  scanf("%d %d %d",&N,&X,&Y);
  while((int)pts.size() < N){
    int x = rand() % X;
    int y = rand() % Y;
    Point pt = Point(x,y);
    if(pts.find(pt) == pts.end()) pts.insert(pt);
  }
  int cnt = 0;
  for(set<Point>::iterator it = pts.begin(); it != pts.end(); it++){
    printf("%lf %lf\n",it->x,it->y);
    cnt++;
  }
  return 0;
}