unsigned long previoustime = 0;
unsigned long currenttime =0;
unsigned long elapsedtime =0;
int interval=100;

boolean ledstate1 =LOW;
void setup() {
  // put your setup code here, to run once:
    pinMode(11,OUTPUT);
}
void loop() {
currenttime=millis();
elapsedtime=currenttime-previoustime; 
if(elapsedtime>= interval){
  ledstate1 = !ledstate1;
 
  digitalWrite(11,ledstate1);
  previoustime = currenttime;
}



}
