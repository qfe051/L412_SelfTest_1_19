/**
  ******************************************************************************
  * @file    bsp_ip_conf.h
  * @author  MEMS Software Solutions Team
  * @brief   BSP IP configuration file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_IP_CONF_H__
#define __BSP_IP_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern TIM_HandleTypeDef htim5;
#define BSP_IP_TIM_HandleAB   htim5
#define BSP_IP_TIM_InitAB     MX_TIM5_Init

extern TIM_HandleTypeDef htim1;
#define BSP_IP_TIM_HandleAL   htim1
#define BSP_IP_TIM_InitAL     MX_TIM1_Init

/* External variables --------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __BSP_IP_CONF_H__ */
