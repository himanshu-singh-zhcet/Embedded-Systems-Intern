#include<LiquidCrystal.h> // include the library code
const int rs=13,en=12,d4=11,d5=10,d6=9,d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

byte heart[8] = {B00000, B01010, B11111, B11111, B01110, B00100, B00000, B00000};
byte smiley[8] = {B00000, B00000, B01010, B00000, B00000, B10001, B01110, B00000};
byte armsdown[8] = {B00100, B01010, B00100, B00100, B01110, B10101, B00100, B01010};
byte armsup[8] = {B00100, B01010, B00100, B10101, B01110, B00100, B00100, B01010};

void setup() {
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
  analogWrite(3,220);
  lcd.begin(16,2);
  lcd.createChar(0,heart);
  lcd.createChar(1,smiley);
  lcd.createChar(2,armsdown);
  lcd.createChar(3,armsup);
  lcd.setCursor(2,0);
  lcd.write(byte(0));
  lcd.setCursor(4,0);
  lcd.write(byte(1));
  lcd.setCursor(6,0);
  lcd.write(byte(2));
  lcd.setCursor(8,0);
  lcd.write(byte(3));  
  lcd.setCursor(1, 1);
  lcd.print("Himanshu Singh");
}

void loop(){
  
}
