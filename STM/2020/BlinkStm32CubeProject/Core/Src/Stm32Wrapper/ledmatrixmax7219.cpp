/*
 * ledmatrixmax7219.cpp
 *
 *  Created on: 30 дек. 2020 г.
 *      Author: rusla
 */

#include "ledmatrixmax7219.h"

struct Character
{
	int width{};
	int columns[8]{};
};

Character characters[] =
{
		{0, {0, 0, 0, 0, 0, 0, 0, 0}},  // 0 - 'Empty Cell'
		{5, {0x3e, 0x5b, 0x4f, 0x5b, 0x3e, 0, 0, 0}}, // 1 - 'Sad Smiley'
		{5, {0x3e, 0x6b, 0x4f, 0x6b, 0x3e, 0, 0, 0}},  // 2 - 'Happy Smiley'
		{5, {0x1c, 0x3e, 0x7c, 0x3e, 0x1c, 0, 0, 0}},  // 3 - 'Heart'
		{5, {0x18, 0x3c, 0x7e, 0x3c, 0x18, 0, 0, 0}},  // 4 - 'Diamond'
		{5, {0x1c, 0x57, 0x7d, 0x57, 0x1c, 0, 0, 0}},  // 5 - 'Clubs'
		{5, {0x1c, 0x5e, 0x7f, 0x5e, 0x1c, 0, 0, 0}},  // 6 - 'Spades'
		{4, {0x00, 0x18, 0x3c, 0x18, 0, 0, 0, 0}},  // 7 - 'Bullet Point'
		{5, {0xff, 0xe7, 0xc3, 0xe7, 0xff, 0, 0, 0}},  // 8 - 'Rev Bullet Point'
		{4, {0x00, 0x18, 0x24, 0x18, 0, 0, 0, 0}},  // 9 - 'Hollow Bullet Point'
		{5, {0xff, 0xe7, 0xdb, 0xe7, 0xff, 0, 0, 0}},  // 10 - 'Rev Hollow BP'
		{5, {0x30, 0x48, 0x3a, 0x06, 0x0e, 0, 0, 0}},  // 11 - 'Male'
		{5, {0x26, 0x29, 0x79, 0x29, 0x26, 0, 0, 0}},  // 12 - 'Female'
		{5, {0x40, 0x7f, 0x05, 0x05, 0x07, 0, 0, 0}},  // 13 - 'Music Note 1'
		{5, {0x40, 0x7f, 0x05, 0x25, 0x3f, 0, 0, 0}},  // 14 - 'Music Note 2'
		{5, {0x5a, 0x3c, 0xe7, 0x3c, 0x5a, 0, 0, 0}},  // 15 - 'Snowflake'
		{5, {0x7f, 0x3e, 0x1c, 0x1c, 0x08, 0, 0, 0}},  // 16 - 'Right Pointer'
		{5, {0x08, 0x1c, 0x1c, 0x3e, 0x7f, 0, 0, 0}},  // 17 - 'Left Pointer'
		{5, {0x14, 0x22, 0x7f, 0x22, 0x14, 0, 0, 0}},  // 18 - 'UpDown Arrows'
		{5, {0x5f, 0x5f, 0x00, 0x5f, 0x5f, 0, 0, 0}},  // 19 - 'Double Exclamation'
		{5, {0x06, 0x09, 0x7f, 0x01, 0x7f, 0, 0, 0}},  // 20 - 'Paragraph Mark'
		{4, {0x66, 0x89, 0x95, 0x6a, 0, 0, 0, 0}},  // 21 - 'Section Mark'
		{5, {0x60, 0x60, 0x60, 0x60, 0x60, 0, 0, 0}},  // 22 - 'Double Underline'
		{5, {0x94, 0xa2, 0xff, 0xa2, 0x94, 0, 0, 0}},  // 23 - 'UpDown Underlined'
		{5, {0x08, 0x04, 0x7e, 0x04, 0x08, 0, 0, 0}},  // 24 - 'Up Arrow'
		{5, {0x10, 0x20, 0x7e, 0x20, 0x10, 0, 0, 0}},  // 25 - 'Down Arrow'
		{5, {0x08, 0x08, 0x2a, 0x1c, 0x08, 0, 0, 0}},  // 26 - 'Right Arrow'
		{5, {0x08, 0x1c, 0x2a, 0x08, 0x08, 0, 0, 0}},  // 27 - 'Left Arrow'
		{5, {0x1e, 0x10, 0x10, 0x10, 0x10, 0, 0, 0}},  // 28 - 'Angled'
		{5, {0x0c, 0x1e, 0x0c, 0x1e, 0x0c, 0, 0, 0}},  // 29 - 'Squashed #'
		{5, {0x30, 0x38, 0x3e, 0x38, 0x30, 0, 0, 0}},  // 30 - 'Up Pointer'
		{5, {0x06, 0x0e, 0x3e, 0x0e, 0x06, 0, 0, 0}},  // 31 - 'Down Pointer'
		{3, {0x00, 0x00, 0x00, 0, 0, 0, 0, 0}}, // 32 - 'Space'
		{1, {0x5f, 0, 0, 0, 0, 0, 0, 0}}, // 33 - '!'
		{3, {0x07, 0x00, 0x07, 0, 0, 0, 0, 0}}, // 34 - '"'
		{5, {0x14, 0x7f, 0x14, 0x7f, 0x14, 0, 0, 0}},   // 35 - '#'
		{5, {0x24, 0x2a, 0x7f, 0x2a, 0x12, 0, 0, 0}},  // 36 - '$'
		{5, {0x23, 0x13, 0x08, 0x64, 0x62, 0, 0, 0}},  // 37 - '%'
		{5, {0x36, 0x49, 0x56, 0x20, 0x50, 0, 0, 0}},  // 38 - '&'
		{3, {0x08, 0x07, 0x03, 0, 0, 0, 0, 0}},  // 39 - '''
		{3, {0x1c, 0x22, 0x41, 0, 0, 0, 0, 0}},  // 40 - '('
		{3, {0x41, 0x22, 0x1c, 0, 0, 0, 0, 0}},  // 41 - ')'
		{5, {0x2a, 0x1c, 0x7f, 0x1c, 0x2a, 0, 0, 0}}, // 42 - '*'
		{5, {0x08, 0x08, 0x3e, 0x08, 0x08, 0, 0, 0}},  // 43 - '+'
		{3, {0x80, 0x70, 0x30, 0, 0, 0, 0, 0}},  // 44 - ','
		{5, {0x08, 0x08, 0x08, 0x08, 0x08, 0, 0, 0}},  // 45 - '-'
		{2, {0x61, 0x60, 0, 0, 0, 0, 0, 0}},  // 46 - '.'
		{5, {0x20, 0x10, 0x08, 0x04, 0x02, 0, 0, 0}},  // 47 - '/'
		{5, {0x3e, 0x51, 0x49, 0x45, 0x3e, 0, 0, 0}},  // 48 - '0'
		{3, {0x42, 0x7f, 0x40, 0, 0, 0, 0, 0}},  // 49 - '1'
		{5, {0x72, 0x49, 0x49, 0x49, 0x46, 0, 0, 0}},  // 50 - '2'
		{5, {0x21, 0x41, 0x49, 0x4d, 0x33, 0, 0, 0}},  // 51 - '3'
		{8, {0x18, 0x14, 0x12, 0x7f, 0x10, 0, 0, 0}},  // 52 - '4'
		{5, {0x27, 0x45, 0x45, 0x45, 0x39, 0, 0, 0}},  // 53 - '5'
		{5, {0x3c, 0x4a, 0x49, 0x49, 0x31, 0, 0, 0}},  // 54 - '6'
		{5, {0x41, 0x21, 0x11, 0x09, 0x07, 0, 0, 0}},  // 55 - '7'
		{5, {0x36, 0x49, 0x49, 0x49, 0x36, 0, 0, 0}},  // 56 - '8'
		{5, {0x46, 0x49, 0x49, 0x29, 0x1e, 0, 0, 0}},  // 57 - '9'
		{1, {0x14, 0, 0, 0, 0, 0, 0, 0}},  // 58 - ':'
		{2, {0x80, 0x68, 0, 0, 0, 0, 0, 0}},  // 59 - ';'
		{4, {0x08, 0x14, 0x22, 0x41, 0, 0, 0, 0}},  // 60 - '<'
		{5, {0x14, 0x14, 0x14, 0x14, 0x14, 0, 0, 0}},  // 61 - '='
		{4, {0x41, 0x22, 0x14, 0x08, 0, 0, 0, 0}},  // 62 - '>'
		{5, {0x02, 0x01, 0x59, 0x09, 0x06, 0, 0, 0}},  // 63 - '?'
		{5, {0x3e, 0x41, 0x5d, 0x59, 0x4e, 0, 0, 0}},  // 64 - '@'
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},// 65 - 'A'
		{5, {0x7f, 0x49, 0x49, 0x49, 0x36, 0, 0, 0}},// 66 - 'B'
		{5, {0x3e, 0x41, 0x41, 0x41, 0x22, 0, 0, 0}}, // 67 - 'C'
		{5, {0x7f, 0x41, 0x41, 0x41, 0x3e, 0, 0, 0}}, // 68 - 'D'
		{5, {0x7f, 0x49, 0x49, 0x49, 0x41, 0, 0, 0}}, // 69 - 'E'
		{5, {0x7f, 0x09, 0x09, 0x09, 0x01, 0, 0, 0}}, // 70 - 'F'
		{5, {0x3e, 0x41, 0x41, 0x51, 0x73, 0, 0, 0}}, // 71 - 'G'
		{5, {0x7f, 0x08, 0x08, 0x08, 0x7f, 0, 0, 0}},// 72 - 'H'
		{3, {0x41, 0x7f, 0x41, 0, 0, 0, 0, 0}},// 73 - 'I'
		{5, {0x20, 0x40, 0x41, 0x3f, 0x01, 0, 0, 0}},// 74 - 'J'
		{5, {0x7f, 0x08, 0x14, 0x22, 0x41, 0, 0, 0}},// 75 - 'K'
		{5, {0x7f, 0x40, 0x40, 0x40, 0x40, 0, 0, 0}},// 76 - 'L'
		{5, {0x7f, 0x02, 0x1c, 0x02, 0x7f, 0, 0, 0}},// 77 - 'M'
		{5, {0x7f, 0x04, 0x08, 0x10, 0x7f, 0, 0, 0}},// 78 - 'N'
		{5, {0x3e, 0x41, 0x41, 0x41, 0x3e, 0, 0, 0}},// 79 - 'O'
		{5, {0x7f, 0x09, 0x09, 0x09, 0x06, 0, 0, 0}},// 80 - 'P'
		{5, {0x3e, 0x41, 0x51, 0x21, 0x5e, 0, 0, 0}},// 81 - 'Q'
		{5, {0x7f, 0x09, 0x19, 0x29, 0x46, 0, 0, 0}},// 82 - 'R'
		{5, {0x26, 0x49, 0x49, 0x49, 0x32, 0, 0, 0}},// 83 - 'S'
		{5, {0x03, 0x01, 0x7f, 0x01, 0x03, 0, 0, 0}},// 84 - 'T'
		{5, {0x3f, 0x40, 0x40, 0x40, 0x3f, 0, 0, 0}},// 85 - 'U'
		{5, {0x1f, 0x20, 0x40, 0x20, 0x1f, 0, 0, 0}},// 86 - 'V'
		{5, {0x3f, 0x40, 0x38, 0x40, 0x3f, 0, 0, 0}},// 87 - 'W'
		{5, {0x63, 0x14, 0x08, 0x14, 0x63, 0, 0, 0}},// 88 - 'X'
		{5, {0x03, 0x04, 0x78, 0x04, 0x03, 0, 0, 0}},// 89 - 'Y'
		{5, {0x61, 0x59, 0x49, 0x4d, 0x43, 0, 0, 0}},// 90 - 'Z'
		{3, {0x7f, 0x41, 0x41, 0, 0, 0, 0, 0}},  // 91 - '['
		{5, {0x02, 0x04, 0x08, 0x10, 0x20, 0, 0, 0}},  // 92 - '\'
		{3, {0x41, 0x41, 0x7f, 0, 0, 0, 0, 0}},  // 93 - ']'
		{5, {0x04, 0x02, 0x01, 0x02, 0x04, 0, 0, 0}},  // 94 - '^'
		{5, {0x40, 0x40, 0x40, 0x40, 0x40, 0, 0, 0}},  // 95 - '_'
		{3, {0x03, 0x07, 0x08, 0, 0, 0, 0, 0}},  // 96 - '`'
		{5, {0x20, 0x54, 0x54, 0x78, 0x40, 0, 0, 0}},  // 97 - 'a'
		{5, {0x7f, 0x28, 0x44, 0x44, 0x38, 0, 0, 0}},  // 98 - 'b'
		{5, {0x38, 0x44, 0x44, 0x44, 0x00, 0, 0, 0}},  // 99 - 'c'
		{5, {0x38, 0x44, 0x44, 0x28, 0x7f, 0, 0, 0}},  // 100 - 'd'
		{5, {0x38, 0x54, 0x54, 0x54, 0x18, 0, 0, 0}},  // 101 - 'e'
		{4, {0x08, 0x7e, 0x09, 0x02, 0, 0, 0, 0}},  // 102 - 'f'
		{5, {0x18, 0xa4, 0xa4, 0x9c, 0x78, 0, 0, 0}},  // 103 - 'g'
		{5, {0x7f, 0x08, 0x04, 0x04, 0x78, 0, 0, 0}},  // 104 - 'h'
		{3, {0x44, 0x7d, 0x40, 0, 0, 0, 0, 0}},  // 105 - 'i'
		{4, {0x40, 0x80, 0x80, 0x7a, 0, 0, 0, 0}},  // 106 - 'j'
		{4, {0x7f, 0x10, 0x28, 0x44, 0, 0, 0, 0}},  // 107 - 'k'
		{3, {0x41, 0x7f, 0x40, 0, 0, 0, 0, 0}},  // 108 - 'l'
		{5, {0x7c, 0x04, 0x78, 0x04, 0x78, 0, 0, 0}},  // 109 - 'm'
		{5, {0x7c, 0x08, 0x04, 0x04, 0x78, 0, 0, 0}},  // 110 - 'n'
		{5, {0x38, 0x44, 0x44, 0x44, 0x38, 0, 0, 0}},  // 111 - 'o'
		{5, {0xfc, 0x18, 0x24, 0x24, 0x18, 0, 0, 0}},  // 112 - 'p'
		{5, {0x18, 0x24, 0x24, 0x18, 0xfc, 0, 0, 0}},  // 113 - 'q'
		{5, {0x7c, 0x08, 0x04, 0x04, 0x08, 0, 0, 0}},  // 114 - 'r'
		{5, {0x48, 0x54, 0x54, 0x54, 0x24, 0, 0, 0}},  // 115 - 's'
		{4, {0x04, 0x3f, 0x44, 0x24, 0, 0, 0, 0}},  // 116 - 't'
		{5, {0x3c, 0x40, 0x40, 0x20, 0x7c, 0, 0, 0}},  // 117 - 'u'
		{5, {0x1c, 0x20, 0x40, 0x20, 0x1c, 0, 0, 0}},  // 118 - 'v'
		{5, {0x3c, 0x40, 0x30, 0x40, 0x3c, 0, 0, 0}},  // 119 - 'w'
		{5, {0x44, 0x28, 0x10, 0x28, 0x44, 0, 0, 0}},  // 120 - 'x'
		{5, {0x4c, 0x90, 0x90, 0x90, 0x7c, 0, 0, 0}},  // 121 - 'y'
		{5, {0x44, 0x64, 0x54, 0x4c, 0x44, 0, 0, 0}},  // 122 - 'z'
		{3, {0x08, 0x36, 0x41, 0, 0, 0, 0, 0}},  // 123 - '{'
		{1, {0x77, 0, 0, 0, 0, 0, 0, 0}},  // 124 - '|'
		{3, {0x41, 0x36, 0x08, 0, 0, 0, 0, 0}}, // 125 - '}'
		{5, {0x02, 0x01, 0x02, 0x04, 0x02, 0, 0, 0}},  // 126 - '~'
		{8, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}}  // Full fill
};

