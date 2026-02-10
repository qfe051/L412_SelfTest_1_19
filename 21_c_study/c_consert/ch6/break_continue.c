#include <stdio.h>

int main(void) {

  // 최소 구문 1번은 실행 함
  int data[] = {3, 0, 5, 0, 7, -1, 9};
  int i = 0;

  while (1) {
    int value = data[i++];

    if (value == -1) {
      printf("종료합니다. 입력값 : %d \r\n", value);
      break;
    }
    if (value == 0) {
      continue;
    }
    printf("value : %d \r\n", value);
  }

  printf("비밀번호 입력 성공 \r\n");

  return 0;
}