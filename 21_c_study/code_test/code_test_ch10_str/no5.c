/*
5. 공백 제거 및 단어 첫 글자 대문자화
문제: 소문자로 된 여러 단어의 문장을 입력받아,
단어 사이의 공백을 제거하면서 각 단어의
첫 글자만 대문자로 바꾸어 붙여서 출력하세요.

• 예시: i love coding → ILoveCoding


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

  int len_enter = strcspn(s, "\n");

  s[len_enter] = '\0';

  int len =strlen(s);

  if (s[0] != ' ') {
   s[0] -= 32;
  }

  for (int i =0; i<len; i++) {
    if (s[i]==' ') {
      s[i + 1] -= 32;
      for (int j=i; j<len; j++) {
        s[j]= s[j+1];
      }
    }
  }


  printf("s : %s \n",s);



}
