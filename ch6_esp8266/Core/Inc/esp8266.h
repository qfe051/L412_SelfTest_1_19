#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef ESP8266_h_
#define ESP8266_h_

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef hlpuart1;


typedef struct queue {
//   uint8_t rear;
//   uint8_t count;
  uint8_t *data;
  uint8_t max;

} queue;

typedef struct ring_buf {
  uint8_t rear;
  uint8_t front;
  uint8_t index;
  uint8_t *data;
  uint8_t *recv_data;
  uint8_t recv_cnt;
  uint8_t max_size;
  uint8_t *time_data;
  uint8_t bootstep;
  uint8_t error_cnt;
  uint8_t error_cnt_max;

} ring;

typedef struct state {
  bool ready_state;
  bool wifi_state;
  bool parsing_state;
  bool tcp_state;
} state;

// uint8_t -> uint32_t로 저장 주소값 저장 목적
typedef struct time {
  char day[5];
  uint8_t date;
  char month[5];
  uint32_t year;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
} time;

// 모든 단계를 send, wait로 나눠보기
// enum ready_state_old {
//   ESP8266_READY = 0,
//   ESP8266_GET_WIFI,
//   ESP8266_PARSE,
//   ESP8266_WAIT_TCP,
//   ESP8266_TCP,
//   ESP8266_GET_TIME,
//   ESP8266_ERROR
// };

enum ready_state {
  ESP8266_READY_SEND = 0,
  ESP8266_READY_WAIT,
  ESP8266_WIFI_SEND,
  ESP8266_WIFI_WAIT,
  ESP8266_CIPSTART_SEND,
  ESP8266_CIPSTART_WAIT,
  ESP8266_CIPSEND_SEND,
  ESP8266_CIPSEND_WAIT,
  ESP8266_TCP_NAVER,
  ESP8266_GET_TIME,
  ESP8266_ERROR
};



void print_data(uint8_t *index_in, uint8_t *index_out, 
                uint8_t buff[]);
void init_data(uint8_t *index_in, uint8_t *index_out, uint8_t buff[]);
void initQueue(queue *q, int size);
void get_rxdata(queue *q, uint8_t rxdata, uint8_t count);

uint8_t dequeue_ring(ring *r);
void enqueue_ring(ring *r, uint8_t input_data);

void is_wifi_enable(ring *r, state *s);
void is_ready(ring *r, state *s);
void is_parsing_enable(ring *r, state *s);

void test_uart(void);
void init_esp8266(void);
void process_ring(ring *r, time *t);



#endif
