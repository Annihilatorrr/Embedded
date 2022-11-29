/*
 * display7segmentmax7219.h
 *
 *  Created on: Nov 29, 2022
 *      Author: VertexNi
 */

#ifndef SRC_DISPLAY7SEGMENTMAX7219_H_
#define SRC_DISPLAY7SEGMENTMAX7219_H_

#include "stm32f1xx_hal.h"

class Display7segmentMax7219 {
	SPI_HandleTypeDef& m_spi;
	uint8_t decodeMode;
public:


	static uint8_t SYMBOLS[16];

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

	Display7segmentMax7219(SPI_HandleTypeDef& spi):m_spi(spi){}

	void init(uint8_t intensivity);
	void printDigit(int position, Letters numeric, bool point);
	void sendData(uint8_t reg, uint8_t value);
	void clean(void);
	void clearDigit(uint8_t digit);
	void turnOn(void);
	void turnOff(void);
	void setDecodeMode(void);
	void resetDecodeMode(void);
	void setIntensity(uint8_t intensivity);
	int print(int position, float value, uint8_t n);
	int printNtos(int position, uint32_t value, uint8_t n);
	int printItos(int position, int value);
	virtual ~Display7segmentMax7219();
};

#endif /* SRC_DISPLAY7SEGMENTMAX7219_H_ */
