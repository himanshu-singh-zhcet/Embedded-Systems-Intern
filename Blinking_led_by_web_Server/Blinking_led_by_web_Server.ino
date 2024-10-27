#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Redmi 8292";
const char* password = "11111111";

WebServer server(80);

const int led = 2;

void handleNotFound() {
  server.send(404, "text/plain", "Not Found"); 
}

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




void setup(void) {
  pinMode(led, OUTPUT);
  delay(5000);
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
    server.send(200, "text/html", HTML);
  });
  
  server.on("/L", OFFLED);
  server.on("/H", ONLED);
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

void ONLED() {
  digitalWrite(led, HIGH);
  server.send(200, "text/html", HTML);
}
void OFFLED() {
  digitalWrite(led, LOW);
  server.send(200, "text/html", HTML);
}
