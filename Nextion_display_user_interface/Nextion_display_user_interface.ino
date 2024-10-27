#include "DHT.h"
#include "EEPROM.h"
#include<WiFi.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define displaySerial Serial2

unsigned long previoustime = 0;
unsigned long currenttime = 0;
unsigned long elapsedtime = 0;
int interval = 2000;

String ssid, password,Message;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  EEPROM.begin(512);
  displaySerial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  currenttime = millis();
  elapsedtime = currenttime - previoustime;
  if (elapsedtime >= interval) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.println(F("°C "));
    previoustime = currenttime;
    String temp = "t0.txt=\"Temp: "  + String(t) + "\"";
    writeString(temp);
    //  String humi = "t2.txt=\"Humi: " + String(h) + "\"";
    //  writeString(humi);
    int i = map(t, 20, 30, 1, 100);
    String te = "j0.val=" + String(i);
    writeString(te);
  }
  if (displaySerial.available()) {
    long lt = millis();
    int i = 0;
    String response1;
    while (millis() - lt < 50) {
      while (displaySerial.available()) {
        if (displaySerial.available()) {
          byte b = displaySerial.read();
          response1 += (char)b;
        }
      }
    }
    while (response1.indexOf("$") > -1) {
      response1.remove(0, response1.indexOf("$") + 1);
      String response = response1.substring(0, response1.indexOf("$"));
      //      response1.remove(0, response1.indexOf("$") + 1);

      Message = "Display message: " + response + "\n";
      Serial.print(Message);
      response.trim();
    }
  }
}

void writeString(String stringData) {
  for (int i = 0; i < stringData.length(); i++)
    displaySerial.write(stringData[i]);

  displaySerial.write(0xff);
  displaySerial.write(0xff);
  displaySerial.write(0xff);
  displaySerial.flush();
}

void setwifi() {
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED and millis() <= interval + 5000) {
    delay(500);
    Serial.println("Connecting to wifi");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to wifi Network");
  }
}
