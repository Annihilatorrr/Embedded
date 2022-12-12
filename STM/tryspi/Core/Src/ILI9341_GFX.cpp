#include "ILI9341_GFX.h"

volatile uint16_t LCD_HEIGHT = SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH	 = SCREEN_WIDTH;

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Deselect when Tx Complete */
  //if(hspi == HSPI_INSTANCE)
  {
	  HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
  }
}

Ili9341TftDisplay::Ili9341TftDisplay(SPI_HandleTypeDef& hspi):
		m_hspi(hspi)
{
	Init();
}

void Ili9341TftDisplay::SPI_Tx(uint8_t data)
{
	while(!__HAL_SPI_GET_FLAG(&m_hspi, SPI_FLAG_TXE));
	HAL_SPI_Transmit_DMA(&m_hspi, &data, 1);
	//HAL_SPI_Transmit(HSPI_INSTANCE, &data, 1, 10);
}

void Ili9341TftDisplay::SPI_TxBuffer(uint8_t *buffer, uint16_t len)
{
	while(!__HAL_SPI_GET_FLAG(&m_hspi, SPI_FLAG_TXE));
	HAL_SPI_Transmit_DMA(&m_hspi, buffer, len);
	//HAL_SPI_Transmit(HSPI_INSTANCE, buffer, len, 10);
}

void Ili9341TftDisplay::WriteCommand(uint8_t cmd)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);	//command
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	SPI_Tx(cmd);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void Ili9341TftDisplay::WriteData(uint8_t data)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	//data
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	SPI_Tx(data);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void Ili9341TftDisplay::WriteBuffer(uint8_t *buffer, uint16_t len)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	//data
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	SPI_TxBuffer(buffer, len);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void Ili9341TftDisplay::SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint8_t buffer[4] {x1 >> 8, buffer[1] = x1, buffer[2] = x2 >> 8, x2};

	WriteCommand(0x2A);
	WriteBuffer(buffer, sizeof(buffer));

	buffer[0] = y1 >> 8;
	buffer[1] = y1;
	buffer[2] = y2 >> 8;
	buffer[3] = y2;

	WriteCommand(0x2B);
	WriteBuffer(buffer, sizeof(buffer));

	WriteCommand(0x2C);
}

void Ili9341TftDisplay::Reset(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);	//Disable
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);		//Select
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);		//Enable
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET); 		//Deselect
}

void Ili9341TftDisplay::Enable(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);		//Enable
}

