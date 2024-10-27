/* IN THIS CODE OUR PC ACT AS A MASTER AND ESP32 MICROCONTROLLLER ACT AS A SLAVE OUR MASTER REQUEST EVERY 1 SECOND   
AND THEN SLAVE SENDS THE DATA TO MASTER THAT IS SO2 SENSOR VALUE 
 */


#include <ModbusRtu.h>
#include "ADS1X15.h"

ADS1115 ADS(0x48);
uint16_t x[6];
Modbus slave(1,Serial,0); // this is slave @1 and RS-232 or USB-FTDI

void setup() {
  Serial.begin( 9600 ); 
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);

  ADS.begin();
  slave.start();
}

void loop() {
  ADS.setGain(0);
  slave.poll(x, 6 );
  int16_t val_0 = ADS.readADC(0); 
  float f = ADS.toVoltage(1);  // voltage factor
  float t = val_0 * f;
  float so2_value  = ((t-0.6)*20/2.4);
  x[0] = so2_value*100;
  delay(100);
}
