/**
 * @brief QN800X (QN8006 and QN8007) ARDUINO LIBRARY
 *
 * @details This is an Arduino library for the QN800X FM RX/TX device (Digital FM Transceiver for Portable Devices).
 * @details The communication used by this library is I2C.
 * @details This file contains: const (#define), Defined Data type and Methods declarations 
 * @details You can see a complete documentation on  <https://github.com/pu2clr/QN800X>
 * @details There are examples that can help you in your project on  <https://github.com/pu2clr/QN800X/tree/master/examples>
 * @see [General Documentation](https://pu2clr.github.io/QN800X/)
 *
 * @author PU2CLR - Ricardo Lima Caratti
 * @date  2024
 */

#ifndef _qn800x_H // Prevent this file from being compiled more than once
#define _qn800x_H

#include <Arduino.h>
#include <Wire.h>

#define qn800x_I2C_ADDRESS 0x21   // See Datasheet pag. 16.
#define qn800x_RESET_DELAY 1000   // Delay after reset in us
#define qn800x_DELAY_COMMAND 2500 // Delay after command

/**
 * @brief QN800X Register addresses
 *
 */

#define QN_SYSTEM1    0x00  //!< Sets device modes.
#define QN_SYSTEM2    0x01  //!< Sets device modes, resets.
#define QN_DEV_ADD    0x02  //!< Sets device address.
#define QN_ANACTL1    0x03  //!< Analog control functions.
#define QN_REG_VGA    0x04  //!< TX mode input impedance, crystal cap load setting.
#define QN_CIDR1      0x05  //!< Device ID numbers.
#define QN_CIDR2      0x06  //!< Device ID numbers.
#define QN_I2S        0x07  //!< Sets I2S parameters. 
#define QN_CH         0x08  //!< Lower 8 bits of 10-bit channel index.
#define QN_CH_START   0x09  //!< Lower 8 bits of 10-bit channel scan start channel index.
#define QN_CH_STOP    0x0A  //!< Lower 8 bits of 10-bit channel scan stop channel index.
#define QN_CH_STEP    0x0B  //!< Channel scan frequency step. Highest 2 bits of channel indexes.
#define QN_PAC_TARGET 0x0C  //!< Output power calibration control.
#define QN_TXAGC_GAIN 0x0D  //!< Sets TX parameters.
#define QN_TX_FDEV    0x0E  //!< Specify total TX frequency deviation.
#define QN_GAIN_TXPLT 0x0F  //!< Gain of TX pilot frequency deviation, I2S buffer clear.
#define QN_RDSD0      0x10  //!< RDS data byte 0.
#define QN_RDSD1      0x11  //!< RDS data byte 1.
#define QN_RDSD2      0x12  //!< RDS data byte 2.
#define QN_RDSD3      0x13  //!< RDS data byte 3.
#define QN_RDSD4      0x14  //!< RDS data byte 4.
#define QN_RDSD5      0x15  //!< RDS data byte 5.
#define QN_RDSD6      0x16  //!< RDS data byte 6.
#define QN_RDSD7      0x17  //!< RDS data byte 7.
#define QN_RDSFDEV    0x18  //!< Specify RDS frequency deviation, RDS mode selection.
#define QN_CCA        0x19  //!< Sets CCA parameters.    
#define QN_STATUS1    0x1A  //!< Device status indicators.
#define QN_STATUS3    0x1B  //!< RDS status indicators.
#define QN_RSSISIG    0x1C  //!< In-band signal RSSI dBμV value.
#define QN_RSSIMP     0x21  //!< Multipath signal RSSI (Received signal strength indicator) DB value.
#define QN_SNR        0x22  //!< Estimated RF input CNR value from noise floor around the pilot after FM demodulation.
#define QN_REG_XLT3   0x49  //!< XCLK pin control.
#define QN_REG_DAC    0x4F  //!< DAC output stage gain.
#define QN_PAC_CAL    0x59  //!< PA tuning cap calibration.    
#define QN_PAG_CAL    0x5A  //!< PA gain calibration.


/** @defgroup group00 Union, Struct and Defined Data Types
 * @section group01 Data Types
 *
 * @brief QN800X data representation
 *
 * @details
 *
 */

/**
 * @ingroup group00
 *
 * @brief System1 - Sets device modes (Address: 00h)
 */

