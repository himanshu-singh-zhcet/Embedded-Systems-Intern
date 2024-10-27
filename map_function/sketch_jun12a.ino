void setup() {
  Serial.begin(9600);
  pinMode(7,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("enter the voltage value");
  if(Serial.available()){
    int input= Serial.readStringUntil('\n').toInt();
    int i = map(input,0,5,0,255);
    analogWrite(7,i);
    Serial.println(i);
  }
  
}
