/*
5. 도형 넓이 계산기 (공용체와 구조체 융합)
문제: 도형의 종류(원, 사각형)를 나타내는 열거형과,
원의 반지름 또는 사각형의 가로/세로를 저장하는 **공용체(union)**를 만드세요.
이들을 멤버로 가지는 Shape 구조체를 이용해,
사용자가 선택한 도형의 넓이를 출력하는 프로그램을 작성하세요.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>


int main() {

  struct  {
    int kind_of;
    union{
      int r;
      int w;
    } length1;
    union{
      int r;
      int c;
    } length2;
  }shape;

  enum KIND_OF_SHAPE {
    CIRCLE = 1,
    SQUARE
  };

  void calculate_square(int a, int b) {
    int space = 0;

    space = a * b;

    printf("square space is %d \n",space);
  }

  void calculate_circle(int r) {
    float space = 0;
    float PI = 3.14;

    space = PI * r * r;

    printf("circle space is %.2f \n",space);
  }


  printf(" [1] CIRCLE , [2] SQUARE \n");
  printf("type your shape : ");
  scanf("%d", &shape.kind_of);

  switch (shape.kind_of) {
  case CIRCLE:
    printf("type r : ");
    scanf("%d", &shape.length.r);
    calculate_circle(shape.length.r);
    break;
  case SQUARE:
    printf("type row : ");
    scanf("%d", &shape.length.len_sqr.row);
    printf("type col : ");
    scanf("%d", &shape.length.len_sqr.col);
    calculate_square(shape.length.len_sqr.row, shape.length.len_sqr.col);
    break;
  default:
    printf("wrong event \n");
  }

  return 0;
}