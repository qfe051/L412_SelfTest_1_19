/*
4. 문자열 속 숫자만 골라 합계 구하기
문제: 알파벳과 숫자가 섞인 문자열을 입력받으세요.
문자열 전체를 훑으며 숫자인 문자만 찾아내어 그 숫자들의 합을 구하세요.
• 예시: C2026Feb20 → 2+0+2+6+2+0 = 12





*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main() {
  char a[11] = {'0','1','2','3','4','5','6','7','8','9'};
  char s[20];
  char b[20];
  int sum = 0 ;

  printf("type string : ");
  fgets(s, sizeof(s), stdin);


  printf("1 : %d \n", '0');



  printf("s : %s \n", s);

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      if (s[i]==a[j]) {
        sum += s[i];
        sum -= 48;
        printf("s[%d] = %d \n",i,s[i] );
      }
    }
  }
  printf("sum : %d \n", sum);

}
