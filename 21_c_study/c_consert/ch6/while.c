#include <stdio.h>

int main(void) {

  int input = 0;

  while (input != 1234) {
    printf("비밀번호 입력: ");
    scanf("%d", &input);
    printf("입력값 : %d \r\n", input);
  }

  printf("비밀번호 입력 성공 \r\n");

  return 0;
}