/*
 * display7segmentmax7219.h
 *
 *  Created on: Jul 13, 2020
 *      Author: VertexLT
 */

#ifndef SRC_DISPLAY7SEGMENTMAX7219_H_
#define SRC_DISPLAY7SEGMENTMAX7219_H_

#define MAX7219_OFF (0x0)
#define MAX7219_ON  (0x1)
#define MAX7219_SHUTDOWN_REG    (0X0C)

#define MAX7219_DECODE_REG      (0x09)
#define MAX7219_DIGIT_REG(pos)  ((pos) + 1)
#define MAX7219_INTENSITY_REG   (0x0A)
#define MAX7219_SCANLIMIT_REG   (0x0B)
#define MAX7219_DISPLAYTEST_REG (0x0F)

#include "spi.h"
class Display7SegmentMax7219
{
	SPI m_spi;

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

public:
	Display7SegmentMax7219()
	{
		m_spi.init();
	}

	void shutDownOn()
	{
		sendCommand(MAX7219_SHUTDOWN_REG, MAX7219_ON); // 0x0C
	}

	void clear(unsigned char digits)
	{
        for (int i = 1; i <= digits; i++)
        {
            sendCommand(i, 0x0f); // clear
        }



        sendCommand(MAX7219_SCANLIMIT_REG, 5); // 0x0B
        sendCommand(MAX7219_DECODE_REG, 0xff); // 0x09 no decode mode for 0-7 digits
        sendCommand(MAX7219_DIGIT_REG(0), 0x1);
        sendCommand(MAX7219_DIGIT_REG(1), 0x2);
        sendCommand(MAX7219_DIGIT_REG(2), 0x3);

	}

	void shutDownOff()
	{
		sendCommand(MAX7219_SHUTDOWN_REG, MAX7219_OFF); // 0x0C
	}
};

#endif /* SRC_DISPLAY7SEGMENTMAX7219_H_ */
