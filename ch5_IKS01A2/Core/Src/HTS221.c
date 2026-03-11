#include "HTS221.h"
#include "main.h"
#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_it.h"

// 내부 함수에서 사용되는 변수 선언



// 드라이버 코드에 넣기!! 
uint8_t address_HTS221 = 0xBE; // 0xBE -> datasheet 값

uint8_t data[] = {};

uint8_t reg_temper_address = 0x2A;
uint8_t reg_humid_address = 0x28;
uint8_t reg_humid_address_2 = 0x29;





void check_slave_enable(void) {
    // 모듈 주소 출력 테스트 
  HAL_StatusTypeDef module_status;
  for (uint8_t i = 0; i < 128; i++) {
    HAL_Delay(10);

    module_status = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 3, 10);
    if (module_status == HAL_OK) {
      printf("module address : %#x \r\n",i);
    }
  }
}





void test_read_mem_HTS221(uint8_t dev_address, uint8_t regi_address) {

  uint8_t output_data=0;

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

// void read_temperature_HTS221(void) {

//   uint8_t read_temper_data[2] = {0};


//   HAL_I2C_Master_Transmit(&hi2c1, address_HTS221, &reg_temper_address, 1, 50);
//   HAL_I2C_Master_Receive(&hi2c1, address_HTS221, read_temper_data, 2, 50);

//   // stop 없이 re- start 
// //   HAL_I2C_Mem_Read(&hi2c1, address_HTS221,&reg_temper_address,1,read_temper_data,2,50);
// //   HAL_I2C_Mem_Read(&hi2c1, address_HTS221,&input,1,read_temper_data,2,50);


//   printf("[raw]TEMP_OUT_L data : %#x \r\n", read_temper_data[0]);
//   printf("[raw]TEMP_OUT_H data : %#x \r\n", read_temper_data[1]);

//   printf(" \r\n");
// }

void read_humid_HTS221(void) {
// 자료형 점검하기 
  printf("====================================================================== \r\n");

  uint8_t read_H_OUT_data[2] = {0};

  int16_t H_OUT = 0;


//   HAL_I2C_Master_Transmit(&hi2c1, address_HTS221, &reg_humid_address, 1, 50);
//   HAL_I2C_Master_Receive(&hi2c1, address_HTS221, read_humid_data, 2, 50);


// 2개 한번에 받기 왜 안되는지 확인 
// stop 없이 re - start
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_humid_address,1,&read_H_OUT_data[0],1,50);
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_humid_address+1,1,&read_H_OUT_data[1],1,50);



  printf("[raw]HUMIDITY_OUT_L (H_OUT) data : 0x%02X \r\n", read_H_OUT_data[0]);
  printf("[raw]HUMIDITY_OUT_H (H_OUT) data : 0x%02X \r\n", read_H_OUT_data[1]);

  H_OUT = read_H_OUT_data[1] << 8 | read_H_OUT_data[0];

printf("[raw]H_OUTdata : 0x%04X \r\n", H_OUT);


printf(" \r\n");


// 2. H0_rH x2, H1_rH x2 값 구하기 -> unsigned 사용 
//H0_rH
uint8_t reg_H0_rH = 0x30;
uint8_t H0_rH_OUT_x2 = 0;

HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_H0_rH, 1, &H0_rH_OUT_x2,
                    1, 50);
printf("[raw]read_H0_rH_data : 0x%02X \r\n", H0_rH_OUT_x2);

//H1_rH
uint8_t reg_H1_rH = 0x31;
uint8_t H1_rH_OUT_x2 = 0;

HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_H1_rH, 1, &H1_rH_OUT_x2,
                    1, 50);
printf("[raw]read_H1_rH_data : 0x%02X \r\n", H1_rH_OUT_x2);

// 3. H0_OUT H1_OUT 구하기

