#include<LiquidCrystal.h> // include the library code
const int rs=13,en=12,d4=11,d5=10,d6=9,d7=8;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
void setup() {
 pinMode(7,OUTPUT);
 digitalWrite(7,HIGH);
 lcd.begin(16,2);
 lcd.setCursor(0,0);// coloumn , no , row no
 lcd.print("hello world");
 delay(1000);
}

void loop() {
  // scrolling 11 position (string length) to the left
  // to move it off scrren left
  for(int i =0;i<11;i++){
    lcd.scrollDisplayLeft();
    delay(150);
  }
  // scrolling 27 positions (string length+display length) to the right 
  // to move it off screen right
  for(int i =0;i<27;i++){
    lcd.scrollDisplayRight();
    delay(150);  
  }
  // scroll 16 positions (display length) to the k=left
  // to move it back to center 
   for(int i =0;i<16;i++){
    lcd.scrollDisplayLeft();
    delay(150);
  }
}
