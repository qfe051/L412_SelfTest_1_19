#include "ds1302.h"
#include "main.h"
#include <stdint.h>

// static 함수 정의 하기 -> 내부에서만 사용하는 함수 

// static void DS1302_IO_Set_Output(bool isOutput);



// data - output
void set_output_ds1302() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = DS_DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
// data - input
void set_input_ds1302() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = DS_DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


void write_byte_ds1302(uint8_t data) {
  set_output_ds1302();


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
  set_input_ds1302();
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



uint8_t bcd_2_dec(uint8_t data) {
  uint8_t data_1 = 0;
  uint8_t data_10 = 0;

  data_1 = (data & 0x0F);
  data_10 = (data >> 4) & 0x0F;

  data = 10 * data_10 + data_1;

  return data;
}

uint8_t bcd_2_dec_hour(uint8_t data) {
  uint8_t data_1 = 0;
  uint8_t data_10 = 0;

  if (data & 0x80) {
    data_1 = (data & 0x0F);
    data_10 = (data >> 4) & 0x01;
  } else {
    data_1 = (data & 0x0F);
    data_10 = (data >> 4) & 0x03;
  }

    data = 10 * data_10 + data_1;

    return data;
}

uint8_t bcd_2_dec_sec(uint8_t data) {
  uint8_t data_1 = 0;
  uint8_t data_10 = 0;

  data_1 = (data & 0x0F);
  data_10 = (data >> 4) & 0x07;

  data = 10 * data_10 + data_1;

  return data;
}

void s_burst_mode_read(s_rtc_time *r) {

  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_SET);

  write_byte_ds1302(0xBF);

  r->sec = read_byte_ds1302();
  r->min = read_byte_ds1302();
  r->hour = read_byte_ds1302();
  r->date = read_byte_ds1302();
  r->month = read_byte_ds1302();
  r->day = read_byte_ds1302();
  r->year = read_byte_ds1302();

  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_RESET);
}

void u_burst_mode_read(u_rtc_time *r) {
  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_SET);

  write_byte_ds1302(0xBF);

  r->idx_data[0] = read_byte_ds1302();
  r->idx_data[1] = read_byte_ds1302();
  r->idx_data[2] = read_byte_ds1302();
  r->idx_data[3] = read_byte_ds1302();
  r->idx_data[4] = read_byte_ds1302();
  r->idx_data[5] = read_byte_ds1302();
  r->idx_data[6] = read_byte_ds1302();


  HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_RESET);
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
}

void s_burst_mode_print(s_rtc_time *r) {
  printf("sec : %d \r\n", bcd_2_dec_sec(r->sec));
  printf("min : %d \r\n", bcd_2_dec(r->min));
  printf("hour : %d \r\n", bcd_2_dec_hour(r->hour));
  print_AM_PM(r->hour);
  printf("date : %d \r\n", bcd_2_dec(r->date));
  printf("month : %d \r\n", bcd_2_dec(r->month));
  day_print(r->day);
  printf("year : %d \r\n", bcd_2_dec(r->year));
}

void u_burst_mode_print(u_rtc_time *r) {
  printf("sec : %d \r\n", bcd_2_dec_sec(r->idx_data[0]));
  printf("min : %d \r\n", bcd_2_dec(r->idx_data[1]));
  printf("hour : %d \r\n", bcd_2_dec_hour(r->idx_data[2]));
  print_AM_PM(r->idx_data[2]);
  printf("date : %d \r\n", bcd_2_dec(r->idx_data[3]));
  printf("month : %d \r\n", bcd_2_dec(r->idx_data[4]));
  day_print(r->idx_data[5]);
  printf("year : %d \r\n", bcd_2_dec(r->idx_data[6]));
}

// void burst_read_print(d_rtc_time *r) {

//   HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_SET);

//   write_byte_ds1302(0xBF);

//   r->input_data = read_byte_ds1302();
//   printf("sec : %d \r\n", bcd_2_dec_sec(r->sec));

//   r->input_data = read_byte_ds1302();
//   printf("min : %d \r\n", bcd_2_dec(r->min));

//   r->input_data = read_byte_ds1302();
//   printf("hour : %d \r\n", bcd_2_dec_hour(r->hour));
//   print_AM_PM(r->hour);