// H0_OUT
uint8_t read_H0_OUT_data[2] = {0};
uint8_t reg_H0_OUT_address = 0x36;

  int16_t H0_OUT = 0;

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_H0_OUT_address,1,&read_H0_OUT_data[0],1,50);
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_H0_OUT_address+1,1,&read_H0_OUT_data[1],1,50);



  printf("[raw]H0_OUT_L data(H0_OUT) : 0x%02X \r\n", read_H0_OUT_data[0]);
  printf("[raw]H0_OUT_H data(H0_OUT) : 0x%02X \r\n", read_H0_OUT_data[1]);

  H0_OUT = read_H0_OUT_data[1]  << 8 | read_H0_OUT_data[0];

  printf("[raw]H0_OUTdata : 0x%04X \r\n", H0_OUT);
  printf("[dec]H0_OUTdata : %d \r\n", H0_OUT);

// H1_OUT

uint8_t read_H1_OUT_data[2] = {0};
uint8_t reg_H1_OUT_address = 0x3A;

  int16_t H1_OUT = 0;

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_H1_OUT_address,1,&read_H1_OUT_data[0],1,50);
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_H1_OUT_address+1,1,&read_H1_OUT_data[1],1,50);



  printf("[raw]H1_OUT_L data : 0x%02X \r\n", read_H1_OUT_data[0]);
  printf("[raw]H1_OUT_H data : 0x%02X \r\n", read_H1_OUT_data[1]);

  H1_OUT = read_H1_OUT_data[1] << 8 | read_H1_OUT_data[0];

  printf("[raw]H1_OUTdata : 0x%04X \r\n", H1_OUT);
  printf("[dec]H1_OUTdata : %d \r\n", H1_OUT);

  // data set
  uint8_t H0_rH_OUT = H0_rH_OUT_x2 / 2;
  uint8_t H1_rH_OUT = H1_rH_OUT_x2 / 2;

  //result
  uint8_t H_rH_OUT = 0;

  H_rH_OUT = H0_rH_OUT + (H_OUT-H0_OUT)*(H1_rH_OUT-H0_rH_OUT)/(H1_OUT-H0_OUT);



  printf("[dec]Humid data : %d  <<<<< \\% ' \r\n", H_rH_OUT);

  // 계산법 2
  uint8_t H_rH_OUT_x2 = 0;
  H_rH_OUT_x2 = H0_rH_OUT_x2 + (H_OUT-H0_OUT)*(H1_rH_OUT_x2-H0_rH_OUT_x2)/(H1_OUT-H0_OUT);
//   H_rH_OUT_x2 = H_rH_OUT_x2 / 2;
  printf("[dec]Humid data x2  : %d % \r\n", H_rH_OUT_x2);

  printf("====================================================================== \r\n");


  printf(" \r\n");
}

void read_temperature_HTS221(void) {


  printf("====================================================================== \r\n");
  uint8_t read_T_OUT_data[2] = {0};
  uint8_t reg_T_OUT_address = 0x2A;

  int16_t T_OUT = 0;



//   HAL_I2C_Master_Transmit(&hi2c1, address_HTS221, &reg_humid_address, 1, 50);
//   HAL_I2C_Master_Receive(&hi2c1, address_HTS221, read_humid_data, 2, 50);


// 2개 한번에 받기 왜 안되는지 확인 
// stop 없이 re - start
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T_OUT_address,1,&read_T_OUT_data[0],1,50);
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T_OUT_address+1,1,&read_T_OUT_data[1],1,50);



  printf("[raw]TEMPERATURE_OUT_L data : 0x%02X \r\n", read_T_OUT_data[0]);
  printf("[raw]TEMPERATURE_OUT_H data : 0x%02X \r\n", read_T_OUT_data[1]);

  T_OUT = read_T_OUT_data[1] | read_T_OUT_data[0] << 8;

printf("[raw]T_OUTdata : 0x%04X \r\n", T_OUT);


