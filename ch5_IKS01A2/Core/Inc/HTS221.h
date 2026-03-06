#include "main.h"


#ifndef HTS221_H
#define HTS221_H

void read_temper_HTS221(void);

void read_temperature_HTS221(void);
void read_humid_HTS221(void);


void read_is_sensor_enable(void);
void init_HTS221(void);

// 여기서 정의 해줘야 하는 이유?
// main.c랑 여기랑 더 적절한 위치 찾기! 
extern I2C_HandleTypeDef hi2c1;








#endif