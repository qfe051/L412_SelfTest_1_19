/*
2. 포인터 연산으로 배열 합계 구하기
• 문제: 정수형 배열 arr[5] = {10, 20, 30, 40, 50};을 선언하세요.
arr[i] 대신 *(ptr + i) 형태의 포인터 연산만을 사용하여
배열 요소 전체의 합을 구해 출력하세요.


*/
#include <stdio.h>




int main() {
  int arr[5] = {10,20,30,40,50};

  int *ptr = arr;
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum +=*(ptr+i);
  }

  printf("sum : %d \n", sum);
}
