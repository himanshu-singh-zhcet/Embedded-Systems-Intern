#define led 2
void setup() {
  // put your setup code here, to run once:
  
Serial.begin(9600);
Serial2.begin(9600);
pinMode(led,OUTPUT);
}

void loop() {
  if(Serial2.available()){
    String s =Serial2.readStringUntil('\n');
    Serial.println(s);
    String s1 = "on";
    String s2 = "off";
    if(s == s1){
      digitalWrite(led, HIGH);
    }
    else if(s == s2){
      digitalWrite(led, HIGH);
    }
    
  }

}
