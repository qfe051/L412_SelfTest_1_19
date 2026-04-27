#include "printf.h"

// &hlpuart1 버전
// weak로 선언된 _write 함수 재정의
int _write(int file, char *ptr, int len) {

  //(void)file;
  HAL_UART_Transmit(&hlpuart1, (uint8_t *)ptr, len, 100);
  return len;
  
}

