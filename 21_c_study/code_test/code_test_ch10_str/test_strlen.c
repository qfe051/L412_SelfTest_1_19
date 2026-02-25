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
  char s[20] = {0};
  char s2[20] = {0};
  
  printf("type string : ");
  fgets(s, sizeof(s), stdin);

  int len = strlen(s);
  printf("len : %d \n",len);

  // 0부터 시작이라서 'len-1' 해주어야 함 
  s[len-1] = '\0';

  int i = 0;
  for (i = 0; i < len; i++) {
    s2[i] = s[i];
  }



  printf("s : %s \n", s);
  printf("s2 : %s \n",s2);



}
