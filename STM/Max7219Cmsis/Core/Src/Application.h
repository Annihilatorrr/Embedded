/*
 * application.h
 *
 *  Created on: Apr 5, 2020
 *      Author: VertexLT
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "stm32f4xx_hal.h"


// the MAX7219 address map (datasheet table 2)
#define MAX7219_DECODE_REG      (0x09)
#define MAX7219_DIGIT_REG(pos)  ((pos) + 1)
#define MAX7219_INTENSITY_REG   (0x0A)
#define MAX7219_SCANLIMIT_REG   (0x0B)
#define MAX7219_SHUTDOWN_REG    (0X0C)
#define MAX7219_DISPLAYTEST_REG (0x0F)
#define MAX7219_OFF (0x0)
#define MAX7219_ON  (0x1)

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

    void sendCommand(uint8_t address, uint8_t command)
    {
        GPIOA->ODR &= ~GPIO_ODR_ODR_4;    // chip select
        SPI1->DR = address;
        while(!(SPI1->SR & SPI_SR_TXE));// make sure TX buffer is empty
        while(SPI1->SR & SPI_SR_BSY);         // make sure SPI isn't busy

        SPI1->DR = command;
        while(!(SPI1->SR & SPI_SR_TXE));      // make sure TX buffer is empty
        while(SPI1->SR & SPI_SR_BSY);         // make sure SPI isn't busy

        GPIOA->ODR |= GPIO_ODR_ODR_4;              // deselect
    }

    void SPI1_Init(void)
    {

        RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN ;

        RCC->APB2ENR |=RCC_APB2ENR_SPI1EN ;


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
        sendCommand(MAX7219_SHUTDOWN_REG, MAX7219_ON); // 0x0C
        for (int i = 1; i <= 8; i++)
        {
            sendCommand(i, 0x0f); // clear
        }


        sendCommand(MAX7219_SCANLIMIT_REG, 5); // 0x0B
        sendCommand(MAX7219_DECODE_REG, 0xff); // 0x09 no decode mode for 0-7 digits
        sendCommand(MAX7219_DIGIT_REG(0), 0x1);
        sendCommand(MAX7219_DIGIT_REG(1), 0x2);
        while(1){}
    }
};

#endif /* APPLICATION_H_ */
