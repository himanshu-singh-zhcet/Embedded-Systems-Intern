void handleDisplay() {

  if (displaySerial.available()) {
    lt = millis();
    int resp[20];
    int i = 0;
    byte endByte[3];
    bool startMessage = false;
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
      response1.remove(0, response1.indexOf("$") + 1);

      Message = "Display message: " + response + "\n";
      Serial.print(Message);
      saveLog(Message);
      response.trim();
      if (response == "p_homepage") {
        if (startUp) startUp = false;
        homePageFlag = true;
        softFlag = false;
        //if (!startUp)
        sendDisplayVal();
        // if (startUp) startUp = false;
      } else {
        homePageFlag = false;
        softFlag = true;
        lt4 = millis();
      }
      if (response == F("p_menu")) {

      } else if (response == F("p_splash")) {
        //        sendInfo();
      } else if (response == F("p_setTime")) {
        displayTime();
      } else if (response == F("p_selectPara")) {
        //        for nextion 3.5
        //        sendThis = "t2.txt=\"" + String(cocalib) + "\"";
        //        writeString(sendThis);
        //        sendThis = "t3.txt=\"" + String(o3calib) + "\"";
        //        writeString(sendThis);

        //        for nextion 7
        sendThis = "pHCalib.t0.txt=\"" + String(cocalib) + "\"";
        writeString(sendThis);
        sendThis = "tssCalib.t0.txt=\"" + String(o3calib) + "\"";
        writeString(sendThis);
      } else if (response.startsWith("Time")) {
        response.remove(0, response.indexOf(":") + 1);
        updateTime(response, "hh:mm");
        sendDisplayVal();
      } else if (response == "p_systemInfo") {
        sendInfo();
      } else if (response == "p_modbusInfo") {
        modbusInfo();
      }
      else if (response.startsWith("logInterval")) {
        response.remove(0, response.indexOf(':') + 1);
        response.remove(response.indexOf(':'));
        Serial.println("Log Interval: " + response);
        PumpTime = response.toInt();
        //        EEPROM.write(E_LOGINTERVAL, saveDataInterval);
        EEPROM.write(E_SAMPLEINTERVAL, PumpTime);
        _displayDialog("Message", "Log Interval Updated");
      }
      else if (response.startsWith("calibrate")) {
        _displayDialog("Message", "Device is factory calibrated");
        sendThis = F("p_menu");
        writeString(sendThis);
      }
      else if (response.startsWith("kFac")) {
        response.remove(0, response.indexOf(':') + 1);
        int p = response.substring(0, response.indexOf(':')).toInt();
        response.remove(0, response.indexOf(':') + 1);
        Serial.println(p);
        if (p) {
          o3calib = response.toFloat();
          EEPROM.put(E_o3calib, o3calib);
          //        EEPROM.get(E_o3calib, o3calib);
          Serial.println("NOX Calibration Done");
          _displayDialog("Message", "NOX Calibration Done", "selectPara");
          //          sendThis = F("p_menu");
          //          writeString(sendThis);
        }
        else {
          cocalib = response.toFloat();
          EEPROM.put(E_cocalib, cocalib);
          Serial.println("SOX Calibration Done");
          //        EEPROM.get(E_cocalib, cocalib);
          _displayDialog("Message", "SOX Calibration Done", "selectPara");
        }
      }
      //      else if (response == F("ZeroCalib")) {
      //        sendThis = "dialog.dialMsg.txt=\"\"";
      //        writeString(sendThis);
      //        zeroCalib();
      //        sendThis = F("dialog.dialHead.txt=\"Message\"");
      //        writeString(sendThis);
      //        sendThis = "dialog.dialMsg.txt=\"zero calib done\"";
      //        writeString(sendThis);
      //        delay(2000);
      //
      //        writeString(homepage);
      //      }
    }
  }
}

void initNextion() {
  sendThis = "sys0=" + sys0;
  writeString(sendThis);

  sendThis = "sys1=" + sys1;
  writeString(sendThis);

  sendThis = "sys2=" + sys2;
  writeString(sendThis);

  delay(100);
  writeString(homepage);
}

void displayDialog() {
  sendThis = F("page dialog");
  writeString(sendThis);
  sendThis = F("dialog.dialHead.txt=\"Message\"");
  writeString(sendThis);
  sendThis = F("dialog.dialMsg.txt=\"Updated Successfully\"");
  writeString(sendThis);
  delay(2000);
  writeString(homepage);
}


