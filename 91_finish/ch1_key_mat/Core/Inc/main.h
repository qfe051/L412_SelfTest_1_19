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
#define SMPS_EN_Pin GPIO_PIN_4
#define SMPS_EN_GPIO_Port GPIOA
#define SMPS_V1_Pin GPIO_PIN_5
#define SMPS_V1_GPIO_Port GPIOA
#define SMPS_PG_Pin GPIO_PIN_6
#define SMPS_PG_GPIO_Port GPIOA
#define SMPS_SW_Pin GPIO_PIN_7
#define SMPS_SW_GPIO_Port GPIOA
#define In_2_Pin GPIO_PIN_13
#define In_2_GPIO_Port GPIOB
#define In_1_Pin GPIO_PIN_14
#define In_1_GPIO_Port GPIOB
#define Out_5_Pin GPIO_PIN_7
#define Out_5_GPIO_Port GPIOC
#define Out_2_Pin GPIO_PIN_8
#define Out_2_GPIO_Port GPIOA
#define Out_3_Pin GPIO_PIN_11
#define Out_3_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Out_4_Pin GPIO_PIN_15
#define Out_4_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define Out_1_Pin GPIO_PIN_6
#define Out_1_GPIO_Port GPIOB
#define In_3_Pin GPIO_PIN_7
#define In_3_GPIO_Port GPIOB
#define In_4_Pin GPIO_PIN_8
#define In_4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
