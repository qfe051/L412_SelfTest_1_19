/*
2. 모음 제거 및 개수 세기
문제: 영어 문장을 입력받아,
해당 문장에서 모음(a, e, i, o, u - 대소문자 포함)이 총 몇 개인지 출력하고,
모음을 제외한 자음들만 출력하는 프로그램을 작성하세요.

• 예시: Hello World → 모음 개수: 3, 결과: Hll Wrld



*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool compare(char data) {
  bool status = false;
  char a[] = {'a', 'e', 'i', 'o', 'u'};
  for (int i=0; i<5; i++) {
    if (a[i]==data) {
      status = true;
    }
  }
}


int main() {
  char a[] = {'a', 'e', 'i', 'o', 'u'};
  char b[20];
  char s[20];
  int count = 0;

  printf("type string : ");
  fgets(s, sizeof(s), stdin);

  printf("s : %s \n",s);

  for (int i = 0; i < sizeof(s[20]); i++) {
    
    if (compare(s[i])) {
      count++;
    }
    else {
      // strcpy(&s[i], &b[i]);
      b[i]=s[i];
    }
  }

  printf("b : %s \n", b);
  printf("count : %d \n",count);

  



}
