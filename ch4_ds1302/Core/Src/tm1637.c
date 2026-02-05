#include "main.h"
#include "tm1637.h"
#include <stdint.h>

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

  for (int i = 0; i < 8; i++) {
    if ((data & 0x01) == 1) {
      HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_SET);
    } else {
      HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);
    }
    data = data >> 1;

    HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_RESET);
  }
  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);


  // ACK 동작
  set_input_tm1637();
  HAL_GPIO_ReadPin(GPIOC, DS_DAT_Pin);
  
  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_RESET);

  set_output_tm1637();
}

void set_start(void) {
  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);
}

void set_stop(void) {
  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, TM_CLK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, TM_DAT_Pin, GPIO_PIN_SET);
}

void send_cmd(uint8_t data) {
  set_start();

  write_byte_tm1637(data);

  set_stop();
}