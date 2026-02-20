#include <stdio.h>

int main(void) {

  // 변수의 주소 확인
  printf("1) \n");

  int a = 10;

  printf("a : %d \n", a);
  printf("&a : %p \n", &a);

  // 변수의 주소를 포인터 변수에 저장
  printf("2) \n");

  int *p = &a;

  printf("p : %p \n", p);
  printf("*p : %d \n", *p);

  // 포인터로 변수 값 변경
  printf("3) \n");
  *p = 20;
  printf("after *p : %d \n", *p);
  printf("a : %d \n", a);

  // array가 저장하는 값 확인 
  printf("4) \n");
  int arr[5] = {10, 20, 30, 40, 50};

  // 타입 모두 다름
  // 각각의 타입은?
  
  printf("arr : %p \n", arr);
  printf("&arr[0] : %p \n", &arr[0]);
  printf("&arr : %p \n", &arr);

  // 포인터 값 연산
  printf("5) \n");

  //여기에서 위의 arr, &arr[0], &arr는 모두 같은 값을 출력
  int *q = arr;

  printf("q: %p \n", q);
  //주소연산
  printf("q+1: %p \n", q + 1);
  // 의미 다르지만 환경 때문에 값이 같음
  // 값 다르도록 수정
  printf("*q+1: %d \n", *q + 1);
  printf("*(q+1): %d \n", *(q + 1));
  printf("q[2]: %d \n", q[2]);

  printf("6) \n");

  // 10씩 차이나는 이유는?
  // arr+1 -> int 1칸 이동
  // &arr+1 -> int arr[5] 1칸 이동
  printf("arr+1 : %p \n", arr+1);
  printf("&arr+1 : %p \n", &arr + 1);

  printf("7) \n");
  // 의미 파악하기 -> 주소값 0으로 들어감
  int *null_ptr = NULL;
  printf("null_ptr : %p \n", null_ptr);
  printf("null_ptr + 1 : %p \n", null_ptr + 1);

  return 0;
}