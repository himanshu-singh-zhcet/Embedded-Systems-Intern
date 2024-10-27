#define ModelNo "IAQ-G-A4"
#define OSVer "EES 3.0.1"

// ID ENE01373, SER 2205007100984

#define ssPin 53
#define sdPower 6    // sd card power pin
#define motorPin A8
#define gsmPowerPin 23

#include <Wire.h>                //Prerequisite to i2c communications
#include <EEPROM.h>              //To save the data at ROM
#include <SdFat.h>
#include <avr/wdt.h>
#include "ADS1X15.h"
#include <ModbusRtu.h>
#include <RtcDS3231.h>
//#include <SoftwareSerial.h>

#define SerialConnect Serial
//#define pmSerial      Serial3
#define RS485Serial   Serial2
#define displaySerial   Serial3

//SoftwareSerial displaySerial(11, 12);

const int slaveID = 2;

ADS1115 ADS(0x48);
RtcDS3231<TwoWire> Rtc(Wire);
Modbus slave(slaveID, RS485Serial, 0);

// SD declarations
SdFat sd1, sd;
SdFile file;
SdFile dirFile;

// EEPROM Declare
#define E_CONFIG    0
#define E_cocalib  10
#define E_o3calib  20
#define E_pm25calib 30
#define E_pm10calib 40
#define E_SAMPLEINTERVAL 50
#define E_APN       100
#define E_LOGINTERVAL 150
#define E_DEVICE_INFO 300


//////////DEVICE ID//////////////////////
struct device_info {
  char PID[13];
  char SER[16];
} device;

////////////////Display//////////////////
String sys0 = "-1";   //  0 => GSM, 1 => WiFi, -1 => No Telemetry Option
String sys1 = "-2";   //  0-4 => set wifi/gsm strength, -1 => no wifi/gsm connected, -2 => hide wifi/gsm strength
String sys2 = "2";    //  0 => tabular homepage, 1 => 1 card homepage, 2 => 2 card homepage
String homepage = "page homepage2";   //  0 => tabular homepage, 1 => 1 card homepage, 2 => 2 card homepage || (homepage0-2)


////////////////parameters/////////////////
const uint8_t no_para = 2;
enum para_enum {CO, O3};
String para[no_para] = {"CO", "O3"};
String para_unit[no_para] = {"ppm", "ppm"};
String para_val_str[no_para] = {"0", "0"};
float para_val[no_para];

float adsFactor;

///////////////////////////////////////////
// Sensor Variables

uint16_t au16data[12];

bool startUp = true;
unsigned long pump_lt = 0, logTime = 0, lt_save = 0;
uint32_t lt = millis();
//float slope, intercept;
int choice, PumpTime = 1, valueCounter = 0, saveDataInterval = 10;
long int lts;
bool getValues = true;

String apn = "airteliot.com";

void(* resetFunc) (void) = 0;

const uint16_t nMax = 50;
int fileIndex = 0; // Position of file's directory entry.
uint16_t dirIndex[nMax];
bool checkBox[nMax];
String line = "";
//int choice;
uint16_t n = 0;
bool enter = true;
String SelectedFiles[50], network;
bool softFlag = false, debug = false;
bool homePageFlag = false;
String sendThis = "";
int k = 0;
unsigned long lt4, pm_lt;
bool inibat = true;
int preBat, vb;
float v, VZERO;
float EMA_S, EMA_a = 0.2;
String Message;

volatile int rst_counter = 0, pumpInterval = 0, extPumpInterval = 0;
volatile bool switchPump = false, switchExtPump = false;

//Variable Declarations for PM Sensors
byte ask[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
byte setMode[] = {0xFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47};
byte qaMode[] = {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};
float o3calib , cocalib, pm25calib , pm10calib;
ISR (WDT_vect) {
  rst_counter++;
  //  pumpInterval++;

  //  if (pumpInterval == 6) {
  //    switchPump = true;
  //  }
  //  else if (pumpInterval == 80) {
  //    switchPump = true;
  //  }
  //  else if (pumpInterval == 85) {
  //    switchPump = true;
  //    pumpInterval = 0;
  //  }

  if (rst_counter == 240) {   // 30 mins
    resetFunc();
  }
}  // end of WDT_vect

void setup() {

  pinMode(gsmPowerPin, OUTPUT);

  Serial.begin(115200);

  RS485Serial.begin(9600);
  displaySerial.begin(9600);
  delay(200);
  deviceConfigure();
  delay(100);
  Serial.println(F("PUMP : ON"));

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, HIGH);
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }

  slave.start();

  serialHandle();

  ADS.begin();
  ADS.setGain(0);
  adsFactor = ADS.toVoltage(1);

  Serial.println("Startup");




  delay(500);
  sendDisplayVal();
  initNextion();

  MCUSR = 0;
  // allow changes, disable reset
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval
  WDTCSR = bit (WDIE) | bit (WDP3) | bit (WDP0);    // set WDIE, and 8 seconds delay
  wdt_reset();  // pat the dog
}

void loop() {

  slave.poll( au16data, 12 );

  //  if (switchPump) {
  //    if (digitalRead(motorPin)) {
  //      digitalWrite(motorPin, LOW);
  //      Serial.println(F("Start Pump"));
  //    }
  //    else {
  //      digitalWrite(motorPin, HIGH);
  //      Serial.println(F("Stop Pump"));
  //    }
  //    switchPump = false;
  //  }

  if (millis() - pump_lt > PumpTime * 60000UL) {
    Serial.println(F("PUMP : ON"));
    digitalWrite(motorPin, HIGH);
    pump_lt = millis();
  }

  if ( millis() > pump_lt + 30000UL) {
    if (digitalRead(motorPin)) {
      sendThis = F("page msg_box");
      writeString(sendThis);
      sendThis = F("msg_box.head.txt=\"Sending Data\"");
      writeString(sendThis);
      saveToSD();
      Serial.println(F("PUMP : OFF"));
      digitalWrite(motorPin, LOW);
      //      sendData();
      //      delay(500);
      writeString(homepage);
    }
  }

  if (millis() - pm_lt > 5000L) {
    if (debug)
      Serial.println("ask");

    getSensorValue();

    //    para_val_str[PM2_5] = String(para_val[PM2_5], 0);
    //    para_val_str[PM10]  = String(para_val[PM10], 0);

    if (debug) {
      for (int i = 0; i < no_para; i++)
        Serial.println(para[i] + ": " + para_val_str[i]);
      Serial.println();
    }

    au16data[0] = (uint16_t)(para_val[CO] * 100);
    au16data[1] = (uint16_t)(para_val[O3] * 100);
    sendDisplayVal();
    pm_lt = millis();
  }


  //  if (millis() - lt_save > saveDataInterval * 60000UL) {
  //    saveToSD();
  //    lt_save = millis();
  //  }

  if (softFlag & millis() - lt > 30000L ) {
    softFlag = false;
    writeString(homepage);
  }

  handleDisplay();
  serialHandle();

  if (!homePageFlag & millis() - lt4 > 30000) {
    writeString(homepage);
  }

}