typedef union {
  struct {
    uint8_t CCA_CH_DIS:1;   //!< CH (channel index) selection method. 0=CH is determined by internal CCA; 1=CH is determined by the content in CH
    uint8_t RDSEN:1;        //!< RDS enable: 0=Disable; 1=Enable 
    uint8_t TXI2S:1;        //!< I2S enable in transmitting mode: 0=analog input for TX audio; 1=I2S digital signal for TX audio
    uint8_t RXI2S:1;        //!< I2S enable in receiving mode: 0=analog output for RX audio; 1=I2S digital interface for RX audio
    uint8_t STNBY:1;        //!< Request immediately to enter Standby mode if the chip is in IDLE and no TXREQ or RXREQ is received. 
    uint8_t CHSC:1;         //!< Channel Scan mode enable; 0=Normal operation.;1=Channel Scan mode operation.
    uint8_t TXREQ:1;        //!< Transmission request: 0=Non TX mode; 1=Enter Transmit mode.
    uint8_t RXREQ:1;        //!< Receiving request (overwrites TXREQ and STNBY): 0=Non RX mode; 1=Enter Receiving mode.
  } arg;
  uint8_t raw;
} qn800x_system1;

/**
 * @ingroup group00
 *
 * @brief System2 - Sets device modes (Address: 01h)
 *
 */

typedef union {
  struct {
    uint8_t TMOUT:2;      //!< Time out setting for IDLE to standby state transition: 0 = 1 min; 1 = 3 min; 2 = 5 min; 3 = never 
    uint8_t RDSTXRDY:1;   //!< Toggle this bit to transmit all 8 bytes in RDS0~RDS7. The chip will internally fetch these bytes after completing transmit of the current group.
    uint8_t TC:1;         //!< 0 = 50us; 1=75us  
    uint8_t ST_MO_TX:1;   //!< TX stereo and mono mode selection: 0=Stereo; 1=Mono
    uint8_t FORCE_MO:1;   //!< Force receiver in MONO mode: 0=Not forced. ST/MONO auto selected; 1=Forced in MONO mode.
    uint8_t RECAL:1;      //!< Reset the state to initial states and recalibrate all blocks: 0=No reset. FSM runs normally; 1=Reset the FSM. After this bit is de-asserted, FSM will go through all the power up and calibration sequence. 
    uint8_t SWRST:1;      //!< Reset all registers to default values: 0=Keep the current value; 1=Reset to default values.
  } arg;
  uint8_t raw;
} qn800x_system2;




/**
 * @ingroup group00
 * @brief   Programmed device address
 * @details Programmed device address when SEB=1: If SEB=0, the default device address (0101011) is used. After power up, if SEB=1, the device address is decided by this register (default 010 1010).
 */
typedef union {
  struct {
    uint8_t DADD: 7;    //!< Programmed device address when SEB=1: If SEB=0, the default device address (0101011) is used. After power up, if SEB=1, the device address is decided by this register (default 010 1010).
    uint8_t RXCCAD:1;   //!< RX CCA threshold MSB. See CCA register 19h.
  } arg;
  uint8_t raw;
} qn800x_dev_add;

/**
 * @ingroup group00
 * @brief Analog control functions.
 * 
 * | XTAL Value | Frequency in MHz  |
 * | ---------- | ----------------  | 
 * | 0000  (0)  | 11.2896           |
 * | 0001  (1)  | 12                | 
 * | 0010  (2)  | 12.288            | 
 * | 0011  (3)  | 13                | 
 * | 0100  (4)  | 16.367            | 
 * | 0101  (5)  | 18.414            | 
 * | 0110  (6)  | 19.2              | 
 * | 0111  (7)  | Reserved          | 
 * | 1000  (8)  | 22.5792           |
 * | 1001  (9)  | 24                |
 * | 1010 (10)  | 24.576            |   
 * | 1011 (11)  | 26 (default)      |
 * | 1100 (12)  | 32.734            | 
 * | 1101 (13)  | 36.828            | 
 * | 1110 (14)  | 38.4              | 
 * | 1111 (15)  | 7.6               |  
 */
typedef union {
  struct {
    uint8_t XSEL:4;       //!< Crystal Frequency Selection (See table). 
    uint8_t ANT_SEL:1;    //!< Select the antenna for TX channel scan mode: 0=Use the receiver antenna from RFI; 1=Use the transmitter antenna on RFO.
    uint8_t RSTB_BB:1;    //!< Reset signal of baseband data-path: (Low active): 0=Reset; 1=No action
    uint8_t I2S_SCKINV:1; //!< I2S MCK invert: 0=Non inverted; 1=Inverted
    uint8_t MUTE_EN:1;    //!< TX and RX audio mute enable: 0=Un-mute;1=Mute
  } arg;
  uint8_t raw;
} qn800x_anactl1;

