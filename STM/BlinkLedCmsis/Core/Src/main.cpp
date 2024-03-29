/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
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

#include <stdint.h>
#include "stm32f1xx.h"
#include "application.h"
#include "display7segmentmax7219.h"
#include "ILI9341_GFX.h"
#include <stdio.h>
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define F_CPU 		72000000UL

volatile uint32_t sysTick = 0;

extern "C" void SysTick_Handler(void)
{
	if (sysTick > 0)
	{
		--sysTick;
	}
}

void delayMs(uint32_t ms)
{
	sysTick = ms;
	while (sysTick);
}

void SysTick_Init(void) {
	SysTick->LOAD &= ~SysTick_LOAD_RELOAD_Msk;
	SysTick->LOAD = F_CPU/1000-1;
	SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
	SysTick->CTRL = (SysTick_CTRL_TICKINT_Msk   |  /* Enable SysTick exception */
			SysTick_CTRL_ENABLE_Msk) |    /* Enable SysTick system timer */
					SysTick_CTRL_CLKSOURCE_Msk;   /* Use processor clock source */

}

int main(void)
{
	SysTick_Init();
	Application::getInstance().initPeriferal();
	LCD_CS_PORT->BRR =  (1 << LCD_CS_PIN);
	ILI9341_Init();

	    // Simple Text writing (Text, Font, X, Y, Color, BackColor)
	    // Available Fonts are FONT1, FONT2, FONT3 and FONT4
	    ILI9341_FillScreen(RED);

	    ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
	    ILI9341_DrawText("HELLO WORLD", FONT4, 90, 110, BLACK, WHITE);
	    delayMs(1000);

	    //Writing numbers
	    ILI9341_FillScreen(WHITE);
	    static char BufferText[30];
	    for(uint8_t i = 0; i <= 5; i++)
	    {
	      sprintf(BufferText, "COUNT : %d", i);
	      ILI9341_DrawText(BufferText, FONT3, 10, 10, BLACK, WHITE);
	      ILI9341_DrawText(BufferText, FONT3, 10, 30, BLUE, WHITE);
	      ILI9341_DrawText(BufferText, FONT3, 10, 50, RED, WHITE);
	      ILI9341_DrawText(BufferText, FONT3, 10, 70, GREEN, WHITE);
	      ILI9341_DrawText(BufferText, FONT3, 10, 90, YELLOW, WHITE);
	      ILI9341_DrawText(BufferText, FONT3, 10, 110, PURPLE, WHITE);
	      ILI9341_DrawText(BufferText, FONT3, 10, 130, ORANGE, WHITE);
	      ILI9341_DrawText(BufferText, FONT3, 10, 150, MAROON, WHITE);
	      ILI9341_DrawText(BufferText, FONT3, 10, 170, WHITE, BLACK);
	      ILI9341_DrawText(BufferText, FONT3, 10, 190, BLUE, BLACK);
	    }

//	GPIOA->BSRR = GPIO_BSRR_BS0;
//	GPIOC->BSRR = GPIO_BSRR_BS13;
//	GPIOC->BSRR = GPIO_BSRR_BS14;
	//Display7segmentMax7219 display;
	//display.init(5, 7);
	while (1)
	{
		delayMs(500);
		Application::getInstance().setPin(GPIOC, 13); // LED is inverted, turn it off.
		delayMs(500);
		Application::getInstance().resetPin(GPIOC, 13);
		//display.print(123);
	}
}
