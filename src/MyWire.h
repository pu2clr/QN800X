#ifndef _MYWIRE_H // Prevent this file from being compiled more than once
#define _MYWIRE_H

#include <Arduino.h>


/**
 * @ingroup GA01
 * @brief Converts 16 bits word to two bytes 
 */
typedef union
{
    struct
    {
        uint8_t lowByte;
        uint8_t highByte;
    } refined;
    uint16_t raw;
} word16_to_bytes;


class MyWire {

private:    
    uint8_t pin_sda, pin_clk; 
    uint8_t deviceAddress;
    uint8_t bytesToRead = 1;

public: 
    void begin(); 
    void setAddress(uint8_t deviceAddress);
    void init(uint8_t deviceAddress, uint8_t pin_sda = 4, uint8_t pin_clk = 5);
    void beginTransmission(uint8_t deviceAddress);
    void endTransaction();
    void ack();
    void nack();
    uint8_t receiveAck();
    void writeByte(uint8_t data);
    uint8_t readByte();
    void writeRegister(uint8_t reg, uint16_t value);
    uint16_t readRegister(uint8_t reg);
    void requestFrom(uint8_t deviceAddress, uint8_t count = 1);
};

#endif // MYWIRE