/**
 * @ingroup group00
 * @brief TX mode input impedance, crystal cap load setting.
 * @details TX mode input impedance for both L/R channels is expressed in kΩ. The table below shows the velues available.
 *
 * | RIN Value | Impedance  | 
 * | --------- | ---------- | 
 * | 00 (0)    | 10kΩ       |
 * | 01 (1)    | 20kΩ       |
 * | 10 (2)    | 40kΩ       |
 * | 11 (3)    | 80kΩ       |
 *   
 */
typedef union {
  struct {
    uint8_t XCSEL: 6;   //!< Crystal cap load setting: The loading cap on each side is: 10+XCSEL*0.32 pF, i.e. it ranges from 10pF to 30pF. Default is 20 pF.
    uint8_t RIN: 2;     //!< TX mode input impedance for both L/R channels: (kΩ)
  } arg;
  uint8_t raw;
} qn800x_reg_vga;


/**
 * @ingroup group00
 * @brief Device ID
 */
typedef union {
  struct {
    uint8_t   CID2:2;
    uint8_t   CID1:4;
    uint8_t   Rsvd:2;
  } arg;
  uint8_t raw;
} qn800x_cidr1;

/**
 * @ingroup group00
 * @brief Device ID
 */
typedef union {
  struct {
    uint8_t   CID4:2;
    uint8_t   CID3:4;
  } arg;
  uint8_t raw;
} qn800x_cidr2;


/**
 * @ingroup group00
 * @brief Sets I2S parameters.
 * @details I2S format in TX mode
 * | TX mode value | Description        | 
 * | ------------- | -----------------  | 
 * |  000 (0)      | MSB justified mode | 
 * |  001 (1)      | I2S mode           |
 * |  010 (2)      | DSP1 mode          |
 * |  011 (3)      | DSP2 mode          |
 * |  100 (4)      | LSB justified mode |
 * |  > 4          | Reserved           |  
 *
 * @details I2S data rate
 * | I2S data rate | Description  | 
 * | ------------- | -------------| 
 * |   00 (0)      | 32 kbps      | 
 * |   01 (1)      | 40 kbps      |
 * |   10 (2)      | 44.1 kbps    |
 * |   11 (3)      | 48 kbps      |
 *
 * @details I2S bit width
 * | I2S bit width | Description  | 
 * | ------------- | -------------| 
 * |   00 (0)      |  8 bits      | 
 * |   01 (1)      | 16 bits      |
 * |   10 (2)      | 24 bits      |
 * |   11 (3)      | 32 bits      |
 */
typedef union {
  struct {
    uint8_t I2SFMT: 3;      //!< See I2S format in TX mode table
    uint8_t I2SMODE:1;      //!< I2S mode: 0=Slave; 1=Master
    uint8_t I2SDRATE:2;     //!< See I2S data rate table
    uint8_t I2SBW:2;        //!< See I2S bit width table
  } arg;
  uint8_t raw;
} qn800x_i2s;


/**
 * @ingroup group00
 * @brief Lower 8 bits of 10-bit channel index.
 * @details Lower 8 bits of 10-bit Channel Index: Channel used for TX/RX has two origins, one is from this 
 * @details register and CH[9:8] at 0Bh which can be written by the user, another is from CCA/CCS. 
 * @details CCA/CCS selected channels are stored in an internal register, which is different from the CH 
 * @details register, but it can be read out through register CH and be used for TX/RX when CCA_CH_DIS 
 * @details (reg. 00h bit [0]) = 0.
 */
typedef union {
  uint8_t CH;
  uint8_t raw;
} qn800x_ch;


/**
 * @ingroup group00
 * @brief Lower 8 bits of 10-bit channel scan start channel index.
 */
typedef union {
  uint8_t CH_STA;
  uint8_t raw;
} qn800x_ch_start;

/**
 * @ingroup group00
 * @brief Lower 8 bits of 10-bit channel scan stop channel index.
 */
typedef union {
  uint8_t CH_STP;
  uint8_t raw;
} qn800x_ch_stop;

/**
 * @ingroup group00
 * @brief Channel scan frequency step. Highest 2 bits of channel indexes.
 * @details Frequency step table
 * | Frequency step | Description | 
 * | -------------  | ------------| 
 * |   00 (0)       |  50 kHz     | 
 * |   01 (1)       | 100 kHz     |
 * |   10 (2)       | 200 KHz     |
 * |   11 (3)       | Reserved    |
 */
