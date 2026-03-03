#include "HTS221.h"
#include "main.h"
#include "stm32l4xx_hal_i2c.h"

// 내부 함수에서 사용되는 변수 선언

// uint8_t address_HTS221 = 0xBE;

// uint8_t data[] = {};
// uint8_t read_temper_data[2] = {0};
// uint8_t read_humid_data[2] = {0};

// uint8_t reg_temper_address = 0x2A;
// uint8_t reg_humid_address = 0x28;



// void read_temper_HTS221(void) {

//   HAL_I2C_Master_Transmit(&hi2c1, address_HTS221, reg_temper_address , 1, 50);
//   HAL_I2C_Master_Receive(&hi2c1, address_HTS221, read_temper_data[0], 1, 50);


//   printf("read_temper_data[0] : %#x \r\n",read_temper_data[0]);
// }