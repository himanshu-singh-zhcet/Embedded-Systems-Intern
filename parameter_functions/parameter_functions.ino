
#include "ADS1X15.h"
#include "EEPROM.h"
ADS1115 ADS(0x48);

const uint8_t no_para = 12;
enum para_enum {SOX, NOX, CO, O2, NH3, H2S, HCL, H2, PH_3, O3, CL, HF};
String para[no_para] = {"SOX", "NOX", "CO", "O2", "NH3", "H2S", "HCL", "H2", "PH_3", "O3", "CL", "HF"};
String para_val_str[no_para] = {"0"};
float para_val[no_para];
float para_val_cal[no_para];

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);
  para_val_cal[SOX] = EEPROM.readFloat(0);
  para_val_cal[NOX] = EEPROM.readFloat(10);
  para_val_cal[CO] = EEPROM.readFloat(20);
  para_val_cal[O2] = EEPROM.readFloat(30);
  ADS.begin();
}


void loop()
{
  ADS.setGain(0);

  handle_serial();
  Serial.println(get_so2_value());

  delay(1000);
}



void handle_serial() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    Serial.println(s);
    if (s.startsWith("so2")) {
      s.remove(0, s.indexOf(':') + 1);
      para_val_cal[SOX] = s.toFloat();
      EEPROM.writeFloat(0, para_val_cal[SOX] );
      EEPROM.commit();
      Serial.println(para_val_cal[SOX]);
    }
    else if (s.startsWith("no2")) {
      s.remove(0, s.indexOf(':') + 1);
      para_val_cal[NOX] = s.toFloat();
      EEPROM.writeFloat(10, para_val_cal[NOX] );
      EEPROM.commit();
      Serial.println(para_val_cal[NOX]);
    }
    else if (s.startsWith("co")) {
      s.remove(0, s.indexOf(':') + 1);
      para_val_cal[CO] = s.toFloat();
      EEPROM.writeFloat(20, para_val_cal[CO]);
      EEPROM.commit();
      Serial.println(para_val_cal[CO]);
    }
    else if (s.startsWith("o2")) {
      s.remove(0, s.indexOf(':') + 1);
      para_val_cal[O2] = s.toFloat();
      EEPROM.writeFloat(30, para_val_cal[O2] );
      EEPROM.commit();
      Serial.println(para_val_cal[O2]);
    }

  }
}

float get_so2_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC(0);
  float so2_value  = ((v1 - para_val_cal[SOX]) * 20 / 2.4);
  return so2_value;
}



float get_no2_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC(1);
  float no2_value  = ((v1 - para_val_cal[NOX]) * 20 / -2);
  return no2_value;
}



float get_co_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC(2);
  float co_value  = ((v1 - para_val_cal[CO]) * 1000 / 2.4);
  return co_value;
}

float get_o2_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC(3);
  float o2_value  = ((v1 - para_val_cal[O2]) * 100 / -1.5);
  return o2_value;
}

float get_nh3_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC();
  float nh3_value  = ((v1 - para_val_cal[NH3]) * 100 / 2.4);
  return nh3_value;
}


float get_h2s_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC();
  float h2s_value  = ((v1 - para_val_cal[H2S]) * 100 / 2.4);
  return h2s_value;
}

float get_hcl_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC();
  float hcl_value  = ((v1 - para_val_cal[HCL]) * 10 / -2);
  return hcl_value;
}

float get_h2_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC();
  float h2_value  = ((v1 - para_val_cal[H2]) * 1000 / 2.4);
  return h2_value;
}

float get_ph3_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC();
  float ph3_value  = ((v1 - para_val_cal[PH_3]) * 1000 / 2.4);
  return ph3_value;
}

float get_o3_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC();
  float o3_value  = ((v1 - para_val_cal[O3]) * 20 / -2);
  return o3_value;
}

float get_cl2_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC();
  float cl2_value  = ((v1 - para_val_cal[CL]) * 10 / -2);
  return cl2_value;
}

float get_hf_value() {
  float v1 = ADS.toVoltage(1) * ADS.readADC();
  float hf_value  = ((v1 - para_val_cal[HF]) * 10 / -2);
  return hf_value;
}
