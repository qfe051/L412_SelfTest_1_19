/*
2. 점 사이의 거리 계산 (구조체와 함수)
문제: 2차원 좌표 x, y를 저장하는 Point 구조체를 만드세요.
두 점의 좌표를 입력받아, 두 점 사이의 거리를 계산하여 반환하는
double getDistance(Point p1, Point p2) 함수를 작성하세요.

*/

#include <stdio.h>
#include <string.h>
#include <math.h>

  struct point {
    float x;
    float y;
  };

  struct point p[2];

  double getDistance(struct point p1,struct point p2) {
    double a = 0.0;
    double b = 0.0;
    double distance = 0.0;

    a = (p1.x - p2.x) * (p1.x - p2.x);
    b = (p1.y - p2.y) * (p1.y - p2.y);

    distance = sqrt(a + b);
    
    return distance;
  }

int main() {



  for (int i = 0; i < 2; i++) {
    printf("type p[%d].x: ",i);
    scanf("%f",&p[i].x);

    printf("type p[%d].y: ",i);
    scanf("%f",&p[i].y);
  }

  printf("distance a-b : %f ",getDistance(p[0],p[1]));



  return 0;
}