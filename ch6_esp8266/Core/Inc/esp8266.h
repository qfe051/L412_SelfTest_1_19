#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef ESP8266_h_
#define ESP8266_h_

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef hlpuart1;

void test_uart(void);
void init_esp8266(void);

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
  // uint8_t recv_data;
  uint8_t *time_data;

} ring;

typedef struct state {
  bool ready_state;
  bool wifi_state;
  bool parsing_state;
  bool tcp_state;
} state;

typedef struct time {
  uint8_t day;
  uint8_t date;
  uint8_t month;
  uint8_t year;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
} time;



void print_data(uint8_t *index_in, uint8_t *index_out, 
                uint8_t buff[]);
void init_data(uint8_t *index_in, uint8_t *index_out, uint8_t buff[]);
void initQueue(queue *q, int size);
void get_rxdata(queue *q, uint8_t rxdata, uint8_t count);

uint8_t dequeue_ring(ring *r);

void is_wifi_enable(ring *r, state *s);
void is_ready(ring *r, state *s);
void is_parsing_enable(ring *r, state *s);

#endif
