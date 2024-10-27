#include<LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define ENT 2
#define UP 3
#define DOWN 4
#define EXIT 5
void setup() {
  Serial.begin(9600);
  pinMode(ENT, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(EXIT, INPUT_PULLUP);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); // coloumn , no , row no
  lcd.print("Press enter for");
  lcd.setCursor(6, 1);
  lcd.print("MENU");
}


void in_room(int count){
  lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Room ");
   lcd.print(count);
   int count1 = 0;
   while(1){
   if (!digitalRead(DOWN) and count1 <= 2) {
        count1++;
      }
      if (!digitalRead(DOWN) and count1 == 3) {

        count1 = 1;
      }
      switch (count1) {
        case 1:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Room ");
          lcd.print(count);
          lcd.setCursor(0, 1);
          lcd.print("Tubelight");
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Room ");
          lcd.print(count);
          lcd.setCursor(0, 1);
          lcd.print("Fan");
          break;
      }
   delay(300);
   if (!digitalRead(EXIT)){
    return;
   }
   }
}
void loop() {
  if (!digitalRead(ENT)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Room No");
    int count = 1;
    while (1) {
      if (!digitalRead(DOWN) and count <= 4) {
        count++;
      }
      if (!digitalRead(DOWN) and count == 5) {

        count = 1;
      }
      switch (count) {
        case 1:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Room No");
          lcd.setCursor(0, 1);
          lcd.print(count);
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Room No");
          lcd.setCursor(0, 1);
          lcd.print(count);
          break;

        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Room No");
          lcd.setCursor(0, 1);
          lcd.print(count);
          break;

        case 4:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Room No");
          lcd.setCursor(0, 1);
          lcd.print(count);
          break;
      }
      if(!digitalRead(UP)){
        in_room(count);
      }
      if (!digitalRead(EXIT)) {
        lcd.clear();
        lcd.setCursor(0, 0); // coloumn , no , row no
        lcd.print("Press enter for");
        lcd.setCursor(6, 1);
        lcd.print("MENU");
        return;
      }
      delay(300);
    }
  }
  delay(500);
}
