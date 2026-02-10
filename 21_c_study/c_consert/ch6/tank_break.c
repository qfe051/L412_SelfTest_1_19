#include <stdio.h>

int main(void) {

  // 처음엔 int로 선언
  int tank = 200;

  printf("탱크의 현재 수준 : %d \r\n", tank);

  while (1) {
    float use_tank = 0.0f;

    printf("사용할 탱크의 수준 + - 사용하기  : ");
    scanf("%f", &use_tank);

    // int형 tank를 float으로 실수로 형변환해서 계산 결과를 출력 (소수점 유지)
    printf("계산된 최종 탱크 수준 (실수) : %.2f \r\n", (float)tank + use_tank);

    // 실제 tank 변수는 int이므로 저장할 때는 소수점이 버려짐
    tank += (int)use_tank;

    printf("저장된 탱크 수준 (정수) : %d \r\n", tank);

    if (tank < 10) {
      printf("(경고) 탱크의 수준이 10미만입니다. \r\n");
      break;
    }
  }

  return 0;
}