/*
 * ledmatrixmax7219.h
 *
 *  Created on: 2 mar. 2023
 *      Author: VertexNi
 */

#ifndef LEDMATRIXMAX7219_H_
#define LEDMATRIXMAX7219_H_

#define	SPI_DATA_SIZE	(sizeof(uint8_t)*4*2)	///< Size of the SPI data buffers
#define	SPI_OFFSET(i,x)	(((LAST_BUFFER-(i))*2)+(x))	///< SPI data offset for buffer i, digit x
#define	FIRST_BUFFER	0						        ///< First buffer number
#define	LAST_BUFFER		(4-1)			///< Last buffer number

#include "stm32f1xx.h"
#include "spi.h"
#include "delay.h"
#include <string.h>

struct Character
{
	int width{};
	uint8_t rows[8]{};
};

template <Controller controllerModel, int MatrixSize, int CascadeCount> class LedMatrixMax7219 {
	Spi<controllerModel>* m_spi;
	uint16_t m_spiCsPin;
	uint8_t m_maxDigits;
	int m_rows;
	int m_columns;
	int m_matrixSize = MatrixSize;
	int m_cascadeSize = CascadeCount;
	uint8_t buffer[80]{};
	uint8_t buffer_row[80]{};
	Character buf[256]{};
	uint32_t actualBufferLength;
	static constexpr Character characters[141]
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
		{2, {0x60, 0x60, 0, 0, 0, 0, 0, 0}},  // 46 - '.'
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
		{5, {0b00000011, 0b00000001, 0b01111111, 0b00000001, 0b00000011, 0, 0, 0}},// 84 - 'T'
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
		{8, {0xff, 0b10000001, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},  // Full fill
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 128 - 'Cyrillic upper case A'
		{5, {0x7f, 0x49, 0x49, 0x49, 0x36, 0, 0, 0}},  // 129
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 130
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 131
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 132
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 133
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 134
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 135
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 136
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 137
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 138
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}},  // 139
		{5, {0x7c, 0x12, 0x11, 0x12, 0x7c, 0, 0, 0}}  // 140
										  };

	void sendData(uint8_t rg, uint8_t dt)
	{
		m_spi->sendData(rg, dt);
	}

	void sendData(uint8_t rg, uint8_t* dt, int dataLength)
	{
		m_spi->sendData(rg, dt, dataLength);
	}

	void sendByte(uint8_t dt)
	{
		m_spi->sendByte(dt);
	}

	void setLedSate(int row, int column, bool on)
	{
		int transformedRow = MatrixSize - row;
		int cascadeIndex = column / m_matrixSize;
		int columnInCascade = column - cascadeIndex*m_matrixSize;
		int transformedColumn = MatrixSize - columnInCascade;
		uint8_t sequenceOfRows[CascadeCount]{};
		for(int i = 0; i <CascadeCount;++i)
		{
			sequenceOfRows[i] = buf[i].rows[transformedRow];
		}
		if (on)
		{
			sequenceOfRows[cascadeIndex] |= 1 << (transformedColumn - 1);
			buf[cascadeIndex].rows[transformedRow] = sequenceOfRows[cascadeIndex];
		}
		else
		{
			sequenceOfRows[cascadeIndex] &= ~(1 << (transformedColumn - 1));
			buf[cascadeIndex].rows[transformedRow] = sequenceOfRows[cascadeIndex];
		}
		sendData(transformedRow, sequenceOfRows, CascadeCount);
	}

	void init()
	{
		for (int i = 0; i < m_cascadeSize; ++i)
		{
			sendData(OperationCode::OP_DISPLAYTEST, 0x00);
		}
		for (int i = 0; i < m_cascadeSize; ++i)
			sendData(OperationCode::OP_DECODEMODE, 0x00);  //  no decoding
		for (int i = 0; i < m_cascadeSize; ++i)
			sendData(OperationCode::OP_SCANLIMIT, 0x0f);   //  scan limit = 8 LEDs
		for (int i = 0; i < m_cascadeSize; ++i)
			sendData(OperationCode::OP_INTENSITY, 0);       //  brightness intensity
		for (int i = 0; i < m_cascadeSize; ++i)
			sendData(OperationCode::OP_SHUTDOWN, 0x01);    //  power down = 0, normal mode = 1
		for (int i = 0; i < m_cascadeSize; ++i)
			sendData(OperationCode::OP_INTENSITY, 7);       //  brightness intensity
		clearDisplay();
	}
