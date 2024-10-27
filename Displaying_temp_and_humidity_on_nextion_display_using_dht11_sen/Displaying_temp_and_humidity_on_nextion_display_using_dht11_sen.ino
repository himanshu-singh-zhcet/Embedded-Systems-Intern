#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define displaySerial Serial2

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  displaySerial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  delay(2000);


  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }



  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  String temp = "t0.txt=\"Temp: "  +String(t) + "\"";
  writeString(temp);
//  String humi = "t2.txt=\"Humi: " + String(h) + "\"";
//  writeString(humi);
  int i = map(t,20,30,1,100);
  String te = "j0.val=" +String(i);
  writeString(te);
 
}

void writeString(String stringData) {
  for (int i = 0; i < stringData.length(); i++)
    displaySerial.write(stringData[i]);

  displaySerial.write(0xff);
  displaySerial.write(0xff);
  displaySerial.write(0xff);
  displaySerial.flush();
}
