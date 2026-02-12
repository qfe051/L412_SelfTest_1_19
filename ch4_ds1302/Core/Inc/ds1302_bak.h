// #include <stdint.h>
// #include <stdbool.h>

// #ifndef __DS1302_H
// #define __DS1302_H

// // 옵션 전차리기로 정의
// #define NEW_SET_TIME 0 // 0: 기존 시간 , 1 : 새로운 시간 세팅

// #define IS_ENABLE 1 // 0 : 시계 비활성화, 1 : 시계 활성화
// #define SET_SEC 10   // 0~59
// #define SET_MIN 20   // 0~59
// #define SET_DATE 11  // 1~31
// #define SET_MONTH 2 // 1~12
// #define SET_DAY 11  // 1~7
// #define SET_Year 26 // 0~99

// #define SET_HOUR 22 // 1~12 or 0~23
// #define SET_AM_PM 0 // 0 : AM , 1: PM

// #define CH_12_2_24 1 // 0 : 바활성화, 1 : 활성화
// #define CH_24_2_12 0 // 0 : 바활성화, 1 : 활성화


// // 함수 선언
// // 열거형으로 0,1 순서대로 사용 
// enum {
//   HOUR_TYPE_12 = 0,
//   HOUR_TYPE_24
// } HOUR_TYPE;

// // 디버깅용으로 필요
// void write_reg_ds1302(uint8_t address, uint8_t data);
// uint8_t read_reg_ds1302(uint8_t address);


// // 디버깅용 

// // 구조체 선언, day - 숫자로 값 받고 -> 출력할 때 문자 변환 함수 
// typedef struct{
//   uint8_t sec;
//   uint8_t min;
//   uint8_t hour;
//   uint8_t date;
//   uint8_t month;
//   uint8_t day;
//   uint8_t year;
// } s_rtc_time;

// typedef union {
//   uint64_t full_data;
//   uint8_t idx_data[7];
// } u_rtc_time;


// // bitfield 기반 구조체 정의

// typedef union  {
//   uint8_t raw;
//   struct sec_bit {
//     uint8_t sec_1 : 4;
//     uint8_t sec_10 : 3;
//     uint8_t ch : 1;

//   }sec_bitfield;
// } _sec;

// typedef union {
//   uint8_t raw;
//   struct min_bit {
//     uint8_t min_1 : 4;
//     uint8_t min_10 : 3;
//     uint8_t blank : 1;

//   } min_bitfield;
// } _min;

// typedef union {
//   uint8_t raw;
//   struct hour_bit {
//     uint8_t hour_1 : 4;
//     uint8_t hour_10 : 2;
//     uint8_t time_24 : 2;

//   } hour_bitfield;
// } _hour;

// typedef union {
//   uint8_t raw;
//   struct date_bit {
//     uint8_t date_1 : 4;
//     uint8_t date_10 : 2;
//     uint8_t blank : 2;

//   } date_bitfield;
// } _date;

// typedef union {
//   uint8_t raw;
//   struct month_bit {
//     uint8_t month_1 : 4;
//     uint8_t month_10 : 1;
//     uint8_t blank : 3;

//   } month_bitfield;
// } _month;

// typedef union {
//   uint8_t raw;
//   struct day_bit {
//     uint8_t day_1 : 3;
//     uint8_t blank : 5;
//   } day_bitfield;
// } _day;

// typedef union {
//   uint8_t raw;
//   struct year_bit {
//     uint8_t year_1 : 4;
//     uint8_t year_10 : 4;
//   } year_bitfield;
// } _year;

// // // 구조체 , 공용체 선언
// // s_rtc_time now_s;
// // u_rtc_time now_u;
// // 구조체 관련 함수
// void bust_mode_Union(u_rtc_time *r);
// void bust_mode_Struct(u_rtc_time *r);
// void bust_mode_Bitfield(void);


// // bool 함수 추가
// bool DS1302_Init(bool is_init);


// void DS1302_Clock_Enable(bool isEnable, _sec *r);

// // void burst_read_print(d_rtc_time *r);

// #endif