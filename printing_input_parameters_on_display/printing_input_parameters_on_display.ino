int size = 4, page = 0;
String  Parameter[] = {"TEMP", "HUMD", "PM10", "PM11"};
String  Values[] = {"37 'C", "60%", "10", "11"};

#include<LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  lcd.begin(16, 2);


}

void loop() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Parameter[page]);
  lcd.setCursor(9, 0);
  lcd.print(Values[page]);
  lcd.setCursor(0, 1);
  lcd.print(Parameter[page + 1]);
  lcd.setCursor(9, 1);
  lcd.print(Values[page + 1]);

  page += 2;
  if (page >= size) {
    page = 0;
  }
  delay(2000);

  // int n = sizeof(Parameter) / sizeof(Parameter[0]);
  // lcd.print()
  // int i = 0;
  // while(i<=size){
  //  lcd.clear();
  //  lcd.setCursor(0,0);
  //  lcd.print(Parameter[i]);
  //  lcd.setCursor(9,0);
  //  lcd.print(Values[i]);
  //  lcd.setCursor(0,1);
  //  lcd.print(Parameter[i+1]);
  //  lcd.setCursor(9,1);
  //  lcd.print(Values[i+1]);
  //  i++;
  //  i++;
  //  delay(2000);
  // }


}
