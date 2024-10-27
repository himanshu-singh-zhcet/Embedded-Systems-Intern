#include "EEPROM.h"
#define relay1 13
#define relay2 14
#define relay3 26

float the_spm, the_sox, the_nox;
float spm, sox, nox;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  the_spm = EEPROM.readFloat(0);
  the_sox = EEPROM.readFloat(10);
  the_nox = EEPROM.readFloat(20);

}

void loop() {
  static long lt = 0;

  if (millis() - lt > 2000UL ) {
    Serial.println("SOX: " + String(sox));
    Serial.println("NOX: " + String(nox));
    Serial.println("SPM: " + String(spm));
    Serial.println();
    lt = millis();
  }
   handleRelay2();
  handle_serial();
  delay(100);
}

void handle_serial() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    Serial.println(s);
    if (s.startsWith("spm")) {
      s.remove(0, s.indexOf(':') + 1);
      the_spm = s.toFloat();
      EEPROM.writeFloat(0, the_spm);
      EEPROM.commit();
      Serial.println(the_spm);
    }
    else if (s.startsWith("sox")) {
      s.remove(0, s.indexOf(':') + 1);
      the_sox = s.toFloat();
      EEPROM.writeFloat(10, the_sox);
      EEPROM.commit();
      Serial.println(the_sox);
    }
    else if (s.startsWith("nox")) {
      s.remove(0, s.indexOf(':') + 1);
      the_nox = s.toFloat();
      EEPROM.writeFloat(20, the_nox);
      EEPROM.commit();
      Serial.println(the_nox);
    }

    else if (s.startsWith("vspm")) {
      s.remove(0, s.indexOf(':') + 1);
      spm = s.toFloat();
    }
    else if (s.startsWith("vsox")) {
      s.remove(0, s.indexOf(':') + 1);
      sox = s.toFloat();
    }
    else if (s.startsWith("vnox")) {
      s.remove(0, s.indexOf(':') + 1);
      nox = s.toFloat();
    }
  }
}


void handleRelay(){
  if((spm>=the_spm) && digitalRead(relay1)){
    digitalWrite(relay1,LOW);
  }
  else if((spm<the_spm) && !digitalRead(relay1)){
    digitalWrite(relay1,HIGH);
  }
  if((sox>the_sox) && digitalRead(relay2)){
    digitalWrite(relay2,LOW);
  }
  else if((sox<the_sox) && !digitalRead(relay2)){
    digitalWrite(relay2,HIGH);
  }
  if((nox>the_nox) && digitalRead(relay3)){
    digitalWrite(relay3,LOW);
  }
  else if((nox<the_nox) && !digitalRead(relay3)){
    digitalWrite(relay3,HIGH);
  }
 
}

void handleRelay2(){
  if((spm>=the_spm || sox>=the_sox || nox>=the_nox) && digitalRead(relay1) ){
    digitalWrite(relay1,LOW);
  }
  else if((spm<the_spm && sox<the_sox && nox<the_nox) && !digitalRead(relay1) ){
    digitalWrite(relay1,HIGH);
  }
}
