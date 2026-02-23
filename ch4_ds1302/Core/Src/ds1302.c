#include "ds1302.h"
#include "main.h"
#include "tm1637.h"
#include <stdbool.h>
#include <stdint.h>




// static 함수 정의 하기 -> 내부에서만 사용하는 함수
static void DS1302_IO_Set_Output(bool isOutput);

static void write_byte_ds1302(uint8_t data);
static uint8_t read_byte_ds1302(void);

static void day_print(uint8_t data);

static void bitfield_burst_mode_read(void);
static void bitfield_burst_mode_print(void);
static void bit_print_AM_PM(void);

// static void set_MCU_clock_inform(_MCU_time_data *r);
static void set_MCU_clock_inform(_MCU_time_data *r, uint8_t input_sec,uint8_t input_min,uint8_t input_hour , uint8_t input_date , uint8_t input_month, uint8_t input_day, uint8_t input_year) ;


// Init용 함수

// bool 함수 추가
// static bool set_hour_type_12_24(_MCU_time_data *r, HOUR_TYPE_12_24 type);
// static bool set_hour_type_AM_PM(HOUR_TYPE_AM_PM type);

/*
< 미사용 함수>
void enable_clock(void);
void disable_clock(void);

void enable_24H(void);
void disable_24H(void);

void set_AM(void);
void set_PM(void);

*/

// 기존 함수 묶어서 사용하기

void bust_mode_Bitfield(void) {
  bitfield_burst_mode_read();
  bitfield_burst_mode_print();
}


// static void DS1302_IO_Set_Output(bool isOutput);




bool isOutput;

void DS1302_IO_Set_Output(bool isOutput) {
  if (isOutput == true) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DS_DAT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  else {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DS_DAT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  
}

// Clock 활성화
// bool isEnable;
void DS1302_Clock_Enable(bool isEnable, _sec *r) {
  if (isEnable == true) {
    r->sec_bitfield.ch = 0;
  }
  else {
    r->sec_bitfield.ch = 1;
  }
}

void write_byte_ds1302(uint8_t data) {
  isOutput = true;
  DS1302_IO_Set_Output(isOutput);

  for (int i = 0; i < 8; i++) {
    if ((data & 0x01) == 1) {
      HAL_GPIO_WritePin(GPIOC,DS_DAT_Pin, GPIO_PIN_SET);
    }
    else {
      HAL_GPIO_WritePin(GPIOC, DS_DAT_Pin, GPIO_PIN_RESET);
    }
    data = data >> 1;
    
    HAL_GPIO_WritePin(GPIOC, DS_CLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, DS_CLK_Pin, GPIO_PIN_RESET);
  }
  HAL_GPIO_WritePin(GPIOC, DS_DAT_Pin, GPIO_PIN_RESET);
}

uint8_t read_byte_ds1302(void) {
  isOutput = false;
  DS1302_IO_Set_Output(isOutput);
  // set_input_ds1302();
  uint8_t read_data = 0;
  uint8_t temp_data = 0;


  for (int i = 0; i < 8; i++) {
    HAL_GPIO_WritePin(GPIOC, DS_CLK_Pin, GPIO_PIN_SET);
    if (HAL_GPIO_ReadPin(GPIOC, DS_DAT_Pin) == 1) {
    //   temp_data = 1;
    //   temp_data = (temp_data << i);
    //   read_data = (read_data | temp_data);
    read_data |= (1 << i);
    }
    
    HAL_GPIO_WritePin(GPIOC, DS_CLK_Pin, GPIO_PIN_RESET);
    
  }

  return read_data;
}

void write_reg_ds1302(uint8_t address, uint8_t data) {

  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_SET);

  write_byte_ds1302(address);

  write_byte_ds1302(data);

  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_RESET);
}

uint8_t read_reg_ds1302(uint8_t address) {
  uint8_t read_data = 0;

  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_SET);

  write_byte_ds1302(address);

  read_data = read_byte_ds1302();

  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_RESET);

  return read_data;
}

