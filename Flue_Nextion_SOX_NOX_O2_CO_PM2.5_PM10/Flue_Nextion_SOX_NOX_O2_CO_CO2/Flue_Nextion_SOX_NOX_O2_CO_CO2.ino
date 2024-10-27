#define ModelNo "IAQ-G-A5"
#define OSVer "EES 3.0.1"

#define last_updated String(__DATE__) + " " + String(__TIME__)
#define sensor_info "ELECTROCHEMICAL-SOX-NOX-CO-O2 (ADS-0x4A), NDIR-CO2 (Serial 3), Nextion-3.5 (SoftwareSerial[11,12])"

#include <Wire.h>                //Prerequisite to i2c communications
#include <SdFat.h>
#include <EEPROM.h>              //To save the data at ROM
#include <avr/wdt.h>
#include "ADS1X15.h"
//#include <ModbusRtu.h>
#include <RtcDS3231.h>
#include <SoftwareSerial.h>

#define SerialConnect Serial
#define pmSerial      Serial1
#define CO2Serial     Serial2
#define displaySerial Serial3


#define ssPin 53
#define sdPower 6
#define pumpPin A8

uint8_t slaveID = 1;

RtcDS3231<TwoWire> Rtc(Wire);
//Modbus slave(slaveID, RS485Serial, 0);

// SD declarations
SdFat sd1, sd;
SdFile file;
SdFile dirFile;

// EEPROM Declare
#define E_CONFIG      0
#define E_SLAVE       10
#define E_LOGINTERVAL 150
#define E_DEVICE_INFO 300
//#define E_SAMPLEINTERVAL 400


//////////DEVICE ID//////////////////////
struct device_info {
  char PID[13];
  char SER[16];
} device;

////////////////Display//////////////////
String sys0 = "-1";   //  0 => GSM, 1 => WiFi, -1 => No Telemetry Option
String sys1 = "-2";   //  0-4 => set wifi/gsm strength, -1 => no wifi/gsm connected, -2 => hide wifi/gsm strength
String sys2 = "0";    //  0 => tabular homepage, 1 => 1 card homepage, 2 => 2 card homepage
String homepage = "page homepage";   //  0 => tabular homepage, 1 => 1 card homepage, 2 => 2 card homepage || (homepage0-2)


////////////////parameters/////////////////
const uint8_t no_para = 7;
const uint8_t paraPerDisp = 4.0;
const uint8_t  no_disp = ceil((float)no_para / (float)paraPerDisp);

enum para_enum {SOX, NOX, O2, CO, CO2, PM2_5, PM10};
String para[no_para] = {"SOX", "NOX", "O2", "CO", "CO2", "PM2.5", "PM10"};
String para_unit[no_para] = {"PPM", "PPM", "%", "PPM", "PPM", "ug/m^3", "ug/m^3"};
String para_val_str[no_para] = {"0", "0", "0", "0", "0", "0", "0"};
float para_val[no_para];
int eeprom_addr[no_para] = {20, 30, 40, 50, 60, 70, 80};
float para_fac[no_para];

///////////////////////////////////////////
// Sensor Variables

//uint16_t au16data[8];

bool startUp = true, debug = false;
uint32_t lt = millis();
//float slope, intercept;
int choice, rst_counter = 0, logTime = 1;
long int lts;

void(* resetFunc) (void) = 0;

ADS1115 ADS(0x48);

volatile bool wd_timer_flag = false;

// Loops
unsigned long lt4, serial_lt, log_lt = 0;

const uint16_t nMax = 50;
int fileIndex = 0; // Position of file's directory entry.
uint16_t dirIndex[nMax];
bool checkBox[nMax];
String line = "";
//int choice;
uint16_t n = 0;
bool enter = true;
String SelectedFiles[50], network;
bool softFlag = false;
bool homePageFlag = false;
String sendThis = "";
int k = 0;
float adsFactor;
String Message;

// for CO2 Sensors
byte askCO2[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

//Variable Declarations for PM Sensors
byte ask[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte setMode[] = {0xFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47};
byte qaMode[] = {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};


ISR (WDT_vect)
{
  rst_counter++;
  if (rst_counter == 450) {   //15
    resetFunc();
  }

}  // end of WDT_vect

void setup() {
  Serial.begin(115200);
  CO2Serial.begin(9600);
  displaySerial.begin(9600);
  pmSerial.begin(9600);

  //  slave.begin(9600);

  deviceConfigure();
  delay(100);
  
  Serial.println(1);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
Serial.println(2);

  pmSerial.write(qaMode, 9);
  
  Rtc.Begin();
  delay(200);
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  getPM();

  CO2Serial.write(askCO2, 9);
  delay(100);
  pmSerial.write(ask, 9);
  handleSoftwareConnect();

  ADS.begin();
  ADS.setGain(0);
  adsFactor = ADS.toVoltage(1);

  delay(200);
  initNextion();
  getPM();
  getCO2();
  getsensorValues();
  Serial.println("Startup");
  log_lt = millis();
}

void loop() {

  //  slave.poll( au16data, 8 );
  if (millis() - serial_lt > 5000L) {
    pmSerial.write(ask, 9);
    CO2Serial.write(askCO2, 9);
    delay(100);
    getsensorValues();

    if (debug) {
      for (int k = 0; k < no_para; k++) {
        Serial.println(para[k] + " : " + para_val_str[k]);
      }
      Serial.println();
    }

    sendDisplayVal();

    //    au16data[0] = (int)para_val[PM2_5];
    //    au16data[1] = (int)para_val[PM10];
    //    au16data[2] = (para_val[TEMP] * 100.0);
    //    au16data[3] = (para_val[HUM] * 100.0);
    //    au16data[4] = (para_val[SOX] * 100.0);
    //    au16data[5] = (para_val[NOX] * 100.0);
    //    au16data[6] = (para_val[CO] * 100.0);
    serial_lt = millis();
  }
  getPM();
  getCO2();
  if (!softFlag) {

    if (millis() - log_lt > logTime * 60000UL) {
      Serial.println(F("PUMP ON"));
      digitalWrite(pumpPin, LOW);
      log_lt = millis();
    }

    if ( millis()  > log_lt + 30000UL) {
      if (!digitalRead(pumpPin)) {
        Serial.println(F("PUMP OFF"));
        saveToSD() ? Serial.println("Data Saved") : Serial.println("Memory Error!");
        digitalWrite(pumpPin, HIGH);
      }
    }
  }

  if (softFlag && millis() - lt > 30000UL ) {
    softFlag = false;
    writeString(homepage);
  }

  if (!homePageFlag & millis() - lt4 > 60000UL) {
    writeString(homepage);
  }

  handleDisplay();
  handleSoftwareConnect();
}
