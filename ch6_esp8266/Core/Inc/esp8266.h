
#ifndef ESP8266_H
#define ESP8266_H

#include "main.h"
#include "queue.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// esp8266에서 사용할 ring 구조체, 변수
extern uint8_t rx_data_esp;
extern User_ring rx_ring_esp;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef hlpuart1;

// parsing 저장할 구조체
typedef enum {
  RESP_UNKNOWN = 0, // RESP 받기 전 초기화 or 모르는 데이터도?
  RESP_OK,          // OK가 나오는 응답
  RESP_ERROR,       // 에러 상황
  RESP_READY,       // reset 되는 경우
  RESP_ETC          // OK 이외에 다른 방식으로 나오는 데이터
} Response_type;

// type에 따라서 param을 count 함
typedef struct {
  Response_type type;
  char params[5][100];
  int param_count;
} AT_Response;

// 지정된 사이즈로 data 수
bool recv_ring_data(uint8_t *p_rxdata, uint8_t recv_size); // p_rxdata
bool recv_data_task(void); // p_rxdata를 -> recv_buffer에 저장, p_rxdata를
                           // 받아줄 변수 1개 필요

// 전체 프로세스 진행하며 단계별로 판단하기 -> 응답은 구조체 활용!
// last_resp가 실시간으로 업데이트 되니, void로 받아오기
bool esp_8266_control(void);

bool init_esp8266(void);

#endif