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

static void restart_esp8266(ring *r, state *s);
static void get_state(ring *r);



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

////////////////////////////////////////////

// r->data = buf; 위치에 buf -> 주소형태로 써야함
// 초기화 해주는 시점은? 시작부 or 지속적으로
void init_ring(ring *r,uint8_t buf_data[],uint8_t buf_recv_data[],uint8_t buf_time[]) {
  
  r->rear = 0;
  r->front = 0;
  r->index = 0;
  r->data = buf_data;
  r->recv_data = buf_recv_data;
  r->recv_cnt = 0;
  r->max_size = 100;
  r->time_data = buf_time;
  r->bootstep = ESP8266_READY_SEND;
  r->error_cnt = 0;
  r->error_cnt_max = 5000;
}

// void enqueue_ring(ring *r, uint8_t rxdata) {

//   // 이 코드만 개선 해보기 
//   r->data[r->rear] = rxdata;
  
//   // max_size 단위로 순환 -> 약 100개 
//   r->rear = ((r->rear)+1) % (r->max_size);

// }


// // 'r->data'에서 +index 하는 형식으로 주소 입력해도 되는지
// // 기존 동작 확인 코드 
// void dequeue_ring(ring *r) {
//   // rear , front 다른 경우 출력

//   while ((r->rear) != (r->front)) {
//     r->recv_data[r->index] = r->data[r->front];
//     r->front=((r->front)+1) % (r->max_size);

//     // 출력용 버퍼 대입 
//     // data -> recv_data로 뽑아서 사용 
//     if (r->recv_data[r->index]=='\n' || r->index == r->max_size -2 ) {
//       r->recv_data[r->index + 1] = '\0';
//       HAL_UART_Transmit(&hlpuart1, r->recv_data, strlen(r->recv_data), 100);
//       r->index =0;
//     }
//     else{
//     r->index++;
//     }

//   }
// }

// void dequeue_ring(ring *r, state *s) {
//   // rear , front 다른 경우 출력

//   // (r->front) - front_flag 와 같이 시작점 관점으로도 보는 것 가능
//   uint8_t front_flag = r->front;


//   //escape 추가로 필요한지 확인하기
//   while ((r->rear) != (r->front)) {
//     // r->recv_data[r->index] = r->data[r->front];
    

//     //  r->front 부터 시작할 수 있게 하려면?  => front_flag가 r->front보다 더 큰 경우 생김 
//     // 2가지 다른 경우로 분기 해야할 것
//     // if로 먼저 자르기 
//     if (r->front == r->max_size -1) {
//        r->data[r->front + 1] = '\0';
//       HAL_UART_Transmit(&hlpuart1, (r->data) + front_flag, 100 - front_flag,
//                         100);
//       front_flag = 0;
//     }
//     else if (r->data[r->front]=='\n') {
//       r->data[r->front - 1] = '\n';
//       r->data[r->front] = '\0';
//       HAL_UART_Transmit(&hlpuart1, (r->data) + front_flag, r->front +1 - front_flag,
//                         100);
//       // front_flag = r->front;

//       // 작업속도 측면에서 생각 해보기
//       if (s->ready_state == false) {
//         //ready 판별
//         if ((strstr(r->data + front_flag+1, "OK"))) {
//           printf("[strstr]ESP8266 is ready \r\n");
//            s->ready_state = 1;
//         }
//       }
//       else if (s->wifi_state == false) {
//         if ((strstr(r->data + front_flag+1, "CON"))) {
//           printf("[strstr]WIFI is ready \r\n");
//             s->wifi_state = 1;
//         }
//       }
//       else if (s->parsing_state == false) {
//         if ((strstr(r->data + front_flag+1, "CONNECT"))) {
//           printf("[strstr]Parsing is ready \r\n");
//             s->parsing_state = 1;
//         }
//       }
//       else {
        
//         if (strstr(r->data + front_flag+1, "Date")) {
//           printf("[strstr]Get Time Data \r\n");
//           r->time_data = strstr(r->data + front_flag, "Date");
//         }
//         else if (strstr(r->data + front_flag + 1, "AT+CIPSEND")) {
//           s-> tcp_state =true;
//           //탈출 구문 추가
//           r->front=r->rear-1;
//         }
//         else if (strstr(r->data + front_flag + 1, "link is not valid")) {
//           printf("[strstr]reset esp8266 \r\n");
//           init_state(s);
//           restart_esp8266(r,s);
//         }

