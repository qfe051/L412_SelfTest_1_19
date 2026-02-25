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
  for (int j=0; j<5; j++) {
    if (a[j]==data) {
      status = true;
      // printf("true in a[%d] , %c \n", j, data);
      break;
    }
  }
  return status;
}


int main() {
  // char a[] = {'a', 'e', 'i', 'o', 'u'};
  char b[20] ={0};
  char s[20] ={0};
  int count = 0;

  printf("type string : ");
  fgets(s, sizeof(s), stdin);

  // int len_enter = strcspn(s, "\n");

  // s[len_enter] = '\0';

  int len = strlen(s);
  
  printf("lem : %d \n",len);

  printf("s : %s \n",s);

  int i = 0;
  for (i = 0; i < len-1; i++) {
   // compare 함수만 동작하면 됨
    if (compare(s[i])) {
      count++;
    }
    else {
      // strcpy(&s[i], &b[i]);
      b[i-count]=s[i];
    }


  }
  b[i-count] = '\0';



  printf("b : %s\n",b);
  printf("count : %d \n",count);

  



}
