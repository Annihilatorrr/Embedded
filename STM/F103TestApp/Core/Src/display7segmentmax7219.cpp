/*
 * display7segmentmax7219.cpp
 *
 *  Created on: Nov 29, 2022
 *      Author: VertexNi
 */

#include <Core/Src/display7segmentmax7219.h>

Display7segmentMax7219::Display7segmentMax7219(SPI_HandleTypeDef spi):m_spi(spi)
{
	// TODO Auto-generated constructor stub

}

Display7segmentMax7219::~Display7segmentMax7219() {
	// TODO Auto-generated destructor stub
}

void Display7segmentMax7219::init(uint8_t intensivity){

}
void Display7segmentMax7219::clean(void){
	uint8_t clear = 0x00;

	if(decodeMode == 0xFF)
	{
		clear = (uint8_t)Display7segmentMax7219::Letters::BLANK;
	}

	for (int i = 0; i < 8; ++i)
	{
		SendData(i + 1, clear);
	}
}

void Display7segmentMax7219::SendData(uint8_t reg, uint8_t value)
{
	uint8_t tx_data[2] = { reg, value };
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&m_spi, tx_data, 2, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	//	HAL_Delay(1);
}
