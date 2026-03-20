#include "esp8266.h"
#include <stdint.h>

// // uart 입출력 잘되는지 확인
// void test_uart(void) {
//   char AT_Data[] = "AT\r\n";
//   char AT_output_Data[20];

//   printf("size AT_Data : %d  \r\n", sizeof(AT_Data));
//   HAL_UART_Transmit(&huart1,AT_Data,sizeof(AT_Data),50);
//   HAL_UART_Receive(&huart1, AT_output_Data, sizeof(AT_output_Data), 50);

//   printf("AT_output_Data : %s \r\n ", AT_output_Data);
//   printf("AT_Data : %s \r\n ",AT_Data);
// }


// flag 사용 어떻게 할지 생각해보기

void test_uart(void) {
  //최종 목표
  // char AT_Data[] = "AT+CIPSTART=\"TCP\",\"www.naver.com\",80 \r\n";

  //테스트 진행
  // char AT_Data[] = "AT+CWLAP=\"book3\"\r\n";

  //성공예제
  char AT_Data[] = "AT+CIFSR\r\n";

  // HAL_UART_Transmit(&hlpuart1, AT_Data, sizeof(AT_Data), 50);
  // HAL_Delay(800);
  // printf("size AT_Data : %d  \r\n", sizeof(AT_Data));
  HAL_UART_Transmit(&huart1, (uint8_t *)AT_Data, sizeof(AT_Data)-1, 50);
  // HAL_UART_Transmit_IT(&huart1,AT_Data, sizeof(AT_Data));
  
  
}

void init_esp8266(void) {

  //echo 없음
  char AT_Data[] = "ATE1\r\n";
  HAL_UART_Transmit(&huart1, AT_Data, sizeof(AT_Data)-1, 50);
}

void is_ready_esp8266(void) {}

// a - 데이터 입력하기

// 배열, 포인터 모두 각각의 주소 입력하기 
void input_data(queue *q, uint8_t *index_in,uint8_t *index_out, uint8_t buff[], uint8_t len_buff) {

  uint8_t len_data = (q->max+1);

  printf(">>len_data : %d \r\n", q->max+1);

  for (int i = 0; i < len_data; i++) {
    printf(">>q->data[%d] : 0x%02X \r\n",i, q->data[i]);
    buff[(*index_in + i) % len_buff] = q->data[i];

    // printf(">>(*index_in + i)  len_buff : %d \r\n", (*index_in + i) % len_buff);
  }

  
  //기존 *index_in 부터 출력을 시작 함
  *index_out = *index_in;
  // 최종 input +1 위치에 새로운 input 넣어두기
  *index_in = (*index_in + len_data) % len_buff;

  // 다음 시작하는 인덱스에 NULL 채워두기! -> 문자열로 한번 출력하도 또 안씀

  // print buf 따로 있어서 필요 없을듯?
  // buff[*index_in] = '\0';
}

// *index_in 필요한지?
void print_data(uint8_t *index_in, uint8_t *index_out, 
                uint8_t buff[]) {
  // 출력용 버퍼 만들기

  uint8_t len_print = (100 + *index_in - *index_out) % 100;
  uint8_t buff_p[len_print+1];

  for (int i = 0; i < len_print; i++) {
    buff_p[i] = buff[*index_out+i];
  }
  buff_p[len_print] = '\0';

  HAL_UART_Transmit(&hlpuart1, buff_p, strlen(buff_p), 100);

  *index_out = *index_in;

}

void init_data(uint8_t *index_in, uint8_t *index_out, uint8_t buff[]) {
  printf("index_in = %d \r\n", index_in);
  printf("*index_in = %d \r\n",*index_in);
  // *index_in = 0;
  // *index_out = 0;
  memset(buff,0,sizeof(buff));
}

void initQueue(queue *q, int size) {
  q->max = size;
  q->data = malloc(size*sizeof(uint8_t));
}

void get_rxdata(queue *q,uint8_t rxdata,uint8_t count) {
  // 굳이 malloc 안써도 될 것 같으니 확인해보기 
  q->max = count;
  uint8_t *buff = malloc(q->max * sizeof(uint8_t));
  

  for (int i = 0; i < q->max; i++) {
    buff[i]=q->data[i];
    printf("q->data [%d] : 0x%02X \r\n",i,q->data[i]);
    printf("buff [%d] : 0x%02X \r\n",i, buff[i]);
  }
  // 1개 추가한다고 생각하기
  q->data = realloc(q->data, (q->max+1) * sizeof(uint8_t));
  if (q->data ==NULL) {
    printf("q->data realloc error \r\n");
    return 0;
  }
  // q->data[i]에 buff[i] 넣는 작업 추가
  for (int i = 0; i < q->max; i++) {
    q->data[i]=buff[i];
  }
  printf("q->max+1 : %d \r\n",q->max+1);

  q->data[q->max] = rxdata;
  printf("q->data[q->max] : 0x%02X \r\n",q->data[q->max]);
  printf("rxdata : 0x%02X \r\n",rxdata);

  free(buff);
}

// r->data = buf; 위치에 buf -> 주소형태로 써야함
// 초기화 해주는 시점은? 시작부 or 지속적으로
void init_ring(ring *r) {
  uint8_t buf[100] = {0,};
  uint8_t queue_buf[100] = {0,};
  r->rear = 0;
  r->front = 0;
  r->index = 0;
  r->data = buf;
  r->q_data = queue_buf;
  r->max_size = 100;
  r->recv_data = 0;
}

void enqueue_ring(ring *r,uint8_t rxdata) {
  r->data[r->rear] = rxdata;
  printf("r->rear : 0x%02X \r\n",r->rear);
  printf("r->data[r->rear] : 0x%02X \r\n",r->data[r->rear]);
  r->recv_data = r->data[r->rear];
  r->recv_data = rxdata;

  // max_size 단위로 순환 -> 약 100개 
  r->rear = ((r->rear)+1) % r->max_size;

}

void dequeue_ring(ring *r ) {
  // rear , front 다른 경우 출력

  // 출력용 버퍼 대입 
  r->q_data[r->index] = r->data[r->front];

  // data -> q_data로 뽑아서 사용 
  if (r->q_data[r->index]=='\n' || r->q_data[r->index] ==98 ) {
    r->q_data[r->index + 1] = '\0';
    HAL_UART_Transmit(&hlpuart1, r->q_data, strlen(r->q_data), 100);


    r->index =0;
  }
  else{
	r->index++;
  }
  
  if ((100 + r->rear - (r->front)) % 100) {
    // /n에서 

    r->front=((r->front)+1) % r->max_size;
  }
  else {
    // 동일하면 +1만 안하는걸로!  
    // r->front=((r->front)+1) % r->max_size;
  }
}
