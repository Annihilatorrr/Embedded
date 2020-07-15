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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "application.h"
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

volatile uint32_t msTicks = 0;                              /* Variable to store millisecond ticks */

void SysTick_Handler(void)
{                               /* SysTick interrupt Handler. */
	msTicks++;                                                /* See startup file startup_LPC17xx.s for SysTick vector */
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void initTimer()
{
	// Basic Timer configuration
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;         // Enable timer 2 clock
	TIM2->CR1    &= ~TIM_CR1_CEN;               // Disable counter
	TIM2->CR2    &= ~TIM_CR2_TI1S;              // TI1 only

	TIM2->PSC     = 1000;         // 10000*100s = 1s
	TIM2->CR1    &= ~TIM_CR1_DIR;               // Up direction

	// Capture 2
	GPIOA->MODER  |=  GPIO_MODE_AF_PP << 2;     // Alternate function on pin 1
	GPIOA->AFR[0] |=  GPIO_AF1_TIM2 << 4;
	TIM2->CCMR1   |=  TIM_CCMR1_CC2S_0;         // Set capture channel 2 as input on TI2 (CC2S = 01)
	TIM2->CCER    &= ~TIM_CCER_CC2P;            // Set capture on rising edge event
	TIM2->CCER    &= ~TIM_CCER_CC2NP;
	TIM2->CCER    |=  TIM_CCER_CC2E;            // Enable capture on channel 1
	TIM2->DIER    |=  TIM_DIER_CC2IE;           // Enable interrupt on capture channel 1

	// Timer start and IRQ enable
	TIM2->DIER |= TIM_DIER_UIE;                 // Enable Update interrupt
	NVIC_EnableIRQ( TIM2_IRQn );                // Vector name with my startup is TIM2_IRQHandler()
	TIM2->CR1 |= TIM_CR1_CEN;                   // Faster Pussycat Kill! Kill!

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
	initTimer();
	/* USER CODE END Init */

	/* Configure the system clock */
	//SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	/* USER CODE BEGIN 2 */
	Application app;
	app.start();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
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
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_SYSTICK_Config( HAL_RCC_GetHCLKFreq() / 1000 );
	HAL_SYSTICK_CLKSourceConfig( SYSTICK_CLKSOURCE_HCLK );

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority( SysTick_IRQn, 0, 0 );
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{

	/* GPIO Ports Clock Enable */
	//__HAL_RCC_GPIOH_CLK_ENABLE();
	//__HAL_RCC_GPIOA_CLK_ENABLE();

}

void TIM2_IRQHandler(void) {

	//Is the timer set?
	if(TIM2->SR & TIM_SR_UIF) {

		//Draw a rectangle on screen (test purpose).


		//Clears flag.
		TIM2->SR &= ~TIM_SR_UIF;
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
