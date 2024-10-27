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
        //        if (!startUp)
        //          sendDisplayVal();
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
      } else if (response.startsWith("Time")) {
        response.remove(0, response.indexOf(":") + 1);
        updateTime(response, "hh:mm");
        sendDisplayVal();
      } else if (response == "p_systemInfo") {
        sendInfo();
      }
      //      else if (response.startsWith("calibrate")) {
      //        _displayDialog("Message", "No need for Calibration");
      //      }
      else if (response.startsWith("calib")) {
        response.remove(0, response.indexOf(':') + 1);
        String paraName = response.substring(0, response.indexOf('#'));
        paraName.toUpperCase();
        response.remove(0, response.indexOf('#') + 1);
        float kfac = response.toFloat();
        int index = 0;
        Serial.println("paraName: " + String(paraName));
        Serial.println("kfac    : " + String(kfac));
        for (int i = 0; i < no_para; i++) {
          if (para[i] == paraName) {
            index = i;
            break;
          }
        }

        para_fac[index] = kfac;
        Serial.println("para[index]     : " + String(para[index]));
        Serial.println("para_fac[index] : " + String(para_fac[index]));

        EEPROM.write(eeprom_addr[index], para_fac[index]);
        _displayDialog("Message", "Factor Updated", "pointCalib");
      }
      else if (response.startsWith("logInterval")) {
        response.remove(0, response.indexOf(':') + 1);
        response.substring(0, response.lastIndexOf(':'));
        logTime  = response.toInt();
        Serial.println(logTime);
        EEPROM.write(E_LOGINTERVAL, logTime);
        _displayDialog("Message", "Log Time Updated", "settings");
      }
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

  delay(1000);
  sendDisplayVal();
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
  delay(2000);
  sendThis = F("p_menu");
  writeString(sendThis);
}

void _displayDialog(String head, String message, String page) {
  sendThis = "dialog.dialHead.txt=\"" + head + "\"";
  writeString(sendThis);
  sendThis = "dialog.dialMsg.txt=\"" + message + "\"";
  writeString(sendThis);
  sendThis = F("page dialog");
  writeString(sendThis);
  delay(2000);
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

  for (uint8_t i = 0; i < 4; i++) {
    sendThis = "systemInfo.key" + String(i) + ".txt=\"" + key[i] + "\"";
    writeString(sendThis);

    sendThis = "systemInfo.value" + String(i) + ".txt=\"" + value[i] + "\"";
    writeString(sendThis);

    Serial.println(sendThis);
  }
}

void sendDisplayVal() {
  String page = homepage.substring(5, homepage.length());

  //Update Time
  sendThis = page + ".dateTime.txt=\"" + getDateTime2() + "\"";
  writeString(sendThis);

  for (int i = 0; i < no_para; i++) {
    sendThis = page + ".para" + String(i) + ".txt=\"" + para[i] + "\"";
    writeString(sendThis);

    sendThis = page + ".value" + String(i) + ".txt=\"" + para_val_str[i] + "\"";
    writeString(sendThis);

    sendThis = page + ".unit" + String(i) + ".txt=\"" + para_unit[i] + "\"";
    writeString(sendThis);
  }
  //  if (homePageFlag)
  //    writeString(homepage);
}


void writeString(String stringData) {
  for (int i = 0; i < stringData.length(); i++)
    displaySerial.write(stringData[i]);

  displaySerial.write(0xff);
  displaySerial.write(0xff);
  displaySerial.write(0xff);
  displaySerial.flush();
}