void delay();
void delay2();


LedMatrixMax7219::LedMatrixMax7219(int rows, int columns, int matrixSize):m_rows(rows), m_columns(columns), m_matrixSize(matrixSize)
{
	// TODO Auto-generated constructor stub

}

void LedMatrixMax7219::sendCommand(uint8_t address, uint8_t command)
{
	GPIOA->ODR &= ~GPIO_ODR_ODR_4;
	for (int i=0;i<m_columns; i++)
	{
		sendByte(address);
		sendByte(command);
	}
	GPIOA->ODR &= ~GPIO_ODR_ODR_4;
	GPIOA->ODR |= GPIO_ODR_ODR_4;
}

void LedMatrixMax7219::setled(uint8_t row, uint8_t col, uint8_t value)
{
	if (value)
	{
		buffer[col]= buffer[col] | (1UL << (row));
	}
	else
	{
		buffer[col] = buffer[col] & ~(1UL << (row));
	}

	int n = col >> 3;
	int c = col % 8;
	GPIOA->ODR &= ~GPIO_ODR_ODR_4;
	for (int i=0; i<m_columns; i++)
	{
		if (i == (m_columns-(n+1)))
		{
			sendByte (((c+1)));
			sendByte (buffer[col]);
		}
		else
		{
			sendByte (0);
			sendByte (0);
		}
	}
	GPIOA->ODR &= ~GPIO_ODR_ODR_4;
	GPIOA->ODR |= GPIO_ODR_ODR_4;
}

