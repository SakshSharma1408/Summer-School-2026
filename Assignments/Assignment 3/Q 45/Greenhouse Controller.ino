#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ===== Wi-Fi Configuration =====
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ===== MQTT Configuration =====
const char* mqtt_server = "broker.hivemq.com";
WiFiClient espClient;
PubSubClient client(espClient);

// ===== Pin Definitions =====
#define DHTPIN 4
#define DHTTYPE DHT22
#define LDR_PIN 34

#define HEATER_PIN 25
#define FAN_PIN 26
#define LIGHT_PIN 27

// ===== Object Initialization =====
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== Target Environmental Conditions =====
float tempMin = 18, tempMax = 22;
float humMin = 60, humMax = 70;

float tempHyst = 1;
float humHyst = 5;

// ===== LDR Sampling Buffer =====
int samples[10];
int idx = 0;

// ===== LCD Display Settings =====
int mode = 0;
unsigned long lastLCD = 0;

// ===== Timing Variables =====
unsigned long lastPublish = 0;

// ===== Wi-Fi Connection Function =====
void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

// ===== MQTT Reconnection Function =====
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect("ESP32Greenhouse")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed rc=");
      Serial.print(client.state());
      delay(1000);
    }
  }
}

// ===== LDR Average Reading Function =====
int getLDR() {
  samples[idx] = analogRead(LDR_PIN);
  idx = (idx + 1) % 10;

  long sum = 0;
  for (int i = 0; i < 10; i++) sum += samples[i];

  return sum / 10;
}

// ===== Greenhouse Control Logic =====
void control(float t, float h, int l) {

  if (t < tempMin - tempHyst) {
    digitalWrite(HEATER_PIN, HIGH);
    digitalWrite(FAN_PIN, LOW);
  }
  else if (t > tempMax + tempHyst) {
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(HEATER_PIN, LOW);
  }

  if (h > humMax + humHyst) {
    digitalWrite(FAN_PIN, HIGH);
  }

  if (l < 1500)
    digitalWrite(LIGHT_PIN, HIGH);
  else
    digitalWrite(LIGHT_PIN, LOW);
}

// ===== Data Publishing (MQTT & Serial Monitor) =====
void sendData(float t, float h, int l) {

  // ----- Serial Monitor Output -----
  Serial.println("------ GREENHOUSE DATA ------");
  Serial.print("Temperature: "); Serial.print(t); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(h); Serial.println(" %");
  Serial.print("Light Avg: "); Serial.println(l);

  Serial.print("Heater: "); Serial.println(digitalRead(HEATER_PIN));
  Serial.print("Fan: "); Serial.println(digitalRead(FAN_PIN));
  Serial.print("Light: "); Serial.println(digitalRead(LIGHT_PIN));
  Serial.println("-----------------------------\n");

  // ----- MQTT Data Publishing -----
  client.publish("greenhouse/temp", String(t).c_str());
  client.publish("greenhouse/humidity", String(h).c_str());
  client.publish("greenhouse/light", String(l).c_str());
  client.publish("greenhouse/heater", String(digitalRead(HEATER_PIN)).c_str());
  client.publish("greenhouse/fan", String(digitalRead(FAN_PIN)).c_str());
  client.publish("greenhouse/lightStatus", String(digitalRead(LIGHT_PIN)).c_str());
}

// ===== LCD Display Function =====
void display(float t, float h, int l) {

  if (millis() - lastLCD > 3000) {
    mode = (mode + 1) % 3;
    lastLCD = millis();
    lcd.clear();
  }

  if (mode == 0) {
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.print(t);
    lcd.setCursor(0,1);
    lcd.print("Hum:");
    lcd.print(h);
  }
  else if (mode == 1) {
    lcd.setCursor(0,0);
    lcd.print("Light:");
    lcd.setCursor(0,1);
    lcd.print(l);
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("H:");
    lcd.print(digitalRead(HEATER_PIN));
    lcd.print(" F:");
    lcd.print(digitalRead(FAN_PIN));
    lcd.setCursor(0,1);
    lcd.print("L:");
    lcd.print(digitalRead(LIGHT_PIN));
  }
}

// ===== System Setup =====
void setup() {

  Serial.begin(115200);

  pinMode(HEATER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);

  dht.begin();
  lcd.init();
  lcd.backlight();

  setup_wifi();
  client.setServer(mqtt_server, 1883);

  for (int i = 0; i < 10; i++) samples[i] = 0;
}

// ===== Main Program Loop =====
void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) return;

  int light = getLDR();

  control(temp, hum, light);
  display(temp, hum, light);

  // ----- Publish Data Every 3 Seconds -----
  if (millis() - lastPublish > 3000) {
    lastPublish = millis();
    sendData(temp, hum, light);
  }

  delay(200);
}