// bitfield test
_sec sec;
_min min;
_hour hour;
_date date;
_month month;
_day day;
_year year;

void bitfield_burst_mode_read(void) {

  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_SET);

  write_byte_ds1302(0xBF);

  sec.raw = read_byte_ds1302();
  min.raw = read_byte_ds1302();
  hour.raw = read_byte_ds1302();
  date.raw = read_byte_ds1302();
  month.raw = read_byte_ds1302();
  day.raw = read_byte_ds1302();
  year.raw = read_byte_ds1302();

  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_RESET);
}

// bcd -> dec 변환 없이 bit 단위로 10진수 출력 (%d)
void bitfield_burst_mode_print(void) {
  if (hour.hour_bitfield.time_24) {
    hour.hour_bitfield.hour_10 = hour.hour_bitfield.hour_10 & 0x1;
  }

  uint8_t sec_data = sec.sec_bitfield.sec_10 * 10  +sec.sec_bitfield.sec_1;
  uint8_t min_data = 10 * min.min_bitfield.min_10 + min.min_bitfield.min_1;
  uint8_t hour_data = 10 * hour.hour_bitfield.hour_10 + hour.hour_bitfield.hour_1;
  uint8_t date_data = 10 * date.date_bitfield.date_10 + date.date_bitfield.date_1;
  uint8_t month_data = 10 * month.month_bitfield.month_10 + month.month_bitfield.month_1;
  uint8_t day_data = day.day_bitfield.day_1;
  uint8_t year_data = 10 * year.year_bitfield.year_10 + year.year_bitfield.year_1;

  

  printf("sec : %d \r\n", sec_data);
  printf("min : %d \r\n", min_data);
  printf("hour : %d \r\n", hour_data);
  bit_print_AM_PM();
  printf("date : %d \r\n", date_data);
  printf("month : %d \r\n", month_data);
  day_print(day_data);
  printf("year : %d \r\n", year_data);


  show_tm1637(hour_data, min_data);
}

// PM인 경우 함수 만들기
bool is_hour_PM_mode(void) {
  bitfield_burst_mode_read();
  if ((hour.hour_bitfield.hour_10 == 0x2 | hour.hour_bitfield.hour_10 == 0x3) && hour.hour_bitfield.time_24 == 0x2) {
    return true;
  }
  else {
  return false;
  }
}

// 24시간 형태로 잡아두기
// MCU 함수 하나로
void set_new_time(_MCU_time_data *r,INIT_TIME_TYPE type, uint8_t input_sec,uint8_t input_min,uint8_t input_hour , uint8_t input_date , uint8_t input_month, uint8_t input_day, uint8_t input_year) {
  get_MCU_clock_inform(r);
  if (type) {
    set_MCU_clock_inform(r,input_sec,input_min,input_hour,input_date,input_month,input_day,input_year);
    
  }
  write_MCU_clock_to_ds1302(r);
  printf("MCU 기반 시간 세팅 완료 \r\n");
  print_MCU_clock_inform(r);
  printf("\r\n");


}
// 내부 clock 저장 함수
void get_MCU_clock_inform(_MCU_time_data *r) {
  bitfield_burst_mode_read();
  
  if (is_hour_PM_mode()) {
    r->hour = 10 * (hour.hour_bitfield.hour_10 - 2) + hour.hour_bitfield.hour_1;
    r->hour += 12;
  }
  else {
    r->hour =10 * hour.hour_bitfield.hour_10 + hour.hour_bitfield.hour_1;
  }

  r->sec = sec.sec_bitfield.sec_10 * 10  +sec.sec_bitfield.sec_1;
  r->min = 10 * min.min_bitfield.min_10 + min.min_bitfield.min_1;
  // r->hour =10 * hour.hour_bitfield.hour_10 + hour.hour_bitfield.hour_1;
  r->date = 10 * date.date_bitfield.date_10 + date.date_bitfield.date_1;
  r->month= 10 * month.month_bitfield.month_10 + month.month_bitfield.month_1;
  r->day= day.day_bitfield.day_1;
  r->year = 10 * year.year_bitfield.year_10 + year.year_bitfield.year_1;

  // PM인 경우 'hour.hour_bitfield.hour_10'가 2,3으로 정의 됨 
  
}

