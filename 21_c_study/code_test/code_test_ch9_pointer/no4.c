/*
4. 이중 포인터를 이용한 값 변경
• 문제: 정수 num = 10과 이를 가리키는 포인터 p,
그리고 p를 가리키는 이중 포인터 pp를 선언하세요.
오직 **pp만을 이용(역참조 2번)**하여
num의 값을 500으로 바꾸고 출력하세요.


*/
#include <stdio.h>



int main() {
  int num = 10;
  int *p;
  int *pp ;


  // 주소 동일하게
  p = pp;
  // 동일한 주소에 값 대입
  *pp = 500;
  num = *p;

  printf("num : %d",num);


}
