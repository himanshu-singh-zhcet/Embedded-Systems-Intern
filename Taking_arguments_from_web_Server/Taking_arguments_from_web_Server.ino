#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Redmi 8292";
const char* password = "11111111";

WebServer server(80);

const int led = 2;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
} 

String formHtml = "<!DOCTYPE html>\
<html>\
<body>\
<h1>Switch Led</h1>\
<br>\
<form method=\"get\" action=\"http://192.168.137.158/set_wifi\" style=\"text-align: center;\">\
                  <p> Enter your SSID </p>\
                  <input type=\"text\" name=\"ssid\" placeholder=\"Enter ssid here\">\
                  <P> Enter Your Password </P>\
                  <input type=\"password\" name=\"paasword\" placeholder=\"Enter Password here\">\
                  <br>\
                  <br>\
                  <button type=\"submit\"> submit </button>\
                  </form>\
                  </body>\
</html>";


void setup(void) {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", []() {
    server.send(200, "text/html", formHtml);
  }); 
  
  server.on("/set_wifi", setWifi);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}

void setWifi(){
String message;
for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  Serial.println(message);
  server.send(200, "text/html", formHtml);
}
