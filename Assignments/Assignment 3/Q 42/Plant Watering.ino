#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi Credentials

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// OpenWeather API

const char* API_KEY = "YOUR API KEY";
const char* CITY = "Jammu";

// Google Apps Script URL

const char* SCRIPT_URL =
"YOUR SCRIPT URL";

// Pins

#define SOIL_PIN      34
#define LDR_PIN        35

#define BUTTON_PIN     18

#define PUMP_PIN       19


unsigned long previousRead = 0;

const unsigned long READ_INTERVAL = 30000;

unsigned long lastWatered = 0;

unsigned long manualOverrideTime = 0;

const unsigned long LOCKOUT = 300000;

bool rainAPI = false;

bool rainLDR = false;

bool pumpState = false;

String moistureState = "";

int moistureRaw = 0;

int moisturePercent = 0;

int lightRaw = 0;

// WiFi

void connectWiFi()
{

  Serial.print("Connecting");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  Serial.println("WiFi Connected");

}

// Weather API

bool checkWeather()
{

  HTTPClient http;

  String url =
  "http://api.openweathermap.org/data/2.5/weather?q=Jammu&appid=" +
  String(CITY) +
  "&appid=" +
  String(API_KEY);

  http.begin(url);

  int code = http.GET();

  if(code == 200)
  {

    DynamicJsonDocument doc(2048);

    deserializeJson(doc, http.getString());

    String weather =
    doc["weather"][0]["main"];

    http.end();

    if(weather == "Rain")
      return true;

    if(weather == "Drizzle")
      return true;

    if(weather == "Thunderstorm")
      return true;

    return false;

  }

  http.end();

  return false;

}

// Read Soil Moisture

void readSoil()
{

  moistureRaw = analogRead(SOIL_PIN);

  moisturePercent = map(moistureRaw, 0, 4095, 0, 100);

  if(moisturePercent < 30)
    moistureState = "DRY";

  else if(moisturePercent < 70)
    moistureState = "OPTIMAL";

  else
    moistureState = "WET";

}

// Read LDR

void readLDR()
{

  lightRaw = analogRead(LDR_PIN);

  if(lightRaw < 1500)
    rainLDR = true;

  else
    rainLDR = false;

}

// Water Plant

void waterPlant()
{

  pumpState = true;

  digitalWrite(PUMP_PIN, HIGH);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Pump Running");

  lcd.setCursor(0,1);
  lcd.print("Watering...");

  delay(5000);

  digitalWrite(PUMP_PIN, LOW);

  pumpState = false;

  lastWatered = millis();

}

// LCD Display

void updateLCD()
{

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("M:");

  lcd.print(moisturePercent);

  lcd.print("% ");

  lcd.print(moistureState);

  lcd.setCursor(0,1);

  lcd.print("Last:");

  lcd.print(lastWatered/1000);

  lcd.print("s");

}

// Serial Output

void printStatus()
{

  Serial.println("-----------------------");

  Serial.print("Moisture : ");
  Serial.print(moisturePercent);
  Serial.println("%");

  Serial.print("State : ");
  Serial.println(moistureState);

  Serial.print("LDR Rain : ");

  if(rainLDR)
    Serial.println("YES");

  else
    Serial.println("NO");

  Serial.print("Weather API Rain : ");

  if(rainAPI)
    Serial.println("YES");

  else
    Serial.println("NO");

  Serial.print("Last Watered : ");

  Serial.print(lastWatered/1000);

  Serial.println(" sec");

}

// Google Sheets Logging

void logGoogleSheets()
{

  if(WiFi.status() != WL_CONNECTED)
    return;

  HTTPClient http;

  String url =

  String(SCRIPT_URL) +

  "?moisture=" +

  String(moisturePercent) +

  "&status=" +

  moistureState +

  "&rain=" +

  String(rainAPI);

  http.begin(url);

  http.GET();

  http.end();

}

// Manual Override

void manualOverride()
{

  if(digitalRead(BUTTON_PIN) == LOW)
  {

    if(millis() - manualOverrideTime >= LOCKOUT)
    {

      manualOverrideTime = millis();

      Serial.println("Manual Override Activated");

      waterPlant();

    }

    else
    {

      Serial.println("Manual Override Locked");

    }

  }

}

// SETUP

void setup()
{

  Serial.begin(115200);

  lcd.init();

  lcd.backlight();

  pinMode(PUMP_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(PUMP_PIN, LOW);

  connectWiFi();

  lcd.clear();

  lcd.setCursor(0,0);

  lcd.print("Plant Watering");

  lcd.setCursor(0,1);

  lcd.print("System Ready");

  delay(2000);

}

// LOOP

void loop()
{

  // Manual Override

  manualOverride();

  // Read every 30 seconds
  // (30 minutes on real hardware)

  if(millis() - previousRead >= READ_INTERVAL)
  {

    previousRead = millis();

    readSoil();

    readLDR();

    rainAPI = checkWeather();

    updateLCD();

    printStatus();

    logGoogleSheets();


    if(moistureState == "DRY")
    {

      if(!rainLDR && !rainAPI)
      {

        Serial.println("Conditions OK");

        Serial.println("Starting Pump");

        waterPlant();

      }

      else
      {

        Serial.println("Watering Skipped");

        Serial.println("Rain Detected");

      }

    }

    else if(moistureState == "OPTIMAL")
    {

      Serial.println("Soil Moisture Optimal");

    }

    else
    {

      Serial.println("Soil Already Wet");

    }

    Serial.println();

  }

}