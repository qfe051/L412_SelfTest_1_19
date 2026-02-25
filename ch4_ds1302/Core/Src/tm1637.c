#include "tm1637.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <stm32l412xx.h>

uint8_t ADD_DOT_DISPLAY = 0x00;

// static 함수 정의
static uint8_t cal_segment(uint8_t input);
static void ack_tm1637(void);

void TM1637_IO_Set_Output(bool isOutput) {
  if (isOutput == true) {
	  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = TM_DAT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  else {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = TM_DAT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
}


//test용 함수
void test_write_tm1637(void) {
  printf("TM1637 test \r\n");
  // 명령어 1개 세트

  set_start();
  write_byte_tm1637(0x40);
  ack_tm1637();
  set_stop();

  set_start();
  write_byte_tm1637(0xC0);
  ack_tm1637();


  write_byte_tm1637(0xff);
  ack_tm1637();
  write_byte_tm1637(0xff);
  ack_tm1637();
  write_byte_tm1637(0xff);
  ack_tm1637();
  write_byte_tm1637(0xff);
  ack_tm1637();

  set_start();
  write_byte_tm1637(0x8f);
  ack_tm1637();
  set_stop();
  
  
}

// data_1,2에 값을 업데이트 해주기 
void show_tm1637(_Display_data *r,uint8_t *led_blink,uint8_t mode) {
  uint8_t d1, d2, d3, d4 = 0;

  d1 = r->data_1 / 10;
  d2 = r->data_1 % 10;

  d3 = r->data_2 / 10;
  d4 = r->data_2 % 10;

  

  if ((*led_blink) == DOT_LED_ON) {
    ADD_DOT_DISPLAY = 0x80;
    if (mode <2) {
      ADD_DOT_DISPLAY = 0x00;
    }
  }
  else {
    ADD_DOT_DISPLAY = 0x00;
  }

  // cmd1
  set_start();
  write_byte_tm1637(0x40);
  ack_tm1637();
  set_stop();

  // cmd2
  set_start();
  write_byte_tm1637(0xC0);
  ack_tm1637();

  // data
  write_byte_tm1637(cal_segment(d1));
  ack_tm1637();
  // ADD_DOT_DISPLAY 여기서만 조정 가능
  write_byte_tm1637(cal_segment(d2)+ADD_DOT_DISPLAY);
  ack_tm1637();
  write_byte_tm1637(cal_segment(d3));
  ack_tm1637();
  write_byte_tm1637(cal_segment(d4));
  ack_tm1637();

  // cmd3
  set_start();
  write_byte_tm1637(0x8f);
  ack_tm1637();
  set_stop();

  (*led_blink)++;
  *led_blink = *led_blink % 2;

  printf("state of led_blink : %d \r\n",*led_blink);

}






void write_byte_tm1637(uint8_t data) {
  TM1637_IO_Set_Output(true);
  delay_5us();
  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_RESET);
  delay_5us();
  for (int i = 0; i < 8; i++) {
    if ((data & 0x01) == 1) {
      HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_SET);
      delay_5us();
    } else {
      HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);
      delay_5us();
    }
    data = data >> 1;

    HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_SET);
    delay_5us();
    HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_RESET);
    delay_5us();
  }
}

void ack_tm1637(void) {
  TM1637_IO_Set_Output(false);
  delay_5us();

  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_SET);
  delay_5us();

  HAL_GPIO_ReadPin(GPIOC, TM_DAT_Pin);
  delay_5us();

  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_RESET);
  TM1637_IO_Set_Output(true);
  delay_5us();
  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);
  delay_5us();
}

void set_start(void) {
	  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_SET);
	  delay_5us();

  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_SET);
  delay_5us();

  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);
  delay_5us();
}

void set_stop(void) {
  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_RESET);
  delay_5us();

  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);
  delay_5us();

  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_SET);
  delay_5us();

  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_SET);
  delay_5us();
}

// void send_cmd(uint8_t data) {
//   set_start();

//   write_byte_tm1637(data);
//   ack_tm1637();

//   set_stop();
// }

// void send_cmd_2(uint8_t data) {
//   set_start();

//   write_byte_tm1637(data);
//   ack_tm1637();
// }

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
  default:
    seg_data = 0xff;
    break;
  }

  return seg_data;
}

// void delay_us(uint8_t time_us) {
//   __HAL_TIM_SET_COUNTER(&htim2, 0);
//   while (__HAL_TIM_GET_COUNTER(&htim2)<time_us) {
//   }
// }
