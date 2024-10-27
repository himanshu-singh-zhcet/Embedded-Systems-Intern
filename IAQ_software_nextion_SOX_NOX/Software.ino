//void handleSoftwareConnect() {
//  if (SerialConnect.available()) {
//    String s = SerialConnect.readStringUntil('\n');
//    lt = millis();
//    s.trim();
//
//
//  }
//}

void handleSoftwareConnect(String s) {
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
        //          softFlag = true;
        //          lcd.clear();
        //          lcd.print("soft. connected");
        SerialConnect.println("$:$");
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

        //          else if (s.indexOf("log") > -1)
        //          {
        //            s.remove(0, s.lastIndexOf(':') + 1);
        //            //Serial.println(s);
        //            PumpTime = s.toInt();
        //            //Serial.print("LI:"); Serial.println(LI);
        //            SerialConnect.println("@108:li:" + String(PumpTime));
        //            EEPROM.write(E_SAMPLEINTERVAL, PumpTime);
        //          }

        break;
      }

    //Get RTC Time for Software Module
    case 107:
      {

        getSensorValue();

        //          para_val[CO] = getCo();
        //          SerialConnect.println("@107:0:CO:" + String(CO) + ":PPM");
        for (int i = 0; i < no_para; i++) {
          SerialConnect.println("@107:" + String(i) + ":" + para[i] + ":" + para_val_str[i] + ":" + para_unit[i]);
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
        SerialConnect.println("@108:li:" + String(PumpTime));
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
