///*
// * liquidcrystali2c.cpp
// *
// *  Created on: Dec 6, 2022
// *      Author: VertexNi
// */
//
//#include "liquidcrystali2c.h"
//
//LiquidCrystalI2C::LiquidCrystalI2C(I2C_HandleTypeDef& hi2c, uint8_t address, uint8_t columns, uint8_t rows):
//m_hi2c(hi2c),
//m_address(address),
//_columns(columns),
//_rows(rows),
//_backlightval(static_cast<uint8_t>(BacklightFlags::LCD_BACKLIGHT))
//{
//	//sendCommand(0b00110000);   // 8ми битный интерфейс
//	//sendCommand(0b00000010);   // установка курсора в начале строки
//	//sendCommand(0b00001100);   // нормальный режим работы
//	//sendCommand(0b00000001);   // очистка дисплея
//	HAL_Delay(100);
//	sendCommand(0x30);
//	HAL_Delay(50);
//	sendCommand(0x30);
//	HAL_Delay(50);
//	sendCommand(0x30);
//	HAL_Delay(50);
//	sendCommand(0x20);
//	HAL_Delay(100);
//	sendCommand(0x28); //0010 1000 (Function set DL = 0 (4bit), N = 1 (2 line), F = 0 (5x8))
//	sendCommand(0x28); //0010 1000 (Function set DL = 0 (4bit), N = 1 (2 line), F = 0 (5x8))
//	HAL_Delay(1);
//	sendCommand(0x0C); // 0000 1000 (Display on/off Display = 0, Cursor = 0, Blinking = 0)
//	HAL_Delay(1);
//	sendCommand(0x01); // clear
//	HAL_Delay(1);
//	HAL_Delay(1);
//	sendCommand(0x06); // increment cursor automatically
//	HAL_Delay(1);
//	sendCommand(0x80); // set cursor to row 1, column 1
//	//sendCommand(_backlightval);
//	// 4-bit mode, 2 lines, 5x7 format
//	//sendCommand(0b00110000);
//	// display & cursor home (keep this!)
//	//sendCommand(0b00000010);
//	// display on, right shift, underline off, blink off
//	//sendCommand(0b00001100);
//	// clear display (optional here)
//	//sendCommand(0b00000001);
//	HAL_Delay(1000);
//	//blink();
//	//noBlink();
//}
//
//void LiquidCrystalI2C::clear()
//{
//	sendCommand(static_cast<uint8_t>(Commands::LCD_CLEARDISPLAY));
//}
//
//void LiquidCrystalI2C::showCursor()
//{
//	_displaycontrol |= static_cast<uint8_t>(DisplayControlFlags::LCD_DISPLAYON) | static_cast<uint8_t>(DisplayControlFlags::LCD_CURSORON);
//	sendCommand(static_cast<uint8_t>(Commands::LCD_DISPLAYCONTROL) | _displaycontrol);
//}
//
//void LiquidCrystalI2C::hideCursor()
//{
//	_displaycontrol &= ~static_cast<uint8_t>(DisplayControlFlags::LCD_CURSORON);
//	sendCommand(static_cast<uint8_t>(Commands::LCD_DISPLAYCONTROL) | _displaycontrol);
//}
//
//
//void LiquidCrystalI2C::send(uint8_t data, uint8_t isData)
//{
//	HAL_StatusTypeDef res;
//	uint8_t pinEnabled = 1 << 2;         // бит, по изменению сост. которого считывается информация
//	while(true)
//	{
//		res = HAL_I2C_IsDeviceReady(&m_hi2c, m_address, 1, 5);
//		if(res == HAL_OK) break;
//	}
//
//	uint8_t hi = data & 0xF0;
//	uint8_t low = (data << 4) & 0xF0;
//
//	uint8_t data_arr[]
//	 {
//		 hi|isData|_backlightval |pinEnabled,
//		 hi|isData|_backlightval ,
//		 low|isData|_backlightval |pinEnabled,
//		 low|isData|_backlightval
//	 };
//
//
//	HAL_I2C_Master_Transmit(&m_hi2c, m_address, data_arr, sizeof(data_arr), 5);
//	HAL_Delay(5);
//}
//
//void LiquidCrystalI2C::sendString(char *str)
//{
//	while(*str)
//	{
//		if (*str != '\n')
//		{
//			sendData((uint8_t)(*str));
//		}
//		else
//		{
//			setCursor(1, 0);
//		}
//		str++;
//	}
//}
//
//void LiquidCrystalI2C::turnOffBacklight()
//{
//	_backlightval = static_cast<uint8_t>(BacklightFlags::LCD_NOBACKLIGHT);
//	sendData(0);
//}
//
//void LiquidCrystalI2C::turnOnBacklight()
//{
//	_backlightval = static_cast<uint8_t>(BacklightFlags::LCD_BACKLIGHT);
//	sendData(0);
//}
//
//void LiquidCrystalI2C::autoscroll(void)
//{
//	_displaycontrol |= static_cast<uint8_t>(DisplayEntryModeds::LCD_ENTRYSHIFTINCREMENT);
//	sendCommand(static_cast<uint8_t>(Commands::LCD_ENTRYMODESET) | _displaycontrol);
//}
//
//void LiquidCrystalI2C::noAutoscroll(void)
//{
//	_displaycontrol &= ~static_cast<uint8_t>(DisplayEntryModeds::LCD_ENTRYSHIFTINCREMENT);
//	sendCommand(static_cast<uint8_t>(Commands::LCD_ENTRYMODESET) | _displaycontrol);
//}
//
//void LiquidCrystalI2C::blink()
//{
//	_displaycontrol |= static_cast<uint8_t>(DisplayControlFlags::LCD_DISPLAYON) | static_cast<uint8_t>(DisplayControlFlags::LCD_BLINKON);
//	sendCommand(static_cast<uint8_t>(Commands::LCD_DISPLAYCONTROL) | _displaycontrol);
//}
//
//void LiquidCrystalI2C::noBlink()
//{
//	_displaycontrol &= ~static_cast<uint8_t>(DisplayControlFlags::LCD_BLINKON);
//	sendCommand(static_cast<uint8_t>(Commands::LCD_DISPLAYCONTROL) | _displaycontrol);
//}
//
//void LiquidCrystalI2C::writeByte(uint8_t data)
//{
//	uint8_t pinEnabled = 1 << 2;
//	uint8_t low = (data << 4) & 0xF0;
//	uint8_t buf[2]{low|_displaycontrol|_backlightval|pinEnabled, low|_displaycontrol|_backlightval};
//	HAL_I2C_Master_Transmit(&m_hi2c,m_address, buf, sizeof(buf), 5);
//}
//
//void LiquidCrystalI2C::sendCommand(uint8_t value)
//{
//	send(value, 0);
//}
//void LiquidCrystalI2C::sendData(uint8_t value)
//{
//	send(value, 1);
//}
//
//void LiquidCrystalI2C::setCursor(uint8_t row, uint8_t column)
//{
//	uint8_t cellIndex = 0x40*row | column;
//	auto cmd = static_cast<uint8_t>(Commands::LCD_SETDDRAMADDR)|cellIndex;
//	sendCommand(cmd);
//}
//
//LiquidCrystalI2C::~LiquidCrystalI2C() {
//	// TODO Auto-generated destructor stub
//}
//
//