void LedMatrixMax7219::maxInit(uint8_t brightness)
{
	sendCommand(0x09, 0x00);       //  no decoding
	sendCommand(0x0b, 0x07);       //  scan limit = 8 LEDs
	sendCommand(0x0c, 0x01);       //  power down =0,normal mode = 1
	sendCommand(0x0f, 0x00);       //  no test display

	maxClear ();

	sendCommand(0x0a, brightness);       //  brightness intensity
	//writeChar(127, 1);
	//shiftLeft();
	//shiftChar(127, 1, 'L');
	//drawSmile('2', 0);
	//for (int i = 0; i < 10;++i)
	{
		writeChar('g', 1);

	}

}

void LedMatrixMax7219::drawSmile (char c, uint8_t matrixIndex)
{
	int width = characters[c].width;
	int start= (c<<3)+1;
	int startColumnIndex = 7; // aligned left
	for (int j = 0; j < 7; j++)
	{
		setColumn (startColumnIndex + ((matrixIndex)<<3), characters[c].columns[j]);
		--startColumnIndex;
	}
}
void LedMatrixMax7219::writeChar (char c, uint8_t matrixIndex)
{
	int width = characters[c].width;
	int startColumnIndex = 7; // aligned left
	for (int j = 0; j < width; j++)
	{
		setColumn (startColumnIndex + ((matrixIndex)<<3), characters[c].columns[j]);
		--startColumnIndex;
	}
}
void LedMatrixMax7219::setColumn(uint8_t columnIndexInDisplay, uint8_t value)
{
	int matrixIndex = columnIndexInDisplay >> 3;
	int columnIndexInMatrix = columnIndexInDisplay % 8;

	int matrixRightMostColumnIndex = matrixIndex<<3;

	uint8_t store = value;
	for (int i = 0; i < m_columns; ++i)
	{
		if (i != matrixIndex)
		{
			sendByte(0);
			sendByte(0);
		}
		else
		{
			//for (int col=0+(8*n); col<8+(8*n); col++)  // uncomment this if the character looks inverted about X axis
			for (int col = 7 + matrixRightMostColumnIndex; col >= matrixRightMostColumnIndex; --col)
			{
				bool b = value&0x80;
				setled (columnIndexInMatrix, col, b);
				value<<=1;
			}
		}
	}
	buffer_row[columnIndexInDisplay] = store;
}

