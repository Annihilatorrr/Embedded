#define F_CPU 8000000UL

#include <avr/io.h>
#include "twi.h"

void I2cTwix::init (void)
{
    TWBR=0x20;//скорость передачи (при 8 мгц получается 100 кгц, что и необходимо для общения с ds1307)
}

void I2cTwix::startCondition(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));//подождем пока установится TWIN
}

void I2cTwix::stopCondition(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2cTwix::sendByte(unsigned char c)
{
    TWDR = c;//запишем байт в регистр данных
    TWCR = (1<<TWINT)|(1<<TWEN);//включим передачу байта
    while (!(TWCR & (1<<TWINT)));//подождем пока установится TWIN
}

void I2cTwix::sendByteByADDR(unsigned char c,unsigned char addr)
{
    startCondition(); // Отправим условие START
    sendByte(addr); // Отправим в шину адрес устройства + бит чтения-записи
    sendByte(c);// Отправим байт данных
    stopCondition();// Отправим условие STOP
}

unsigned char I2cTwix::readByte(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));//ожидание установки бита TWIN
    return TWDR;//читаем регистр данных
}

unsigned char I2cTwix::readLastByte(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));//ожидание установки бита TWIN
    return TWDR;//читаем регистр данных
}