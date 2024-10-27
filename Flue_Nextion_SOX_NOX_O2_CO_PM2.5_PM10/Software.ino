void handleSoftwareConnect() {
  if (SerialConnect.available()) {
    String s = SerialConnect.readStringUntil('\n');
    s.trim();

    if (s.startsWith("help")) {
      Serial.println(F("1. Log Time    :[logT:<value>] , value(1,30) in minutes"));
      Serial.println(F("2. Update Clock:[time:<time string>] , [21:02:16:09:43 -> YY:MM:DD:hh:mm]"));
      Serial.println(F("3. Slave ID    :[slave:<ID>]"));
    }
    else if (s.startsWith("config")) {
      RtcDateTime dt = Rtc.GetDateTime();
      Serial.print(F("PID        : ")); Serial.println(device.PID);
      Serial.print(F("SER        : ")); Serial.println(device.SER);
      Serial.print(F("Device Time: ")); Serial.println(getDateTime2());
      Serial.print(F("Log Time   : ")); Serial.println(logTime);
      Serial.print(F("Sensor info: ")); Serial.println(sensor_info);
      Serial.print(F("SOX Factor : ")); Serial.println(para_fac[SOX]);
      Serial.print(F("NOX Factor : ")); Serial.println(para_fac[NOX]);
      Serial.print(F("CO Factor  : ")); Serial.println(para_fac[CO]);
//      Serial.print(F("O2 Factor  : ")); Serial.println(para_fac[O2]);
//      Serial.print(F("CO2 Factor : ")); Serial.println(para_fac[CO2]);
    }
    else if (s.startsWith("logT")) {
      s.remove(0, s.indexOf(':') + 1);
      logTime  = s.toInt();
      EEPROM.write(E_LOGINTERVAL, logTime);
      Serial.print(F("Log Time Updated: ")); Serial.println(EEPROM.read(E_LOGINTERVAL));
    }
    //Update Time   [time:<time string>]
    else if (s.startsWith("time")) {
      s.remove(0, s.indexOf(":") + 1);
      String YY, MM, DD, hh, mm, arr[5];
      for (uint8_t i = 0; i < 5; i++) {
        arr[i] = s.substring(0, s.indexOf(":"));
        s.remove(0, s.indexOf(":") + 1);
      }

      YY = arr[0];      MM = arr[1];      DD = arr[2];
      hh = arr[3];      mm = arr[4];

      updateTime(YY, MM, DD, hh, mm);
      Serial.println(F("RTC Time updated"));
      Serial.print(F("Time: "));
      Serial.println(getDateTimeNow());
    }
    else if (s.startsWith("debug")) {
      debug = !debug;
      debug ? Serial.println(F("Debug Mode: ON")) : Serial.println(F("Debug Mode: OFF"));
    }
    else if (s.startsWith("restart")) {
      Serial.println("System Restarting...");
      delay(500);
      resetFunc();
    }
    else if (s.startsWith("reset_id")) {
      Serial.print(F("Reseting Product ID ... "));
      EEPROM.write(E_CONFIG, 255);
      Serial.println(F("Done"));
      resetFunc();
    }
    else if (s.indexOf("##$$&&") > -1) {
      Serial.print(F("Reseting Product ID ... "));
      EEPROM.write(E_CONFIG, 255);
      Serial.println(F("Done"));
      resetFunc();
    }
    else {

      int choice = s.substring(s.indexOf('$') + 1, s.indexOf(':')).toInt();
      s.remove(0, s.indexOf(':') + 1);
      switch (choice)
      {
        case 99: {
            //printlogFile();
            break;
          }
        case 98: {
            //DeletelogFile();
            break;
          }
        case 101: {
            softFlag = true;
            if (startUp)
              sendDisplayVal();
            //          lcd.clear();
            //          lcd.print("soft. connected");
            SerialConnect.println("$:$");
            if (!digitalRead(pumpPin))
              digitalWrite(pumpPin, HIGH);  // OFF PUMP

            break;
          }

        case 103: {
            SdInit(ssPin);

            //          if (!sd1.begin(sdPin, SD_SCK_MHZ(50))) {
            //            SerialConnect.println("@100:#!:SD card is Missing or SPI problem");
            //          }

            if (!dirFile.open("/", O_READ)) {
              //  sd1.errorHalt("open root failed");
            }

            while (file.openNext(&dirFile, O_READ)) {
              // Skip directories and hidden files.
              if (!file.isSubDir() && !file.isHidden()) {
                // Save dirIndex of file in directory.
                dirIndex[n] = file.dirIndex();

                SerialConnect.write("@103:");
                file.printName(&SerialConnect);
                SerialConnect.println();
                delay(120);
              }
              file.close();
            }
            lt = millis();          //in case of long file copy
            dirFile.close();
            break;
          }

        case 104: {
            SdInit(ssPin);

            fileIndex = 0;
            int c;
            int count = s.toInt();

            s.trim();
            for (int i = 0; i < count; i++)
            {
              SerialConnect.println("@99:filename:" + SelectedFiles[fileIndex]);
              String fileName = SelectedFiles[fileIndex];
              fileName.trim();
              readFile(fileName);
              char last = 0;

              if (last != '\n') {
                //              SerialConnect.println();
              }
              file.close();
              fileIndex++;
            }
            SerialConnect.println("@101:terminate");
            for (int i = 0; i < count; i++) {
              SelectedFiles[i] = "";
              fileIndex = 0;
            }
            break;
          }

        //Saving the file names in array: SelectedFiles[]
        case 105: {
            SelectedFiles[fileIndex] = s;
            fileIndex++;
            break;
          }

        //Setting dateTime from Serial Software Module
        case 106: {
            if (s.indexOf("dt") > -1)
            {
              s.remove(0, s.lastIndexOf(':') + 1);
              int a[6];
              SerialConnect.println("@106:" + s);
              for (int i = 0; i < 6; i++)
              {
                a[i] = s.substring(0, s.indexOf(',')).toInt();
                s.remove(0, s.indexOf(',') + 1);
              }

              RtcDateTime compiled = RtcDateTime(a[0], a[1], a[2], a[3], a[4], a[5]);
              Rtc.SetDateTime(compiled);
            }
            else if (s.indexOf("log") > -1)
            {
              s.remove(0, s.lastIndexOf(':') + 1);
              //Serial.println(s);
              logTime = s.toInt();
              SerialConnect.println("@108:li:" + String(logTime));
              EEPROM.write(E_LOGINTERVAL, logTime);
            }
            break;
          }

        //Get RTC Time for Software Module
        case 107:
          {
            //          para_val[CO] = getCo();
            //          SerialConnect.println("@107:0:CO:" + String(CO) + ":PPM");
            for (int i = 0; i < no_para; i++) {
              Serial.println("@107:" + String(i) + ":" + para[i] + ":" + para_val_str[i] + ":" + para_unit[i]);
              delay(50);
            }
            SerialConnect.println("@107:dt:" + getTimeNow());
            break;
          }

        //Get Product Info for Software Module
        case 108:
          {
            RtcDateTime now = Rtc.GetDateTime();
            SerialConnect.println("@108:pro:" + String(device.PID));
            delay(50);
            SerialConnect.println("@108:ser:" + String(device.SER));
            delay(50);
            SerialConnect.println("@108:dt:" + getDateTime(now));
            delay(50);
            SerialConnect.println("@108:li:" + String(logTime));
            break;
          }

        //Remove the file from SD(Internal Memory)
        case 109: {
            SdInit(ssPin);
            String filename = s;
            filename.toLowerCase();
            filename.trim();
            if (s.indexOf("@") > -1) {
              if (!dirFile.open("/", O_READ)) {
                //  sd1.errorHalt("open root failed");
              }

              while (file.openNext(&dirFile, O_READ)) {
                // Skip directories and hidden files.
                if (!file.isSubDir() && !file.isHidden()) {
                  // Save dirIndex of file in directory.
                  dirIndex[n] = file.dirIndex();

                  SerialConnect.write("@103:");
                  file.printName(&SerialConnect);
                  SerialConnect.println();
                  delay(120);
                }
                file.close();
              }
              dirFile.close();
            } else deleteFile(filename);
            powerDownSD();
            break;
          }
      }
      lt = millis();
    }
  }
}
