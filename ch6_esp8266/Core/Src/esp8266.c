#include "esp8266.h"

#include <stdbool.h>
#include <stdint.h>

uint8_t rx_data_esp;
User_ring rx_ring_esp;
bool recv_flag = true;

#define RECV_BUF_SIZE 100
#define RECV_TIMEOUT 5000
#define RECV_WAIT_TIME 1000

#define LED_ON HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET)
#define LED_OFF HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET)
#define RST_RECV_BUF memset(recv_buf, 0, sizeof(uint8_t) * RECV_BUF_SIZE)

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

typedef enum _esp8266_step {
  ESP8266_READY_SEND = 0,
  ESP8266_READY_WAIT,
  ESP8266_CWMODE_SEND,
  ESP8266_CWMODE_WAIT,
  ESP8266_CIPMUX_SEND,
  ESP8266_CIPMUX_WAIT,
  ESP8266_CIPSERVER_SEND,
  ESP8266_CIPSERVER_WAIT,

  ESP8266_DEVICE_WAIT,
  ESP8266_D_CIPSEND_SEND,
  ESP8266_D_CIPSEND_WAIT,
  ESP8266_HTML_SEND,
  ESP8266_HTML_WAIT,
  ESP8266_BUTTON_WAIT

} ESP_8266_STEP;

static uint8_t recv_buf[RECV_BUF_SIZE];
static uint8_t recv_cnt;
static uint32_t tickstart_esp;
static AT_Response
    last_resp; // 마지막 응답 저장 -> 이 응답 바탕으로 나머지 연산들 진행
static ESP_8266_STEP esp8266_step;

// esp8266.c 내부에서 사용하는 함수

static AT_Response parse_response(uint8_t *raw_data);

static bool send_AT_CMD(char *input_str);
static bool send_Serial(char *input_str);
static bool recv_ring_data(uint8_t *p_rxdata, uint8_t recv_size);

// static uint8_t recv_buf[RECV_BUF_SIZE]; 활용하기
//
// 링버퍼로 부터 받을 데이터 [인자] 1)주소, 2)크기 설정
// [반환값]F : p_rxdata==NULL||recv_size==0 인 경우, | T : 이외 모든 경우
static bool recv_ring_data(uint8_t *p_rxdata, uint8_t recv_size) {
  if (p_rxdata==NULL||recv_size==0) {
    return false;
  }
  for (int i = 0; i < recv_size; i++) {
    if (is_empty(&rx_ring_esp)) {
    } else {
      dequeue(&rx_ring_esp, &p_rxdata[i]);
    }
  }
  return true;
}

// main의 while에서 처리하기엔 부족 -> 별도의 반복루프 필요함
// flag 방식이든지 사용해보기

// main.c의 while문 내에서 반복적으로, 처리 예정
// [목적] : 링버퍼내의 데이터 출력용 버퍼(recv_buf)에 1)저장, 2)serial 출력
void recv_data_task(void) {

  uint8_t recv_data;

  // 추출할 데이터 있으면 반복 동작하기
  while (!is_empty(&rx_ring_esp)) {

    // data를 받는지부터 확인(recv_ring_data) -> 1개 데이터 뽑는 동작 성공하는지
    // check
    if (recv_ring_data(&recv_data, 1)) {
      recv_buf[recv_cnt] = recv_data;

      if (recv_cnt == RECV_BUF_SIZE - 2) {
        recv_buf[RECV_BUF_SIZE - 1] = '\0';
        send_Serial(recv_buf);
        last_resp = parse_response(recv_buf);

        recv_cnt = 0;

      } else if (recv_buf[recv_cnt] == '\n') {
        recv_buf[recv_cnt + 1] = '\0';
        send_Serial(recv_buf);
        last_resp = parse_response(recv_buf);

        recv_cnt = 0;
      } else {
        recv_cnt = (recv_cnt + 1) % RECV_BUF_SIZE;
      }
    }
  }
  return;
}

// AT CMD 전송 | 입력 : input_str 주소 , 출력 : T/F
static bool send_AT_CMD(char *input_str) {
  if (input_str == NULL || &huart1 == NULL) {

    return false;
  }
  last_resp.type = RESP_UNKNOWN;
  HAL_UART_Transmit(&huart1, (uint8_t *)input_str, strlen(input_str), 50);

  return true;
}

// Serial 출력 용도 | 입력 : input_str , 출력 : T/F
static bool send_Serial(char *input_str) {
  if (input_str == NULL || &hlpuart1 == NULL) {

    return false;
  }
  HAL_UART_Transmit(&hlpuart1, (uint8_t *)input_str, strlen(input_str), 50);

  return true;
}

