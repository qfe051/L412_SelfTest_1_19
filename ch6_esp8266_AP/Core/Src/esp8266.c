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

static uint32_t wait_time_for_display(uint32_t tickstart);
static uint32_t wait_time_ms(uint32_t tickstart);
static bool time_parsing(uint8_t *buffer_data, time *t);
static void init_recv_buf(ring *r, esp_8266 *s);
static void send_AT_CMD(char *input_str);
static void esp_8266_reset(esp_8266 *s);
static void check_err_cnt(esp_8266 *s);

// const char -> AT 명령어 처리하기
const char *AT = "AT\r\n";
const char *ATE1 = "ATE1\r\n";
const char *ATE0 = "ATE0\r\n";
const char *AT_RESET = "AT+RST\r\n";
const char *AT_SET_WIFI = "AT+CWJAP=\"book3\"\,\"22222222\"\r\n";
const char *AT_WEB = "AT+CIPSTART=\"TCP\",\"www.naver.com\",80\r\n";
const char *AT_TCP_CMD = "AT+CIPSEND=0,239\r\n";
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

void enable_esp8266_echo(void) { send_AT_CMD(ATE1); }
void disable_esp8266_echo(void) { send_AT_CMD(ATE0); }

// 임시 문자열 구하기 함수 -> send 위함
print_html_strlen(void) {
  // 이 값 그대로 "AT+CIPSEND=0,239\r\n" 하기
  printf("strlen(AT_HTML) : %d \r\n", strlen(AT_HTML));
}

// str 함수 사용해서 값 이어보기, connect된 기기 까지!
// connect 기기 , strlen(AT_HTML)
char set_AT_CIPSEND() {
  // html 데이터
  char len_html[2048];
  // 연결된 기기 번호
  char num_dev[10];
  char s_return[50];

  char *AT_CIPSEND_1 = "AT+CIPSEND=";
  char *AT_CIPSEND_2 = ",";
  char *AT_CIPSEND_3 = "/r/n";

  // 여기에 파싱해서 값 넣기
  uint8_t num_dev_raw = 0;

  sprintf(len_html, "%d", strlen(AT_HTML));
  sprintf(num_dev, "%d", num_dev_raw);

  strcat(s_return, AT_CIPSEND_1);
  printf("s_return : %s \r\n", s_return);
  strcat(s_return, num_dev);
  printf("s_return : %s \r\n", s_return);
  strcat(s_return, AT_CIPSEND_2);
  printf("s_return : %s \r\n", s_return);
  strcat(s_return, len_html);
  printf("s_return : %s \r\n", s_return);
  strcat(s_return, AT_CIPSEND_3);
  printf("s_return : %s \r\n", s_return);

  // 확인 후 지우기
  printf("s_return : %s \r\n", s_return);

  return s_return;
}

// ring에 해당하는 것 빼고 다 넘기기 -> 다른 구조체 만들어서 분할
void init_esp_8266(esp_8266 *s, uint8_t *p_output_buf) {
  s->server_status = 0;
  s->recv_cnt = 0;
  s->recv_buf = p_output_buf;
  s->err_cnt = 0;
}

// 2차 과제 함수 작성

// 이번에는 return으로 문자열 반환 해보기

// 이 프로젝트에서만 이러한 형식으로 데이터 출력 ->ring 가져다가만 씀
bool process_ring_html(ring *r, esp_8266 *s) {
  while (r->rear != r->front) {
    // 출력용 데이터와 버퍼 구현
    if (dequeue_ring(r)) {
      // dequeue_ring(r)이 true 일 때, dequeue data 저장값 넣기
      s->recv_buf[s->recv_cnt] = r->dequeue_data;
    }

    // 데이터를 ring으로 처리 가능? max_size일 때 대응 어떻게?
    // 얘는 ring 아니라 초기화 해주는 data
    if (s->recv_cnt == r->max_size - 2) {
      s->recv_buf[r->max_size - 1] = '\0';
      HAL_UART_Transmit(&hlpuart1, s->recv_buf, strlen(s->recv_buf), 100);
      s->recv_cnt = 0;

    } else if ((s->recv_buf[s->recv_cnt]) == '\n') {
      s->recv_buf[s->recv_cnt + 1] = '\0';
      HAL_UART_Transmit(&hlpuart1, s->recv_buf, strlen(s->recv_buf), 100);
      s->recv_cnt = 0;

    } else {
      s->recv_cnt = (s->recv_cnt + 1) % (r->max_size);
    }
  }

  // s->recv_buf 문자열 길이가 있으면 true 반환
  if (strlen(s->recv_buf)) {
    return true;
  } else {
    return false;
  }
};

