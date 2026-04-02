#include <stdio.h>
#include <string.h>
#include <stdbool.h>


// 구조체 정의 -> 일단 4개로 구성 : 추가 기능 필요 시 늘리기
typedef struct {
  uint8_t *buffer;
  uint8_t rear;
  uint8_t front;
  uint8_t max;
} ring_st;


// 함수 정의
static bool enqueue(ring_st *r);
static bool is_full(ring_st *r);
static bool dequeue(ring_st *r);


// 통신용 버퍼 정의
uint8_t size_1 = 100;
uint8_t buf_1[size_1];




int main() {
  
}


// 함수 선언 이 아래에 해주기
bool enqueue(ring_st *r) {
  // is_full 함수 여기 안에 넣기 
  if (expression) {
  
  }
}

bool is_full(ring_st *r) {
  if (r->front == r->rear) {
    return 1;
  }
  else {
    return 0;
  }
}

bool dequeue(ring_st *r) {
  
}

