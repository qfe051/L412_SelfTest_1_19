/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SET_OUT1_PIN() \
  HAL_GPIO_WritePin(Out_1_GPIO_Port, Out_1_Pin, GPIO_PIN_SET); \
  HAL_GPIO_WritePin(Out_2_GPIO_Port, Out_2_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_3_GPIO_Port, Out_3_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_5_GPIO_Port, Out_5_Pin, GPIO_PIN_RESET);
#define SET_OUT2_PIN() \
  HAL_GPIO_WritePin(Out_1_GPIO_Port, Out_1_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_2_GPIO_Port, Out_2_Pin, GPIO_PIN_SET); \
  HAL_GPIO_WritePin(Out_3_GPIO_Port, Out_3_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_5_GPIO_Port, Out_5_Pin, GPIO_PIN_RESET);
#define SET_OUT3_PIN() \
  HAL_GPIO_WritePin(Out_1_GPIO_Port, Out_1_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_2_GPIO_Port, Out_2_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_3_GPIO_Port, Out_3_Pin, GPIO_PIN_SET); \
  HAL_GPIO_WritePin(Out_5_GPIO_Port, Out_5_Pin, GPIO_PIN_RESET);
#define SET_OUT5_PIN() \
  HAL_GPIO_WritePin(Out_1_GPIO_Port, Out_1_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_2_GPIO_Port, Out_2_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_3_GPIO_Port, Out_3_Pin, GPIO_PIN_RESET); \
  HAL_GPIO_WritePin(Out_5_GPIO_Port, Out_5_Pin, GPIO_PIN_SET);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//add
#include <stdio.h>
#include <string.h>

static uint8_t last_key_id = 0;   // 0이면 없음

