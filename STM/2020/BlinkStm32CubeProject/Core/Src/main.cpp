/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Stm32Wrapper/f4xxx.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

void delay()
{
	volatile int i;
	for( i=0; i<100000; ++i );
}

void ledBlinking()
{
	F4xxx f4;
	//f4.clockInit(8, 336, 4, 7);
	f4.enableAHB1(F4xxx::RCCEnableAhb1PeripheralClockFor::PortA);
	f4.setPinMode(F4xxx::Port::A, F4xxx::PortMode::Output, 5);

	volatile int i;

	while(1)
	{

		f4.setPinHigh(F4xxx::Port::A, 5);
		delay();
		f4.setPinLow(F4xxx::Port::A, 5);
		delay();
	}
}

//#define TIMx_CLK                16000000UL // Hz
//#define TIMx_Internal_Frequency 1000UL     // Hz
//#define TIMx_Out_Frequency      1UL        // Hz

#define TIMx_CLK                48000000UL // Hz
#define TIMx_Internal_Frequency 16000000UL     // Hz
#define TIMx_Out_Frequency      1UL        // Hz

#define Prescaler (TIMx_CLK /TIMx_Internal_Frequency)-1UL
#define Period (TIMx_Internal_Frequency/TIMx_Out_Frequency)-1UL

void initTimer()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->PSC = 0;
	TIM2->ARR = 0; // the reload value of the timer counter

	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
	NVIC_SetPriority(TIM2_IRQn,0);
	NVIC_EnableIRQ(TIM2_IRQn);
}

extern "C" void TIM2_IRQHandler(void) {

	//Is the timer set?
	if(TIM2->SR & TIM_SR_UIF) {

		GPIOA->ODR ^= (0x1UL << 5);
		GPIOA->ODR ^= (0x1UL << 5);
		TIM2->SR = ~TIM_SR_UIF;
		//TIM2->SR = ~TIM_SR_UIF;
		//Clears flag.
		//TIM2->SR &= ~TIM_SR_UIF;
	}
}

int main(void)
{
	__enable_irq();
	//F4xxx f4;

	//f4.clockInit(8, 336, 4, 7);
	//f4.enableAHB1(F4xxx::RCCEnableAhb1PeripheralClockFor::PortA);
	//f4.setPinMode(F4xxx::Port::A, F4xxx::PortMode::Output, 5);
	//f4.setPinMode(F4xxx::Port::A, F4xxx::PortMode::Alternative, 6);
	ledBlinking();
	//initTimer();
	while(true)
	{

	}
	//__enable_irq();
	//  /* USER CODE BEGIN 1 */
	//
	//  /* USER CODE END 1 */
	//
	//  /* MCU Configuration--------------------------------------------------------*/
	//
	//  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	//  HAL_Init();
	//
	//  /* USER CODE BEGIN Init */
	//
	//  /* USER CODE END Init */
	//
	//  /* Configure the system clock */
	//  SystemClock_Config();
	//
	//  /* USER CODE BEGIN SysInit */
	//
	//  /* USER CODE END SysInit */
	//
	//  /* Initialize all configured peripherals */
	//  /* USER CODE BEGIN 2 */
	//
	//  /* USER CODE END 2 */
	//
	//  /* Infinite loop */
	//  /* USER CODE BEGIN WHILE */
	//  while (1)
	//  {
	//    /* USER CODE END WHILE */
	//
	//    /* USER CODE BEGIN 3 */
	//  }
	//  /* USER CODE END 3 */
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
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