printf(" \r\n");


// 2. T0_degC x8, T8_degC x8 값 구하기
// T0 1 msb 데이터 세팅 하기
uint8_t reg_T_degC_msb = 0x35;
uint8_t T_0_1_msb = 0;
uint8_t T_0_1_msb_data[2] = {0};
HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_T_degC_msb, 1, &T_0_1_msb,
                    1, 50);
printf("[raw]T_0_1_msb _data : 0x%02X \r\n", T_0_1_msb);
// 배열에 대입 후 역할 넣기 
// T0
T_0_1_msb_data[0] = T_0_1_msb & 0x03;
// T1
T_0_1_msb_data[1] = (T_0_1_msb & 0x0C) >> 2;

//H0_rH
uint8_t reg_T0_degC = 0x32;
uint16_t T0_degC_OUT_x8 = 0;

HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_T0_degC, 1, &T0_degC_OUT_x8,
                    1, 50);
printf("[raw]T0_degC_OUT_x8 _data : 0x%02X \r\n", T0_degC_OUT_x8);


T0_degC_OUT_x8 = T0_degC_OUT_x8 | T_0_1_msb_data[0] << 8;

printf("[raw] (add msb) T0_degC_OUT_x8 _data  : 0x%04X \r\n", T0_degC_OUT_x8);

//H1_rH
uint8_t reg_T1_degC = 0x33;
uint16_t T1_degC_OUT_x8 = 0;

HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_T1_degC, 1, &T1_degC_OUT_x8,
                    1, 50);
printf("[raw]T1_degC_OUT_x8 data : 0x%02X \r\n", T1_degC_OUT_x8);

T1_degC_OUT_x8 = T1_degC_OUT_x8 | T_0_1_msb_data[1] << 8;

printf("[raw] (add msb) T1_degC_OUT_x8 _data  : 0x%04X \r\n", T1_degC_OUT_x8);



// 3. T0_OUT T1_OUT 구하기

// T0_OUT
int8_t read_T0_OUT_data[2] = {0};
uint8_t reg_T0_OUT_address = 0x3C;

int16_t T0_OUT = 0;

HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T0_OUT_address,1,&read_T0_OUT_data[0],1,50);
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T0_OUT_address+1,1,&read_T0_OUT_data[1],1,50);



  printf("[raw]T0_OUT_L data : 0x%02X \r\n", read_T0_OUT_data[0]);
  printf("[raw]T0_OUT_H data : 0x%02X \r\n", read_T0_OUT_data[1]);

  T0_OUT = read_T0_OUT_data[1] | read_T0_OUT_data[0] << 8;

printf("[raw]T0_OUT data : 0x%04X \r\n", T0_OUT);

// T1_OUT

int8_t read_T1_OUT_data[2] = {0};
uint8_t reg_T1_OUT_address = 0x3E;

int16_t T1_OUT = 0;

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T1_OUT_address,1,&read_T1_OUT_data[0],1,50);
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T1_OUT_address+1,1,&read_T1_OUT_data[1],1,50);



  printf("[raw]T1_OUT_L data : 0x%02X \r\n", read_T1_OUT_data[0]);
  printf("[raw]T1_OUT_H data : 0x%02X \r\n", read_T1_OUT_data[1]);

  T1_OUT = read_T1_OUT_data[1] | read_T1_OUT_data[0] << 8;

  printf("[raw]T1_OUT data : 0x%02X \r\n", T1_OUT);

  // data set
  int8_t T0_degC_OUT = T0_degC_OUT_x8 / 8;
  int8_t T1_degC_OUT = T1_degC_OUT_x8 / 8;

  //result
  int8_t T_degC_OUT = 0;

  T_degC_OUT = T0_degC_OUT + (T_OUT-T0_OUT)*(T1_degC_OUT-T0_degC_OUT)/(T1_OUT-T0_OUT);

  printf("[dec]Temperature data : %d °C  <<<<<\r\n", T_degC_OUT);

  // 계산법 2
  int8_t T_degC_OUT_x8 = 0;
  T_degC_OUT_x8 = T0_degC_OUT_x8 + (T_OUT-T0_OUT)*(T1_degC_OUT_x8-T0_degC_OUT_x8)/(T1_OUT-T0_OUT);
