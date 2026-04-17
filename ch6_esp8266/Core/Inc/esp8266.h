
#ifndef ESP8266_H
#define ESP8266_H
#include "queue.h"
#include <stdint.h>

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
bool recv_data_task(); // p_rxdata를 -> recv_buffer에 저장, p_rxdata를 받아줄
                       // 변수 1개 필요

// recv_buffer 내부 분석 필요, response 받을 구조체 만들기

bool init_esp8266();

#endif