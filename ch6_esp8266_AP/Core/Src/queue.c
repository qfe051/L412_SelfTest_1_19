#include "queue.h"
#include <stdio.h>


void enqueue_ring(ring *r, uint8_t input_data) {
  // rear 먼저 계산

  if (((r->rear + 1) % r->max_size) == (r->front)) {
    // 포화 상태에서 동작 안함 -> 덮어쓰기 방지
  } else {
    r->rear = (r->rear + 1) % r->max_size;
    r->data[r->rear] = input_data;
  }
}

// return값 형태 개선 필요
uint8_t dequeue_ring(ring *r) {
  if (r->rear == r->front) {
    return 0;
  }

  r->front = (r->front + 1) % r->max_size;

  return r->data[r->front];
}

void init_ring(ring *r, uint8_t buf_data[], uint8_t buf_size) {
  r->rear = 0;
  r->front = 0;
  r->data = buf_data;
  r->max_size = buf_size;
}