void Ili9341TftDisplay::Init(void)
{
	Enable();
	Reset();

	//SOFTWARE RESET
	WriteCommand(0x01);
	HAL_Delay(10);

	//POWER CONTROL A
	WriteCommand(0xCB);
	WriteData(0x39);
	WriteData(0x2C);
	WriteData(0x00);
	WriteData(0x34);
	WriteData(0x02);

	//POWER CONTROL B
	WriteCommand(0xCF);
	WriteData(0x00);
	WriteData(0xC1);
	WriteData(0x30);

	//DRIVER TIMING CONTROL A
	WriteCommand(0xE8);
	WriteData(0x85);
	WriteData(0x00);
	WriteData(0x78);

	//DRIVER TIMING CONTROL B
	WriteCommand(0xEA);
	WriteData(0x00);
	WriteData(0x00);

	//POWER ON SEQUENCE CONTROL
	WriteCommand(0xED);
	WriteData(0x64);
	WriteData(0x03);
	WriteData(0x12);
	WriteData(0x81);

	//PUMP RATIO CONTROL
	WriteCommand(0xF7);
	WriteData(0x20);

	//POWER CONTROL,VRH[5:0]
	WriteCommand(0xC0);
	WriteData(0x23);

	//POWER CONTROL,SAP[2:0];BT[3:0]
	WriteCommand(0xC1);
	WriteData(0x10);

	//VCM CONTROL
	WriteCommand(0xC5);
	WriteData(0x3E);
	WriteData(0x28);

	//VCM CONTROL 2
	WriteCommand(0xC7);
	WriteData(0x86);

	//MEMORY ACCESS CONTROL
	WriteCommand(0x36);
	WriteData(0x48);

	//PIXEL FORMAT
	WriteCommand(0x3A);
	WriteData(0x55);

	//FRAME RATIO CONTROL, STANDARD RGB COLOR
	WriteCommand(0xB1);
	WriteData(0x00);
	WriteData(0x18);

	//DISPLAY FUNCTION CONTROL
	WriteCommand(0xB6);
	WriteData(0x08);
	WriteData(0x82);
	WriteData(0x27);

	//3GAMMA FUNCTION DISABLE
	WriteCommand(0xF2);
	WriteData(0x00);

	//GAMMA CURVE SELECTED
	WriteCommand(0x26);
	WriteData(0x01);

	//POSITIVE GAMMA CORRECTION
	WriteCommand(0xE0);
	WriteData(0x0F);
	WriteData(0x31);
	WriteData(0x2B);
	WriteData(0x0C);
	WriteData(0x0E);
	WriteData(0x08);
	WriteData(0x4E);
	WriteData(0xF1);
	WriteData(0x37);
	WriteData(0x07);
	WriteData(0x10);
	WriteData(0x03);
	WriteData(0x0E);
	WriteData(0x09);
	WriteData(0x00);

	//NEGATIVE GAMMA CORRECTION
	WriteCommand(0xE1);
	WriteData(0x00);
	WriteData(0x0E);
	WriteData(0x14);
	WriteData(0x03);
	WriteData(0x11);
	WriteData(0x07);
	WriteData(0x31);
	WriteData(0xC1);
	WriteData(0x48);
	WriteData(0x08);
	WriteData(0x0F);
	WriteData(0x0C);
	WriteData(0x31);
	WriteData(0x36);
	WriteData(0x0F);

	//EXIT SLEEP
	WriteCommand(0x11);
	HAL_Delay(100);

	//TURN ON DISPLAY
	WriteCommand(0x29);

	//STARTING ROTATION
	SetRotation(SCREEN_VERTICAL_1);
}

void Ili9341TftDisplay::SetRotation(uint8_t rotation)
{
	WriteCommand(0x36);
	HAL_Delay(1);

	switch(rotation)
	{
	case SCREEN_VERTICAL_1:
		WriteData(0x40|0x08);
		LCD_WIDTH = 240;
		LCD_HEIGHT = 320;
		break;
	case SCREEN_HORIZONTAL_1:
		WriteData(0x20|0x08);
		LCD_WIDTH  = 320;
		LCD_HEIGHT = 240;
		break;
	case SCREEN_VERTICAL_2:
		WriteData(0x80|0x08);
		LCD_WIDTH  = 240;
		LCD_HEIGHT = 320;
		break;
	case SCREEN_HORIZONTAL_2:
		WriteData(0x40|0x80|0x20|0x08);
		LCD_WIDTH  = 320;
		LCD_HEIGHT = 240;
		break;
	default:
		break;
	}
}

void Ili9341TftDisplay::DrawColor(uint16_t color)
{
	uint8_t buffer[2] = {color>>8, color};
	WriteBuffer(buffer, sizeof(buffer));
}

void Ili9341TftDisplay::DrawColorBurst(uint16_t color, uint32_t size)
{
	uint32_t bufferSize = ((size<<1) < BURST_MAX_SIZE)?size:BURST_MAX_SIZE;

	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);

	uint8_t chifted = color >> 8;
	uint8_t BurstBuffer[bufferSize];

	for(uint32_t j = 0; j < bufferSize; j+=2)
	{
		BurstBuffer[j] = chifted;
		BurstBuffer[j+1] = color;
	}

	uint32_t SendingSize = size<<1;
	uint32_t SendingInBlock = SendingSize / bufferSize;
	uint32_t RemainderFromBlock = SendingSize % bufferSize;

	if(SendingInBlock != 0)
	{
		for(uint32_t j = 0; j < (SendingInBlock); j++)
		{
			HAL_SPI_Transmit(&m_hspi, BurstBuffer, bufferSize, 10);
		}
	}

	HAL_SPI_Transmit(&m_hspi, BurstBuffer, RemainderFromBlock, 10);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void Ili9341TftDisplay::FillScreen(uint16_t color)
{
	SetAddress(0, 0, LCD_WIDTH, LCD_HEIGHT);
	DrawColorBurst(color, LCD_WIDTH*LCD_HEIGHT);
}

