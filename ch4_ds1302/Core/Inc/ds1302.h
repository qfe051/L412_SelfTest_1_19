#include <stdint.h>

#ifndef __DS1302_H
#define __DS1302_H

void set_output_ds1302();
void set_input_ds1302();

void write_byte_ds1302(uint8_t data);
uint8_t read_byte_ds1302(void);

void write_reg_ds1302(uint8_t address, uint8_t data);
uint8_t read_reg_ds1302(uint8_t address);

#endif