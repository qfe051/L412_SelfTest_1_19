#ifndef PRINTF_H
#define PRINTF_H

#include <stdint.h>
#include "main.h"

extern UART_HandleTypeDef hlpuart1;

// &hlpuart1 버전
// weak로 선언된 _write 함수 재정의
int _write(int file, char *ptr, int len);

#endif