// void set_MCU_clock_inform(_MCU_time_data *r) {
//   bitfield_burst_mode_read();
//   // INIT_TIME_TYPE 활성화 된 경우
  
//   r->sec = SET_SEC;
//   r->min = SET_MIN;
//   r->hour =SET_HOUR;
//   r->date = SET_DATE;
//   r->month= SET_MONTH;
//   r->day= SET_DAY;
//   r->year = SET_Year;
  
  
  

//   // PM인 경우 'hour.hour_bitfield.hour_10'가 2,3으로 정의 됨 
//   if (is_hour_PM_mode()) {
//     r->hour = r->hour % 12;
//   }
// }


void set_MCU_clock_inform(_MCU_time_data *r, uint8_t input_sec,uint8_t input_min,uint8_t input_hour , uint8_t input_date , uint8_t input_month, uint8_t input_day, uint8_t input_year) {
  bitfield_burst_mode_read();
  // INIT_TIME_TYPE 활성화 된 경우
  
  r->sec = input_sec;
  r->min = input_min;
  r->hour =input_hour;
  r->date = input_date;
  r->month= input_month;
  r->day= input_day;
  r->year = input_year;
  
  // PM인 경우 'hour.hour_bitfield.hour_10'가 2,3으로 정의 됨 
  if (is_hour_PM_mode()) {
    r->hour = r->hour % 12;
  }
}

void print_MCU_clock_inform(_MCU_time_data *r) {
  printf(" MCU_clock_inform \r\n");
  printf("sec : %d \r\n", r->sec);
  printf("min : %d \r\n", r->min);
  printf("hour : %d \r\n", r->hour);
  printf("date : %d \r\n", r->date);
  printf("month : %d \r\n", r->month);
  day_print(r->day);
  printf("year : %d \r\n", r->year);
}

// INIT_TIME_TYPE type 인자 필요한지? 
// 옵션은 따로 진행 -> 함수화 하여 넣어주기 
void write_MCU_clock_to_ds1302(_MCU_time_data *r) {
  bitfield_burst_mode_read();
  if (enable_ch(SET_IS_CLOCK_ENABLE)) {
    sec.sec_bitfield.ch = 0;
  } else {

    sec.sec_bitfield.ch = 1;
    printf("시계 비활성화\r\n");
  }


  // sec
  sec.sec_bitfield.sec_10 = r->sec / 10;
  sec.sec_bitfield.sec_1 = r->sec % 10;

  printf("sec.raw : %#x \r\n",sec.raw);

  // min
  min.min_bitfield.min_10 = r->min / 10;
  min.min_bitfield.min_1 = r->min % 10;


  // pm 이면 12 빼서 넣기
  //  if
  //  (set_hour_type_12_24(SET_HOUR_TYPE_12_24)&&set_hour_type_AM_PM(SET_HOUR_TYPE_AM_PM))
  // hour  | am, pm 옵션 확인 추가 - enum 활용하기

  printf("[before-write] r->hour : %d \r\n",r->hour);
  if (hour.hour_bitfield.time_24 == 2) {
    if (r->hour >12) {
      r->hour = r->hour % 12;
    }
  }
  printf("[cal-write] r->hour : %d \r\n",r->hour);
  hour.hour_bitfield.hour_10 = r->hour / 10;
  hour.hour_bitfield.hour_1 = r->hour % 10;
  // AM , PM 추가 명시


  //set_hour_type_AM_PM() 이전에 쓰기 읽기 동작 추가 -> 동작 확인 
  bitfield_burst_mode_write();
  bitfield_burst_mode_read();

  set_hour_type_AM_PM(SET_HOUR_TYPE_AM_PM);
  
  
  printf("hour.hour_bitfield.time_24 : %d \r\n", hour.hour_bitfield.time_24);
  printf("hour.hour_bitfield.hour_10 : %d \r\n", hour.hour_bitfield.hour_10);
  printf("hour.hour_bitfield.hour_1 : %d \r\n", hour.hour_bitfield.hour_1);

  printf("hour.hour_bitfield.time_24 : %#x \r\n", hour.hour_bitfield.time_24);
  printf("hour.hour_bitfield.hour_10 : %#x \r\n", hour.hour_bitfield.hour_10);
  printf("hour.hour_bitfield.hour_1 : %#x \r\n",hour.hour_bitfield.hour_1);
  
  // date
  date.date_bitfield.date_10 = r->date / 10;
  date.date_bitfield.date_1 = r->date % 10;

  // month
  month.month_bitfield.month_10 = r->month / 10;
  month.month_bitfield.month_1 = r->month % 10;

  // day
  day.day_bitfield.day_1 = r->day;

  // year
  year.year_bitfield.year_10 = r->year / 10;
  year.year_bitfield.year_1 = r->year % 10;

 
  bitfield_burst_mode_write();

  
}



