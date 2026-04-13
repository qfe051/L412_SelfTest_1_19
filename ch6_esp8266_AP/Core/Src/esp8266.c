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
const char *AT_TCP_CMD = "AT+CIPSEND=0,771\r\n";
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

// 파싱으로 업그레이드 하기
const char *AT_LED_OFF = "+IPD,1,480:GET /led/off HTTP/1.1\r\n";
const char *AT_LED_ON = "+IPD,1,479:GET /led/on HTTP/1.1\r\n";

// device 관련 명령어
const char *AT_HTML =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"
    "\r\n"
    "<html>\r\n"
    "  <body>\r\n"
    "    <h1>ESP8266 LED Control</h1>\r\n"
    "    <a href=\"/led/on\"><button>LED ON</button></a>\r\n"
    "    <a href=\"/led/off\"><button>LED OFF</button></a>\r\n"
    "  </body>\r\n"
    "</html>\r\n";

const char *AT_D_CIPSEND = "AT+CIPSEND=0,239\r\n";

void enable_esp8266_echo(void) {

  //echo 없음
  // char AT_Data[] = "ATE1\r\n";
  HAL_UART_Transmit(&huart1, ATE1, sizeof(ATE1)-1, 50);
}

// 임시 문자열 구하기 함수 -> send 위함
print_html_strlen(void) {
  printf("strlen(AT_HTML) : %d \r\n", strlen(AT_HTML));
  printf("sizeof(AT_HTML) : %d \r\n", sizeof(AT_HTML));
}

// ring에 해당하는 것 빼고 다 넘기기 -> 다른 구조체 만들어서 분할

// r->data = buf; 위치에 buf -> 주소형태로 써야함
// 초기화 해주는 시점은? 시작부 or 지속적으로
// uint8_t buf_recv_data[] -> process로 이동
void init_ring(ring *r, uint8_t buf_data[]) {
  r->rear = 0;
  r->front = 0;
  r->data = buf_data;
  r->recv_cnt = 0;
  r->max_size = BUF_SIZE;
}

