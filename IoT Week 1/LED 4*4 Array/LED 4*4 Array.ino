#include<Servo.h>
int seg[7] = {1,2,3,4,5,6,7};

// a b c d e f g
int dice[6][7] = {
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}  // 6
};

void setup() {
  for(int i=0;i<7;i++) {
    pinMode(seg[i], OUTPUT);
  }
}

void loop() {

  int num = random(0,6);
  for(int i=0;i<7;i++) {
    digitalWrite(seg[i], dice[num][i]);
  }

  delay(1000);
}