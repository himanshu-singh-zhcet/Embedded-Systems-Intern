int interval = 10000;
#include "EEPROM.h"
#include<WiFi.h>
String ssid;
String password;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(512);
  ssid = EEPROM.readString(0);
  password = EEPROM.readString(50);

}

void loop() {
   if (WiFi.status() != WL_CONNECTED) {
    while(millis()<=5000){
      Serial.println("wait");
      delay(100);
    }
    Serial.println(ssid);
    Serial.println(password);
    WiFi.begin(ssid.c_str(), password.c_str());
    

    while (WiFi.status() != WL_CONNECTED and millis() <= interval+5000) {
      delay(500);
      Serial.println("Connecting to wifi");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to wifi Network");
    }
  }
  
  handleSerial();
}

void handleSerial() {
  if (Serial.available()) {
    Serial.println("hii");
    String s = Serial.readStringUntil('\n');
    Serial.println(s);
    if (s.startsWith("time")) {
      s.remove(0, s.indexOf(':') + 1);
      Serial.println(s);
      String st[6] = {"DAY ", "MONTH ", "YEAR ", "HOUR ", "MINUTE ", "SECOND "};  // wifi:Redmi 9 Power:12345678
      for (int i = 0; i < 6; i++) {
        int index = s.indexOf(":");
        String p =  s.substring(0, index);
        s.remove(0, s.indexOf(':') + 1);
        Serial.print(st[i]);
        Serial.println(p);
      }
    }
    else if (s.startsWith("wifi")) {
      s.remove(0, s.indexOf(':') + 1);
      Serial.println(s);
      int index = s.indexOf(":");
      ssid = s.substring(0, index);
      password = s.substring(index + 1);
      Serial.println(ssid);
      Serial.println(password);
      EEPROM.writeString(0, ssid);
      EEPROM.writeString(50, password);
      EEPROM.commit();
     
     
    }
  }
}
