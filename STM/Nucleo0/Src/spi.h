/*
 * spi.h
 *
 *  Created on: Jul 13, 2020
 *      Author: VertexLT
 */

#ifndef SRC_SPI_H_
#define SRC_SPI_H_

#include "stm32f4xx.h"

class SPI
{
public:
    void init()
    {

        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN ;

        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN ;

        GPIOA->MODER &= ~
                (GPIO_MODER_MODER4 |
                        GPIO_MODER_MODER5 |
                        GPIO_MODER_MODER6 |
                        GPIO_MODER_MODER7
                );

        GPIOA->MODER |=
                GPIO_MODER_MODER4_0 |
                GPIO_MODER_MODER5_1 |
                GPIO_MODER_MODER6_1 |
                GPIO_MODER_MODER7_1
                ;

        GPIOA->OTYPER &= ~
                (GPIO_OTYPER_OT_4 |
                        GPIO_OTYPER_OT_5 |
                        GPIO_OTYPER_OT_6 |
                        GPIO_OTYPER_OT_7
                );

        GPIOA->OSPEEDR &= ~
                (GPIO_OSPEEDER_OSPEEDR4 |
                        GPIO_OSPEEDER_OSPEEDR5 |
                        GPIO_OSPEEDER_OSPEEDR6 |
                        GPIO_OSPEEDER_OSPEEDR7
                );

        GPIOA->OSPEEDR |=
                GPIO_OSPEEDER_OSPEEDR4_0 |
                GPIO_OSPEEDER_OSPEEDR5_0 |
                GPIO_OSPEEDER_OSPEEDR6_0 |
                GPIO_OSPEEDER_OSPEEDR7_0
                ;


        GPIOA->PUPDR &= ~
                (GPIO_PUPDR_PUPDR4 |
                        GPIO_PUPDR_PUPDR5 |
                        GPIO_PUPDR_PUPDR6 |
                        GPIO_PUPDR_PUPDR7);

        GPIOA->AFR[0] |= (5 << 4*5) | (5 << 4*6) | (5 << 4*7);

        SPI1->CR1 = 0 << SPI_CR1_DFF_Pos // 8bit frame
                | 0 << SPI_CR1_LSBFIRST_Pos      // MSB first
                | 1 << SPI_CR1_SSM_Pos           // Software SS
                | 1 << SPI_CR1_SSI_Pos           // SS high
                | 0x04 << SPI_CR1_BR_Pos         // Speed: F_PCLK/32
                | 1 << SPI_CR1_MSTR_Pos          // Master mode
                | 0 << SPI_CR1_CPOL_Pos
                | 0 << SPI_CR1_CPHA_Pos;         //SPI: 0

                SPI1->CR1 |= SPI_CR1_SPE;


                while(!(SPI1->SR & SPI_SR_TXE));      // make sure TX buffer is empty
                while(SPI1->SR & SPI_SR_BSY);         // make sure SPI isn't busy

    }
};

#endif /* SRC_SPI_H_ */