//   H_rH_OUT_x2 = H_rH_OUT_x2 / 2;
    printf("[dec]Temperature data x8  : %d  \r\n", T_degC_OUT_x8);


  printf(" \r\n");
}

void read_is_sensor_enable(void) {
  uint8_t status_reg = 0x27;
  uint8_t status_data = 0;
  uint8_t T_DA = 0;
  uint8_t H_DA = 0;


  HAL_I2C_Mem_Read(&hi2c1, address_HTS221, status_reg, 1, &status_data, 1, 50);
  T_DA = status_data & 0x01;
  H_DA = (status_data>>1) & 0x01;


  printf("[Is_enable]Temperature (enable : 1 / disable :0)  : %d  \r\n", T_DA);
  printf("[Is_enable]Humid (enable : 1 / disable :0)  : %d  \r\n", H_DA);


  printf("====================================================================== \r\n");

  
}

void write_is_sensor_enable(void) {

    uint8_t AV_CONF_reg = 0x10;
  uint8_t AV_CONF_data = 0x2D;

  HAL_Delay(100);
  HAL_I2C_Mem_Write(&hi2c1, address_HTS221, AV_CONF_reg, 1, &AV_CONF_data, 1, 50);
  
  uint8_t CTRL1_reg = 0x20;
  uint8_t CTRL1_data = 0x86;

  uint8_t read_CTRL1_data = 0;

  uint8_t CTRL2_reg = 0x21;
  uint8_t CTRL2_data = 0x03;
      HAL_I2C_Mem_Write(&hi2c1, address_HTS221, CTRL2_reg, 1, &CTRL2_data, 1, 50);

  HAL_Delay(100);
//     HAL_I2C_Mem_Read(&hi2c1, address_HTS221, CTRL2_reg, 1, &read_CTRL1_data, 1, 50);


  HAL_I2C_Mem_Write(&hi2c1, address_HTS221, CTRL1_reg, 1, &CTRL1_data, 1, 50);

  HAL_Delay(100);
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221, CTRL1_reg, 1, &read_CTRL1_data, 1,
                   50);



  


  printf("[Is_enable] Write (enable : 1 / disable :0) 0x87 =?   : %#x \r\n",read_CTRL1_data);
}

// input_AVGT , input_AVGH : 0x00~0x07
void set_AV_CONF(uint8_t input_AVGT, uint8_t input_AVGH) {
  uint8_t AV_CONF_reg = 0x10;
  uint8_t AV_CONF_data = (input_AVGT << 3) | input_AVGH ;
  uint8_t read_AV_CONF_data = 0;

  HAL_I2C_Mem_Write(&hi2c1, address_HTS221, AV_CONF_reg, 1, &AV_CONF_data, 1,
                    50);
  HAL_Delay(100);
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221 + 1, AV_CONF_reg, 1,
                   &read_AV_CONF_data, 1, 50);

  printf("[raw] read_AV_CONF_data  : %#x \r\n",read_AV_CONF_data);
}

// input_PD , input_BUD, input_ODR1 , input_ODR0 : 0x00~0x01
void set_CTRL_REG1(uint8_t input_PD, uint8_t input_BUD ,uint8_t input_ODR1,uint8_t input_ODR0) {
  uint8_t CTRL1_reg = 0x20;
  uint8_t CTRL1_data = (input_PD <<7) |(input_BUD <<2)| (input_ODR1 << 1) | input_ODR0 ;
  uint8_t read_CTRL1_data = 0;

  HAL_I2C_Mem_Write(&hi2c1, address_HTS221, CTRL1_reg, 1, &CTRL1_data, 1,
                    50);
  HAL_Delay(100);
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221 + 1, CTRL1_reg, 1,
                   &read_CTRL1_data, 1, 50);

  printf("[raw] read_CTRL1_data  : %#x \r\n",read_CTRL1_data);
}

