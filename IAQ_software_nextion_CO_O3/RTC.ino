#define countof(a) (sizeof(a) / sizeof(a[0]))

String getTime() {
  RtcDateTime now = Rtc.GetDateTime();
  char datestring[6];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u:%02u"),
             now.Hour(),
             now.Minute() );
  return datestring;
}

String getDate()
{
  RtcDateTime dt = Rtc.GetDateTime();
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year() - 2000
            );
  return datestring;
}

int getMinute() {
  RtcDateTime dt = Rtc.GetDateTime();
  return dt.Minute();
}




String getDateTimeNow()
{
  RtcDateTime dt = Rtc.GetDateTime();
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%04u-%02u-%02u %02u:%02u:%02u"),
             dt.Year(),
             dt.Month(),
             dt.Day(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  return datestring;
}

String getDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u,%02u:%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  return datestring;
}


String getTimeNow()
{
  RtcDateTime dt = Rtc.GetDateTime();
  char datestring[9];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u:%02u:%02u"),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  return datestring;
}


String getTimeStamp()
{
  RtcDateTime dt = Rtc.GetDateTime();
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u-%02u-%02u %02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year() - 2000,
             dt.Hour(),
             dt.Minute());
  return datestring;
}

String getTimeStamp2()
{
  RtcDateTime dt = Rtc.GetDateTime();
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u-%02u-%04u %02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute());
  return datestring;
}



void updateTime(String st, String format) {
  int serTimeArray[6];
  RtcDateTime sevTime;
  //hh:mm:DD:MM:YY:
  if (st.indexOf("-") > -1) {
    //DD:MM:YY hh:mm:ss&

    for (int i = 0; i < 6; i++) {
      String s = st.substring(0, 2);
      st.remove(0, 3);
      serTimeArray[i] = s.toInt();
    }
    sevTime = RtcDateTime(serTimeArray[2] + 2000, serTimeArray[1], serTimeArray[0], serTimeArray[3], serTimeArray[4], serTimeArray[5]);
  } else {
    for (int i = 0; i < 5; i++) {
      String s = st.substring(0, st.indexOf(":"));
      st.remove(0, st.indexOf(":") + 1);
      serTimeArray[i] = s.toInt();
    }
    if (format == "hh:mm")
      sevTime = RtcDateTime(serTimeArray[4] + 2000, serTimeArray[3], serTimeArray[2], serTimeArray[0], serTimeArray[1], 0);
    else if ( format == "mm:hh")
      sevTime = RtcDateTime(serTimeArray[4] + 2000, serTimeArray[3], serTimeArray[2], serTimeArray[1], serTimeArray[0], 0);
  }
  RtcDateTime now = Rtc.GetDateTime();
  //yy,MM,DD,hh,mm,ss

  Serial.println(sevTime);
  Serial.println(now);
  Serial.println((long int)now - (long int)sevTime);
  if (abs((long int)now - (long int)sevTime) > 55) {
    Serial.println("updating time...");
    Rtc.SetDateTime(sevTime);
  }
}


void updateTime(String st) {
  st.trim();
  Serial.println(st);
  unsigned long t = st.toInt();
  Serial.println(t);
  RtcDateTime sevTime = t + (5 * 60 * 60 + 30 * 60) - 946684800;
  RtcDateTime now = Rtc.GetDateTime();

  Serial.print("Server Time: "); Serial.println(sevTime);
  Serial.print("RTC    Time: "); Serial.println((unsigned long)now);
  if (abs(sevTime - now) > 30) {
    Serial.println("updating time...");
    //Rtc.SetDateTime(sevTime);
  }
}
