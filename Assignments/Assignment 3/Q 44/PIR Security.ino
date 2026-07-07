#include <WiFi.h>

/* -------------------- Pin Configuration -------------------- */
#define PIR_PIN 27
#define BUZZER_PIN 25

#define LED_GREEN 18
#define LED_YELLOW 19
#define LED_RED 21

// Potentiometer used to adjust alert sensitivity
#define POT_PIN 34

/* -------------------- Monitoring Time -------------------- */
int startHour = 0;
int endHour = 24;
bool windowSet = false;

// Simulated current hour
int currentHour = 12;

/* -------------------- Global Variables -------------------- */
unsigned long lastTrigger = 0;

int alertLevel = 1;

/* -------------------- Buzzer Function -------------------- */
void beep(int freq, int duration)
{
  ledcWriteTone(BUZZER_PIN, freq);
  delay(duration);
  ledcWriteTone(BUZZER_PIN, 0);
}

/* -------------------- Read Serial Command -------------------- */
void readCommand()
{
  if (Serial.available())
  {
    String cmd = Serial.readStringUntil('\n');

    if (cmd.startsWith("SET_HOURS"))
    {
      sscanf(cmd.c_str(),
             "SET_HOURS %d %d",
             &startHour,
             &endHour);

      windowSet = true;

      Serial.print("Monitoring Hours: ");
      Serial.print(startHour);
      Serial.print(" -> ");
      Serial.println(endHour);
    }
  }
}

/* -------------------- Check Active Time -------------------- */
bool isActiveTime()
{
  if (!windowSet)
    return true;

  if (startHour < endHour)
  {
    return (currentHour >= startHour &&
            currentHour < endHour);
  }

  return (currentHour >= startHour ||
          currentHour < endHour);
}

/* -------------------- Alert Handler -------------------- */
void alertSystem(int level)
{
  Serial.print("[");
  Serial.print(millis() / 1000);
  Serial.print(" sec] ");

  if (level == 1)
  {
    Serial.println("WARNING");

    digitalWrite(LED_GREEN, HIGH);

    beep(1000, 200);

    digitalWrite(LED_GREEN, LOW);
  }
  else if (level == 2)
  {
    Serial.println("ALARM");

    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, HIGH);

    beep(1600, 350);

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
  }
  else
  {
    Serial.println("URGENT");

    for (int i = 0; i < 4; i++)
    {
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, HIGH);

      beep(2500, 150);

      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, LOW);

      delay(100);
    }
  }
}

/* -------------------- Setup -------------------- */
void setup()
{
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(POT_PIN, INPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Configure buzzer using PWM
  ledcAttach(BUZZER_PIN, 2000, 8);

  Serial.println("==================================");
  Serial.println(" PIR Security Camera Trigger");
  Serial.println("==================================");
  Serial.println("Command Example:");
  Serial.println("SET_HOURS 22 06");
  Serial.println();
}

/* -------------------- Main Loop -------------------- */
void loop()
{
  // Check for commands from Serial Monitor
  readCommand();

  // Read potentiometer value
  int potValue = analogRead(POT_PIN);

  if (potValue < 1365)
    alertLevel = 1;

  else if (potValue < 2730)
    alertLevel = 2;

  else
    alertLevel = 3;

  // Read PIR sensor status
  int pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH && isActiveTime())
  {
    if (millis() - lastTrigger > 3000)
    {
      lastTrigger = millis();

      Serial.println("--------------------------------");

      Serial.print("Timestamp : ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");

      Serial.println("Motion Detected");

      Serial.print("Sensitivity Value : ");
      Serial.println(potValue);

      Serial.print("Alert Level : ");

      switch (alertLevel)
      {
        case 1:
          Serial.println("WARNING");
          break;

        case 2:
          Serial.println("ALARM");
          break;

        case 3:
          Serial.println("URGENT");
          break;
      }

      alertSystem(alertLevel);

      Serial.println("Motion Event Logged");

      Serial.println("--------------------------------");
    }
  }

  // Small delay to reduce CPU usage
  delay(100);
}