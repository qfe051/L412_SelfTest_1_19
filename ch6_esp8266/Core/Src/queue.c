#include "queue.h"

// ring buffer 초기화 함수 | 입력 : User_ring ,init_buf(주소). 출력 : T/F
bool ring_init(User_ring *r, uint8_t init_size, uint8_t *init_buf) {
  if (r==NULL || init_buf == NULL || init_size==0) {
    return false;
  }

  r->front = 0;
  r->rear = 0;
  r->p_buffer = init_buf;
  r->size = init_size;

  return true;
}

//ring buffer가 enqueue가 가능한 상태인지 확인| 입력 : User_ring, 출력 : T/F
bool is_full(User_ring *r) {
  // 구조체 존재 여부 확인
  if (r==NULL) {
    return false;
  }
  //full 상태 판단
  if ((r->rear+1)%r->size==r->front) {
    return true;
  }
  else {
    return false;
  }
}


//ring buffer가 dequeue 가능한 상태인지 확인 |  입력 : User_ring, 출력 : T/F
bool is_empty(User_ring *r) {
  // 구조체 존재 여부 확인
  if (r==NULL) {
    return false;
  }
  // empty 상태 판단
  if (r->rear == r->front) {
    return true;
  }
  else {
    return false;
  }
}

// callback 함수에 의해 ring buffer r->p_buffer[r->rear]에 *input 데이터 저장 | 입력 : User_ring, input(주소), 출력 : T/F
bool enqueue(User_ring *r, uint8_t *input) {
  // 구조체 , input 존재 여부 확인
  if (r==NULL || input ==NULL) {
    return false;
  }

  if (is_full(r)) {
    return false;
  } else {
    r->rear = (r->rear + 1) % r->size;
    r->p_buffer[r->rear] = *input;
    
    return true;
  }
}

// *output에 ring buffer 내부 r->p_buffer[r->front]값 저장 | 입력 User_ring, output(주소), 출력: T/F
bool dequeue(User_ring *r, uint8_t *output) {
  if (r==NULL || output ==NULL) {
    return false;
  }

  if (is_empty(r)) {
    return false;
  }
  else {
    *output = r->p_buffer[r->front];
    r->front = (r->front + 1) % r->size;

    return true;
  }
}

// User_ring의 rear, front 값을 통해 현재 저장된 데이터 크기를 반환 | 입ㄺ User_ring , 출력 data_size
uint8_t get_ring_datasize(User_ring *r){}
