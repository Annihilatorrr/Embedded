#ifndef TWI_H_
#define TWI_H_

class I2cTwix
{
    public:
    
    I2cTwix(){}
    void init (void); //инициализация i2c
    void startCondition(void); //Отправим условие START
    void stopCondition(void); //Отправим условие STOP
    void sendByte(unsigned char c); //передача байта в шину
    void sendByteByADDR(unsigned char c,unsigned char addr); //передача байта в шину на устройство по адресу
    unsigned char readByte(void); //читаем байт
    unsigned char readLastByte(void); //читаем последний байт

};
#endif /* TWI_H_ */