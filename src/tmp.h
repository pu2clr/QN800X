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

