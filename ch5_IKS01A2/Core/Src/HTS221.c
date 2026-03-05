#include "HTS221.h"
#include "main.h"
#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_it.h"

// 내부 함수에서 사용되는 변수 선언



// 드라이버 코드에 넣기!! 
uint8_t address_HTS221 = 0xBE; // 0xBE -> datasheet 값

uint8_t data[] = {};
uint8_t read_temper_data[2] = {0};
uint8_t read_humid_data[2] = {0};

uint8_t reg_temper_address = 0x2A;
uint8_t reg_humid_address = 0x28;



void read_temper_HTS221(void) {

  HAL_I2C_Master_Transmit(&hi2c1, address_HTS221, reg_temper_address , 1, 50);
  HAL_I2C_Master_Receive(&hi2c1, address_HTS221, read_temper_data[0], 1, 50);


  printf("read_temper_data[0] : %#x \r\n",read_temper_data[0]);
}


//   // 모듈 주소 출력 테스트 
//   HAL_StatusTypeDef module_status;
//   for (uint8_t i = 0; i < 128; i++) {
//     HAL_Delay(10);

//     module_status = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 3, 10);
//     if (module_status == HAL_OK) {
//       printf("module address : %#x \r\n",i);
//     }
//   }

//  for (uint8_t i = 0; i < 128; i++) {
//    HAL_Delay(10);

//    module_status = HAL_I2C_IsDeviceReady(&hi2c2, i << 1, 3, 10);
//    if (module_status == HAL_OK) {
//      printf("[hi2c2]module address : %#x \r\n",i);
//    }
//  }


void test_read_temper_HTS221(uint8_t dev_address, uint8_t regi_address) {

  uint8_t output_data=0;

  uint8_t buf[2]={regi_address,0};
  
  HAL_I2C_Master_Transmit(&hi2c1, dev_address, buf , 2, 50);
  HAL_I2C_Master_Receive(&hi2c1, dev_address+1, &output_data, 1, 50);


  printf("output_data(%#x) : %#x \r\n",dev_address,output_data);
}

void test_read_mem_HTS221(uint8_t dev_address, uint8_t regi_address) {

  uint8_t output_data=0;

  
  HAL_I2C_Mem_Write(&hi2c1, dev_address, regi_address,1,0xff , 1, 50);
  // HAL_I2C_Mem_Read(&hi2c1, dev_address+1, regi_address,1,output_data, 1, 50);
  HAL_I2C_Mem_Read(&hi2c1, dev_address+1, regi_address , 1,output_data, 1, 50);

  printf("output_data(%#x) : %#x \r\n",dev_address,output_data);
}


void test_read_transmit_and_mem_HTS221(uint8_t dev_address, uint8_t regi_address) {
  uint8_t output_data_1 = 0;
  uint8_t output_data_2 = 0;

  // HAL_Delay(2000);
  HAL_I2C_Master_Transmit(&hi2c1, dev_address, &regi_address, 1, 50);
  HAL_I2C_Master_Receive(&hi2c1, dev_address, &output_data_1, 1, 50);

  HAL_Delay(2000);
  // HAL_I2C_Mem_Write(&hi2c1, dev_address, &regi_address,1, NULL, 0, 50);

  // stop 없이 re- start 
  HAL_I2C_Mem_Read(&hi2c1, dev_address,&regi_address,1,&output_data_2,1,50);

  printf("Master output_data : %#x \r\n", output_data_1);
  printf("Mem output_data : %#x \r\n", output_data_2);

  printf(" \r\n");
}
