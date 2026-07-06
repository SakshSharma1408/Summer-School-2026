 # ESP32 REST API Weather Data Fetcher

## Problem Statement

Develop an ESP32-based IoT application that fetches real time weather data from the OpenWeatherMap REST API and compares it with local environmental readings obtained from a DHT11 sensor. The system displays the city name, weather description, temperature, humidity, and the difference between API data and local sensor readings through the Serial Monitor.

---

## Objective

- Connect ESP32 to a Wi-Fi network.
- Fetch live weather data using the OpenWeatherMap REST API.
- Read local temperature and humidity using a DHT11 sensor.
- Compare API readings with local sensor values.
- Display the results on the Serial Monitor.

---

## Hardware Required

- ESP32 Dev Module
- DHT11 Temperature & Humidity Sensor (used DHT22 for simulation)
- Jumper Wires
- Breadboard (optional)

---

## Circuit Connections

| DHT11 Pin | ESP32 Pin |
|------------|-----------|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO 33 |

---

## Libraries Used

- WiFi.h
- HTTPClient.h
- ArduinoJson
- DHT Sensor Library (Adafruit)
- Adafruit Unified Sensor

---

## Setup

1. Install the required libraries.
2. Create a `config.h` file containing:
   - Wi-Fi SSID
   - Wi-Fi Password
   - OpenWeatherMap API Key
   - City Name
3. Upload the code to the ESP32.
4. Open the Serial Monitor at **115200 baud**.

---

## Sample Output

```text
City: Jammu

Weather: Broken Clouds

API Temperature: 40.65 °C
Local Temperature: 24.00 °C
Temperature Difference: -16.65 °C

API Humidity: 22 %
Local Humidity: 59 %
Humidity Difference: 37 %
```

---

## Project Structure

```
WeatherFetcher/
│
├── WeatherFetcher.ino
├── config.example.h
├── README.md
├── .gitignore
└── screenshot.png
```

---

## Security

The actual `config.h` file is excluded from version control using `.gitignore` to prevent exposing Wi-Fi credentials and the OpenWeatherMap API key.

---

## Features

- Live weather retrieval using REST API
- Local temperature and humidity monitoring
- API vs local sensor comparison
- Automatic Wi-Fi connection
- JSON parsing using ArduinoJson
- Secure configuration management