//       }
//       front_flag = r->front;
//     }
//               // r->front 는 무조건 더해주는 것이 맞음 
//     r->front=((r->front)+1) % (r->max_size);
//   }

//   // 초기화 함수
//   // 2개만 초기화해서 값 넣어보기 
//   r->rear = 0;
//   r->front = 0;

// }


void init_state(state *s) {
  s->ready_state = false;
  s->wifi_state = false;
  s->parsing_state = false;
  s->tcp_state = false;
}


void is_ready(ring *r, state *s) {

  // ready 체크

  printf("Try ready ESP8266 ... \r\n");
  printf("s->ready_state : %d \r\n",s->ready_state);
  // AT 보내기 -> 될 때 까지 진행
  uint8_t AT_Data[] = "AT\r\n";

  while (s->ready_state == false) {
    HAL_UART_Transmit(&huart1, AT_Data, strlen(AT_Data), 100);

    dequeue_ring(r);
    printf("Try ready ESP8266 ...>> \r\n");

    if ((s->ready_state == 1)) {
      printf("ESP8266 is ready \r\n");
    }

    HAL_Delay(1000);
  }

  // wait 하는 형식으로 바꿔보기
}

// 현재 특정 핫스팟만 접속하도록 설정
void is_wifi_enable(ring *r, state *s) {
  printf("Try ready WIFI ... \r\n");
  printf("s->wifi_state : %d \r\n",s->wifi_state);
  // AT 보내기 -> 될 때 까지 진행
  // AT+CWJAP="공유기 이름","공유기 PW"
  uint8_t AT_Data[] = "AT+CWJAP=\"book3\"\,\"22222222\"\r\n";

  while (s->wifi_state == false) {
    HAL_UART_Transmit(&huart1, AT_Data, strlen(AT_Data), 100);

    dequeue_ring(r);
    printf("Try ready WIFI ...>> \r\n");

    if (s->wifi_state == true) {
      printf("WIFI is ready \r\n");
    }

    HAL_Delay(1000);
  }
}

void is_parsing_enable(ring *r, state *s) {
  printf("Try ready Parsing ... \r\n");
  printf("s->parsing_state : %d \r\n",s->wifi_state);
  // AT 보내기 -> 될 때 까지 진행
  // AT+CWJAP="공유기 이름","공유기 PW"
  uint8_t AT_Data[] = "AT+CIPSTART=\"TCP\",\"www.naver.com\",80,5\r\n";

  while (s->parsing_state == false) {
    HAL_UART_Transmit(&huart1, AT_Data, strlen(AT_Data), 100);
    // HAL_Delay(1000);
    dequeue_ring(r);
    printf("Try ready Parsing ...>> \r\n");

    if (s->parsing_state == true) {
      printf("Parsing is ready \r\n");
    }

    HAL_Delay(1000);
  }
}

void test_uart(void) {
  //최종 목표
  // char AT_Data[] = "AT+CIPSTART=\"TCP\",\"www.naver.com\",80 \r\n";

  //테스트 진행
  // uint8_t AT_Data[] = "AT+CIPDINFO=1\r\n";
  uint8_t AT_Data[] = "AT+CWLAP=\"book3\"\r\n";


  //성공예제
  // char AT_Data[] = "AT+CIFSR\r\n";


  HAL_UART_Transmit(&huart1, (uint8_t *)AT_Data, strlen(AT_Data), 50);
}

void check_naver(void) {

  //테스트 진행
  uint8_t AT_Data[] = "AT+PING=\"www.naver.com\"\r\n";

  HAL_UART_Transmit(&huart1, (uint8_t *)AT_Data, strlen(AT_Data), 50);
}

// 우리가 보내줄 데이터 아니까 미리 보내기 -> 얘가 파싱까지 다 해서 while에?
// void send_TCP_data(ring *r, state *s,time *t) {
//   uint8_t AT_Data[] = "AT+CIPSEND=41\r\n";
//   HAL_UART_Transmit(&huart1, (uint8_t *)AT_Data, strlen(AT_Data), 50);

//   dequeue_ring(r, s);

//   HAL_Delay(1000);

//   if (s-> tcp_state ==true) {
//     uint8_t CIPSEND_Data[] = "GET / HTTP/1.1\r\nHost: www.naver.com\r\n\r\n";
//     HAL_UART_Transmit(&huart1, (uint8_t *)CIPSEND_Data, strlen(CIPSEND_Data),
//                       50);

