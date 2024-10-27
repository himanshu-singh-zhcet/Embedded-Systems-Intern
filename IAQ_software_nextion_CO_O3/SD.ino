#define countof(a) (sizeof(a) / sizeof(a[0]))

bool saveLog(String data) {
  bool ret = true;
  File file = sd.open("log.txt", FILE_WRITE);
  if (file) {
    file.print(data);
  } else {
    ret = false;
  }
  file.close();
  return ret;
}

void printlogFile() {
  Serial.println("------LOG FILE------");
  File file = sd.open("log.txt");
  if (file) {
    while (file.available()) {
      char c = file.read();
      Serial.print(c);
    }

  }
  file.close();
  Serial.println("------- END -------");
}

void DeletelogFile() {
  deleteFile("log.txt");
}

bool saveToSD() {
  bool write_header = true;
  bool ret = false;

  SdInit(ssPin);

  RtcDateTime now = Rtc.GetDateTime();
  String dateTime = getDateTime(now);
  String fileName = "data.csv";
  File file1 = sd.open(fileName.c_str());


  if (file1) write_header = false;
  file1.close();

  File file = sd.open(fileName.c_str(), FILE_WRITE);
  if (file) {
    Serial.println("Data saved");
    if (write_header) {
      file.print(F("Date,Time,"));
      for (int i = 0; i < no_para; i++) {
        file.print(para[i]);
        file.print("(");
        file.print(para_unit[i]);
        file.print(")");

        if (i == no_para - 1) {
          file.println();
        } else {
          file.print(",");
        }
      }
    }

    file.print(getDateTime(now) + ",");
    for (int i = 0; i < no_para; i++) {
      file.print(para_val_str[i]);

      if (i == no_para - 1) {
        file.println();
      } else {
        file.print(",");
      }
    }
    
    ret = true;
  } 
  else ret = false;
  
  file.close();
  powerDownSD();
  return ret;
}

int scanFiles() {
  SdInit(ssPin);
  SdFile dirFile;
  SdFile file;
  uint16_t n = 0;

  if (!dirFile.open("/", O_READ)) {
    Serial.println(F("open root failed"));
    return -1;
  }
  while (file.openNext(&dirFile, O_READ)) {

    // Skip directories and hidden files.
    if (!file.isSubDir() && !file.isHidden()) {

      // Save dirIndex of file in directory.
      dirIndex[n++] = file.dirIndex();

      // Print the file number and name.
      // Serial.print(n++);
      //Serial.write(' ');
      //Serial.print(file.printFileName());
      //Serial.println();
    }
    file.close();
  }
  dirFile.close();
  return n;
}

//String getFileName(int k) {
//  k--;
//  SdFile dirFile;
//  SdFile file;
//  String Name;
//  if (!dirFile.open("/", O_READ)) {
//    Serial.println(F("open root failed"));
//    return F("NAN");
//  }
//
//  if (!file.open(&dirFile, dirIndex[k], O_READ)) {
//    return F("NAN");
//  }
//  Name = file.printFileName();
//  file.close();
//  dirFile.close();
//  return formatFileName(Name);
//}


String formatFileName(String f) {
  int l = f.length();
  if (l < 11) {
    for (int i = l; i < 12; i++) {
      f += " ";
    }
  }
  return f;
}

void SdInit(int cs) {
  pinMode (sdPower, OUTPUT);
  digitalWrite (sdPower, HIGH);
  delay(200);
  Serial.print(F("Initializing SD card..."));

  if (!sd.begin(cs)) {
    Serial.println(F("initialization failed!"));

  } else {
    Serial.println(F("initialization done."));
  }
}

void powerDownSD() {
  //  digitalWrite (sdPower, LOW);
  //  delay (50);
  //  pinMode (sdPower, INPUT);
}

String setFileName() {
  String pId = String(device.PID);
  pId = pId.substring(5, 8);
  //Serial.print("hhhhhh");
  //Serial.println(pId);
  char s[4];
  pId.toCharArray(s, 4);
  RtcDateTime now = Rtc.GetDateTime();
  char thisString[13];

  snprintf_P(thisString,
             countof(thisString),
             PSTR("%02u_%s.CSV"),
             now.Year() - 2000,
             s);
  return thisString;
}

bool fileDlete() {
  bool ret;
  SdInit(ssPin);

  File file = sd.open(setFileName(), FILE_WRITE);
  if (file) {
    file.remove();
    ret = true;
  } else ret = false;
  file.close();


  powerDownSD();
  return ret;
}



void readFile2() {
  SdInit(ssPin);

  File file = sd.open(setFileName());
  if (file) {
    while (file.available()) {
      char c = file.read();
      Serial.print(c);
    }
  } else Serial.println(F("unable to read file"));
  file.close();
  powerDownSD();
}

void readFile(String fileName) {
  File file = sd.open(fileName);
  if (file) {
    while (file.available()) {
      char c = file.read();
      SerialConnect.print(c);
    }
    SerialConnect.println();
    SerialConnect.println(F("@101:?+end"));
  } else {
    SerialConnect.println(String(F("unable to open file: ")) + fileName);
  }
  file.close();
}

void deleteFile(String filename) {
  File file = sd.open(filename.c_str(), O_RDWR | O_CREAT | O_AT_END);
  if (!file.remove()) {
    Serial.println("File Not Removed");
  } else {
    Serial.print(filename); Serial.println(" removed successfully");
  }
  file.close();
}