public:

	enum OperationCode: uint8_t
	{
		OP_DECODEMODE = 9,	///< MAX72xx opcode for DECODE MODE
				OP_INTENSITY = 10,	///< MAX72xx opcode for SET INTENSITY
				OP_SCANLIMIT = 11,	///< MAX72xx opcode for SCAN LIMIT
				OP_SHUTDOWN = 12,	///< MAX72xx opcode for SHUT DOWN
				OP_DISPLAYTEST = 15	///< MAX72xx opcode for DISPLAY TEST
	};

	LedMatrixMax7219(Spi<controllerModel>* spi, int rows, int columns, int matrixSize):
		m_spi(spi), m_rows(rows), m_columns(columns), m_matrixSize(matrixSize)
	{
		init();
	}

	Character rotate(const Character& ch)
	{
		Character temp;
		temp.width = ch.width;
		for(int i = 0; i < MatrixSize; ++i)
		{
			for(int j = 0; j < MatrixSize; ++j)
			{
				temp.rows[i] |= ((ch.rows[j] >> i & 1) << (MatrixSize - 1 -j));
			}
		}
		return temp;
	}

	void displayString(const char* s)
	{
		memset(buf, 0, 256*sizeof(Character));
		actualBufferLength = strlen(s);
		for(int i = 0; s[i];++i)
		{
			buf[i] = rotate(characters[static_cast<int>(s[i])]);
		}
		update();
	}

	void shiftString(const char* s, int frameDelay, int initialDelayMs = 1000)
	{
		displayString(s);
		delayMs(initialDelayMs);
		for (auto i = 0U; i < (strlen(s))*MatrixSize;++i)
		{
			shiftLeft();
			delayMs(frameDelay);
		}
	}
	void displayString(uint8_t s[], int dalaLength)
	{
		actualBufferLength = dalaLength;
		for(int i = 0; i < dalaLength;++i)
		{
			buf[i] = characters[s[i]];
		}
		update();
	}

	void update()
	{
		for(int j = 0; j < m_matrixSize; ++j)
		{
			uint8_t bufrow[CascadeCount];
			for(int i = 0; i <CascadeCount;++i)
			{
				bufrow[i] = buf[i].rows[j];
			}
			sendData(j + 1, bufrow, CascadeCount);
		}
	}


	void shiftLeft()
	{
		int rowsInMatrix = m_matrixSize;
		for (uint32_t i = 0; i < actualBufferLength; ++i)
		{
			for (int j = 0; j < rowsInMatrix;++j)
			{
				uint8_t& currentMatrixRow = buf[i].rows[j];
				currentMatrixRow <<= 1;

				auto carryFromNextCascade = ( (i < actualBufferLength - 1) && (buf[i+1].rows[j] & (1<<(m_matrixSize-1))));
				if (carryFromNextCascade)
				{
					currentMatrixRow |= 1;
				}
			}
		}

		update();
	}
	void setColumn(int columnIndex, char columnBits)
	{
		while(columnIndex >= 0)
		{
			for (int i = 0; i < m_matrixSize; i++)
			{
				sendData(i+1, (columnBits >> i)&1);
			}
			--columnIndex;
		}

	}





	void setLed(int row, int column)
	{
		setLedSate(row, column, true);
	}

	void resetLed(int row, int column)
	{
		setLedSate(row, column, false);
	}



	void setColumn(uint8_t columnIndexInDisplay, uint8_t value)
	{

		int matrixIndex = columnIndexInDisplay >> 3;
		int columnIndexInMatrix = columnIndexInDisplay % 8;

		int matrixRightMostColumnIndex = matrixIndex<<3;

		uint8_t store = value;
		for (int i = 0; i < m_columns; ++i)
		{
			if (i != matrixIndex)
			{
				sendData(0, 0);
			}
			else
			{
				//for (int col=0+(8*n); col<8+(8*n); col++)  // uncomment this if the character looks inverted about X axis
				for (int col = 7 + matrixRightMostColumnIndex; col >= matrixRightMostColumnIndex; --col)
				{
					bool b = value&0x80;
					//setled(columnIndexInMatrix, col, b);
					value<<=1;
				}
			}
		}
		buffer_row[columnIndexInDisplay] = store;
	}

	void clearDisplay()
	{
		for(int j = 0; j < m_matrixSize; ++j)
		{
			uint8_t bufrow[CascadeCount];
			for(int i = 0; i <CascadeCount;++i)
			{
				bufrow[i] = 0;
			}
			sendData(j + 1, bufrow, CascadeCount);

		}
	}


	//	void setled(uint8_t row, uint8_t col, uint8_t value)
	//	{
	//		if (value)
	//		{
	//			buffer[col]= buffer[col] | (1UL << (row));
	//		}
	//		else
	//		{
	//			buffer[col] = buffer[col] & ~(1UL << (row));
	//		}
	//
	//		int n = col >> 3;
	//		int c = col % 8;
	//		for (int i=0; i<m_columns; i++)
	//		{
	//			if (i == (m_columns-(n+1)))
	//			{
	//				sendData((c+1), buffer[col]);
	//			}
	//			else
	//			{
	//				sendData(0, 0);
	//			}
	//		}
	//	}
};


#endif /* LEDMATRIXMAX7219_H_ */
