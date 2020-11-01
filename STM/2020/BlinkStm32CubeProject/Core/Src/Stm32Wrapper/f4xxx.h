/*
 * f4xxx.h
 *
 *  Created on: 31 окт. 2020 г.
 *      Author: rusla
 */

#ifndef SRC_STM32WRAPPER_F4XXX_H_
#define SRC_STM32WRAPPER_F4XXX_H_

#include <inttypes.h>

class F4xxx {
	void enableHse();
	uint32_t m_gpioModerPositions[16]{0U, 1U, 2U, 3U, 4U ,5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U};
public:

	enum class PortMode
	{
		Input = 0,
		Output,
		Alternative,
		Analog
	};

	F4xxx();
	void enableAHB1PortA();
	void setPinMode(PortMode mode, unsigned int pin);
	void setPin5High();
	int clockInit(void);
};

#endif /* SRC_STM32WRAPPER_F4XXX_H_ */
