/*
 * application.h
 *
 *  Created on: Apr 5, 2020
 *      Author: VertexLT
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "stm32f4xx_hal.h"

#define LED_PORT GPIOA
#define LED_GREEN (1 << 5) /* port A, pin 5 */

class Application
{
public:
	void runPA5Blinking(int delay)
	{
		LED_PORT->ODR = LED_GREEN;
		delayMs(delay);
		switch_leds_off();
		delayMs(delay);
	}

	void runPA5BlinkingV2(int delay)
	{
		LED_PORT->BSRR = LED_GREEN;
		delayMs(delay);
		LED_PORT->BSRR = 0x00200000; // reset bit PIN5 by setting bit 21 of bssr
		delayMs(delay);
	}

	void switch_leds_off(void){
		LED_PORT->ODR = 0;
	}

	void delayMs(int delay)
	{
		int j = 0;
		for (int i=0 ; i < delay ; i++)
		{
			j++;
		}
	}
};

#endif /* APPLICATION_H_ */