//   r->input_data = read_byte_ds1302();
//   printf("date : %d \r\n", bcd_2_dec(r->date));

//   r->input_data = read_byte_ds1302();
//   printf("month : %d \r\n", bcd_2_dec(r->month));

//   r->input_data = read_byte_ds1302();
//   day_print(r->day);
  
//   r->input_data = read_byte_ds1302();
//   printf("year : %d \r\n", bcd_2_dec(r->year));


//   HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_RESET);
// }

void enable_clock(void) {
  uint8_t now_sec = 0;
  uint8_t enable_now_sec = 0;

  now_sec = read_reg_ds1302(0x81);
  enable_now_sec = now_sec & 0x7F;

  write_reg_ds1302(0x80, enable_now_sec);
}

void disable_clock(void) {
  uint8_t now_sec = 0;
  uint8_t disable_now_sec = 0;

  now_sec = read_reg_ds1302(0x81);
  disable_now_sec = now_sec | 0x80;

  write_reg_ds1302(0x80, disable_now_sec);
}

void enable_24H(void) {
  uint8_t now_hour = 0;
  uint8_t enable_hour_24 = 0;
  now_hour = read_reg_ds1302(0x85);

  // 현재 상태와 동일한 경우 break
  if ((0x80 & now_hour) == 0) {
  } // PM인 경우 분기
  else if (0x20 & now_hour) {
    // pm1~7시
    if ((now_hour & 0x1F) < 0x07) {
      enable_hour_24 = (now_hour & 0x0F) + 0x12;
      write_reg_ds1302(0x84, enable_hour_24);
    } // pm12시
    else if ((now_hour & 0x1F) == 0x12) {
      enable_hour_24 = 0x00;
      write_reg_ds1302(0x84, enable_hour_24);
    } // pm8~11시
    else {
      enable_hour_24 = (now_hour & 0x2F) - 0x08;
      write_reg_ds1302(0x84, enable_hour_24);
    }
  } // AM 인 경우 , AM12시 경우 추가하기
  else {
    enable_hour_24 = now_hour & 0x7F;
    write_reg_ds1302(0x84, enable_hour_24);
  }

  
}

void disable_24H(void) {
  uint8_t now_hour = 0;
  uint8_t disable_hour_24 = 0;
  now_hour = read_reg_ds1302(0x85);

  // 현재 상태와 동일한 경우 break
  if ((0x80 & now_hour)) {
  } // 13~23시
  else if ((0x20 & now_hour) | ((0x10 & now_hour) && (0x02 < (now_hour & 0x0F)))) {
    disable_hour_24 = (now_hour | 0x80) - 0x12;
    disable_hour_24 = disable_hour_24 | 0x20; // PM 표시
    write_reg_ds1302(0x84, disable_hour_24);
  } //24시
  else if (now_hour == 0x00) {
    disable_hour_24 = 0x92;
    write_reg_ds1302(0x84, disable_hour_24);
  } //12시
  else if (now_hour == 0x12) {
    disable_hour_24 = 0xB2;
    write_reg_ds1302(0x84, disable_hour_24);
  } // 1~11시 
  else {
    disable_hour_24 = now_hour | 0x80;
    write_reg_ds1302(0x84, disable_hour_24);
  }

  
}

void set_AM(void) {
  uint8_t now_hour = 0;
  uint8_t set_AM = 0;

  disable_24H();
  now_hour = read_reg_ds1302(0x85);
  set_AM = now_hour & 0xDF;
  


  write_reg_ds1302(0x84, set_AM);
}

void set_PM(void) {
  uint8_t now_hour = 0;
  uint8_t set_PM = 0;

  disable_24H();
  now_hour = read_reg_ds1302(0x85);
  set_PM = now_hour | 0x20;

  write_reg_ds1302(0x84, set_PM);
}

// const char* day_num_2_str(uint8_t data) {
//   char days;

//   switch (data) {
//   case 1:
//     days= "Mon";
//     break;
//   case 2:
//     days= "Tue";
//     break;
//   case 3:
//     days = "Wed";
//     break;
//   case 4:
//     days= "Thu";
//     break;
//   case 5:
//     days="Fri";
//     break;
//   case 6:
//     days="Sat";
//     break;
//   case 7:
//     days= "Sun";
//     break;
//   }

