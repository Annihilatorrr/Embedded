#ifndef STM32_DRIVER_H
#define STM32_DRIVER_H

#include "stm32f1xx_hal.h"

#define SCREEN_HEIGHT 	240
#define SCREEN_WIDTH 	320

#define HORIZONTAL_IMAGE	0
#define VERTICAL_IMAGE		1



/* PIN Configuration */

#define LCD_CS_PORT				GPIOB
#define LCD_CS_PIN				GPIO_PIN_10
#define LCD_DC_PORT				GPIOB
#define LCD_DC_PIN				GPIO_PIN_0
#define	LCD_RST_PORT			GPIOB
#define	LCD_RST_PIN				GPIO_PIN_1

#define BURST_MAX_SIZE 			500
#define BLACK      				0x0000
#define NAVY        			0x000F
#define DARKGREEN   			0x03E0
#define DARKCYAN    			0x03EF
#define MAROON      			0x7800
#define PURPLE      			0x780F
#define OLIVE       			0x7BE0
#define LIGHTGREY   			0xC618
#define DARKGREY    			0x7BEF
#define BLUE        			0x001F
#define GREEN       			0x07E0
#define CYAN        			0x07FF
#define RED         			0xF800
#define MAGENTA     			0xF81F
#define YELLOW      			0xFFE0
#define WHITE       			0xFFFF
#define ORANGE      			0xFD20
#define GREENYELLOW 			0xAFE5
#define PINK        			0xF81F

#define SCREEN_VERTICAL_1		0
#define SCREEN_HORIZONTAL_1		1
#define SCREEN_VERTICAL_2		2
#define SCREEN_HORIZONTAL_2		3

class Ili9341TftDisplay
{
	SPI_HandleTypeDef& m_hspi;
	void Init(void);
public:
	Ili9341TftDisplay(SPI_HandleTypeDef& hspi1);
	void WriteCommand(uint8_t cmd);
	void SPI_Tx(uint8_t data);
	void SPI_TxBuffer(uint8_t *buffer, uint16_t len);
	void WriteData(uint8_t data);
	void WriteBuffer(uint8_t *buffer, uint16_t len);
	void Reset(void);
	void Enable(void);

	void SetRotation(uint8_t rotation);
	void SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void DrawColor(uint16_t color);
	void DrawColorBurst(uint16_t color, uint32_t size);
	void FillScreen(uint16_t color);
	void DrawPixel(uint16_t x,uint16_t y,uint16_t color);
	void DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
	void DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color);
	void DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color);
	void DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
	void DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
	void DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
	void DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
	void DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
	void DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
	void DrawImage(const uint8_t* image, uint8_t orientation);
};
#endif
