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

// a - 데이터 입력하기

// 배열, 포인터 모두 각각의 주소 입력하기 
void input_data(uint8_t *index_in,uint8_t *index_out, uint8_t data[] ,uint8_t buff[],uint8_t len_data, uint8_t len_buff) {

  printf(">>*index_in : %d \r\n", *index_in);
  printf(">>*index_in + 1 : %d \r\n", *index_in+1);
  printf(">>index_in : %d \r\n", index_in);
  printf(">>&index_in : %d \r\n", &index_in);
  printf(">>len_buff : %d \r\n", len_buff);
  

  for (int i = 0; i < len_data; i++) {
    buff[(*index_in + i) % len_buff] = data[i];

    printf(">>(*index_in + i)  len_buff : %d \r\n", (*index_in + i) % len_buff);
  }

  printf(">>size_data : %d \r\n", len_data);
  printf(">>size_buff : %d \r\n",len_buff);

  *index_out = *index_in ;
}