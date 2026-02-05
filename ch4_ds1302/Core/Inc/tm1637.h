#include "main.h"
#include <stdint.h>

#ifndef __TM1637_H
#define __TM1637_H

void set_output_tm1637(void);
void set_input_tm1637(void);

void write_byte_tm1637(uint8_t data);

void set_start(void);
void set_stop(void);

void send_cmd(uint8_t data);
void send_cmd_2(uint8_t data);


#endif