void init_status(status *s) {
  s->server_status = 0;
  s->d_connect_status = -1;
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

//
// get set 했던 것들 한번에 처리하는  함수 만들기 -enum 참고
// AP 설정, client 대기(connection 메시지 처리)
void ready_sequence_html(ring *r, uint8_t buf_recv_data[], status *s) {
  // raw_data 변수 선언 후 내부에서 데이터 처리
  uint8_t raw_data[100] = {
      0,
  };

  if (process_ring_html(r,buf_recv_data)) {
    strcpy(raw_data,buf_recv_data);
  }
  // test용
  // printf("sequence :%s \r\n", raw_data);

  // raw_data 기반 sequence 작성하기

  switch (s->server_status) {
  case ESP8266_READY_SEND:
    HAL_UART_Transmit(&huart1, (uint8_t *)AT, strlen(AT), 50);
    s->server_status = ESP8266_READY_WAIT;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_READY_WAIT:
    if (!strcmp(raw_data, APPLY_AT)) {
      printf("[strcmp]ESP8266_WIFI_WAIT : OK \r\n");
      s->server_status = ESP8266_CWMODE_SEND;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->server_status = ESP8266_READY_SEND; \r\n");
      s->server_status = ESP8266_READY_SEND;
    }
    break;
  case ESP8266_CWMODE_SEND:
    HAL_UART_Transmit(&huart1, (uint8_t *)AT_CWMODE, strlen(AT_CWMODE), 50);
    s->server_status = ESP8266_CWMODE_WAIT;
    tickstart_esp = HAL_GetTick();
    break;

  case ESP8266_CWMODE_WAIT:
    if (!strcmp(raw_data, APPLY_AT)) {
      printf("[strcmp]ESP8266_CWMODE_WAIT : OK \r\n");
      s->server_status = ESP8266_CIPMUX_SEND;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->server_status = ESP8266_CWMODE_SEND; \r\n");
      s->server_status = ESP8266_CWMODE_SEND;
    }
    break;
  case ESP8266_CIPMUX_SEND:
    HAL_UART_Transmit(&huart1, (uint8_t *)AT_CIPMUX, strlen(AT_CIPMUX), 50);
    s->server_status = ESP8266_CIPMUX_WAIT;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_CIPMUX_WAIT:
    if (!strcmp(raw_data, APPLY_AT)) {
      printf("[strcmp]ESP8266_CIPMUX_WAIT : OK \r\n");
      s->server_status = ESP8266_CIPSERVER_SEND;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->server_status = ESP8266_CIPMUX_SEND; \r\n");
      s->server_status = ESP8266_CIPMUX_SEND;
    }
    break;
  case ESP8266_CIPSERVER_SEND:
    HAL_UART_Transmit(&huart1, (uint8_t *)AT_CIPSERVER, strlen(AT_CIPSERVER),
                      50);
    s->server_status = ESP8266_CIPSERVER_WAIT;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_CIPSERVER_WAIT:
    if (!strcmp(raw_data, APPLY_AT)) {
      printf("[strcmp]ESP8266_CIPSERVER_WAIT _1: OK \r\n");
      // status 처리 어떻게? -> 별도의 변수 건들기
      s->d_connect_status = ESP8266_DEVICE_WAIT;
      s->server_status = -1;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME * 600) {
      // 10분마다 상태 전환 함 -> 필요 없을 듯
      printf("[Change State]s->server_status = ESP8266_CIPSERVER_SEND; \r\n");
      s->server_status = ESP8266_CIPSERVER_SEND;
    }
    break;
  }
}

// html 데이터 전송 , LED 기능
void connect_html(ring *r, uint8_t buf_recv_data[], status *s) {
  // raw_data 변수 선언 후 내부에서 데이터 처리
  uint8_t raw_data[100] = {
      0,
  };

  if (process_ring_html(r, buf_recv_data)) {
    strcpy(raw_data, buf_recv_data);
  }

  switch (s->d_connect_status) {
  case ESP8266_DEVICE_WAIT:
    if (strstr(raw_data, "GET")) {
      printf("[strcmp]ESP8266_CIPSERVER_WAIT _2 : OK \r\n");
      s->d_connect_status = ESP8266_D_CIPSEND_SEND;
      tickstart_esp = HAL_GetTick();
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[ESP8266_DEVICE_WAIT] : WAIT DEVICE \r\n");
      tickstart_esp = HAL_GetTick();
    }
    break;
  case ESP8266_D_CIPSEND_SEND:
    if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      HAL_UART_Transmit(&huart1, (uint8_t *)AT_D_CIPSEND, strlen(AT_D_CIPSEND),
                        50);
      s->d_connect_status = ESP8266_D_CIPSEND_WAIT;
      tickstart_esp = HAL_GetTick();
    }
    break;
  case ESP8266_D_CIPSEND_WAIT:
    // 값 판단 읽어오기 못함
    // if (!strcmp(raw_data, APPLY_AT) | !strcmp(raw_data, AT_D_CIPSEND))
    if (1) {
      printf("[strcmp]ESP8266_D_CIPSEND_WAIT : OK \r\n");
      s->d_connect_status = ESP8266_HTML_SEND;
      tickstart_esp = HAL_GetTick();
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf(
          "[Change State]s->d_connect_status = ESP8266_D_CIPSEND_SEND; \r\n");
      s->d_connect_status = ESP8266_D_CIPSEND_SEND;
    }
    break;
  case ESP8266_HTML_SEND:
    if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      HAL_UART_Transmit(&huart1, (uint8_t *)AT_HTML, strlen(AT_HTML), 50);
      s->d_connect_status = ESP8266_HTML_WAIT;
      // s->d_connect_status = ESP8266_BUTTON_WAIT;
      tickstart_esp = HAL_GetTick();
    }
    break;
  case ESP8266_HTML_WAIT:
    if (strstr(raw_data, "Recv")) {
      printf("[strcmp]ESP8266_HTML_WAIT : OK \r\n");
      s->d_connect_status = ESP8266_BUTTON_WAIT;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->d_connect_status = ESP8266_HTML_SEND; \r\n");
      s->d_connect_status = ESP8266_HTML_SEND;
    }
    break;
  case ESP8266_BUTTON_WAIT:
    if (strstr(raw_data, "/led/on")) {
      printf("[Change LED State] LED_ON \r\n\r\n");
      HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
    } else if (strstr(raw_data, "/led/off")) {
      printf("[Change LED State] LED_OFF \r\n\r\n");
      HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    }
    break;
  }
}

void test_uart(void) {
  HAL_UART_Transmit(&huart1, (uint8_t *)AT, strlen(AT), 50);
}

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

