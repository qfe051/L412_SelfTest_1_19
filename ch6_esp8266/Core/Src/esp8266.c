#include "esp8266.h"

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
  char AT_Data[] = "AT+CIFSR\r\n";
  char AT_output_Data[20];

  // printf("size AT_Data : %d  \r\n", sizeof(AT_Data));
  HAL_UART_Transmit(&huart1, AT_Data, sizeof(AT_Data), 50);
  HAL_UART_Transmit(&hlpuart1, AT_Data, sizeof(AT_Data), 50);
  
}

void get_buf(uint8_t *buf_recv , uint8_t *buf_data , uint8_t *count, uint8_t rxData) {
  *buf_recv[*count] = rxData;
//   count++;
  if (rxData == '\n') {
    // for (int i =0; i<(*count)+1; i++) {
    //     buf_data[i] = buf_recv[i];
    // }
//    memcpy(buf_data,buf_recv,*count);
     HAL_Delay(10);
      *count =0;
      
    }


}
