#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include <ModbusRtu.h>
#include "ADS1X15.h"

uint16_t data[10];
ADS1115 ADS(0x48);
RtcDS3231<TwoWire> Rtc(Wire);

Modbus slave(1,Serial1,0);
void setup ()
{
  Serial.begin(9600);
  Serial1.begin(9600,SERIAL_8N1,32,33);
  Rtc.Begin();

  ADS.begin();
  slave.start();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time, updating DateTime");
    Rtc.SetDateTime(compiled);
  }
}
void loop() {
  ADS.setGain(0);
  slave.poll(data,10 );
  RtcDateTime now = Rtc.GetDateTime();
  data[0] = get_date(now);
  data[1] = get_month(now);
  data[2] = get_year(now);
  data[3] = get_hour(now);
  data[4] = get_minute(now);
  data[5] = get_second(now);
  Serial.println(data[0]);
  delay(100);
}

int get_date(const RtcDateTime& dt)
{
  return dt.Day();
}

int get_month(const RtcDateTime& dt)
{
  return dt.Month();
}

int get_year(const RtcDateTime& dt)
{
  return dt.Year();
}

int get_hour(const RtcDateTime& dt) {
  return dt.Hour();
}

int get_minute(const RtcDateTime& dt) {
  return dt.Minute();
}

int get_second(const RtcDateTime& dt) {
  return dt.Second();
}
