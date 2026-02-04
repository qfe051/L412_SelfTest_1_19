#include "ds1302.h"
#include "main.h"
#include <stdint.h>


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
//   //CE ON
//   HAL_GPIO_WritePin(GPIOC, DS_RST_Pin, GPIO_PIN_SET);

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
  enable_hour_24 = now_hour & 0x7F;

  write_reg_ds1302(0x84, enable_hour_24);
}

void disable_24H(void) {
  uint8_t now_hour = 0;
  uint8_t disable_hour_24= 0;

  now_hour = read_reg_ds1302(0x85);
  disable_hour_24 = now_hour | 0x80;

  write_reg_ds1302(0x84, disable_hour_24);
}

void set_AM(void) {
  uint8_t now_hour = 0;
  uint8_t set_AM = 0;

  disable_24H();
    now_hour = read_reg_ds1302(0x85);
  set_AM = now_hour & 0xDF;
  

  printf("set_AM %#x \r\n", set_AM);

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