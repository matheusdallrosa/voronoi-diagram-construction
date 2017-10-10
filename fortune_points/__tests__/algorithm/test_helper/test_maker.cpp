#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

struct Point{
  int x,y;
  Point(){}
  Point(int _x,int _y){
    x = _x, y = _y;
  }
  bool operator<(Point o)const{
    return (y != o.y) ? y < o.y : x < o.x; 
  }
};

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
    printf("%d %d\n",it->x,it->y);
    cnt++;
  }
  return 0;
}