void Ili9341TftDisplay::DrawPixel(uint16_t x,uint16_t y,uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	uint8_t bufferX[4] = {x>>8, x, (x+1)>>8, (x+1)};
	uint8_t bufferY[4] = {y>>8, y, (y+1)>>8, (y+1)};
	uint8_t bufferC[2] = {color>>8, color};

	WriteCommand(0x2A);						//ADDRESS
	WriteBuffer(bufferX, sizeof(bufferX));	//XDATA

	WriteCommand(0x2B);						//ADDRESS
	WriteBuffer(bufferY, sizeof(bufferY));	//YDATA

	WriteCommand(0x2C);						//ADDRESS
	WriteBuffer(bufferC, sizeof(bufferC));	//COLOR
}

void Ili9341TftDisplay::DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((x+width-1)>=LCD_WIDTH)
	{
		width=LCD_WIDTH-x;
	}

	if((y+height-1)>=LCD_HEIGHT)
	{
		height=LCD_HEIGHT-y;
	}

	SetAddress(x, y, x+width-1, y+height-1);
	DrawColorBurst(color, height*width);
}

void Ili9341TftDisplay::DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((x+width-1)>=LCD_WIDTH)
	{
		width=LCD_WIDTH-x;
	}

	SetAddress(x, y, x+width-1, y);
	DrawColorBurst(color, width);
}

void Ili9341TftDisplay::DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((y+height-1)>=LCD_HEIGHT)
	{
		height=LCD_HEIGHT-y;
	}

	SetAddress(x, y, x, y+height-1);
	DrawColorBurst(color, height);
}

/* imprecise small delay */
__STATIC_INLINE void DelayUs(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000);
	while (us--);
}

void Ili9341TftDisplay::DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color)
{
	int x = radius-1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y)
	{
		DrawPixel(X + x, Y + y, color);
		DrawPixel(X + y, Y + x, color);
		DrawPixel(X - y, Y + x, color);
		DrawPixel(X - x, Y + y, color);
		DrawPixel(X - x, Y - y, color);
		DrawPixel(X - y, Y - x, color);
		DrawPixel(X + y, Y - x, color);
		DrawPixel(X + x, Y - y, color);

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += (-radius << 1) + dx;
		}
	}
}

void Ili9341TftDisplay::DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color)
{

	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;

	while (x >= y)
	{
		for (int i = X - x; i <= X + x; i++)
		{
			DrawPixel(i, Y + y,color);
			DrawPixel(i, Y - y,color);
		}

		for (int i = X - y; i <= X + y; i++)
		{
			DrawPixel(i, Y + x,color);
			DrawPixel(i, Y - x,color);
		}

		y++;
		radiusError += yChange;
		yChange += 2;

		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

void Ili9341TftDisplay::DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color)
{
	uint16_t xLen = 0;
	uint16_t yLen = 0;
	uint8_t negX = 0;
	uint8_t negY = 0;
	float negCalc = 0;

	negCalc = X1 - X0;
	if(negCalc < 0) negX = 1;
	negCalc = 0;

	negCalc = Y1 - Y0;
	if(negCalc < 0) negY = 1;

	//DRAW HORIZONTAL!
	if(!negX)
	{
		xLen = X1 - X0;
	}
	else
	{
		xLen = X0 - X1;
	}
	DrawHLine(X0, Y0, xLen, color);
	DrawHLine(X0, Y1, xLen, color);

	//DRAW VERTICAL!
	if(!negY)
	{
		yLen = Y1 - Y0;
	}
	else
	{
		yLen = Y0 - Y1;
	}

	DrawVLine(X0, Y0, yLen, color);
	DrawVLine(X1, Y0, yLen, color);

	if((xLen > 0)||(yLen > 0))
	{
		DrawPixel(X1, Y1, color);
	}
}

void Ili9341TftDisplay::DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color)
{
	uint16_t xLen = 0;
	uint16_t yLen = 0;
	uint8_t negX = 0;
	uint8_t negY = 0;
	int32_t negCalc = 0;
	uint16_t X0True = 0;
	uint16_t Y0True = 0;

	negCalc = X1 - X0;
	if(negCalc < 0) negX = 1;
	negCalc = 0;

	negCalc = Y1 - Y0;
	if(negCalc < 0) negY = 1;

	if(!negX)
	{
		xLen = X1 - X0;
		X0True = X0;
	}
	else
	{
		xLen = X0 - X1;
		X0True = X1;
	}

	if(!negY)
	{
		yLen = Y1 - Y0;
		Y0True = Y0;
	}
	else
	{
		yLen = Y0 - Y1;
		Y0True = Y1;
	}

	DrawRectangle(X0True, Y0True, xLen, yLen, color);
}