//
// get set 했던 것들 한번에 처리하는  함수 만들기 -enum 참고
// AP 설정, client 대기(connection 메시지 처리)
void sequence_html(ring *r, esp_8266 *s) {

  switch (s->server_status) {
  case ESP8266_READY_SEND:
    init_recv_buf(r, s);
    // HAL_UART_Transmit(&huart1, (uint8_t *)AT, strlen(AT), 50);
    send_AT_CMD(AT);
    s->server_status = ESP8266_READY_WAIT;
    tickstart_esp = HAL_GetTick();

    break;
  case ESP8266_READY_WAIT:
    if (!strcmp(s->recv_buf, APPLY_AT)) {
      printf("[strcmp]ESP8266_WIFI_WAIT : OK \r\n");
      s->server_status = ESP8266_CWMODE_SEND;
      s->err_cnt = 0;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->server_status = ESP8266_READY_SEND; \r\n");
      s->server_status = ESP8266_READY_SEND;
      check_err_cnt(s);
    }
    break;
  case ESP8266_CWMODE_SEND:
    init_recv_buf(r, s);
    send_AT_CMD(AT_CWMODE);
    s->server_status = ESP8266_CWMODE_WAIT;
    tickstart_esp = HAL_GetTick();
    break;

  case ESP8266_CWMODE_WAIT:
    if (!strcmp(s->recv_buf, APPLY_AT)) {
      printf("[strcmp]ESP8266_CWMODE_WAIT : OK \r\n");
      s->server_status = ESP8266_CIPMUX_SEND;
      s->err_cnt = 0;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->server_status = ESP8266_CWMODE_SEND; \r\n");
      s->server_status = ESP8266_CWMODE_SEND;
      check_err_cnt(s);
    }
    break;
  case ESP8266_CIPMUX_SEND:
    init_recv_buf(r, s);
    send_AT_CMD(AT_CIPMUX);
    s->server_status = ESP8266_CIPMUX_WAIT;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_CIPMUX_WAIT:
    if (!strcmp(s->recv_buf, APPLY_AT)) {
      printf("[strcmp]ESP8266_CIPMUX_WAIT : OK \r\n");
      s->server_status = ESP8266_CIPSERVER_SEND;
      s->err_cnt = 0;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->server_status = ESP8266_CIPMUX_SEND; \r\n");
      s->server_status = ESP8266_CIPMUX_SEND;
      check_err_cnt(s);
    }
    break;
  case ESP8266_CIPSERVER_SEND:
    init_recv_buf(r, s);
    send_AT_CMD(AT_CIPSERVER);
    s->server_status = ESP8266_CIPSERVER_WAIT;
    tickstart_esp = HAL_GetTick();
    break;
  case ESP8266_CIPSERVER_WAIT:
    if (!strcmp(s->recv_buf, APPLY_AT)) {
      printf("[strcmp]ESP8266_CIPSERVER_WAIT _1: OK \r\n");
      // status 처리 어떻게? -> 별도의 변수 건들기
      s->server_status = ESP8266_DEVICE_WAIT;
      s->err_cnt = 0;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME * 600) {
      // 10분마다 상태 전환 함 -> 필요 없을 듯
      printf("[Change State]s->server_status = ESP8266_CIPSERVER_SEND; \r\n");
      s->server_status = ESP8266_CIPSERVER_SEND;
      check_err_cnt(s);
    }
    break;
  case ESP8266_DEVICE_WAIT:
    //"0,CONNECT" 로 변경하기
    if (strstr(s->recv_buf, "GET")) {
      s->server_status = ESP8266_D_CIPSEND_SEND;
      tickstart_esp = HAL_GetTick();
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[ESP8266_DEVICE_WAIT] : WAIT DEVICE \r\n");
      tickstart_esp = HAL_GetTick();
    }
    break;
  case ESP8266_D_CIPSEND_SEND:
    init_recv_buf(r, s);
    // 몇번 포트로 들어왔는지에 따른 분기 필요
    if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      send_AT_CMD(AT_D_CIPSEND);
      s->server_status = ESP8266_D_CIPSEND_WAIT;
      tickstart_esp = HAL_GetTick();
    }
    break;
  case ESP8266_D_CIPSEND_WAIT:
    // 값 판단 읽어오기 못함
    // if (!strcmp(s->recv_buf, APPLY_AT) | !strcmp(s->recv_buf,
    // AT_D_CIPSEND))
    if (1) {
      printf("[strcmp]ESP8266_D_CIPSEND_WAIT : OK \r\n");
      s->server_status = ESP8266_HTML_SEND;
      tickstart_esp = HAL_GetTick();
      s->err_cnt = 0;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->server_status = ESP8266_D_CIPSEND_SEND; \r\n");
      s->server_status = ESP8266_D_CIPSEND_SEND;
      check_err_cnt(s);
    }
    break;
  case ESP8266_HTML_SEND:
    init_recv_buf(r, s);
    if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      send_AT_CMD(AT_HTML);
      s->server_status = ESP8266_HTML_WAIT;
      // s->server_status = ESP8266_BUTTON_WAIT;
      tickstart_esp = HAL_GetTick();
    }
    break;
  case ESP8266_HTML_WAIT:
    if (strstr(s->recv_buf, "Recv")) {
      printf("[strcmp]ESP8266_HTML_WAIT : OK \r\n");
      s->server_status = ESP8266_BUTTON_WAIT;
      s->err_cnt = 0;
    } else if (wait_time_ms(tickstart_esp) > WAIT_TIME) {
      printf("[Change State]s->server_status = ESP8266_HTML_SEND; \r\n");
      s->server_status = ESP8266_HTML_SEND;
      check_err_cnt(s);
    }
    break;
  case ESP8266_BUTTON_WAIT:
    if (strstr(s->recv_buf, "/led/on")) {
      printf("[Change LED State] LED_ON \r\n\r\n");
      HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
    } else if (strstr(s->recv_buf, "/led/off")) {
      printf("[Change LED State] LED_OFF \r\n\r\n");
      HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    }
    break;
  }
}

