#include "esp8266.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define WAIT_TIME 5000
#define WAIT_TIME_DISPLAY 1000

#ifndef BUF_SIZE
#define BUF_SIZE 100
#endif

uint32_t tickstart_esp = 0;
uint32_t tickstart_display = 0;
// flag 사용 어떻게 할지 생각해보기=

static void restart_esp8266(ring *r, state *s);
static void get_state(ring *r, time *t);
static uint32_t wait_time_for_display(uint32_t tickstart);
static uint32_t wait_time_ms(uint32_t tickstart);
static bool time_parsing(uint8_t *buffer_data, time *t);

// const char -> AT 명령어 처리하기
const char *AT = "AT\r\n";
const char *ATE1 = "ATE1\r\n";
const char *AT_RESET = "AT+RST\r\n";
const char *AT_SET_WIFI = "AT+CWJAP=\"book3\"\,\"22222222\"\r\n";
const char *AT_WEB = "AT+CIPSTART=\"TCP\",\"www.naver.com\",80\r\n";
const char *AT_TCP_CMD = "AT+CIPSEND=39\r\n";
const char *AT_GET_HTML = "GET / HTTP/1.1\r\nHost: www.naver.com\r\n\r\n";
const char *AT_RESTORE = "AT+RESTORE\r\n";
const char *AT_SEARCH_WIFI = "AT+CWLAP\r\n";
const char *AT_CWMODE = "AT+CWMODE=2\r\n"; // OK
const char *AT_CIPMUX = "AT+CIPMUX=1\r\n"; // OK
const char *AT_CIPSERVER = "AT+CIPSERVER=1\r\n"; // OK 

const char *APPLY_AT = "OK\r\n";
const char *APPLY_AT_SET_WIFI = "WIFI CONNECTED\r\n";
const char *APPLY_AT_SET_WIFI_2 = "WIFI GOT IP\r\n";
const char *APPLY_AT_WEB = "CONNECT\r\n";
const char *APPLY_AT_WEB_2 = "ALREADY CONNECTED\r\n";
const char *APPLY_AT_TCP_CMD = "OK\r\n";
const char *APPLY_AT_TCP_NOT_VALID = "link is not valid\r\n";



void init_esp8266(void) {

  //echo 없음
  // char AT_Data[] = "ATE1\r\n";
  HAL_UART_Transmit(&huart1, ATE1, sizeof(ATE1)-1, 50);
}

// ring에 해당하는 것 빼고 다 넘기기 -> 다른 구조체 만들어서 분할

// r->data = buf; 위치에 buf -> 주소형태로 써야함
// 초기화 해주는 시점은? 시작부 or 지속적으로
// uint8_t buf_recv_data[] -> process로 이동
void init_ring(ring *r,uint8_t buf_data[]) {
  
  r->rear = 0;
  r->front = 0;
  r->data = buf_data;
  r->recv_cnt = 0;
  r->max_size = BUF_SIZE;
}

