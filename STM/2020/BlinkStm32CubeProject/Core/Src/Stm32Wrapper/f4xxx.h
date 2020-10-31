/*
 * f4xxx.h
 *
 *  Created on: 31 окт. 2020 г.
 *      Author: rusla
 */

#ifndef SRC_STM32WRAPPER_F4XXX_H_
#define SRC_STM32WRAPPER_F4XXX_H_

class F4xxx {
	void enableHse();
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
	void setPin5Mode(PortMode mode);
	void setPin5High();
	int clockInit(void);
};

#endif /* SRC_STM32WRAPPER_F4XXX_H_ */
