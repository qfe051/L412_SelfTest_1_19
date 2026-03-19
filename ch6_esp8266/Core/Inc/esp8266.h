#include "main.h"
#include <stdint.h>

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
  uint8_t *data;
  uint8_t max_size;
  uint8_t recv_data;

} ring;



void print_data(uint8_t *index_in, uint8_t *index_out, 
                uint8_t buff[]);
void init_data(uint8_t *index_in, uint8_t *index_out, uint8_t buff[]);
void initQueue(queue *q, int size);
void get_rxdata(queue *q,uint8_t rxdata,uint8_t count);

#endif
