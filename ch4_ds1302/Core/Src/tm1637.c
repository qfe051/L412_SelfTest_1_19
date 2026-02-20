#include "tm1637.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <stm32l412xx.h>

// static 함수 정의
static uint8_t cal_segment(uint8_t input);


void TM1637_IO_Set_Output(bool isOutput) {
  if (isOutput == true) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = TM_DAT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  }
  else {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = TM_DAT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  }
}
//test용 함수
void test_write_tm1637(void) {
  printf("TM1637 test \r\n");
  // 명령어 1개 세트
  send_cmd(0x40);
  send_cmd_2(0xC0);

  write_byte_tm1637(0x01);
  write_byte_tm1637(0x02);
  write_byte_tm1637(0x04);
  write_byte_tm1637(0x08);

  send_cmd(0x88);
  // 명령어 1개 세트
  send_cmd(0x40);
  send_cmd_2(0xC0);

  write_byte_tm1637(0x3F);
  write_byte_tm1637(0x06);
  write_byte_tm1637(0x5B);
  write_byte_tm1637(0x4F);
  ////////////////////////////
  send_cmd(0x87);
}

// data_1,2에 값을 업데이트 해주기 
void show_tm1637(uint8_t data_1, uint8_t data_2) {
  uint8_t d1, d2, d3, d4 = 0;

  d1 = data_1 / 10;
  d2 = data_1 % 10;

  d3 = data_2 / 10;
  d4 = data_2 % 10;

  // 1. set data
  send_cmd(0x40);
  // 2. set address
  send_cmd_2(0xC0);
  // send data
  write_byte_tm1637(cal_segment(d1));
  write_byte_tm1637(cal_segment(d2));
  write_byte_tm1637(cal_segment(d3));
  write_byte_tm1637(cal_segment(d4));
  //
  set_stop();

  // // 3. control display
  // send_cmd(0x87);

}

void set_output_tm1637(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = TM_DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
}

void set_input_tm1637(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = TM_DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
}

void write_byte_tm1637(uint8_t data) {
  set_output_tm1637();
  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_CLK_Pin, GPIO_PIN_RESET);
  for (int i = 0; i < 8; i++) {
    if ((data & 0x01) == 1) {
      delay_5us();
      HAL_GPIO_WritePin(GPIOA, TM_DAT_Pin, GPIO_PIN_SET);
    } else {
      delay_5us();
      HAL_GPIO_WritePin(GPIOA, TM_DAT_Pin, GPIO_PIN_RESET);
    }
    data = data >> 1;

    delay_5us();
    HAL_GPIO_WritePin(GPIOA, TM_CLK_Pin, GPIO_PIN_SET);
    delay_5us();
    HAL_GPIO_WritePin(GPIOA, TM_CLK_Pin, GPIO_PIN_RESET);
  }
  delay_5us();
  // HAL_GPIO_WritePin(GPIOA, TM_DAT_Pin, GPIO_PIN_RESET);

  // ACK 동작
  set_input_tm1637();
  HAL_GPIO_ReadPin(GPIOA, DS_DAT_Pin);

  HAL_GPIO_WritePin(GPIOA, TM_CLK_Pin, GPIO_PIN_SET);
  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_CLK_Pin, GPIO_PIN_RESET);

  set_output_tm1637();
}

void set_start(void) {
  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_DAT_Pin, GPIO_PIN_SET);

  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_CLK_Pin, GPIO_PIN_SET);

  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_DAT_Pin, GPIO_PIN_RESET);
}

void set_stop(void) {
  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_CLK_Pin, GPIO_PIN_RESET);

  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_DAT_Pin, GPIO_PIN_RESET);

  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_CLK_Pin, GPIO_PIN_SET);

  delay_5us();
  HAL_GPIO_WritePin(GPIOA, TM_DAT_Pin, GPIO_PIN_SET);
}

void send_cmd(uint8_t data) {
  set_start();

  write_byte_tm1637(data);

  set_stop();
}

void send_cmd_2(uint8_t data) {
  set_start();

  write_byte_tm1637(data);
}

uint8_t cal_segment(uint8_t input) {
  uint8_t seg_data = 0;

  switch (input) {
  case 0:
    seg_data = 0x3f;
    break;
  case 1:
    seg_data = 0x06;
    break;
  case 2:
    seg_data = 0x5b;
    break;
  case 3:
    seg_data = 0x4f;
    break;
  case 4:
    seg_data = 0x66;
    break;
  case 5:
    seg_data = 0x6d;
    break;
  case 6:
    seg_data = 0x7d;
    break;
  case 7:
    seg_data = 0x07;
    break;
  case 8:
    seg_data = 0x7f;
    break;
  case 9:
    seg_data = 0x6f;
    break;
  }

  return seg_data;
}

// void delay_us(uint8_t time_us) {
//   __HAL_TIM_SET_COUNTER(&htim2, 0);
//   while (__HAL_TIM_GET_COUNTER(&htim2)<time_us) {
//   }
// }