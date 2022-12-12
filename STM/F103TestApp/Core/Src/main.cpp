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
#include "../CommonTools/liquidcrystali2c.h"

#include "ILI9341_GFX.h"
#include "fonts.h"
//#include "img.h"
#include "xpt2046_touch.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
//I2C_HandleTypeDef hi2c1;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
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
	HAL_GPIO_WritePin(GPIOB, TOUCH_CS_Pin|TFT_CS_Pin|TFT_RESET_Pin|TFT_DC_Pin, GPIO_PIN_RESET);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = TOUCH_CS_Pin|TFT_CS_Pin|TFT_RESET_Pin|TFT_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : IRQ_Pin */
	GPIO_InitStruct.Pin = IRQ_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(IRQ_GPIO_Port, &GPIO_InitStruct);

	MX_SPI1_Init();
	//MX_I2C1_Init();
	//LiquidCrystalI2C lcd(hi2c1, 0x27 << 1, 2, 20);
	//lcd.showCursor();
	//lcd.sendString("Hello\nWorld");
//	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
//
//	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(EXTI1_IRQn);


//	DISP_CS_UNSELECT;
//	TOUCH_CS_UNSELECT; // это нужно только если есть тач
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ILI9341_Init(); // инициализация дисплея
//
//	/////////////////////////// далее демонстрируются различные пользовательские функции ////////////////////////////
//	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2); // установка ориентации экрана (варианты в файле ILI9341_GFX.h)
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ILI9341_Fill_Screen(MYFON); // заливка всего экрана цветом (цвета в файле ILI9341_GFX.h)

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* вывод строк разными шрифтами (шрифты определены в файле fonts.h, а массивы шрифтов в файле fonts.c)
	  первый и второй аргумент это начало координат (справа, сверху), четвёртый аргумент шрифт
	  два последних аргумента это цвет шрифта и цвет фона шрифта */

//	ILI9341_WriteString(10, 10, "Hello World", Font_7x10, WHITE, MYFON); // можно передавать непосредственно текст
//	ILI9341_WriteString(20, 30, "Hello World", Font_11x18, WHITE, MYFON);
//	ILI9341_WriteString(30, 60, "Hello World", Font_16x26, BLUE, DARKGREEN);

	//lcd.turnOffBacklight();
	//lcd.turnOnBacklight();
	//lcd.showCursor();
	//lcd.setCursor(1, 0);
	//lcd.hideCursor();
	//lcd.blink();
	//lcd.noBlink();
	//lcd.clear();
	/* EXTI interrupt init*/


	AppConfig::getInstance().setDelay(1000);
	//Init_7219(7);
	Display7segmentMax7219 display(hspi1, GPIOA, GPIO_PIN_4);
	display.init(14, 8);

	display.print(1);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(10);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(11, 3);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-1);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-10);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-11, 4);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(12.34f, 1);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(12.34f, 2);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(12.0f, 2);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(12.0f, 0);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(0.05f, 2);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(0.05f, 3);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-12.34f, 1);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-12.34f, 2);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-12.0f, 2);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-12.0f, 0);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-0.05f, 2);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(-0.05f, 3);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	display.print(99999999);
	HAL_Delay(AppConfig::getInstance().getDelay());
	display.clean();

	//display.printItos(8, 567890);
	while (1) {
		Port<Ports::GPIOc>::set(13);
		HAL_Delay(AppConfig::getInstance().getDelay());
		Port<Ports::GPIOc>::reset(13);
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

static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
//	hi2c1.Instance = I2C1;
//	hi2c1.Init.ClockSpeed = 100000;
//	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
//	hi2c1.Init.OwnAddress1 = 0;
//	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//	hi2c1.Init.OwnAddress2 = 0;
//	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//	{
//		Error_Handler();
//	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

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
