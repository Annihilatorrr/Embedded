#ifndef TWI_H_
#define TWI_H_

class I2cTwix
{
    void startCondition(void); // send start
    void stopCondition(void); // send stop
    void sendByte(unsigned char c); // send byte to bus
    void waitTwi();
    public:
    
    I2cTwix(){}
    void init (void); // i2c initialization
    
    
    void sendByteByADDR(unsigned char c,unsigned char addr); // send byte to bus by device address
    unsigned char readByte(void); 
    unsigned char readLastByte(void);

};
#endif /* TWI_H_ */