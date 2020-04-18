#define F_CPU 8000000UL

#include <avr/io.h>
#include "twi.h"

void I2cTwix::init (void)
{
    TWBR=0x20; //TWI Bit Rate Register (for 8 MHZ if CPU is 100 KHZ for ds1307)
}

void I2cTwix::waitTwi()
{
    while (!(TWCR & (1<<TWINT)));// wait until TWIN is set
}

void I2cTwix::startCondition(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    waitTwi();
}

void I2cTwix::stopCondition(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2cTwix::sendByte(unsigned char c)
{
    TWDR = c;//запишем байт в регистр данных
    TWCR = (1<<TWINT)|(1<<TWEN);//включим передачу байта
    waitTwi();
}

void I2cTwix::sendByteByADDR(unsigned char dataByte,unsigned char addr)
{
    startCondition(); // Отправим условие START
    sendByte(addr); // Отправим в шину адрес устройства + бит чтения-записи
    sendByte(dataByte);// Отправим байт данных
    stopCondition();// Отправим условие STOP
}

unsigned char I2cTwix::readByte(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    waitTwi();
    return TWDR;//читаем регистр данных
}

unsigned char I2cTwix::readLastByte(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    waitTwi();
    return TWDR;//читаем регистр данных
}