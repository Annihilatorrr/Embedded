/*
 * display7segmentmax7219.cpp
 *
 *  Created on: Nov 29, 2022
 *      Author: VertexNi
 */

#include "display7segmentmax7219.h"
#include "stm32f1xx_hal_spi.h"

uint8_t Display7segmentMax7219::SYMBOLS[] = {
		0x7E,	// numeric 0
		0x30,	// numeric 1
		0x6D,	// numeric 2
		0x79,	// numeric 3
		0x33,	// numeric 4
		0x5B,	// numeric 5
		0x5F,	// numeric 6
		0x70,	// numeric 7
		0x7F,	// numeric 8
		0x7B,	// numeric 9
		0x01,	// minus
		0x4F,	// letter E
		0x37,	// letter H
		0x0E,	// letter L
		0x67,	// letter P
		0x00	// blank
};

static int numberOfDigits = 8;

static uint32_t lcdPow10(uint8_t n)
{
	uint32_t retval = 1u;

	while (n > 0u)
	{
		retval *= 10u;
		n--;
	}

	return retval;
}

//Display7segmentMax7219::Display7segmentMax7219(void* spi)
////		m_spi((__SPI_HandleTypeDef*)spi)
//{
//	// TODO Auto-generated constructor stub
//
//}

Display7segmentMax7219::~Display7segmentMax7219() {
	// TODO Auto-generated destructor stub
}

void Display7segmentMax7219::init(uint8_t intensity){
	sendData(static_cast<uint8_t>(Registers::REG_DISPLAY_TEST), 0);//disokay test
	setDecodeMode();//включим режим декодирования
	sendData(static_cast<uint8_t>(Registers::REG_SCAN_LIMIT), numberOfDigits - 1);
	setIntensity(intensity);
	clean();

}

void Display7segmentMax7219::setIntensity(uint8_t intensivity)
{
	if (intensivity > 0x0F)
	{
		return;
	}

	sendData(static_cast<uint8_t>(Registers::REG_INTENSITY), intensivity);
}

void Display7segmentMax7219::clearDigit(uint8_t digit)
{
	uint8_t clear = decodeMode == 0xFF ? (uint8_t)Display7segmentMax7219::Letters::BLANK : 0x00;

	sendData(digit, clear);
}

void Display7segmentMax7219::clean(void){
	uint8_t clear = 0x00;

	if(decodeMode == 0xFF)
	{
		clear = (uint8_t)Display7segmentMax7219::Letters::BLANK;
	}

	for (int i = 0; i < 8; ++i)
	{
		sendData(i + 1, clear);
	}
}

void Display7segmentMax7219::sendData(uint8_t reg, uint8_t value)
{
	uint8_t tx_data[2] = { reg, value };
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&m_spi, tx_data, 2, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void Display7segmentMax7219::printDigit(int position, Letters numeric, bool point)
{
	if(position > 8)
	{
		return;
	}

	if(point)
	{
		if(decodeMode == 0x00)
		{
			sendData(position, SYMBOLS[(int)numeric] | (1 << 7));
		}
		else if(decodeMode == 0xFF)
		{
			sendData(position, (int)numeric | (1 << 7));
		}
	}
	else
	{
		if(decodeMode == 0x00)
		{
			sendData(position, SYMBOLS[(int)numeric]  & (~(1 << 7)));
		}
		else if(decodeMode == 0xFF)
		{
			sendData(position,(int)numeric & (~(1 << 7)));
		}
	}
}

int Display7segmentMax7219::print(int position, float value, uint8_t n)
{
	if(n > 4)
	{
		n = 4;
	}

	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), 0xFF);

	if (value < 0.0)
	{
		if(position > 0)
		{
			sendData(position, static_cast<uint8_t>(Letters::MINUS));
			position--;
		}

		value = -value;
	}

	position = printItos(position, (int32_t) value);

	if (n > 0u)
	{
		printDigit(position + 1, (Letters)(((int32_t) value) % 10), true);

		position = printNtos(position, (uint32_t) (value * (float) lcdPow10(n)), n);
	}

	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), decodeMode);

	return position;
}

int Display7segmentMax7219::printItos(int position, int value)
{
	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), 0xFF);

	int32_t numberOfDigits{};
	int length{};
	if (value < 0)
	{
		if(position > 0)
		{
			sendData(position, static_cast<uint8_t>(Letters::MINUS));
			position--;
		}
		value = -value;
		++length;
	}

	int tempValue = value;

	while (tempValue != 0)
	{
		tempValue /= 10;
		++numberOfDigits;
	}
	length += numberOfDigits;

	int rightCursor = position - numberOfDigits + 1;

	int trailingSpacesCount = position - numberOfDigits;


	tempValue = value;
	while (tempValue != 0)
	{
		int digit = tempValue % 10;
		tempValue /= 10;

		if (rightCursor >= 0)
		{
			sendData(rightCursor, digit);
		}
		++rightCursor;
	}
//	while(trailingSpacesCount-->0)
//	{
//		clearDigit(trailingSpacesCount);
//	}
//	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), decodeMode);


	return length;
}

int Display7segmentMax7219::printNtos(int position, uint32_t value, uint8_t n)
{
	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), 0xFF);

	if (n > 0u)
	{
		uint32_t i = lcdPow10(n - 1u);

		while (i > 0u)	/* Display at least one symbol */
		{
			if(position > 0u)
			{
				sendData(position, (value / i) % 10u);
				position--;
			}

			i /= 10u;
		}
	}

	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), decodeMode);

	return position;
}

void Display7segmentMax7219::turnOn(void)
{
	sendData(static_cast<uint8_t>(Registers::REG_SHUTDOWN), 0x01);
}

void Display7segmentMax7219::turnOff(void)
{
	sendData(static_cast<uint8_t>(Registers::REG_SHUTDOWN), 0x00);
}

void Display7segmentMax7219::setDecodeMode(void)
{
	decodeMode = 0xFF;
	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), decodeMode);
}

void Display7segmentMax7219::resetDecodeMode(void)
{
	decodeMode = 0x00;
	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), decodeMode);
}
