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
    para_val[PM2_5] = ((response[2] << 8) + response[3]);// * pm25calib;
    para_val[PM10] = ((response[4] << 8) + response[5]);// * pm10calib;

    para_val_str[PM2_5] = String(para_val[PM2_5], 0);
    para_val_str[PM10]  = String(para_val[PM10], 0);
  }
}

void getTH(){
  float _t = dht.readTemperature();
  float _h = dht.readHumidity();
  para_val[TEMP] = _t;
    para_val[HUM] = _h;

    para_val_str[TEMP] = String(para_val[TEMP], 2);
    para_val_str[HUM]  = String(para_val[HUM], 2);
}
