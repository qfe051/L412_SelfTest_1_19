#include "main.h"
#include <stdint.h>


#ifndef __TM1637_H
#define __TM1637_H

// void set_output_tm1637(void);
// void set_input_tm1637(void);

// test 이후 static으로 넘기기

enum BLINK_DOT_LED {
  DOT_LED_OFF = 0,
  DOT_LED_ON
};

// // 화면 출력용 데이터
// typedef struct {
//   uint8_t data_1;
//   uint8_t data_2;
// } _Display_data;

void write_byte_tm1637(uint8_t data);

void set_start(void);
void set_stop(void);

void send_cmd(uint8_t data);
void send_cmd_2(uint8_t data);

void test_write_tm1637(void);

// ds1302에서 사용
void show_tm1637(_Display_data *r,uint8_t *led_blink,uint8_t mode);


#endif
