#include<String.h>;
#include<LiquidCrystal.h> // include the library code
const int rs=13,en=12,d4=11,d5=10,d6=9,d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
  analogWrite(3,220);
  lcd.begin(16,2);
  lcd.setCursor(0,0);

}

void loop() {
  if(Serial.available()){
    String input= Serial.readString();
    lcd.print(input);
    
  }
  // put your main code here, to run repeatedly:

}
