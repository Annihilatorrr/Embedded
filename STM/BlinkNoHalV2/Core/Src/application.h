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

	void SPI1_Init(void)
	{
	  //Включаем тактирование SPI1 и GPIOA
	  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	  /**********************************************************/
	  /*** Настройка выводов GPIOA на работу совместно с SPI1 ***/
	  /**********************************************************/
	  //PA7 - MOSI
	  //PA6 - MISO
	  //PA5 - SCK

	  //Для начала сбрасываем все конфигурационные биты в нули
	  /*GPIOA->CRL &= ~(GPIO_CRL_CNF5_Msk | GPIO_CRL_MODE5_Msk
	                | GPIO_CRL_CNF6_Msk | GPIO_CRL_MODE6_Msk
	                | GPIO_CRL_CNF7_Msk | GPIO_CRL_MODE7_Msk);

	  //Настраиваем
	  //SCK: MODE5 = 0x03 (11b); CNF5 = 0x02 (10b)
	  GPIOA->CRL |= (0x02<<GPIO_CRL_CNF5_Pos) | (0x03<<GPIO_CRL_MODE5_Pos);

	  //MISO: MODE6 = 0x00 (00b); CNF6 = 0x01 (01b)
	  GPIOA->CRL |= (0x01<<GPIO_CRL_CNF6_Pos) | (0x00<<GPIO_CRL_MODE6_Pos);

	  //MOSI: MODE7 = 0x03 (11b); CNF7 = 0x02 (10b)
	  GPIOA->CRL |= (0x02<<GPIO_CRL_CNF7_Pos) | (0x03<<GPIO_CRL_MODE7_Pos);

*/
	  //MOSI: MODE7 = 0x03 (11b); CNF7 = 0x02 (10b)
	  GPIOA->CR |= (0x02<<GPIO_CRL_CNF7_Pos) | (0x03<<GPIO_CRL_MODE7_Pos);
	  /**********************/
	  /*** Настройка SPI1 ***/
	  /**********************/

	  SPI1->CR1 = 0<<SPI_CR1_DFF_Pos  //Размер кадра 8 бит
	    | 0<<SPI_CR1_LSBFIRST_Pos     //MSB first
	    | 1<<SPI_CR1_SSM_Pos          //Программное управление SS
	    | 1<<SPI_CR1_SSI_Pos          //SS в высоком состоянии
	    | 0x04<<SPI_CR1_BR_Pos        //Скорость передачи: F_PCLK/32
	    | 1<<SPI_CR1_MSTR_Pos         //Режим Master (ведущий)
	    | 0<<SPI_CR1_CPOL_Pos | 0<<SPI_CR1_CPHA_Pos; //Режим работы SPI: 0

	  SPI1->CR1 |= 1<<SPI_CR1_SPE_Pos; //Включаем SPI
	}

	void SPI1_Tx(uint8_t *data, int32_t len)
	{
	  if(len<=0)
	    return;

	  //Ждем, пока SPI освободится от предыдущей передачи
	  while(SPI1->SR & SPI_SR_BSY)
	    ;

	  //Настройка переменных, которые будут
	  //использоваться в обработчике прерывания SPI
	  tx_index = 0;
	  tx_len = len;
	  tx_data = data;

	  //Разрешаем прерывание TXEIE И запускаем обмен
	  SPI1->CR2 |= (1<<SPI_CR2_TXEIE_Pos);
	}
};

#endif /* APPLICATION_H_ */