void displayDialog_sd() {
  sendThis = F("page dialog");
  writeString(sendThis);
  sendThis = F("dialog.dialHead.txt=\"Message\"");
  writeString(sendThis);
  sendThis = F("dialog.dialMsg.txt=\"saved Successfully\"");
  writeString(sendThis);
  delay(2000);
  writeString(homepage);
}

void _displayDialog(String head, String message) {
  sendThis = "dialog.dialHead.txt=\"" + head + "\"";
  writeString(sendThis);
  sendThis = "dialog.dialMsg.txt=\"" + message + "\"";
  writeString(sendThis);
  sendThis = F("page dialog");
  writeString(sendThis);
  delay(4000);
  sendThis = F("page settings");
  writeString(sendThis);
}

void _displayDialog(String head, String message, String page) {
  sendThis = "dialog.dialHead.txt=\"" + head + "\"";
  writeString(sendThis);
  sendThis = "dialog.dialMsg.txt=\"" + message + "\"";
  writeString(sendThis);
  sendThis = F("page dialog");
  writeString(sendThis);
  delay(4000);
  sendThis = "page " + page;
  writeString(sendThis);
}



String getPicString(int stren) {
  String ret = F("btwifi.pic=");
  if (stren < 10) {
    ret += "98";
  } else if ( stren >= 10 & stren < 18) {
    ret += "99";
  } else if ( stren >= 18 & stren < 28) {
    ret += "100";
  } else if ( stren >= 28 ) {
    ret += "101";
  }
  return ret;
}


void displayTime() {
  RtcDateTime now = Rtc.GetDateTime();
  sendThis = "hourVal.val=" + String(now.Hour());
  writeString(sendThis);
  sendThis = "minVal.val=" + String(now.Minute());
  writeString(sendThis);
  sendThis = "dayVal.val=" + String(now.Day());
  writeString(sendThis);
  sendThis = "monVal.val=" + String(now.Month());
  writeString(sendThis);
  sendThis = "yearVal.val=" + String(now.Year() - 2000);
  writeString(sendThis);
}

void sendInfo() {

  String key[4] = {"Product ID", "Serial Number", "Model", "OS Version"};
  String value[4] = {String(device.PID), String(device.SER), String(ModelNo), String(OSVer)};
  //  String key[3] = {"Product ID", "Serial Number", "OS Version"};
  //  String value[3] = {String(device.PID), String(device.SER), String(OSVer)};

  for (uint8_t i = 0; i < 4; i++) {
    sendThis = "systemInfo.key" + String(i) + ".txt=\"" + key[i] + "\"";
    writeString(sendThis);

    sendThis = "systemInfo.value" + String(i) + ".txt=\"" + value[i] + "\"";
    writeString(sendThis);

    Serial.println(sendThis);
  }
}

void modbusInfo() {
  sendThis = "modbusInfo.key0.txt=\"" + String("Slave Id") + "\"";
  writeString(sendThis);
  sendThis = "modbusInfo.value0.txt=\"" + String(slaveID) + "\"";
  writeString(sendThis);

  sendThis = "modbusInfo.key1.txt=\"" + String("Baud Rate") + "\"";
  writeString(sendThis);
  sendThis = "modbusInfo.value1.txt=\"" + String("9600")  + "\"";
  writeString(sendThis);

  sendThis = "modbusInfo.key2.txt=\"" + String("Bits Setting") + "\"";
  writeString(sendThis);
  sendThis = "modbusInfo.value2.txt=\"" + String("8, None, 1") + "\"";
  writeString(sendThis);

  sendThis = "modbusInfo.key3.txt=\"" + String("Read Register addr") + "\"";
  writeString(sendThis);
  sendThis = "modbusInfo.value3.txt=\"Follow DataSheet \"";
  writeString(sendThis);
}


void sendDisplayVal() {
  String page = homepage.substring(5);

  //Update Time
  sendThis = page + ".dateTime.txt=\"" + getTimeStamp2() + "\"";
  writeString(sendThis);

  for (int i = 0; i < no_para; i++) {
    sendThis = page + ".para" + String(i) + ".txt=\"" + para[i] + "\"";
    writeString(sendThis);

    sendThis = page + ".value" + String(i) + ".txt=\"" + para_val_str[i] + "\"";
    writeString(sendThis);

    sendThis = page + ".unit" + String(i) + ".txt=\"" + para_unit[i] + "\"";
    writeString(sendThis);
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
