#include<WiFi.h>
#include<WebServer.h>

#define ssid "ESP32_SAP"
#define password "12345678"

#define led 2

bool LEDStatus =LOW;

IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>Switch Led</h1>\
<br>\
<a href=\"L\">\
    <button> led off </button>\
</a>\
<br>\
<a href=\"H\">\
    <button> led on </button>\
</a>\
</body>\
</html>";

void setup(){
    Serial.begin(115200);
    pinMode(led ,OUTPUT);
    WiFi.softAP(ssid,password);
    WiFi.softAPConfig(local_ip,gateway,subnet);
    delay(1000);
    server.on("/",handle_onconnect);   
    server.on("/H",ledon);
    server.on("/L",ledoff);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("HTTP server started");   
}
void loop(){
  server.handleClient();
}
void handle_onconnect(){
  LEDStatus = LOW;
  Serial.println("LED Status: OFF"); 
  server.send(200,"text/html",HTML);
}
void ledon() {
  digitalWrite(led, HIGH);
  server.send(200, "text/html", HTML);
}
void ledoff() {
  digitalWrite(led, LOW);
  server.send(200, "text/html", HTML);
}
void handle_NotFound(){
  server.send(404,"text/plain","Not Found");
}
