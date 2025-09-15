/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
unsigned int echo_time = 0;
unsigned int echo2_time = 0;
int dist;
int dist2;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE {
	if (ch == '\n')
		HAL_UART_Transmit(&huart2, (uint8_t*) "\r", 1, 0xFFFF);
	HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, 0xFFFF);

	return ch;
}

void timer_start(void) {
	HAL_TIM_Base_Start(&htim1);
}

void delay_us(uint16_t us) {
	__HAL_TIM_SET_COUNTER(&htim1, 0); // initialize counter to start from 0
	while ((__HAL_TIM_GET_COUNTER(&htim1)) < us)
		; // wait count until us
}

void trig(void) {
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, 1);
	delay_us(10);
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, 0);
}

/**
 * @brief echo 신호가 HIGH를 유지하는 시간을 (㎲)단위로 측정하여 리턴한다.
 * @param no param(void)
 */

long unsigned int echo(void) {
	long unsigned int echo = 0;

	while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == 0) {
	}
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == 1)
		;
	echo = __HAL_TIM_GET_COUNTER(&htim1);

	if (echo >= 240 && echo <= 23000)
		return echo;
	else
		return 0;
}

void timer2_start(void) {
	HAL_TIM_Base_Start(&htim2);
}

void delay2_us(uint16_t us) {
	__HAL_TIM_SET_COUNTER(&htim2, 0); // initialize counter to start from 0
	while ((__HAL_TIM_GET_COUNTER(&htim2)) < us)
		; // wait count until us
}

void trig2(void) {
	HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, 1);
	delay2_us(10);
	HAL_GPIO_WritePin(TRIG2_GPIO_Port, TRIG2_Pin, 0);
}

long unsigned int echo2(void) {
	long unsigned int echo2 = 0;

	while (HAL_GPIO_ReadPin(ECHO2_GPIO_Port, ECHO2_Pin) == 0) {
	}
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while (HAL_GPIO_ReadPin(ECHO2_GPIO_Port, ECHO2_Pin) == 1)
		;
	echo2 = __HAL_TIM_GET_COUNTER(&htim2);

	if (echo2 >= 240 && echo2 <= 23000)
		return echo2;
	else
		return 0;
}

void smartcar_forward(void) {
	HAL_GPIO_WritePin(LFF_GPIO_Port, LFF_Pin, 1);
	HAL_GPIO_WritePin(LFB_GPIO_Port, LFB_Pin, 0);

	HAL_GPIO_WritePin(LRF_GPIO_Port, LRF_Pin, 1);
	HAL_GPIO_WritePin(LRB_GPIO_Port, LRB_Pin, 0);

	HAL_GPIO_WritePin(RFF_GPIO_Port, RFF_Pin, 1);
	HAL_GPIO_WritePin(RFB_GPIO_Port, RFB_Pin, 0);

	HAL_GPIO_WritePin(RRF_GPIO_Port, RRF_Pin, 1);
	HAL_GPIO_WritePin(RRB_GPIO_Port, RRB_Pin, 0);
}

void smartcar_backward(void) {
	HAL_GPIO_WritePin(LFF_GPIO_Port, LFF_Pin, 0);
	HAL_GPIO_WritePin(LFB_GPIO_Port, LFB_Pin, 1);

	HAL_GPIO_WritePin(LRF_GPIO_Port, LRF_Pin, 0);
	HAL_GPIO_WritePin(LRB_GPIO_Port, LRB_Pin, 1);

	HAL_GPIO_WritePin(RFF_GPIO_Port, RFF_Pin, 0);
	HAL_GPIO_WritePin(RFB_GPIO_Port, RFB_Pin, 1);

	HAL_GPIO_WritePin(RRF_GPIO_Port, RRF_Pin, 0);
	HAL_GPIO_WritePin(RRB_GPIO_Port, RRB_Pin, 1);
}

void smartcar_right(void) {
	HAL_GPIO_WritePin(LFF_GPIO_Port, LFF_Pin, 1);
	HAL_GPIO_WritePin(LFB_GPIO_Port, LFB_Pin, 0);

	HAL_GPIO_WritePin(LRF_GPIO_Port, LRF_Pin, 1);
	HAL_GPIO_WritePin(LRB_GPIO_Port, LRB_Pin, 0);

	HAL_GPIO_WritePin(RFF_GPIO_Port, RFF_Pin, 0);
	HAL_GPIO_WritePin(RFB_GPIO_Port, RFB_Pin, 1);

	HAL_GPIO_WritePin(RRF_GPIO_Port, RRF_Pin, 0);
	HAL_GPIO_WritePin(RRB_GPIO_Port, RRB_Pin, 1);
}

void smartcar_left(void) {
	HAL_GPIO_WritePin(LFF_GPIO_Port, LFF_Pin, 0);
	HAL_GPIO_WritePin(LFB_GPIO_Port, LFB_Pin, 1);

	HAL_GPIO_WritePin(LRF_GPIO_Port, LRF_Pin, 0);
	HAL_GPIO_WritePin(LRB_GPIO_Port, LRB_Pin, 1);

	HAL_GPIO_WritePin(RFF_GPIO_Port, RFF_Pin, 1);
	HAL_GPIO_WritePin(RFB_GPIO_Port, RFB_Pin, 0);

	HAL_GPIO_WritePin(RRF_GPIO_Port, RRF_Pin, 1);
	HAL_GPIO_WritePin(RRB_GPIO_Port, RRB_Pin, 0);
}

