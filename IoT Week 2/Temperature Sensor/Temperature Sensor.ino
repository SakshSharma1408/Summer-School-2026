#include<Servo.h>
int sensor = A0;
int motorPin = 3;   // PWM pin

void setup()
{
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);}

void loop()
{
  int reading = analogRead(sensor);

  float voltage = reading * (5.0 / 1023.0);
  float temp = (voltage - 0.5) * 100;

  Serial.print("Temp = ");
  Serial.println(temp);

  if(temp < 25)
  {
    analogWrite(motorPin, 0);      // Fan OFF
  }
  else if(temp < 35)
  {
    analogWrite(motorPin, 50);    // Medium speed
  }
  else
  {
    analogWrite(motorPin, 255);    // Full speed
  }
}