void Ili9341TftDisplay::DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor)
{
	if ((ch < 31) || (ch > 127)) return;

	uint8_t fOffset, fWidth, fHeight, fBPL;
	uint8_t *tempChar;

	fOffset = font[0];
	fWidth = font[1];
	fHeight = font[2];
	fBPL = font[3];

	tempChar = (uint8_t*)&font[((ch - 0x20) * fOffset) + 4]; /* Current Character = Meta + (Character Index * Offset) */

	/* Clear background first */
	DrawRectangle(X, Y, fWidth, fHeight, bgcolor);

	for (int j=0; j < fHeight; j++)
	{
		for (int i=0; i < fWidth; i++)
		{
			uint8_t z =  tempChar[fBPL * i + ((j & 0xF8) >> 3) + 1]; /* (j & 0xF8) >> 3, increase one by 8-bits */
			uint8_t b = 1 << (j & 0x07);
			if (( z & b ) != 0x00)
			{
				DrawPixel(X+i, Y+j, color);
			}
		}
	}
}

void Ili9341TftDisplay::DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor)
{
	uint8_t charWidth;			/* Width of character */
	uint8_t fOffset = font[0];	/* Offset of character */
	uint8_t fWidth = font[1];	/* Width of font */

	while (*str)
	{
		DrawChar(*str, font, X, Y, color, bgcolor);

		/* Check character width and calculate proper position */
		uint8_t *tempChar = (uint8_t*)&font[((*str - 0x20) * fOffset) + 4];
		charWidth = tempChar[0];

		if(charWidth + 2 < fWidth)
		{
			/* If character width is smaller than font width */
			X += (charWidth + 2);
		}
		else
		{
			X += fWidth;
		}

		str++;
	}
}

void Ili9341TftDisplay::DrawImage(const uint8_t* image, uint8_t orientation)
{
	if(orientation == SCREEN_HORIZONTAL_1)
	{
		SetRotation(SCREEN_HORIZONTAL_1);
		SetAddress(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	}
	else if(orientation == SCREEN_HORIZONTAL_2)
	{
		SetRotation(SCREEN_HORIZONTAL_2);
		SetAddress(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	}
	else if(orientation == SCREEN_VERTICAL_2)
	{
		SetRotation(SCREEN_VERTICAL_2);
		SetAddress(0,0,SCREEN_HEIGHT,SCREEN_WIDTH);
	}
	else if(orientation == SCREEN_VERTICAL_1)
	{
		SetRotation(SCREEN_VERTICAL_1);
		SetAddress(0,0,SCREEN_HEIGHT,SCREEN_WIDTH);
	}

	uint32_t counter = 0;
	for(uint32_t i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
	{
		WriteBuffer((uint8_t*)(image + counter), BURST_MAX_SIZE);
		counter += BURST_MAX_SIZE;

		/* DMA Tx is too fast, It needs some delay */
		DelayUs(1);
	}
}