typedef union {
  struct {
    uint8_t CH:2;       //!< Highest 2 bits of 10-bit channel index: Channel freq is (76+CH*0.05) MHz.
    uint8_t CH_STA:2;   //!< Highest 2 bits of 10-bit CCA (channel scan) start channel index: Start freq is (76+CH_STA*0.05) MHz.
    uint8_t CH_STP:2;   //!< Highest 2 bits of 10-bit CCA (channel scan) stop channel index: Stop freq is (76+CH_STP*0.05) MHz.
    uint8_t FSTEP:2;    //!< See CCA (channel scan) frequency step table 
  } arg;
  uint8_t raw;
} qn800x_ch_step;


/**
 * @ingroup group00
 * @brief Output power calibration control.
 * @details PA calibration target value. PA output target is (0.37*PAC_TARGET+68) dBuV. Valid values are 31-131 dBuV.
 */
typedef union {
  uint8_t PAC_TARGET;   //!< Valid values are 31-131 dBuV
  uint8_t raw;
} qn800x_pac_target;


/**
 * @ingroup group00
 * @brief Sets TX gain parameters.
 * | TXAGC_GVGA | Attenuation/Gain depending on RIN - 0, 1, 2 AND 3 RESPECTIVELY  |
 * | ---------- | ---------------------------------------------------------- |
 * |  0 - 0000  |  4.5; -1.5; -7.5; -13.5 |
 * |  1 - 0001  |  6.0;  0.0; -6.0; -12.0 |
 * |  2 - 0010  |  7.5;  1.5; -4.5; -10.5 |
 * |  3 - 0011  |  9.0;  3.0; -3.0; -9.0  |
 * |  4 - 0100  | 10.5;  4.5; -1.5; -7.5  |
 * |  5 - 0101  | 12.0;  6.0;  0.0; -6.0  |  
 * |  6 - 0110  | 13.5;  7.5;  1.5; -4.5  |
 * |  7 - 0111  | 15.0;  9.0;  3.0; -3.0  |
 * |  8 - 1000  | 16.5; 10.5;  4.5; -1.5  | 
 * |  9 - 1001  | 18.0; 12.0;  6.0;  0.0  |
 * | 10 - 1010  | 19.5; 13.5;  7.5;  1.5  |
 * | 11 - 1011  | 21.0; 15.0;  9.0;  3.0  |
 * |  Others    | Reserved |
 */
typedef union {
  struct {
    uint8_t TXAGC_GVGA:4;     //!< TX input buffer gain: (dB). See table.
    uint8_t TXAGC_GDB:1;      //!< TX digital gain: 0=0 dB; 1=1dB
    uint8_t IMR:1;            //!< Image Rejection: 0=LO<RF, image is in lower side;1=LO>RF, image is in upper side.
    uint8_t TAGC_GAIN_SEL:1;  //!< TX AGC Gain selection method:
    uint8_t TX_SFTCLPEN:1;    //!< TX soft clipping enable:0=Disabled;1=Enabled 
  } arg;
  uint8_t raw;
} qn800x_txagc_gain;



/**
 * @ingroup group00
 * @brief Specify total TX frequency deviation.
 */
typedef union {
  uint8_t TX_FDEV;    //!< Specify total TX frequency deviation: TX frequency deviation = 0.69 kHz*TX_FEDV.
  uint8_t raw;
} qn800x_tx_fdev;


/**
 * @ingroup group00
 * @brief Gain of TX pilot frequency deviation, I2S buffer clear.
 * @details TX CCA / RX CCA Interrupt Enable: When CCA_INT_EN=1, a 4.5ms low pulse will be output from DIN/INT (RX mode) or DOUT/INT (TX mode) when TXCCA (TX mode) or a RXCCA (RX mode) is finished.
 * @details RDS RX/TX Interrupt Enable: When RDS_INT_EN=1, a 4.5ms low pulse will be output from DIN/INT (RX mode) or DOUT/INT (TX mode) when a new group of data in RDSD0~RDSD7 is loaded into the internal transmitting buffer after user toggles RDSTXRDY (TX mode) or a new group of data is received and stored into RDS0~RDS7 (RX mode).
 * @details GAIN_TXPLT - Gain of TX pilot to adjust pilot frequency deviation: values from 7 to 10.
 *
 */
