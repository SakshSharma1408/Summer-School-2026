#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "config.h"

#define DHTPIN 33
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 10000;   // Fetch every 10 seconds

void connectWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

void setup()
{
  Serial.begin(115200);
  dht.begin();

  connectWiFi();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWiFi();
  }

  if (millis() - previousMillis >= interval)
  {
    previousMillis = millis();

    float localTemp = dht.readTemperature();
    float localHumidity = dht.readHumidity();

    if (isnan(localTemp) || isnan(localHumidity))
    {
      Serial.println("Failed to read DHT11!");
      return;
    }

    HTTPClient http;

    String url =
    "http://api.openweathermap.org/data/2.5/weather?q=Jammu&appid=" +
    String(API_KEY) +
    "&units=metric";
    http.begin(url);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();

      DynamicJsonDocument doc(2048);

      DeserializationError error = deserializeJson(doc, payload);

      if (!error)
      {
        String city = doc["name"];
        float apiTemp = doc["main"]["temp"];
        int apiHumidity = doc["main"]["humidity"];
        String description = doc["weather"][0]["description"];

        Serial.println("----------------------------------------");

        Serial.print("City: ");
        Serial.println(city);

        Serial.print("Weather: ");
        Serial.println(description);

        Serial.println();

        Serial.print("API Temperature: ");
        Serial.print(apiTemp);
        Serial.println(" °C");

        Serial.print("Local DHT11 Temperature: ");
        Serial.print(localTemp);
        Serial.println(" °C");

        Serial.print("Temperature Difference: ");
        Serial.print(localTemp - apiTemp);
        Serial.println(" °C");

        Serial.println();

        Serial.print("API Humidity: ");
        Serial.print(apiHumidity);
        Serial.println(" %");

        Serial.print("Local DHT11 Humidity: ");
        Serial.print(localHumidity);
        Serial.println(" %");

        Serial.print("Humidity Difference: ");
        Serial.print(localHumidity - apiHumidity);
        Serial.println(" %");

        Serial.println("----------------------------------------");
      }
      else
      {
        Serial.println("JSON Parsing Failed!");
      }
    }
    else
    {
      Serial.print("HTTP Error: ");
      Serial.println(httpCode);
    }

    http.end();
  }
}