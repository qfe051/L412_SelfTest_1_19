#include "tm1637.h"
#include <stdint.h>

//delay 추가 
void delay_us(uint8_t data) {
  for (int i = 0; i < data; i++) {
    HAL_GPIO_WritePin(GPIOC, DELAY_US_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, DELAY_US_Pin, GPIO_PIN_SET);
  }
}

// 함수 선언
// 1. DAT (I/O) 변경해주기
// 출력설정 함수
void TM1637_SetOutput(void) {
  // {0} 집어넣는 작업 하는 이유는?
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DAT_DIS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_Init_A;
}

// 입력 설정 함수
void TM1637_SetInput(void) {
  // {0} 집어넣는 작업 하는 이유는?
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DAT_DIS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_Init_A;
}

// 2. 1바이트 쓰기 / 읽기 단위로 진행

// data는 0,1 뿐이라서
// MCU -> DS1302 로 write
// DS1302에서 반환하는 값이 없으니, 'void' 형식으로 진행
void TM1637_WriteByte(uint8_t data) {
  // 출력모드 세팅
  TM1637_SetOutput();
  delay_us(5);

  for (int i = 0; i < 8; i++) {

    // LSB (0번비트 부터!)
    // 가장 오른쪽 비트와 비교
    // 1 -> 0000 0001 과 & 연산하여 1인지 확인
    if ((data & 1) == 1) {
      // data 가 1인 경우에만 set으로 설정 ?
      HAL_GPIO_WritePin(GPIOA, DAT_DIS_Pin, GPIO_PIN_SET);
    } else {
      HAL_GPIO_WritePin(GPIOA, DAT_DIS_Pin, GPIO_PIN_RESET);
    }
    data >>= 1; // 비트 밀어넣기 (시프트)
    CLK_DIS_ON;
    CLK_DIS_OFF;
  }
  delay_us(5);
  //9번째 pulse
  TM1637_SetInput();
  CLK_DIS_ON;
  CLK_DIS_OFF;
}
// uint8_t -> 8bit data를 받아와야 함
// void -> 입력값 없이 그냥 읽기만 해도 됨
uint8_t TM1637_ReadByte(void) {
  // data 받아오기 전 초기화
  uint8_t data = 0;
  TM1637_SetInput();

  for (int i = 0; i < 8; i++) {
    CLK_DIS_ON;

    // LSB 부터 데이터 읽기
    // 값이 1인 데이터 읽어오기
    // GPIO_PIN_SET 대신 1 집어넣는 것이 안정적인지 확인
    if (HAL_GPIO_ReadPin(GPIOA, DAT_DIS_Pin) == GPIO_PIN_SET) {
      data |= (1 << i);
    }
    // CLK 핀 토글 생성
    // 토글 안되면 다른 방법도 보기
    // HAL_GPIO_TogglePin(GPIOC, CLK_Pin);

    // set reset
    CLK_DIS_OFF;
  }
  return data;
}

// 3. 레지스터 단위 R/W
// 여기서 필요없을듯?
// 마찬가지로 쓰기 작업이라 - return 받을 데이터 없음
void TM1637_WriteReg(uint8_t reg, uint8_t data) {
  // CE - High로 올려 통신 시작
  TM1637_WriteByte(reg);  // 주소 -> 시작 시점
  TM1637_WriteByte(data); // 데이터
  // CE - Low로 통신 종료
}

// reg 삽입할 공간만 추가
uint8_t TM1637_ReadReg(uint8_t reg) {
  uint8_t data;
  TM1637_WriteByte(reg);    // 주소 -> 시작 시점 전송 먼저 해야!
  data = TM1637_ReadByte(); // 데이터 저장(수신?)
  // CE - Low로 통신 종료

  return data;
}

void TM1637_Start(void) {
  DAT_DIS_ON;
  CLK_DIS_ON;
  DAT_DIS_OFF;
}

void TM1637_Stop(void) {
  DAT_DIS_OFF;
  CLK_DIS_ON;
  DAT_DIS_ON;
}