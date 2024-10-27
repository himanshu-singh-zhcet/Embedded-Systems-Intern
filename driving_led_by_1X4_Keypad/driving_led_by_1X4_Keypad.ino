#include<EEPROM.h>
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
  pinMode(7,OUTPUT);
}

void loop() {
  int long i= EEPROM.read(40);
  analogWrite(7,i);
  while(1){
  if(!digitalRead(ENT)){
    Serial.println("Enter is pressed");
    i=255;
    analogWrite(7,i);
  }
  if(!digitalRead(UP)and i!=0 ){
    Serial.println("UP is pressed");
    if(i>=0 and i<=240){
    i=i+10;
    analogWrite(7,i);
  }
  else{
    analogWrite(7,255);
    return;
  }
  }
  
  if(!digitalRead(DOWN)and i!= 0 ){
    Serial.println("Down is pressed");
    if(i>=15 and i<=255){
    i=i-10;
    analogWrite(7,i);
  }
  else{
    analogWrite(7,0);
    return;
  }
  }
  if(!digitalRead(EXIT)){
    Serial.println("Exit is pressed");
    i=0;
    analogWrite(7,i);
  }
  delay(100);
  EEPROM.write(40, i);
  }
}
