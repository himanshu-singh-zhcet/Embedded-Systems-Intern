//void getPM() {
//  if (pmSerial.available()) {
//    byte response[9];
//    int i = 0;
//    unsigned long t = millis();
//
//    if (debug)
//      Serial.print("PM resp .. ");
//    while (millis() - t < 100) {
//      while (pmSerial.available()) {
//        response[i++] = pmSerial.read();
//        if (debug) {
//          Serial.print(response[i]);
//          Serial.print(" ");
//        }
//      }
//    }
//    if (debug)
//      Serial.println();
//    //    para_val[PM1] = ((response[6] << 8) + response[7]);
//    para_val[PM2_5] = ((response[2] << 8) + response[3]) * pm25calib;
//    para_val[PM10] = ((response[4] << 8) + response[5]) * pm10calib;
//
//    para_val_str[PM2_5] = String(para_val[PM2_5], 0);
//    para_val_str[PM10]  = String(para_val[PM10], 0);
//  }
//}

void getSensorValue() {
  float soxZeroPoint = 0, noxZeroPoint = 0;
  float v1 = ADS.readADC(0) * adsFactor,
        v2 = ADS.readADC(1) * adsFactor;
  para_val[CO] = ((v2 - 0.6) * 1000 / 2.4);     // 20 / 2.4 * (v1 - soxZeroPoint);
  para_val[O3] = abs((v1 - 2.0) * 20.0 / 2.0);    //20 / 2.0 * (v2 - noxZeroPoint);

  if (debug) {
    Serial.println("Volts1: " + String(v1));
    Serial.println("Volts2: " + String(v2));

    Serial.println("raw co: " + String(para_val[CO]));
    Serial.println("raw o3: " + String(para_val[O3]));
  }

  para_val[CO] *= cocalib;
  para_val[O3] *= o3calib;

  //  Convert to ug/m3
  //  para_val[SOX] = (40.9 * para_val[SOX] * 64.06) / 30.0;
  //  para_val[NOX] = (40.9 * para_val[NOX] * 46.01) / 30.0;
  //
  //  if (para_val[SOX] <= 0) {
  //    para_val[SOX] = 0.01 + random(-1, 2) / 100.0;
  //    para_val[SOX] = (40.9 * para_val[SOX] * 64.06) / 30.0;
  //  }
  //  if (para_val[NOX] <= 0) {
  //    para_val[NOX] = 0.01;
  //    para_val[NOX] = (40.9 * para_val[NOX] * 46.01) / 30.0;
  //  }

  for (int i = 0; i < no_para; i++)
    para_val_str[i] = String(para_val[i], 2);

}