// input_Boot,input_Heater, input_OneShot  : 0x00~0x01
void set_CTRL_REG2(uint8_t input_Boot, uint8_t input_Heater ,uint8_t input_OneShot) {
  uint8_t CTRL2_reg = 0x21;
  uint8_t CTRL2_data = (input_Boot <<7) |(input_Heater <<1)|  input_OneShot ;
  uint8_t read_CTRL2_data = 0;

  HAL_I2C_Mem_Write(&hi2c1, address_HTS221, CTRL2_reg, 1, &CTRL2_data, 1,
                    50);
  HAL_Delay(100);
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221 + 1, CTRL2_reg, 1,
                   &read_CTRL2_data, 1, 50);

  printf("[raw] read_CTRL2_data  : %#x \r\n",read_CTRL2_data);
}

// input_Boot,input_Heater, input_OneShot  : 0x00~0x01
void set_CTRL_REG3(uint8_t input_DRDY_H_L, uint8_t input_PP_OD ,uint8_t input_DRDY_EN) {
  uint8_t CTRL3_reg = 0x22;
  uint8_t CTRL3_data = (input_DRDY_H_L <<7) |(input_PP_OD <<6)|  (input_DRDY_EN<<2) ;
  uint8_t read_CTRL3_data = 0;

  HAL_I2C_Mem_Write(&hi2c1, address_HTS221, CTRL3_reg, 1, &CTRL3_data, 1,
                    50);
  HAL_Delay(100);
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221 + 1, CTRL3_reg, 1,
                   &read_CTRL3_data, 1, 50);

  printf("[raw] read_CTRL3_data  : %#x \r\n",read_CTRL3_data);
}

void read_WHO_AM_I(void) {
  uint8_t WHO_reg = 0x0F;
  uint8_t read_WHO_data = 0;

//  HAL_I2C_Mem_Write(&hi2c1, address_HTS221, CTRL3_reg, 1, &CTRL3_data, 1,
//                    50);
  HAL_Delay(100);
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221 + 1, WHO_reg, 1,
                   &read_WHO_data, 1, 50);

  printf("[raw] read_WHO_data  : %#x \r\n",read_WHO_data);
}

void init_HTS221(void) {
  read_WHO_AM_I();
  HAL_Delay(100);

  set_CTRL_REG1(0x01, 0x00, 0x00, 0x00);
  HAL_Delay(100);
    set_AV_CONF(0x05, 0x05);
  HAL_Delay(100);
//   set_CTRL_REG2(0x00, 0x00, 0x00);
//   HAL_Delay(100);
//   set_CTRL_REG3(0x00, 0x00, 0x00);
//   HAL_Delay(100);
  read_is_sensor_enable();
    HAL_Delay(100);
}

void print_all_reg(void) {


  uint8_t read_data_raw = 0;
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221, 0x28, 1, &read_data_raw, 1, 50);
  printf(" 0x28 : 0x%02x \r\n", read_data_raw);

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221, 0x29, 1, &read_data_raw, 1, 50);
  printf(" 0x29 : 0x%02x \r\n", read_data_raw);

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221, 0x2A, 1, &read_data_raw, 1, 50);
  printf(" 0x2A : 0x%02x \r\n", read_data_raw);

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221, 0x2B, 1, &read_data_raw, 1, 50);
  printf(" 0x2B : 0x%02x \r\n", read_data_raw);

  for (int i = 0; i < 16; i++) {

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221, 0x30+i, 1, &read_data_raw, 1, 50);
  printf(" 0x%02x : 0x%02x \r\n",0x30+i , read_data_raw);
  
  }
}

