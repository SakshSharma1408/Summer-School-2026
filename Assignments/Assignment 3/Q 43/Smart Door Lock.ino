#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Servo.h>

/* -------------------- LCD Display -------------------- */
// If your LCD doesn't respond, try changing 0x27 to 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* -------------------- Servo Motor -------------------- */
Servo doorServo;
#define SERVO_PIN 18

/* -------------------- Keypad -------------------- */
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/* -------------------- Global Variables -------------------- */
String userID = "";
String otp = "";
String inputOTP = "";

bool otpSent = false;

unsigned long otpStartTime = 0;
const unsigned long otpTimeout = 30000;

/* -------------------- Setup -------------------- */
void setup() {

  Serial.begin(115200);

  // Initialize servo (door locked by default)
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Smart Door Lock");
  lcd.setCursor(0,1);
  lcd.print("System Start");

  delay(2000);
  lcd.clear();

  Serial.println("System Ready");
}

/* -------------------- Generate Random OTP -------------------- */
String generateOTP() {

  String code = "";

  for (int i = 0; i < 6; i++) {
    code += String(random(0, 9));
  }

  return code;
}

/* -------------------- Main Program -------------------- */
void loop() {

  char key = keypad.getKey();

  /* ----- Step 1 : User ID Entry ----- */
  if (!otpSent) {

    lcd.setCursor(0,0);
    lcd.print("Enter ID (4 digit)");

    if (key) {

      if (key >= '0' && key <= '9') {

        userID += key;

        lcd.setCursor(userID.length() - 1, 1);
        lcd.print("*");

        Serial.print("*");
      }

      if (userID.length() == 4) {

        Serial.println("\nID Entered: " + userID);

        otp = generateOTP();

        Serial.println("OTP GENERATED: " + otp);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("OTP Sent!");
        lcd.setCursor(0,1);
        lcd.print("Check Serial");

        otpSent = true;
        otpStartTime = millis();

        Serial.println("Enter OTP:");
      }
    }
  }

  /* ----- Step 2 : OTP Verification ----- */
  else {

    if (millis() - otpStartTime > otpTimeout) {

      Serial.println("OTP EXPIRED!");

      lcd.clear();
      lcd.print("OTP Expired");

      resetSystem(false);
      return;
    }

    lcd.setCursor(0,0);
    lcd.print("Enter OTP:");

    if (key) {

      if (key >= '0' && key <= '9') {

        inputOTP += key;

        lcd.setCursor(inputOTP.length() - 1, 1);
        lcd.print("*");

        Serial.print("*");
      }

      if (inputOTP.length() == 6) {

        Serial.println("\nOTP ENTERED: " + inputOTP);

        if (inputOTP == otp) {

          Serial.println("ACCESS GRANTED");

          lcd.clear();
          lcd.print("ACCESS GRANTED");

          doorServo.write(90);
          delay(3000);
          doorServo.write(0);

          Serial.println("Door Unlocked");

          resetSystem(true);
        }
        else {

          Serial.println("ACCESS DENIED");

          lcd.clear();
          lcd.print("ACCESS DENIED");

          resetSystem(false);
        }
      }
    }
  }
}

/* -------------------- Reset System -------------------- */
void resetSystem(bool success) {

  Serial.println("----------------------");

  if (success)
    Serial.println("LOG: SUCCESS ENTRY");
  else
    Serial.println("LOG: FAILED ENTRY");

  userID = "";
  inputOTP = "";
  otp = "";
  otpSent = false;

  delay(1500);
  lcd.clear();
}