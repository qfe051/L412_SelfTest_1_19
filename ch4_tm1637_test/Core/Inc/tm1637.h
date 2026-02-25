#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <stm32l412xx.h>

#ifndef _TM1637_H_
#define _TM1637_H_

uint8_t dec_arr[10] = {0,1,2,3,4,5,6,7,8,9};

void test_TM1637(void);
void show_TM1637(uint8_t data_1,uint8_t data_2,uint8_t data_3,uint8_t data_4);

#endif