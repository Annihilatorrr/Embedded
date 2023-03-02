/*
 * display7segmentmax7219.h
 *
 *  Created on: Nov 29, 2022
 *      Author: VertexNi
 */

#ifndef SRC_DISPLAY7SEGMENTMAX7219_H_
#define SRC_DISPLAY7SEGMENTMAX7219_H_

#include "stm32f1xx.h"
#include "spi.h"

class Display7segmentMax7219
{
	static uint8_t SYMBOLS[16];

	uint8_t decodeMode;
	Spi<Controller::f103> m_spi;
	uint16_t m_spiCsPin;
	uint8_t m_maxDigits;
	uint8_t getLengthInDigits(int value)
	{
		uint8_t numberOfDigits{};
		if (value == 0)
		{
			numberOfDigits = 1;
		}
		while (value != 0)
		{
			value /= 10;
			++numberOfDigits;
		}
		return numberOfDigits;
	}

	void setZeros(uint8_t digits)
	{
		uint8_t clear = decodeMode == 0xFF ? static_cast<uint8_t>(Display7segmentMax7219::Letters::NUM_0) : 0x00;

		for (int i = 0; i < digits;++i)
		{
			sendData(i, clear);
		}
	}
	//	uint8_t getLengthInDigits(float value)
	//	{
	//		uint8_t numberOfIntPartDigits{getLengthInDigits(static_cast<uint8_t>(value))};
	//
	//		while (value != 0)
	//		{
	//			value /= 10;
	//			++numberOfDigits;
	//		}
	//		return numberOfDigits;
	//	}
public:

	enum class Registers
	{
		REG_NO_OP 			= 0x00,
				REG_DIGIT_0 		= 0x01,
				REG_DIGIT_1 		= 0x02,
				REG_DIGIT_2 		= 0x03,
				REG_DIGIT_3 		= 0x04,
				REG_DIGIT_4 		= 0x05,
				REG_DIGIT_5 		= 0x06,
				REG_DIGIT_6 		= 0x07,
				REG_DIGIT_7 		= 0x08,
				REG_DECODE_MODE 	= 0x09,
				REG_INTENSITY 		= 0x0A,
				REG_SCAN_LIMIT 		= 0x0B,
				REG_SHUTDOWN 		= 0x0C,
				REG_DISPLAY_TEST 	= 0x0F
	};

	enum class Digits {
		DIGIT_1 = 1, DIGIT_2 = 2, DIGIT_3 = 3, DIGIT_4 = 4,
				DIGIT_5 = 5, DIGIT_6 = 6, DIGIT_7 = 7, DIGIT_8 = 8
	};

	enum class Letters {
		NUM_0		= 0x00,
				NUM_1		= 0x01,
				NUM_2		= 0x02,
				NUM_3		= 0x03,
				NUM_4		= 0x04,
				NUM_5		= 0x05,
				NUM_6		= 0x06,
				NUM_7		= 0x07,
				NUM_8		= 0x08,
				NUM_9		= 0x09,
				MINUS		= 0x0A,
				LETTER_E	= 0x0B,
				LETTER_H	= 0x0C,
				LETTER_L	= 0x0D,
				LETTER_P	= 0x0E,
				BLANK		= 0x0F
	};

	Display7segmentMax7219(Spi<Controller::f103>::SpiNumber spiNumber, Spi<Controller::f103>::SpiFrameSize frameSize):
		m_spi(spiNumber, frameSize)
	{}

	void init(uint8_t intensivity, uint8_t maxDigits);
	void printDigit(int position, Letters numeric, bool point);
	void sendData(uint8_t reg, uint8_t value);
	void clean(void);
	void clearDigit(uint8_t digit);
	void turnOn(void);
	void turnOff(void);
	void setDecodeMode(void);
	void resetDecodeMode(void);
	void setIntensity(uint8_t intensivity);
	int print(float value, uint8_t digitsAfterPoint);
	int print(float value, uint8_t digitsAfterPoint, int position);
	int print(int value);
	int print(int value, uint8_t position);
	int print(int value, uint8_t position, bool asDecimalart, bool withPoint = false);
	virtual ~Display7segmentMax7219();
};

inline uint8_t Display7segmentMax7219::SYMBOLS[]
{
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
#endif /* SRC_DISPLAY7SEGMENTMAX7219_H_ */
