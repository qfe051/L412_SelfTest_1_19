#include <stdint.h>
#include "ds1302.h"

// // #ifndef 
// // 함수 원형

// typedef struct {
//   uint8_t sec;
//   uint8_t min;
//   uint8_t hour;
//   uint8_t Date;
//   uint8_t Month;
//   uint8_t Day;
//   uint8_t Years;
// } rtc_time;

// 시간 ,날짜 출력 변환
uint8_t bcd_2_dec(uint8_t data) {
  uint8_t dec_1 = 0;
  uint8_t dec_10 = 0;
  uint8_t dec = 0;

  dec_1 = (data & 0x0F);
  dec_10 = ((data >> 4) & 0x0F);
  dec = 10 * dec_10 + dec_1;

  return dec;
}
const char *days(uint8_t data) {
  int temp = 0;
  temp = data++;

  switch (temp) {
  case 1:
    return "Mon";
  case 2:
    return "Tue";
  case 3:
    return "Wed";
  case 4:
    return "Thu";
  case 5:
    return "Fri";
  case 6:
    return "Sat";
  case 7:
    return "Sun";
  }

  return days;
}

// 구조체 선언
// rtc_time now;

// printf 하는 함수와 분리하기
void burst_mode(rtc_time *r) {
  //   rtc_time now;
  CE_ON;

  DS1302_WriteByte(0xBF);

  r->sec = bcd_2_dec(DS1302_ReadByte());
  r->min = bcd_2_dec(DS1302_ReadByte());
  r->hour = bcd_2_dec(DS1302_ReadByte());
  r->Date = bcd_2_dec(DS1302_ReadByte());
  r->Month = bcd_2_dec(DS1302_ReadByte());
  r->Day = days(DS1302_ReadByte());
  r->Years = bcd_2_dec(DS1302_ReadByte());
  CE_OFF;

//   printf("[] Read Sec: %d \r\n", now.sec);
//   printf("[] Read Min: %d \r\n", now.min);
//   printf("[] Read Hours: %d \r\n", now.hour);
//   printf("[] Read Dates: %d \r\n", now.Date);
//   printf("[] Read Month: %d \r\n ", now.Month);
//   printf("[] Read Day: %s \r\n", now.Day);
//   printf("[] Read Years: %d \r\n", now.Years);
};

void burst_mode_printf(rtc_time *r) {
  printf("[] Read Sec: %d \r\n", r->sec);
  printf("[] Read Min: %d \r\n", r->min);
  printf("[] Read Hours: %d \r\n", r->hour);
  printf("[] Read Dates: %d \r\n", r->Date);
  printf("[] Read Month: %d \r\n ", r->Month);
  printf("[] Read Day: %s \r\n", r->Day);
  printf("[] Read Years: %d \r\n", r->Years);
}

// 함수 선언
// 1. DAT (I/O) 변경해주기
// 출력설정 함수
void DS1302_SetOutput(void) {
  // {0} 집어넣는 작업 하는 이유는?
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_Init_C;
}

// 입력 설정 함수
void DS1302_SetInput(void) {
  // {0} 집어넣는 작업 하는 이유는?
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_Init_C;
}

// 2. 1바이트 쓰기 / 읽기 단위로 진행

// data는 0,1 뿐이라서
// MCU -> DS1302 로 write
// DS1302에서 반환하는 값이 없으니, 'void' 형식으로 진행
void DS1302_WriteByte(uint8_t data) {
  // 출력모드 세팅
  DS1302_SetOutput();

  for (int i = 0; i < 8; i++) {

    // LSB (0번비트 부터!)
    // 가장 오른쪽 비트와 비교
    // 1 -> 0000 0001 과 & 연산하여 1인지 확인
    if ((data & 1) == 1) {
      // data 가 1인 경우에만 set으로 설정 ?
      HAL_GPIO_WritePin(GPIOC, DAT_Pin, GPIO_PIN_SET);
    } else {
      HAL_GPIO_WritePin(GPIOC, DAT_Pin, GPIO_PIN_RESET);
    }
    data >>= 1; // 비트 밀어넣기 (시프트)
    CLK_ON;
    CLK_OFF;
  }
}
// uint8_t -> 8bit data를 받아와야 함
// void -> 입력값 없이 그냥 읽기만 해도 됨
uint8_t DS1302_ReadByte(void) {
  // data 받아오기 전 초기화
  uint8_t data = 0;
  DS1302_SetInput();

  for (int i = 0; i < 8; i++) {
    CLK_ON;

    // LSB 부터 데이터 읽기
    // 값이 1인 데이터 읽어오기
    // GPIO_PIN_SET 대신 1 집어넣는 것이 안정적인지 확인
    if (HAL_GPIO_ReadPin(GPIOC, DAT_Pin) == GPIO_PIN_SET) {
      data |= (1 << i);
    }
    // CLK 핀 토글 생성
    // 토글 안되면 다른 방법도 보기
    // HAL_GPIO_TogglePin(GPIOC, CLK_Pin);

    // set reset
    CLK_OFF;
  }
  return data;
}

// 3. 레지스터 단위 R/W
// 마찬가지로 쓰기 작업이라 - return 받을 데이터 없음
void DS1302_WriteReg(uint8_t reg, uint8_t data) {
  // CE - High로 올려 통신 시작
  CE_ON;
  DS1302_WriteByte(reg);  // 주소 -> 시작 시점
  DS1302_WriteByte(data); // 데이터
  // CE - Low로 통신 종료
  CE_OFF;
}

// reg 삽입할 공간만 추가
uint8_t DS1302_ReadReg(uint8_t reg) {
  uint8_t data;
  CE_ON;
  DS1302_WriteByte(reg);    // 주소 -> 시작 시점 전송 먼저 해야!
  data = DS1302_ReadByte(); // 데이터 저장(수신?)
  // CE - Low로 통신 종료
  CE_OFF;

  return data;
}

// // #endif

// /* USER CODE BEGIN 0 */

// // // UART로 출력 보내기 위한 함수
// // int _write(int file, char *ptr, int len) {
// //   if (file == STDOUT_FILENO) {
// //     HAL_UART_Transmit(&hlpuart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
// //     return len;
// //   }
// //   return -1;
// // }
