#include <WiFi.h>
#include <HTTPClient.h>

/* -------------------- Wi-Fi Credentials -------------------- */
const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

/* -------------------- Telegram Bot Details -------------------- */
const char* BOT_TOKEN = "YOUR_BOT_TOKEN";
const char* CHAT_ID   = "YOUR_CHAT_ID";

/* -------------------- Connect to Wi-Fi -------------------- */
void connectWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Connected");
}

/* -------------------- Send Telegram Alert -------------------- */
void sendTelegramAlert(String level)
{
  if (WiFi.status() != WL_CONNECTED)
    return;

  HTTPClient http;

  String msg =
    "🚨 Motion Detected%0A"
    "Level: " + level +
    "%0ATime: " + String(millis() / 1000) + " sec";

  String url =
    "https://api.telegram.org/bot" +
    String(BOT_TOKEN) +
    "/sendMessage?chat_id=" +
    String(CHAT_ID) +
    "&text=" +
    msg;

  http.begin(url);

  int httpCode = http.GET();

  if (httpCode > 0)
  {
    Serial.println("Telegram alert sent.");
  }
  else
  {
    Serial.println("Failed to send Telegram alert.");
  }

  http.end();
}