static void uart_print(const char *s)
{
  HAL_UART_Transmit(&hlpuart1, (uint8_t*)s, (uint16_t)strlen(s), HAL_MAX_DELAY);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */

  //add
  char buf__init[32];
  snprintf(buf__init, sizeof(buf__init), "Start Push Switch\r\n");
  uart_print(buf__init);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* USER CODE BEGIN 3 */
    // char buf_1[32] = {0};
    // char buf_2[32] = {0};
    // char buf_3[32] = {0};
    // char buf_4[32] = {0};

    SET_OUT1_PIN()
    HAL_Delay(5);
    {
      uint8_t row = 0;
      uint8_t col = 0xFF; //입력 안한 경우. if문 사용 용도

      if (HAL_GPIO_ReadPin(In_1_GPIO_Port,In_1_Pin) == GPIO_PIN_SET)
        col = 0;
      else if (HAL_GPIO_ReadPin(In_2_GPIO_Port,In_2_Pin) == GPIO_PIN_SET)
        col = 1;
      else if (HAL_GPIO_ReadPin(In_3_GPIO_Port,In_3_Pin) == GPIO_PIN_SET)
        col = 2;
      else if (HAL_GPIO_ReadPin(In_4_GPIO_Port,In_4_Pin) == GPIO_PIN_SET)
        col = 3;

      if(col != 0xFF)
      {
        uint8_t key_id = row * 4 + col + 1; //키 번호 

        // if (key_id != last_key_id){
        char buf_1[32];
        snprintf(buf_1, sizeof(buf_1), "S%d pressed \r\n\n", key_id);
        uart_print(buf_1);

        
        //   last_key_id = key_id ;
        // }
      }
    }

    HAL_Delay(10);

    SET_OUT2_PIN()
    HAL_Delay(5);
    {
      uint8_t row = 1;
      uint8_t col = 0xFF; //입력 안한 경우. if문 사용 용도

      if (HAL_GPIO_ReadPin(In_1_GPIO_Port,In_1_Pin) == GPIO_PIN_SET)
        col = 0;
      else if (HAL_GPIO_ReadPin(In_2_GPIO_Port,In_2_Pin) == GPIO_PIN_SET)
        col = 1;
      else if (HAL_GPIO_ReadPin(In_3_GPIO_Port,In_3_Pin) == GPIO_PIN_SET)
        col = 2;
      else if (HAL_GPIO_ReadPin(In_4_GPIO_Port,In_4_Pin) == GPIO_PIN_SET)
        col = 3;

      if(col != 0xFF)
      {
        uint8_t key_id = row * 4 + col + 1; //키 번호 

        char buf_2[32];
        snprintf(buf_2, sizeof(buf_2), "S%d pressed \r\n\n", key_id);
        uart_print(buf_2);

      }
    }

    HAL_Delay(10);

    SET_OUT3_PIN()
    HAL_Delay(5);
    {
      uint8_t row = 2;
      uint8_t col = 0xFF; //입력 안한 경우. if문 사용 용도

      if (HAL_GPIO_ReadPin(In_1_GPIO_Port,In_1_Pin) == GPIO_PIN_SET)
        col = 0;
      else if (HAL_GPIO_ReadPin(In_2_GPIO_Port,In_2_Pin) == GPIO_PIN_SET)
        col = 1;
      else if (HAL_GPIO_ReadPin(In_3_GPIO_Port,In_3_Pin) == GPIO_PIN_SET)
        col = 2;
      else if (HAL_GPIO_ReadPin(In_4_GPIO_Port,In_4_Pin) == GPIO_PIN_SET)
        col = 3;

      if(col != 0xFF)
      {
        uint8_t key_id = row * 4 + col + 1; //키 번호 

        char buf_3[32];
        snprintf(buf_3, sizeof(buf_3), "S%d pressed\r\n\n", key_id);
        uart_print(buf_3);

      }
    }

    HAL_Delay(10);



    SET_OUT5_PIN()
    HAL_Delay(5);
    {
      uint8_t row = 3;
      uint8_t col = 0xFF; //입력 안한 경우. if문 사용 용도

      if (HAL_GPIO_ReadPin(In_1_GPIO_Port,In_1_Pin) == GPIO_PIN_SET)
        col = 0;
      else if (HAL_GPIO_ReadPin(In_2_GPIO_Port,In_2_Pin) == GPIO_PIN_SET)
        col = 1;
      else if (HAL_GPIO_ReadPin(In_3_GPIO_Port,In_3_Pin) == GPIO_PIN_SET)
        col = 2;
      else if (HAL_GPIO_ReadPin(In_4_GPIO_Port,In_4_Pin) == GPIO_PIN_SET)
        col = 3;

      if(col != 0xFF)
      {
        uint8_t key_id = row * 4 + col + 1; //키 번호 

        char buf_4[32];
        snprintf(buf_4, sizeof(buf_4), "S%d pressed\r\n\n", key_id);
        uart_print(buf_4);

      }
    }
    // if (buf_1[0] || buf_2[0] || buf_3[0] || buf_4[0]) 
    // {
    //   if (buf_1[0]) uart_print(buf_1);
    //   if (buf_2[0]) uart_print(buf_2);
    //   if (buf_3[0]) uart_print(buf_3);
    //   if (buf_4[0]) uart_print(buf_4);
    //   uart_print("\r\n");
    // }

    HAL_Delay(10);


    // char buf[32];
    // snprintf(buf, sizeof(buf), "\r\n\n");
    // uart_print(buf);
  /* USER CODE END 3 */
  }

}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SMPS_EN_Pin|SMPS_V1_Pin|SMPS_SW_Pin|Out_2_Pin
                          |Out_3_Pin|Out_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Out_5_GPIO_Port, Out_5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Out_1_GPIO_Port, Out_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SMPS_EN_Pin SMPS_V1_Pin SMPS_SW_Pin */
  GPIO_InitStruct.Pin = SMPS_EN_Pin|SMPS_V1_Pin|SMPS_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SMPS_PG_Pin */
  GPIO_InitStruct.Pin = SMPS_PG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SMPS_PG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : In_2_Pin In_1_Pin In_3_Pin In_4_Pin */
  GPIO_InitStruct.Pin = In_2_Pin|In_1_Pin|In_3_Pin|In_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Out_5_Pin */
  GPIO_InitStruct.Pin = Out_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Out_5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Out_2_Pin Out_3_Pin Out_4_Pin */
  GPIO_InitStruct.Pin = Out_2_Pin|Out_3_Pin|Out_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Out_1_Pin */
  GPIO_InitStruct.Pin = Out_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Out_1_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

static inline void set_row(uint8_t row)
{
    HAL_GPIO_WritePin(Out_1_GPIO_Port, Out_1_Pin, (row == 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Out_2_GPIO_Port, Out_2_Pin, (row == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Out_3_GPIO_Port, Out_3_Pin, (row == 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Out_5_GPIO_Port, Out_5_Pin, (row == 3) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
