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
// add
#include <stdio.h>
#include <stm32l412xx.h>
#include <string.h>
#include <unistd.h>

#include "ds1302.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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


// UART로 출력 보내기 위한 함수
int _write(int file, char *ptr, int len) {
  if (file == STDOUT_FILENO) {
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
  }
  return -1;
}

// 함수 선언
// 1. DAT (I/O) 변경해주기
// 출력설정 함수
void DS1302_SetOutput(void) {
  // {0} 집어넣는 작업 하는 이유는? 
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_Init_C;
}

// 입력 설정 함수
void DS1302_SetInput(void) {
  // {0} 집어넣는 작업 하는 이유는?
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_Init_C;
}

// 2. 1바이트 쓰기 / 읽기 단위로 진행

// data는 0,1 뿐이라서
// MCU -> DS1302 로 write
// DS1302에서 반환하는 값이 없으니, 'void' 형식으로 진행
void DS1302_WriteByte(uint8_t data) {
  // 출력모드 세팅
  DS1302_SetOutput();

  for (int i = 0; i < 8; i++) {

    // LSB (0번비트 부터!)
    // 가장 오른쪽 비트와 비교
    // 1 -> 0000 0001 과 & 연산하여 1인지 확인 
    if ((data & 1)==1) {
      // data 가 1인 경우에만 set으로 설정 ?
      HAL_GPIO_WritePin(GPIOC, DAT_Pin, GPIO_PIN_SET);
    }
    else {
      HAL_GPIO_WritePin(GPIOC, DAT_Pin, GPIO_PIN_RESET);
    }
    data >>= 1; // 비트 밀어넣기 (시프트)
    CLK_ON;
    CLK_OFF;
  }
}
// uint8_t -> 8bit data를 받아와야 함
// void -> 입력값 없이 그냥 읽기만 해도 됨
uint8_t DS1302_ReadByte(void) {
  // data 받아오기 전 초기화
  uint8_t data = 0;
  DS1302_SetInput();

  for (int i = 0; i < 8; i++) {
    CLK_ON;

    // LSB 부터 데이터 읽기
    // 값이 1인 데이터 읽어오기
    // GPIO_PIN_SET 대신 1 집어넣는 것이 안정적인지 확인
    if (HAL_GPIO_ReadPin(GPIOC, DAT_Pin) == GPIO_PIN_SET) {
      data |= (1<<i);
    }
    // CLK 핀 토글 생성
    // 토글 안되면 다른 방법도 보기
    // HAL_GPIO_TogglePin(GPIOC, CLK_Pin);

    // set reset
    CLK_OFF;
  }
  return data;
}

// 3. 레지스터 단위 R/W
// 마찬가지로 쓰기 작업이라 - return 받을 데이터 없음
void DS1302_WriteReg(uint8_t reg, uint8_t data) {
  // CE - High로 올려 통신 시작
  CE_ON;
  DS1302_WriteByte(reg); // 주소 -> 시작 시점
  DS1302_WriteByte(data); // 데이터
  // CE - Low로 통신 종료
  CE_OFF;
}

// reg 삽입할 공간만 추가
uint8_t DS1302_ReadReg(uint8_t reg) {
  uint8_t data;
  CE_ON;
  DS1302_WriteByte(reg);  // 주소 -> 시작 시점 전송 먼저 해야!
  data = DS1302_ReadByte(); // 데이터 저장(수신?)
  // CE - Low로 통신 종료
  CE_OFF;

  return data;
}

uint8_t sec_min(uint8_t data) {
  uint8_t sec_1 = 0;
  uint8_t sec_10 = 0;
  uint8_t sec = 0;

  sec_1 = (data & 0x0F);
  sec_10 = ((data >> 4) & 0x07);
  sec = 10 * sec_10 + sec_1;

  return sec;
}

uint8_t hours(uint8_t data) {
  uint8_t hours_1 = 0;
  uint8_t hours_10 = 0;
  uint8_t hours = 0;

  hours_1 = (data & 0x0F);
  hours_10 = ((data >> 4) & 0x03);
  hours = 10 * hours_10 + hours_1;

  return hours;
}