void cal_temperature_gradient(_temperature *t) {
  // 2. T0_degC x8, T8_degC x8 값 구하기
// T0 1 msb 데이터 세팅 하기
uint8_t reg_T_degC_msb = 0x35;
uint8_t T_0_1_msb = 0;
uint8_t T_0_1_msb_data[2] = {0};
HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_T_degC_msb, 1, &T_0_1_msb,
                    1, 50);
printf("[raw]T_0_1_msb _data : 0x%02X \r\n", T_0_1_msb);
// 배열에 대입 후 역할 넣기 
// T0
T_0_1_msb_data[0] = T_0_1_msb & 0x03;
// T1
T_0_1_msb_data[1] = (T_0_1_msb & 0x0C) >> 2;

//H0_rH
uint8_t reg_T0_degC = 0x32;
uint16_t T0_degC_OUT_x8 = 0;

HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_T0_degC, 1, &T0_degC_OUT_x8,
                    1, 50);
printf("[raw]T0_degC_OUT_x8 _data : 0x%02X \r\n", T0_degC_OUT_x8);


T0_degC_OUT_x8 = T0_degC_OUT_x8 | T_0_1_msb_data[0] << 8;

printf("[raw] (add msb) T0_degC_OUT_x8 _data  : 0x%04X \r\n", T0_degC_OUT_x8);

//H1_rH
uint8_t reg_T1_degC = 0x33;
uint16_t T1_degC_OUT_x8 = 0;

HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_T1_degC, 1, &T1_degC_OUT_x8,
                    1, 50);
printf("[raw]T1_degC_OUT_x8 data : 0x%02X \r\n", T1_degC_OUT_x8);

T1_degC_OUT_x8 = T1_degC_OUT_x8 | T_0_1_msb_data[1] << 8;

printf("[raw] (add msb) T1_degC_OUT_x8 _data  : 0x%04X \r\n", T1_degC_OUT_x8);



// 3. T0_OUT T1_OUT 구하기

// T0_OUT
int8_t read_T0_OUT_data[2] = {0};
uint8_t reg_T0_OUT_address = 0x3C;

int16_t T0_OUT = 0;

HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T0_OUT_address,1,&read_T0_OUT_data[0],1,50);
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T0_OUT_address+1,1,&read_T0_OUT_data[1],1,50);



  printf("[raw]T0_OUT_L data : 0x%02X \r\n", read_T0_OUT_data[0]);
  printf("[raw]T0_OUT_H data : 0x%02X \r\n", read_T0_OUT_data[1]);

  T0_OUT = read_T0_OUT_data[1] << 8| (read_T0_OUT_data[0] & 0x00FF) ;

  printf("[raw]T0_OUT data : 0x%04X \r\n", T0_OUT);
  printf("[dec]T1_OUT data : %d \r\n", T0_OUT);

// T1_OUT

int8_t read_T1_OUT_data[2] = {0};
uint8_t reg_T1_OUT_address = 0x3E;

int16_t T1_OUT = 0;

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T1_OUT_address,1,&read_T1_OUT_data[0],1,50);
HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T1_OUT_address+1,1,&read_T1_OUT_data[1],1,50);



  printf("[raw]T1_OUT_L data : 0x%02X \r\n", read_T1_OUT_data[0]);
  printf("[raw]T1_OUT_H data : 0x%02X \r\n", read_T1_OUT_data[1]);

  T1_OUT = read_T1_OUT_data[1]  << 8 | (read_T1_OUT_data[0] & 0x00FF);

  printf("[raw]T1_OUT data : 0x%04X \r\n", T1_OUT);
  printf("[dec]T1_OUT data : %d \r\n", T1_OUT);

  // data set
  float T0_degC_OUT = T0_degC_OUT_x8 / 8.0f;
  float T1_degC_OUT = T1_degC_OUT_x8 / 8.0f;
  printf("[dec]T0_degC_OUT data : %.3f \r\n", T0_degC_OUT);
  printf("[dec]T1_degC_OUT data : %.3f \r\n", T1_degC_OUT);

  //result
  float T_degC_OUT = 0;

  float T_degC_gradient = (T1_degC_OUT - T0_degC_OUT) / (T1_OUT - T0_OUT);

  t->gradient_temperature = T_degC_gradient;
  t->init_temperature = T0_degC_OUT;
  t->init_OUT = T0_OUT;