typedef union {
  struct {
    uint8_t CCA_INT_EN:1;     //!< TX CCA / RX CCA Interrupt Enable: 0=Disable;1=Enable
    uint8_t RDS_INT_EN:1;     //!< RDS RX/TX Interrupt Enable: 0=Disable;1=Enable 
    uint8_t GAIN_TXPLT:4;     //!< Gain of TX pilot to adjust pilot frequency deviation: Refer to peak frequency deviation of MPX signal when audio input is full scale.
    uint8_t I2SOVFL_CLR:1;    //!< I2S buffer overflow clear: User has to de-assert this bit after clearing: 0=No action;1=Clear
    uint8_t I2SUNDFL_CLR:1;   //!< I2S buffer underflow clear: User has to de-assert this bit after clearing: 0=No Action;1=Clear
  } arg;
  uint8_t raw;
} qn800x_gain_txplt;

/**
 * @ingroup group00
 *
 * @brief RDS - RDS data byte 0 to byte 7 
 *
 */

typedef union {
  struct {
    uint8_t RDSD0; //!< RDS data byte 0  - 0x10
    uint8_t RDSD1; //!< RDS data byte 1  - 0x11
    uint8_t RDSD2; //!< RDS data byte 2  - 0x12
    uint8_t RDSD3; //!< RDS data byte 3  - 0x13
    uint8_t RDSD4; //!< RDS data byte 4  - 0x14
    uint8_t RDSD5; //!< RDS data byte 5  - 0x15
    uint8_t RDSD6; //!< RDS data byte 6  - 0x16
    uint8_t RDSD7; //!< RDS data byte 7  - 0x17
  } arg;
  uint8_t data[8];
} qn800x_rds;


/**
 * @ingroup group00
 * @brief RDSFDEV - Specify RDS frequency deviation, RDS mode selection.
 */
typedef union {
  struct {
    uint8_t RDSFDEV:7;     //!< Specify RDS frequency deviation: RDS frequency deviation = 0.35 kHz*RDSFDEV.
    uint8_t RDS_ONLY:1;    //!< RDS mode selection: 0=RDS and MMBS blocks;1=RDS block only.
  } arg;
  uint8_t raw;
} qn800x_rdsfdev;


/**
 * @ingroup group00
 * @brief Device ID
 */
typedef union {
  struct {
    uint8_t   dummy;
  } arg;
  uint8_t raw;
} qn800x_xxxx;

/**
 * @ingroup group00
 * @brief Device ID
 */
typedef union {
  struct {
    uint8_t   dummy;
  } arg;
  uint8_t raw;
} qn800x_yyyy;




/**
 * @ingroup group00 RDS
 * @brief RDS - First block (RDS_BLOCK1 datatype)
 * @details PI Code Function: Identifies the radio station. This code is essential 
 * @details for allowing receivers to identify the source of the radio signal.
 */
typedef union { 
  struct {
    uint8_t reference : 7 ;
    uint8_t programId : 4 ;
    uint8_t countryId : 4 ;
  } field;
  unsigned char byteContent[2];
  uint16_t pi;   // pi Code
} RDS_BLOCK1;



/**
 * @ingroup group00 RDS
 *
 * @brief Block 2 (RDS_BLOCK2 data type)
 * @details Specifies the type of data being transmitted and includes information such as 
 * @details program type (e.g., news, music) and whether the station transmits traffic information.
 * @details The table below show some program types you can use  to check your transmitter.
 * | PTY Code	 | Program Type |
 * | ----------| ------------ |  
 * | 0	| No PTY (undefined)  | 
 * | 1	| News | 
 * | 3	| Information | 
 * | 4	| Sport | 
 * | 5	| Education | 
 * | 7	| Culture | 
 * | 8	| Science |
 * | 10	| Pop Music | 
 * | 11	| Rock Music |
 * | 15	| Other Music |
 * | 16	| Weather |
 * | 17	| Finance |
 * | 18	| Children's Programs |
 * | 20	| Religion |
 * | 24	| Jazz Music |
 * | 25	| Country Music |
 * | 26	| National Music |
 * | 27	| Oldies Music |
 * | 28	| Folk Music |
 * | 29	| Documentary |
 * | 31	| Alarm |
 *
 * @details For GCC on System-V ABI on 386-compatible (32-bit processors), the following stands:
 * 1) Bit-fields are allocated from right to left (least to most significant).
 * 2) A bit-field must entirely reside in a storage unit appropriate for its declared type.
 *    Thus a bit-field never crosses its unit boundary.
 * 3) Bit-fields may share a storage unit with other struct/union members, including members that are not bit-fields.
 *    Of course, struct members occupy different parts of the storage unit.
 * 4) Unnamed bit-fields' types do not affect the alignment of a structure or union, although individual
 *    bit-fields' member offsets obey the alignment constraints.
 * @see also https://en.wikipedia.org/wiki/Radio_Data_System
 *
 */