void check_connection(ring *r, esp_8266 *s) {

  if (!strcmp(s->recv_buf, "ready\r\n")) {
    s->server_status = ESP8266_READY_SEND;
    printf(
        "<+ready> [Change State]s->server_status =  ESP8266_READY_SEND \r\n");
    // s->recv_buf의 memset을 함수화 하기
    init_recv_buf(r, s);
    // memset(s->recv_buf, 0, sizeof(uint8_t) * r->max_size);
  } else if (!strcmp(s->recv_buf, "0,CLOSED\r\n")) {
    s->server_status = ESP8266_DEVICE_WAIT;
    printf("<0,CLOSED> [Change State]s->server_status =  ESP8266_DEVICE_WAIT "
           "\r\n");
    init_recv_buf(r, s);
    // memset(s->recv_buf, 0, sizeof(uint8_t) * r->max_size);
  }
}

static void init_recv_buf(ring *r, esp_8266 *s) {
  memset(s->recv_buf, 0, sizeof(uint8_t) * r->max_size);
}

static void send_AT_CMD(char *input_str) {
  HAL_UART_Transmit(&huart1, (uint8_t *)input_str, strlen(input_str), 50);
  // 지우기
  // printf("strlen(input_str) : %d \r\n", strlen(input_str));
  // printf("input_str : %s \r\n", input_str);
}

static void send_serial(char *input_str) {
  HAL_UART_Transmit(&hlpuart1, (uint8_t *)input_str, strlen(input_str), 50);
}

// ring 초기화 필요?
static void esp_8266_reset(esp_8266 *s) {
  // esp_8266 구조체 일부 초기화
  s->server_status = ESP8266_READY_SEND;
  s->recv_cnt = 0;
  s->err_cnt = 0;

  send_AT_CMD(AT_RESET);
}

static void check_err_cnt(esp_8266 *s) {
  s->err_cnt++;
  printf("[ERROR] error count is %d \r\n", s->err_cnt);
  printf("If [error count==5], will be Reset \r\n");

  if (s->err_cnt > 4) {
    esp_8266_reset(s);
  }
}


void restore_esp8266(void) {
  printf("Restore ESP8266 \r\n");
  // uint8_t AT_CMD[] = "AT+RESTORE\r\n";
  send_AT_CMD(AT_RESTORE);
}

// if 문에서 WAIT_TIME 이랑 조합해서 사용해보기
uint32_t wait_time_ms(uint32_t tickstart) {
  uint32_t wait_time = HAL_GetTick() - tickstart;

  return wait_time;
}
