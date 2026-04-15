#include "main.h"
#include "queue.h"
#include <stdbool.h>
#include <stdint.h>

#ifndef ESP8266_h_
#define ESP8266_h_

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef hlpuart1;

// 구조체 이름 변경해서 -> recv_buf, recv_cnt 다 넣어보기
typedef struct esp_8266_control {
  uint8_t server_status;
  uint8_t recv_cnt;
  uint8_t *recv_buf;
  uint8_t err_cnt;
  char *str_CIPSEND;
} esp_8266;

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
  ESP8266_CONNECTED_CLIENT,

  ESP8266_DEVICE_WAIT,
  ESP8266_D_CIPSEND_SEND,
  ESP8266_D_CIPSEND_WAIT,
  ESP8266_HTML_SEND,
  ESP8266_HTML_WAIT,
  ESP8266_BUTTON_WAIT
};

void test_uart(void);
void restore_esp8266(void);
void enable_esp8266_echo(void);
void disable_esp8266_echo(void);

void init_esp_8266(esp_8266 *s, uint8_t *p_output_buf);

// ring으로 빼기 X
bool process_ring_html(ring *r, esp_8266 *s);

// main 사용 함수
void sequence_html(ring *r, esp_8266 *s);
void check_connection(ring *r, esp_8266 *s);

#endif