//     dequeue_ring(r, s);

//     r->time_data[36] = '\0';
//     printf("time_data : %s \r\n", r->time_data);
//   }
//   s-> tcp_state = false;
// }

void restart_esp8266(ring *r, state *s) {
  uint8_t AT_Data[] = "AT+RST\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t *)AT_Data, strlen(AT_Data), 50);

  is_ready(r, s);
  is_wifi_enable(r, s);
  is_parsing_enable(r, s);
}

// 링버퍼에 넣는 역할만 함 
void enqueue_ring(ring *r, uint8_t input_data) {
  // rear 먼저 계산
  r->rear = (r->rear + 1) % r->max_size;
  r->data[r->rear] = input_data;
}

uint8_t dequeue_ring(ring *r) {
  if (r->rear == r->front) {
    return;
  }

  r->front = (r->front + 1) % r->max_size;

  return r->data[r->front];
}

void process_ring(ring *r) {
  // if (r->rear == r->front) {
  //   return;
  // }
  while (r->rear != r->front) {
    // 출력용 데이터와 버퍼 구현
    uint8_t recv_data = dequeue_ring(r);
    r->recv_data[r->recv_cnt] = recv_data;
    
    // 데이터를 ring으로 처리 가능? max_size일 때 대응 어떻게?
    // 얘는 ring 아니라 초기화 해주는 data
    if (r->recv_cnt == r->max_size - 2) {
      r->recv_data[r->max_size - 1] = '\0';
      HAL_UART_Transmit(&hlpuart1, r->recv_data, strlen(r->recv_data), 100);
      r->recv_cnt = 0;

      // 판정 함수 대입하기 
      get_state(r);
    }
    else if ((r->recv_data[r->recv_cnt])=='\n') {
      r->recv_data[r->recv_cnt + 1] = '\0';
      HAL_UART_Transmit(&hlpuart1, r->recv_data, strlen(r->recv_data), 100);
      r->recv_cnt = 0;

      get_state(r);
    }
    else {
      r->recv_cnt=(r->recv_cnt+1)%(r->max_size);

    }
  }
  
  get_state(r);
  
}

// process_ring 내부에 투입 예정 
//void get_state(ring *r,state *s) {
//
//  // ready 단계인 경우
//  if (s->ready_state==false) {
//    // 받는 데이터만 해석
//    if (strstr(r->recv_data,"OK")) {
//      printf("[strstr]ESP8266 is ready \r\n");
//      s->ready_state=true;
//    }
//  }
//  else if (s->wifi_state == false) {
//    if ((strstr(r->recv_data, "CON"))) {
//          printf("[strstr]WIFI is ready \r\n");
//            s->wifi_state = 1;
//    }
//  }
//  else if (s->parsing_state == false) {
//    if ((strstr(r->recv_data, "CONNECT"))) {
//      printf("[strstr]Parsing is ready \r\n");
//      s->parsing_state = 1;
//    }
//  }
//  else {
//    // 전부 true인 경우
//
//  }
//}

