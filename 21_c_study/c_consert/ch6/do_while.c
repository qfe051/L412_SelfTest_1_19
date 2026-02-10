#include <stdio.h>

int main(void) {

  // 최소 구문 1번은 실행 함 
  int input = 1234;

  do {
    printf("비밀번호 입력: ");
    scanf("%d", &input);
    printf("입력값 : %d \r\n", input);
  } while (input != 1234);

  printf("비밀번호 입력 성공 \r\n");

  return 0;
}