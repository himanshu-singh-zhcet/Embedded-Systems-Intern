#include<LiquidCrystal.h>
const int rs=13,en=12,d4=11,d5=10,d6=9,d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
void setup() {
 pinMode(7,OUTPUT);
 digitalWrite(7,HIGH);
 lcd.begin(16,2);
 lcd.setCursor(0,0);// coloumn , no , row no
 lcd.print("hello world");
}

void loop() {
}