void bitfield_burst_mode_write(void) {
  // rST 핀 set
  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_SET);

  // 명령어 바이트
  write_byte_ds1302(0xBE);

  // 데이터 바이트
  write_byte_ds1302(sec.raw);
  write_byte_ds1302(min.raw);
  write_byte_ds1302(hour.raw);
  write_byte_ds1302(date.raw);
  write_byte_ds1302(month.raw);
  write_byte_ds1302(day.raw);
  write_byte_ds1302(year.raw);


  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_RESET);

}


void day_print(uint8_t data) {
  const char* days;

  switch (data) {
  case 1:
    days = "Mon";
    break;
  case 2:
    days = "Tue";
    break;
  case 3:
    days = "Wed";
    break;
  case 4:
    days = "Thu";
    break;
  case 5:
    days = "Fri";
    break;
  case 6:
    days = "Sat";
    break;
  case 7:
    days = "Sun";
    break;
  default:
	 days = "Wrong";
	 break;
  }

  printf("day : %s \r\n",days);
}

// bitfield 기반 함수 설정 

void bit_print_AM_PM(void) {
  hour.raw=read_reg_ds1302(0x85);
  uint8_t now_time, AM_PM = 0;
  now_time = hour.hour_bitfield.time_24;
  AM_PM = hour.hour_bitfield.hour_10;
  // printf("AM_PM : %#x \r\n",AM_PM);

  if (now_time == 0x2) {
    if (AM_PM == 0x2 | AM_PM == 0x3) {
      printf("Time : PM \r\n");
    } else {
      printf("Time : AM \r\n");
    }

  } else {
    printf("Time : 24H  \r\n");
  }
}

// mcu가 읽기 -> 형태에 따라서 hour type 바꿔주기
// 내부에서 set_hour_type 실행까지 하기
// 재사용 함수 기능만 하기 
bool set_hour_type_12_24(_MCU_time_data *r, HOUR_TYPE_12_24 type) {
  bool status = true;
  bitfield_burst_mode_read();
  printf("hour.hour_bitfield.time_24 : %d \r\n", hour.hour_bitfield.time_24);
  printf("hour.hour_bitfield.hour_10 : %d \r\n", hour.hour_bitfield.hour_10);
  printf("hour.hour_bitfield.hour_1 : %d \r\n",hour.hour_bitfield.hour_1);
  get_MCU_clock_inform(r);
  printf("[before] r->hour : %d \r\n",r->hour);
  
  switch (type) {
  case HOUR_TYPE_12:
    //24h -> 12h 변경 
    if (hour.hour_bitfield.time_24 == 0) {
      if ((r->hour == 12) | (r->hour == 24)) {
        r->hour = 12;
        printf("[] r->hour 24 or 12 : %d \r\n", r->hour);
      }
      else {
        r->hour = r->hour % 12;
        printf("[] r->hour normal : %d \r\n", r->hour);
      }
    }
    printf("[after_12h] r->hour : %d \r\n", r->hour);
    hour.hour_bitfield.time_24 = 2;
    bitfield_burst_mode_write();
    write_MCU_clock_to_ds1302(r);
    
    status = true;
    break;
  case HOUR_TYPE_24:
    // 기존 MCU 값이 24시간제로 되어 있어, 그대로 사용하면 됨.
    
    // if (is_hour_PM_mode()) {
    //   r->hour += 12;
    // }
    printf("[after_24h] r->hour : %d \r\n", r->hour);
    hour.hour_bitfield.time_24 = 0;
    // 변경 후 write 필수!
    bitfield_burst_mode_write();
    write_MCU_clock_to_ds1302(r);
    status = true;
    break;
  default:
    status = false;
    break;
  }
  return status;
}

