/*
5. 함수 포인터를 이용한 사칙연산 선택
• 문제: int add(int a, int b)와 int sub(int a, int b) 함수를 각각 만드세요.
그 후 함수 포인터를 하나 선언하여,
사용자가 1을 누르면 add를,
2를 누르면 sub를 가리키게 한 뒤
포인터를 통해 계산 결과를 출력하세요.


*/
#include <stdio.h>

void add(int a, int b) {
  int c = 0;

  c = a + b;

  printf("result : %d \n",c);

}
void sub(int a, int b) {
  int c = 0;

  c = a - b;

  printf("result : %d \n",c);
}

int *calculate();


int main() {
  int input = 0;
  int a = 10;
  int b = 5;
  
  printf("a: %d , b : %d \n" , a,b);
  printf("type 1(a+b) or 2(a-b) :");
  scanf("%d",&input);

  if (input == 1) {
    calculate() = &add(int a,int b);

  }
  else if (input == 2) {
    
  }
  else {
    printf("Wrong Number \n");
  }


 


}
