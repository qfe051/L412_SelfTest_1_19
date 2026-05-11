// boot_app.c 실행 가능하도록 주소 옮겨주기
#include "boot_jump.h"

/*
App가 정상적으로 빌드된 경우:
첫 4바이트 = stack pointer
→ 항상 SRAM 영역 주소
*/

void jump_to_app(void) {

  // 함수 진행 전 test
  printf("SP: 0x%08X \r\n", *(uint32_t*)APP_ADDR);
  printf("Reset: 0x%08X \r\n", *(uint32_t*)(APP_ADDR + 4));

  // 주소의 4바이트 값 읽음
  uint32_t app_sp = *(uint32_t *)APP_ADDR;
  // +4 하면 리셋 기능 있다는 메모리맵 있는지?
  // +4 한 위치에서 값 읽기 : reset handler 시작 위치
  uint32_t app_reset = *(uint32_t*)(APP_ADDR + 4);

  // 그냥 함수를 주소로 넣고 쓰가 위함
  typedef void (*app_func_t)(void);
  // app_entry에 app_reset의 주소를 대입
  // Reset_Handler 주소를 “함수 포인터”로 바꿔서 저장
  // app_entry = (함수 포인터)
  // app_entry() 실행 : Application의 Reset_Handler 실행
  app_func_t app_entry = (app_func_t)app_reset;

  // app 점프 순간에 인터럽트 X
  __disable_irq();

  // SysTick의 타이머를 멈춤
  SysTick->CTRL = 0;

  // 인터럽트 벡터 테이블을 APP_ADDR로 변경
  // APP_ADDR 기준으로 정해진 기능들이 주소로 나열되어 있음
  SCB->VTOR = APP_ADDR;
  // 스택을 app 기준으로 변경
  // - 이제 함수 호출, 지역변수, return address, interrupt stacking은 App이
  // 사용할 스택 영역 기준으로 해라.
  __set_MSP(app_sp);

  // app reset handler 실행
  app_entry();
}