// 링버퍼에 넣는 역할만 함
// 포화상태 처리 추가 
void enqueue_ring(ring *r, uint8_t input_data) {
  // rear 먼저 계산

  if (((r->rear + 1) % r->max_size) == (r->front)) {
    // 포화 상태에서 동작 안함 -> 덮어쓰기 방지
  }
  else {
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

// 2차 과제 함수 작성

// 이번에는 return으로 문자열 반환 해보기

bool process_ring_html(ring *r,uint8_t buf_recv_data[]) {
    while (r->rear != r->front) {
    // 출력용 데이터와 버퍼 구현
    buf_recv_data[r->recv_cnt] = dequeue_ring(r);
    
    // 데이터를 ring으로 처리 가능? max_size일 때 대응 어떻게?
    // 얘는 ring 아니라 초기화 해주는 data
    if (r->recv_cnt == r->max_size - 2) {
      buf_recv_data[r->max_size - 1] = '\0';
      HAL_UART_Transmit(&hlpuart1, buf_recv_data, strlen(buf_recv_data), 100);
      r->recv_cnt = 0;

    }
    else if ((buf_recv_data[r->recv_cnt])=='\n') {
      buf_recv_data[r->recv_cnt + 1] = '\0';
      HAL_UART_Transmit(&hlpuart1, buf_recv_data, strlen(buf_recv_data), 100);
      r->recv_cnt = 0;

    }
    else {
      r->recv_cnt=(r->recv_cnt+1)%(r->max_size);

    }
    }

    if (buf_recv_data == NULL) {
      return false;
    }

  // buf_recv_data 반환하기
  // 이후에 strcpy로 문자열 복사

  return true;
};

// get set 했던 것들 한번에 처리하는  함수 만들기
// AP 설정, client 대기(connection 메시지 처리)
void sequence_html(ring *r, uint8_t buf_recv_data[],status *s) {
	uint8_t raw_data[100] = {0,};
  if (process_ring_html(r,buf_recv_data)) {
    strcpy(raw_data,buf_recv_data);
  }
  // test용
  printf("sequence :%s \r\n", raw_data);

  // raw_data 기반 sequence 작성하기

  switch (s->server_status) {
  
  }
}

void led_button_html() {
  
}

void test_uart(void) {
  HAL_UART_Transmit(&huart1, (uint8_t *)AT, strlen(AT), 50);
}




/*
// strstr 대신 strcmp 사용해보기 
// buf_recv_data 값 분석하기 -> process 함수 내부에서 사용하기 
void get_state(ring *r, time *t) {
  // case 따라서 판정기준 다르게! 
  switch (r->bootstep) {
  case ESP8266_READY_WAIT:

    
    
    if (!strcmp(buf_recv_data,APPLY_AT)) {
      printf("[strcmp]ESP8266_READY_WAIT : OK \r\n");
      r->bootstep = ESP8266_WIFI_SEND;
    }
    else if(wait_time_ms(tickstart_esp)>WAIT_TIME){
        printf("[Change State]r->bootstep = ESP8266_READY_SEND; \r\n");
        r->bootstep = ESP8266_READY_SEND;
    }
    
      break;
  case ESP8266_WIFI_WAIT:
    if ((!strcmp(buf_recv_data,APPLY_AT_SET_WIFI))|(!strcmp(buf_recv_data,APPLY_AT_SET_WIFI_2))) {
      printf("[strcmp]ESP8266_WIFI_WAIT : OK \r\n");
      r->bootstep = ESP8266_CIPSTART_SEND;
    }
    else if (wait_time_ms(tickstart_esp)>WAIT_TIME) {
        printf("[Change State]r->bootstep = ESP8266_WIFI_SEND; \r\n");
        r->bootstep = ESP8266_WIFI_SEND;
      }
    
  
      break;
  case ESP8266_CIPSTART_WAIT:
    if (!strcmp(buf_recv_data,APPLY_AT_WEB)|!strcmp(buf_recv_data,APPLY_AT_WEB_2)) {
      printf("[strcmp]ESP8266_CIPSTART_WAIT : OK \r\n");
      r->bootstep = ESP8266_CIPSEND_SEND;
    }
    else if (wait_time_ms(tickstart_esp)>WAIT_TIME) {
        printf("[Change State]r->bootstep = ESP8266_CIPSTART_SEND; \r\n");
        r->bootstep = ESP8266_CIPSTART_SEND;
      }
    
  
      break;
  case ESP8266_CIPSEND_WAIT:
    if (!strcmp(buf_recv_data,APPLY_AT_TCP_CMD)) {
        printf("[strcmp]ESP8266_CIPSEND_WAIT : OK \r\n");
        r->bootstep = ESP8266_TCP_NAVER;
    }
    else if (!strcmp(buf_recv_data,APPLY_AT_TCP_NOT_VALID)) {
        printf("[Change State] r->bootstep = ESP8266_CIPSTART_SEND \r\n");
        r->bootstep = ESP8266_CIPSTART_SEND;
    }
    else if(wait_time_ms(tickstart_esp)>WAIT_TIME){
      r->error_cnt++;
      if (r->error_cnt > r->error_cnt_max) {
        printf("[Change State]r->bootstep = ESP8266_CIPSEND_SEND; \r\n");
        r->bootstep = ESP8266_CIPSEND_SEND;
        r->error_cnt =0;
      }
    }
  
      break;
  // 내부 파싱 알고리즘 설계하기    
  case ESP8266_GET_TIME:
      if (strstr(buf_recv_data,"Date")) {
        printf("[strstr]ESP8266_GET_TIME : OK \r\n");
        // data 기반 파싱작업
        printf(" <Date> address : %d \r\n", strstr(buf_recv_data, "Date"));
        tickstart_esp = HAL_GetTick();

        time_parsing(buf_recv_data,t);

        return;
      }
      else if (wait_time_ms(tickstart_esp)>(WAIT_TIME*10)) {
          printf("[strcmp]ESP8266_GET_TIME : FAIL>> \r\n");
          r->bootstep = ESP8266_ERROR;
          printf("[Reset]ESP8266 \r\n");
          r->error_cnt =0;
        }
      
      break;
      // case ESP8266_ERROR:
      //   if (strstr(buf_recv_data,"ready")) {
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
	  // uint8_t AT_READY_CMD[] = "AT\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT, strlen(AT), 50);
    r->bootstep = ESP8266_READY_WAIT;
    r->error_cnt = 0;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_WIFI_SEND:
	  // uint8_t AT_GET_WIFI_CMD[] = "AT+CWJAP=\"book3\"\,\"22222222\"\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_SET_WIFI, strlen(AT_SET_WIFI), 50);
    r->bootstep = ESP8266_WIFI_WAIT;
    r->error_cnt = 0;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_CIPSTART_SEND:
	  // uint8_t AT_PARSE_CMD[] = "AT+CIPSTART=\"TCP\",\"www.naver.com\",80\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_WEB, strlen(AT_WEB), 50);
    r->bootstep = ESP8266_CIPSTART_WAIT;
    r->error_cnt = 0;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_CIPSEND_SEND:
	  // uint8_t AT_TCP_CMD[] = "AT+CIPSEND=40\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_TCP_CMD, strlen(AT_TCP_CMD), 50);
    r->bootstep = ESP8266_CIPSEND_WAIT;
    r->error_cnt = 0;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_TCP_NAVER:
	  // uint8_t AT_TIME_CMD[] = "GET / HTTP/1.1\r\nHost: www.naver.com\r\n\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_GET_HTML, strlen(AT_GET_HTML), 1000);
    printf("strlen(AT_GET_HTML) : %d \r\n",strlen(AT_GET_HTML));
    r->bootstep = ESP8266_GET_TIME;
    r->error_cnt = 0;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_ERROR:
	  // uint8_t AT_RST_CMD[] = "AT+RST\r\n";
	  HAL_UART_Transmit(&huart1, (uint8_t *)AT_RESET, strlen(AT_RESET), 50);
    r->bootstep = ESP8266_READY_SEND;
    r->error_cnt =0;
    break;
  }
}
*/

void restore_esp8266(void) {
  printf("Restore ESP8266 \r\n");
  // uint8_t AT_CMD[] = "AT+RESTORE\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t *)AT_RESTORE, strlen(AT_RESTORE), 50);
}

// if 문에서 WAIT_TIME 이랑 조합해서 사용해보기
uint32_t wait_time_ms(uint32_t tickstart) {
  uint32_t wait_time = HAL_GetTick() - tickstart;

  return wait_time;
}

uint32_t wait_time_for_display(uint32_t tickstart) {
  uint32_t wait_time = HAL_GetTick() - tickstart;

  return wait_time;
}

// 파싱할 데이터 넣기, 시간 데이터용 구조체 |
bool time_parsing(uint8_t *buffer_data, time *t) {

  printf("parsing data : \r\n");
  //버리는 데이터 - 1번째 
  char *ptr = strtok(buffer_data,":, ");



  // ptr로 순서대로 대입하기
  // t->date
  strcpy(t->day, strtok(NULL, ":, "));
  t->date = atoi(strtok(NULL, ":, "));
  strcpy(t->month, strtok(NULL, ":, "));
  t->year = atoi(strtok(NULL, ":, "));
  t->hour = atoi(strtok(NULL, ":, "));
  t->min = atoi(strtok(NULL, ":, "));
  t->sec = atoi(strtok(NULL, ":, "));

  // 출력
   printf("t->day %s \r\n", t->day);
  printf("t->date %d \r\n", t->date);
  printf("t->month %s \r\n", t->month);
  printf("t->year %d \r\n", t->year);
  printf("t->hour %d \r\n", t->hour);
  printf("t->min %d \r\n", t->min);
  printf("t->sec %d \r\n", t->sec);
  

  // while (ptr != NULL) {
  //   printf("%s \r\n", ptr);
  //   ptr = strtok(NULL,":, ");
  // }
}

bool display_time(time *t) {
  // 예외처리
  if (t==NULL) {
    printf("Time data is NULL  \r\n");
    return false;
  }
  // 한국 시간으로 업데이트
  // t->hour = t->hour + 9;
  // if (t->hour>23) {
  //   t->hour = t->hour % 24;
  //   // date 업데이트
  //   t->date = t->date + 1;

  //   // 월별 처리 알고리즘 작성하기 
  //   if (t->date>=28) {
    
  //   }
  // }

  if (wait_time_for_display(tickstart_display)>=WAIT_TIME_DISPLAY) {
    // 출력
  printf("date : %d.%d.%d  (%s) \r\n", t->year, t->month, t->date, t->day);
  printf("time : %d:%d:%d \r\n", t->hour, t->min, t->sec);
  }

  tickstart_display = HAL_GetTick();
}

