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
}

void loop() {
  if(!digitalRead(ENT)){
    Serial.println("Enter is pressed");
  }
  if(!digitalRead(UP)){
    Serial.println("UP is pressed");
  }
  if(!digitalRead(DOWN)){
    Serial.println("Down is pressed");
  }
  if(!digitalRead(EXIT)){
    Serial.println("Exit is pressed");
  }
  delay(100);
}
