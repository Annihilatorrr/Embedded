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

class Application {
public:
    void runPA5Blinking(int delay) {
        LED_PORT->ODR = LED_GREEN;
        delayMs(delay);
        switch_leds_off();
        delayMs(delay);
    }

    void runPA5BlinkingV2(int delay) {
        LED_PORT->BSRR = LED_GREEN;
        delayMs(delay);
        LED_PORT->BSRR = 0x00200000; // reset bit PIN5 by setting bit 21 of bssr
        delayMs(delay);
    }

    void switch_leds_off(void) {
        LED_PORT->ODR = 0;
    }

    void delayMs(int delay) {
        int j = 0;
        for (int i = 0; i < delay; i++) {
            j++;
        }
    }

    void sendCommand(uint8_t address, uint8_t command) {
        GPIOA->ODR &= ~GPIO_ODR_ODR_4;            // chip select
        SPI1->DR = address;
        while (!(SPI1->SR & SPI_SR_TXE))
            ;      // make sure TX buffer is empty
        while (SPI1->SR & SPI_SR_BSY)
            ;         // make sure SPI isn't busy

        SPI1->DR = command;
        while (!(SPI1->SR & SPI_SR_TXE))
            ;      // make sure TX buffer is empty
        while (SPI1->SR & SPI_SR_BSY)
            ;         // make sure SPI isn't busy

        GPIOA->ODR |= GPIO_ODR_ODR_4;              // deselect
    }

    void SPI1_Init(void) {

        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

        // PA5 as SCK
        GPIOA->MODER &= ~(GPIO_MODER_MODER5_0);
        GPIOA->MODER |= (GPIO_MODER_MODER5_1);  // alternate function

        // MISO PA6
        GPIOA->MODER &= ~(GPIO_MODER_MODER6_0);
        GPIOA->MODER |= GPIO_MODER_MODER6_1;  // alternate function

        // MOSI PA7
        GPIOA->MODER &= ~(GPIO_MODER_MODER7_0);
        GPIOA->MODER |= GPIO_MODER_MODER7_1;  // alternate function

        GPIOA->MODER |= GPIO_MODER_MODER4_0;  // GPO
        GPIOA->ODR = GPIO_ODR_ODR_4;              // deselect

        SPI1->CR1 = 0 << SPI_CR1_DFF_Pos // 8bit frame
        | 0 << SPI_CR1_LSBFIRST_Pos      // MSB first
        | 1 << SPI_CR1_SSM_Pos           // Software SS
        | 1 << SPI_CR1_SSI_Pos           // SS high
        | 0x04 << SPI_CR1_BR_Pos         // Speed: F_PCLK/32
        | 1 << SPI_CR1_MSTR_Pos          // Master mode
        | 0 << SPI_CR1_CPOL_Pos
		| 0 << SPI_CR1_CPHA_Pos;         //SPI: 0

        SPI1->CR1 |= 1 << SPI_CR1_SPE_Pos; //Включаем SPI

        // transmit test packet
        while (!(SPI1->SR & SPI_SR_TXE));      // make sure TX buffer is empty
        while (SPI1->SR & SPI_SR_BSY);         // make sure SPI isn't busy

        for (int i = 1; i <= 8; i++)
        {
            sendCommand(i, 0x0f);
        }

        sendCommand(0x0C, 0x01);
        sendCommand(0x0B, 0x07);
        sendCommand(0x09, 0x00);

        sendCommand(0x08, 0x30); // 1
        sendCommand(0x07, 0x6D); // 2
        sendCommand(0x06, 0x79); // 3
        sendCommand(0x05, 0x37); // H
        sendCommand(0x04, 0x4F); // E
        sendCommand(0x03, 0x0E); // L
        sendCommand(0x02, 0x0E); // L
        sendCommand(0x01, 0x7E); // O
    }
};

#endif /* APPLICATION_H_ */
