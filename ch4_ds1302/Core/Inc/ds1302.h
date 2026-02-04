#include <stdint.h>

#ifndef __DS1302_H
#define __DS1302_H

// 함수 선언
void set_output_ds1302();
void set_input_ds1302();

void write_byte_ds1302(uint8_t data);
uint8_t read_byte_ds1302(void);

void write_reg_ds1302(uint8_t address, uint8_t data);
uint8_t read_reg_ds1302(uint8_t address);

uint8_t bcd_2_dec(uint8_t data);
uint8_t bcd_2_dec_hour(uint8_t data);
uint8_t bcd_2_dec_sec(uint8_t data);



void enable_clock(void);
void disable_clock(void);

void enable_24H(void);
void disable_24H(void);

void set_AM(void);
void set_PM(void);

// const char *day_num_2_str(uint8_t data);
void day_print(uint8_t data);
void print_AM_PM(uint8_t data);

// 구조체 선언, day - 숫자로 값 받고 -> 출력할 때 문자 변환 함수 
typedef struct{
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t date;
  uint8_t month;
  uint8_t day;
  uint8_t year;
} s_rtc_time;

typedef union {
  uint64_t full_data;
  uint8_t idx_data[7];
} u_rtc_time;


// 구조체 관련 함수
void s_burst_mode_read(s_rtc_time *r);
void u_burst_mode_read(u_rtc_time *r);
void s_burst_mode_print(s_rtc_time *r);
void u_burst_mode_print(u_rtc_time *r);

#endif