uint8_t years(uint8_t data) {
  uint8_t years_1 = 0;
  uint8_t years_10 = 0;
  uint8_t years = 0;

  years_1 = (data & 0x0F);
  years_10 = ((data >> 4) & 0x0F);
  years = 10 * years_10 + years_1;

  return years;
}

const char* days(uint8_t data) {
  int temp = 0;
  temp = data++;

  switch (temp) {
  case 1:
    return "Mon";
  case 2:
    return "Tue";
  case 3:
    return "Wed";
  case 4:
    return "Thu";
  case 5:
    return "Fri";
  case 6:
    return "Sat";
  case 7:
    return "Sun";
  }

  return days;
}

// uint8_t sec_10(uint8_t data) {
//   uint8_t sec_10 = 0;
//   sec_10 = ((data >> 4) & 0x07);

//   return sec_10;
// }
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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

  // printf("UART Test \r\n");

  // 핀 초기화 (RST , CLK )
  CE_OFF;
  CLK_OFF;

  // uart 테스트 출력
  printf("DS1302 Test Start...\r\n");

  // 구조체 테스트

  now.sec = 10;
  printf("struct sec test : %d \r\n", now.sec);

  // 쓰기 방지 설정 가능성
  // // WRITE-PROTECT BIT 설정이 있음
  Enable_Write;
  // 초시계 활성화
  printf("Clock init \r\n");
  Enable_Clock;

  // 24시간제
  printf("24H \r\n");
  DS1302_WriteReg(0x84, 0x00);

  // 날짜 시각 초기회
  // 26년 1월 28일 17시 00분 수요일(3)
  TimeSet_v1700;

  // // test용 값 입력 -> RAM의 시작값에 입력 함
  // Vcc 3.3->5v 변경 후 재시도 해보기

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    
    //초단위 출력
    printf("\r\n");

    printf("RTC \r\n");
    printf("[] Read Sec: %d \r\n", sec_min(DS1302_ReadReg(0x81)));
    printf("[] Read Min: %d \r\n", sec_min(DS1302_ReadReg(0x83)));
    printf("[] Read Hours: %d \r\n", hours(DS1302_ReadReg(0x85)));
    printf("[] Read Dates: %d \r\n", hours(DS1302_ReadReg(0x87)));
    printf("[] Read Month: %d \r\n", hours(DS1302_ReadReg(0x89)));
    printf("[] Read Day: %s \r\n",days(DS1302_ReadReg(0x8B)));
    printf("[] Read Years: %d \r\n", sec_min(DS1302_ReadReg(0x8D)));
    printf("\r\n");

    HAL_Delay(500);

    printf("Burst Mode \r\n");
    CE_ON;

    DS1302_WriteByte(0xBF);

    printf("[] Read Sec: %d \r\n", sec_min(DS1302_ReadByte()));
    printf("[] Read Min: %d \r\n", sec_min(DS1302_ReadByte()));
    printf("[] Read Hours: %d \r\n", hours(DS1302_ReadByte()));
    printf("[] Read Dates: %d \r\n", hours(DS1302_ReadByte()));
    printf("[] Read Month: %d \r\n", hours(DS1302_ReadByte()));
    printf("[] Read Day: %s \r\n", days(DS1302_ReadByte()));
    printf("[] Read Years: %d \r\n", sec_min(DS1302_ReadByte()));
    CE_OFF;

    HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  HAL_GPIO_WritePin(GPIOC, CLK_Pin|DAT_Pin|RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SMPS_EN_Pin|SMPS_V1_Pin|SMPS_SW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CLK_Pin DAT_Pin */
  GPIO_InitStruct.Pin = CLK_Pin|DAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_Init_C;

  /*Configure GPIO pin : RST_Pin */
  GPIO_InitStruct.Pin = RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RST_GPIO_Port, &GPIO_InitStruct);

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

  /*Configure GPIO pin : LD4_Pin */
  GPIO_InitStruct.Pin = LD4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