// r->recv_data 값 분석하기 -> process 함수 내부에서 사용하기 
void get_state(ring *r) {
  // case 따라서 판정기준 다르게! 
  switch (r->bootstep) {
    case ESP8266_READY_WAIT:
    if (strstr(r->recv_data,"OK")) {
      printf("[strstr]ESP8266_READY_WAIT : OK \r\n");
      r->bootstep = ESP8266_WIFI_SEND;
    }
    else {
      r->error_cnt++;
      if (r->error_cnt > r->error_cnt_max) {
        printf("[Change State]r->bootstep = ESP8266_READY_SEND; \r\n");
        r->bootstep = ESP8266_READY_SEND;
        r->error_cnt =0;
      }
    }
      break;
    case ESP8266_WIFI_WAIT:
    if (strstr(r->recv_data,"CONNECTED")) {
      printf("[strstr]ESP8266_WIFI_WAIT : OK \r\n");
      r->bootstep = ESP8266_CIPSTART_SEND;
    }
    else {
      r->error_cnt++;
      if (r->error_cnt > r->error_cnt_max) {
        printf("[Change State]r->bootstep = ESP8266_WIFI_SEND; \r\n");
        r->bootstep = ESP8266_WIFI_SEND;
        r->error_cnt =0;
      }
    }
      break;
    case ESP8266_CIPSTART_WAIT:
    if (strstr(r->recv_data,"CONNECT")) {
      printf("[strstr]ESP8266_CIPSTART_WAIT : OK \r\n");
      r->bootstep = ESP8266_CIPSEND_SEND;
    }
    else {
      r->error_cnt++;
      if (r->error_cnt >r->error_cnt_max) {
        printf("[Change State]r->bootstep = ESP8266_CIPSTART_SEND; \r\n");
        r->bootstep = ESP8266_CIPSTART_SEND;
        r->error_cnt =0;
      }
    }
      break;
    case ESP8266_CIPSEND_WAIT:
    if (strstr(r->recv_data,"OK")) {
        printf("[strstr]ESP8266_CIPSEND_WAIT : OK \r\n");
        r->bootstep = ESP8266_TCP_NAVER;
    }
    else {
      r->error_cnt++;
      if (r->error_cnt > r->error_cnt_max) {
        printf("[Change State]r->bootstep = ESP8266_CIPSEND_SEND; \r\n");
        r->bootstep = ESP8266_CIPSEND_SEND;
        r->error_cnt =0;
      }
    }
      break;
    case ESP8266_GET_TIME:
      if (strstr(r->recv_data,"Date")) {
      printf("[strstr]ESP8266_GET_TIME : OK \r\n");
      }
      else {
        printf("[strstr]ESP8266_GET_TIME : FAIL>> \r\n");
        r->error_cnt++;
        if (r->error_cnt>r->error_cnt_max) {
          r->bootstep = ESP8266_ERROR;
          printf("[Reset]ESP8266 \r\n");
          r->error_cnt =0;
        }
      }
      break;
    // case ESP8266_ERROR:
    //   if (strstr(r->recv_data,"ready")) {
    //     printf("[strstr]ESP8266_ERROR : OK \r\n");
    //     r->bootstep = ESP8266_READY_SEND;
    //  }
    //   break;
  }
}

void set_state(ring *r) {
//  uint8_t AT_CMD[] = {0,};
  // switch 문 사용하여 상태 바탕을 분기
  switch (r->bootstep) {
  case ESP8266_READY_SEND:
	  uint8_t AT_READY_CMD[] = "AT\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_READY_CMD, strlen(AT_READY_CMD), 50);
    r->bootstep = ESP8266_READY_WAIT;
    r->error_cnt =0;
    break;
  case ESP8266_WIFI_SEND:
	  uint8_t AT_GET_WIFI_CMD[] = "AT+CWJAP=\"book3\"\,\"22222222\"\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_GET_WIFI_CMD, strlen(AT_GET_WIFI_CMD), 50);
    r->bootstep = ESP8266_WIFI_WAIT;
    r->error_cnt =0;
    break;
  case ESP8266_CIPSTART_SEND:
	  uint8_t AT_PARSE_CMD[] = "AT+CIPSTART=\"TCP\",\"www.naver.com\",80\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_PARSE_CMD, strlen(AT_PARSE_CMD), 50);
    r->bootstep = ESP8266_CIPSTART_WAIT;
    r->error_cnt =0;
    break;
  case ESP8266_CIPSEND_SEND:
	  uint8_t AT_TCP_CMD[] = "AT+CIPSEND=37\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_TCP_CMD, strlen(AT_TCP_CMD), 50);
    r->bootstep = ESP8266_CIPSEND_WAIT;
    r->error_cnt =0;
    break;
  case ESP8266_TCP_NAVER:
	  uint8_t AT_TIME_CMD[] = "GET / HTTP/1.1\r\nHost: www.naver.com\r\n\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_TIME_CMD, strlen(AT_TIME_CMD), 1000);
    printf("strlen(AT_TIME_CMD) : %d \r\n",strlen(AT_TIME_CMD));
    r->bootstep = ESP8266_GET_TIME;
    r->error_cnt =0;
    break;
  case ESP8266_ERROR:
	  uint8_t AT_RST_CMD[] = "AT+RST\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_RST_CMD, strlen(AT_RST_CMD), 50);
    r->bootstep = ESP8266_READY_SEND;
    r->error_cnt =0;
    break;
  }

}

void restore_esp8266(void) {
  printf("Restore ESP8266 \r\n");
  uint8_t AT_CMD[] = "AT+RESTORE\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t *)AT_CMD, strlen(AT_CMD), 50);
}