void smartcar_stop(void) {
	HAL_GPIO_WritePin(LFF_GPIO_Port, LFF_Pin, 0);
	HAL_GPIO_WritePin(LFB_GPIO_Port, LFB_Pin, 0);

	HAL_GPIO_WritePin(LRF_GPIO_Port, LRF_Pin, 0);
	HAL_GPIO_WritePin(LRB_GPIO_Port, LRB_Pin, 0);

	HAL_GPIO_WritePin(RFF_GPIO_Port, RFF_Pin, 0);
	HAL_GPIO_WritePin(RFB_GPIO_Port, RFB_Pin, 0);

	HAL_GPIO_WritePin(RRF_GPIO_Port, RRF_Pin, 0);
	HAL_GPIO_WritePin(RRB_GPIO_Port, RRB_Pin, 0);
}
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
	MX_USART2_UART_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */
	printf("=== UART2 Receive Test ===\r\n");
	timer_start();
	timer2_start();
	uint8_t ch;

	smartcar_stop();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
//		HAL_UART_Receive(&huart2, &ch, 1, HAL_MAX_DELAY);

		trig();
		echo_time = echo();

		if (echo_time != 0) {
			dist = (int) (17 * echo_time / 100);
			printf("\n Distance = %d(mm)\n", dist);
		} else
			printf("Out of Range!\n");

		trig2();
		echo2_time = echo2();
		if (echo2_time != 0) {
			dist2 = (int) (17 * echo2_time / 100);
			printf("Distance2 = %d(mm)\n\n", dist2);
		} else
			printf("Out of Range2!\n");

		smartcar_forward();

		if (dist <= 200 || dist2 <= 200){
			if (dist > dist2){
				smartcar_right();
				HAL_Delay(300);
			}
			else if (dist < dist2){
				smartcar_left();
				HAL_Delay(300);
			}
		}

//
//		if (ch == 'w') {
//			printf("Forward \r\n");
//			smartcar_forward();
//			HAL_Delay(100);
//		} else if (ch == 's') {
//			printf("backward \r\n");
//			smartcar_backward();
//			HAL_Delay(100);
//		} else if (ch == 'd') {
//			printf("right \r\n");
//			smartcar_right();
//			HAL_Delay(100);
//		} else if (ch == 'a') {
//			printf("left \r\n");
//			smartcar_left();
//			HAL_Delay(100);
//		} else if (ch == 'f') {
//			printf("stop \r\n");
//			smartcar_stop();
//		}
//
//		smartcar_stop();

//
//	  smartcar_forward();
//	  HAL_Delay(5000);
//
//	  smartcar_backward();
//	  HAL_Delay(5000);
//
//	  smartcar_stop();
//	  HAL_Delay(5000);

//	  HAL_GPIO_WritePin(RFF_GPIO_Port, RFF_Pin, 1);
//	  HAL_GPIO_WritePin(RFB_GPIO_Port, RFB_Pin, 0);
//
//	  HAL_GPIO_WritePin(RRF_GPIO_Port, RRF_Pin, 1);
//	  HAL_GPIO_WritePin(RRB_GPIO_Port, RRB_Pin, 0);
//
//	  HAL_GPIO_WritePin(LFF_GPIO_Port, LFF_Pin, 1);
//	  HAL_GPIO_WritePin(LFB_GPIO_Port, LFB_Pin, 0);
//
//	  HAL_GPIO_WritePin(LRF_GPIO_Port, LRF_Pin, 1);
//	  HAL_GPIO_WritePin(LRB_GPIO_Port, LRB_Pin, 0);
//
//	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
//	  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
//	  HAL_Delay(100);
//	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
//	  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
//	  HAL_Delay(100);

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 64 - 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 64 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LED2_Pin | LD2_Pin | TRIG2_Pin | LRF_Pin | LFB_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
	LRB_Pin | LFF_Pin | RFF_Pin | RFB_Pin | RRF_Pin | RRB_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LED2_Pin LD2_Pin TRIG2_Pin LRF_Pin
	 LFB_Pin */
	GPIO_InitStruct.Pin = LED2_Pin | LD2_Pin | TRIG2_Pin | LRF_Pin | LFB_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : ECHO2_Pin */
	GPIO_InitStruct.Pin = ECHO2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ECHO2_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : ECHO_Pin */
	GPIO_InitStruct.Pin = ECHO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ECHO_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LRB_Pin LFF_Pin RFF_Pin RFB_Pin
	 RRF_Pin RRB_Pin */
	GPIO_InitStruct.Pin = LRB_Pin | LFF_Pin | RFF_Pin | RFB_Pin | RRF_Pin
			| RRB_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : TRIG_Pin */
	GPIO_InitStruct.Pin = TRIG_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TRIG_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
