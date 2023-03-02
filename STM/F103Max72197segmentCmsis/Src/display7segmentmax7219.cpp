/*
 * display7segmentmax7219.cpp
 *
 *  Created on: Nov 29, 2022
 *      Author: VertexNi
 */

#include "display7segmentmax7219.h"


static uint32_t getPow10n(uint8_t n)
{
	uint32_t retval = 1u;

	while (n > 0u)
	{
		retval *= 10u;
		n--;
	}

	return retval;
}

Display7segmentMax7219::~Display7segmentMax7219() {
	// TODO Auto-generated destructor stub
}

void Display7segmentMax7219::init(uint8_t intensity, uint8_t maxDigits){
	m_maxDigits = maxDigits;
	sendData(static_cast<uint8_t>(Registers::REG_DISPLAY_TEST), 0);
	setDecodeMode();
	sendData(static_cast<uint8_t>(Registers::REG_SCAN_LIMIT), m_maxDigits - 1);
	setIntensity(intensity);
	turnOn();
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
	uint8_t clear = decodeMode == 0xFF ? static_cast<uint8_t>(Display7segmentMax7219::Letters::BLANK) : 0x00;
	sendData(digit, clear);
}

void Display7segmentMax7219::clean(void){
	uint8_t clear = decodeMode == 0xFF ? static_cast<uint8_t>(Display7segmentMax7219::Letters::BLANK) : 0x00;

	for (int i = 0; i < 8; ++i)
	{
		sendData(i + 1, clear);
	}
}

void Display7segmentMax7219::sendData(uint8_t rg, uint8_t dt)
{
	m_spi.sendData(rg, dt);
}

void Display7segmentMax7219::printDigit(int position, Letters numeric, bool point)
{
	if(position > m_maxDigits)
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

int Display7segmentMax7219::print(float value, uint8_t digitsAfterPoint)
{
	int32_t numberOfDigits{getLengthInDigits(value)+digitsAfterPoint+int(value < 0)};
	setZeros(numberOfDigits);
	return print(value, digitsAfterPoint, numberOfDigits);
}

int Display7segmentMax7219::print(float value, uint8_t digitsAfterPoint, int position)
{
	if(digitsAfterPoint > 4)
	{
		digitsAfterPoint = 4;
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

	int decimal = (value - static_cast<int>(value))*getPow10n(digitsAfterPoint);
	position -= print(static_cast<int>(value), position, false, decimal !=0 || digitsAfterPoint>0);


	auto firstDecimalPosition = position + 1;
	while(position)
	{
		auto v = int(value*getPow10n(firstDecimalPosition - position))%10;
		sendData(position, v);
		--position;
	}

	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), decodeMode);

	return position;
}

int Display7segmentMax7219::print(int value)
{
	int32_t numberOfDigits{getLengthInDigits(value)+int(value < 0)};
	setZeros(numberOfDigits);
	return print(value, numberOfDigits, false, false);
}

int Display7segmentMax7219::print(int value, uint8_t position)
{
	return print(value, position, false, false);
}
int Display7segmentMax7219::print(int value, uint8_t position, bool asDecimalPart, bool withPoint)
{
	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), 0xFF);

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

	int32_t numberOfDigits{getLengthInDigits(value)};

	int rightCursor = position - numberOfDigits + 1;

	//int trailingSpacesCount = position - numberOfDigits;

	int tempValue = value;
	int lastIntDigit{tempValue%10};
	if (tempValue == 0)
	{
		if (!asDecimalPart)
		{
			sendData(rightCursor, 0);
			++length;
		}
		else
		{

		}
	}
	while (tempValue != 0)
	{
		int digit = tempValue % 10;
		tempValue /= 10;

		if (rightCursor >= 0)
		{
			sendData(rightCursor, digit);
			++length;
		}
		++rightCursor;
	}
	//	while(trailingSpacesCount>0)
	//	{
	//		clearDigit(trailingSpacesCount);
	//		--trailingSpacesCount;
	//	}
	if(withPoint)
	{
		if(decodeMode == 0x00)
		{
			sendData(position - numberOfDigits + 1, SYMBOLS[lastIntDigit] | (1 << 7));
		}
		else if(decodeMode == 0xFF)
		{
			sendData(position - numberOfDigits + 1, lastIntDigit | (1 << 7));
		}
	}
	// set back initial decode mode
	sendData(static_cast<uint8_t>(Registers::REG_DECODE_MODE), decodeMode);

	return length;
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
