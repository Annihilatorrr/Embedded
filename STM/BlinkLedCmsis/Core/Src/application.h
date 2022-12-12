/*
 * application.h
 *
 *  Created on: Dec 9, 2022
 *      Author: VertexNi
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "stm32f1xx.h"

class Application {
	int clockInit(void);
	Application();

	void initAltFunctionsClock();
	void initPortCClock();
	void initPortBClock();
	void initPortAClock();
	void initDmaClock();
	void initSwdOnlyDebugging();
	void initOuputPushPull2Mhz();

public:
	void spi1_init(void);
	static void spiDma_Send(uint8_t *data, uint16_t len);
    static Application& getInstance()
    {
        static Application _instance;
    	return _instance;
    }
	void initPeriferal();
	virtual ~Application();

	void setPin(GPIO_TypeDef* port, uint8_t pin);
	void resetPin(GPIO_TypeDef* port, uint8_t pin);

};

#endif /* APPLICATION_H_ */