//   return days;
// }

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
  }

  printf("day : %s \r\n",days);
}

void print_AM_PM(uint8_t data) {

  uint8_t now_time = 0;
  now_time = read_reg_ds1302(0x85);
  //   now_time = read_reg_ds1302(0x85);

  if ((now_time & 0x80) == 0x80) {
    if ((now_time & 0x20) == 0x20) {
      printf("Time PM \r\n");
    } else {
      printf("Time AM\r\n");
    }

    } else {
    printf("Time : 24H  \r\n");
    }
}

// bitfield 기반 함수 설정 

void bit_print_AM_PM(void) {

  uint8_t now_time, AM_PM = 0;
  now_time = hour.hour_bitfield.time_24;
  AM_PM = hour.hour_bitfield.hour_10;


  if (now_time == 0x02) {
    if (AM_PM == 0x02) {
      printf("Time PM \r\n");
    } else {
      printf("Time AM\r\n");
    }

  } else {
    printf("Time : 24H  \r\n");
  }
}

// bool 함수 추가

bool DS1302_Init(void) {
  
}

bool DS1302_Set_Sec(uint8_t set_sec) {
  sec.raw = read_reg_ds1302(0x81);
  sec.sec_bitfield.sec_10 = set_sec / 10;
  sec.sec_bitfield.sec_1 = set_sec % 10;

  uint8_t sec_data = sec.sec_bitfield.sec_10 * 10 + sec.sec_bitfield.sec_1;

  if (sec_data == set_sec && set_sec < 60) {
 

    write_reg_ds1302(0x80, sec.raw);

    return true;
  } else {
    return false;
  }
}

bool DS1302_Set_Min(uint8_t set_min) {
  min.raw = read_reg_ds1302(0x83);
  min.min_bitfield.min_10 = set_min / 10;
  min.min_bitfield.min_1 = set_min % 10;

  uint8_t min_data = min.min_bitfield.min_10 * 10 + min.min_bitfield.min_1;
  // uint8_t read_min_data;

  if (min_data == set_min && set_min < 60) {
    write_reg_ds1302(0x82, min.raw);

    return true;
  } else {
    return false;
  }
}

