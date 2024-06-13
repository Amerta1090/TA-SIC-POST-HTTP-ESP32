#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// configurasi wifi
const char* ssid = "istrikecil";
const char* password = "microwife";

// Server endpoint
const char* serverURL = "http://192.168.191.166:5000/post-data";

// konfigurasi sensor suhu
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// konfigurasi sensor gas
#define MQ135_PIN 13

void setup() {
  Serial.begin(115200);
  
  // connect ke wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi network");

  dht.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // membaca sensor data suhu dan sensor gas
    float temp = dht.readTemperature();
    float humid = dht.readHumidity();
    int airQual = analogRead(MQ135_PIN);
 
    if (isnan(temp) || isnan(humid)) {
      Serial.println("Error reading from DHT sensor");
      return;
    }

    String jsonPayload = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(humid) + ",\"airQuality\":" + String(airQual) + "}";

    // mengirimkan HTTP POST request
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("POST request error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Wi-Fi disconnected");
  }
  //delay lama data sensor dikirimkan
  delay(10000);
}
