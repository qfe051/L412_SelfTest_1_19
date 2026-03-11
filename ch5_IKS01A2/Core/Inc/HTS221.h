#include "main.h"


#ifndef HTS221_H
#define HTS221_H

void read_temper_HTS221(void);

void read_temperature_HTS221(void);
void read_humid_HTS221(void);


void read_is_sensor_enable(void);
void init_HTS221(void);


typedef struct {
  float cur_humid;
  float cur_temperature;
} _cur;



// 기울기 여기서 관리하기
// 습도 관련
typedef struct {
  float gradient_humid;
  float init_humid;
  float init_OUT;
} _humid ;


void cal_humid_gradient(_humid *h);

// 온도 관련

typedef struct {
  float gradient_temperature;
  float init_temperature;
  float init_OUT;
} _temperature ;


void cal_temperature_gradient(_temperature *t);

void result_humid(_humid *h , _cur *c);

// 여기서 정의 해줘야 하는 이유?
// main.c랑 여기랑 더 적절한 위치 찾기! 
extern I2C_HandleTypeDef hi2c1;








#endif