/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "app.h"
#include "appconfig.h"
#include "../CommonTools/pins.h"
#include "../CommonTools/display7segmentmax7219.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef enum {
	DIGIT_0 = 0x1,
	DIGIT_1 = 0x2,
	DIGIT_2 = 0x3,
	DIGIT_3 = 0x4,
	DIGIT_4 = 0x5,
	DIGIT_5 = 0x6,
	DIGIT_6 = 0x7,
	DIGIT_7 = 0x8,

	DECODE_MODE = 0x9,
	INTENSITY   = 0xA,
	SCAN_LIMIT  = 0xB,
	SHUTDOWN    = 0xC,
	DISPLAY_TEST= 0xF
} MAX7219_COMMAND;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case GPIO_PIN_0:
		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_0) != 0)
		{
			static int i = 1;
			AppConfig::getInstance ().setDelay (
					AppConfig::getInstance ().getDelay () / 2);
			++i;
			Port<Ports::GPIOc>::set(14);
		}
		else
		{
			Port<Ports::GPIOc>::reset(14);
		}

		HAL_GPIO_EXTI_IRQHandler (GPIO_PIN_0);
		break;
	case GPIO_PIN_1:
		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1) != 0)
		{
			static int i = 1;
			AppConfig::getInstance ().setDelay (
					AppConfig::getInstance ().getDelay () * 2);
			--i;
			Port<Ports::GPIOc>::set(14);
		}
		else
		{
			Port<Ports::GPIOc>::reset(14);
		}

		HAL_GPIO_EXTI_IRQHandler (GPIO_PIN_1);
		break;

	}
}

void Send_7219(uint8_t reg, uint8_t value)
{
	uint8_t tx_data[2] = { reg, value };
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	//	HAL_Delay(1);
}

void Clear_7219 (char dg)
{
	uint8_t i = dg;
	do
	{
		Send_7219(i,0x0);
	} while (--i);
}

void Number_7219(long n, char dg)
{
	uint8_t ng=0;//переменная для минуса
	if(n<0)
	{
		ng=1;
		n*=-1;
	}
	Clear_7219(dg);
	if(n==0)
	{
		Send_7219(0x01,0);//в первый разряд напишем 0
		return;
	}
	uint8_t i=0;
	do
	{
		Send_7219(++i,n%10);
		n/=10;
	} while (n);
	if (ng)
	{
		Send_7219(i+1,0xA);//символ —
	}
}

void Init_7219(char dg)
{
	Send_7219(DISPLAY_TEST,0);//disokay test
	Send_7219(DECODE_MODE,0b1111);//включим режим декодирования
	Send_7219(SCAN_LIMIT,dg-1);//кол-во используемых разрядов
	Send_7219(INTENSITY,32);//интенсивность свечения

	Send_7219(SHUTDOWN,0x01);//включим индикатор
	Clear_7219(dg);
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
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	App::initializePorts();
	MX_SPI1_Init();
	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	AppConfig::getInstance().setDelay(1000);
	//Init_7219(7);
	Display7segmentMax7219 display(hspi1);
	display.init(4);
//	display.printItos(4, 567890);
//	display.printItos(5, 567890);
//	display.printItos(6, 567890);
	display.printItos(7, 567890);
	HAL_Delay(AppConfig::getInstance().getDelay());
	//display.printItos(8, 567890);
	while (1) {
		Port<Ports::GPIOc>::set(13);
		Send_7219(1, 0x01);//1
		HAL_Delay(AppConfig::getInstance().getDelay());
		Port<Ports::GPIOc>::reset(13);
		Send_7219(2, 0x02);//2
		HAL_Delay(AppConfig::getInstance().getDelay());
		//printf("Hello World\n\r");
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
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

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
