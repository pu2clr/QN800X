#include <MyWire.h>

/**
 * @defgroup TowWire BEKEN I2C BUS
 * @section TowWire I2C
 *
 * @brief Custom 2-Wire (I2C) implementation
 * @details This I2C (2-Wire) library was developed to address cases where the standard Arduino library (Wire) does not work with certain devices. 
 * @details One example of such a case is the BK1080E DSP device.
 */

/**
 * @ingroup TowWire
 * @brief Sets address and MCU pins connected to the I2C bus
 * @details Configures the I2C bus for MyWire
 *
 * @param deviceAddress Device Address 
 * @param pin_sda SDA/SDIO MCU/Arduino pin
 * @param pin_clk CLK/SCLK MCU/Arduino pin
 */
void MyWire::init(uint8_t deviceAddress, uint8_t pin_sda, uint8_t pin_clk)
{
    this->deviceAddress = deviceAddress;
    this->pin_sda = pin_sda;
    this->pin_clk = pin_clk;
}

/**
 * @ingroup TowWire
 * @brief Another way to start MyWire 
 * @details Configures the I2C bus for MyWire
 */
void MyWire::begin()
{
    this->pin_sda = 4;
    this->pin_clk = 5;
}

/**
 * @ingroup TowWire
 * @brief Starts the I2C bus transaction
 */
void MyWire::beginTransmission(uint8_t deviceAddress)
{
    this->deviceAddress = deviceAddress;
    pinMode(this->pin_sda, OUTPUT);
    pinMode(this->pin_clk, OUTPUT);
    digitalWrite(this->pin_sda, HIGH);
    digitalWrite(this->pin_clk, HIGH);
    delayMicroseconds(1);

    digitalWrite(this->pin_sda, LOW);
    delayMicroseconds(1);
    digitalWrite(this->pin_clk, LOW);
    delayMicroseconds(1);
    digitalWrite(this->pin_sda, HIGH);
}

/**
 * @ingroup TowWire
 * @brief Finish the I2C bus  transaction
 */
void MyWire::endTransaction()
{
    pinMode(pin_sda, OUTPUT);
    digitalWrite(this->pin_sda, LOW);
    delayMicroseconds(1);

    digitalWrite(this->pin_clk, HIGH);
    delayMicroseconds(1);

    digitalWrite(this->pin_sda, HIGH);
    delayMicroseconds(1);
}

/**
 * @ingroup TowWire
 * @brief Sends Acknowledge (ACK)
 * @details Each byte of data (including the address byte) have to be followed by one ACK bit from the receiver.
 * @details The ACK bit allows the receiver to communicate to the transmitter.
 * @see https://www.ti.com/lit/an/slva704/slva704.pdf
 */
void MyWire::ack()
{
    pinMode(pin_sda, OUTPUT);
    digitalWrite(this->pin_clk, LOW);
    digitalWrite(this->pin_sda, LOW);
    delayMicroseconds(1);
    digitalWrite(this->pin_clk, HIGH);
    delayMicroseconds(1);
    digitalWrite(this->pin_clk, LOW);
}

/**
 * @ingroup TowWire
 * @brief Sends Not Acknowledge (ACK)
 * @see https://www.ti.com/lit/an/slva704/slva704.pdf
 */
void MyWire::nack()
{
    pinMode(pin_sda, OUTPUT);

    digitalWrite(this->pin_clk, LOW);
    digitalWrite(this->pin_sda, HIGH);
    delayMicroseconds(1);
    digitalWrite(this->pin_clk, HIGH);
    delayMicroseconds(1);
    digitalWrite(this->pin_clk, LOW);
}

/**
 * @ingroup TowWire
 * @brief Gets Acknowledge (ACK)
 * @see https://www.ti.com/lit/an/slva704/slva704.pdf
 * @return ack value
 */
uint8_t MyWire::receiveAck()
{
    uint8_t ack;
    pinMode(pin_sda, INPUT);
    delayMicroseconds(1);

    digitalWrite(this->pin_clk, HIGH);
    delayMicroseconds(1);

    ack = digitalRead(this->pin_sda);

    digitalWrite(this->pin_clk, LOW);
    delayMicroseconds(1);

    return ack;
}

/**
 * @ingroup TowWire
 * @brief Sends a Byte to the slave device
 * @param data to be sent to the slave device
 */
void MyWire::writeByte(uint8_t data)
{
    pinMode(pin_sda, OUTPUT);
    delayMicroseconds(1);

    for (int i = 0; i < 8; i++)
    {

        digitalWrite(this->pin_sda, (bool)(data & this->deviceAddress));

        delayMicroseconds(1);
        digitalWrite(this->pin_clk, HIGH);
        delayMicroseconds(1);
        digitalWrite(this->pin_clk, LOW);
        data = data << 1;
    }
}

/**
 * @ingroup TowWire
 * @brief Gets a Byte from the slave device
 * @return value read from the device
 */
uint8_t MyWire::readByte()
{
    uint8_t value = 0;

    pinMode(pin_sda, INPUT);
    delayMicroseconds(1);

    for (int i = 0; i < 8; i++)
    {
        digitalWrite(this->pin_clk, HIGH);
        value = value << 1;
        delayMicroseconds(1);
        if (digitalRead(this->pin_sda))
            value = value | 1;
        digitalWrite(this->pin_clk, LOW);
        delayMicroseconds(1);
    }

    return value;
}

/**
 * @ingroup TowWire
 * @brief Sends an array of values to a MyWire given register
 * @param reg register to be written
 * @param value content to be stored into the register
 */
void MyWire::writeRegister(uint8_t reg, uint16_t value)
{

    word16_to_bytes data;
    data.raw = value;

    this->beginTransmission();
    this->writeByte(this->deviceAddress);
    this->receiveAck();

    reg = reg << 1; // Converts address and sets to write operation

    this->writeByte(reg);
    this->receiveAck();

    this->writeByte(data.refined.highByte);
    this->receiveAck();
    this->writeByte(data.refined.lowByte);
    this->receiveAck();

    this->endTransaction();
}

/**
 * @ingroup TowWire
 * @brief Gets an array of values from a MyWire given register
 * @param reg register to be read
 * @return register content
 */
uint16_t MyWire::readRegister(uint8_t reg)
{

    word16_to_bytes data;

    this->beginTransmission();
    this->writeByte(this->deviceAddress);
    this->receiveAck();

    reg = (reg << 1) | 1; // Converts address and sets to read operation

    this->writeByte(reg);
    this->receiveAck();

    data.refined.highByte = this->readByte();
    this->ack();
    data.refined.lowByte = this->readByte();
    this->nack();

    this->endTransaction();

    return data.raw;
}

void MyWire::requestFrom(uint8_t deviceAddress, uint8_t count) {
    this->deviceAddress = deviceAddress;
    this->bytesToRead = count;
}


