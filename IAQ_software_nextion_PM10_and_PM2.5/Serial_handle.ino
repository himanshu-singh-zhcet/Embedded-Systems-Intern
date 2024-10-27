void serialHandle() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();

    if (s.startsWith("debug")) {                             //// debug mode
      debug = !debug;
      Serial.print(F("Debug Mode: "));
      debug ? Serial.println("ON") : Serial.println("OFF");
    }
    else if (s.startsWith("help")) {                             //// debug mode
      Serial.println(F("Debug       : <debug>"));
      Serial.println(F("Send Data   : <pushdata>"));
      Serial.println(F("SampleTime  : <logT:value>"));
      Serial.println(F("Log Data Now: <logData>"));
      Serial.println(F("K Fac Sox   : <sox:factor>"));
      Serial.println(F("K Fac Nox   : <nox:factor>"));
    }
    else if (s.startsWith("config")) {                             //// debug mode
      Serial.print(F("DEVICE ID    : ")); Serial.println(device.PID);
      Serial.print(F("SERIAL NO    : ")); Serial.println(device.SER);
      Serial.print(F("Slave ID     : ")); Serial.println(slaveID);
      Serial.print(F("Last Updated : ")); Serial.println(String(__DATE__) + " " + String(__TIME__));
      Serial.print(F("Debug        : ")); Serial.println(debug ? "ON" : "OFF");
      Serial.print(F("SampleTime   : ")); Serial.println(PumpTime);
      Serial.print(F("K Fac PM2_5  : ")); Serial.println(pm25calib);
      Serial.print(F("K Fac PM10   : ")); Serial.println(pm10calib);
    }
    else if (s.startsWith("logT")) {   // st:10
      s.remove(0, s.indexOf(":") + 1);
      PumpTime = s.toInt();
      EEPROM.write(E_SAMPLEINTERVAL, PumpTime);
      Serial.println("Log Time Updated: " + String(PumpTime));
    }
    else if (s.startsWith("logData")) {                             //// debug mode
      Serial.print(F("Log Data ... "));
      saveToSD() ? Serial.println("Success") : Serial.println("Failed");
    }
    else if (s.startsWith("pm25")) {
      s.remove(0, s.indexOf(':') + 1);
      pm25calib = s.toFloat();
      EEPROM.put(E_pm25calib, pm25calib);
      Serial.println("PM2_5 Fac: " + String(pm25calib));
    }
    else if (s.startsWith("pm10")) {
      s.remove(0, s.indexOf(':') + 1);
      pm10calib = s.toFloat();
      EEPROM.put(E_pm10calib, pm10calib);
      Serial.println("PM10 Fac: " + String(pm10calib));
    }
    else if (s.startsWith("sox")) {
      s.remove(0, s.indexOf(':') + 1);
      soxcalib = s.toFloat();
      EEPROM.put(E_soxcalib, soxcalib);
      Serial.println("SOX Fac: " + String(soxcalib));
    }
    else if (s.startsWith("nox")) {
      s.remove(0, s.indexOf(':') + 1);
      noxcalib = s.toFloat();
      EEPROM.put(E_noxcalib, noxcalib);
      Serial.println("NOX Fac: " + String(noxcalib));
    }
    else if (s.startsWith("sox")) {
      s.remove(0, s.indexOf(':') + 1);
      soxcalib = s.toFloat();
      EEPROM.put(E_soxcalib, soxcalib);
      Serial.println("SOX Fac: " + String(soxcalib));
    }
    else if (s.indexOf("##$$&&") > -1) {
      Serial.print(F("Reseting Product ID ... "));
      EEPROM.write(E_CONFIG, 255);
      Serial.println(F("Done"));
      resetFunc();
    }
    else {
      handleSoftwareConnect(s);
    }
  }
}
