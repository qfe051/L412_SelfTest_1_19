#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifndef QUEUE_h
#define QUEUE_h

typedef struct ring_buf {
  uint8_t rear;
  uint8_t front;
  uint8_t *data;
  uint8_t max_size;
  uint8_t dequeue_data;
  uint8_t over_cnt;

} ring;

void enqueue_ring(ring *r, uint8_t input_data);
bool dequeue_ring(ring *r);
void init_ring(ring *r, uint8_t buf_data[], uint8_t buf_size);

#endif