//   T_degC_OUT = T0_degC_OUT + (T_OUT-T0_OUT)*(T1_degC_OUT-T0_degC_OUT)/(T1_OUT-T0_OUT);

  printf("[dec]Temperature data : %d °C  <<<<<\r\n", T_degC_OUT);
}

void cal_humid_gradient(_humid *h) {
  printf("[=======================================================================]  \r\n");
  // 2. H0_rH x2, H1_rH x2 값 구하기 -> unsigned 사용 
//H0_rH
    uint8_t reg_H0_rH = 0x30;
    uint8_t H0_rH_OUT_x2 = 0;

    HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_H0_rH, 1, &H0_rH_OUT_x2,
                        1, 50);
    printf("[raw]read_H0_rH_data : 0x%02X \r\n", H0_rH_OUT_x2);

    //H1_rH
    uint8_t reg_H1_rH = 0x31;
    uint8_t H1_rH_OUT_x2 = 0;

    HAL_I2C_Mem_Read(&hi2c1, address_HTS221, reg_H1_rH, 1, &H1_rH_OUT_x2,
                        1, 50);
    printf("[raw]read_H1_rH_data : 0x%02X \r\n", H1_rH_OUT_x2);

// 3. H0_OUT H1_OUT 구하기

// H0_OUT
    uint8_t read_H0_OUT_data[2] = {0};
    uint8_t reg_H0_OUT_address = 0x36;

  int16_t H0_OUT = 0;

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_H0_OUT_address,1,&read_H0_OUT_data[0],1,50);
    HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_H0_OUT_address+1,1,&read_H0_OUT_data[1],1,50);



  printf("[raw]HUMIDITY_OUT_L data(H0_OUT) : 0x%02X \r\n", read_H0_OUT_data[0]);
  printf("[raw]HUMIDITY_OUT_H data(H0_OUT) : 0x%02X \r\n", read_H0_OUT_data[1]);

  H0_OUT = read_H0_OUT_data[1]  << 8 | read_H0_OUT_data[0];

  printf("[raw]H0_OUTdata : 0x%04X \r\n", H0_OUT);
    printf("[dec]H0_OUTdata : %d \r\n", H0_OUT);


// H1_OUT

    uint8_t read_H1_OUT_data[2] = {0};
    uint8_t reg_H1_OUT_address = 0x3A;
// 음수 되어서 int16_t -> uint16_t
  int16_t H1_OUT = 0;

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_H1_OUT_address,1,&read_H1_OUT_data[0],1,50);
    HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_H1_OUT_address+1,1,&read_H1_OUT_data[1],1,50);



  printf("[raw]H1_OUT_L data : 0x%02X \r\n", read_H1_OUT_data[0]);
  printf("[raw]H1_OUT_H data : 0x%02X \r\n", read_H1_OUT_data[1]);
  printf("[raw]read_H1_OUT_data[1] << 8 data : 0x%02X \r\n", read_H1_OUT_data[1] << 8);

  H1_OUT = (read_H1_OUT_data[1] << 8) | (read_H1_OUT_data[0]);
  //   H1_OUT = read_H1_OUT_data[1] *256 + read_H1_OUT_data[0];

