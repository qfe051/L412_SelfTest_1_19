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
static bool enqueue_overwrite(ring_st *r, uint8_t rxdata);
static bool is_full(ring_st *r);
static bool dequeue(ring_st *r, uint8_t *output_data);
static bool is_empty(ring_st *r);
static bool get_ring_data(ring_st *r,uint8_t *pbuffer,uint8_t *output_data);
static void ring_status(ring_st *r);
static bool print_temp_data(uint8_t *pbuffer);


// 통신용 버퍼 정의
uint8_t buf_1[BUFFER_SIZE_1];
uint8_t buf_2[BUFFER_SIZE_2];

uint8_t buf_data_1[BUFFER_SIZE_1];
uint8_t dequeue_data = 0;





int main() {
  // 1번 링 구조체 선언
  ring_st ring_1;

  //  case1) 3+4 이어쓰기
  printf("case 1) \n");

  init_ring(&ring_1, buf_1, BUFFER_SIZE_1);

  enqueue_overwrite(&ring_1, 10);
  enqueue_overwrite(&ring_1, 20);
  enqueue_overwrite(&ring_1, 30);

  //dequeue 이전
  printf("[Before] dequeue \n");
  ring_status(&ring_1);
  get_ring_data(&ring_1, buf_data_1,&dequeue_data);
  print_temp_data(buf_data_1);


  enqueue_overwrite(&ring_1, 40);
  enqueue_overwrite(&ring_1, 50);
  enqueue_overwrite(&ring_1, 60);
  enqueue_overwrite(&ring_1, 70);

  //dequeue 이전
  printf("[Before] dequeue \n");
  ring_status(&ring_1);
  get_ring_data(&ring_1, buf_data_1,&dequeue_data);
  print_temp_data(buf_data_1);    


  //  case2) overflow -> 7개 한번에
  printf("case 2) \n");

  init_ring(&ring_1, buf_1, BUFFER_SIZE_1);

  enqueue_overwrite(&ring_1, 10);
  enqueue_overwrite(&ring_1, 20);
  enqueue_overwrite(&ring_1, 30);
  enqueue_overwrite(&ring_1, 40);
  enqueue_overwrite(&ring_1, 50);
  enqueue_overwrite(&ring_1, 60);
  enqueue_overwrite(&ring_1, 70);
  
  //dequeue 이전
  printf("[Before] dequeue \n");
  ring_status(&ring_1);
  get_ring_data(&ring_1, buf_data_1,&dequeue_data);
  print_temp_data(buf_data_1);


}

//buffer_size에 #define 된 값 넣기 
void init_ring(ring_st *r,uint8_t *pbuffer ,uint8_t buffer_size) {
  r->buffer = pbuffer;
  r->rear = 0;
  r->front = 0;
  r->size = buffer_size;
}


// 함수 선언 이 아래에 해주기
bool enqueue_overwrite(ring_st *r, uint8_t rxdata) {
  // is_full 함수 여기 안에 넣기
  if (is_full(r)) {
    // full 일 때 front 덮어쓰기 (밀어내기 후 저장) 옵션
    r->front = (r->front +1)%(r->size);
  }
  r->buffer[r->rear] = rxdata;
  r->rear = (r->rear +1)%(r->size);
}

bool enqueue(ring_st *r, uint8_t rxdata) {
  // is_full 함수 여기 안에 넣기
  if (is_full(r)) {
    // full 일 때 front 덮어쓰기 (밀어내기 후 저장) 옵션
    return false;
  }
  r->buffer[r->rear] = rxdata;
  r->rear = (r->rear +1)%(r->size);
}

//rear이 front 따라잡기 직전 상황
bool is_full(ring_st *r) {
  if (r->front == (r->rear + 1) % (r->size)) {
    printf("ring buffer is full \n");
    return true;
  }
  else {
    return false;
  }
}

// front가 rear 따라 잡아 동일한 상황
bool is_empty(ring_st *r) {
    if (r->front == r->rear) {
    return true;
  }
  else {
    return false;
  }
}

// 교재에서는 0으로 반환, 하지만 예외처리 필요
bool dequeue(ring_st *r, uint8_t *output_data) {
  if (is_empty(r)) {
    return false;
  }
  *output_data = r->buffer[r->front];
  r->front = (r->front +1)% (r->size);

  return true;
}

// 데이터 쌓기용으로 만들기
bool get_ring_data(ring_st *r,uint8_t *pbuffer,uint8_t *output_data) {

    for (int i=0; i<BUFFER_SIZE_1; i++) {
        pbuffer[i]=0;
    }

  if (is_empty(r)) {
    printf("ring buffer is empty \n");
    return false;
  }

  // while문 안에 for문. 써도 괜찮은 스타일인지 확인하기 
  while (!is_empty(r)) {
    for (int i=0; i<BUFFER_SIZE_1; i++) {
        if (dequeue(r,output_data))
        {
          pbuffer[i]=*output_data;
        }
    }
  }

  printf("\n"); 
  printf("[After]buffer state : size %d | front: %d |rear %d \n", r->size, r->front,
         r->rear);

  printf("\n --------------------------------------------------- \n");

  return true;
}

void ring_status(ring_st *r) {
  printf("[]buffer state : size %d | front: %d |rear %d \n", r->size,
         r->front, r->rear);
}

bool print_temp_data(uint8_t *pbuffer) {
    if (pbuffer[0]==0) {
      printf("No temp buff data \n");

      return false;
    }
  
  printf("temp buff data : ");

  for (int i = 0; i < BUFFER_SIZE_1; i++) {
    if (pbuffer[i] == 0) {
      printf("\n");
      printf("\n --------------------------------------------------- \n");
        return true;
    }
    printf("%d ",pbuffer[i]);
  }
  // return true;
}