typedef union
{
    struct
    {
        uint16_t additionalData : 4;         //!< Additional data bits, depending on the group.
        uint16_t textABFlag : 1;             //!< Do something if it chanhes from binary "0" to binary "1" or vice-versa
        uint16_t programType : 5;            //!< PTY (Program Type) code
        uint16_t trafficProgramCode : 1;     //!< (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint16_t versionCode : 1;            //!< (B0) => 0=A; 1=B
        uint16_t groupType : 4;              //!< Group Type code.
    } commonFields;

    struct
    {
        uint16_t address : 2;                //!< Depends on Group Type and Version codes. If 0A or 0B it is the Text Segment Address.
        uint16_t DI : 1;                     //!< Decoder Control bit
        uint16_t MS : 1;                     //!< Music/Speech
        uint16_t TA : 1;                     //!< Traffic Announcement
        uint16_t programType : 5;            //!< PTY (Program Type) code
        uint16_t trafficProgramCode : 1;     //!< (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint16_t versionCode : 1;            //!< (B0) => 0=A; 1=B
        uint16_t groupType : 4;              //!< Group Type code.
    } group0Field;

    struct
    {
        uint16_t address : 4;                //!< Depends on Group Type and Version codes. If 2A or 2B it is the Text Segment Address.
        uint16_t textABFlag : 1;             //!< Do something if it changes from binary "0" to binary "1" or vice-versa
        uint16_t programType : 5;            //!< PTY (Program Type) code
        uint16_t trafficProgramCode : 1;     //!< (TP) => 0 = No Traffic Alerts; 1 = Station gives Traffic Alerts
        uint16_t versionCode : 1;            //!< (B0) => 0=A; 1=B
        uint16_t groupType : 4;              //!< Group Type code.
    } group2Field;
    unsigned char byteContent[2];
    uint16_t raw;                            //!< Raw 16-bit representation
} RDS_BLOCK2;

/**
 * @ingroup group00 RDS
 * @brief Block 3 (RDS_BLOCK3 data type)
 */
typedef union {
  unsigned char byteContent[2];
  uint16_t raw;
} RDS_BLOCK3;

/**
 * @ingroup group00 RDS
 * @brief Block 4 (RDS_BLOCK4 data type)
 */
typedef union {
  unsigned char byteContent[2];
  struct {
    uint16_t  offset: 5; //!< Local Time Offset  
    uint16_t  offset_sign: 1; //!< Offset sign (+/-) 
    uint16_t  min:  6; //!< UTC Minutes (0–59)  
    uint16_t  hour: 4; //!< Four least significant bits of the hour - UTC Hours (0–23)
  } utc;
  uint16_t raw;
} RDS_BLOCK4;

typedef union
{
    struct
    {
        uint32_t offset : 5;       //!< Local Time Offset
        uint32_t offset_sense : 1; //!< Local Offset Sign ( 0 = + , 1 = - )
        uint32_t minute : 6;       //!< UTC Minutes
        uint32_t hour : 5;         //!< UTC Hours
        uint32_t mjd : 17;         //!< Modified Julian Day Code
    } arg;
    uint8_t raw[6];
} RDS_DATE_TIME;


typedef union { 
  uint16_t value;
  uint8_t  raw[2];  
} WORD16;


/**
 * @ingroup  CLASSDEF
 * @brief QN800X Class
 * @details This class implements all functions that will help you to control
 * the QN800X devices.
 *
 * @author PU2CLR - Ricardo Lima Caratti
 */
class QN800X {
private:

protected:

public:


bool detectDevice();
uint8_t scanI2CBus(uint8_t *device);
uint8_t getRegister(uint8_t registerNumber); 
void setRegister(uint8_t registerNumber, uint8_t value);


void convertToChar(uint16_t value, char *strValue, uint8_t len, uint8_t dot, uint8_t separator, bool remove_leading_zeros);
char* formatCurrentFrequency(char decimalSeparator);


};
#endif // _qn800x_H