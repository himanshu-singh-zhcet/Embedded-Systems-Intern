
void getsensorValues() {

  float v0 = ADS.readADC(0) * adsFactor;  // SOX
  float v1 = ADS.readADC(1) * adsFactor;  // NOX
  float v2 = ADS.readADC(2) * adsFactor;  // CO
  float v3 = ADS.readADC(4) * adsFactor;  // O2

  para_val[SOX] = abs((v0 - 0.61) * 20 / 2.4) * para_fac[SOX];
  para_val[NOX] = abs((v1 - 2.0) * 20 / 2.0) * para_fac[NOX];
  para_val[CO]  = abs((v2 - 0.60) * 1000 / 2.4) * para_fac[CO];
  para_val[O2] = abs((v3 - 1.5) * 20 / 1.5) * para_fac[O2];

  if (debug) {
    Serial.println("SOX Volts: " + String(v0));
    Serial.println("NOX Volts: " + String(v1));
    Serial.println("CO Volts: " + String(v2));
    Serial.println("O2 Volts: " + String(v3));
  }

  para_val_str[SOX] = String(para_val[SOX], 2);
  para_val_str[NOX] = String(para_val[NOX], 2);
  para_val_str[CO]  = String(para_val[CO], 2);
  para_val_str[O2]  = String(para_val[O2], 2);
}

void getCO2() {
  if (CO2Serial.available()) {
    byte response[9];
    int i = 0;
    long int t = millis();
    if (debug)
      Serial.print("CO2 resp: ");
    while (millis() - t < 100) {
      while (CO2Serial.available()) {
        response[i++] = CO2Serial.read();
        if (debug) {
          Serial.print(response[i - 1], HEX);
          Serial.print(" ");
        }
      }
    }
    if (debug)
      Serial.println();

    para_val[CO2] = ((response[2] << 8) + response[3]) * para_fac[CO2];
    para_val_str[CO2] = String(para_val[CO2], 0);
  }
}

void getPM() {
  if (pmSerial.available()) {
    byte response[9];
    int i = 0;
    unsigned long t = millis();

    if (debug)
      Serial.print("PM resp .. ");
    while (millis() - t < 100) {
      while (pmSerial.available()) {
        response[i++] = pmSerial.read();
        if (debug) {
          Serial.print(response[i]);
          Serial.print(" ");
        }
      }
    }
    if (debug)
      Serial.println();
    //    para_val[PM1] = ((response[6] << 8) + response[7]);
    para_val[PM2_5] = ((response[2] << 8) + response[3]) * para_fac[PM2_5];
    para_val[PM10] = ((response[4] << 8) + response[5]) * para_fac[PM10];

    para_val_str[PM2_5] = String(para_val[PM2_5], 0);
    para_val_str[PM10]  = String(para_val[PM10], 0);
  }
}
