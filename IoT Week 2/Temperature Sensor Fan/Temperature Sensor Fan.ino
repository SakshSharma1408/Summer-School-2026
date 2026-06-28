#include<Servo.h>
Servo door;

int irSensor = 2;

void setup()
{
  door.attach(9);
  pinMode(irSensor, INPUT);
  door.write(0);
}

void loop()
{
  if(digitalRead(irSensor) == LOW)   // object detected
  {
    door.write(90);                  // keep open
  }
  else
  {
    door.write(0);                   // close
  }
}