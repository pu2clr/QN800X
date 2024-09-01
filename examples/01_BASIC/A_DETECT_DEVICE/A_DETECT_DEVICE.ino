#include <QN800X.h>


QN800X  dv; 

void setup() {

    uint8_t deviceList[5], deviceCount = 0;
    char str[80];

    Serial.begin(9600);
    while(!Serial);

    deviceCount = dv.scanI2CBus(deviceList);
    if ( deviceCount > 0  ) { 
        for ( uint8_t  i = 0; i < deviceCount; i++ ) { 
            sprintf(str,"\nDevice found  at: %x in HEX - %d in DEC", deviceList[i], deviceList[i]);
            Serial.print(str);
        }
    }

    sprintf(str,"\nProduct ID:  %d \nProduct Family: %d",  dv.getDeviceProductID().arg.CID1, dv.getDeviceProductFamily().arg.CID3);
    Serial.print(str);

    
    sprintf(str,"\n\nCheck SUSTEM1: %x ", dv.getRegister(QN_SYSTEM1));
    Serial.print(str);


}

void loop() {

 }

 