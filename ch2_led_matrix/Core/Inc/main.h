/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define C_8_Pin GPIO_PIN_0
#define C_8_GPIO_Port GPIOC
#define C_5_Pin GPIO_PIN_1
#define C_5_GPIO_Port GPIOC
#define R_6_Pin GPIO_PIN_2
#define R_6_GPIO_Port GPIOC
#define R_4_Pin GPIO_PIN_3
#define R_4_GPIO_Port GPIOC
#define SMPS_EN_Pin GPIO_PIN_4
#define SMPS_EN_GPIO_Port GPIOA
#define SMPS_V1_Pin GPIO_PIN_5
#define SMPS_V1_GPIO_Port GPIOA
#define SMPS_PG_Pin GPIO_PIN_6
#define SMPS_PG_GPIO_Port GPIOA
#define SMPS_SW_Pin GPIO_PIN_7
#define SMPS_SW_GPIO_Port GPIOA
#define C_6_Pin GPIO_PIN_10
#define C_6_GPIO_Port GPIOB
#define C_4_Pin GPIO_PIN_13
#define C_4_GPIO_Port GPIOB
#define C_2_Pin GPIO_PIN_14
#define C_2_GPIO_Port GPIOB
#define R_2_Pin GPIO_PIN_15
#define R_2_GPIO_Port GPIOB
#define C_3_Pin GPIO_PIN_7
#define C_3_GPIO_Port GPIOC
#define C_1_Pin GPIO_PIN_8
#define C_1_GPIO_Port GPIOA
#define R_3_Pin GPIO_PIN_11
#define R_3_GPIO_Port GPIOA
#define R_1_Pin GPIO_PIN_12
#define R_1_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define R_8_Pin GPIO_PIN_15
#define R_8_GPIO_Port GPIOA
#define C_7_Pin GPIO_PIN_3
#define C_7_GPIO_Port GPIOB
#define R_7_Pin GPIO_PIN_5
#define R_7_GPIO_Port GPIOB
#define R_5_Pin GPIO_PIN_6
#define R_5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
