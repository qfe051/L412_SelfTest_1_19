/*
1. 두 변수의 값 바꾸기 (Call by Reference)
• 문제: void swap(int *p1, int *p2) 함수를 만드세요.
메인 함수에서 a = 10, b = 20을 선언한 뒤,
이 함수를 호출하여 두 변수의 값을 서로 바꾸고 출력하세요.

*/
#include <stdio.h>

void swap(int *p1, int *p2) {
  int temp = 0;
  temp = *p2;
  *p2 = *p1;
  *p1 = temp;
}


int main() {
  int a = 10;
  int b = 20;

    // 주소 대입
    swap(&a, &b);

  printf("a : %d \n", a);
  printf("b : %d \n", b);
  


}
