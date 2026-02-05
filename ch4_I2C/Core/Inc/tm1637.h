#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "main.h"

#ifndef __TM1637_H_
#define __TM1637_H_

#define CLK_DIS_ON HAL_GPIO_WritePin(GPIOA, CLK_DIS_Pin, GPIO_PIN_SET);
#define CLK_DIS_OFF HAL_GPIO_WritePin(GPIOA, CLK_DIS_Pin, GPIO_PIN_RESET);

#define DAT_DIS_ON HAL_GPIO_WritePin(GPIOA, DAT_DIS_Pin, GPIO_PIN_SET);
#define DAT_DIS_OFF HAL_GPIO_WritePin(GPIOA, DAT_DIS_Pin, GPIO_PIN_RESET);

#define GPIO_Init_A HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

void TM1637_SetOutput(void);
void TM1637_SetInput(void);

void TM1637_WriteByte(uint8_t data);
uint8_t TM1637_ReadByte(void);

void TM1637_WriteReg(uint8_t reg, uint8_t data);
uint8_t TM1637_ReadReg(uint8_t reg);

void TM1637_Start(void);
void TM1637_Stop(void);

void TM1637_Send_cmd(uint8_t data);

#endif