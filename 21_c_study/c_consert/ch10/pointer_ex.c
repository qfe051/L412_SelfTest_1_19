#include <stdio.h>
#include <string.h>

int main(void) {
  printf("1) \n");

  {
  char s1[20];
  char s2[20];

  // 길이제한 테스트 
  printf("[scanf] %%19s type \n");
  scanf("%19s", s1);

  // 버퍼 삭제
  // scanf 과정에서 이미 '\n'을 받음 
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF) {
  }

  printf("[fgets] type \n");
  fgets(s2, sizeof(s2), stdin);

  printf("result [scanf] : %s \n", s1);
  printf("result [fgets] : %s \n",s2);
  }

  
  

  

  return 0;
}