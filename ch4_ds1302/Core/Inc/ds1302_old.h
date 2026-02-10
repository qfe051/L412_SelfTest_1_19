#include <stdbool.h>
#include <stdint.h>



// 함수 선언
void set_output_ds1302();
void set_input_ds1302();

void write_byte_ds1302(uint8_t data);
uint8_t read_byte_ds1302(void);

uint8_t bcd_2_dec(uint8_t data);
uint8_t bcd_2_dec_hour(uint8_t data);
uint8_t bcd_2_dec_sec(uint8_t data);