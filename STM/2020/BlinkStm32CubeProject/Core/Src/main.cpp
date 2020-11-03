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
	f4.clockInit();
	f4.enableAHB1(F4xxx::RCC_AHB1::PortA);
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

#define TIMx_CLK                16000000UL // Hz
#define TIMx_Internal_Frequency 1000UL     // Hz
#define TIMx_Out_Frequency      1UL        // Hz

#define Prescaler (TIMx_CLK /TIMx_Internal_Frequency)-1UL
#define Period (TIMx_Internal_Frequency/TIMx_Out_Frequency)-1UL

void initTimer()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;    // Тактирование на таймер 6

	TIM2->PSC = Prescaler;			// Шина на 36мгц, делим. Получаем 1Мгц
	TIM2->ARR = Period;			// Тикать будет с частотой 10Кгц.

	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1|=TIM_CR1_CEN;
	NVIC_SetPriority(TIM2_IRQn,0);
	NVIC_EnableIRQ(TIM2_IRQn);

	//RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;    	// Подаем тактирование на таймер от шины APB1
	/*TIM2->PSC = 15;				// Частота этой шины 16 мегагерц. Так что в предделитель записываем 36-1, получим 1МГц
	TIM2->ARR = (1000000-1); // 1s				// Потолком счета таймера укажем 100-1. Получим деление на 100 в частоте вызова прерываний.


	TIM2->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS | TIM_CR1_CEN;  		//  ARPE=1 - буфферизируем регистр предзагрузки таймера опциональная вещь.
	// URS=1	- разрешаем из событий таймера только события от переполнения
	// CEN=1 - запускаем таймер
	TIM2->DIER  |= TIM_DIER_UIE;					// UIE=1 - Разрешаем прерывание от переполнения

	NVIC_SetPriority(TIM2_IRQn,14);				// Очень важный момент!!! Надо правильно выставить приоритеты. Они должны быть в диапазоне между
	// configKERNEL_INTERRUPT_PRIORITY  и configMAX_SYSCALL_INTERRUPT_PRIORITY
	NVIC_EnableIRQ(TIM2_IRQn);
	return;
	F4xxx f4;
	f4.clockInit();
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
	 */
	//Configure the Internal Clock source
	/*TIM2->SMCR &= ~TIM_SMCR_SMS;  // Internal clock source
	TIM2-> CR1 |= TIM_CR1_CEN;

	//Select the up counter mode
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM2->CR1 &= ~TIM_CR1_CKD;

	// Set the clock division to 1
	TIM2->CR1 |= TIM_CR1_CEN;

	//Set the Autoreload value
	TIM2->ARR = 49999;//PERIOD

	// Generate an update event to reload the Prescaler value immediately
	TIM2->EGR = TIM_EGR_UG;
	TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;

	TIM2 ->RCR |= ~TIM_RCR_REP;

	TIM2 ->SR = 0;*/

}

extern "C" void TIM2_IRQHandler(void) {

	//Is the timer set?
	if(TIM2->SR & TIM_SR_UIF) {

		GPIOA->ODR ^= (0x1UL << 5);
		TIM2->SR = ~TIM_SR_UIF;
		//Clears flag.
		//TIM2->SR &= ~TIM_SR_UIF;
	}
}

int main(void)
{
	__enable_irq();
	F4xxx f4;
	f4.clockInit();
	f4.enableAHB1(F4xxx::RCC_AHB1::PortA);
	f4.setPinMode(F4xxx::Port::A, F4xxx::PortMode::Output, 5);
	//ledBlinking();
	initTimer();
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
