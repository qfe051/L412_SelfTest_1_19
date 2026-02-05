#include "tm1637.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stm32l412xx.h>

void set_output_tm1637(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = TM_DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
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
  HAL_GPIO_WritePin(GPIOA, TM_DAT_Pin, GPIO_PIN_RESET);

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

// void delay_us(uint8_t time_us) {
//   __HAL_TIM_SET_COUNTER(&htim2, 0);
//   while (__HAL_TIM_GET_COUNTER(&htim2)<time_us) {
//   }
// }