// recv_buf 내의 데이터를 AT_Response 구조체 형태로 파싱| 입력 : recv_buf
// ,출력 : AT_Response 구조체 데이터
static AT_Response parse_response(uint8_t *raw_data) {
  // AT_Response 구조체 초기화
  AT_Response resp = {RESP_UNKNOWN, {0}, 0};

  // strcmp 반환값이 0인지 확인
  // type 부터 확인
  if (strcmp((char *)raw_data, "OK\r\n") == 0) {
    resp.type = RESP_OK;
  } else if (strcmp((char *)raw_data, "ERROR\r\n") == 0) {
    resp.type = RESP_ERROR;
  } else if (strcmp((char *)raw_data, "ready\r\n") == 0) {
    resp.type = RESP_READY;
  } else if (strcmp((char *)raw_data, "\r\n") == 0) {
    return;
  } else {
    resp.type = RESP_ETC;

    // 분리하여 작업 strtok
    // 쉼표와 공백으로 분리 해보기 -> 쉼표 공백, 아무거나 있어도 구분 됨
    char *token = strtok((char *)raw_data, ", ");
    if (token) {
      // 초반 분류 tag 없으니, 바로 param 사용하기
      strcpy(resp.params[resp.param_count], token);
      // resp.params 출력
      //      printf("resp.params[%d] : %s \r\n", resp.param_count,
      //             resp.params[resp.param_count]);
      resp.param_count++;
    }
    // while ((token == strtok(NULL, ", ")) != NULL && resp.param_count < 5)
    while (resp.param_count < 5) {

      if ((token = strtok(NULL, ", ")) != NULL) {
        strcpy(resp.params[resp.param_count], token);
        // resp.params 출력 -> 정상동작확인
        //        printf("resp.params[%d] : %s \r\n", resp.param_count,
        //               resp.params[resp.param_count]);
      }
      resp.param_count++;
    }
  }

  return resp;
}

// [목적] : esp8266사용을 위한 변수 초기화
void init_esp8266(void) {
  last_resp.type = RESP_UNKNOWN;
  // last_resp.params =
  last_resp.param_count = 0;
  esp8266_step=ESP8266_READY_SEND;
}