bool set_hour_type_AM_PM(HOUR_TYPE_AM_PM type) {
  bool status = true;
  bitfield_burst_mode_read();
  if (hour.hour_bitfield.time_24 == 2) {
  switch (type) {
    case HOUR_TYPE_PM:
      hour.hour_bitfield.hour_10 = hour.hour_bitfield.hour_10 | 0x2;
      bitfield_burst_mode_write();
      status = true;
      break;
    case HOUR_TYPE_AM:
      hour.hour_bitfield.hour_10 = hour.hour_bitfield.hour_10 & 0x1;
      bitfield_burst_mode_write();
      status = true;
      break;
    default:
      status = false;
      break;
    }
  }
   else {
    printf("24시간제 입니다. \r\n");
   status = false;
   }
  
  return status;
}

bool enable_ch(IS_CLOCK_ENABLE type) {
  bitfield_burst_mode_read();
  bool status = true;
  switch (type) {
  case ENABLE_CLOCK:
    sec.sec_bitfield.ch = 0;
    bitfield_burst_mode_write();
    status = true;
    break;
  case DISABLE_CLOCK:
    sec.sec_bitfield.ch = 1;
    bitfield_burst_mode_write();
    status = true;
    break;
  default:
    status = false;
    break;    
  }
  return status;
}

bool enable_write(IS_CLOCK_ENABLE type) {
  // disable 하는 경우
  bool status = true;
  switch (type) {
    case ENABLE_WRITE:
      write_reg_ds1302(0x8E, 0x00);
      status = true;
      break;
    case DISABLE_WRITE:
      write_reg_ds1302(0x8E, 0x80);
      status = true;
      break;
    default:
      status = false;
      break;
  }

  return status;
}

bool DS1302_Init(_MCU_time_data *r) {
  get_MCU_clock_inform(r);

  // 재활성화
  set_hour_type_12_24(r,HOUR_TYPE_24);
  set_hour_type_AM_PM(HOUR_TYPE_PM);
  enable_ch(ENABLE_CLOCK);
  

  bitfield_burst_mode_write();

  enable_write(SET_ENABLE_WRITE);
}

// 24시 기준 시각 환산
// int calculate_hour(void) {
//   int hour_cal = 0;
//   if (hour.hour_bitfield.time_24 == 2) {
//     if ((hour.hour_bitfield.hour_10 == 3) | (hour.hour_bitfield.hour_10 == 2)) {
//       hour_cal = (hour.hour_bitfield.hour_10-2)*10 +hour.hour_bitfield.hour_1;
//     }
//     else {
//       hour_cal = hour.hour_bitfield.hour_10 * 10 + hour.hour_bitfield.hour_1;
//     }
//   }
//   //24h 기준
//   else {
//     hour_cal = hour.hour_bitfield.hour_10 * 10 + hour.hour_bitfield.hour_1;
//   }

  

//   return hour_cal;
// }


// bool 함수, 점검하기 위한 함수 필요
