int analogPin = A1;  

void setup() {
  
  Serial.begin(9600);  
}

void loop() {
  // Read the analog voltage from the analog pin
  int value1 =0;
  for(int i=0;i<10;i++){
  int value = analogRead(analogPin);
  value1 =value1+value;
  }
  // Print the value to the serial monitor
  value1=value1/10;
  Serial.println(value1);

  delay(100);  
}
