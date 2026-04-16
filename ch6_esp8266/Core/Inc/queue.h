#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h> //NULL 사용하기 위함


//over_count 살려야 하는지? 
typedef struct _user_ring {
  uint8_t *p_buffer;
  uint8_t rear, front;
  uint8_t size;
} User_ring;

// over_cnt 목적에 맞게 대입 해보기 -> 포화상태에서 카운팅!

bool is_full(User_ring *r);
bool is_empty(User_ring *r);

// 다른 함수에서 사용할 것.
bool enqueue(User_ring *r, uint8_t *input);
// output에 값 넣는 과정
bool dequeue(User_ring *r,uint8_t *output);

// 인자로 받는 값들 항상 에러처리하기 
bool ring_init(User_ring *r,uint8_t init_size,uint8_t *init_buf);

uint8_t get_ring_datasize(User_ring *r);


#endif 