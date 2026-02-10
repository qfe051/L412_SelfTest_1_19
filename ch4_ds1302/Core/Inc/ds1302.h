#include <stdint.h>
#include<stdbool.h>

#ifndef __DS1302_H
#define __DS1302_H

// bool 함수 사용법
bool DS1302_Init(void);
bool DS1302_Set_Sec(uint8_t sec);
bool DS1302_Set_Min(uint8_t set_min);
bool DS1302_Set_Hour_case_24(uint8_t set_hour);
// 2개 함수 합치기
// DS1302_Set_Hour_case_12 , DS1302_Set_AM_PM
bool DS1302_Set_Hour_case_12(uint8_t set_hour, uint8_t set_AM_PM);
bool DS1302_Set_AM_PM(uint8_t set_AM_PM);
bool DS1302_Set_Hour_12h(void);
bool DS1302_Get_Sec(uint8_t *pSec);
bool DS1302_Get_Time1_12_24();



// 함수 선언
void set_output_ds1302();
void set_input_ds1302();

void write_byte_ds1302(uint8_t data);
uint8_t read_byte_ds1302(void);

// 디버깅용으로 필요 
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


// bitfield 기반 구조체 정의

typedef union  {
  uint8_t raw;
  struct sec_bit {
    uint8_t sec_1 : 4;
    uint8_t sec_10 : 3;
    uint8_t ch : 1;

  }sec_bitfield;
} _sec;

typedef union {
  uint8_t raw;
  struct min_bit {
    uint8_t min_1 : 4;
    uint8_t min_10 : 3;
    uint8_t blank : 1;

  } min_bitfield;
} _min;

typedef union {
  uint8_t raw;
  struct hour_bit {
    uint8_t hour_1 : 4;
    uint8_t hour_10 : 2;
    uint8_t time_24 : 2;

  } hour_bitfield;
} _hour;

typedef union {
  uint8_t raw;
  struct date_bit {
    uint8_t date_1 : 4;
    uint8_t date_10 : 2;
    uint8_t blank : 2;

  } date_bitfield;
} _date;

typedef union {
  uint8_t raw;
  struct month_bit {
    uint8_t month_1 : 4;
    uint8_t month_10 : 1;
    uint8_t blank : 3;

  } month_bitfield;
} _month;

typedef union {
  uint8_t raw;
  struct day_bit {
    uint8_t day_1 : 3;
    uint8_t blank : 5;
  } day_bitfield;
} _day;

typedef union {
  uint8_t raw;
  struct year_bit {
    uint8_t year_1 : 4;
    uint8_t year_10 : 4;
  } year_bitfield;
} _year;

// 구조체 관련 함수
void s_burst_mode_read(s_rtc_time *r);
void u_burst_mode_read(u_rtc_time *r);
void s_burst_mode_print(s_rtc_time *r);
void u_burst_mode_print(u_rtc_time *r);

void bitfield_burst_mode_read(void);
void bitfield_burst_mode_print(void);
void bit_print_AM_PM(void);

// bool 함수 추가
bool DS1302_Init(void);
bool DS1302_Set_Sec(uint8_t set_sec);
bool DS1302_Set_Min(uint8_t set_min);
bool DS1302_Set_Hour_case_24(uint8_t set_hour);
bool DS1302_Set_Hour_case_12(uint8_t set_hour, uint8_t set_AM_PM);
bool DS1302_Set_AM_PM(uint8_t set_AM_PM);

bool DS1302_Set_Hour_12h(void);
bool DS1302_Set_Hour_24h(void);

bool DS1302_Set_Date(uint8_t set_date);
bool DS1302_Set_Month(uint8_t set_month);
bool DS1302_Set_Day(uint8_t set_day);
bool DS1302_Set_year(uint8_t set_year);


// void burst_read_print(d_rtc_time *r);

#endif