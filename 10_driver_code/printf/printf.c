#include "printf.h"

// &hlpuart1 버전
// weak로 선언된 _write 함수 재정의
// [목적] STM32 MCU에서,printf 사용하기 위함
int _write(int file, char *ptr, int len) {

  HAL_UART_Transmit(&hlpuart1, (uint8_t *)ptr, len, 100);
  return len;
  
}