//   H1_OUT = H1_OUT&0x0000FFFF;


  printf("[raw]H1_OUTdata : 0x%04X \r\n", H1_OUT);
  printf("[dec]H1_OUTdata : %d \r\n", H1_OUT);

  // data set
  float H0_rH_OUT = H0_rH_OUT_x2 / 2.0f;
//   H0_rH_OUT = H0_rH_OUT_x2 / 2;
  float H1_rH_OUT = H1_rH_OUT_x2 / 2.0f;
  //float으로 변경
  printf("[dec]H0_rH_OUT : %.1f \r\n", H0_rH_OUT);
    printf("[dec]H1_rH_OUT : %.1f \r\n", H1_rH_OUT);




  // result , 소수 음수 출력?
    float gradient_humid_temp = 0;
    int8_t gradient_humid_temp_r = 0;
    gradient_humid_temp = (float)((H1_rH_OUT - H0_rH_OUT) / (H1_OUT - H0_OUT));

    gradient_humid_temp_r = (H1_OUT - H0_OUT)/(H1_rH_OUT - H0_rH_OUT);
  printf("\r\n");

  printf("[dec]gradient_humid_temp_r :%d \r\n", gradient_humid_temp_r);
  printf("[float]gradient_humid :%.3f \r\n", gradient_humid_temp);
  printf("[dec]initial humid H0_rH_OUT  :%.1f \r\n", H0_rH_OUT);

  h->init_humid = H0_rH_OUT;
  h->gradient_humid = gradient_humid_temp;
  h->init_OUT = H0_OUT;

    return 0;
}

void result_humid(_humid *h , _cur *c) {

  printf("====================================================================== \r\n");

  uint8_t read_H_OUT_data[2] = {0};

  int16_t H_OUT = 0;

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_humid_address,1,&read_H_OUT_data[0],1,50);
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_humid_address+1,1,&read_H_OUT_data[1],1,50);



  printf("[raw]HUMIDITY_OUT_L (H_OUT) data : 0x%02X \r\n", read_H_OUT_data[0]);
  printf("[raw]HUMIDITY_OUT_H (H_OUT) data : 0x%02X \r\n", read_H_OUT_data[1]);

  H_OUT = read_H_OUT_data[1] << 8 | read_H_OUT_data[0];

  printf("[raw]H_OUTdata : 0x%04X \r\n", H_OUT);

  float t_init = h->init_humid;
  float t_grad = h->gradient_humid;
  float t_init_OUT = h->init_OUT;

  float result = 0;
  result = t_init + (H_OUT-t_init_OUT) * t_grad;

  c->cur_humid = result;

  printf("[dec ]cur_humid  data : %f <<<<<< \r\n", result);
}

void result_temperature(_temperature *t , _cur *c) {

  uint8_t read_T_OUT_data[2] = {0};
  uint8_t reg_T_OUT_address = 0x2A;

  int16_t T_OUT = 0;

  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T_OUT_address,1,&read_T_OUT_data[0],1,50);
  HAL_I2C_Mem_Read(&hi2c1, address_HTS221,reg_T_OUT_address+1,1,&read_T_OUT_data[1],1,50);

  printf("[raw]TEMPERATURE_OUT_L data : 0x%02X \r\n", read_T_OUT_data[0]);
  printf("[raw]TEMPERATURE_OUT_H data : 0x%02X \r\n", read_T_OUT_data[1]);

  T_OUT = read_T_OUT_data[1]  << 8 | read_T_OUT_data[0];

  printf("[raw]T_OUTdata : 0x%04X \r\n", T_OUT);
  printf("[dec]T_OUTdata : %d \r\n", T_OUT);

  float t_init = t->init_temperature;
  float t_grad = t->gradient_temperature;
  float t_init_OUT = t->init_OUT;
  

  float result = 0;
  result = t_init + (T_OUT - t_init_OUT) * t_grad;

  c->cur_temperature = result;

  printf("[dec ]cur_temperature  data : %f <<<<<< \r\n", result);

  
}