#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#ifndef __DS1302_H
#define __DS1302_H

// 옵션 전차리기로 정의
// 전체 옵션 변수화 하기 , 열거형은 그냥 두기 

#define SET_SEC 20   // 0~59
#define SET_MIN 30   // 0~59
#define SET_DATE 19  // 1~31
#define SET_MONTH 2 // 1~12
#define SET_DAY 4  // 1~7
#define SET_Year 26 // 0~99

#define SET_HOUR 11 // 1~12 or 0~23

// type형 변수 전처리기로 정의
// 모든 설정 변경하기 위함 
#define SET_INIT_TIME_TYPE IS_SET_NEW_TIME //IS_CURRENT_TIME, IS_SET_NEW_TIME

// 각각 함수에서 옵션 사용하기 
// // IS_SET_NEW_TIME 설정 시 동작 
#define SET_HOUR_TYPE_AM_PM HOUR_TYPE_AM // HOUR_TYPE_AM, HOUR_TYPE_PM
#define SET_HOUR_TYPE_12_24 HOUR_TYPE_12 // HOUR_TYPE_12, HOUR_TYPE_24
#define SET_IS_CLOCK_ENABLE ENABLE_CLOCK //DISABLE_CLOCK , ENABLE_CLOCK

#define SET_ENABLE_WRITE ENABLE_WRITE // ENABLE_WRITE , DISABLE_WRITE

// 변수 선언
// uint8_t set_sec = 10; // 0~59
// uint8_t set_min = 20; // 0~59
// uint8_t set_date = 13;  // 1~31
// uint8_t set_month = 2; // 1~12
// uint8_t set_day = 5; // 1~7
// uint8_t set_year = 10;  // 0~99

// uint8_t set_hour = 10 ; // 1~12 or 0~23

// 함수 선언
// 열거형으로 0,1 순서대로 사용 
typedef enum {
  HOUR_TYPE_12 = 0,
  HOUR_TYPE_24
} HOUR_TYPE_12_24;

typedef enum {
  IS_CURRENT_TIME = 0,
  IS_SET_NEW_TIME
} INIT_TIME_TYPE;

typedef enum {
  HOUR_TYPE_AM = 0,
  HOUR_TYPE_PM
} HOUR_TYPE_AM_PM;

typedef enum {
  ENABLE_CLOCK = 0,
  DISABLE_CLOCK
} IS_CLOCK_ENABLE;

typedef enum {
  ENABLE_WRITE = 0,
  DISABLE_WRITE
} IS_ENABLE_WRITE;

// bool ENABLE_WRITE = true;
// bool DISABLE_WRITE = false;

// 디버깅용으로 필요
void write_reg_ds1302(uint8_t address, uint8_t data);
uint8_t read_reg_ds1302(uint8_t address);


// MCU에 data 담기 
typedef struct {
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t date;
  uint8_t month;
  uint8_t day;
  uint8_t year;
} _MCU_time_data;


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




// // 구조체 , 공용체 선언


// 구조체 관련 함수

void bust_mode_Bitfield(_Display_data *r,uint8_t mode);


// bool 함수 추가
bool DS1302_Init(_MCU_time_data *r);


bool enable_write(IS_CLOCK_ENABLE type);

// 코드 정리 이후 추가

// 코드 작성 중
bool is_hour_PM_mode(void);
void get_MCU_clock_inform(_MCU_time_data *r);
void print_MCU_clock_inform(_MCU_time_data *r);
void write_MCU_clock_to_ds1302(_MCU_time_data *r);
void bitfield_burst_mode_write(void);
// void set_new_time(_MCU_time_data *r, INIT_TIME_TYPE type);
void set_new_time(_MCU_time_data *r,INIT_TIME_TYPE type, uint8_t input_sec,uint8_t input_min,uint8_t input_hour , uint8_t input_date , uint8_t input_month, uint8_t input_day, uint8_t input_year);


bool enable_ch(IS_CLOCK_ENABLE type);

// 재사용 함수
bool set_hour_type_12_24(_MCU_time_data *r, HOUR_TYPE_12_24 type);
bool set_hour_type_AM_PM(HOUR_TYPE_AM_PM type);

// void burst_read_print(d_rtc_time *r);

#endif