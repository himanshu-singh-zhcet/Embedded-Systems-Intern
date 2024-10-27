#include <DHT22.h>
//define pin no
#define data 2

DHT22 dht22(data); 
 
void setup() {
  Serial.begin(9600); 
}

void loop() {
  
delay(2000);
  float temperature = dht22.getTemperature();
  float humidity = dht22.getHumidity();

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
}
