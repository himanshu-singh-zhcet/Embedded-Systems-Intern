// A Program that scans the all available wifi networks with in the range
#include<WiFi.h>
void setup() {
Serial.begin(115200);
WiFi.mode(WIFI_STA); // SETUP THE WIFI MODE
WiFi.disconnect();   // disconnect the wifi devices if it is already connected 
delay(1000);
}

void loop() {
  Serial.println("Scanning available networks ...");
  int n = WiFi.scanNetworks();   // this function returns the no of available networks 
  Serial.print(n); Serial.println("Networks Found");
  if(n!= 0){
  for(int i =0;i<n;++i){
    Serial.print("network");Serial.print(i+1);Serial.print(":");Serial.print(WiFi.SSID(i));Serial.print(" (");Serial.print(WiFi.RSSI(i));Serial.print(")");
    Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)? "OPEN" : "****");
    delay(50);
  }
  }
  else{
    Serial.println("no networks found");
  }
delay(5000);
}