// 0~23시 세팅 가능
bool DS1302_Set_Hour_case_24(uint8_t set_hour) {
  hour.raw = read_reg_ds1302(0x85);
  printf("read hour.raw %#x \r\n", hour.raw);
  if (hour.hour_bitfield.time_24 == 0) {
    hour.hour_bitfield.hour_10 = set_hour / 10;
    hour.hour_bitfield.hour_1 = set_hour % 10;

    uint8_t hour_data = hour.hour_bitfield.hour_10 * 10 + hour.hour_bitfield.hour_1;

    if (hour_data == set_hour && set_hour < 24) {
      write_reg_ds1302(0x84, hour.raw);
      printf("hour.raw %#x \r\n",hour.raw);

      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

//  1~12시 세팅 가능 ,  AM PM 세팅 가능
bool DS1302_Set_Hour_case_12(uint8_t set_hour, uint8_t set_AM_PM) {
  hour.raw = read_reg_ds1302(0x85);

  if (set_AM_PM == 0) {
    hour.raw = hour.raw & 0xCF;
    write_reg_ds1302(0x84, hour.raw);
  } else if (set_AM_PM == 1) {
    hour.raw = hour.raw | 0x20;
    write_reg_ds1302(0x84, hour.raw);
  }

  if (hour.hour_bitfield.time_24 == 2) {
    hour.hour_bitfield.hour_10 = set_hour / 10;
    hour.hour_bitfield.hour_1 = set_hour % 10;

    uint8_t hour_data = hour.hour_bitfield.hour_10 * 10 + hour.hour_bitfield.hour_1;

    if (hour_data == set_hour && set_hour < 13) {
      write_reg_ds1302(0x84, hour.raw);

      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

// AM PM 세팅 가능
bool DS1302_Set_AM_PM(uint8_t set_AM_PM) {
  uint8_t AM, PM;
  hour.raw = read_reg_ds1302(0x85);
  if (set_AM_PM == AM) {
    hour.raw = hour.raw & 0xCF;
    write_reg_ds1302(0x84, hour.raw);
    return true;
  }
  else if (set_AM_PM == PM){
    hour.raw = hour.raw | 0x20;
    write_reg_ds1302(0x84, hour.raw);
    return true;
  }
  else {
    return false;
  }
}

//  12시간제, 24시간제 세팅
bool DS1302_Set_Hour_12h(void) {
  hour.raw = read_reg_ds1302(0x85);
  // 24시간제 -> 12시간제
  if (hour.hour_bitfield.time_24 == 0x0) {
    // PM인 경우 12 ~ 23h
    if (hour.hour_bitfield.hour_10 == 0x2) {
      // 20시 21시인 경우
      if (hour.hour_bitfield.hour_1 == 1 | hour.hour_bitfield.hour_1 == 0) {
        hour.hour_bitfield.hour_1 = hour.hour_bitfield.hour_1 + 8;
        hour.hour_bitfield.time_24 = 0x2;
        printf("set hour.raw 20h %#x \r\n", hour.raw);
        write_reg_ds1302(0x84, hour.raw);
      }// 나머지 모든경우
      else{
        hour.hour_bitfield.hour_1 = hour.hour_bitfield.hour_1 - 2;
        hour.hour_bitfield.time_24 = 0x2;
        printf("set hour case2  %#x \r\n", hour.raw);
        write_reg_ds1302(0x84, hour.raw);
      }
    }
    // AM인 경우
    else {
      if (hour.hour_bitfield.hour_10 == 0x1 && hour.hour_bitfield.hour_1 == 0x02) {
        hour.hour_bitfield.hour_1 = 0;
        hour.hour_bitfield.hour_10 = 0;
        hour.hour_bitfield.time_24 = 0x2;
        printf("set hour case3  %#x \r\n", hour.raw);
        write_reg_ds1302(0x84, hour.raw);
      }
      hour.hour_bitfield.time_24 = 0x2;
      printf("set hour case4  %#x \r\n", hour.raw);
      write_reg_ds1302(0x84, hour.raw);
    }
  

    return true;
  }
  else {
      return false;
  }
}

// 12->24 시간제로 세팅 -> 수정하기 
bool DS1302_Set_Hour_24h(void) {
  
}

bool DS1302_Set_Date(uint8_t set_date) {
  date.raw = read_reg_ds1302(0x87);
  date.date_bitfield.date_10 = set_date / 10;
  date.date_bitfield.date_1 = set_date % 10;

  uint8_t date_data =
      date.date_bitfield.date_10 * 10 + date.date_bitfield.date_1;
  // uint8_t read_min_data;

  if (date_data == set_date && set_date < 32) {
    write_reg_ds1302(0x86, date.raw);

    return true;
  } else {
    return false;
  }
}

bool DS1302_Set_Month(uint8_t set_month) {
  month.raw = read_reg_ds1302(0x89);
  month.month_bitfield.month_10 = set_month / 10;
  month.month_bitfield.month_1 = set_month % 10;

  uint8_t month_data =
      month.month_bitfield.month_10 * 10 + month.month_bitfield.month_1;
  // uint8_t read_min_data;

  if (month_data == set_month && set_month < 13) {
    write_reg_ds1302(0x88, month.raw);

    return true;
  } else {
    return false;
  }
}

bool DS1302_Set_Day(uint8_t set_day) {
  day.raw = read_reg_ds1302(0x8B);
  day.day_bitfield.day_10 = set_day / 10;
  day.day_bitfield.day_1 = set_day % 10;

  uint8_t day_data = day.day_bitfield.day_10 * 10 + day.day_bitfield.day_1;
  // uint8_t read_min_data;

  if (day_data == set_day && set_day < 8) {
    write_reg_ds1302(0x8A, day.raw);

    return true;
  } else {
    return false;
  }
}

bool DS1302_Set_year(uint8_t set_year) {
  year.raw = read_reg_ds1302(0x8D);
  year.year_bitfield.year_10 = set_year / 10;
  year.year_bitfield.year_1 = set_year % 10;

  uint8_t year_data =
      year.year_bitfield.year_10 * 10 + year.year_bitfield.year_1;
  // uint8_t read_min_data;

  if (year_data == set_year && set_year < 100) {
    write_reg_ds1302(0x8C, year.raw);

    return true;
  } else {
    return false;
  }
}