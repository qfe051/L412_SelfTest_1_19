#include "main.h"
#include "queue.h"
#include <stdbool.h>
#include <stdint.h>

#ifndef ESP8266_h_
#define ESP8266_h_

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef hlpuart1;

typedef struct queue {
//   uint8_t rear;
//   uint8_t *count;
  uint8_t *data;
  uint8_t max;

} queue;

typedef struct status {
  uint8_t server_status;
  uint8_t d_connect_status;
} status;

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

enum set_up_state {
  ESP8266_READY_SEND = 0,
  ESP8266_READY_WAIT,
  ESP8266_CWMODE_SEND,
  ESP8266_CWMODE_WAIT,
  ESP8266_CIPMUX_SEND,
  ESP8266_CIPMUX_WAIT,
  ESP8266_CIPSERVER_SEND,
  ESP8266_CIPSERVER_WAIT,
  ESP8266_CONNECTED_CLIENT
};

// 별도 함수에서 사용 예정이므로 0에서 시작
enum connect_device_state {
  ESP8266_DEVICE_WAIT = 0,
  ESP8266_D_CIPSEND_SEND,
  ESP8266_D_CIPSEND_WAIT,
  ESP8266_HTML_SEND,
  ESP8266_HTML_WAIT,
  ESP8266_BUTTON_WAIT

};

void test_uart(void);
void init_esp8266(void);

// ring으로 빼기 X
bool process_ring_html(ring *r, uint8_t buf_recv_data[], uint8_t *count);

// main 사용 함수
void ready_sequence_html(ring *r, uint8_t buf_recv_data[], status *s,
                         uint8_t *count);
void connect_html(ring *r, uint8_t buf_recv_data[], status *s, uint8_t *count);

#endif
