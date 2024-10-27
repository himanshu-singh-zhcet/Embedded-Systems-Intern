void deviceConfigure() {
  //    EEPROM.write(E_CONFIG, 255);

  //Start Device Configuration
  if (EEPROM.read(E_CONFIG) == 255) {
    uint8_t a = 0, b = 0;
    Serial.println("Configure Device[Contact Admin]");
    while (true) {
      if (Serial.available()) {
        String msg = Serial.readStringUntil('\n');
        //msg.trim();
        String info = msg.substring(msg.indexOf(":") + 1, msg.length());

        if (msg.indexOf("$@handshake@$") > -1) {
          Serial.print("$hello:$");
        }
        if (msg.indexOf("pid") > -1) {
          info.toCharArray(device.PID, 13);
          Serial.println("Device ID: " + String(device.PID));
          a = 1;
        }
        if (msg.indexOf("sno") > -1) {
          info.toCharArray(device.SER, 16);
          Serial.println("Device Serial Number: " + String(device.SER));
          b = 1;
        }
        if (a + b == 2) {
          EEPROM.put(E_DEVICE_INFO, device);
          EEPROM.write(E_CONFIG, 1);
          Serial.print("$started:$");
          delay(2000);
          break;
        }
      }
    }
  }
  //End Device Configuration

  if (EEPROM.read(E_LOGINTERVAL) == 255) EEPROM.write(E_LOGINTERVAL, logTime);
  //  if (EEPROM.read(E_SAMPLEINTERVAL) == 255) EEPROM.write(E_SAMPLEINTERVAL, PumpTime);
  //  if (EEPROM.read(E_SLOPE) == 255) EEPROM.put(E_SLOPE, slope);
  //  if (EEPROM.read(E_INTERCEPT) == 255) EEPROM.put(E_INTERCEPT, intercept);

  logTime = EEPROM.read(E_LOGINTERVAL);

  EEPROM.get(E_DEVICE_INFO, device);
  //  EEPROM.get(E_SLOPE, slope);
  //  EEPROM.get(E_INTERCEPT, intercept);

  para_fac[SOX] = EEPROM.read(eeprom_addr[SOX]);
  para_fac[NOX] = EEPROM.read(eeprom_addr[NOX]);
  para_fac[CO] = EEPROM.read(eeprom_addr[CO]);
  para_fac[PM2_5] = EEPROM.read(eeprom_addr[PM2_5]);
  para_fac[PM10] = EEPROM.read(eeprom_addr[PM10]);

  printDash(50);
  Serial.println("SYSTEM WAKEUP...");
  Serial.print(F("DEVICE ID    : ")); Serial.println(device.PID);
  Serial.print(F("SERIAL NO    : ")); Serial.println(device.SER);
  Serial.print(F("SENSOR INFO  : ")); Serial.println(sensor_info);
  Serial.print(F("Last Updated : ")); Serial.println(last_updated);
  Serial.print(F("SOX Factor   : ")); Serial.println(para_fac[SOX]);
  Serial.print(F("NOX Factor   : ")); Serial.println(para_fac[NOX]);
  Serial.print(F("CO Factor    : ")); Serial.println(para_fac[CO]);
  Serial.print(F("PM2.5 Factor    : ")); Serial.println(para_fac[PM2_5]);
  Serial.print(F("PM10 Factor   : ")); Serial.println(para_fac[PM10]);
  Serial.println("FLUE GAS | EESPL | 2023");
  printDash(50);
}

void printDash(int n) {
  for (int i = 0; i < n; i++) Serial.print("#");
  Serial.println();
}
