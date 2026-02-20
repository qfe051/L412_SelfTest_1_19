/*
3. 알파벳 시프트 (암호화 기초)
문제: 영문 문자열과 정수 n을 입력받으세요.
각 알파벳을 알파벳 순서상 뒤로 n칸씩 밀어서 만든 문자열을 출력하세요.
(단, 'z'에서 1칸 밀면 'a'가 되어야 합니다.)




*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main() {
  char a = 'a';
  char s[20];
  char b[20];
  int n = 0 ;

  printf("type string : ");
  fgets(s, sizeof(s), stdin);

  printf("type int 'n' : ");
  scanf("%d",&n);



  printf("s : %s \n", s);

  for (int i = 0; i < 20; i++) {
    s[i] += n;
    if (s[i]>122) {
      s[i] -=26;
    }
  }
  printf("s + n : %s \n", s);

}
