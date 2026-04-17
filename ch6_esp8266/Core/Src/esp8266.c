#include "esp8266.h"

#include <stdbool.h>
#include <stdint.h>

#define RECV_BUF_SIZE 100

static uint8_t recv_buf[RECV_BUF_SIZE];

static AT_Response parse_response(uint8_t *raw_data) {}

//static uint8_t recv_buf[RECV_BUF_SIZE]; 활용하기
bool recv_ring_data(uint8_t *p_rxdata, uint8_t recv_size) {
  if (p_rxdata==NULL||recv_size==0) {
    return false;
  }
  for (int i=0; i<recv_size; i++) {
    
  }
}
