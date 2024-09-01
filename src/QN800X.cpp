/**
 * @mainpage QN8006 and QN8007 Arduino Library implementation
  *
 * @details This is an Arduino library for the QN8006/07 FM RX/TX devices (Digital FM Transceiver for Portable Devices).
 * @details The communication used by this library is I2C.
 * @details You can see a complete documentation on <https://github.com/pu2clr/QN800X> or <https://pu2clr.github.io/QN800X/>
 * @details
 * @details This library can be freely distributed. This means that you can copy, modify, merge, and redistribute the code for free under the terms of the [MIT License](https://github.com/pu2clr/QN800X/blob/main/LICENSE).
 * @details There are examples that can help you in your project on <https://github.com/pu2clr/QN800X/tree/master/examples>
 * @see [General Documentation](https://pu2clr.github.io/QN800X/)
 * @author PU2CLR - Ricardo Lima Caratti -  pu2clr@gmail.com
 * @date  2024-06
 * @copyright MIT Free Software model. See [Copyright (c) 2024 Ricardo Lima Caratti](https://github.com/pu2clr/QN800X/blob/main/LICENSE).
 */

#include <QN800X.h>

/** @defgroup group01 Device Checking*/

/**
 * @ingroup group01 Detect Device
 * @brief   Checks communication with QN800X via I2C
 * @details Checks if the QN800X is available on the I2C bus. Remember that the QN800X responds to the address 0x21
 * @return  true or false
 */
bool QN800X::detectDevice() {

  Wire.begin();
  // check 0x21 I2C address
  Wire.beginTransmission(qn800x_I2C_ADDRESS);
  return !Wire.endTransmission();
}

/**
 * @ingroup group01 Scan I2C Devices
 * @brief  Scans the I2C bus and returns the addresses of the devices found.
 * @details Searches for devices connected to the I2C bus. The addresses of the devices found are stored in the "device" array.
 * @param device array of device addresses found.
 * @return uint8_t number of devices found or 0 if no device found or error.
 */
uint8_t QN800X::scanI2CBus(uint8_t *device) {

  uint8_t error, address;
  uint8_t idxDevice = 0;

  Wire.begin();

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      device[idxDevice] = address;
      idxDevice++;
    } else if (error == 4) {
      return 0;
    }
  }
  return idxDevice;
}

/** @defgroup group02 Basic Functions*/

/**
 * @ingroup group02 I2C
 * @brief Gets register information
 * @details 8066 commands. It provides a means to run commands that are not currently supported by the standard API.
 * @param registerNumber
 * @return uint8_t Value of the register
 */
uint8_t QN800X::getRegister(uint8_t registerNumber) {

  Wire.beginTransmission(qn800x_I2C_ADDRESS);
  Wire.write(registerNumber);
  Wire.endTransmission();
  delayMicroseconds(qn800x_DELAY_COMMAND);

  Wire.requestFrom(qn800x_I2C_ADDRESS, 1);
  return Wire.read();
}

/**
 * @ingroup group02 I2C
 * @brief Stores a velue to a given register
 * @details 8066 commands. It provides a means to run commands that are not currently supported by the standard API.
 * @param registerNumber
 * @param value
 */
void QN800X::setRegister(uint8_t registerNumber, uint8_t value) {

  Wire.beginTransmission(qn800x_I2C_ADDRESS);
  Wire.write(registerNumber);
  Wire.write(value);
  Wire.endTransmission();
  delayMicroseconds(qn800x_DELAY_COMMAND);
}



/** @defgroup group99 Helper and Tools functions*/

/**
 * @ingroup group99 Covert numbers to char array
 * @brief Converts a number to a char array
 * @details It is useful to mitigate memory space used by functions like sprintf or other generic similar functions
 * @details You can use it to format frequency using decimal or thousand separator and also to convert small numbers.
 *
 * @param value  value to be converted
 * @param strValue char array that will be receive the converted value
 * @param len final string size (in bytes)
 * @param dot the decimal or thousand separator position
 * @param separator symbol "." or ","
 * @param remove_leading_zeros if true removes up to two leading zeros (default is true)
 * @code
 * #include <QN800X.h>
 * QN800X tx;
 * char strFrequency[7];
 * uint16_t txFrequency = 1069;
 * void setup() {
 *   char strFrequency[7];
 *   tx.setup();
 *   tx.setTX(1069); // Set the transmitter to 106.9 MHz
 *   tx.convertToChar(txFrequency, strFrequency, 5, 3, ','); // Convert 1069 to a array of char "106.9"
 * }
 *
 * void loop() {
 * }
 * @endcode
 *
 */
void QN800X::convertToChar(uint16_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator, bool remove_leading_zeros)
{
    char d;
    for (int i = (len - 1); i >= 0; i--)
    {
        d = value % 10;
        value = value / 10;
        strValue[i] = d + 48;
    }
    strValue[len] = '\0';
    if (dot > 0)
    {
        for (int i = len; i >= dot; i--)
        {
            strValue[i + 1] = strValue[i];
        }
        strValue[dot] = separator;
    }

    if (remove_leading_zeros)
    {
        if (strValue[0] == '0')
        {
            strValue[0] = ' ';
            if (strValue[1] == '0')
                strValue[1] = ' ';
        }
    }
}

/**
 * @ingroup group99 Covert numbers to char array
 * @brief Convert the current frequency to a formated string (char *) frequency
 * @details The current frequency is the latest setted frequency by setFrequency, seek, setFrequencyUp and setFrequencyDown.
 * @param char decimalSeparator - the symbol that separates the decimal part (Exe: . or ,)
 * @return point char string strFrequency (member variable)
 * @see setFrequency, seek, setFrequencyUp and setFrequencyDown
*/
char* QN800X::formatCurrentFrequency(char decimalSeparator)
{
   this->convertToChar(this->currentFrequency, this->strCurrentFrequency, 4, 3, decimalSeparator, true);
   return this->strCurrentFrequency;
}
