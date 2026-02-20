#include <stdio.h>

int main(void) {

  int *ptr1;
  int **ptr2;
  int num = 100;

  ptr1 = &num;

  // 1)
  *ptr2 = ptr1;
  // 2)
  // ptr2라는 방에 ptr1의 방의 주소(&ptr1)를 대입
  ptr2=&ptr1;

  printf("*ptr1 : %d \n", *ptr1);
  printf("**ptr2 : %d \n",**ptr2);
  

  return 0;
}