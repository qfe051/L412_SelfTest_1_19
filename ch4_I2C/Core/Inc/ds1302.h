#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "main.h"

#ifndef __DS1302_H_
#define __DS1302_H_
// 함수 선언만!


int _write(int file, char *ptr, int len);
void DS1302_SetOutput(void);
void DS1302_SetInput(void);
void DS1302_WriteByte(uint8_t data);
uint8_t DS1302_ReadByte(void);
void DS1302_WriteReg(uint8_t reg, uint8_t data);
uint8_t DS1302_ReadReg(uint8_t reg);

// BCD -> DEC 함수 하나로!
uint8_t bcd_2_dec(uint8_t data);
const char *days(uint8_t data);

// 핀에 대한 define 정의
// GPIOC에 그대로 RST_Pin이름 쓰면 사용 가능 
#define CE_ON HAL_GPIO_WritePin(GPIOC, RST_Pin, GPIO_PIN_SET);
#define CE_OFF HAL_GPIO_WritePin(GPIOC, RST_Pin, GPIO_PIN_RESET);

#define CLK_ON HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_SET);
#define CLK_OFF HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_RESET);

#define GPIO_Init_C HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

#define Enable_Write DS1302_WriteReg(0x8E, 0x00);
#define Enable_Clock DS1302_WriteReg(0x80, 0x00);

#define Set_24H DS1302_WriteReg(0x84, 0x00);

// 26년 1월 28일 17시 00분 수요일(3)
// min, hours, date, month, days,years
#define TimeSet_v1700                                                          \
  DS1302_WriteReg(0x82, 0x00);                                                 \
  DS1302_WriteReg(0x84, 0x17);                                                 \
  DS1302_WriteReg(0x86, 0x28);                                                 \
  DS1302_WriteReg(0x88, 0x01);                                                 \
  DS1302_WriteReg(0x8A, 0x03);                                                 \
  DS1302_WriteReg(0x8C, 0x26);

typedef struct {
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t Date;
  uint8_t Month;
  uint8_t Day;
  uint8_t Years;
} rtc_time;

void burst_mode(rtc_time *r);
void burst_mode_printf(rtc_time *r);
// main.c로 이동
// ㄴ
#endif