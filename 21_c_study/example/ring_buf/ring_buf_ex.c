#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE_1 5
#define BUFFER_SIZE_2 8

// 구조체 정의 -> 일단 4개로 구성 : 추가 기능 필요 시 늘리기
typedef struct ring {
  uint8_t *buffer;
  uint8_t rear;
  uint8_t front;
  uint8_t size;
} ring_st;


// 함수 정의
static void init_ring(ring_st *r,uint8_t *pbuffer ,uint8_t buffer_size);
static bool enqueue(ring_st *r,uint8_t rxdata);
static bool is_full(ring_st *r);
static uint8_t dequeue(ring_st *r);
static bool is_empty(ring_st *r);
static bool display(ring_st *r);


// 통신용 버퍼 정의
uint8_t buf_1[BUFFER_SIZE_1];
uint8_t buf_2[BUFFER_SIZE_2];





int main() {
  // 1번 링 구조체 선언
  ring_st ring_1;

  //  case1) 3+4 이어쓰기
  printf("case 1) \n");

  init_ring(&ring_1, buf_1, BUFFER_SIZE_1);

  enqueue(&ring_1, 10);
  enqueue(&ring_1, 20);
  enqueue(&ring_1, 30);

  // dequeue 함수 따로 만들기 
  uint8_t temp;
  temp = dequeue(&ring_1);
  display(&ring_1);

  enqueue(&ring_1, 40);
  enqueue(&ring_1, 50);
  enqueue(&ring_1, 60);
  enqueue(&ring_1, 70);

  display(&ring_1);

  //  case2) overflow -> 7개 한번에
  printf("case 2) \n");
  
  init_ring(&ring_1, buf_1, BUFFER_SIZE_1);

  enqueue(&ring_1, 10);
  enqueue(&ring_1, 20);
  enqueue(&ring_1, 30);
  enqueue(&ring_1, 40);
  enqueue(&ring_1, 50);
  enqueue(&ring_1, 60);
  enqueue(&ring_1, 70);

  display(&ring_1);
}

//buffer_size에 #define 된 값 넣기 
void init_ring(ring_st *r,uint8_t *pbuffer ,uint8_t buffer_size) {
  r->buffer = pbuffer;
  r->rear = 0;
  r->front = 0;
  r->size = buffer_size;
}


// 함수 선언 이 아래에 해주기
bool enqueue(ring_st *r, uint8_t rxdata) {
  // is_full 함수 여기 안에 넣기
  if (is_full(r)) {
    // full 일 때 front 덮어쓰기 (밀어내기 후 저장) 옵션
    r->front = (r->front +1)%(r->size);
  }
  r->buffer[r->rear] = rxdata;
  r->rear = (r->rear +1)%(r->size);
}

//rear이 front 따라잡기 직전 상황
bool is_full(ring_st *r) {
  if (r->front == (r->rear + 1) % (r->size)) {
    printf("ring buffer is full \n");
    return 1;
  }
  else {
    return 0;
  }
}

// front가 rear 따라 잡아 동일한 상황
bool is_empty(ring_st *r) {
    if (r->front == r->rear) {
    return 1;
  }
  else {
    return 0;
  }
}

uint8_t dequeue(ring_st *r) {
  if (is_empty(r)) {
    return false;
  }
  uint8_t output_data = r->buffer[r->front];
  r->front = (r->front +1)% (r->size);

  return output_data;
}

bool display(ring_st *r) {

  if (is_empty(r)) {
    printf("ring buffer is empty \n");
    return false;
  }
  
  printf("[Before]buffer state : size %d | front: %d |rear %d \n", r->size, r->front,
         r->rear);

  printf("buf data :");
  while (!is_empty(r)) {
    printf("[%d]%d ",(r->front+4)%5 ,dequeue(r));
  }

  printf("\n"); 
  printf("[After]buffer state : size %d | front: %d |rear %d \n", r->size, r->front,
         r->rear);

  printf("\n --------------------------------------------------- \n");
}