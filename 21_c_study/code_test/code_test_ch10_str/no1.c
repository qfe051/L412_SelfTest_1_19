/*
1. 포인터로 문자열 뒤집기 (In-place Reverse)
문제: 사용자로부터 문자열을 입력받은 뒤,
별도의 추가 배열을 만들지 말고 **포인터 두 개(시작과 끝)**만을 사용하여
문자열의 내용을 앞뒤로 뒤집어 출력하세요.
• 예시: apple → elppa


*/
#include <stdio.h>
#include <string.h>



int main() {
  char s[20];

  printf("type string : ");
  fgets(s, sizeof(s), stdin);

  printf("s : %s \n",s);

  printf("reverse : ");

  for (int i=1; i <= sizeof(s); i++) {
    printf("%c ",s[sizeof(s)-i]);
  }

  

  

  

  
  // printf("a : %d \n", a);


}
