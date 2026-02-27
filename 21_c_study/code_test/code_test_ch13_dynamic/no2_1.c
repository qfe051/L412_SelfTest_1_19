/*
2. 문자열 결합 함수 (딱 맞는 크기 할당)
문제: 두 문자열을 받아 합쳐진 새로운 문자열을
딱 맞는 크기로 할당해 반환하는
char* concat(char *s1, char *s2) 함수를 작성하세요.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* concat(char *s1, char *s2) {
  // 1. strlen으로 크기 계산 -> malloc -> 합치기 -> 주소 반환
  int len = 0;
  len = (strlen(s1)+1) + (strlen(s2)+1);

  printf("len : %d \n",len);

  // char *s;
  // s = malloc(len);
  // *s = 0;

  // printf("sizeof(s) : %d \n", sizeof(s));

  s1 = malloc(len);
  *s1 = 0;
  strcat(s1,s2);
  printf("s1 : %s \n", s1);

  return s;
}
int main() {
    char *res = concat("Hello", "World");
    // [디버깅 틀] 결과 및 길이 확인
    if (res) {
      printf("result: %s, length: %zu", res, strlen(res));
      free(res);
    }
    
    return 0;
}
