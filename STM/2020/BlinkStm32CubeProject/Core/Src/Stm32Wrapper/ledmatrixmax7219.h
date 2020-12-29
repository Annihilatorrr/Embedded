/*
 * ledmatrixmax7219.h
 *
 *  Created on: 30 дек. 2020 г.
 *      Author: rusla
 */

#ifndef SRC_STM32WRAPPER_LEDMATRIXMAX7219_H_
#define SRC_STM32WRAPPER_LEDMATRIXMAX7219_H_

#include "max7219base.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

class LedMatrixMax7219: public Max7219Base
{
	uint8_t buffer[80]{};
	uint8_t buffer_row[80]{};
public:
	LedMatrixMax7219();
	void sendCommand(uint8_t address, uint8_t command, int numOfMatrix);
	void setled(uint8_t row, uint8_t col, uint8_t value, int numOfMatrix);
	void maxInit (uint8_t brightness);
	void maxClear(int numOfMatrix);
	void setrow(uint8_t row, uint8_t value, int numOfMatrix);
	void writeChar (char c, uint8_t max);
	void shiftLeft(int numOfMatrix);
	void shiftRight(int numOfMatrix);
	void shiftChar (char c, uint32_t speed, char direction, int numOfMatrix);
	void scrollString (uint8_t *string, uint32_t speed, char direction, int numOfMatrix);
};
#endif /* SRC_STM32WRAPPER_LEDMATRIXMAX7219_H_ */
