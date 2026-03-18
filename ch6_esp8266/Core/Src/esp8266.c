#include "esp8266.h"
#include <stdint.h>

// // uart 입출력 잘되는지 확인
// void test_uart(void) {
//   char AT_Data[] = "AT\r\n";
//   char AT_output_Data[20];

//   printf("size AT_Data : %d  \r\n", sizeof(AT_Data));
//   HAL_UART_Transmit(&huart1,AT_Data,sizeof(AT_Data),50);
//   HAL_UART_Receive(&huart1, AT_output_Data, sizeof(AT_output_Data), 50);

//   printf("AT_output_Data : %s \r\n ", AT_output_Data);
//   printf("AT_Data : %s \r\n ",AT_Data);
// }


// flag 사용 어떻게 할지 생각해보기

void test_uart(void) {
  //최종 목표
  // char AT_Data[] = "AT+CIPSTART=\"TCP\",\"www.naver.com\",80 \r\n";

  //테스트 진행
  // char AT_Data[] = "AT+CWLAP=\"book3\"\r\n";

  //성공예제
  char AT_Data[] = "AT+CIFSR\r\n";

  // HAL_UART_Transmit(&hlpuart1, AT_Data, sizeof(AT_Data), 50);
  // HAL_Delay(800);
  // printf("size AT_Data : %d  \r\n", sizeof(AT_Data));
  HAL_UART_Transmit(&huart1, (uint8_t *)AT_Data, sizeof(AT_Data)-1, 50);
  // HAL_UART_Transmit_IT(&huart1,AT_Data, sizeof(AT_Data));
  
  
}

void init_esp8266(void) {

  //echo 없음
  char AT_Data[] = "ATE1\r\n";
  HAL_UART_Transmit(&huart1, AT_Data, sizeof(AT_Data)-1, 50);
}

void is_ready_esp8266(void) {}

