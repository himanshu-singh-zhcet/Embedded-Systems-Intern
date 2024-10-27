bool EEPROMWrite(String val, int index) {
  int len = val.length();
  EEPROM.write(index, len);
  if (len > 20) {
    return false;
  } else {
    for (int i = 0; i < len; i++) {
      EEPROM.write(index + i + 1, val[i]);
    }
  }
}

String EEPROMRead(int address) {
  int len = EEPROM.read(address);
  if (len > 20) {
    return "NaN";
  } else {
    String str = "";
    for (int i = 0; i < len; i++) {
      str += (char)EEPROM.read(address + i + 1);
    }
    return str;
  }
}
