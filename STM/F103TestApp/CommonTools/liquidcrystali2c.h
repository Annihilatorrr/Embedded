/*
 * liquidcrystali2c.h
 *
 *  Created on: Dec 6, 2022
 *      Author: VertexNi
 */

#ifndef LIQUIDCRYSTALI2C_H_
#define LIQUIDCRYSTALI2C_H_

#include "stm32f1xx_hal.h"

class LiquidCrystalI2C
{
	I2C_HandleTypeDef& m_hi2c;
	uint8_t m_address;
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _numlines;
	uint8_t _columns;
	uint8_t _rows;
	uint8_t _backlightval;

	void send(uint8_t data, uint8_t flags);

	void writeByte(uint8_t data);

public:
	LiquidCrystalI2C(I2C_HandleTypeDef& m_hi2c, uint8_t address, uint8_t columns, uint8_t rows);
	virtual ~LiquidCrystalI2C();

	void blink();
	void noBlink();
	void clear();
	void showCursor();
	void hideCursor();
	void sendCommand(uint8_t data);
	void sendValue(uint8_t data);
	void sendString(char *str);
	enum class BacklightFlags
	{
		LCD_BACKLIGHT = 0x08,
		LCD_NOBACKLIGHT = 0x00
	};

	enum class DisplayControlFlags
	{
		LCD_DISPLAYON = 0x04,
		LCD_DISPLAYOFF = 0x00,
		LCD_CURSORON = 0x02,
		LCD_CURSOROFF = 0x00,
		LCD_BLINKON = 0x01,
		LCD_BLINKOFF = 0x00
	};

	enum class Commands
	{
		LCD_CLEARDISPLAY = 0x01,
		LCD_RETURNHOME = 0x02,
		LCD_ENTRYMODESET = 0x04,
		LCD_DISPLAYCONTROL = 0x08,
		LCD_CURSORSHIFT = 0x10,
		LCD_FUNCTIONSET = 0x20,
		LCD_SETCGRAMADDR = 0x40,
		LCD_SETDDRAMADDR = 0x80
	};
};

#endif /* LIQUIDCRYSTALI2C_H_ */
