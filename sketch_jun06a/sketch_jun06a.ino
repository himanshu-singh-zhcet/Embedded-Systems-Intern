int ledPin = 11;  // The pin connected to an LED

void setup() {
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // Fade the LED on
 
 // analogWrite(ledPin, 200);
    if(Serial.available()){
    int input= Serial.readStringUntil('\n').toInt();
    analogWrite(ledPin,input);
  }
    delay(100);                                                     
}
