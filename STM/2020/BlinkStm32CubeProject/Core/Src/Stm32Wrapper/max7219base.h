/*
 * max7219base.h
 *
 *  Created on: 30 дек. 2020 г.
 *      Author: rusla
 */

#ifndef SRC_STM32WRAPPER_MAX7219BASE_H_
#define SRC_STM32WRAPPER_MAX7219BASE_H_

#include "spi.h"

class Max7219Base {

protected:
	SPI m_spi;

public:
	Max7219Base();
	void sendByte(uint8_t data);
    void sendCommand(uint8_t address, uint8_t command);

};

#endif /* SRC_STM32WRAPPER_MAX7219BASE_H_ */