void LedMatrixMax7219::maxClear()
{
	int lastIndexOfDisplayColumn = (m_columns << 3) - 1;
	for (int i = 0; i <= lastIndexOfDisplayColumn; i++)
	{
		setColumn(i, 0);
	}
	for (int i = 0; i < 80; ++i)
	{
		buffer[i] = 0;
		buffer_row[i] = 0;
	}
}

void LedMatrixMax7219::shiftLeft()
{
	int lastIndexOfDisplayColumn = (m_columns << 3) - 1;
	uint8_t old = buffer_row[lastIndexOfDisplayColumn];
	for (int i = 0; i <= lastIndexOfDisplayColumn; ++i)
	{
		setColumn(i, buffer_row[i]);
	}
	for (int i = 79; i > 0; --i)
	{
		buffer_row[i] = buffer_row[i-1];
	}

	buffer_row[0] = old;
}


void LedMatrixMax7219::shiftRight()
{
	int lastIndexOfDisplayColumn = (m_columns << 3) - 1;
	uint8_t old = buffer_row[0];

	for (int i = lastIndexOfDisplayColumn; i >= 0; --i)
	{
		setColumn (i, buffer_row[i]);
	}

	for (int i = 0; i < 80; ++i)
	{
		buffer_row[i] = buffer_row[i+1];
	}

	buffer_row[lastIndexOfDisplayColumn] = old;
}

void LedMatrixMax7219::shiftChar (char c, uint32_t speed, char direction)
{
	int width = characters[c].width;
	int lastIndexOfDisplayColumn = (m_columns << 3) - 1;

	switch (direction)
	{
	case ('L') :
		for (int j = 0; j <= width; j++)
		{
			setColumn(0, characters[c].columns[j]);
			shiftLeft();
			delay();
		}
	break;

	case ('R') :
	{
		for (int j = width; j >= 0; j--)
		{
			setColumn(lastIndexOfDisplayColumn, characters[c].columns[j]);
			shiftRight();
			delay();
		}
	}
	break;

	default :
		break;
	}
}



void LedMatrixMax7219::scrollString (uint8_t *string, uint32_t speed, char direction)
{
	while (*string != 0)
	{
		shiftChar (*string, speed, direction);
		string++;
	}
}
