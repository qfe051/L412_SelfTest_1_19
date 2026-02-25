#include "tm1637.h"
#include "stdbool.h"
#include <stdint.h>

static void is_DIO_Output(bool is_DIO);
static uint8_t cal_digit(uint8_t input);
static void write_byte_TM1637(uint8_t input);
static void set_Start_TM1637(void);
static void set_Stop_TM1637(void);

// input output
void is_DIO_Output(bool is_DIO) {
  if (is_DIO) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  else {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
}

void write_byte_TM1637(uint8_t input) {
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, DIO_Pin, GPIO_PIN_RESET);

  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_RESET);
  // DIO 핀 먼저
  for (int i=0; i<8; i++) {
    if (input & 0x01) {
      HAL_Delay(1);
      HAL_GPIO_WritePin(GPIOC, DIO_Pin, GPIO_PIN_SET);
    }
    else {
      HAL_Delay(1);
      HAL_GPIO_WritePin(GPIOC, DIO_Pin, GPIO_PIN_RESET);
    }
    input = input >> 1;
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_RESET);
  }
  
}

void set_Start_TM1637(void) {
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, DIO_Pin, GPIO_PIN_SET);

  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, DIO_Pin, GPIO_PIN_RESET);

  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_RESET);
}

void set_Stop_TM1637(void) {
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, DIO_Pin, GPIO_PIN_RESET);

  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_RESET);

  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, DIO_Pin, GPIO_PIN_SET);
}

void ack_TM1637(void) {
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, DIO_Pin, GPIO_PIN_RESET);

  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_RESET);

  HAL_Delay(1);
  is_DIO_Output(false);

  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_RESET);

  HAL_Delay(1);
  is_DIO_Output(true);


}

void test_TM1637(void) {
  set_Start_TM1637();
  write_byte_TM1637(0x40);
  ack_TM1637();
  set_Stop_TM1637();

  set_Start_TM1637();
  write_byte_TM1637(0xC0);
  ack_TM1637();

  write_byte_TM1637(0xFF);
  ack_TM1637();
  write_byte_TM1637(0xFF);
  ack_TM1637();
  write_byte_TM1637(0xFF);
  ack_TM1637();
  write_byte_TM1637(0xFF);
  ack_TM1637();

  set_Start_TM1637();
  write_byte_TM1637(0x8F);
  ack_TM1637();
  set_Stop_TM1637();
}

void show_TM1637(uint8_t data_1,uint8_t data_2,uint8_t data_3,uint8_t data_4) {
  set_Start_TM1637();
  write_byte_TM1637(0x40);
  ack_TM1637();
  set_Stop_TM1637();

  set_Start_TM1637();
  write_byte_TM1637(0xC0);
  ack_TM1637();

  write_byte_TM1637(cal_digit(data_1));
  ack_TM1637();
  write_byte_TM1637(cal_digit(data_2));
  ack_TM1637();
  write_byte_TM1637(cal_digit(data_3));
  ack_TM1637();
  write_byte_TM1637(cal_digit(data_4));
  ack_TM1637();

  set_Start_TM1637();
  write_byte_TM1637(0x8F);
  ack_TM1637();
  set_Stop_TM1637();
}

uint8_t cal_digit(uint8_t input) {

  uint8_t hex_data = 0;
  
  switch (input) {
  case 0:
    hex_data = 0x3f;
    break;
  case 1:
    hex_data = 0x06;
    break;
  case 2:
    hex_data = 0x5b;
    break;
  case 3:
    hex_data = 0x4f;
    break;
  case 4:
    hex_data = 0x66;
    break;
  case 5:
    hex_data = 0x6d;
    break;
  case 6:
    hex_data = 0x7d;
    break;
  case 7:
    hex_data = 0x07;
    break;
  case 8:
    hex_data = 0x7f;
    break;
  case 9:
    hex_data = 0x6f;
    break;
  default:
    hex_data = 0x00;
    break;

  }

  return hex_data;
}