// [목적] : esp8266_step에 따른 switch문 동작.
// [반환] :  T-정해진 case로 동작할 때, F-정의해주지 않은 case(defalt)로 동작할
// 때.
bool esp_8266_control(void) {

  switch (esp8266_step) {
  case ESP8266_READY_SEND:
    send_Serial("[ESP8266] START \r\n");
    send_AT_CMD("AT\r\n");
    esp8266_step = ESP8266_READY_WAIT;
    tickstart_esp = HAL_GetTick();
    return true;
    break;
  case ESP8266_READY_WAIT:
    if (HAL_GetTick() - tickstart_esp > RECV_TIMEOUT) {
      esp8266_step = ESP8266_READY_SEND;
    } else {
      // 원하는 상황에서는 'RESP_OK' 까지만 응답 오고 끝나야 함.
      // 여기서 wait해서 기다릴 필요 있는지 확인하기(예제 코드 참고)
      if (last_resp.type == RESP_OK) {
        send_Serial("[ESP8266] READY : OK \r\n");
        esp8266_step = ESP8266_CWMODE_SEND;
      }
    }
    return true;
    break;
  case ESP8266_CWMODE_SEND:
    send_AT_CMD("AT+CWMODE=2\r\n");
    esp8266_step = ESP8266_CWMODE_WAIT;
    tickstart_esp = HAL_GetTick();
    return true;
    break;
  case ESP8266_CWMODE_WAIT:
    if (HAL_GetTick() - tickstart_esp > RECV_TIMEOUT) {
      esp8266_step = ESP8266_READY_SEND;
    } else {
      if (last_resp.type == RESP_OK) {
        send_Serial("[ESP8266] AT+CWMODE=2 : OK \r\n");
        esp8266_step = ESP8266_CIPMUX_SEND;
      }
    }
    return true;
    break;
  case ESP8266_CIPMUX_SEND:
    send_AT_CMD("AT+CIPMUX=1\r\n");
    esp8266_step = ESP8266_CIPMUX_WAIT;
    tickstart_esp = HAL_GetTick();
    return true;
    break;
  case ESP8266_CIPMUX_WAIT:
    if (HAL_GetTick() - tickstart_esp > RECV_TIMEOUT) {
      esp8266_step = ESP8266_READY_SEND;
    } else {
      if (last_resp.type == RESP_OK) {
        send_Serial("[ESP8266] AT+CIPMUX=1 : OK \r\n");
        esp8266_step = ESP8266_CIPSERVER_SEND;
      } else if (strstr(recv_buf, "link")) {
        RST_RECV_BUF;
        send_Serial("[ESP8266] AT+CIPMUX=1 : OK \r\n");
        esp8266_step = ESP8266_CIPSERVER_SEND;
      }
    }
    return true;
    break;
  case ESP8266_CIPSERVER_SEND:
    send_AT_CMD("AT+CIPSERVER=1\r\n");
    esp8266_step = ESP8266_CIPSERVER_WAIT;
    tickstart_esp = HAL_GetTick();
    return true;
    break;
  case ESP8266_CIPSERVER_WAIT:
    if (HAL_GetTick() - tickstart_esp > RECV_TIMEOUT) {
      esp8266_step = ESP8266_READY_SEND;
    } else {
      if (last_resp.type == RESP_OK) {
        send_Serial("[ESP8266] AT+CIPSERVER=1 : OK \r\n");
        esp8266_step = ESP8266_DEVICE_WAIT;

        // 다음단계 WAIT이므로 동작 추가
        tickstart_esp = HAL_GetTick();
      }
    }
    return true;
    break;
  case ESP8266_DEVICE_WAIT:
    if (HAL_GetTick() - tickstart_esp > RECV_TIMEOUT) {
      send_Serial("[ESP8266] DEVICE_WAIT \r\n");
      tickstart_esp = HAL_GetTick();
    } else {
      if (last_resp.type == RESP_ETC) {
        if (strcmp((char *)last_resp.params[1], "CONNECT\r\n") == 0) {
          send_Serial("[ESP8266] DEVICE_WAIT : OK \r\n");
          esp8266_step = ESP8266_D_CIPSEND_SEND;

          // 다음단계 WAIT이므로 동작 추가
          tickstart_esp = HAL_GetTick();
        }
      }
    }
    return true;
    break;
  case ESP8266_D_CIPSEND_SEND:
    send_AT_CMD("AT+CIPSEND=0,239\r\n");
    esp8266_step = ESP8266_D_CIPSEND_WAIT;
    tickstart_esp = HAL_GetTick();
    return true;
    break;
  case ESP8266_D_CIPSEND_WAIT:
    if (HAL_GetTick() - tickstart_esp > RECV_TIMEOUT) {
      esp8266_step = ESP8266_READY_SEND;
    } else {
      if (last_resp.type == RESP_OK) {
        // 0,239는 현재 상황에서만 적용되는 값
        send_Serial("[ESP8266] AT+CIPSEND=0,239 : OK \r\n");
        esp8266_step = ESP8266_HTML_SEND;

        // 다음단계 WAIT이므로 동작 추가
        tickstart_esp = HAL_GetTick();
      }
    }
    return true;
    break;
  case ESP8266_HTML_SEND:
    send_AT_CMD(AT_HTML);
    esp8266_step = ESP8266_HTML_WAIT;
    tickstart_esp = HAL_GetTick();
    return true;
    break;
  case ESP8266_HTML_WAIT:
    if (HAL_GetTick() - tickstart_esp > RECV_TIMEOUT) {
      esp8266_step = ESP8266_READY_SEND;
    } else {
      if (last_resp.type == RESP_ETC) {
        if (strcmp((char *)last_resp.params[0], "Recv") == 0) {
          send_Serial("[ESP8266] ESP8266_HTML_WAIT : OK \r\n");
          esp8266_step = ESP8266_BUTTON_WAIT;

          // 다음단계 WAIT이므로 동작 추가
          tickstart_esp = HAL_GetTick();
        }
      }
    }
    return true;
    break;
  case ESP8266_BUTTON_WAIT:
    if (HAL_GetTick() - tickstart_esp > RECV_TIMEOUT) {
      send_Serial("[ESP8266] BUTTON_WAIT \r\n");
      tickstart_esp = HAL_GetTick();
    } else {

      if (strstr(recv_buf, "/led/on")) {
        RST_RECV_BUF;
        LED_ON;
        send_Serial("[ESP8266] LED is ON \r\n");
      } else if (strstr(recv_buf, "/led/off")) {
        RST_RECV_BUF;
        LED_OFF;
        send_Serial("[ESP8266] LED is OFF \r\n");
      }
    }
    return true;
    break;

  default:
    send_Serial("[ESP8266] State Error \r\n");
    return false;
    break;
  }
}

// [목적] : last_resp 파싱 여부 디버깅 용도
void test_task(void) {
  // 1st
  send_AT_CMD("AT\r\n");

  recv_data_task();

  if (last_resp.type == RESP_OK) {
    printf("RESP_OK  \r\n");
  } else if (last_resp.type == RESP_ETC) {
    printf("RESP_ETC  \r\n");
  }

  printf("check last_resp [1] \r\n");
}

// [목적] : esp8266, client 와의 연결 상태 확인하여 T/F 반환
// T - if문 이외의 상황 , F - 문자열 내에서 "ready", "CLOSE" 확인
bool check_connection(void) {
  // "CLOSED" 계속 떠서 비교할 필요 X
  // (strstr(recv_buf, "CLOSED") != NULL)
  if ((strstr(recv_buf, "ready") != NULL)) {
    RST_RECV_BUF;
    esp8266_step = ESP8266_READY_SEND;
    printf("[ESP8266] ESP8266_READY_SEND \r\n");

    return false;
  }
  return true;
}