const int ldrPin = A0;
const int pirPin = 2;
const int ledPin = 9;

unsigned long motionTime = 0;

void setup() {

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int light = analogRead(ldrPin);
  bool motion = digitalRead(pirPin);

  // Daytime
  if (light > 600) {

    analogWrite(ledPin, 0);

    Serial.println("DAY : Light OFF");
  }

  // Night mode
  else {

    if (motion) {

      analogWrite(ledPin, 255);

      motionTime = millis();

      Serial.println("Motion Detected : LED Full Brightness");
    }

    else {

      if (millis() - motionTime < 30000) {

        analogWrite(ledPin, 255);
      }

      else {

        analogWrite(ledPin, 51);

        Serial.println("No Motion : LED at 20%");
      }
    }
  }

  delay(500);
}